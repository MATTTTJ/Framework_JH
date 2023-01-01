#pragma once
#include "Client_Defines.h"
#include "UI.h"

BEGIN(Client)

class CPlayerUI_Base final : public CUI
{
private:
	CPlayerUI_Base(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CPlayerUI_Base(const CPlayerUI_Base& rhs);
	virtual ~CPlayerUI_Base() = default;

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
	static CPlayerUI_Base*	Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(const wstring& wstrPrototypeTag, void* pArg = nullptr) override;
	virtual void Free() override;
};

class CPlayerUI_Skill final : public CUI
{
private:
	CPlayerUI_Skill(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CPlayerUI_Skill(const CPlayerUI_Skill& rhs);
	virtual ~CPlayerUI_Skill() = default;

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
	static CPlayerUI_Skill*	Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(const wstring& wstrPrototypeTag, void* pArg = nullptr) override;
	virtual void Free() override;
};

class CPlayer_UI_Throw final : public CUI
{
private:
	CPlayer_UI_Throw(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CPlayer_UI_Throw(const CPlayer_UI_Throw& rhs);
	virtual ~CPlayer_UI_Throw() = default;

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
	static CPlayer_UI_Throw*	Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(const wstring& wstrPrototypeTag, void* pArg = nullptr) override;
	virtual void Free() override;
};

class CPlayer_UI_Dash final : public CUI
{
private:
	CPlayer_UI_Dash(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CPlayer_UI_Dash(const CPlayer_UI_Dash& rhs);
	virtual ~CPlayer_UI_Dash() = default;

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
	static CPlayer_UI_Dash*	Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(const wstring& wstrPrototypeTag, void* pArg = nullptr) override;
	virtual void Free() override;
};

class CPlayer_UI_Weapon final : public CUI
{
private:
	CPlayer_UI_Weapon(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CPlayer_UI_Weapon(const CPlayer_UI_Weapon& rhs);
	virtual ~CPlayer_UI_Weapon() = default;

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
	static CPlayer_UI_Weapon*	Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(const wstring& wstrPrototypeTag, void* pArg = nullptr) override;
	virtual void Free() override;
};

class CPlayer_UI_Weapon_First final : public CUI
{
private:
	CPlayer_UI_Weapon_First(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CPlayer_UI_Weapon_First(const CPlayer_UI_Weapon_First& rhs);
	virtual ~CPlayer_UI_Weapon_First() = default;

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
	static CPlayer_UI_Weapon_First*	Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(const wstring& wstrPrototypeTag, void* pArg = nullptr) override;
	virtual void Free() override;
};

class CPlayer_UI_Weapon_Pic final : public CUI
{
private:
	CPlayer_UI_Weapon_Pic(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CPlayer_UI_Weapon_Pic(const CPlayer_UI_Weapon_Pic& rhs);
	virtual ~CPlayer_UI_Weapon_Pic() = default;

protected:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize_Clone(const wstring& wstrPrototypeTag, void* pArg) override;
	virtual void	Tick(_double dTimeDelta) override;
	virtual void	Late_Tick(_double dTimeDelta) override;
	virtual HRESULT Render() override;

private:
	HRESULT			SetUp_Component();
	HRESULT			SetUp_ShaderResources();
private:
	wstring			m_wstrWeaponName = L"WEAPON_DEFAULT";
public:
	static CPlayer_UI_Weapon_Pic*	Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(const wstring& wstrPrototypeTag, void* pArg = nullptr) override;
	virtual void Free() override;
};
END