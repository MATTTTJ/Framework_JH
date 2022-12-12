#include "..\public\Animation.h"
#include "Channel.h"


CAnimation::CAnimation()
	: m_bIsLooping(true)
{
}

HRESULT CAnimation::Save_Animation(HANDLE& hFile, DWORD& dwByte)
{
	return S_OK;
}

HRESULT CAnimation::Load_Animation(HANDLE& hFile, DWORD& dwByte)
{
	return S_OK;
}

HRESULT CAnimation::Initialize(aiAnimation* pAIAnimation, CModel* pModel)
{
	m_strName = pAIAnimation->mName.data;
	m_Duration = pAIAnimation->mDuration;
	m_dTIckPerSecond = pAIAnimation->mTicksPerSecond;
	m_iNumChannels = pAIAnimation->mNumChannels;

	for (_uint i = 0; i < m_iNumChannels; ++i)
	{
		//aiNodeAnim�� Ư�� �ִϸ��̼��� Ű�������� �����ִ� ������ �����͵��� ����ü��.
		aiNodeAnim*		pAINodeAnim = pAIAnimation->mChannels[i];
		CChannel*		pChannel = CChannel::Create(pAINodeAnim, pModel);
		NULL_CHECK_RETURN(pChannel, E_FAIL)

			m_vecChannels.push_back(pChannel);
	}


	return S_OK;
}

_bool CAnimation::Update_Bones(_double dTimeDelta)
{
	if (false == m_bIsLooping && true == m_bIsFinished)
	{
		return false;;
	}

	m_dPlayTime += m_dTIckPerSecond * dTimeDelta;

	if (m_dPlayTime >= m_Duration)
	{
		m_dPlayTime = 0.0;
		m_bIsFinished = true;
		return true;
	}

	if (m_bIsLooping && m_bIsFinished)
	{
		for (auto iter : m_vecChannels)
			iter->Reset_KeyFrameIndex();
	}

	for (auto iter : m_vecChannels)
	{
		iter->Update_TransformMatrix(m_dPlayTime);
	}

	if (true == m_bIsFinished)
	{
		m_bIsFinished = false;
	}

	// for(_uint i =0; i < m_iNumChannels; ++i)
	// {
	// 	if (true == m_bIsFinished)
	// 		m_vecChannels[i]->Reset_KeyFrameIndex();
	//
	// 	// �ִϸ��̼��� ����� �� �ʿ��� Ư�� ������ ��ȸ�ϸ鼭 ���� ����� ������Ʈ���ش�. 
	// 	m_vecChannels[i]->Update_TransformMatrix(m_dPlayTime);
	// }

	return false;
}

_bool CAnimation::Update_Lerp(_double dTimeDelta, CAnimation* pBefore, _bool bFinish)
{
	if (m_bIsLerpEnd)
	{
		m_dPlayTime = 0.f;

		for (auto iter : m_vecChannels)
		{
			iter->Reset_KeyFrameIndex();
			iter->Reset_LerpIndex();
		}
		m_bIsLerpEnd = false;

		return false;
	}
	else
	{
		for (auto iter : m_vecChannels)
		{
			for (auto Before : pBefore->m_vecChannels)
			{
				if (iter->Get_ChannelName() == Before->Get_ChannelName())
				{
					m_bIsLerpEnd = iter->Update_TransformLerpMatrix(m_dPlayTime, Before, iter, bFinish);
					break;
				}
			}
			// Ư�� ���� �̵��� ���߰� ������ ���⼭ ��ġ ����
		}
		return true;
	}
	return true;
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

