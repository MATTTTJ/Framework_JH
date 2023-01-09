#include "stdafx.h"
#include "..\public\Human_Sword.h"

#include "Bullet.h"
#include "GameInstance.h"
#include "Human_Sword_State.h"
#include "Player.h"

CHuman_Sword::CHuman_Sword(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CMonster(pDevice, pContext)
{
}

CHuman_Sword::CHuman_Sword(const CHuman_Sword& rhs)
	:CMonster(rhs)
{
}

HRESULT CHuman_Sword::Initialize_Prototype()
{
	m_bHasModel = true;

	FAILED_CHECK_RETURN(__super::Initialize_Prototype(), E_FAIL);

	return S_OK;
}

HRESULT CHuman_Sword::Initialize_Clone(const wstring& wstrPrototypeTag, void* pArg)
{
	ZeroMemory(&m_tMonsterOption, sizeof(MONSTEROPTION));

	if (nullptr != pArg)
		memcpy(&m_tMonsterOption, pArg, sizeof(MONSTEROPTION));
	else
	{
		m_tMonsterOption.m_bFirstSpawnType[STATE_ALREADYSPAWN] = true;
	}

	FAILED_CHECK_RETURN(__super::Initialize_Clone(wstrPrototypeTag, &m_tMonsterOption), E_FAIL);

	FAILED_CHECK_RETURN(SetUp_Components(), E_FAIL);

	m_pHuman_Sword_State = CHuman_Sword_State::Create(this, m_pState, m_pModelCom, m_pTransformCom, m_pNavigationCom);

	m_pModelCom->Set_CurAnimIndex(CHuman_Sword_State::SWORD_NODETECTED);

	// m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(0.f, 0.f, 10.f, 1.f));

	return S_OK;
}

void CHuman_Sword::Tick(_double TimeDelta)
{
	__super::Tick(TimeDelta);
	
	Set_On_NaviMesh();

	m_pState->Tick(TimeDelta);
	m_pHuman_Sword_State->Tick(TimeDelta);

	if(m_bPlayAnimation)
		m_pModelCom->Play_Animation(TimeDelta);

	Collider_Tick(TimeDelta);

	for (_uint i = 0; i < m_vecMonsterUI.size(); ++i)
	{
		m_vecMonsterUI[i]->Tick(TimeDelta);
	}



}

void CHuman_Sword::Late_Tick(_double TimeDelta)
{
	__super::Late_Tick(TimeDelta);

	m_pHuman_Sword_State->Late_Tick(TimeDelta);

	for (_uint i = 0; i < m_vecMonsterUI.size(); ++i)
	{
		m_vecMonsterUI[i]->Late_Tick(TimeDelta);
	}


	if (nullptr != m_pRendererCom &&
		true == CGameInstance::GetInstance()->isInFrustum_WorldSpace(m_pTransformCom->Get_State(CTransform::STATE_TRANSLATION), 2.f))
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHABLEND, this);
}

HRESULT CHuman_Sword::Render()
{
	FAILED_CHECK_RETURN(__super::Render(), E_FAIL);

	FAILED_CHECK_RETURN(SetUp_ShaderResources(), E_FAIL);

	_uint iNumMeshes = m_pModelCom->Get_NumMeshes();

	for(_uint i =0; i < iNumMeshes; ++i)
	{
		m_pModelCom->Bind_Material(m_pShaderCom, i, aiTextureType_DIFFUSE, L"g_DiffuseTexture");
		m_pModelCom->Bind_Material(m_pShaderCom, i, aiTextureType_NORMALS, L"g_NormalTexture");

		m_pModelCom->Render(m_pShaderCom, i, L"g_BoneMatrices");
	}

#ifdef _DEBUG
	m_pColliderCom[COLLTYPE_DETECTED]->Render();
	m_pColliderCom[COLLTYPE_HITBODY]->Render();
	m_pColliderCom[COLLTYPE_HITHEAD]->Render();
	m_pColliderCom[COLLTYPE_ATTPOS]->Render();
	m_pColliderCom[COLLTYPE_ATTRANGE]->Render();

#endif

	return S_OK;
}

void CHuman_Sword::Collider_Tick(_double TimeDelta)
{
	m_pColliderCom[COLLTYPE_DETECTED]->Update(m_pTransformCom->Get_WorldMatrix());
	m_pColliderCom[COLLTYPE_HITBODY]->Update(Get_BoneMatrix("Bip001 Spine") * CGameUtils::Get_PlayerPivotMatrix() * m_pTransformCom->Get_WorldMatrix());
	m_pColliderCom[COLLTYPE_HITHEAD]->Update(Get_BoneMatrix("Bip001 Head") * CGameUtils::Get_PlayerPivotMatrix() * m_pTransformCom->Get_WorldMatrix());
	m_pColliderCom[COLLTYPE_ATTPOS]->Update(Get_BoneMatrix("Bip001 Prop1") * CGameUtils::Get_PlayerPivotMatrix() * m_pTransformCom->Get_WorldMatrix());
	m_pColliderCom[COLLTYPE_ATTRANGE]->Update(m_pTransformCom->Get_WorldMatrix());
}

_bool CHuman_Sword::Collision_Detected(CCollider* pOtherCollider)
{
	return m_pColliderCom[COLLTYPE_DETECTED]->Collision(pOtherCollider);
}

_bool CHuman_Sword::Collider_HitHead(CCollider* pOtherCollider)
{
	return m_pColliderCom[COLLTYPE_HITHEAD]->Collision(pOtherCollider);
}

_bool CHuman_Sword::Collider_HitBody(CCollider* pOtherCollider)
{
	return m_pColliderCom[COLLTYPE_HITBODY]->Collision(pOtherCollider);
}

_bool CHuman_Sword::Collider_AttRange(CCollider* pOtherCollider)
{
	return m_pColliderCom[COLLTYPE_HITBODY]->Collision(pOtherCollider);
}

_bool CHuman_Sword::Collision_Bullet_Body(vector<CGameObject*>* pGameObject)
{
	size_t BulletCnt = (_uint)pGameObject->size();

	if(BulletCnt != 0)
	{
		auto iter = pGameObject->begin();

		for(_uint i =0; i < BulletCnt; ++i)
		{
			++iter;
		}

		if (iter == pGameObject->end())
			return false;

		CCollider* pBulletCollider = dynamic_cast<CBullet*>(*iter)->Get_ColliderPtr();

		NULL_CHECK_RETURN(pBulletCollider, false);

		return m_pColliderCom[COLLTYPE_HITBODY]->Collision(pBulletCollider);
	}
	else
		return false;
}

_bool CHuman_Sword::Collision_Bullet_Head(vector<CGameObject*>* pGameObject)
{
	_uint  BulletCnt = (_uint)pGameObject->size();

	if (BulletCnt != 0)
	{
		auto iter = pGameObject->begin();

		for (_uint i = 0; i < BulletCnt; ++i)
		{
			++iter;
		}

		if (iter == pGameObject->end())
			return false;

		CCollider* pBulletCollider = dynamic_cast<CBullet*>(*iter)->Get_ColliderPtr();

		NULL_CHECK_RETURN(pBulletCollider, false);

		return m_pColliderCom[COLLTYPE_HITHEAD]->Collision(pBulletCollider);
	}
	else
		return false;
}

void CHuman_Sword::Set_On_NaviMesh()
{
	_float fCellHeight = m_pNavigationCom->Get_CellHeight();

	_vector vPlayerPos = m_pTransformCom->Get_State(CTransform::STATE_TRANSLATION);

	vPlayerPos = vPlayerPos + XMVectorSet(0.f, fCellHeight, 0.f, 0.f);
}

HRESULT CHuman_Sword::Ready_UI()
{
	// TODO:: 몬스터 유아이 만들기

	// CGameObject*		pPlayerUI = nullptr;
	//
	// CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);
	//
	// pPlayerUI = pGameInstance->Clone_GameObject(L"Prototype_GameObject_PlayerUI_Base");
	// NULL_CHECK_RETURN(pPlayerUI, );
	//
	// m_vecMonsterUI.push_back(pPlayerUI);

	return S_OK;
}

HRESULT CHuman_Sword::SetUp_Components()
{
	FAILED_CHECK_RETURN(__super::Add_Component(CGameInstance::Get_StaticLevelIndex(), L"Prototype_Component_Renderer", L"Com_Renderer", (CComponent**)&m_pRendererCom, this), E_FAIL);
	FAILED_CHECK_RETURN(__super::Add_Component(CGameInstance::Get_StaticLevelIndex(), L"Prototype_Component_State", L"Com_State", (CComponent**)&m_pState, this), E_FAIL);
	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Shader_VtxAnimModel", L"Com_Shader", (CComponent**)&m_pShaderCom, this), E_FAIL);
	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Model_Human_Sword", L"Com_Model", (CComponent**)&m_pModelCom, this), E_FAIL);

	// _float Xmin = (_float)SHRT_MAX, Xmax = (_float)SHRT_MIN, Ymin = (_float)SHRT_MAX, Ymax = (_float)SHRT_MIN, Zmin = (_float)SHRT_MAX, Zmax = (_float)SHRT_MIN;
	// FAILED_CHECK_RETURN(m_pModelCom->Check_MeshSize("CritterSmall_01_LOD0", Xmin, Xmax, Ymin, Ymax, Zmin, Zmax), E_FAIL);

	CCollider::COLLIDERDESC	ColliderDesc;

	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));
	ColliderDesc.vSize = _float3(17.f, 17.f, 17.f);
	ColliderDesc.vPosition = _float3(0.f, 0.f, 0.f);
	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Collider_SPHERE", L"Com_DetectedSphere", (CComponent**)&m_pColliderCom[COLLTYPE_DETECTED], this, &ColliderDesc), E_FAIL);

	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));
	ColliderDesc.vSize = _float3(0.7f, 0.7f, 0.7f);
	ColliderDesc.vPosition = _float3(0.25f, 0.f, 0.f);
	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Collider_SPHERE", L"Com_HitBodySphere", (CComponent**)&m_pColliderCom[COLLTYPE_HITBODY], this, &ColliderDesc), E_FAIL);

	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));
	ColliderDesc.vSize = _float3(0.8f, 0.8f, 0.8f);
	ColliderDesc.vPosition = _float3(0.25f, 0.f, 0.f);
	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Collider_SPHERE", L"Com_HitHeadSphere", (CComponent**)&m_pColliderCom[COLLTYPE_HITHEAD], this, &ColliderDesc), E_FAIL);

	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));
	ColliderDesc.vSize = _float3(0.f, 0.f, 0.3f);
	ColliderDesc.vPosition = _float3(0.f, 0.f, -0.7f);
	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Collider_SPHERE", L"Com_AttackPosSphere", (CComponent**)&m_pColliderCom[COLLTYPE_ATTPOS], this, &ColliderDesc), E_FAIL);

	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));
	ColliderDesc.vSize = _float3(4.7f, 4.7f, 4.7f);
	ColliderDesc.vPosition = _float3(0.f, 0.f, 0.f);
	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Collider_SPHERE", L"Com_AttackRangeSphere", (CComponent**)&m_pColliderCom[COLLTYPE_ATTRANGE], this, &ColliderDesc), E_FAIL);

	CNavigation::NAVIDESC		NaviDesc;
	ZeroMemory(&NaviDesc, sizeof(CNavigation::NAVIDESC));

	NaviDesc.iCurrentIndex = 0;

	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Navigation", L"Com_Navigation", (CComponent**)&m_pNavigationCom, this, &NaviDesc), E_FAIL);

	return S_OK;
}

HRESULT CHuman_Sword::SetUp_ShaderResources()
{
	NULL_CHECK_RETURN(m_pShaderCom, E_FAIL);

	CGameInstance*		pGameInstance = CGameInstance::GetInstance();

	Safe_AddRef(pGameInstance);

	FAILED_CHECK_RETURN(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, L"g_WorldMatrix"), E_FAIL);

	m_pShaderCom->Set_Matrix(L"g_ViewMatrix", &pGameInstance->Get_TransformFloat4x4(CPipeLine::D3DTS_VIEW));
	m_pShaderCom->Set_Matrix(L"g_ProjMatrix", &pGameInstance->Get_TransformFloat4x4(CPipeLine::D3DTS_PROJ));

	Safe_Release(pGameInstance);

	return S_OK;
}

void CHuman_Sword::Collision_Event(CBullet* pBullet)
{
	CBullet::BULLETOPTION BulletDesc;
	BulletDesc = pBullet->Get_BulletOption();

	m_tMonsterOption.GameObjectDesc.m_iDamage -= BulletDesc.BulletDesc.m_iDamage;

	m_pHuman_Sword_State->Reset_Damaged();
	//
	// if(m_tMonsterOption.GameObjectDesc.m_iHP < 0)
	// {
	// 	Set_Dead(true);
	// }
}

_bool CHuman_Sword::Collision_Test()
{
	CGameInstance*			pGameInstance = GET_INSTANCE(CGameInstance);

	CCollider*		pTargetCollider = (CCollider*)pGameInstance->Get_ComponentPtr(LEVEL_GAMEPLAY, L"Layer_Bullet", L"Com_BulletSPHERE");
	NULL_CHECK_RETURN(pTargetCollider, false);

	RELEASE_INSTANCE(CGameInstance);

	return	m_pColliderCom[COLLTYPE_HITBODY]->Collision(pTargetCollider);

}


CHuman_Sword* CHuman_Sword::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CHuman_Sword*		pInstance = new CHuman_Sword(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Create : CHuman_Sword");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CHuman_Sword::Clone(const wstring& wstrPrototypeTag, void* pArg)
{
	CHuman_Sword*		pInstance = new CHuman_Sword(*this);

	if (FAILED(pInstance->Initialize_Clone(wstrPrototypeTag, pArg)))
	{
		MSG_BOX("Failed to Clone : CHuman_Sword");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CHuman_Sword::Free()
{
	__super::Free();
	
	Safe_Release(m_pHuman_Sword_State);
	Safe_Release(m_pNavigationCom);
	Safe_Release(m_pState);

	for(_uint i =0; i<COLLTYPE_END; ++i)
	{
		Safe_Release(m_pColliderCom[i]);
	}
	
	for (auto& pUI : m_vecMonsterUI)
	{
		Safe_Release(pUI);
	}
	
	Safe_Release(m_pModelCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pRendererCom);
}
