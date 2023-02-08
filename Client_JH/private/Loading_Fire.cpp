#include "stdafx.h"
#include "..\public\Loading_Fire.h"
#include "GameInstance.h"
#include "Player.h"

CLoading_Fire::CLoading_Fire(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CGameObject(pDevice, pContext)
{
}

CLoading_Fire::CLoading_Fire(const CLoading_Fire& rhs)
	: CGameObject(rhs)
{
}

HRESULT CLoading_Fire::Initialize_Prototype()
{
	FAILED_CHECK_RETURN(__super::Initialize_Prototype(), E_FAIL);

	return S_OK;
}

HRESULT CLoading_Fire::Initialize_Clone(const wstring& wstrPrototypeTag, void* pArg)
{
	if (nullptr != pArg)
	{
		m_tEffectDesc = *(EFFECTDESC*)pArg;

		FAILED_CHECK_RETURN(__super::Initialize_Clone(wstrPrototypeTag, &m_tEffectDesc), E_FAIL);
		FAILED_CHECK_RETURN(SetUp_Component(), E_FAIL);
		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, (XMVectorSet(m_tEffectDesc.m_tGameObjectDesc.TransformDesc.vInitPos.x, m_tEffectDesc.m_tGameObjectDesc.TransformDesc.vInitPos.y, m_tEffectDesc.m_tGameObjectDesc.TransformDesc.vInitPos.z, 1.f)));
	}
	else
	{
		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(0.f, 0.f, 0.f, 1.f));
		m_pTransformCom->Set_Scaled(_float3(1000.f, 1000.f, 1.f));
	}

	m_pTransformCom->Set_Scaled(_float3(174.f, 163.f, 1.f));
	XMStoreFloat4x4(&m_ViewMatrix, XMMatrixIdentity());
	XMStoreFloat4x4(&m_ProjMatrix, XMMatrixOrthographicLH((_float)g_iWinSizeX, (_float)g_iWinSizeY, 0.f, 1.f));
	return S_OK;
}

void CLoading_Fire::Tick(_double dTimeDelta)
{
	if (m_bIsDead == true)
		return;

	__super::Tick(dTimeDelta);

	m_fFrame += (_float)dTimeDelta * 30.0f;

	if (m_fFrame >= 30.0f)
		m_fFrame = 0.f;

}

void CLoading_Fire::Late_Tick(_double dTimeDelta)
{
	__super::Late_Tick(dTimeDelta);

	if (m_pRendererCom != nullptr)
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);
}

HRESULT CLoading_Fire::Render()
{
	FAILED_CHECK_RETURN(__super::Render(), E_FAIL);
	FAILED_CHECK_RETURN(SetUp_ShaderResources(), E_FAIL);
	m_pShaderCom->Begin(1);
	m_pVIBufferCom->Render();

	return S_OK;
}

HRESULT CLoading_Fire::SetUp_Component()
{
	FAILED_CHECK_RETURN(__super::Add_Component(CGameInstance::Get_StaticLevelIndex(), L"Prototype_Component_Renderer", L"Com_Renderer", (CComponent**)&m_pRendererCom, this), E_FAIL);
	FAILED_CHECK_RETURN(__super::Add_Component(CGameInstance::Get_StaticLevelIndex(), L"Prototype_Component_Shader_VtxTex", L"Com_Shader", (CComponent**)&m_pShaderCom, this), E_FAIL);
	FAILED_CHECK_RETURN(__super::Add_Component(CGameInstance::Get_StaticLevelIndex(), L"Prototype_Component_VIBuffer_Rect", L"Com_VIBuffer", (CComponent**)&m_pVIBufferCom, this), E_FAIL);
	FAILED_CHECK_RETURN(__super::Add_Component(CGameInstance::Get_StaticLevelIndex(), L"Prototype_Component_Texture_Logo_Fire", L"Com_Texture", (CComponent**)&m_pTextureCom, this), E_FAIL);

	return S_OK;
}

HRESULT CLoading_Fire::SetUp_ShaderResources()
{
	
	NULL_CHECK_RETURN(m_pShaderCom, E_FAIL);

	FAILED_CHECK_RETURN(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, L"g_WorldMatrix"), E_FAIL);
	FAILED_CHECK_RETURN(m_pShaderCom->Set_Matrix(L"g_ViewMatrix", &m_ViewMatrix), E_FAIL);
	FAILED_CHECK_RETURN(m_pShaderCom->Set_Matrix(L"g_ProjMatrix", &m_ProjMatrix), E_FAIL);

	FAILED_CHECK_RETURN(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, L"g_Texture", (_uint)m_fFrame), E_FAIL);

	return S_OK;
}

CLoading_Fire* CLoading_Fire::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CLoading_Fire*		pInstance = new CLoading_Fire(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CLoading_Fire");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CLoading_Fire::Clone(const wstring& wstrPrototypeTag, void* pArg)
{
	CLoading_Fire*		pInstance = new CLoading_Fire(*this);

	if (FAILED(pInstance->Initialize_Clone(wstrPrototypeTag, pArg)))
	{
		MSG_BOX("Failed to Cloned : CLoading_Fire");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CLoading_Fire::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pRendererCom);
}
