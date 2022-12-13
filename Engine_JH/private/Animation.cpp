#include "..\public\Animation.h"
#include "Channel.h"


CAnimation::CAnimation()
	: m_bIsLooping(true)
{
}

HRESULT CAnimation::Save_Animation(HANDLE& hFile, DWORD& dwByte)
{
	_uint			iNameLength = (_uint)m_strName.length() + 1;
	const char*	pName = m_strName.c_str();
	WriteFile(hFile, &iNameLength, sizeof(_uint), &dwByte, nullptr);
	WriteFile(hFile, pName, sizeof(char) * iNameLength, &dwByte, nullptr);

	WriteFile(hFile, &m_dDuration, sizeof(_double), &dwByte, nullptr);
	WriteFile(hFile, &m_dTickPerSecond, sizeof(_double), &dwByte, nullptr);
	WriteFile(hFile, &m_bIsLooping, sizeof(_bool), &dwByte, nullptr);

	WriteFile(hFile, &m_iNumChannels, sizeof(_uint), &dwByte, nullptr);
	for (auto& pChannel : m_vecChannels)
	{
		if (FAILED(pChannel->Save_Channel(hFile, dwByte)))
		{
			MSG_BOX("Failed to Save : Channel");
			return E_FAIL;
		}
	}

	return S_OK;
}

HRESULT CAnimation::Load_Animation(HANDLE& hFile, DWORD& dwByte)
{
	_uint			iNameLength = 0;
	ReadFile(hFile, &iNameLength, sizeof(_uint), &dwByte, nullptr);

	char*			pName = new char[iNameLength];
	ReadFile(hFile, pName, sizeof(char) * iNameLength, &dwByte, nullptr);

	m_strName = pName;

	Safe_Delete_Array(pName);

	ReadFile(hFile, &m_dDuration, sizeof(_double), &dwByte, nullptr);
	ReadFile(hFile, &m_dTickPerSecond, sizeof(_double), &dwByte, nullptr);
	ReadFile(hFile, &m_bIsLooping, sizeof(_bool), &dwByte, nullptr);

	ReadFile(hFile, &m_iNumChannels, sizeof(_uint), &dwByte, nullptr);
	m_vecChannels.reserve(m_iNumChannels);

	for (_uint i = 0; i < m_iNumChannels; ++i)
	{
		CChannel*		pChannel = CChannel::Create(nullptr, m_pModel);
		NULL_CHECK_RETURN(pChannel, E_FAIL);
		FAILED_CHECK_RETURN(pChannel->Load_Channel(hFile, dwByte), E_FAIL);

		m_vecChannels.push_back(pChannel);
	}

	return S_OK;
}

HRESULT CAnimation::Initialize(aiAnimation* pAIAnimation, CModel* pModel)
{
	m_pModel = pModel;

	if (pAIAnimation == nullptr)
		return S_OK;

	m_strName = pAIAnimation->mName.data;
	m_dDuration = pAIAnimation->mDuration;
	m_dTickPerSecond = pAIAnimation->mTicksPerSecond;
	m_iNumChannels = pAIAnimation->mNumChannels;

	for (_uint i = 0; i < m_iNumChannels; ++i)
	{
		//aiNodeAnim은 특정 애니메이션의 키프레임이 갖고있는 뼈들의 데이터들의 구조체다.
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

	m_dPlayTime += m_dTickPerSecond * dTimeDelta;

	if (m_dPlayTime >= m_dDuration)
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
	// 	// 애니메이션을 재생할 때 필요한 특정 뼈들을 순회하면서 상태 행렬을 업데이트해준다. 
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
			// 특정 뼈의 이동을 멈추고 싶으면 여기서 위치 고정
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

