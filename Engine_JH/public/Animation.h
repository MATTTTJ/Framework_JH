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

public:
	HRESULT			Initialize(aiAnimation* pAIAnimation, class CModel* pModel);
	void			Update_Bones(_double TimeDelta);
private:
	class CModel*				m_pModel = nullptr;

	string						m_strName = "";
	_double						m_Duration = 0.0; // 애니메이션의 길이
	_double						m_dTIckPerSecond; // 애니메이션이 재생되는 속도 (높을수록 빠름)
	_double						m_dPlayTime = 0.0; // 애니메이션이 재생된 시간을 저장
	_bool						m_bIsFinished = false;
	_bool						m_bIsLooping = false; 
	_uint						m_iNumChannels = 0; // 이 애니메이션을 재생하기 위해 갱신해야하는 뼈들
	vector<class CChannel*>		m_vecChannels;

public:
	static CAnimation*	Create(aiAnimation* pAIAnimation, class CModel* pModel);
	virtual void		Free() override;
};

END