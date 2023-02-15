#include "stdafx.h"
#include "..\public\Sound.h"
#include "..\public\Transform.h"

_float	CSound::m_fMasterVolume = 0.5f;

CSound::CSound()
{
}

void CSound::Set_SoundDesc(SOUND_DESC& SoundDesc)
{
	Clear_SoundDesc();

	memcpy(&m_SoundDesc, &SoundDesc, sizeof(SOUND_DESC));

	_vector	vStart = m_SoundDesc.pStartTransform->Get_State(CTransform::STATE_TRANSLATION);
	_vector	vEnd = m_SoundDesc.pTargetTransform->Get_State(CTransform::STATE_TRANSLATION);
	_float		fLength = XMVectorGetX(XMVector4Length(vStart - vEnd));

	m_fVolume[VOLUME_3D_RANGE] = (1.f - (fLength / m_SoundDesc.fRange)) * (fLength < m_SoundDesc.fRange);

	Safe_AddRef(m_SoundDesc.pStartTransform);
	Safe_AddRef(m_SoundDesc.pTargetTransform);
}

void CSound::Set_Volume(FMOD_CHANNEL* pChannel, _float fVolume)
{
	NULL_CHECK_RETURN(pChannel, );

	m_fVolume[VOLUME_DEFAULT] = fVolume > 1.f ? 1.f : fVolume < 0.f ? 0.f : fVolume;

	UpdateUseVolume(pChannel);
}

HRESULT CSound::Initialize(FMOD_SOUND* pSound, const char* pFilePath)
{
	m_pSound = pSound;
	strcpy_s(m_szFilePath, pFilePath);

	ZeroMemory(&m_SoundDesc, sizeof(SOUND_DESC));



	return S_OK;
}

void CSound::Tick(_double TimeDelta, FMOD_CHANNEL* pChannel)
{
	if (m_SoundDesc.bIs3D == false)
		return;

	_vector	vStart = m_SoundDesc.pStartTransform->Get_State(CTransform::STATE_TRANSLATION);
	_vector	vEnd = m_SoundDesc.pTargetTransform->Get_State(CTransform::STATE_TRANSLATION);
	_float		fLength = XMVectorGetX(XMVector4Length(vStart - vEnd));

	m_fVolume[VOLUME_3D_RANGE] = (1.f - (fLength / m_SoundDesc.fRange)) * (fLength < m_SoundDesc.fRange);

	UpdateUseVolume(pChannel);
}

void CSound::UpdateUseVolume(FMOD_CHANNEL* pChannel)
{
	if (pChannel != nullptr)
	{
		m_fVolume[VOLUME_USE] = m_fVolume[VOLUME_DEFAULT] * m_fMasterVolume * m_fVolume[VOLUME_3D_RANGE];
		FMOD_Channel_SetVolume(pChannel, m_fVolume[VOLUME_USE]);
	}
}

void CSound::Clear_SoundDesc()
{
	Safe_Release(m_SoundDesc.pStartTransform);
	Safe_Release(m_SoundDesc.pTargetTransform);

	ZeroMemory(&m_SoundDesc, sizeof(SOUND_DESC));
}

CSound* CSound::Create(FMOD_SOUND* pSound, const char* pFilePath)
{
	CSound*	pInstance = new CSound;

	if (FAILED(pInstance->Initialize(pSound, pFilePath)))
	{
		MSG_BOX("Failed to Create : CSound");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSound::Free()
{
	Clear_SoundDesc();
	FMOD_Sound_Release(m_pSound);
}

