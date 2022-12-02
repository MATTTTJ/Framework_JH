#include "..\public\Animation.h"
#include "Channel.h"


CAnimation::CAnimation()
{
}

HRESULT CAnimation::Initialize(aiAnimation* pAIAnimation, CModel* pModel)
{
	strcpy_s(m_szName, pAIAnimation->mName.data); // �ִϸ��̼� �̸� ����

	m_Duration = pAIAnimation->mDuration;

	m_TIckPerSecond = pAIAnimation->mTicksPerSecond;

	m_iNumChannels = pAIAnimation->mNumChannels;

	for(_uint i =0; i< m_iNumChannels; ++i)
	{
		//aiNodeAnim�� Ư�� �ִϸ��̼��� Ű�������� �����ִ� ������ �����͵��� ����ü��.
		aiNodeAnim*		pAINodeAnim = pAIAnimation->mChannels[i];
		//������ �����Ұ� ���Ƽ� Ŭ���� ���� ����
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
		// �ִϸ��̼��� ����� �� �ʿ��� Ư�� ������ ��ȸ�ϸ鼭 ���� ����� ������Ʈ���ش�. 
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

