#pragma once
#include "Client_Defines.h"
#include "UI.h"

BEGIN(Client)

class CNumberUI final :	public CUI
{
public:
	enum NUMUITYPE { NUMUI_GOLD, NUMUI_BULLET, NUMUI_THROW, NUMUI_END };

public:
	CNumberUI(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CNumberUI(const CNumberUI& rhs);
	virtual ~CNumberUI() = default;

	virtual HRESULT					Initialize_Prototype() override;
	virtual HRESULT					Initialize_Clone(const wstring& wstrPrototypeTag, void* pArg) override;
	virtual void					Tick(_double dTimeDelta) override;
	virtual void					Late_Tick(_double dTimeDelta) override;
	virtual HRESULT					Render() override;

private:
	HRESULT							SetUp_Components();
	HRESULT							SetUp_ShaderResources();

private:
	int								m_iNumCnt;
	int								m_iNumber;
	_float4							m_vNumColor;
	_int							m_iType = 0;
public:
	static CNumberUI*				Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject*			Clone(const wstring& wstrPrototypeTag, void* pArg = nullptr) override;
	virtual void					Free() override;
};

END