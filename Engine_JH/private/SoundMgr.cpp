#include "stdafx.h"
#include "SoundMgr.h"

IMPLEMENT_SINGLETON(CSoundMgr)


CSoundMgr::CSoundMgr()
{
}

void CSoundMgr::Set_Volume(_uint iManualChannelIndex, _float fVolume)
{
	if (m_iNumManualChannels == 0 || iManualChannelIndex >= MAX_CHANNEL_COUNT)
		return;

	if (m_Channels[iManualChannelIndex].second == NO_USE_CHNNEL)
		return;

	m_Channels[iManualChannelIndex].second->Set_Volume(m_Channels[iManualChannelIndex].first, fVolume);
}

void CSoundMgr::Set_MasterVolume(_float fVolume)
{
	fVolume = fVolume > 1.f ? 1.f : fVolume < 0.f ? 0.f : fVolume;
	CSound::Set_MasterVolume(fVolume);
	for (_uint i = 0; i < m_iNumManualChannels; ++i)
	{
		if (m_Channels[i].second)
			m_Channels[i].second->UpdateUseVolume(m_Channels[i].first);
	}
}

void CSoundMgr::Set_SoundDesc(const wstring& wstrSoundKey, CSound::SOUND_DESC& SoundDesc)
{
	SOUNDS::iterator Pair = find_if(m_mapSound.begin(), m_mapSound.end(), CTag_Finder(wstrSoundKey));
	if (Pair == m_mapSound.end())
		return;

	Pair->second->Set_SoundDesc(SoundDesc);
}

HRESULT CSoundMgr::Copy_Sound(_tchar* pOriginSoundKey, _tchar* pCopySoundKeyOut)
{
	SOUNDS::iterator Pair = find_if(m_mapSound.begin(), m_mapSound.end(), CTag_Finder((wstring)pOriginSoundKey));

	if (Pair == m_mapSound.end())
		return E_FAIL;

	FMOD_SOUND* pSound = nullptr;
	FMOD_RESULT Ret = FMOD_System_CreateSound(m_pSystem, Pair->second->Get_FilePath(), FMOD_DEFAULT, 0, &pSound);
	if (Ret != FMOD_OK) return E_FAIL;

	_tchar szTemp[MAX_PATH] = { 0, };
	swprintf_s(szTemp, TEXT("%s_%d"), pOriginSoundKey, Pair->second->Get_CopyCount());

	int iStrLen = (int)wcslen(szTemp) + 1;
	_tchar* pSoundKey = new _tchar[iStrLen];
	ZeroMemory(pSoundKey, sizeof(_tchar) * iStrLen);
	lstrcpy(pSoundKey, szTemp);

	CSound *pSoundObj = CSound::Create(pSound, Pair->second->Get_FilePath());
	if (pSound == nullptr)
		return E_FAIL;

	m_mapSound.emplace(pSoundKey, pSoundObj);

	lstrcpy(pCopySoundKeyOut, pSoundKey);
	Pair->second->Add_CopyCount();

	return S_OK;
}

HRESULT CSoundMgr::Reserve_Manager(const char* pFilePath, _uint iNumManualChannels)
{
	if (pFilePath == nullptr)
		return E_FAIL;

	m_iNumManualChannels = iNumManualChannels;

	for (_uint i = 0; i < MAX_CHANNEL_COUNT; ++i)
	{
		m_Channels[i].first = nullptr;
		m_Channels[i].second = NO_USE_CHNNEL;
	}

	FMOD_System_Create(&m_pSystem, FMOD_VERSION);
	FMOD_System_Init(m_pSystem, MAX_CHANNEL_COUNT, FMOD_INIT_NORMAL, NULL);
	Load_SoundFile(pFilePath);

	return S_OK;
}

void CSoundMgr::Tick(_double TimeDelta)
{
	for (auto& Pair : m_mapSound)
	{
		Pair.second->Tick(TimeDelta, nullptr);
	}

	CHANNEL* pChannel = nullptr;
	for (_int i = 0; i < MAX_CHANNEL_COUNT; ++i)
	{
		pChannel = &m_Channels[i];
		if (pChannel->second)
		{
			pChannel->second->UpdateUseVolume(m_Channels[i].first);

			FMOD_BOOL bPlayFlag = FALSE;
			FMOD_Channel_IsPlaying(m_Channels[i].first, &bPlayFlag);
			if (bPlayFlag == FALSE)
			{
				FMOD_Channel_Stop(pChannel->first);
				pChannel->second = NO_USE_CHNNEL;
			}
		}
	}
}

void CSoundMgr::Play_Sound(const wstring& wstrSoundKey, _float fVolume, _bool bIsBGM, _bool bRefresh,	_int iManualChannelIndex)
// {
// 	if (m_iNumManualChannels == MAX_CHANNEL_COUNT)
// 		return;
//
// 	SOUNDS::iterator Pair = find_if(m_mapSound.begin(), m_mapSound.end(), CTag_Finder(wstrSoundKey));
// 	if (Pair == m_mapSound.end())
// 		return;
//
// 	CSound* pUseSound = Pair->second;
//
// 	FMOD_BOOL bIsPlaying = FALSE;
// 	_uint iPlayIndex = 0;
//
// 	if (pUseSound->Get_ChannelIndex() == -1)
// 	{
// 		if (iManualChannelIndex == -1)
// 		{
// 			for (_uint i = m_iNumManualChannels; i < MAX_CHANNEL_COUNT; ++i)
// 			{
// 				if (FMOD_Channel_IsPlaying(m_Channels[i].first, &bIsPlaying))
// 				{
// 					iPlayIndex = i;
// 					break;
// 				}
// 			}
// 		}
// 		else
// 			iPlayIndex = iManualChannelIndex;
//
// 		if (iPlayIndex >= MAX_CHANNEL_COUNT || iPlayIndex < 0)
// 			return;
//
// 		pUseSound->Set_ChannelIndex(iPlayIndex);
// 	}
//
// 	FMOD_CHANNEL*	pUseChannel;
// 	FMOD_SOUND*		pSound = pUseSound->GetSoundPtr();
// 	_int					iChannelIndex = pUseSound->Get_ChannelIndex();
//
// 	FMOD_Channel_IsPlaying(m_Channels[iChannelIndex].first, &bIsPlaying);
//
// 	if (bIsPlaying == false)
// 	{
// 		FMOD_System_PlaySound(m_pSystem, pUseSound->GetSoundPtr(), nullptr, FALSE, &pUseChannel);
//
// 		if (bIsBGM)
// 			FMOD_Channel_SetMode(pUseChannel, FMOD_LOOP_NORMAL);
//
// 		pUseSound->Set_Volume(pUseChannel, fVolume);
//
// 		m_Channels[iPlayIndex] = { pUseChannel, pUseSound };
// 	}
// 	else
// 	{
// 		if (bRefresh == true)
// 		{
// 			FMOD_Channel_Stop(m_Channels[iChannelIndex].first);
//
// 			FMOD_System_PlaySound(m_pSystem, pUseSound->GetSoundPtr(), nullptr, FALSE, &pUseChannel);
//
// 			if (bIsBGM)
// 				FMOD_Channel_SetMode(pUseChannel, FMOD_LOOP_NORMAL);
//
// 			pUseSound->Set_Volume(pUseChannel, fVolume);
// 		}
// 	}
//
// 	FMOD_System_Update(m_pSystem);
// }

{
	if (m_iNumManualChannels == MAX_CHANNEL_COUNT)
		return;

	SOUNDS::iterator Pair = find_if(m_mapSound.begin(), m_mapSound.end(), CTag_Finder(wstrSoundKey));
	if (Pair == m_mapSound.end())
		return;

	FMOD_BOOL bPlayFlag = FALSE;
	_int iPlayIndex = -1;
	if (iManualChannelIndex == -1)
	{
		for (_uint i = m_iNumManualChannels; i < MAX_CHANNEL_COUNT; i++)
		{
			if (FMOD_Channel_IsPlaying(m_Channels[i].first, &bPlayFlag))
			{
				iPlayIndex = i;
				break;
			}
		}
	}
	else
		iPlayIndex = iManualChannelIndex;

	if (iPlayIndex >= MAX_CHANNEL_COUNT || iPlayIndex < 0)
		return;

	FMOD_CHANNEL* pUseChannel;
	CSound* pUseSound = Pair->second;

	FMOD_System_PlaySound(m_pSystem, pUseSound->GetSoundPtr(), nullptr, FALSE, &pUseChannel);
	if (bIsBGM)
		FMOD_Channel_SetMode(pUseChannel, FMOD_LOOP_NORMAL);

	pUseSound->Set_Volume(pUseChannel, fVolume);
	FMOD_System_Update(m_pSystem);

	m_Channels[iPlayIndex] = { pUseChannel, pUseSound };
}

void CSoundMgr::Stop_Sound(_uint iManualChannelIndex)
{
	if (m_iNumManualChannels == 0 || iManualChannelIndex >= MAX_CHANNEL_COUNT)
		return;

	FMOD_Channel_Stop(m_Channels[iManualChannelIndex].first);

	// if (m_Channels[iManualChannelIndex].second != NO_USE_CHNNEL)
		// m_Channels[iManualChannelIndex].second->Set_ChannelIndex(-1);

	m_Channels[iManualChannelIndex].second = NO_USE_CHNNEL;
}

void CSoundMgr::Stop_All()
{
	for (_uint i = 0; i < MAX_CHANNEL_COUNT; ++i)
	{
		FMOD_Channel_Stop(m_Channels[i].first);

		// if (m_Channels[i].second != NO_USE_CHNNEL)
			// m_Channels[i].second->Set_ChannelIndex(-1);

		m_Channels[i].second = NO_USE_CHNNEL;
	}
}

HRESULT CSoundMgr::Load_SoundFile(const char* pFilePath)
{
	char szPath[MAX_PATH] = "";
	strcat_s(szPath, pFilePath);
	strcat_s(szPath, "*.*");

	_finddata_t FindData;
	intptr_t handle = _findfirst(szPath, &FindData);
	if (handle == -1)
		return E_FAIL;

	char szFullPath[MAX_PATH] = "";
	char szCurPath[MAX_PATH] = "";
	strcpy_s(szCurPath, pFilePath);

	_int	iResult = 0;
	while (iResult != -1)
	{
		strcpy_s(szFullPath, szCurPath);
		strcat_s(szFullPath, FindData.name);

		FMOD_SOUND* pSound = nullptr;
		FMOD_RESULT Ret = FMOD_System_CreateSound(m_pSystem, szFullPath, FMOD_DEFAULT, 0, &pSound);
		if (Ret == FMOD_OK)
		{
			string		strFileName = FindData.name;
			wstring	wstrFileName = L"";

			wstrFileName.assign(strFileName.begin(), strFileName.end());

			CSound*	pSoundObj = CSound::Create(pSound,pFilePath );
			NULL_CHECK_RETURN(pSoundObj, E_FAIL);

			m_mapSound.emplace(wstrFileName, pSoundObj);
		}

		iResult = _findnext(handle, &FindData);
	}

	FMOD_System_Update(m_pSystem);
	_findclose(handle);

	return S_OK;
}

void CSoundMgr::Stop_SoundAuto()
{
	if (m_iNumManualChannels == MAX_CHANNEL_COUNT)
		return;

	FMOD_BOOL bPlayFlag = FALSE;
	for (_uint i = 0; i < MAX_CHANNEL_COUNT; ++i)
	{
		FMOD_Channel_IsPlaying(m_Channels[i].first, &bPlayFlag);
		if (bPlayFlag == FALSE && m_Channels[i].second != NO_USE_CHNNEL)
		{
			FMOD_Channel_Stop(m_Channels[i].first);
			// m_Channels[i].second->Set_ChannelIndex(-1);
			m_Channels[i].second = NO_USE_CHNNEL;
		}
	}
}

void CSoundMgr::Free()
{
	Stop_All();

	for (auto& Pair : m_mapSound)
	{
		// delete[] Pair.first;
		Safe_Release(Pair.second);
	}

	m_mapSound.clear();

	FMOD_System_Release(m_pSystem);
	FMOD_System_Close(m_pSystem);
}

