#pragma once
#include "Client_Defines.h"
#include "UI.h"
#include "Boss_Golem_State.h"

BEGIN(Client)
class CFadeInOut final : public CUI
{
public:
	friend CBoss_Golem_State;

public:
	CFadeInOut(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CFadeInOut(const CFadeInOut& rhs);
	virtual ~CFadeInOut() = default;

protected:
	void			Fade_Out(_bool	bFadeOutIstrue) { m_bFadeOut = bFadeOutIstrue; }
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize_Clone(const wstring& wstrPrototypeTag, void* pArg) override;
	virtual void	Tick(_double dTimeDelta) override;
	virtual void	Late_Tick(_double dTimeDelta) override;
	virtual HRESULT Render() override;

private:
	HRESULT			SetUp_Component();
	HRESULT			SetUp_ShaderResources();

private:
	_bool			m_bFadeOut = false;
	_float			m_fFadeAlpha = 0.f;

	FADEDESC		m_tFadeDesc;

public:
	static CFadeInOut*	Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(const wstring& wstrPrototypeTag, void* pArg = nullptr) override;
	virtual void Free() override;
};
END