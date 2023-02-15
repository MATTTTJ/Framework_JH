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
	/* 사운드 크기 조절 함수 수동 채널일 경우 사용 가능
	볼륨 설정할 수동 채널 인덱스, 설정할 볼륨 크기 */
	void											Set_Volume(_uint iManualChannelIndex, _float fVolume);

	/* 마스터 볼륨 크기 설정 함수 모든 채널에 공통으로 부여되어 적용 */
	void											Set_MasterVolume(_float fVolume);

	/* 거리에 따라 사운드 크기가 바뀌는 기능
	설정할 사운드 키 값, 첫번째 인자로 전달한 사운드는 두번째 인자로 전달된 SoundDesc에 정보로 거리를 계산하여 볼륨을 조절한다
	CSound::SOUND_DESC 주석 참고 */
	void											Set_SoundDesc(const wstring& wstrSoundKey, CSound::SOUND_DESC& SoundDesc);

	HRESULT											Copy_Sound(_tchar* pOriginSoundKey, _tchar* pCopySoundKeyOut);


public:
	/* Sound_Manager 초기화 함수
	사운드 파일 경로, 개발자가 수동으로 제어할 채널의 갯수 인자 전달
	iNumManualChannels 설정 인자가 10이라면 0 ~ 9 인덱스 채널을 수동 채널로 사용 */
	HRESULT											Reserve_Manager(const char* pFilePath, _uint iNumManualChannels = 0);

	/* 자동 채널 사운드 종료 시 채널 제거, 3D 사운드일 경우 거리 계산하여 음량 조절 업데이트 함수 */
	void											Tick(_double TimeDelta);

	/* 사운드 재생 함수
	사운드 파일이름, 설정할 볼륨 크기 0 ~ 1.f,
	BGM이라면 true로 false일 경우 한번 재생 후 종료,
	수동 제어할 채널일 경우 개발자가 Reserve_Manager 인자로 설정한 수동 제어 채널 인덱스 유효 값 중 원하는 값으로 설정하여 사용
	Default Parameter -1 일 경우 자동채널로 부여되어 이 후 사운드 크기 설정 불가능, 사운드 종료는 자동으로 수행된다 */
	void											Play_Sound(const wstring& wstrSoundKey, _float fVolume, _bool bIsBGM, _bool bRefresh, _int iManualChannelIndex = -1);

	/* 사운드 종료 함수 수동 채널일 경우 사용 가능 자동 채널은 따로 호출하지 않아도 자동으로 종료됨*/
	void											Stop_Sound(_uint iManualChannelIndex);
	void											Stop_All();

private:
	HRESULT											Load_SoundFile(const char* pFilePath);
	void											Stop_SoundAuto();

public:
	virtual void									Free() override;
};
END
