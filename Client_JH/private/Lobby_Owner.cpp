#include "stdafx.h"
#include "..\public\Lobby_Owner.h"

#include "Dynamic_Camera.h"
#include "GameInstance.h"

CLobby_Owner::CLobby_Owner(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CGameObject(pDevice, pContext)
{

}

CLobby_Owner::CLobby_Owner(const CLobby_Owner & rhs)
	: CGameObject(rhs)
{

}

HRESULT CLobby_Owner::Initialize_Prototype()
{
	m_bHasModel = true;

	FAILED_CHECK_RETURN(__super::Initialize_Prototype(), E_FAIL);

	return S_OK;
}

HRESULT CLobby_Owner::Initialize_Clone(const wstring& wstrPrototypeTag, void * pArg)
{
	CGameObject::GAMEOBJECTDESC		GameObjectDesc;
	ZeroMemory(&GameObjectDesc, sizeof(GAMEOBJECTDESC));
	FAILED_CHECK_RETURN(__super::Initialize_Clone(wstrPrototypeTag, &GameObjectDesc), E_FAIL);

	FAILED_CHECK_RETURN(SetUp_Components(), E_FAIL);

	m_pModelCom->Set_CurAnimIndex(0);
	CGameInstance::GetInstance()->Play_Sound(L"Lobby_Test.mp3", 0.5f, true, false, 1);

	return S_OK;
}

void CLobby_Owner::Tick(_double TimeDelta)
{
	__super::Tick(TimeDelta);
	
	m_pModelCom->Play_Animation(TimeDelta);

}

void CLobby_Owner::Late_Tick(_double TimeDelta)
{
	__super::Late_Tick(TimeDelta);

	if (nullptr != m_pRendererCom)
	{
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_OUTLINE, this);

		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_DYNAMIC_SHADOWDEPTH, this);

		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHABLEND, this);
	}
}

HRESULT CLobby_Owner::Render()
{
	FAILED_CHECK_RETURN(__super::Render(), E_FAIL);

	_uint iNumMeshes = m_pModelCom->Get_NumMeshes();

	for (_uint i = 0; i < iNumMeshes; ++i)
	{
		m_pModelCom->Bind_Material(m_pShaderCom, i, aiTextureType_DIFFUSE, L"g_DiffuseTexture");
		if (i == 1)
		{
			m_pNormalTex->Bind_ShaderResource(m_pShaderCom, L"g_MonsterNormalTexture", 0);
			m_bNormalTexOn = true;
		}
		else
			m_bNormalTexOn = false;

		FAILED_CHECK_RETURN(SetUp_ShaderResources(), E_FAIL);

		m_pModelCom->Render(m_pShaderCom, i, L"g_BoneMatrices",2);
	}

	return S_OK;
}

HRESULT CLobby_Owner::Render_ShadowDepth()
{
	if (nullptr == m_pShaderCom ||
		nullptr == m_pModelCom)
		return E_FAIL;

	CGameInstance*		pGameInstance = CGameInstance::GetInstance();
	FAILED_CHECK_RETURN(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, L"g_WorldMatrix"), E_FAIL);
	m_pShaderCom->Set_Matrix(L"g_ViewMatrix", &pGameInstance->Get_LightTransform(1, 0)); // D3DTS_VIEW
	m_pShaderCom->Set_Matrix(L"g_ProjMatrix", &pGameInstance->Get_LightTransform(1, 1)); // D3DTS_PROJ

	_uint iNumMeshes = m_pModelCom->Get_NumMeshes();

	for (_uint i = 0; i < iNumMeshes; ++i)
	{
		m_pModelCom->Render(m_pShaderCom, i, L"g_BoneMatrices", 5);
	}

	return S_OK;
}

HRESULT CLobby_Owner::Render_OutLineFlag()
{
	if (nullptr == m_pShaderCom ||
		nullptr == m_pModelCom)
		return E_FAIL;

	/* 셰이더 전역변수에 값을 던진다. */
	FAILED_CHECK_RETURN(SetUp_ShaderResources(), E_FAIL);
	FAILED_CHECK_RETURN(m_pShaderCom->Set_RawValue(L"g_vColor", &XMVectorSet(1.f, 0.3f, 0.3f, 1.f), sizeof(_float4)), E_FAIL);



	_uint iNumMeshes = m_pModelCom->Get_NumMeshes();

	for (_uint i = 0; i < iNumMeshes; ++i)
	{
		m_pModelCom->Render(m_pShaderCom, i, L"g_BoneMatrices", 6);
	}

	return S_OK;
}

HRESULT CLobby_Owner::SetUp_Components()
{
	FAILED_CHECK_RETURN(__super::Add_Component(CGameInstance::Get_StaticLevelIndex(), L"Prototype_Component_Renderer", L"Com_Renderer",	(CComponent**)&m_pRendererCom, this), E_FAIL);
	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Shader_VtxAnimModel", L"Com_Shader", (CComponent**)&m_pShaderCom, this), E_FAIL);
	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Model_LobbyOwner", L"Com_Model", (CComponent**)&m_pModelCom, this), E_FAIL);
	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_LobbyOwner_NormalTex", L"Com_NormalTex", (CComponent**)&m_pNormalTex, this), E_FAIL);


	
	return S_OK;
}

HRESULT CLobby_Owner::SetUp_ShaderResources()
{
	NULL_CHECK_RETURN(m_pShaderCom, E_FAIL);

	FAILED_CHECK_RETURN(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, L"g_WorldMatrix"), E_FAIL);

	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	FAILED_CHECK_RETURN(m_pShaderCom->Set_Matrix(L"g_ViewMatrix", &pGameInstance->Get_TransformFloat4x4(CPipeLine::D3DTS_VIEW)), E_FAIL);
	// FAILED_CHECK_RETURN(m_pShaderCom->Set_ShaderResourceView(L"g_OutLineTexture", CGameInstance::GetInstance()->Get_OutLineTargetSRV()), E_FAIL);
	FAILED_CHECK_RETURN(m_pShaderCom->Set_RawValue(L"g_bNormalTexOn", &m_bNormalTexOn, sizeof(_bool)), E_FAIL);

	FAILED_CHECK_RETURN(m_pShaderCom->Set_Matrix(L"g_ProjMatrix", &pGameInstance->Get_TransformFloat4x4(CPipeLine::D3DTS_PROJ)), E_FAIL);
	FAILED_CHECK_RETURN(m_pShaderCom->Set_Matrix(L"g_ProjMatrix", &pGameInstance->Get_TransformFloat4x4(CPipeLine::D3DTS_PROJ)), E_FAIL);

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

CLobby_Owner * CLobby_Owner::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CLobby_Owner*		pInstance = new CLobby_Owner(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CLobby_Owner");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CLobby_Owner::Clone(const wstring& wstrPrototypeTag, void * pArg)
{
	CLobby_Owner*		pInstance = new CLobby_Owner(*this);

	if (FAILED(pInstance->Initialize_Clone(wstrPrototypeTag, pArg)))
	{
		MSG_BOX("Failed to Cloned : CLobby_Owner");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CLobby_Owner::Free()
{

	__super::Free();

	Safe_Release(m_pModelCom);
	Safe_Release(m_pNormalTex);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pRendererCom);

}
