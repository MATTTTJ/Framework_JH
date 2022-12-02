#pragma once
#include "Base.h"
BEGIN(Engine)

class CAnimation final : public CBase
{
public:
	CAnimation();
	virtual ~CAnimation() = default;

public:
	HRESULT			Initialize(aiAnimation* pAIAnimation, class CModel* pModel);
	void			Update_Bones(_double TimeDelta);
private:
	char						m_szName[MAX_PATH];
	_double						m_Duration = 0.0; // 애니메이션의 길이
	_double						m_TIckPerSecond; // 애니메이션이 재생되는 속도 (높을수록 빠름)

	_double						m_PlayTime = 0.0; // 애니메이션이 재생된 시간을 저장

	// 이 애니메이션을 재생하기 위해 갱신해야하는 뼈들
	_uint						m_iNumChannels = 0;
	vector<class CChannel*>		m_Channels;

public:
	static CAnimation*	Create(aiAnimation* pAIAnimation, class CModel* pModel);
	virtual void		Free() override;
};

END