#include "stdafx.h"
#include "..\public\Animation.h"
#include "Channel.h"


CAnimation::CAnimation()
	:m_bIsLooping(true)
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

void CAnimation::Reset_Animation()
{
	for (auto& pChannel : m_vecChannels)
		pChannel->Reset_KeyFrameIndex();

	m_dPlayTime = 0.0;
	m_bIsFinished = false;

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

void CAnimation::Update_Bones(_double dTimeDelta, const wstring& wstrRootBoneName)
{
	if (m_bIsFinished)
	{
		if (m_bIsLooping)
			m_bIsFinished = false;
		else
			return;
	}

	m_dPlayTime += m_dTickPerSecond * dTimeDelta;

	if (m_dPlayTime >= m_dDuration)
	{
		m_bIsFinished = true;
	}

	for(_uint i =0; i < m_iNumChannels; ++i)
	{
		if (m_bIsFinished == true)
			m_vecChannels[i]->Reset_KeyFrameIndex();

		m_vecChannels[i]->Update_TransformMatrix(m_dPlayTime, wstrRootBoneName);
	}

	if (m_bIsFinished && m_bIsLooping)
		m_dPlayTime = 0.0;
}


void CAnimation::Update_Lerp(_double dTimeDelta, _float fRatio, const wstring& wstrRootBoneName)
{
	if (m_bIsFinished)
	{
		if (m_bIsLooping)
			m_bIsFinished = false;
		else
			return;
	}

	m_dPlayTime = m_dDuration * dTimeDelta;

	if (m_dPlayTime >= m_dDuration)
	{
		if (m_bIsLooping)
			m_dPlayTime = 0.0;

		m_bIsFinished = true;
	}

	for (_uint i = 0; i < m_iNumChannels; ++i)
	{
		if (m_bIsFinished == true)
			m_vecChannels[i]->Reset_KeyFrameIndex();

		m_vecChannels[i]->Update_TransformLerpMatrix(m_dPlayTime, fRatio, wstrRootBoneName);
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
	for (auto& pChannel : m_vecChannels)
		Safe_Release(pChannel);

	m_vecChannels.clear();
}

