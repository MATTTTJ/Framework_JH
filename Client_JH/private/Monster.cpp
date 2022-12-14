#include "stdafx.h"
#include "..\public\Monster.h"
#include "GameInstance.h"

CMonster::CMonster(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CGameObject(pDevice, pContext)
{

}

CMonster::CMonster(const CMonster & rhs)
	: CGameObject(rhs)
{

}

HRESULT CMonster::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CMonster::Initialize_Clone(const wstring& wstrPrototypeTag, void * pArg)
{
	CGameObject::GAMEOBJECTDESC			GameObjectDesc;
	ZeroMemory(&GameObjectDesc, sizeof GameObjectDesc);

	GameObjectDesc.TransformDesc.fSpeedPerSec = 7.0f;
	GameObjectDesc.TransformDesc.fRotationPerSec = XMConvertToRadians(90.0f);

	if (FAILED(__super::Initialize_Clone(wstrPrototypeTag, &GameObjectDesc)))
		return E_FAIL;

	if (FAILED(SetUp_Components()))
		return E_FAIL;

	m_pModelCom->Set_CurAnimIndex(rand() % 10);
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(rand() % 10, 0.f, rand() % 10, 1.f));


	return S_OK;
}

void CMonster::Tick(_double TimeDelta)
{
	__super::Tick(TimeDelta);

	m_pModelCom->Play_Animation(TimeDelta, 0.1,1.0);

	for (_uint i = 0; i < COLLTYPE_END; ++i)
		m_pColliderCom[i]->Update(m_pTransformCom->Get_WorldMatrix());
}

void CMonster::Late_Tick(_double TimeDelta)
{
	__super::Late_Tick(TimeDelta);

	Collision_ToPlayer();

	if (nullptr != m_pRendererCom)
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHABLEND, this);
}

HRESULT CMonster::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	if (FAILED(SetUp_ShaderResources()))
		return E_FAIL;

	_uint iNumMeshes = m_pModelCom->Get_NumMeshes();

	for (_uint i = 0; i < iNumMeshes; ++i)
	{
		/* 이 모델을 그리기위한 셰이더에 머테리얼 텍스쳐를 전달하낟. */
		m_pModelCom->Bind_Material(m_pShaderCom, i, aiTextureType_DIFFUSE, L"g_DiffuseTexture");

		m_pModelCom->Render(m_pShaderCom, i, L"g_BoneMatrices");
	}

#ifdef _DEBUG
	for (_uint i = 0; i < COLLTYPE_END; ++i)
	{
		if (nullptr != m_pColliderCom[i])
			m_pColliderCom[i]->Render();
	}

#endif

	return S_OK;
}

HRESULT CMonster::SetUp_Components()
{
	FAILED_CHECK_RETURN(__super::Add_Component(CGameInstance::Get_StaticLevelIndex(), L"Prototype_Component_Renderer", L"Com_Renderer",	(CComponent**)&m_pRendererCom, this), E_FAIL);
	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Shader_VtxAnimModel", L"Com_Shader",(CComponent**)&m_pShaderCom, this), E_FAIL);
	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Model_Fiona", L"Com_Model", (CComponent**)&m_pModelCom, this), E_FAIL);

	CCollider::COLLIDERDESC			ColliderDesc;
	
	/* For.Com_AABB */
	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));
	ColliderDesc.vSize = _float3(0.7f, 1.5f, 0.7f);
	ColliderDesc.vPosition = _float3(0.f, ColliderDesc.vSize.y * 0.5f, 0.f);
	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Collider_AABB", L"Com_AABB", (CComponent**)&m_pColliderCom[COLLTYPE_AABB], this, &ColliderDesc), E_FAIL);
	
	/* For.Com_OBB */
	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));
	ColliderDesc.vSize = _float3(1.0f, 1.0f, 1.0f);
	ColliderDesc.vRotation = _float3(0.f, /*XMConvertToRadians(45.0f)*/0.0f, 0.f);
	ColliderDesc.vPosition = _float3(0.f, ColliderDesc.vSize.y * 0.5f, 0.f);
	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Collider_OBB", L"Com_OBB", (CComponent**)&m_pColliderCom[COLLTYPE_OBB], this, &ColliderDesc), E_FAIL);
	
	/* For.Com_SPHERE */
	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));
	ColliderDesc.vSize = _float3(0.7f, 0.7f, 0.7f);
	ColliderDesc.vPosition = _float3(0.f, ColliderDesc.vSize.y * 0.5f, 0.f);
	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Collider_SPHERE", L"Com_SPHERE", (CComponent**)&m_pColliderCom[COLLTYPE_SPHERE], this, &ColliderDesc), E_FAIL);
	
	return S_OK;
}

HRESULT CMonster::SetUp_ShaderResources()
{
	if (nullptr == m_pShaderCom)
		return E_FAIL;

	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, L"g_WorldMatrix")))
		return E_FAIL;

	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	if (FAILED(m_pShaderCom->Set_Matrix(L"g_ViewMatrix", &pGameInstance->Get_TransformFloat4x4(CPipeLine::D3DTS_VIEW))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_Matrix(L"g_ProjMatrix", &pGameInstance->Get_TransformFloat4x4(CPipeLine::D3DTS_PROJ))))
		return E_FAIL;


	/* For.Lights */
	const LIGHTDESC* pLightDesc = pGameInstance->Get_LightDesc(0);
	if (nullptr == pLightDesc)
		return E_FAIL;
	//
	//if (FAILED(m_pShaderCom->Set_RawValue("g_vLightDir", &pLightDesc->vDirection, sizeof(_float4))))
	//	return E_FAIL;
	//if (FAILED(m_pShaderCom->Set_RawValue("g_vLightDiffuse", &pLightDesc->vDiffuse, sizeof(_float4))))
	//	return E_FAIL;
	//if (FAILED(m_pShaderCom->Set_RawValue("g_vLightAmbient", &pLightDesc->vAmbient, sizeof(_float4))))
	//	return E_FAIL;
	//if (FAILED(m_pShaderCom->Set_RawValue("g_vLightSpecular", &pLightDesc->vSpecular, sizeof(_float4))))
	//	return E_FAIL;

	//if (FAILED(m_pShaderCom->Set_RawValue("g_vCamPosition", &pGameInstance->Get_CamPosition(), sizeof(_float4))))
	//	return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);





	return S_OK;
}

void CMonster::Collision_ToPlayer()
{
	CGameInstance*			pGameInstance = GET_INSTANCE(CGameInstance);

	CCollider*		pTargetCollider = (CCollider*)pGameInstance->Get_ComponentPtr(LEVEL_GAMEPLAY, L"Layer_Player", L"Com_AABB");
	if (nullptr == pTargetCollider)
		return;

	m_pColliderCom[COLLTYPE_AABB]->Collision_AABB(pTargetCollider);

	RELEASE_INSTANCE(CGameInstance);
}

CMonster * CMonster::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CMonster*		pInstance = new CMonster(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CMonster");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CMonster::Clone(const wstring& wstrPrototypeTag, void * pArg)
{
	CMonster*		pInstance = new CMonster(*this);

	if (FAILED(pInstance->Initialize_Clone(wstrPrototypeTag,pArg)))
	{
		MSG_BOX("Failed to Cloned : CMonster");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CMonster::Free()
{
	__super::Free();

	for (_uint i = 0; i < COLLTYPE_END; ++i)
		Safe_Release(m_pColliderCom[i]);

	Safe_Release(m_pModelCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pRendererCom);

}
