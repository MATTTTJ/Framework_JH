#include "stdafx.h"
#include "..\public\FadeInOut.h"
#include "GameInstance.h"

CFadeInOut::CFadeInOut(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUI(pDevice, pContext)
{
}

CFadeInOut::CFadeInOut(const CFadeInOut& rhs)
	: CUI(rhs)
{
}

HRESULT CFadeInOut::Initialize_Prototype()
{
	FAILED_CHECK_RETURN(__super::Initialize_Prototype(), E_FAIL);

	return S_OK;
}

HRESULT CFadeInOut::Initialize_Clone(const wstring& wstrPrototypeTag, void* pArg)
{
	GAMEOBJECTDESC		GameObjectDesc;
	ZeroMemory(&GameObjectDesc, sizeof(GAMEOBJECTDESC));

	if (nullptr != pArg)
		m_tFadeDesc = *(FADEDESC*)pArg;

	// if(m_tFadeDesc.m_bFadeIn)
	// {
	// 	m_fFadeAlpha = 1.f;
	// }

	m_fSizeX = 2560.f;
	m_fSizeY = 1369.f;
	m_fX = m_fSizeX * 0.5f;
	m_fY = m_fSizeY * 0.5f;

	FAILED_CHECK_RETURN(__super::Initialize_Clone(wstrPrototypeTag, pArg), E_FAIL);

	FAILED_CHECK_RETURN(SetUp_Component(), E_FAIL);

	m_pTransformCom->Set_Scaled(_float3(m_fSizeX, m_fSizeY, 1.f));
	// m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fX - m_fSizeX * 0.5f, -m_fY + m_fSizeY * 0.5f, 0.f, 1.f));
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(0.f, 0.f, 0.f, 1.f));

	return S_OK;
}

void CFadeInOut::Tick(_double dTimeDelta)
{
	__super::Tick(dTimeDelta);
	
	if (m_bFadeOut)
	{
		m_fFadeAlpha -= (_float)(dTimeDelta * 7.5f);
		
		if (m_fFadeAlpha <= 0.f)
		{
			m_fFadeAlpha = 0.f;
		}
		// m_fFadeAlpha = 0.f;
	}
	else
	{
		m_fFadeAlpha += (_float)(dTimeDelta * 15.f);

		if (m_fFadeAlpha >= 1.f)
		{
			m_fFadeAlpha = 1.f;
		}
	}
}

void CFadeInOut::Late_Tick(_double dTimeDelta)
{
	__super::Late_Tick(dTimeDelta);

	if (nullptr != m_pRendererCom)
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_FADE, this);
}

HRESULT CFadeInOut::Render()
{
	FAILED_CHECK_RETURN(__super::Render(), E_FAIL);

	FAILED_CHECK_RETURN(SetUp_ShaderResources(), E_FAIL);

	m_pShaderCom->Begin(8);

	m_pVIBufferCom->Render();

	return S_OK;
}

HRESULT CFadeInOut::SetUp_Component()
{
	FAILED_CHECK_RETURN(__super::Add_Component(CGameInstance::Get_StaticLevelIndex(), L"Prototype_Component_Renderer", L"Com_Renderer", (CComponent**)&m_pRendererCom, this), E_FAIL);
	FAILED_CHECK_RETURN(__super::Add_Component(CGameInstance::Get_StaticLevelIndex(), L"Prototype_Component_Shader_VtxTex", L"Com_Shader", (CComponent**)&m_pShaderCom, this), E_FAIL);
	FAILED_CHECK_RETURN(__super::Add_Component(CGameInstance::Get_StaticLevelIndex(), L"Prototype_Component_VIBuffer_Rect", L"Com_VIBuffer", (CComponent**)&m_pVIBufferCom, this), E_FAIL);
	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Texture_Fade_In_Out", L"Com_Texture", (CComponent**)&m_pTextureCom, this), E_FAIL);

	return S_OK;
}

HRESULT CFadeInOut::SetUp_ShaderResources()
{
	NULL_CHECK_RETURN(m_pShaderCom, E_FAIL);

	m_pTransformCom->Bind_ShaderResource(m_pShaderCom, L"g_WorldMatrix");
	m_pShaderCom->Set_Matrix(L"g_ViewMatrix", &m_ViewMatrix);
	m_pShaderCom->Set_Matrix(L"g_ProjMatrix", &m_ProjMatrix);
	m_pTextureCom->Bind_ShaderResource(m_pShaderCom, L"g_Texture");
	m_pShaderCom->Set_RawValue(L"g_fFadeAlpha", &m_fFadeAlpha, sizeof(_float));

	return S_OK;
}

CFadeInOut* CFadeInOut::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CFadeInOut*	pInstance = new CFadeInOut(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Create : CFadeInOut");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CFadeInOut::Clone(const wstring& wstrPrototypeTag, void* pArg)
{
	CFadeInOut*	pInstance = new CFadeInOut(*this);

	if (FAILED(pInstance->Initialize_Clone(wstrPrototypeTag, pArg)))
	{
		MSG_BOX("Failed to Create : CFadeInOut");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CFadeInOut::Free()
{
	__super::Free();
}
