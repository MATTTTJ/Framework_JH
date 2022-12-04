#include "stdafx.h"
#include "..\public\Terrain.h"
#include "GameInstance.h"
CTerrain::CTerrain(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CGameObject(pDevice, pContext)
	
{

}

CTerrain::CTerrain(const CTerrain & rhs)
	: CGameObject(rhs)
{
}

HRESULT CTerrain::Initialize_Prototype()
{
	FAILED_CHECK_RETURN(__super::Initialize_Prototype(), E_FAIL);

	return S_OK;
}

HRESULT CTerrain::Initialize_Clone(void * pArg)
{
	FAILED_CHECK_RETURN(__super::Initialize_Clone(pArg), E_FAIL);

	FAILED_CHECK_RETURN(SetUp_Components(), E_FAIL);

	m_bHasModel = false;

	m_vPos = { 0.f,0.f,0.f,0.f };

	return S_OK;
}

void CTerrain::Tick(_double TimeDelta)
{
	__super::Tick(TimeDelta);
	CGameInstance*	pGameInstance = GET_INSTANCE(CGameInstance);

	if(pGameInstance->Get_DIMouseState(CInput_Device::DIM_LB))
	{
		 m_vPos = Picking_Terrain();
	}
	ImGui::Begin("Test");
	ImGui::Text("%f / %f ", m_vPos.x, m_vPos.z);
	ImGui::End();

	RELEASE_INSTANCE(CGameInstance)
}

void CTerrain::Late_Tick(_double TimeDelta)
{
	__super::Late_Tick(TimeDelta);

	if (nullptr != m_pRendererCom)
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_PRIORITY, this);
}

HRESULT CTerrain::Render()
{
	FAILED_CHECK_RETURN(__super::Render(), E_FAIL);
	FAILED_CHECK_RETURN(SetUp_ShaderResources(), E_FAIL);

	m_pShaderCom->Begin(0);

	m_pVIBufferCom->Render();

	return S_OK;
}

_float4 CTerrain::Picking_Terrain()
{
	return	m_pVIBufferCom->PickingOnTerrain(g_hWnd, m_pTransformCom->Get_WorldMatrix_Inverse());
}

HRESULT CTerrain::SetUp_Components()
{
	/* For.Com_Renderer */
	FAILED_CHECK_RETURN(__super::Add_Component(CGameInstance::Get_StaticLevelIndex(), L"Prototype_Component_Renderer", L"Com_Renderer",	(CComponent**)&m_pRendererCom), E_FAIL);

	/* For.Com_Shader */
	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Shader_VtxNorTex",L"Com_Shader",
		(CComponent**)&m_pShaderCom), E_FAIL);				  
															  
	/* For.Com_VIBuffer */									  
	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_VIBuffer_Terrain",L"Com_VIBuffer",
		(CComponent**)&m_pVIBufferCom), E_FAIL);			  
															  
	/* For.Com_Texture */									  
	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Texture_Terrain", L"Com_Texture",
		(CComponent**)&m_pTextureCom[TYPE_DIFFUSE]), E_FAIL); 
															  
	/* For.Com_Brush*/										  
	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Texture_Brush", L"Com_Brush",
		(CComponent**)&m_pTextureCom[TYPE_BRUSH]), E_FAIL);	   
															   
	/* For.Com_Filter */									   
	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Texture_Filter", L"Com_Filter",
		(CComponent**)&m_pTextureCom[TYPE_FILTER]), E_FAIL);

	return S_OK;
}

HRESULT CTerrain::SetUp_ShaderResources()
{
	NULL_CHECK_RETURN(m_pShaderCom, E_FAIL);

	FAILED_CHECK_RETURN(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, L"g_WorldMatrix"), E_FAIL);

	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	FAILED_CHECK_RETURN(m_pShaderCom->Set_Matrix(L"g_ViewMatrix", &pGameInstance->Get_TransformFloat4x4(CPipeLine::D3DTS_VIEW)), E_FAIL);
	FAILED_CHECK_RETURN(m_pShaderCom->Set_Matrix(L"g_ProjMatrix", &pGameInstance->Get_TransformFloat4x4(CPipeLine::D3DTS_PROJ)), E_FAIL);

	/* For.Lights */
	const LIGHTDESC* pLightDesc = pGameInstance->Get_LightDesc(0);
	NULL_CHECK_RETURN(pLightDesc, E_FAIL);

	FAILED_CHECK_RETURN(m_pShaderCom->Set_RawValue(L"g_vLightDir", &pLightDesc->vDirection, sizeof(_float4)), E_FAIL);
	FAILED_CHECK_RETURN(m_pShaderCom->Set_RawValue(L"g_vLightDiffuse", &pLightDesc->vDiffuse, sizeof(_float4)), E_FAIL);
	FAILED_CHECK_RETURN(m_pShaderCom->Set_RawValue(L"g_vLightAmbient", &pLightDesc->vAmbient, sizeof(_float4)), E_FAIL);
	FAILED_CHECK_RETURN(m_pShaderCom->Set_RawValue(L"g_vLightSpecular", &pLightDesc->vSpecular, sizeof(_float4)), E_FAIL);
	FAILED_CHECK_RETURN(m_pShaderCom->Set_RawValue(L"g_vCamPosition", &pGameInstance->Get_CamPos(), sizeof(_float4)), E_FAIL);

	RELEASE_INSTANCE(CGameInstance);

	FAILED_CHECK_RETURN(m_pTextureCom[TYPE_DIFFUSE]->Bind_ShaderResources(m_pShaderCom, L"g_DiffuseTexture"), E_FAIL);
	FAILED_CHECK_RETURN(m_pTextureCom[TYPE_BRUSH]->Bind_ShaderResource(m_pShaderCom, L"g_BrushTexture", 0), E_FAIL);
	FAILED_CHECK_RETURN(m_pTextureCom[TYPE_FILTER]->Bind_ShaderResource(m_pShaderCom, L"g_FilterTexture", 0), E_FAIL);
	FAILED_CHECK_RETURN(m_pShaderCom->Set_RawValue(L"g_vBrushPos", &_float4(15.f, 0.f, 15.f, 1.f), sizeof(_float4)), E_FAIL);

	return S_OK;
}

CTerrain * CTerrain::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CTerrain*		pInstance = new CTerrain(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CTerrain");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CTerrain::Clone(void * pArg)
{
	CTerrain*		pInstance = new CTerrain(*this);

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSG_BOX("Failed to Cloned : CTerrain");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CTerrain::Free()
{
	__super::Free();

	for (auto& pTextureCom : m_pTextureCom)
		Safe_Release(pTextureCom);

	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pRendererCom);

}
