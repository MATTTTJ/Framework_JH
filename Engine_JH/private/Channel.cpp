#include "..\public\Channel.h"

#include "Bone.h"
#include "Model.h"


CChannel::CChannel()
{
}

// Ư�� �ִϸ��̼ǿ��� ���Ǵ� ������ ������
HRESULT CChannel::Initialize(aiNodeAnim* pAIChannel, CModel* pModel)
{
	m_strName = pAIChannel->mNodeName.data;

	m_pBone = pModel->Get_BonePtr(m_strName); // �𵨿��� �����ϴ� ���� ä�ο��� �ҷ��� ���� �̸��� ���� ��� �ּҸ� �Ѱ��ش�.
	Safe_AddRef(m_pBone);
	// ���� �������� Ű�������� ���� ���Ƽ�, �� ������ ���� ã�� ���� �ԾƳ��� �۾�
	m_iNumKeyframes = max(pAIChannel->mNumScalingKeys, pAIChannel->mNumRotationKeys);
	m_iNumKeyframes = max(m_iNumKeyframes, pAIChannel->mNumPositionKeys);

	m_vecKeyframes.reserve(m_iNumKeyframes);

	_float3		vScale;
	_float4		vRotation;
	_float3		vPosition;

	for(_uint i =0 ; i< m_iNumKeyframes; ++i)
	{
		KEYFRAME	KeyFrame;
		ZeroMemory(&KeyFrame, sizeof(KEYFRAME));

		if (i < pAIChannel->mNumScalingKeys)
		{
			memcpy(&vScale, &pAIChannel->mScalingKeys[i].mValue, sizeof(_float3));
			KeyFrame.dTime = pAIChannel->mScalingKeys[i].mTime;
		}

		if (i < pAIChannel->mNumRotationKeys)
		{
			vRotation.x = pAIChannel->mRotationKeys[i].mValue.x;
			vRotation.y = pAIChannel->mRotationKeys[i].mValue.y;
			vRotation.z = pAIChannel->mRotationKeys[i].mValue.z;
			vRotation.w = pAIChannel->mRotationKeys[i].mValue.w;
			KeyFrame.dTime = pAIChannel->mRotationKeys[i].mTime;
		}

		if(i< pAIChannel->mNumPositionKeys)
		{
			memcpy(&vPosition, &pAIChannel->mPositionKeys[i].mValue, sizeof(_float3));
			KeyFrame.dTime = pAIChannel->mPositionKeys[i].mTime;
		}
		KeyFrame.vScale = vScale;
		KeyFrame.vRotation = vRotation;
		KeyFrame.vPosition = vPosition;

		m_vecKeyframes.push_back(KeyFrame);
	}
	return S_OK;
}

void CChannel::Update_TransformMatrix(_double dPlayTime)
{
	// �ִϸ��̼��� ����ϴ� Ư�� ������ ���� ����� �������ִ� �Լ�

	_vector vScale;
	_vector vRotation;
	_vector vPosition;
	_matrix TransformMatrix;

	// ���� ����� �ð��� �� �ڿ��ִ� Ű �������� �ð����� Ŀ���� (�ԾƳ� Ű ������)
	// ������ Ű������ �����͸� ���ܵα����� �۾�
	if(dPlayTime >= m_vecKeyframes.back().dTime)
	{
		m_vLastScale = vScale =	XMLoadFloat3(&m_vecKeyframes.back().vScale);
		m_vLastRotation = vRotation = XMLoadFloat4(&m_vecKeyframes.back().vRotation);
		m_vLastPosition = vPosition = XMLoadFloat3(&m_vecKeyframes.back().vPosition);
		m_vLastPosition = vPosition = XMVectorSetW(vPosition, 1.f);
	}
	else
	{
		while (dPlayTime >= m_vecKeyframes[m_iCurrentKeyframeIndex + 1].dTime)
		{
			// �ִϸ��̼� ����ð��� Ű������ ���̿� ������ ��
			// �÷���Ÿ�� �ð��� ���� Ű������ �ð����� Ŭ �� ���� Ű������ �ε����� ����
			++m_iCurrentKeyframeIndex;
		}
		// ���� �ð� - ���� Ű������ �ε��� �ð� / ���� Ű������ �ε��� �ð� - ���� �ð� 
		_double	dRatio = (dPlayTime - m_vecKeyframes[m_iCurrentKeyframeIndex].dTime)
			/ (m_vecKeyframes[m_iCurrentKeyframeIndex + 1].dTime - m_vecKeyframes[m_iCurrentKeyframeIndex].dTime);
		// �� ������ Ű ������ ������ ������ ���� ���� �������ֱ� ���� �����̴�.
		
		vScale = XMVectorLerp(XMLoadFloat3(&m_vecKeyframes[m_iCurrentKeyframeIndex].vScale), XMLoadFloat3(&m_vecKeyframes[m_iCurrentKeyframeIndex + 1].vScale), (_float)dRatio);
		vRotation = XMQuaternionSlerp(XMLoadFloat4(&m_vecKeyframes[m_iCurrentKeyframeIndex].vRotation), XMLoadFloat4(&m_vecKeyframes[m_iCurrentKeyframeIndex + 1].vRotation), (_float)dRatio);
		vPosition = XMVectorLerp(XMLoadFloat3(&m_vecKeyframes[m_iCurrentKeyframeIndex].vPosition), XMLoadFloat3(&m_vecKeyframes[m_iCurrentKeyframeIndex + 1].vPosition), (_float)dRatio);
		vPosition = XMVectorSetW(vPosition, 1.f);
	}
	// ���͵��� �޾Ƽ� ��ķ� ������ִ� �Լ�. 2��° ���ڴ� �������ε� ������ ����.
	TransformMatrix = XMMatrixAffineTransformation(vScale, XMVectorSet(0.f, 0.f, 0.f, 1.f), vRotation, vPosition);

	m_pBone->Set_TransformMatrix(TransformMatrix);
}

_bool CChannel::Update_TransformLerpMatrix(_double dPlayTime, CChannel* LastChannel, CChannel* CurChannel, _bool bFinish)
{
	_vector vScale;
	_vector vRotation;
	_vector vPosition;
	_matrix TransformMatrix;

	if(-1 == m_iLerpFrameIndex) // ���� ���� �� ���� �ε��� ���� ������ �ʱ�ȭ 
	{
		while(dPlayTime >= m_vecKeyframes[m_iLerpFrameIndex + 1].dTime)
		{
			m_iLerpFrameIndex++;

			if((_int)m_iNumKeyframes <= m_iLerpFrameIndex +1)
			{
				m_iLerpFrameIndex -= 1;
				break;
			}
		}
	}

	_uint iSour = 0;

	if(bFinish)
	{
		iSour = m_iNumKeyframes - 1;
	}
	else
	{
		iSour = m_iLerpFrameIndex;
	}

	_uint iDest = 0;

	if (LastChannel->m_vecKeyframes.size() <= iSour)
		iSour = (_uint)LastChannel->m_vecKeyframes.size() - 1;

	m_dLerpRatio += 0.1;

	_vector	vLastScale, vCurScale;
	_vector vLastRotation, vCurRotation;
	_vector vLastPosition, vCurPosition;

	vLastScale = XMLoadFloat3(&LastChannel->m_vecKeyframes[iSour].vScale);
	vCurScale = XMLoadFloat3(&CurChannel->m_vecKeyframes[iDest].vScale);

	vLastRotation = XMLoadFloat4(&LastChannel->m_vecKeyframes[iSour].vRotation);
	vCurRotation = XMLoadFloat4(&CurChannel->m_vecKeyframes[iDest].vRotation);

	vLastPosition = XMLoadFloat3(&LastChannel->m_vecKeyframes[iSour].vPosition);
	vCurPosition = XMLoadFloat3(&CurChannel->m_vecKeyframes[iDest].vPosition);

	vScale = XMVectorLerp(vLastScale, vCurScale, static_cast<_float>(m_dLerpRatio));
	vRotation = XMQuaternionSlerp(vLastRotation, vCurRotation, static_cast<_float>(m_dLerpRatio));
	vPosition = XMVectorLerp(vLastPosition, vCurPosition, static_cast<_float>(m_dLerpRatio));
	vPosition = XMVectorSetW(vPosition, 1.f);

	TransformMatrix = XMMatrixAffineTransformation(vScale, XMVectorSet(0.f, 0.f, 0.f, 1.f), vRotation, vPosition);

	m_pBone->Set_TransformMatrix(TransformMatrix);

	if (1.f <= m_dLerpRatio)
	{
		m_dLerpRatio = 0.0;
		return true;
	}

	return false;
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

	m_vecKeyframes.clear();
}
