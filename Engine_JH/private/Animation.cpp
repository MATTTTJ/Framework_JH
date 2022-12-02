#include "..\public\Animation.h"
#include "Channel.h"


CAnimation::CAnimation()
{
}

HRESULT CAnimation::Initialize(aiAnimation* pAIAnimation, CModel* pModel)
{
	strcpy_s(m_szName, pAIAnimation->mName.data); // 애니메이션 이름 따기

	m_Duration = pAIAnimation->mDuration;

	m_TIckPerSecond = pAIAnimation->mTicksPerSecond;

	m_iNumChannels = pAIAnimation->mNumChannels;

	for(_uint i =0; i< m_iNumChannels; ++i)
	{
		//aiNodeAnim은 특정 애니메이션의 키프레임이 갖고있는 뼈들의 데이터들의 구조체다.
		aiNodeAnim*		pAINodeAnim = pAIAnimation->mChannels[i];
		//데이터 관리할게 많아서 클래스 만들어서 빼둠
		CChannel*		pChannel = CChannel::Create(pAINodeAnim, pModel);
		NULL_CHECK_RETURN(pChannel, E_FAIL)

		m_Channels.push_back(pChannel);
	}


	return S_OK;
}

void CAnimation::Update_Bones(_double TimeDelta)
{
	m_PlayTime += m_TIckPerSecond * TimeDelta;

	for(_uint i =0; i<m_iNumChannels; ++i)
	{
		// 애니메이션을 재생할 때 필요한 특정 뼈들을 순회하면서 상태 행렬을 업데이트해준다. 
		m_Channels[i]->Update_TransformMatrix(m_PlayTime);
	}
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
	for (auto& pChannel : m_Channels)
		Safe_Release(pChannel);

	m_Channels.clear();
}

