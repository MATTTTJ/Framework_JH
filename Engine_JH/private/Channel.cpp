#include "..\public\Channel.h"

#include "Bone.h"
#include "Model.h"


CChannel::CChannel()
{
}

// 특정 애니메이션에서 사용되는 뼈들을 관리함
HRESULT CChannel::Initialize(aiNodeAnim* pAIChannel, CModel* pModel)
{
	strcpy_s(m_szName, pAIChannel->mNodeName.data);

	m_pBone = pModel->Get_BonePtr(m_szName); // 모델에서 보관하는 뼈와 채널에서 불러온 뼈의 이름이 같은 경우 주소를 넘겨준다.
	Safe_AddRef(m_pBone);
	// 보통 포지션이 키프레임이 제일 많아서, 끝 프레임 수를 찾기 위해 솎아내는 작업
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
	// 애니메이션을 재생하는 특정 뼈들의 상태 행렬을 연산해주는 함수

	_vector vScale;
	_vector vRotation;
	_vector vPosition;

	_matrix TransformMatrix;

	// 현재 재생된 시간이 맨 뒤에있는 키 프레임의 시간보다 커지면 (솎아낸 키 프레임)
	// 마지막 키프레임 데이터를 남겨두기위한 작업
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
			// 애니메이션 재생시간이 키프레임 사이에 존재할 때
			// 플레이타임 시간이 다음 키프레임 시간보다 클 때 현재 키프레임 인덱스를 증가
			++m_iCurrentKeyframeIndex;
		}
		// 현재 시간 - 현재 키프레임 인덱스 시간 / 현재 키프레임 인덱스 시간 - 현재 시간 
		_double		Ratio = (PlayTime - m_Keyframes[m_iCurrentKeyframeIndex].Time) /
			m_Keyframes[m_iCurrentKeyframeIndex + 1].Time - m_Keyframes[m_iCurrentKeyframeIndex].Time;
		// 위 비율은 키 프레임 사이의 데이터 변경 값을 보간해주기 위한 비율이다.

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
	// 백터들을 받아서 행렬로 만들어주는 함수. 2번째 인자는 기준점인데 원점이 들어간다.
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
