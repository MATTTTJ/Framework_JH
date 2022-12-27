#pragma once
#include "Base.h"
BEGIN(Engine)

class CAnimation final : public CBase
{
public:
	CAnimation();
	virtual ~CAnimation() = default;

public:
	HRESULT			Save_Animation(HANDLE& hFile, DWORD& dwByte);
	HRESULT			Load_Animation(HANDLE& hFile, DWORD& dwByte);
	string&			Get_AnimationName() { return m_strName; }
	_bool			Get_AnimFinish(void) { return m_bIsFinished; }
	_bool&			Get_AnimationLoop() { return m_bIsLooping; }
	_float			Get_AnimationProgress() { return _float(m_dPlayTime / m_dDuration); }
	void			Reset_Animation();
public:
	HRESULT			Initialize(aiAnimation* pAIAnimation, class CModel* pModel);
	_bool			Update_Bones(_double dTimeDelta, _double AnimSpeed = 1.0);
	// void			Update_Bones(_double dTimeDelta, _double AnimSpeed = 1.0);

	_bool			Update_Lerp(_double dTimeDelta, CAnimation* pNextAnim ,_double LerpSpeed, _bool bFinish = false);
	void			Update_Lerp(_double dTimeDelta, _float fRatio);
	void			Update_Additive(_double dTimeDelta, _float fRatio);

public:
	

private:
	class CModel*				m_pModel = nullptr;

	string						m_strName = "";
	_double						m_dDuration = 0.0; // 애니메이션의 길이
	_double						m_dTickPerSecond; // 애니메이션이 재생되는 속도 (높을수록 빠름)
	_double						m_dPlayTime = 0.0; // 애니메이션이 재생된 시간을 저장
	_bool						m_bIsFinished = false;
	_bool						m_bIsLooping = false;
	_bool						m_bIsLerpEnd = false;
	_uint						m_iNumChannels = 0; // 이 애니메이션을 재생하기 위해 갱신해야하는 뼈들
	vector<class CChannel*>		m_vecChannels;

public:
	static CAnimation*	Create(aiAnimation* pAIAnimation, class CModel* pModel);
	virtual void		Free() override;
};

END