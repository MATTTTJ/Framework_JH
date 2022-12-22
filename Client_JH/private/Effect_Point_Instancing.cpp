#include "stdafx.h"
#include "..\public\Effect_Point_Instancing.h"
#include "GameInstance.h"


CEffect_Point_Instancing::CEffect_Point_Instancing(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject(pDevice, pContext)
{
}

CEffect_Point_Instancing::CEffect_Point_Instancing(const CEffect_Point_Instancing& rhs)
	: CGameObject(rhs)
{
}

HRESULT CEffect_Point_Instancing::Initialize_Prototype()
{
	FAILED_CHECK_RETURN(__super::Initialize_Prototype(), E_FAIL);

	return S_OK;
}

HRESULT CEffect_Point_Instancing::Initialize_Clone(const wstring& wstrPrototypeTag, void* pArg)
{
	CGameObject::GAMEOBJECTDESC		GameObjectDesc;
	ZeroMemory(&GameObjectDesc, sizeof(GameObjectDesc));

	GameObjectDesc.TransformDesc.fSpeedPerSec = 5.f;
	GameObjectDesc.TransformDesc.fRotationPerSec = XMConvertToRadians(90.0f);

	FAILED_CHECK_RETURN(CGameObject::Initialize_Clone(wstrPrototypeTag, pArg), E_FAIL);
	FAILED_CHECK_RETURN(SetUp_Components(), E_FAIL);

	m_pTransformCom->Set_Scaled(_float3(0.2f, 0.2f, 0.2f));
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(5.f, 0.f, 0.f, 1.f));

	return S_OK;
}

void CEffect_Point_Instancing::Tick(_double dTimeDelta)
{
	__super::Tick(dTimeDelta);

	m_pVIBufferCom->Tick(dTimeDelta);
}

void CEffect_Point_Instancing::Late_Tick(_double dTimeDelta)
{
	__super::Late_Tick(dTimeDelta);

	if (nullptr != m_pRendererCom)
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHABLEND, this);
}

HRESULT CEffect_Point_Instancing::Render()
{
	FAILED_CHECK_RETURN(__super::Render(), E_FAIL);

	FAILED_CHECK_RETURN(SetUp_ShaderResources(), E_FAIL);

	m_pShaderCom->Begin(0);

	m_pVIBufferCom->Render();

	return S_OK;
}

HRESULT CEffect_Point_Instancing::SetUp_Components()
{
	/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(CGameInstance::Get_StaticLevelIndex(), TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"),
		(CComponent**)&m_pRendererCom, this)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxPointInstance"), TEXT("Com_Shader"),
		(CComponent**)&m_pShaderCom, this)))
		return E_FAIL;


	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_VIBuffer_Point_Instancing"), TEXT("Com_VIBuffer"),
		(CComponent**)&m_pVIBufferCom, this)))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Snow"), TEXT("Com_Texture"),
		(CComponent**)&m_pTextureCom, this)))
		return E_FAIL;


	return S_OK;
}

HRESULT CEffect_Point_Instancing::SetUp_ShaderResources()
{
	if (nullptr == m_pShaderCom)
		return E_FAIL;

	// m_pShaderCom->Set_Matrix("g_WorldMatrix", &m_WorldMatrix);
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, L"g_WorldMatrix")))
		return E_FAIL;
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	if (FAILED(m_pShaderCom->Set_Matrix(L"g_ViewMatrix", &pGameInstance->Get_TransformFloat4x4(CPipeLine::D3DTS_VIEW))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_Matrix(L"g_ProjMatrix", &pGameInstance->Get_TransformFloat4x4(CPipeLine::D3DTS_PROJ))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_RawValue(L"g_vCamPosition", &pGameInstance->Get_CamPos(), sizeof(_float4))))
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);

	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, L"g_Texture")))
		return E_FAIL;

	return S_OK;
}

CEffect_Point_Instancing* CEffect_Point_Instancing::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CEffect_Point_Instancing*		pInstance = new CEffect_Point_Instancing(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CEffect_Point_Instancing");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CEffect_Point_Instancing::Clone(const wstring& wstrPrototypeTag, void* pArg)
{
	CEffect_Point_Instancing*		pInstance = new CEffect_Point_Instancing(*this);

	if (FAILED(pInstance->Initialize_Clone(wstrPrototypeTag, pArg)))
	{
		MSG_BOX("Failed to Cloned : CEffect_Point_Instancing");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CEffect_Point_Instancing::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pRendererCom);
}
