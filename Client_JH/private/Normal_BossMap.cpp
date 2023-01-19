#include "stdafx.h"
#include "..\public\Normal_BossMap.h"
#include "GameInstance.h"

CNormal_BossMap::CNormal_BossMap(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CGameObject(pDevice,pContext)
{
}

CNormal_BossMap::CNormal_BossMap(const CNormal_BossMap& rhs)
	:CGameObject(rhs)
{
}

HRESULT CNormal_BossMap::Initialize_Prototype()
{
	m_bHasModel = true;

	FAILED_CHECK_RETURN(__super::Initialize_Prototype(), E_FAIL);

	return S_OK;
}

HRESULT CNormal_BossMap::Initialize_Clone(const wstring& wstrPrototypeTag, void* pArg)
{
	FAILED_CHECK_RETURN(__super::Initialize_Clone(wstrPrototypeTag, pArg), E_FAIL);

	FAILED_CHECK_RETURN(SetUp_Components(), E_FAIL);
	// PivotMatrix.r[3] = XMVectorSet(82.f, 0.f, -30.f, 117.f);
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(120.f, 0.f, 135.f, 1.f));

	return S_OK;
}

void CNormal_BossMap::Tick(_double TimeDelta)
{
	__super::Tick(TimeDelta);
}

void CNormal_BossMap::Late_Tick(_double TimeDelta)
{
	__super::Late_Tick(TimeDelta);

	if (nullptr != m_pRendererCom)
	{
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHABLEND, this);
		// m_pRendererCom->Add_DebugRenderGroup(m_pNavigationCom);
	}
}

HRESULT CNormal_BossMap::Render()
{
	FAILED_CHECK_RETURN(__super::Render(), E_FAIL);
	FAILED_CHECK_RETURN(SetUp_ShaderResources(), E_FAIL);

	_uint iNumMeshes = m_pModelCom->Get_NumMeshes();

	for (_uint i = 0; i < iNumMeshes; ++i)
	{
		HRESULT hr = m_pModelCom->Bind_Material(m_pShaderCom, i, aiTextureType_NORMALS, L"g_NormalTexture");
		if (hr == S_FALSE)
		{
			m_pModelCom->Bind_Material(m_pShaderCom, i, aiTextureType_DIFFUSE, L"g_DiffuseTexture");
			m_bNormalTexOn = false;
		}
		else if (hr == S_OK)
		{
			m_pModelCom->Bind_Material(m_pShaderCom, i, aiTextureType_DIFFUSE, L"g_DiffuseTexture");
			m_bNormalTexOn = true;
		}
		m_pModelCom->Render(m_pShaderCom, i);
	}

	return S_OK;
}

pair<_bool, _float3> CNormal_BossMap::Picking_Mesh()
{
	return m_pModelCom->Picking(g_hWnd, m_pTransformCom);
}

HRESULT CNormal_BossMap::SetUp_Components()
{
	FAILED_CHECK_RETURN(__super::Add_Component(CGameInstance::Get_StaticLevelIndex(), L"Prototype_Component_Renderer", L"Com_Renderer",(CComponent**)&m_pRendererCom, this), E_FAIL);
	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Shader_VtxNonAnim", L"Com_Shader",(CComponent**)&m_pShaderCom, this), E_FAIL);
	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Model_Normal_BossMap", L"Com_Model", (CComponent**)&m_pModelCom, this), E_FAIL);
	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Navigation", L"Com_Navigation", (CComponent**)&m_pNavigationCom, this), E_FAIL);

	return S_OK;
}

HRESULT CNormal_BossMap::SetUp_ShaderResources()
{
	NULL_CHECK_RETURN(m_pShaderCom, E_FAIL);

	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);
	FAILED_CHECK_RETURN(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, L"g_WorldMatrix"), E_FAIL);
	FAILED_CHECK_RETURN(m_pShaderCom->Set_Matrix(L"g_ViewMatrix", &pGameInstance->Get_TransformFloat4x4(CPipeLine::D3DTS_VIEW)), E_FAIL);
	FAILED_CHECK_RETURN(m_pShaderCom->Set_Matrix(L"g_ProjMatrix", &pGameInstance->Get_TransformFloat4x4(CPipeLine::D3DTS_PROJ)), E_FAIL);
	FAILED_CHECK_RETURN(m_pShaderCom->Set_RawValue(L"g_bNormalTexOn", &m_bNormalTexOn, sizeof(_bool)),E_FAIL);

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

CNormal_BossMap* CNormal_BossMap::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CNormal_BossMap*		pInstance = new CNormal_BossMap(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CNormal_BossMap");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CNormal_BossMap::Clone(const wstring& wstrPrototypeTag, void* pArg)
{
	CNormal_BossMap*		pInstance = new CNormal_BossMap(*this);

	if (FAILED(pInstance->Initialize_Clone(wstrPrototypeTag, pArg)))
	{
		MSG_BOX("Failed to Created : CNormal_BossMap");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CNormal_BossMap::Free()
{
	__super::Free();

	Safe_Release(m_pNavigationCom);
	Safe_Release(m_pModelCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pRendererCom);
}
