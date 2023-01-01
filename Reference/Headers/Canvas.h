#pragma once
#include "UI.h"

BEGIN(Engine)

class ENGINE_DLL CCanvas abstract:	public CUI
{
public:
	CCanvas(ID3D11Device*	pDevice, ID3D11DeviceContext* pContext);
	CCanvas(const CCanvas& rhs);
	virtual ~CCanvas() = default;

public:
	virtual HRESULT Initialize_Prototype()override;
	virtual HRESULT Initialize_Clone(const wstring& wstrPrototypeTag, void* pArg) override;
	virtual void	Tick(_double TimeDelta)override;
	virtual void	Late_Tick(_double TimeDelta)override;
	virtual HRESULT Render()override;

public:
	virtual HRESULT	Add_ChildUI(CUI* pUI)sealed;

protected:
	vector<CUI*>		m_vecChildrenUI;

public:
	virtual CGameObject* Clone(const wstring& wstrPrototypeTag, void* pArg = nullptr) = 0;
	virtual void Free() override;

};

END