#include "stdafx.h"
#include "..\public\PlayerUI_Base.h"
#include "GameInstance.h"
#include "Player.h"

CPlayerUI_Base::CPlayerUI_Base(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUI(pDevice, pContext)
{
}

CPlayerUI_Base::CPlayerUI_Base(const CPlayerUI_Base& rhs)
	: CUI(rhs)
{
}

HRESULT CPlayerUI_Base::Initialize_Prototype()
{
	FAILED_CHECK_RETURN(__super::Initialize_Prototype(), E_FAIL);

	return S_OK;
}

HRESULT CPlayerUI_Base::Initialize_Clone(const wstring& wstrPrototypeTag, void* pArg)
{
	m_fSizeX = 512.f;
	m_fSizeY = 512.f;
	m_fX = m_fSizeX * 0.5f;
	m_fY = m_fSizeY * 0.5f;

	FAILED_CHECK_RETURN(__super::Initialize_Clone(wstrPrototypeTag, pArg), E_FAIL);

	FAILED_CHECK_RETURN(SetUp_Component(), E_FAIL);

	m_pTransformCom->Set_Scaled(_float3(m_fSizeX, m_fSizeY, 1.f));
	// m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fX - m_fSizeX * 0.5f, -m_fY + m_fSizeY * 0.5f, 0.f, 1.f));
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(-850.f, -470.f, 0.f, 1.f));

	return S_OK;
}

void CPlayerUI_Base::Tick(_double dTimeDelta)
{
	__super::Tick(dTimeDelta);
}

void CPlayerUI_Base::Late_Tick(_double dTimeDelta)
{
	__super::Late_Tick(dTimeDelta);

	if (nullptr != m_pRendererCom)
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);
}

HRESULT CPlayerUI_Base::Render()
{
	FAILED_CHECK_RETURN(__super::Render(), E_FAIL);

	FAILED_CHECK_RETURN(SetUp_ShaderResources(), E_FAIL);

	m_pShaderCom->Begin(1);

	m_pVIBufferCom->Render();

	return S_OK;
}

HRESULT CPlayerUI_Base::SetUp_Component()
{
	FAILED_CHECK_RETURN(__super::Add_Component(CGameInstance::Get_StaticLevelIndex(), L"Prototype_Component_Renderer", L"Com_Renderer", (CComponent**)&m_pRendererCom, this), E_FAIL);
	FAILED_CHECK_RETURN(__super::Add_Component(CGameInstance::Get_StaticLevelIndex(), L"Prototype_Component_Shader_VtxTex", L"Com_Shader", (CComponent**)&m_pShaderCom, this), E_FAIL);
	FAILED_CHECK_RETURN(__super::Add_Component(CGameInstance::Get_StaticLevelIndex(), L"Prototype_Component_VIBuffer_Rect", L"Com_VIBuffer", (CComponent**)&m_pVIBufferCom, this), E_FAIL);
	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Texture_UI_PlayerInfo", L"Com_Texture", (CComponent**)&m_pTextureCom, this), E_FAIL);

	return S_OK;
}

HRESULT CPlayerUI_Base::SetUp_ShaderResources()
{
	NULL_CHECK_RETURN(m_pShaderCom, E_FAIL);

	m_pTransformCom->Bind_ShaderResource(m_pShaderCom, L"g_WorldMatrix");
	m_pShaderCom->Set_Matrix(L"g_ViewMatrix", &m_ViewMatrix);
	m_pShaderCom->Set_Matrix(L"g_ProjMatrix", &m_ProjMatrix);
	m_pTextureCom->Bind_ShaderResource(m_pShaderCom, L"g_Texture");

	return S_OK;
}

CPlayerUI_Base* CPlayerUI_Base::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CPlayerUI_Base*	pInstance = new CPlayerUI_Base(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Create : CLogo");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CPlayerUI_Base::Clone(const wstring& wstrPrototypeTag, void* pArg)
{
	CPlayerUI_Base*	pInstance = new CPlayerUI_Base(*this);

	if (FAILED(pInstance->Initialize_Clone(wstrPrototypeTag, pArg)))
	{
		MSG_BOX("Failed to Create : CLogo");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CPlayerUI_Base::Free()
{
	__super::Free();
}

// Enter Tex


CPlayerUI_Enter::CPlayerUI_Enter(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUI(pDevice, pContext)
{
}

CPlayerUI_Enter::CPlayerUI_Enter(const CPlayerUI_Enter& rhs)
	: CUI(rhs)
{
}

HRESULT CPlayerUI_Enter::Initialize_Prototype()
{
	FAILED_CHECK_RETURN(__super::Initialize_Prototype(), E_FAIL);

	return S_OK;
}

HRESULT CPlayerUI_Enter::Initialize_Clone(const wstring& wstrPrototypeTag, void* pArg)
{
	m_fSizeX = 256.f;
	m_fSizeY = 256.f;
	m_fX = m_fSizeX * 0.5f;
	m_fY = m_fSizeY * 0.5f;

	FAILED_CHECK_RETURN(__super::Initialize_Clone(wstrPrototypeTag, pArg), E_FAIL);

	FAILED_CHECK_RETURN(SetUp_Component(), E_FAIL);

	m_pTransformCom->Set_Scaled(_float3(m_fSizeX, m_fSizeY, 1.f));
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(200.f, -150.f, 0.f, 1.f));
	return S_OK;
}

void CPlayerUI_Enter::Tick(_double dTimeDelta)
{
	__super::Tick(dTimeDelta);
}

void CPlayerUI_Enter::Late_Tick(_double dTimeDelta)
{
	__super::Late_Tick(dTimeDelta);

	if (nullptr != m_pRendererCom)
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);
}

HRESULT CPlayerUI_Enter::Render()
{
	if (m_pOwner != nullptr)
	{
		if (dynamic_cast<CPlayer*>(m_pOwner)->Get_PortalUI() == true)
		{
			FAILED_CHECK_RETURN(__super::Render(), E_FAIL);

			FAILED_CHECK_RETURN(SetUp_ShaderResources(), E_FAIL);

			m_pShaderCom->Begin(1);

			m_pVIBufferCom->Render();

			return S_OK;
		}
		else
			return S_OK;
	}
	else
		return S_OK;
}

HRESULT CPlayerUI_Enter::SetUp_Component()
{
	FAILED_CHECK_RETURN(__super::Add_Component(CGameInstance::Get_StaticLevelIndex(), L"Prototype_Component_Renderer", L"Com_Renderer", (CComponent**)&m_pRendererCom, this), E_FAIL);
	FAILED_CHECK_RETURN(__super::Add_Component(CGameInstance::Get_StaticLevelIndex(), L"Prototype_Component_Shader_VtxTex", L"Com_Shader", (CComponent**)&m_pShaderCom, this), E_FAIL);
	FAILED_CHECK_RETURN(__super::Add_Component(CGameInstance::Get_StaticLevelIndex(), L"Prototype_Component_VIBuffer_Rect", L"Com_VIBuffer", (CComponent**)&m_pVIBufferCom, this), E_FAIL);
	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Texture_Enter", L"Com_Texture", (CComponent**)&m_pTextureCom, this), E_FAIL);

	return S_OK;
}

HRESULT CPlayerUI_Enter::SetUp_ShaderResources()
{
	NULL_CHECK_RETURN(m_pShaderCom, E_FAIL);

	m_pTransformCom->Bind_ShaderResource(m_pShaderCom, L"g_WorldMatrix");
	m_pShaderCom->Set_Matrix(L"g_ViewMatrix", &m_ViewMatrix);
	m_pShaderCom->Set_Matrix(L"g_ProjMatrix", &m_ProjMatrix);
	m_pTextureCom->Bind_ShaderResource(m_pShaderCom, L"g_Texture");

	return S_OK;
}

CPlayerUI_Enter* CPlayerUI_Enter::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CPlayerUI_Enter*	pInstance = new CPlayerUI_Enter(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Create : CPlayerUI_Enter");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CPlayerUI_Enter::Clone(const wstring& wstrPrototypeTag, void* pArg)
{
	CPlayerUI_Enter*	pInstance = new CPlayerUI_Enter(*this);

	if (FAILED(pInstance->Initialize_Clone(wstrPrototypeTag, pArg)))
	{
		MSG_BOX("Failed to Create : CPlayerUI_Enter");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CPlayerUI_Enter::Free()
{
	__super::Free();
}





// Player_HP


CPlayerUI_Hp_Red::CPlayerUI_Hp_Red(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUI(pDevice, pContext)
{
}

CPlayerUI_Hp_Red::CPlayerUI_Hp_Red(const CPlayerUI_Hp_Red& rhs)
	: CUI(rhs)
{
}

HRESULT CPlayerUI_Hp_Red::Initialize_Prototype()
{
	FAILED_CHECK_RETURN(__super::Initialize_Prototype(), E_FAIL);

	return S_OK;
}

HRESULT CPlayerUI_Hp_Red::Initialize_Clone(const wstring& wstrPrototypeTag, void* pArg)
{
	m_fSizeX = 274.f;
	m_fSizeY = 512.f;
	m_fX = m_fSizeX * 0.5f;
	m_fY = m_fSizeY * 0.5f;

	FAILED_CHECK_RETURN(__super::Initialize_Clone(wstrPrototypeTag, pArg), E_FAIL);

	FAILED_CHECK_RETURN(SetUp_Component(), E_FAIL);

	m_pTransformCom->Set_Scaled(_float3(m_fSizeX, m_fSizeY, 1.f));
	// m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fX - m_fSizeX * 0.5f, -m_fY + m_fSizeY * 0.5f, 0.f, 1.f));
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(-784.f, -470.f, 0.f, 1.f));
	
	m_fProgress = 1.f;
	
	return S_OK;
}

void CPlayerUI_Hp_Red::Tick(_double dTimeDelta)
{
	__super::Tick(dTimeDelta);
	
	
		m_PlayerOption = dynamic_cast<CPlayer*>(m_pOwner)->Get_PlayerStat();
		m_iCurHP = m_PlayerOption.m_iHp;
		if (m_iCurHP != m_iLastHP)
		{
			SetProgress(m_PlayerOption.m_iHp, m_PlayerOption.m_iMaxHp);
			m_fDist = ((m_fSizeX * (1 - m_fProgress)) * 0.5f);
			m_iLastHP = m_iCurHP;
		}

		m_pTransformCom->Set_Scaled(_float3(m_fSizeX * m_fProgress, m_fSizeY, 1.f));
		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(-784.f - m_fDist, -470.f, 0.f, 1.f));

}

void CPlayerUI_Hp_Red::Late_Tick(_double dTimeDelta)
{
	__super::Late_Tick(dTimeDelta);

	if (nullptr != m_pRendererCom)
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);
}

HRESULT CPlayerUI_Hp_Red::Render()
{
	FAILED_CHECK_RETURN(__super::Render(), E_FAIL);

	FAILED_CHECK_RETURN(SetUp_ShaderResources(), E_FAIL);

	m_pShaderCom->Begin(1);

	m_pVIBufferCom->Render();

	return S_OK;
}

HRESULT CPlayerUI_Hp_Red::SetUp_Component()
{
	FAILED_CHECK_RETURN(__super::Add_Component(CGameInstance::Get_StaticLevelIndex(), L"Prototype_Component_Renderer", L"Com_Renderer", (CComponent**)&m_pRendererCom, this), E_FAIL);
	FAILED_CHECK_RETURN(__super::Add_Component(CGameInstance::Get_StaticLevelIndex(), L"Prototype_Component_Shader_VtxTex", L"Com_Shader", (CComponent**)&m_pShaderCom, this), E_FAIL);
	FAILED_CHECK_RETURN(__super::Add_Component(CGameInstance::Get_StaticLevelIndex(), L"Prototype_Component_VIBuffer_Rect", L"Com_VIBuffer", (CComponent**)&m_pVIBufferCom, this), E_FAIL);
	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Texture_UI_PlayerHp", L"Com_Texture", (CComponent**)&m_pTextureCom, this), E_FAIL);

	return S_OK;
}

HRESULT CPlayerUI_Hp_Red::SetUp_ShaderResources()
{
	NULL_CHECK_RETURN(m_pShaderCom, E_FAIL);

	m_pTransformCom->Bind_ShaderResource(m_pShaderCom, L"g_WorldMatrix");
	m_pShaderCom->Set_Matrix(L"g_ViewMatrix", &m_ViewMatrix);
	m_pShaderCom->Set_Matrix(L"g_ProjMatrix", &m_ProjMatrix);
	m_pTextureCom->Bind_ShaderResource(m_pShaderCom, L"g_Texture");
	// m_pShaderCom->Set_RawValue(L"g_fTime", &m_fTime, sizeof(_float));

	// m_fTime
	return S_OK;
}

CPlayerUI_Hp_Red* CPlayerUI_Hp_Red::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CPlayerUI_Hp_Red*	pInstance = new CPlayerUI_Hp_Red(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Create : CPlayerUI_Hp_Red");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CPlayerUI_Hp_Red::Clone(const wstring& wstrPrototypeTag, void* pArg)
{
	CPlayerUI_Hp_Red*	pInstance = new CPlayerUI_Hp_Red(*this);

	if (FAILED(pInstance->Initialize_Clone(wstrPrototypeTag, pArg)))
	{
		MSG_BOX("Failed to Create : CPlayerUI_Hp_Red");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CPlayerUI_Hp_Red::Free()
{
	__super::Free();
}

// Player Shield

CPlayerUI_Hp_Shield::CPlayerUI_Hp_Shield(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUI(pDevice, pContext)
{
}

CPlayerUI_Hp_Shield::CPlayerUI_Hp_Shield(const CPlayerUI_Hp_Shield& rhs)
	: CUI(rhs)
{
}

HRESULT CPlayerUI_Hp_Shield::Initialize_Prototype()
{
	FAILED_CHECK_RETURN(__super::Initialize_Prototype(), E_FAIL);

	return S_OK;
}

HRESULT CPlayerUI_Hp_Shield::Initialize_Clone(const wstring& wstrPrototypeTag, void* pArg)
{
	m_fSizeX = 270.f;
	m_fSizeY = 512.f;
	m_fX = m_fSizeX * 0.5f;
	m_fY = m_fSizeY * 0.5f;

	FAILED_CHECK_RETURN(__super::Initialize_Clone(wstrPrototypeTag, pArg), E_FAIL);

	FAILED_CHECK_RETURN(SetUp_Component(), E_FAIL);

	m_pTransformCom->Set_Scaled(_float3(m_fSizeX, m_fSizeY, 1.f));
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(-779.f, -470.f, 0.f, 1.f));
	m_fProgress = 1.f;
	return S_OK;
}

void CPlayerUI_Hp_Shield::Tick(_double dTimeDelta)
{
	__super::Tick(dTimeDelta);

	m_PlayerOption = dynamic_cast<CPlayer*>(m_pOwner)->Get_PlayerStat();
	m_iCurHP = m_PlayerOption.m_iShieldPoint;

	if (m_iCurHP != m_iLastHP)
	{
		SetProgress(m_PlayerOption.m_iShieldPoint, m_PlayerOption.m_iMaxShieldPoint);
		m_fDist = ((m_fSizeX * (1 - m_fProgress)) * 0.5f);
		m_iLastHP = m_iCurHP;
	}

	m_pTransformCom->Set_Scaled(_float3(m_fSizeX * m_fProgress, m_fSizeY, 1.f));
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(-779.f - m_fDist, -470.f, 0.f, 1.f));
}

void CPlayerUI_Hp_Shield::Late_Tick(_double dTimeDelta)
{
	__super::Late_Tick(dTimeDelta);

	if (nullptr != m_pRendererCom)
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);
}

HRESULT CPlayerUI_Hp_Shield::Render()
{
	FAILED_CHECK_RETURN(__super::Render(), E_FAIL);

	FAILED_CHECK_RETURN(SetUp_ShaderResources(), E_FAIL);

	m_pShaderCom->Begin(1);

	m_pVIBufferCom->Render();

	return S_OK;
}

HRESULT CPlayerUI_Hp_Shield::SetUp_Component()
{
	FAILED_CHECK_RETURN(__super::Add_Component(CGameInstance::Get_StaticLevelIndex(), L"Prototype_Component_Renderer", L"Com_Renderer", (CComponent**)&m_pRendererCom, this), E_FAIL);
	FAILED_CHECK_RETURN(__super::Add_Component(CGameInstance::Get_StaticLevelIndex(), L"Prototype_Component_Shader_VtxTex", L"Com_Shader", (CComponent**)&m_pShaderCom, this), E_FAIL);
	FAILED_CHECK_RETURN(__super::Add_Component(CGameInstance::Get_StaticLevelIndex(), L"Prototype_Component_VIBuffer_Rect", L"Com_VIBuffer", (CComponent**)&m_pVIBufferCom, this), E_FAIL);
	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Texture_UI_PlayerShield", L"Com_Texture", (CComponent**)&m_pTextureCom, this), E_FAIL);

	return S_OK;
}

HRESULT CPlayerUI_Hp_Shield::SetUp_ShaderResources()
{
	NULL_CHECK_RETURN(m_pShaderCom, E_FAIL);

	m_pTransformCom->Bind_ShaderResource(m_pShaderCom, L"g_WorldMatrix");
	m_pShaderCom->Set_Matrix(L"g_ViewMatrix", &m_ViewMatrix);
	m_pShaderCom->Set_Matrix(L"g_ProjMatrix", &m_ProjMatrix);
	m_pTextureCom->Bind_ShaderResource(m_pShaderCom, L"g_Texture");
	// m_pShaderCom->Set_RawValue(L"g_fTime", &m_fTime, sizeof(_float));

	// m_fTime
	return S_OK;
}

CPlayerUI_Hp_Shield* CPlayerUI_Hp_Shield::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CPlayerUI_Hp_Shield*	pInstance = new CPlayerUI_Hp_Shield(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Create : CPlayerUI_Hp_Shield");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CPlayerUI_Hp_Shield::Clone(const wstring& wstrPrototypeTag, void* pArg)
{
	CPlayerUI_Hp_Shield*	pInstance = new CPlayerUI_Hp_Shield(*this);

	if (FAILED(pInstance->Initialize_Clone(wstrPrototypeTag, pArg)))
	{
		MSG_BOX("Failed to Create : CPlayerUI_Hp_Shield");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CPlayerUI_Hp_Shield::Free()
{
	__super::Free();
}


// Player Skill

CPlayerUI_Skill::CPlayerUI_Skill(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUI(pDevice, pContext)
{
}

CPlayerUI_Skill::CPlayerUI_Skill(const CPlayerUI_Skill& rhs)
	: CUI(rhs)
{
}

HRESULT CPlayerUI_Skill::Initialize_Prototype()
{
	FAILED_CHECK_RETURN(__super::Initialize_Prototype(), E_FAIL);

	return S_OK;
}

HRESULT CPlayerUI_Skill::Initialize_Clone(const wstring& wstrPrototypeTag, void* pArg)
{
	m_fSizeX = 256.f;
	m_fSizeY = 256.f;
	m_fX = m_fSizeX * 0.5f;
	m_fY = m_fSizeY * 0.5f;

	FAILED_CHECK_RETURN(__super::Initialize_Clone(wstrPrototypeTag, pArg), E_FAIL);

	FAILED_CHECK_RETURN(SetUp_Component(), E_FAIL);

	m_pTransformCom->Set_Scaled(_float3(m_fSizeX, m_fSizeX, 1.f));
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fX - m_fSizeX * 0.5f, -470.f, 0.f, 1.f));
	// m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(-850.f, -470.f, 0.f, 1.f));
	m_fGlowStrength = 0.f;
	return S_OK;
}

void CPlayerUI_Skill::Tick(_double dTimeDelta)
{
	__super::Tick(dTimeDelta);

	if (m_fGlowStrength >= 1.f)
		m_bIsChange = true;
	else if(m_fGlowStrength <= 0)
		m_bIsChange = false;                                                                                                                                                                                 

	if (m_bIsChange == true)
		m_fGlowStrength += (_float)dTimeDelta * -1.f;
	else
		m_fGlowStrength += (_float)dTimeDelta;
}

void CPlayerUI_Skill::Late_Tick(_double dTimeDelta)
{
	__super::Late_Tick(dTimeDelta);

	if (nullptr != m_pRendererCom)
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);
}

HRESULT CPlayerUI_Skill::Render()
{
	FAILED_CHECK_RETURN(__super::Render(), E_FAIL);

	FAILED_CHECK_RETURN(SetUp_ShaderResources(), E_FAIL);

	m_pShaderCom->Begin(5);

	m_pVIBufferCom->Render();

	return S_OK;
}

HRESULT CPlayerUI_Skill::SetUp_Component()
{
	FAILED_CHECK_RETURN(__super::Add_Component(CGameInstance::Get_StaticLevelIndex(), L"Prototype_Component_Renderer", L"Com_Renderer", (CComponent**)&m_pRendererCom, this), E_FAIL);
	FAILED_CHECK_RETURN(__super::Add_Component(CGameInstance::Get_StaticLevelIndex(), L"Prototype_Component_Shader_VtxTex", L"Com_Shader", (CComponent**)&m_pShaderCom, this), E_FAIL);
	FAILED_CHECK_RETURN(__super::Add_Component(CGameInstance::Get_StaticLevelIndex(), L"Prototype_Component_VIBuffer_Rect", L"Com_VIBuffer", (CComponent**)&m_pVIBufferCom, this), E_FAIL);
	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Texture_UI_PlayerSkillOn", L"Com_OnTexture", (CComponent**)&m_pOnTextureCom, this), E_FAIL);
	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Texture_UI_PlayerSkillOff", L"Com_OffTexture", (CComponent**)&m_pOffTextureCom, this), E_FAIL);
	
	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Texture_UI_PlayerSkillGlow", L"Com_GlowTexture", (CComponent**)&m_pGlowTextureCom, this), E_FAIL);

	return S_OK;

}

HRESULT CPlayerUI_Skill::SetUp_ShaderResources()
{
	NULL_CHECK_RETURN(m_pShaderCom, E_FAIL);

	m_pTransformCom->Bind_ShaderResource(m_pShaderCom, L"g_WorldMatrix");
	m_pShaderCom->Set_Matrix(L"g_ViewMatrix", &m_ViewMatrix);
	m_pShaderCom->Set_Matrix(L"g_ProjMatrix", &m_ProjMatrix);
	m_pShaderCom->Set_RawValue(L"g_glowStrength", &m_fGlowStrength, sizeof(_float));

	m_pOnTextureCom->Bind_ShaderResource(m_pShaderCom, L"g_Texture");
	m_pGlowTextureCom->Bind_ShaderResource(m_pShaderCom, L"g_SkillGlowTexture");

	return S_OK;
}

CPlayerUI_Skill* CPlayerUI_Skill::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CPlayerUI_Skill*	pInstance = new CPlayerUI_Skill(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Create : CLogo");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CPlayerUI_Skill::Clone(const wstring& wstrPrototypeTag, void* pArg)
{
	CPlayerUI_Skill*	pInstance = new CPlayerUI_Skill(*this);

	if (FAILED(pInstance->Initialize_Clone(wstrPrototypeTag, pArg)))
	{
		MSG_BOX("Failed to Create : CLogo");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CPlayerUI_Skill::Free()
{
	__super::Free();
}

// Player Gold Texture

CPlayerUI_Gold::CPlayerUI_Gold(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUI(pDevice, pContext)
{
}

CPlayerUI_Gold::CPlayerUI_Gold(const CPlayerUI_Gold& rhs)
	: CUI(rhs)
{
}

HRESULT CPlayerUI_Gold::Initialize_Prototype()
{
	FAILED_CHECK_RETURN(__super::Initialize_Prototype(), E_FAIL);

	return S_OK;
}

HRESULT CPlayerUI_Gold::Initialize_Clone(const wstring& wstrPrototypeTag, void* pArg)
{
	m_fSizeX = 80.f;
	m_fSizeY = 80.f;
	m_fX = m_fSizeX * 0.5f;
	m_fY = m_fSizeY * 0.5f;

	FAILED_CHECK_RETURN(__super::Initialize_Clone(wstrPrototypeTag, pArg), E_FAIL);

	FAILED_CHECK_RETURN(SetUp_Component(), E_FAIL);

	m_pTransformCom->Set_Scaled(_float3(m_fSizeX, m_fSizeX, 1.f));
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(950.f, 640.f, 0.f, 1.f));
	// m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(-850.f, -470.f, 0.f, 1.f));

	return S_OK;
}

void CPlayerUI_Gold::Tick(_double dTimeDelta)
{
	__super::Tick(dTimeDelta);
}

void CPlayerUI_Gold::Late_Tick(_double dTimeDelta)
{
	__super::Late_Tick(dTimeDelta);

	if (nullptr != m_pRendererCom)
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);
}

HRESULT CPlayerUI_Gold::Render()
{
	FAILED_CHECK_RETURN(__super::Render(), E_FAIL);

	FAILED_CHECK_RETURN(SetUp_ShaderResources(), E_FAIL);

	m_pShaderCom->Begin(1);

	m_pVIBufferCom->Render();

	return S_OK;
}

HRESULT CPlayerUI_Gold::SetUp_Component()
{
	FAILED_CHECK_RETURN(__super::Add_Component(CGameInstance::Get_StaticLevelIndex(), L"Prototype_Component_Renderer", L"Com_Renderer", (CComponent**)&m_pRendererCom, this), E_FAIL);
	FAILED_CHECK_RETURN(__super::Add_Component(CGameInstance::Get_StaticLevelIndex(), L"Prototype_Component_Shader_VtxTex", L"Com_Shader", (CComponent**)&m_pShaderCom, this), E_FAIL);
	FAILED_CHECK_RETURN(__super::Add_Component(CGameInstance::Get_StaticLevelIndex(), L"Prototype_Component_VIBuffer_Rect", L"Com_VIBuffer", (CComponent**)&m_pVIBufferCom, this), E_FAIL);
	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Texture_Gold", L"Com_Texture", (CComponent**)&m_pTextureCom, this), E_FAIL);

	return S_OK;
}

HRESULT CPlayerUI_Gold::SetUp_ShaderResources()
{
	NULL_CHECK_RETURN(m_pShaderCom, E_FAIL);

	m_pTransformCom->Bind_ShaderResource(m_pShaderCom, L"g_WorldMatrix");
	m_pShaderCom->Set_Matrix(L"g_ViewMatrix", &m_ViewMatrix);
	m_pShaderCom->Set_Matrix(L"g_ProjMatrix", &m_ProjMatrix);
	m_pTextureCom->Bind_ShaderResource(m_pShaderCom, L"g_Texture");

	return S_OK;
}

CPlayerUI_Gold* CPlayerUI_Gold::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CPlayerUI_Gold*	pInstance = new CPlayerUI_Gold(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Create : CLogo");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CPlayerUI_Gold::Clone(const wstring& wstrPrototypeTag, void* pArg)
{
	CPlayerUI_Gold*	pInstance = new CPlayerUI_Gold(*this);

	if (FAILED(pInstance->Initialize_Clone(wstrPrototypeTag, pArg)))
	{
		MSG_BOX("Failed to Create : CLogo");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CPlayerUI_Gold::Free()
{
	__super::Free();
}

// Player Skill Base

CPlayerUI_Skill_Base::CPlayerUI_Skill_Base(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUI(pDevice, pContext)
{
}

CPlayerUI_Skill_Base::CPlayerUI_Skill_Base(const CPlayerUI_Skill_Base& rhs)
	: CUI(rhs)
{
}

HRESULT CPlayerUI_Skill_Base::Initialize_Prototype()
{
	FAILED_CHECK_RETURN(__super::Initialize_Prototype(), E_FAIL);

	return S_OK;
}

HRESULT CPlayerUI_Skill_Base::Initialize_Clone(const wstring& wstrPrototypeTag, void* pArg)
{
	m_fSizeX = 256.f;
	m_fSizeY = 256.f;
	m_fX = m_fSizeX * 0.5f;
	m_fY = m_fSizeY * 0.5f;

	FAILED_CHECK_RETURN(__super::Initialize_Clone(wstrPrototypeTag, pArg), E_FAIL);

	FAILED_CHECK_RETURN(SetUp_Component(), E_FAIL);

	m_pTransformCom->Set_Scaled(_float3(m_fSizeX, m_fSizeX, 1.f));
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fX - m_fSizeX * 0.5f, -470.f, 0.f, 1.f));
	// m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(-850.f, -470.f, 0.f, 1.f));

	return S_OK;
}

void CPlayerUI_Skill_Base::Tick(_double dTimeDelta)
{
	__super::Tick(dTimeDelta);
}

void CPlayerUI_Skill_Base::Late_Tick(_double dTimeDelta)
{
	__super::Late_Tick(dTimeDelta);

	if (nullptr != m_pRendererCom)
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);
}

HRESULT CPlayerUI_Skill_Base::Render()
{
	FAILED_CHECK_RETURN(__super::Render(), E_FAIL);

	FAILED_CHECK_RETURN(SetUp_ShaderResources(), E_FAIL);

	m_pShaderCom->Begin(1);

	m_pVIBufferCom->Render();

	return S_OK;
}

HRESULT CPlayerUI_Skill_Base::SetUp_Component()
{
	FAILED_CHECK_RETURN(__super::Add_Component(CGameInstance::Get_StaticLevelIndex(), L"Prototype_Component_Renderer", L"Com_Renderer", (CComponent**)&m_pRendererCom, this), E_FAIL);
	FAILED_CHECK_RETURN(__super::Add_Component(CGameInstance::Get_StaticLevelIndex(), L"Prototype_Component_Shader_VtxTex", L"Com_Shader", (CComponent**)&m_pShaderCom, this), E_FAIL);
	FAILED_CHECK_RETURN(__super::Add_Component(CGameInstance::Get_StaticLevelIndex(), L"Prototype_Component_VIBuffer_Rect", L"Com_VIBuffer", (CComponent**)&m_pVIBufferCom, this), E_FAIL);
	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Texture_UI_PlayerSkill_Base", L"Com_Texture", (CComponent**)&m_pTextureCom, this), E_FAIL);

	return S_OK;
}

HRESULT CPlayerUI_Skill_Base::SetUp_ShaderResources()
{
	NULL_CHECK_RETURN(m_pShaderCom, E_FAIL);

	m_pTransformCom->Bind_ShaderResource(m_pShaderCom, L"g_WorldMatrix");
	m_pShaderCom->Set_Matrix(L"g_ViewMatrix", &m_ViewMatrix);
	m_pShaderCom->Set_Matrix(L"g_ProjMatrix", &m_ProjMatrix);
	m_pTextureCom->Bind_ShaderResource(m_pShaderCom, L"g_Texture");

	return S_OK;
}

CPlayerUI_Skill_Base* CPlayerUI_Skill_Base::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CPlayerUI_Skill_Base*	pInstance = new CPlayerUI_Skill_Base(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Create : CLogo");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CPlayerUI_Skill_Base::Clone(const wstring& wstrPrototypeTag, void* pArg)
{
	CPlayerUI_Skill_Base*	pInstance = new CPlayerUI_Skill_Base(*this);

	if (FAILED(pInstance->Initialize_Clone(wstrPrototypeTag, pArg)))
	{
		MSG_BOX("Failed to Create : CLogo");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CPlayerUI_Skill_Base::Free()
{
	__super::Free();
}


// Player Emerald

CPlayerUI_Emerald::CPlayerUI_Emerald(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUI(pDevice, pContext)
{
}

CPlayerUI_Emerald::CPlayerUI_Emerald(const CPlayerUI_Emerald& rhs)
	: CUI(rhs)
{
}

HRESULT CPlayerUI_Emerald::Initialize_Prototype()
{
	FAILED_CHECK_RETURN(__super::Initialize_Prototype(), E_FAIL);

	return S_OK;
}

HRESULT CPlayerUI_Emerald::Initialize_Clone(const wstring& wstrPrototypeTag, void* pArg)
{
	m_fSizeX = 80.f;
	m_fSizeY = 80.f;
	m_fX = m_fSizeX * 0.5f;
	m_fY = m_fSizeY * 0.5f;

	FAILED_CHECK_RETURN(__super::Initialize_Clone(wstrPrototypeTag, pArg), E_FAIL);

	FAILED_CHECK_RETURN(SetUp_Component(), E_FAIL);

	m_pTransformCom->Set_Scaled(_float3(m_fSizeX, m_fSizeX, 1.f));
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(770.f, 640.f, 0.f, 1.f));
	// m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(-850.f, -470.f, 0.f, 1.f));

	return S_OK;
}

void CPlayerUI_Emerald::Tick(_double dTimeDelta)
{
	__super::Tick(dTimeDelta);
}

void CPlayerUI_Emerald::Late_Tick(_double dTimeDelta)
{
	__super::Late_Tick(dTimeDelta);

	if (nullptr != m_pRendererCom)
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);
}

HRESULT CPlayerUI_Emerald::Render()
{
	FAILED_CHECK_RETURN(__super::Render(), E_FAIL);

	FAILED_CHECK_RETURN(SetUp_ShaderResources(), E_FAIL);

	m_pShaderCom->Begin(1);

	m_pVIBufferCom->Render();

	return S_OK;
}

HRESULT CPlayerUI_Emerald::SetUp_Component()
{
	FAILED_CHECK_RETURN(__super::Add_Component(CGameInstance::Get_StaticLevelIndex(), L"Prototype_Component_Renderer", L"Com_Renderer", (CComponent**)&m_pRendererCom, this), E_FAIL);
	FAILED_CHECK_RETURN(__super::Add_Component(CGameInstance::Get_StaticLevelIndex(), L"Prototype_Component_Shader_VtxTex", L"Com_Shader", (CComponent**)&m_pShaderCom, this), E_FAIL);
	FAILED_CHECK_RETURN(__super::Add_Component(CGameInstance::Get_StaticLevelIndex(), L"Prototype_Component_VIBuffer_Rect", L"Com_VIBuffer", (CComponent**)&m_pVIBufferCom, this), E_FAIL);
	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Texture_Emerald", L"Com_Texture", (CComponent**)&m_pTextureCom, this), E_FAIL);

	return S_OK;
}

HRESULT CPlayerUI_Emerald::SetUp_ShaderResources()
{
	NULL_CHECK_RETURN(m_pShaderCom, E_FAIL);

	m_pTransformCom->Bind_ShaderResource(m_pShaderCom, L"g_WorldMatrix");
	m_pShaderCom->Set_Matrix(L"g_ViewMatrix", &m_ViewMatrix);
	m_pShaderCom->Set_Matrix(L"g_ProjMatrix", &m_ProjMatrix);
	m_pTextureCom->Bind_ShaderResource(m_pShaderCom, L"g_Texture");

	return S_OK;
}

CPlayerUI_Emerald* CPlayerUI_Emerald::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CPlayerUI_Emerald*	pInstance = new CPlayerUI_Emerald(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Create : CLogo");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CPlayerUI_Emerald::Clone(const wstring& wstrPrototypeTag, void* pArg)
{
	CPlayerUI_Emerald*	pInstance = new CPlayerUI_Emerald(*this);

	if (FAILED(pInstance->Initialize_Clone(wstrPrototypeTag, pArg)))
	{
		MSG_BOX("Failed to Create : CLogo");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CPlayerUI_Emerald::Free()
{
	__super::Free();
}

// Player Throw

CPlayer_UI_Throw::CPlayer_UI_Throw(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUI(pDevice, pContext)
{
}

CPlayer_UI_Throw::CPlayer_UI_Throw(const CPlayer_UI_Throw& rhs)
	: CUI(rhs)
{
}

HRESULT CPlayer_UI_Throw::Initialize_Prototype()
{
	FAILED_CHECK_RETURN(__super::Initialize_Prototype(), E_FAIL);

	return S_OK;
}

HRESULT CPlayer_UI_Throw::Initialize_Clone(const wstring& wstrPrototypeTag, void* pArg)
{
	m_fSizeX = 128.f;
	m_fSizeY = 128.f;;
	m_fX = m_fSizeX * 0.5f;
	m_fY = m_fSizeY * 0.5f;

	FAILED_CHECK_RETURN(__super::Initialize_Clone(wstrPrototypeTag, pArg), E_FAIL);

	FAILED_CHECK_RETURN(SetUp_Component(), E_FAIL);

	m_pTransformCom->Set_Scaled(_float3(m_fSizeX, m_fSizeX, 1.f));
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(750.f, -480.f, 0.f, 1.f));
	// m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(-850.f, -470.f, 0.f, 1.f));

	return S_OK;
}

void CPlayer_UI_Throw::Tick(_double dTimeDelta)
{
	__super::Tick(dTimeDelta);

	if (nullptr == m_pOwner)
		return;

	m_iThrownCnt = dynamic_cast<CPlayer*>(m_pOwner)->Get_ThrowCnt();

	
}

void CPlayer_UI_Throw::Late_Tick(_double dTimeDelta)
{
	__super::Late_Tick(dTimeDelta);

	if (nullptr != m_pRendererCom)
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);
}

HRESULT CPlayer_UI_Throw::Render()
{
	FAILED_CHECK_RETURN(__super::Render(), E_FAIL);

	FAILED_CHECK_RETURN(SetUp_ShaderResources(), E_FAIL);

	m_pShaderCom->Begin(1);

	m_pVIBufferCom->Render();

	return S_OK;
}

HRESULT CPlayer_UI_Throw::SetUp_Component()
{
	FAILED_CHECK_RETURN(__super::Add_Component(CGameInstance::Get_StaticLevelIndex(), L"Prototype_Component_Renderer", L"Com_Renderer", (CComponent**)&m_pRendererCom, this), E_FAIL);
	FAILED_CHECK_RETURN(__super::Add_Component(CGameInstance::Get_StaticLevelIndex(), L"Prototype_Component_Shader_VtxTex", L"Com_Shader", (CComponent**)&m_pShaderCom, this), E_FAIL);
	FAILED_CHECK_RETURN(__super::Add_Component(CGameInstance::Get_StaticLevelIndex(), L"Prototype_Component_VIBuffer_Rect", L"Com_VIBuffer", (CComponent**)&m_pVIBufferCom, this), E_FAIL);
	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Texture_UI_PlayerThrowOn", L"Com_OnTexture", (CComponent**)&m_pOnTextureCom, this), E_FAIL);
	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Texture_UI_PlayerThrowOff", L"Com_OffTexture", (CComponent**)&m_pOffTextureCom, this), E_FAIL);

	return S_OK;
}

HRESULT CPlayer_UI_Throw::SetUp_ShaderResources()
{
	NULL_CHECK_RETURN(m_pShaderCom, E_FAIL);

	m_pTransformCom->Bind_ShaderResource(m_pShaderCom, L"g_WorldMatrix");
	m_pShaderCom->Set_Matrix(L"g_ViewMatrix", &m_ViewMatrix);
	m_pShaderCom->Set_Matrix(L"g_ProjMatrix", &m_ProjMatrix);

	if (0 >= m_iThrownCnt)
	{
		m_pOffTextureCom->Bind_ShaderResource(m_pShaderCom, L"g_Texture");
	}
	else 
		m_pOnTextureCom->Bind_ShaderResource(m_pShaderCom, L"g_Texture");

	return S_OK;
}

CPlayer_UI_Throw* CPlayer_UI_Throw::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CPlayer_UI_Throw*	pInstance = new CPlayer_UI_Throw(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Create : CLogo");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CPlayer_UI_Throw::Clone(const wstring& wstrPrototypeTag, void* pArg)
{
	CPlayer_UI_Throw*	pInstance = new CPlayer_UI_Throw(*this);

	if (FAILED(pInstance->Initialize_Clone(wstrPrototypeTag, pArg)))
	{
		MSG_BOX("Failed to Create : CLogo");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CPlayer_UI_Throw::Free()
{
	__super::Free();
}

// Player Dash

CPlayer_UI_Dash::CPlayer_UI_Dash(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUI(pDevice, pContext)
{
}

CPlayer_UI_Dash::CPlayer_UI_Dash(const CPlayer_UI_Dash& rhs)
	: CUI(rhs)
{
}

HRESULT CPlayer_UI_Dash::Initialize_Prototype()
{
	FAILED_CHECK_RETURN(__super::Initialize_Prototype(), E_FAIL);

	return S_OK;
}

HRESULT CPlayer_UI_Dash::Initialize_Clone(const wstring& wstrPrototypeTag, void* pArg)
{
	m_fSizeX = 256.f;
	m_fSizeY = 256.f;;
	m_fX = m_fSizeX * 0.5f;
	m_fY = m_fSizeY * 0.5f;

	FAILED_CHECK_RETURN(__super::Initialize_Clone(wstrPrototypeTag, pArg), E_FAIL);

	FAILED_CHECK_RETURN(SetUp_Component(), E_FAIL);

	m_pTransformCom->Set_Scaled(_float3(m_fSizeX, m_fSizeX, 1.f));
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(880.f, -480.f, 0.f, 1.f));
	// m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(-850.f, -470.f, 0.f, 1.f));

	return S_OK;
}

void CPlayer_UI_Dash::Tick(_double dTimeDelta)
{
	__super::Tick(dTimeDelta);
}

void CPlayer_UI_Dash::Late_Tick(_double dTimeDelta)
{
	__super::Late_Tick(dTimeDelta);

	if (nullptr != m_pRendererCom)
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);
}

HRESULT CPlayer_UI_Dash::Render()
{
	FAILED_CHECK_RETURN(__super::Render(), E_FAIL);

	FAILED_CHECK_RETURN(SetUp_ShaderResources(), E_FAIL);

	m_pShaderCom->Begin(1);

	m_pVIBufferCom->Render();

	return S_OK;
}

HRESULT CPlayer_UI_Dash::SetUp_Component()
{
	FAILED_CHECK_RETURN(__super::Add_Component(CGameInstance::Get_StaticLevelIndex(), L"Prototype_Component_Renderer", L"Com_Renderer", (CComponent**)&m_pRendererCom, this), E_FAIL);
	FAILED_CHECK_RETURN(__super::Add_Component(CGameInstance::Get_StaticLevelIndex(), L"Prototype_Component_Shader_VtxTex", L"Com_Shader", (CComponent**)&m_pShaderCom, this), E_FAIL);
	FAILED_CHECK_RETURN(__super::Add_Component(CGameInstance::Get_StaticLevelIndex(), L"Prototype_Component_VIBuffer_Rect", L"Com_VIBuffer", (CComponent**)&m_pVIBufferCom, this), E_FAIL);
	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Texture_UI_PlayerDashOn", L"Com_OnTexture", (CComponent**)&m_pOnTextureCom, this), E_FAIL);
	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Texture_UI_PlayerDashOff", L"Com_OffTexture", (CComponent**)&m_pOffTextureCom, this), E_FAIL);

	return S_OK;
}

HRESULT CPlayer_UI_Dash::SetUp_ShaderResources()
{
	NULL_CHECK_RETURN(m_pShaderCom, E_FAIL);

	m_pTransformCom->Bind_ShaderResource(m_pShaderCom, L"g_WorldMatrix");
	m_pShaderCom->Set_Matrix(L"g_ViewMatrix", &m_ViewMatrix);
	m_pShaderCom->Set_Matrix(L"g_ProjMatrix", &m_ProjMatrix);
	if(dynamic_cast<CPlayer*>(m_pOwner)->Get_CanDash() == true)
		m_pOnTextureCom->Bind_ShaderResource(m_pShaderCom, L"g_Texture");
	else
		m_pOffTextureCom->Bind_ShaderResource(m_pShaderCom, L"g_Texture");

	return S_OK;
}

CPlayer_UI_Dash* CPlayer_UI_Dash::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CPlayer_UI_Dash*	pInstance = new CPlayer_UI_Dash(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Create : CLogo");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CPlayer_UI_Dash::Clone(const wstring& wstrPrototypeTag, void* pArg)
{
	CPlayer_UI_Dash*	pInstance = new CPlayer_UI_Dash(*this);

	if (FAILED(pInstance->Initialize_Clone(wstrPrototypeTag, pArg)))
	{
		MSG_BOX("Failed to Create : CLogo");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CPlayer_UI_Dash::Free()
{
	__super::Free();
}

// Player Weapon Base

CPlayer_UI_Weapon::CPlayer_UI_Weapon(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUI(pDevice, pContext)
{
}

CPlayer_UI_Weapon::CPlayer_UI_Weapon(const CPlayer_UI_Weapon& rhs)
	: CUI(rhs)
{
}

HRESULT CPlayer_UI_Weapon::Initialize_Prototype()
{
	FAILED_CHECK_RETURN(__super::Initialize_Prototype(), E_FAIL);

	return S_OK;
}

HRESULT CPlayer_UI_Weapon::Initialize_Clone(const wstring& wstrPrototypeTag, void* pArg)
{
	m_fSizeX = 512.f;
	m_fSizeY = 512.f;
	m_fX = m_fSizeX * 0.5f;
	m_fY = m_fSizeY * 0.5f;

	FAILED_CHECK_RETURN(__super::Initialize_Clone(wstrPrototypeTag, pArg), E_FAIL);

	FAILED_CHECK_RETURN(SetUp_Component(), E_FAIL);

	m_pTransformCom->Set_Scaled(_float3(m_fSizeX, m_fSizeX, 1.f));
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(1080.f, -480.f, 0.f, 1.f));
	// m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(-850.f, -470.f, 0.f, 1.f));

	return S_OK;
}

void CPlayer_UI_Weapon::Tick(_double dTimeDelta)
{
	__super::Tick(dTimeDelta);
}

void CPlayer_UI_Weapon::Late_Tick(_double dTimeDelta)
{
	__super::Late_Tick(dTimeDelta);

	if (nullptr != m_pRendererCom)
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);
}

HRESULT CPlayer_UI_Weapon::Render()
{
	FAILED_CHECK_RETURN(__super::Render(), E_FAIL);

	FAILED_CHECK_RETURN(SetUp_ShaderResources(), E_FAIL);

	m_pShaderCom->Begin(1);

	m_pVIBufferCom->Render();

	return S_OK;
}

HRESULT CPlayer_UI_Weapon::SetUp_Component()
{
	FAILED_CHECK_RETURN(__super::Add_Component(CGameInstance::Get_StaticLevelIndex(), L"Prototype_Component_Renderer", L"Com_Renderer", (CComponent**)&m_pRendererCom, this), E_FAIL);
	FAILED_CHECK_RETURN(__super::Add_Component(CGameInstance::Get_StaticLevelIndex(), L"Prototype_Component_Shader_VtxTex", L"Com_Shader", (CComponent**)&m_pShaderCom, this), E_FAIL);
	FAILED_CHECK_RETURN(__super::Add_Component(CGameInstance::Get_StaticLevelIndex(), L"Prototype_Component_VIBuffer_Rect", L"Com_VIBuffer", (CComponent**)&m_pVIBufferCom, this), E_FAIL);
	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Texture_UI_PlayerGunInfo", L"Com_Texture", (CComponent**)&m_pTextureCom, this), E_FAIL);

	return S_OK;
}

HRESULT CPlayer_UI_Weapon::SetUp_ShaderResources()
{
	NULL_CHECK_RETURN(m_pShaderCom, E_FAIL);

	m_pTransformCom->Bind_ShaderResource(m_pShaderCom, L"g_WorldMatrix");
	m_pShaderCom->Set_Matrix(L"g_ViewMatrix", &m_ViewMatrix);
	m_pShaderCom->Set_Matrix(L"g_ProjMatrix", &m_ProjMatrix);
	m_pTextureCom->Bind_ShaderResource(m_pShaderCom, L"g_Texture");

	return S_OK;
}

CPlayer_UI_Weapon* CPlayer_UI_Weapon::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CPlayer_UI_Weapon*	pInstance = new CPlayer_UI_Weapon(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Create : CLogo");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CPlayer_UI_Weapon::Clone(const wstring& wstrPrototypeTag, void* pArg)
{
	CPlayer_UI_Weapon*	pInstance = new CPlayer_UI_Weapon(*this);

	if (FAILED(pInstance->Initialize_Clone(wstrPrototypeTag, pArg)))
	{
		MSG_BOX("Failed to Create : CLogo");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CPlayer_UI_Weapon::Free()
{
	__super::Free();
}


// Player Weapon Number

CPlayer_UI_Weapon_Number::CPlayer_UI_Weapon_Number(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUI(pDevice, pContext)
{
}

CPlayer_UI_Weapon_Number::CPlayer_UI_Weapon_Number(const CPlayer_UI_Weapon_Number& rhs)
	: CUI(rhs)
{
}

HRESULT CPlayer_UI_Weapon_Number::Initialize_Prototype()
{
	FAILED_CHECK_RETURN(__super::Initialize_Prototype(), E_FAIL);

	return S_OK;
}

HRESULT CPlayer_UI_Weapon_Number::Initialize_Clone(const wstring& wstrPrototypeTag, void* pArg)
{
	m_fSizeX = 256.f;
	m_fSizeY = 256.f;
	m_fX = m_fSizeX * 0.5f;
	m_fY = m_fSizeY * 0.5f;

	FAILED_CHECK_RETURN(__super::Initialize_Clone(wstrPrototypeTag, pArg), E_FAIL);

	FAILED_CHECK_RETURN(SetUp_Component(), E_FAIL);

	m_pTransformCom->Set_Scaled(_float3(m_fSizeX, m_fSizeX, 1.f));
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(1070.f, -538.f, 0.f, 1.f));
	// m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(-850.f, -470.f, 0.f, 1.f));


	return S_OK;
}

void CPlayer_UI_Weapon_Number::Tick(_double dTimeDelta)
{
	__super::Tick(dTimeDelta);

	if (nullptr == m_pOwner)
		return;

	m_wstrWeaponNumber = dynamic_cast<CPlayer*>(m_pOwner)->Get_CurWeaponNumber();
}

void CPlayer_UI_Weapon_Number::Late_Tick(_double dTimeDelta)
{
	__super::Late_Tick(dTimeDelta);

	if (nullptr != m_pRendererCom)
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);
}

HRESULT CPlayer_UI_Weapon_Number::Render()
{
	FAILED_CHECK_RETURN(__super::Render(), E_FAIL);

	FAILED_CHECK_RETURN(SetUp_ShaderResources(), E_FAIL);

	m_pShaderCom->Begin(2);

	m_pVIBufferCom->Render();

	return S_OK;
}


HRESULT CPlayer_UI_Weapon_Number::SetUp_Component()
{
	FAILED_CHECK_RETURN(__super::Add_Component(CGameInstance::Get_StaticLevelIndex(), L"Prototype_Component_Renderer", L"Com_Renderer", (CComponent**)&m_pRendererCom, this), E_FAIL);
	FAILED_CHECK_RETURN(__super::Add_Component(CGameInstance::Get_StaticLevelIndex(), L"Prototype_Component_Shader_VtxTex", L"Com_Shader", (CComponent**)&m_pShaderCom, this), E_FAIL);
	FAILED_CHECK_RETURN(__super::Add_Component(CGameInstance::Get_StaticLevelIndex(), L"Prototype_Component_VIBuffer_Rect", L"Com_VIBuffer", (CComponent**)&m_pVIBufferCom, this), E_FAIL);
	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Texture_UI_PlayerGunNumber_1", L"Com_Texture1", (CComponent**)&m_pWeaponNumberCom[WEAPON_FIRST], this), E_FAIL);
	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Texture_UI_PlayerGunNumber_2", L"Com_Texture2", (CComponent**)&m_pWeaponNumberCom[WEAPON_SECOND], this), E_FAIL);
	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Texture_UI_PlayerGunNumber_3", L"Com_Texture3", (CComponent**)&m_pWeaponNumberCom[WEAPON_THIRD], this), E_FAIL);

	return S_OK;
}

HRESULT CPlayer_UI_Weapon_Number::SetUp_ShaderResources()
{
	NULL_CHECK_RETURN(m_pShaderCom, E_FAIL);

	m_pTransformCom->Bind_ShaderResource(m_pShaderCom, L"g_WorldMatrix");
	m_pShaderCom->Set_Matrix(L"g_ViewMatrix", &m_ViewMatrix);
	m_pShaderCom->Set_Matrix(L"g_ProjMatrix", &m_ProjMatrix);

	if (m_wstrWeaponNumber == L"1")
	{
		m_pWeaponNumberCom[WEAPON_FIRST]->Bind_ShaderResource(m_pShaderCom, L"g_Texture");
	}
	else if (m_wstrWeaponNumber == L"2")
	{
		m_pWeaponNumberCom[WEAPON_SECOND]->Bind_ShaderResource(m_pShaderCom, L"g_Texture");
	}
	else if (m_wstrWeaponNumber == L"3")
	{
		m_pWeaponNumberCom[WEAPON_THIRD]->Bind_ShaderResource(m_pShaderCom, L"g_Texture");
	}
	return S_OK;
}

CPlayer_UI_Weapon_Number* CPlayer_UI_Weapon_Number::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CPlayer_UI_Weapon_Number*	pInstance = new CPlayer_UI_Weapon_Number(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Create : CLogo");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CPlayer_UI_Weapon_Number::Clone(const wstring& wstrPrototypeTag, void* pArg)
{
	CPlayer_UI_Weapon_Number*	pInstance = new CPlayer_UI_Weapon_Number(*this);

	if (FAILED(pInstance->Initialize_Clone(wstrPrototypeTag, pArg)))
	{
		MSG_BOX("Failed to Create : CLogo");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CPlayer_UI_Weapon_Number::Free()
{
	__super::Free();
}

// Player Weapon Picture

CPlayer_UI_Weapon_Pic::CPlayer_UI_Weapon_Pic(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUI(pDevice, pContext)
{
}

CPlayer_UI_Weapon_Pic::CPlayer_UI_Weapon_Pic(const CPlayer_UI_Weapon_Pic& rhs)
	: CUI(rhs)
{
}

HRESULT CPlayer_UI_Weapon_Pic::Initialize_Prototype()
{
	FAILED_CHECK_RETURN(__super::Initialize_Prototype(), E_FAIL);

	return S_OK;
}

HRESULT CPlayer_UI_Weapon_Pic::Initialize_Clone(const wstring& wstrPrototypeTag, void* pArg)
{
	m_fSizeX = 300.f;
	m_fSizeY = 300.f;
	m_fX = m_fSizeX * 0.5f;
	m_fY = m_fSizeY * 0.5f;

	FAILED_CHECK_RETURN(__super::Initialize_Clone(wstrPrototypeTag, pArg), E_FAIL);

	FAILED_CHECK_RETURN(SetUp_Component(), E_FAIL);

	m_pTransformCom->Set_Scaled(_float3(m_fSizeX, m_fSizeX, 1.f));
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(1095.f, -430.f, 0.f, 1.f));
	// m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(-850.f, -470.f, 0.f, 1.f));

	return S_OK;
}

void CPlayer_UI_Weapon_Pic::Tick(_double dTimeDelta)
{
	__super::Tick(dTimeDelta);

	if (nullptr == m_pOwner)
		return;

	m_wstrWeaponName = dynamic_cast<CPlayer*>(m_pOwner)->Get_CurWeaponName();

	if (m_wstrWeaponName == L"WEAPON_FIREDRAGON")
	{
		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(1110.f, -460.f, 0.f, 1.f));
	}
	else
	{
		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(1095.f, -460.f, 0.f, 1.f));
	}
}

void CPlayer_UI_Weapon_Pic::Late_Tick(_double dTimeDelta)
{
	__super::Late_Tick(dTimeDelta);

	if (nullptr != m_pRendererCom)
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);
}

HRESULT CPlayer_UI_Weapon_Pic::Render()
{
	FAILED_CHECK_RETURN(__super::Render(), E_FAIL);

	FAILED_CHECK_RETURN(SetUp_ShaderResources(), E_FAIL);

	m_pShaderCom->Begin(1);

	m_pVIBufferCom->Render();

	return S_OK;
}

HRESULT CPlayer_UI_Weapon_Pic::SetUp_Component()
{
	FAILED_CHECK_RETURN(__super::Add_Component(CGameInstance::Get_StaticLevelIndex(), L"Prototype_Component_Renderer", L"Com_Renderer", (CComponent**)&m_pRendererCom, this), E_FAIL);
	FAILED_CHECK_RETURN(__super::Add_Component(CGameInstance::Get_StaticLevelIndex(), L"Prototype_Component_Shader_VtxTex", L"Com_Shader", (CComponent**)&m_pShaderCom, this), E_FAIL);
	FAILED_CHECK_RETURN(__super::Add_Component(CGameInstance::Get_StaticLevelIndex(), L"Prototype_Component_VIBuffer_Rect", L"Com_VIBuffer", (CComponent**)&m_pVIBufferCom, this), E_FAIL);
	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Texture_UI_Default_Pistol", L"Com_DefaultWeaponTexture", (CComponent**)&m_pWeaponTextureCom[CWeapon_State::DEFAULT_PISTOL], this), E_FAIL);
	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Texture_UI_Flame_Bullet", L"Com_FlameBulletWeaponTexture", (CComponent**)&m_pWeaponTextureCom[CWeapon_State::FLAME_BULLET], this), E_FAIL);
	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Texture_UI_Fire_Dragon", L"Com_FireDragonWeaponTexture", (CComponent**)&m_pWeaponTextureCom[CWeapon_State::FIRE_DRAGON], this), E_FAIL);
	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Texture_UI_Poison", L"Com_PoisonWeaponTexture", (CComponent**)&m_pWeaponTextureCom[CWeapon_State::POISON], this), E_FAIL);

	return S_OK;
}

HRESULT CPlayer_UI_Weapon_Pic::SetUp_ShaderResources()
{
	NULL_CHECK_RETURN(m_pShaderCom, E_FAIL);

	m_pTransformCom->Bind_ShaderResource(m_pShaderCom, L"g_WorldMatrix");
	m_pShaderCom->Set_Matrix(L"g_ViewMatrix", &m_ViewMatrix);
	m_pShaderCom->Set_Matrix(L"g_ProjMatrix", &m_ProjMatrix);

	if (m_wstrWeaponName == L"WEAPON_DEFAULT")
	{
		m_pWeaponTextureCom[CWeapon_State::DEFAULT_PISTOL]->Bind_ShaderResource(m_pShaderCom, L"g_Texture");
	}
	else if (m_wstrWeaponName == L"WEAPON_FLAMEBULLET")
	{
		m_pWeaponTextureCom[CWeapon_State::FLAME_BULLET]->Bind_ShaderResource(m_pShaderCom, L"g_Texture");
	}
	else if (m_wstrWeaponName == L"WEAPON_FIREDRAGON")
	{
		m_pWeaponTextureCom[CWeapon_State::FIRE_DRAGON]->Bind_ShaderResource(m_pShaderCom, L"g_Texture");
	}
	else if (m_wstrWeaponName == L"WEAPON_POISON")
	{
		m_pWeaponTextureCom[CWeapon_State::POISON]->Bind_ShaderResource(m_pShaderCom, L"g_Texture");
	}

	return S_OK;
}

CPlayer_UI_Weapon_Pic* CPlayer_UI_Weapon_Pic::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CPlayer_UI_Weapon_Pic*	pInstance = new CPlayer_UI_Weapon_Pic(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Create : CLogo");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CPlayer_UI_Weapon_Pic::Clone(const wstring& wstrPrototypeTag, void* pArg)
{
	CPlayer_UI_Weapon_Pic*	pInstance = new CPlayer_UI_Weapon_Pic(*this);

	if (FAILED(pInstance->Initialize_Clone(wstrPrototypeTag, pArg)))
	{
		MSG_BOX("Failed to Create : CLogo");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CPlayer_UI_Weapon_Pic::Free()
{
	__super::Free();
}

// Player Bullet Type


CPlayer_UI_BulletType::CPlayer_UI_BulletType(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUI(pDevice, pContext)
{
}

CPlayer_UI_BulletType::CPlayer_UI_BulletType(const CPlayer_UI_BulletType& rhs)
	: CUI(rhs)
{
}

HRESULT CPlayer_UI_BulletType::Initialize_Prototype()
{
	FAILED_CHECK_RETURN(__super::Initialize_Prototype(), E_FAIL);

	return S_OK;
}

HRESULT CPlayer_UI_BulletType::Initialize_Clone(const wstring& wstrPrototypeTag, void* pArg)
{
	m_fSizeX = 170.f;
	m_fSizeY = 170.f;
	m_fX = m_fSizeX * 0.5f;
	m_fY = m_fSizeY * 0.5f;

	FAILED_CHECK_RETURN(__super::Initialize_Clone(wstrPrototypeTag, pArg), E_FAIL);

	FAILED_CHECK_RETURN(SetUp_Component(), E_FAIL);

	m_pTransformCom->Set_Scaled(_float3(m_fSizeX, m_fSizeX, 1.f));
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(955.f, -315.f, 0.f, 1.f));

	return S_OK;
}

void CPlayer_UI_BulletType::Tick(_double dTimeDelta)
{
	__super::Tick(dTimeDelta);

	if (nullptr == m_pOwner)
		return;

	m_wstrWeaponName = dynamic_cast<CPlayer*>(m_pOwner)->Get_CurWeaponName();
}

void CPlayer_UI_BulletType::Late_Tick(_double dTimeDelta)
{
	__super::Late_Tick(dTimeDelta);

	if (nullptr != m_pRendererCom)
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);
}

HRESULT CPlayer_UI_BulletType::Render()
{
	FAILED_CHECK_RETURN(__super::Render(), E_FAIL);

	FAILED_CHECK_RETURN(SetUp_ShaderResources(), E_FAIL);

	m_pShaderCom->Begin(2);

	m_pVIBufferCom->Render();

	return S_OK;
}

HRESULT CPlayer_UI_BulletType::SetUp_Component()
{
	FAILED_CHECK_RETURN(__super::Add_Component(CGameInstance::Get_StaticLevelIndex(), L"Prototype_Component_Renderer", L"Com_Renderer", (CComponent**)&m_pRendererCom, this), E_FAIL);
	FAILED_CHECK_RETURN(__super::Add_Component(CGameInstance::Get_StaticLevelIndex(), L"Prototype_Component_Shader_VtxTex", L"Com_Shader", (CComponent**)&m_pShaderCom, this), E_FAIL);
	FAILED_CHECK_RETURN(__super::Add_Component(CGameInstance::Get_StaticLevelIndex(), L"Prototype_Component_VIBuffer_Rect", L"Com_VIBuffer", (CComponent**)&m_pVIBufferCom, this), E_FAIL);
	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Texture_UI_PlayerBullet_Pistol", L"Com_PistolTex", (CComponent**)&m_pWeaponTextureCom[WEAPON_FIRST], this), E_FAIL);
	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Texture_UI_PlayerBullet_Rifle", L"Com_RifleTex", (CComponent**)&m_pWeaponTextureCom[WEAPON_SECOND], this), E_FAIL);
	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Texture_UI_PlayerBullet_Injector", L"Com_InjectorTex", (CComponent**)&m_pWeaponTextureCom[WEAPON_THIRD], this), E_FAIL);

	return S_OK;
}

HRESULT CPlayer_UI_BulletType::SetUp_ShaderResources()
{
	NULL_CHECK_RETURN(m_pShaderCom, E_FAIL);

	m_pTransformCom->Bind_ShaderResource(m_pShaderCom, L"g_WorldMatrix");
	m_pShaderCom->Set_Matrix(L"g_ViewMatrix", &m_ViewMatrix);
	m_pShaderCom->Set_Matrix(L"g_ProjMatrix", &m_ProjMatrix);

	if (m_wstrWeaponName == L"WEAPON_POISON")
	{
		//Rifle
		m_pWeaponTextureCom[WEAPON_SECOND]->Bind_ShaderResource(m_pShaderCom, L"g_Texture");
	}
	else if (m_wstrWeaponName == L"WEAPON_FIREDRAGON")
	{
		//Injector
		m_pWeaponTextureCom[WEAPON_THIRD]->Bind_ShaderResource(m_pShaderCom, L"g_Texture");
	}
	else if (	m_wstrWeaponName == L"WEAPON_FLAMEBULLET" || 	m_wstrWeaponName == L"WEAPON_DEFAULT")
	{
		//Pistol
		m_pWeaponTextureCom[WEAPON_FIRST]->Bind_ShaderResource(m_pShaderCom, L"g_Texture");
	}
	return S_OK;
}

CPlayer_UI_BulletType* CPlayer_UI_BulletType::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CPlayer_UI_BulletType*	pInstance = new CPlayer_UI_BulletType(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Create : CLogo");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CPlayer_UI_BulletType::Clone(const wstring& wstrPrototypeTag, void* pArg)
{
	CPlayer_UI_BulletType*	pInstance = new CPlayer_UI_BulletType(*this);

	if (FAILED(pInstance->Initialize_Clone(wstrPrototypeTag, pArg)))
	{
		MSG_BOX("Failed to Create : CLogo");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CPlayer_UI_BulletType::Free()
{
	__super::Free();
}

// Player CountMachine


CPlayer_UI_CountMachine::CPlayer_UI_CountMachine(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUI(pDevice, pContext)
{
}

CPlayer_UI_CountMachine::CPlayer_UI_CountMachine(const CPlayer_UI_CountMachine& rhs)
	: CUI(rhs)
{
}

HRESULT CPlayer_UI_CountMachine::Initialize_Prototype()
{
	FAILED_CHECK_RETURN(__super::Initialize_Prototype(), E_FAIL);

	return S_OK;
}

HRESULT CPlayer_UI_CountMachine::Initialize_Clone(const wstring& wstrPrototypeTag, void* pArg)
{
	m_fSizeX = 36.f;
	m_fSizeY = 36.f;
	m_fX = m_fSizeX * 0.5f;
	m_fY = m_fSizeY * 0.5f;

	COUNTDESC	tmp;
	ZeroMemory(&tmp, sizeof(COUNTDESC));

	if (nullptr != pArg)
		tmp = *(COUNTDESC*)pArg;

	m_eType = tmp.m_eType;

	FAILED_CHECK_RETURN(__super::Initialize_Clone(wstrPrototypeTag, &tmp), E_FAIL);

	FAILED_CHECK_RETURN(SetUp_Component(), E_FAIL);

	m_pTransformCom->Set_Scaled(_float3(m_fSizeX, m_fSizeX, 1.f));
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(1127.f, -315.f, 0.f, 1.f));

	return S_OK;
}

void CPlayer_UI_CountMachine::Tick(_double dTimeDelta)
{
	__super::Tick(dTimeDelta);

	if (nullptr == m_pOwner || CNT_END == m_eType)
		return;

	m_wstrWeaponName = dynamic_cast<CPlayer*>(m_pOwner)->Get_CurWeaponName();

	switch (m_eType)
	{
	case CNT_GOLD:
		m_iGoldCnt = dynamic_cast<CPlayer*>(m_pOwner)->Get_GoldCnt();
		if (m_iGoldCnt != m_iLastGoldCnt)
		{
			for (auto& iter : m_vecCountUI)
				Safe_Release(iter);
			m_vecCountUI.clear();

			InttoString(m_iGoldCnt, m_vPlayerGoldCntPos, m_vGoldNumColor, m_vGoldNumberSize);
		}
		break;

	case CNT_BULLET:
		if (m_wstrWeaponName == L"WEAPON_POISON")
		{
			//Rifle
			m_eWeaponType = WEAPON_RIFLE;
			m_iPlayer_BulletCnt = dynamic_cast<CPlayer*>(m_pOwner)->Get_RifleBulletCnt();

			// m_iPlayer_BulletCnt = dynamic_cast<CPlayer*>(m_pOwner)->Get_PistolBulletCnt();
			m_iWeapon_BulletCnt = m_pWeapon_State->Get_CurWeaponBulletCnt(m_wstrWeaponName);


			for (auto& iter : m_vecCountUI)
				Safe_Release(iter);
			m_vecCountUI.clear();

			InttoString(m_iWeapon_BulletCnt, m_vWeaponBulletCntPos, m_vRifleNumColor, m_vBulletNumberSize);
			InttoString(m_iPlayer_BulletCnt, m_vPlayerBulletCntPos, m_vRifleNumColor, m_vBulletNumberSize);

		}
		else if (m_wstrWeaponName == L"WEAPON_FIREDRAGON")
		{
			//Injector
			m_eWeaponType = WEAPON_INJECTOR;

			m_iPlayer_BulletCnt = dynamic_cast<CPlayer*>(m_pOwner)->Get_InjectorBulletCnt();
			m_iWeapon_BulletCnt = m_pWeapon_State->Get_CurWeaponBulletCnt(m_wstrWeaponName);

			for (auto& iter : m_vecCountUI)
				Safe_Release(iter);
			m_vecCountUI.clear();

			InttoString(m_iWeapon_BulletCnt, m_vWeaponBulletCntPos, m_vInjectorNumColor, m_vBulletNumberSize);
			InttoString(m_iPlayer_BulletCnt, m_vPlayerBulletCntPos, m_vInjectorNumColor, m_vBulletNumberSize);

		}
		else if (m_wstrWeaponName == L"WEAPON_POISON" || m_wstrWeaponName == L"WEAPON_FLAMEBULLET" || m_wstrWeaponName == L"WEAPON_DEFAULT")
		{
			//Pistol
			m_eWeaponType = WEAPON_PISTOL;

			m_iPlayer_BulletCnt = dynamic_cast<CPlayer*>(m_pOwner)->Get_PistolBulletCnt();
			m_iWeapon_BulletCnt = m_pWeapon_State->Get_CurWeaponBulletCnt(m_wstrWeaponName);

			for (auto& iter : m_vecCountUI)
				Safe_Release(iter);
			m_vecCountUI.clear();

			InttoString(m_iWeapon_BulletCnt, m_vWeaponBulletCntPos, m_vPistolNumColor, m_vBulletNumberSize);
			InttoString(m_iPlayer_BulletCnt, m_vPlayerBulletCntPos, m_vPistolNumColor, m_vBulletNumberSize);

		}
		break;

	case CNT_THROW:
		m_iThrowCnt = dynamic_cast<CPlayer*>(m_pOwner)->Get_ThrowCnt();

		if (m_iThrowCnt != m_iLastThrowCnt)
		{
			for (auto& iter : m_vecCountUI)
				Safe_Release(iter);
			m_vecCountUI.clear();

			// m_vPlayerThrowCntPos = _float2{ 533.f, -447.f };
			InttoString(m_iThrowCnt, m_vPlayerThrowCntPos, m_vThrowNumColor, m_vThrowNumberSize);
		}
		break;

	case CNT_EMERALD:
		m_iEmeraldCnt = dynamic_cast<CPlayer*>(m_pOwner)->Get_EmeraldCnt();

		if(m_iEmeraldCnt != m_iLastEmeraldCnt)
		{
			for (auto& iter : m_vecCountUI)
				Safe_Release(iter);
			m_vecCountUI.clear();

			InttoString(m_iEmeraldCnt, m_vPlayerEmeraldCntPos, m_vGoldNumColor, m_vGoldNumberSize);
		}
		break;

	default:;
	}

	Create_CountUI();

	for (_uint i = 0; i < m_vecCountUI.size(); ++i)
	{
		m_vecCountUI[i]->Tick(dTimeDelta);
	}
}

void CPlayer_UI_CountMachine::Late_Tick(_double dTimeDelta)
{
	__super::Late_Tick(dTimeDelta);

	for (_uint i = 0; i < m_vecCountUI.size(); ++i)
	{
		m_vecCountUI[i]->Late_Tick(dTimeDelta);
	}



	if (nullptr != m_pRendererCom)
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);
}

HRESULT CPlayer_UI_CountMachine::Render()
{
	FAILED_CHECK_RETURN(__super::Render(), E_FAIL);

	FAILED_CHECK_RETURN(SetUp_ShaderResources(), E_FAIL);

	m_pShaderCom->Begin(3);

	m_pVIBufferCom->Render();

	return S_OK;
}

void CPlayer_UI_CountMachine::Create_CountUI()
{

}

void CPlayer_UI_CountMachine::InttoString(_uint ConvertCnt, _float2 vPos, _float4 vColor, _float2 vSize)
{
	string s = to_string(ConvertCnt);
	vector<int> v, v2;

	for (char c : s)
	{
		v.push_back(c);
	}

	for (int i = 0; i < v.size(); i++)
	{
		v2.push_back(s[i] - '0');
	}

	for (int i = 0; i < v2.size(); ++i)
	{
		for (int Number : v2)
		{
			if (v2[i] != Number)
				continue;

			CGameObject*		pNumberUI = nullptr;
			CGameObject::GAMEOBJECTDESC		tmp;
			if (v2.size() == 1)
				tmp.TransformDesc.vInitPos = _float3(vPos.x + 36.f, vPos.y, 0.f);
			else
				tmp.TransformDesc.vInitPos = _float3(vPos.x + (i * 36.f), vPos.y, 0.f);

			tmp.m_iNumCnt = i;
			tmp.m_iNumber = Number;
			tmp.m_vNumColor = vColor;
			tmp.m_vTexSize = vSize;
			pNumberUI = CGameInstance::GetInstance()->Clone_GameObject(L"Prototype_GameObject_Player_NumberUI", &tmp);
			NULL_CHECK_RETURN(pNumberUI, );

			m_vecCountUI.push_back(dynamic_cast<vector<CUI*>::value_type>(pNumberUI));

			m_iLastPlayer_BulletCnt = m_iPlayer_BulletCnt;
			m_iLastWeapon_BulletCnt = m_iWeapon_BulletCnt;
			m_iLastGoldCnt = m_iGoldCnt;
			m_iLastThrowCnt = m_iThrowCnt;
		}
	}
}

HRESULT CPlayer_UI_CountMachine::SetUp_Component()
{
	FAILED_CHECK_RETURN(__super::Add_Component(CGameInstance::Get_StaticLevelIndex(), L"Prototype_Component_Renderer", L"Com_Renderer", (CComponent**)&m_pRendererCom, this), E_FAIL);
	FAILED_CHECK_RETURN(__super::Add_Component(CGameInstance::Get_StaticLevelIndex(), L"Prototype_Component_Shader_VtxTex", L"Com_Shader", (CComponent**)&m_pShaderCom, this), E_FAIL);
	FAILED_CHECK_RETURN(__super::Add_Component(CGameInstance::Get_StaticLevelIndex(), L"Prototype_Component_VIBuffer_Rect", L"Com_VIBuffer", (CComponent**)&m_pVIBufferCom, this), E_FAIL);
	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Texture_Numbering_Slash", L"Com_Texture", (CComponent**)&m_pTextureCom, this), E_FAIL);

	return S_OK;
}

HRESULT CPlayer_UI_CountMachine::SetUp_ShaderResources()
{
	NULL_CHECK_RETURN(m_pShaderCom, E_FAIL);

	m_pTransformCom->Bind_ShaderResource(m_pShaderCom, L"g_WorldMatrix");
	m_pShaderCom->Set_Matrix(L"g_ViewMatrix", &m_ViewMatrix);
	m_pShaderCom->Set_Matrix(L"g_ProjMatrix", &m_ProjMatrix);

	if (m_eWeaponType == WEAPON_PISTOL)
	{
		FAILED_CHECK_RETURN(m_pShaderCom->Set_RawValue(L"g_vSlashColor", &m_vPistolNumColor, sizeof(_float4)), E_FAIL);
	}
	else if (m_eWeaponType == WEAPON_INJECTOR)
	{
		FAILED_CHECK_RETURN(m_pShaderCom->Set_RawValue(L"g_vSlashColor", &m_vInjectorNumColor, sizeof(_float4)), E_FAIL);
	}
	else if (m_eWeaponType == WEAPON_RIFLE)
	{
		FAILED_CHECK_RETURN(m_pShaderCom->Set_RawValue(L"g_vSlashColor", &m_vRifleNumColor, sizeof(_float4)), E_FAIL);
	}

	m_pTextureCom->Bind_ShaderResource(m_pShaderCom, L"g_Texture");

	return S_OK;
}

CPlayer_UI_CountMachine* CPlayer_UI_CountMachine::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CPlayer_UI_CountMachine*	pInstance = new CPlayer_UI_CountMachine(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Create : CLogo");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CPlayer_UI_CountMachine::Clone(const wstring& wstrPrototypeTag, void* pArg)
{
	CPlayer_UI_CountMachine*	pInstance = new CPlayer_UI_CountMachine(*this);

	if (FAILED(pInstance->Initialize_Clone(wstrPrototypeTag, pArg)))
	{
		MSG_BOX("Failed to Create : CLogo");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CPlayer_UI_CountMachine::Free()
{
	__super::Free();

	for (auto& pUI : m_vecCountUI)
		Safe_Release(pUI);
	m_vecCountUI.clear();

}
