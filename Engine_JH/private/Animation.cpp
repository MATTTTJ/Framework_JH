#include "..\public\Animation.h"
#include "Channel.h"


CAnimation::CAnimation()
	: m_bIsLooping(true)
{
}

HRESULT CAnimation::Initialize(aiAnimation* pAIAnimation, CModel* pModel)
{
	m_strName = pAIAnimation->mName.data;
	m_Duration = pAIAnimation->mDuration;
	m_dTIckPerSecond = pAIAnimation->mTicksPerSecond;
	m_iNumChannels = pAIAnimation->mNumChannels;

	for(_uint i =0; i< m_iNumChannels; ++i)
	{
		//aiNodeAnim은 특정 애니메이션의 키프레임이 갖고있는 뼈들의 데이터들의 구조체다.
		aiNodeAnim*		pAINodeAnim = pAIAnimation->mChannels[i];
		CChannel*		pChannel = CChannel::Create(pAINodeAnim, pModel);
		NULL_CHECK_RETURN(pChannel, E_FAIL)

		m_vecChannels.push_back(pChannel);
	}


	return S_OK;
}

void CAnimation::Update_Bones(_double TimeDelta)
{
	if (!m_bIsLooping && m_bIsFinished) return;

	m_dPlayTime += m_dTIckPerSecond * TimeDelta;

	if(m_dPlayTime >= m_Duration)
	{
		m_dPlayTime = 0.0;
		m_bIsFinished = true;
	}

	for(_uint i =0; i < m_iNumChannels; ++i)
	{
		if (true == m_bIsFinished)
			m_vecChannels[i]->Reset_KeyFrameIndex();

		// 애니메이션을 재생할 때 필요한 특정 뼈들을 순회하면서 상태 행렬을 업데이트해준다. 
		m_vecChannels[i]->Update_TransformMatrix(m_dPlayTime);
	}

	if (m_bIsFinished)
		m_bIsFinished = false;
}

CAnimation* CAnimation::Create(aiAnimation* pAIAnimation, CModel* pModel)
{
	CAnimation*		pInstance = new CAnimation();

	if (FAILED(pInstance->Initialize(pAIAnimation, pModel)))
	{
		MSG_BOX("Failed to Created : CAnimation");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CAnimation::Free()
{
	for (auto& pChannel : m_vecChannels)
		Safe_Release(pChannel);

	m_vecChannels.clear();
}

