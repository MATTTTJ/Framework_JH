#pragma once
#include "GameObject.h"

BEGIN(Engine)

class ENGINE_DLL CUI abstract :	public CGameObject
{
protected:
	CUI(ID3D11Device*	pDevice, ID3D11DeviceContext* pContext);
	CUI(const CUI& rhs);
	virtual ~CUI() = default;
public:
	virtual HRESULT			Initialize_Prototype() override;
	virtual HRESULT			Initialize_Clone(const wstring& wstrPrototypeTag, void* pArg) override;

	virtual void			Tick(_double dTimeDelta) override;
	virtual void			Late_Tick(_double dTimeDelta) override;
	virtual HRESULT			Render() override;

	virtual void			Set_parentName(_uint iCulLevel, const _tchar* pParentTag);



public:
	virtual CGameObject*	Clone(const wstring& wstrPrototypeTag, void* pArg) PURE;
	virtual void			Free() override;
};

END