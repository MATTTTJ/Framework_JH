#pragma once
#include "Base.h"
#include "Damage_Font.h"

BEGIN(Engine)
class CDamageFont_Mgr final : public CBase
{
	DECLARE_SINGLETON(CDamageFont_Mgr);

public:
	CDamageFont_Mgr();
	virtual ~CDamageFont_Mgr() = default;

public:
	void Tick();

public:
	HRESULT Add_DamageFont(_uint iLevelIndex, const wstring pLayerTag, void* tFontDesc);

	vector<CGameObject*> m_vecFont;
private:

public:
	virtual void Free() override;

};

END