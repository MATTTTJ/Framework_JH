#include "..\public\Channel.h"

#include "Bone.h"
#include "Model.h"


CChannel::CChannel()
{
}

// Ư�� �ִϸ��̼ǿ��� ���Ǵ� ������ ������
HRESULT CChannel::Initialize(aiNodeAnim* pAIChannel, CModel* pModel)
{
	strcpy_s(m_szName, pAIChannel->mNodeName.data);

	m_pBone = pModel->Get_BonePtr(m_szName); // �𵨿��� �����ϴ� ���� ä�ο��� �ҷ��� ���� �̸��� ���� ��� �ּҸ� �Ѱ��ش�.
	Safe_AddRef(m_pBone);
	// ���� �������� Ű�������� ���� ���Ƽ�, �� ������ ���� ã�� ���� �ԾƳ��� �۾�
	m_iNumKeyframes = max(pAIChannel->mNumRotationKeys, pAIChannel->mNumScalingKeys);
	m_iNumKeyframes = max(pAIChannel->mNumPositionKeys, m_iNumKeyframes);
	

	XMFLOAT3		vScale;
	XMFLOAT4		vRotation;
	XMFLOAT3		vPosition;

	for(_uint i =0 ; i< m_iNumKeyframes; ++i)
	{
		KEYFRAME	KeyFrame;
		ZeroMemory(&KeyFrame, sizeof(KeyFrame));

		if (i < pAIChannel->mNumScalingKeys)
		{
			memcpy(&vScale, &pAIChannel->mScalingKeys[i].mValue, sizeof(XMFLOAT3));
			KeyFrame.Time = pAIChannel->mScalingKeys[i].mTime;
		}

		if (i < pAIChannel->mNumRotationKeys)
		{
			vRotation.x = pAIChannel->mRotationKeys[i].mValue.x;
			vRotation.y = pAIChannel->mRotationKeys[i].mValue.y;
			vRotation.z = pAIChannel->mRotationKeys[i].mValue.z;
			vRotation.w = pAIChannel->mRotationKeys[i].mValue.w;
			KeyFrame.Time = pAIChannel->mRotationKeys[i].mTime;
		}

		if(i< pAIChannel->mNumPositionKeys)
		{
			memcpy(&vPosition, &pAIChannel->mPositionKeys->mValue, sizeof(XMFLOAT3));
			KeyFrame.Time = pAIChannel->mPositionKeys->mTime;
		}
		KeyFrame.vScale = vScale;
		KeyFrame.vRotation = vRotation;
		KeyFrame.vPosition = vPosition;

		m_Keyframes.push_back(KeyFrame);
	}
	return S_OK;
}

void CChannel::Update_TransformMatrix(_double PlayTime)
{
	// �ִϸ��̼��� ����ϴ� Ư�� ������ ���� ����� �������ִ� �Լ�

	_vector vScale;
	_vector vRotation;
	_vector vPosition;

	_matrix TransformMatrix;

	// ���� ����� �ð��� �� �ڿ��ִ� Ű �������� �ð����� Ŀ���� (�ԾƳ� Ű ������)
	// ������ Ű������ �����͸� ���ܵα����� �۾�
	if(PlayTime >= m_Keyframes.back().Time)
	{
		vScale = XMLoadFloat3(&m_Keyframes.back().vScale);
		vRotation = XMLoadFloat4(&m_Keyframes.back().vRotation);
		vPosition = XMLoadFloat3(&m_Keyframes.back().vPosition);
		vPosition = XMVectorSetW(vPosition, 1.f);
	}
	else
	{
		if(PlayTime >= m_Keyframes[m_iCurrentKeyframeIndex + 1].Time)
		{
			// �ִϸ��̼� ����ð��� Ű������ ���̿� ������ ��
			// �÷���Ÿ�� �ð��� ���� Ű������ �ð����� Ŭ �� ���� Ű������ �ε����� ����
			++m_iCurrentKeyframeIndex;
		}
		// ���� �ð� - ���� Ű������ �ε��� �ð� / ���� Ű������ �ε��� �ð� - ���� �ð� 
		_double		Ratio = (PlayTime - m_Keyframes[m_iCurrentKeyframeIndex].Time) /
			m_Keyframes[m_iCurrentKeyframeIndex + 1].Time - m_Keyframes[m_iCurrentKeyframeIndex].Time;
		// �� ������ Ű ������ ������ ������ ���� ���� �������ֱ� ���� �����̴�.

		_vector		vSourScale, vDestScale;
		_vector		vSourRotation, vDestRotation;
		_vector		vSourPosition, vDestPosition;

		vSourScale = XMLoadFloat3(&m_Keyframes[m_iCurrentKeyframeIndex].vScale);
		vSourRotation = XMLoadFloat4(&m_Keyframes[m_iCurrentKeyframeIndex].vRotation);
		vSourPosition = XMLoadFloat3(&m_Keyframes[m_iCurrentKeyframeIndex].vPosition);

		vDestScale = XMLoadFloat3(&m_Keyframes[m_iCurrentKeyframeIndex + 1].vScale);
		vDestRotation = XMLoadFloat4(&m_Keyframes[m_iCurrentKeyframeIndex + 1].vRotation);
		vDestPosition = XMLoadFloat3(&m_Keyframes[m_iCurrentKeyframeIndex + 1].vPosition);

		vScale = XMVectorLerp(vSourScale, vDestScale, Ratio);
		vRotation = XMQuaternionSlerp(vSourRotation, vDestRotation, Ratio);
		vPosition = XMVectorLerp(vSourPosition, vDestPosition, Ratio);
	}
	// ���͵��� �޾Ƽ� ��ķ� ������ִ� �Լ�. 2��° ���ڴ� �������ε� ������ ����.
	TransformMatrix = XMMatrixAffineTransformation(vScale, XMVectorSet(0.f, 0.f, 0.f, 1.f), vRotation, vPosition);

	m_pBone->Set_TransformMatrix(TransformMatrix);
}

CChannel* CChannel::Create(aiNodeAnim* pAIChannel, CModel* pModel)
{
	CChannel*		pInstance = new CChannel();

	if (FAILED(pInstance->Initialize(pAIChannel, pModel)))
	{
		MSG_BOX("Failed to Created : CChannel");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CChannel::Free()
{
	Safe_Release(m_pBone);

	m_Keyframes.clear();
}
