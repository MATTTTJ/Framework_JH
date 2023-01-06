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

private:
	_bool			m_bIsChange = false;



public:
	static CPlayerUI_Skill*	Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(const wstring& wstrPrototypeTag, void* pArg = nullptr) override;
	virtual void Free() override;
};

class CPlayerUI_Skill_Base final : public CUI
{
private:
	CPlayerUI_Skill_Base(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CPlayerUI_Skill_Base(const CPlayerUI_Skill_Base& rhs);
	virtual ~CPlayerUI_Skill_Base() = default;

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
	_bool			m_bIsChange = false;



public:
	static CPlayerUI_Skill_Base*	Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(const wstring& wstrPrototypeTag, void* pArg = nullptr) override;
	virtual void Free() override;
};

class CPlayerUI_Gold final : public CUI
{
private:
	CPlayerUI_Gold(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CPlayerUI_Gold(const CPlayerUI_Gold& rhs);
	virtual ~CPlayerUI_Gold() = default;

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
	static CPlayerUI_Gold*	Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(const wstring& wstrPrototypeTag, void* pArg = nullptr) override;
	virtual void Free() override;
};

class CPlayerUI_Emerald final : public CUI
{
private:
	CPlayerUI_Emerald(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CPlayerUI_Emerald(const CPlayerUI_Emerald& rhs);
	virtual ~CPlayerUI_Emerald() = default;

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
	static CPlayerUI_Emerald*	Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
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
	_uint			m_iThrownCnt = 0;
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

class CPlayer_UI_Weapon_Number final : public CUI
{
private:
	CPlayer_UI_Weapon_Number(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CPlayer_UI_Weapon_Number(const CPlayer_UI_Weapon_Number& rhs);
	virtual ~CPlayer_UI_Weapon_Number() = default;

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
	wstring			m_wstrWeaponNumber = L"";
public:
	static CPlayer_UI_Weapon_Number*	Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
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

class CPlayer_UI_BulletType final : public CUI
{
private:
	CPlayer_UI_BulletType(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CPlayer_UI_BulletType(const CPlayer_UI_BulletType& rhs);
	virtual ~CPlayer_UI_BulletType() = default;

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
	static CPlayer_UI_BulletType*	Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(const wstring& wstrPrototypeTag, void* pArg = nullptr) override;
	virtual void Free() override;
};


class CPlayer_UI_CountMachine  : public CUI
{

private:
	CPlayer_UI_CountMachine(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CPlayer_UI_CountMachine(const CPlayer_UI_CountMachine& rhs);
	virtual ~CPlayer_UI_CountMachine() = default;

protected:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize_Clone(const wstring& wstrPrototypeTag, void* pArg) override;
	virtual void	Tick(_double dTimeDelta) override;
	virtual void	Late_Tick(_double dTimeDelta) override;
	virtual HRESULT Render() override;
	void			Create_CountUI();
	void			InttoString(_uint ConvertCnt, _float2 vPos, _float4 vColor, _float2 vSize);
	
private:
	HRESULT			SetUp_Component();
	HRESULT			SetUp_ShaderResources();
private:
	wstring			m_wstrWeaponName = L"WEAPON_DEFAULT";

	_uint			m_iPlayer_BulletCnt= 50;
	_uint			m_iLastPlayer_BulletCnt;
	_uint			m_iLastWeapon_BulletCnt;
	_uint			m_iWeapon_BulletCnt = 50;
	_uint			m_iThrowCnt;
	_uint			m_iGoldCnt;
	_uint			m_iEmeraldCnt;
	_uint			m_iLastEmeraldCnt;
	_uint			m_iLastGoldCnt = 99;
	_uint			m_iLastThrowCnt = 99;


	COUNTUI			m_eType = CNT_END;
	CGameInstance*	m_pGameInstance = nullptr;
	vector<CUI*>	m_vecCountUI;
	WEAPONTYPE		m_eWeaponType = WEAPONTYPE_END;

	_float2			m_vPlayerEmeraldCntPos = { 700.f, 640.f };
	_float2			m_vPlayerGoldCntPos = { 1005.f, 640.f };
	_float2			m_vPlayerThrowCntPos = { 683.f, -447.f };
	_float2			m_vWeaponBulletCntPos = { 1055.f, -315.f };
	_float2			m_vPlayerBulletCntPos = { 1161.f, -315.f };

	_float4			m_vGoldNumColor = { 1.f,1.f,1.f,1.f };
	_float4			m_vThrowNumColor = { 1.f,1.f,1.f,1.f };
	_float4			m_vPistolNumColor = { 1.f, 0.95f, 0.68f, 0.9f };
	_float4			m_vRifleNumColor = { 0.18f, 0.89f, 0.25f, 0.9f };
	_float4			m_vInjectorNumColor = { 0.18f, 0.85f, 1.f, 0.9f };


	_float2			m_vBulletNumberSize = { 36.f, 36.f };
	_float2			m_vGoldNumberSize = { 28.f, 28.f };
	_float2			m_vThrowNumberSize = { 26.f, 26.f };

public:
	static CPlayer_UI_CountMachine*	Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(const wstring& wstrPrototypeTag, void* pArg = nullptr) override;
	virtual void Free() override;
};
END