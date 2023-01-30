#pragma once
#include "Client_Defines.h"
#include "Base.h"
#include "Effect.h"

BEGIN(Client)

class CEffect_Manager : public CBase
{
	DECLARE_SINGLETON(CEffect_Manager)

public:
	CEffect_Manager();
	virtual ~CEffect_Manager() = default;

public:
	HRESULT		Initialize(LEVEL eLevel);
	CEffect*	GetEffect(LEVEL eLevel, const wstring wstrEffectTag);
	void		Reset(LEVEL eLevel);

private:
	HRESULT		CreatEffect(LEVEL eLevel);
	CEffect*	Find_Effect(LEVEL eLevel, const wstring wstrEffectTag);
	CEffect*	Add_Effect(LEVEL eLevel, CEffect::EFFECTINFO tInfo, list<CEffect*>* pEffectList);
	void		LoadEffectData(LEVEL eLevel);

private:
	list<CEffect*> m_EffectsListt;
	typedef list<CEffect*> EFFECTS;
	//map<const _tchar*, EFFECTS> m_Effects;
	//map<_tchar*, EFFECTS> m_Effects;
	unordered_map<wstring, EFFECTS> m_mapEffects;

public:
	virtual void Free();
};

END