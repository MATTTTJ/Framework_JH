#pragma once
#include "Base.h"
BEGIN(Engine)

class CAnimation final : public CBase
{
public:
	CAnimation();
	virtual ~CAnimation() = default;

public:
	HRESULT			Initialize(aiAnimation* pAIAnimation);

private:
	char					m_szName[MAX_PATH];
	_double					m_Duration = 0.f; // 애니메이션의 길이
	_double					m_TIckPerSecond; // 애니메이션이 재생되는 속도 (높을수록 빠름)

	// 이 애니메이션을 재생하기 위해 갱신해야하는 뼈들

	_uint					m_iNumBones = 0;
	vector<class CBone*>	m_Bones;

public:
	static CAnimation*	Create(aiAnimation* pAIAnimation);
	virtual void		Free() override;
};

END