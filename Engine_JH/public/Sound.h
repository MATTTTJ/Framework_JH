#pragma once
#include "Base.h"

#include <io.h>
#include "Fmod/fmod.h"
#include "Fmod/fmod.hpp"

BEGIN(Engine)

class CSound :	public CBase
{
public:
	enum VOLUME_TYPE { VOLUME_DEFAULT, VOLUME_3D_RANGE, VOLUME_USE, VOLUME_END };

	typedef struct ENGINE_DLL tag3DSoundDesc
	{
		// 3D ����� ���� true�� ���� 
		_bool					bIs3D;
		_float					fRange;

		// ������ ������ġ ���� Transform������Ʈ
		class CTransform*	pStartTransform;
		// Target ��ġ�� �������� ������ �����ȴ�. 
		class CTransform*	pTargetTransform;
	} SOUND_DESC;

protected:
	CSound();
	virtual ~CSound() = default;

public:
	FMOD_SOUND*					GetSoundPtr() { return m_pSound; }
	const _float&				GetVolume(VOLUME_TYPE eType) const { return m_fVolume[eType]; }

	const _int&					Get_ChannelIndex() const { return m_iChannelIndex; }
	void						Set_SoundDesc(SOUND_DESC& SoundDesc);
	void						Set_Volume(FMOD_CHANNEL* pChannel, _float fVolume);
	static void					Set_MasterVolume(_float fVolume) { m_fMasterVolume = fVolume; }
	void						Set_ChannelIndex(_int iChannelIndex) { m_iChannelIndex = iChannelIndex; }

public:
	HRESULT						Initialize(FMOD_SOUND* pSound, const char* pFilePath);
	void						Tick(_double TimeDelta, FMOD_CHANNEL* pChannel);

public:
	void						UpdateUseVolume(FMOD_CHANNEL* pChannel);
	const char*					Get_FilePath() { return m_szFilePath; }
	_uint						Get_CopyCount() { return m_iCopyCount; }
	void						Add_CopyCount() { m_iCopyCount++; }

private:
	FMOD_SOUND*					m_pSound = nullptr;
	SOUND_DESC					m_SoundDesc;

	static _float				m_fMasterVolume;
	_float						m_fVolume[VOLUME_END] = { 0.5f, 1.f, 0.5f };
	_int						m_iChannelIndex = -1;
	char						m_szFilePath[MAX_PATH] = "";
	_uint						m_iCopyCount = 0;

private:
	void						Clear_SoundDesc();

public:
	static CSound*				Create(FMOD_SOUND* pSound, const char* pFilePath);
	virtual void				Free() override;



};

END