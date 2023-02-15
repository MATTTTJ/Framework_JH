#pragma once
#include "Sound.h"

#define MAX_CHANNEL_COUNT		32
#define NO_USE_CHNNEL				nullptr

BEGIN(Engine)

class CSoundMgr :	public CBase
{
	DECLARE_SINGLETON(CSoundMgr);

private:
	typedef map<const wstring, CSound*>		SOUNDS;
	SOUNDS									m_mapSound;

	typedef pair<FMOD_CHANNEL*, CSound*>	CHANNEL;
	CHANNEL									m_Channels[MAX_CHANNEL_COUNT];

	FMOD_SYSTEM*							m_pSystem = nullptr;
	_uint									m_iNumManualChannels = 0;


private:
	CSoundMgr();
	virtual ~CSoundMgr() = default;

public:
	/* ���� ũ�� ���� �Լ� ���� ä���� ��� ��� ����
	���� ������ ���� ä�� �ε���, ������ ���� ũ�� */
	void											Set_Volume(_uint iManualChannelIndex, _float fVolume);

	/* ������ ���� ũ�� ���� �Լ� ��� ä�ο� �������� �ο��Ǿ� ���� */
	void											Set_MasterVolume(_float fVolume);

	/* �Ÿ��� ���� ���� ũ�Ⱑ �ٲ�� ���
	������ ���� Ű ��, ù��° ���ڷ� ������ ����� �ι�° ���ڷ� ���޵� SoundDesc�� ������ �Ÿ��� ����Ͽ� ������ �����Ѵ�
	CSound::SOUND_DESC �ּ� ���� */
	void											Set_SoundDesc(const wstring& wstrSoundKey, CSound::SOUND_DESC& SoundDesc);

	HRESULT											Copy_Sound(_tchar* pOriginSoundKey, _tchar* pCopySoundKeyOut);


public:
	/* Sound_Manager �ʱ�ȭ �Լ�
	���� ���� ���, �����ڰ� �������� ������ ä���� ���� ���� ����
	iNumManualChannels ���� ���ڰ� 10�̶�� 0 ~ 9 �ε��� ä���� ���� ä�η� ��� */
	HRESULT											Reserve_Manager(const char* pFilePath, _uint iNumManualChannels = 0);

	/* �ڵ� ä�� ���� ���� �� ä�� ����, 3D ������ ��� �Ÿ� ����Ͽ� ���� ���� ������Ʈ �Լ� */
	void											Tick(_double TimeDelta);

	/* ���� ��� �Լ�
	���� �����̸�, ������ ���� ũ�� 0 ~ 1.f,
	BGM�̶�� true�� false�� ��� �ѹ� ��� �� ����,
	���� ������ ä���� ��� �����ڰ� Reserve_Manager ���ڷ� ������ ���� ���� ä�� �ε��� ��ȿ �� �� ���ϴ� ������ �����Ͽ� ���
	Default Parameter -1 �� ��� �ڵ�ä�η� �ο��Ǿ� �� �� ���� ũ�� ���� �Ұ���, ���� ����� �ڵ����� ����ȴ� */
	void											Play_Sound(const wstring& wstrSoundKey, _float fVolume, _bool bIsBGM, _bool bRefresh, _int iManualChannelIndex = -1);

	/* ���� ���� �Լ� ���� ä���� ��� ��� ���� �ڵ� ä���� ���� ȣ������ �ʾƵ� �ڵ����� �����*/
	void											Stop_Sound(_uint iManualChannelIndex);
	void											Stop_All();

private:
	HRESULT											Load_SoundFile(const char* pFilePath);
	void											Stop_SoundAuto();

public:
	virtual void									Free() override;
};
END
