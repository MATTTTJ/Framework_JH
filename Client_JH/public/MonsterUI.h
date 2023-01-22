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
	_float			m_fFixProgress = 1.f;

private:
	HRESULT			SetUp_Component();
	HRESULT			SetUp_ShaderResources();

public:
	static CMonsterUI*	Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(const wstring& wstrPrototypeTag, void* pArg = nullptr) override;
	virtual void Free() override;
};

// Base_Elite
class CMonsterUI_Elite final : public CUI
{
public:
private:
	CMonsterUI_Elite(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CMonsterUI_Elite(const CMonsterUI_Elite& rhs);
	virtual ~CMonsterUI_Elite() = default;

protected:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize_Clone(const wstring& wstrPrototypeTag, void* pArg) override;
	virtual void	Tick(_double dTimeDelta) override;
	virtual void	Late_Tick(_double dTimeDelta) override;
	virtual HRESULT Render() override;

private:
	_float			m_fFixProgress = 1.f;

private:
	HRESULT			SetUp_Component();
	HRESULT			SetUp_ShaderResources();

public:
	static CMonsterUI_Elite*	Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject*		Clone(const wstring& wstrPrototypeTag, void* pArg = nullptr) override;
	virtual void				Free() override;
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
			void	Ready_Child();
private:
	HRESULT			SetUp_Component();
	HRESULT			SetUp_ShaderResources();
	_int			m_iCurHP;
	_int			m_iLastHP;
	_float			m_fDist = 1.f;

private:
	CUI*			m_pWhiteUI = nullptr;

public:
	static CMonsterUI_HP_Red*	Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(const wstring& wstrPrototypeTag, void* pArg = nullptr) override;
	virtual void Free() override;
};
//White
class CMonsterUI_HP_White final : public CUI
{
	friend class CMonsterUI_HP_Red;
public:

private:
	CMonsterUI_HP_White(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CMonsterUI_HP_White(const CMonsterUI_HP_White& rhs);
	virtual ~CMonsterUI_HP_White() = default;

protected:
	void			Set_Progress_Test(_float fProgress) { m_fProgress = fProgress; }
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize_Clone(const wstring& wstrPrototypeTag, void* pArg) override;
	virtual void	Tick(_double dTimeDelta) override;
	virtual void	Late_Tick(_double dTimeDelta) override;
	virtual HRESULT Render() override;

private:
	HRESULT			SetUp_Component();
	HRESULT			SetUp_ShaderResources();

public:
	static CMonsterUI_HP_White*	Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(const wstring& wstrPrototypeTag, void* pArg = nullptr) override;
	virtual void Free() override;
};


class CBoss_UI_Base final : public CUI
{
private:
	CBoss_UI_Base(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CBoss_UI_Base(const CBoss_UI_Base& rhs);
	virtual ~CBoss_UI_Base() = default;

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
	static CBoss_UI_Base*	Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(const wstring& wstrPrototypeTag, void* pArg = nullptr) override;
	virtual void Free() override;
};

class CBoss_UI_Red final : public CUI
{
private:
	CBoss_UI_Red(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CBoss_UI_Red(const CBoss_UI_Red& rhs);
	virtual ~CBoss_UI_Red() = default;

protected:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize_Clone(const wstring& wstrPrototypeTag, void* pArg) override;
	virtual void	Tick(_double dTimeDelta) override;
	virtual void	Late_Tick(_double dTimeDelta) override;
	virtual HRESULT Render() override;
	void			Ready_Child();

private:
	HRESULT			SetUp_Component();
	HRESULT			SetUp_ShaderResources();

private:
	_int			m_iCurHP;
	_int			m_iLastHP;
	_float			m_fDist = 1.f;

private:
	CUI*			m_pWhiteUI = nullptr;

public:
	static CBoss_UI_Red*	Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(const wstring& wstrPrototypeTag, void* pArg = nullptr) override;
	virtual void Free() override;
};

class CBoss_UI_White final : public CUI
{
	friend class CBoss_UI_Red;

private:
	CBoss_UI_White(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CBoss_UI_White(const CBoss_UI_White& rhs);
	virtual ~CBoss_UI_White() = default;

protected:
	void			Set_Progress_Test(_float fProgress) { m_fProgress = fProgress; }
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize_Clone(const wstring& wstrPrototypeTag, void* pArg) override;
	virtual void	Tick(_double dTimeDelta) override;
	virtual void	Late_Tick(_double dTimeDelta) override;
	virtual HRESULT Render() override;

private:
	HRESULT			SetUp_Component();
	HRESULT			SetUp_ShaderResources();
	_float			m_fDist = 0.f;
public:
	static CBoss_UI_White*	Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(const wstring& wstrPrototypeTag, void* pArg = nullptr) override;
	virtual void Free() override;
};
















END