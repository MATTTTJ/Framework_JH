#pragma once
#include "Client_Defines.h"
#include "UI.h"

BEGIN(Engine)

// Base
class CMonsterUI final:	public CUI
{
public:

private:
	CMonsterUI(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CMonsterUI(const CMonsterUI& rhs);
	virtual ~CMonsterUI() = default;

protected:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize_Clone(const wstring& wstrPrototypeTag, void* pArg) override;
	virtual void	Tick(_double dTimeDelta) override;
	virtual void	Late_Tick(_double dTimeDelta) override;
	virtual HRESULT Render() override;

private:
	HRESULT			SetUp_Component();
	HRESULT			SetUp_ShaderResources();

public:
	static CMonsterUI*	Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(const wstring& wstrPrototypeTag, void* pArg = nullptr) override;
	virtual void Free() override;
};

// HP

class CMonsterUI_HP_Red final : public CUI
{
public:

private:
	CMonsterUI_HP_Red(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CMonsterUI_HP_Red(const CMonsterUI_HP_Red& rhs);
	virtual ~CMonsterUI_HP_Red() = default;

protected:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize_Clone(const wstring& wstrPrototypeTag, void* pArg) override;
	virtual void	Tick(_double dTimeDelta) override;
	virtual void	Late_Tick(_double dTimeDelta) override;
	virtual HRESULT Render() override;

private:
	HRESULT			SetUp_Component();
	HRESULT			SetUp_ShaderResources();

public:
	static CMonsterUI_HP_Red*	Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(const wstring& wstrPrototypeTag, void* pArg = nullptr) override;
	virtual void Free() override;
};























END