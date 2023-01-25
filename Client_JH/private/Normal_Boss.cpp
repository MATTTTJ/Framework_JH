#include "stdafx.h"
#include "..\public\Normal_Boss.h"
#include "GameInstance.h"
#include "Player.h"
#include "Boss_Golem_State.h"
#include "Bullet.h"
#include "StonePillar.h"
#include "UI.h"


CNormal_Boss::CNormal_Boss(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CMonster(pDevice, pContext)
{
}

CNormal_Boss::CNormal_Boss(const CNormal_Boss& rhs)
	: CMonster(rhs)
{
}

HRESULT CNormal_Boss::Render_UI()
{
	CUI* pMonsterUI = nullptr;

	CGameInstance* pInst = CGameInstance::GetInstance();

	CUI::MONSTERUIDESC MonsterUIDesc;
	MonsterUIDesc.pMonster = this;
	MonsterUIDesc.pModel = m_pModelCom;

	pMonsterUI = dynamic_cast<CUI*>(pInst->Clone_GameObject(L"Prototype_GameObject_BossUI_Base", &MonsterUIDesc));
	NULL_CHECK_RETURN(pMonsterUI, E_FAIL);
	pMonsterUI->Set_Monster(this, m_pModelCom);
	m_vecMonsterUI.push_back(pMonsterUI);

	pMonsterUI = dynamic_cast<CUI*>(pInst->Clone_GameObject(L"Prototype_GameObject_BossUI_Red", &MonsterUIDesc));
	NULL_CHECK_RETURN(pMonsterUI, E_FAIL);
	pMonsterUI->Set_Monster(this, m_pModelCom);
	m_vecMonsterUI.push_back(pMonsterUI);

	return S_OK;
}

HRESULT CNormal_Boss::Initialize_Prototype()
{
	m_bHasModel = true;

	FAILED_CHECK_RETURN(__super::Initialize_Prototype(), E_FAIL);

	return S_OK;
}

HRESULT CNormal_Boss::Initialize_Clone(const wstring& wstrPrototypeTag, void* pArg)
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
	m_pGolem_State = CBoss_Golem_State::Create(this, m_pState, m_pModelCom, m_pTransformCom, m_pNavigationCom);

	FAILED_CHECK_RETURN(Ready_UI(), E_FAIL);

	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(110.f, 1.3f, 179.f, 1.f));

	if (m_tMonsterOption.m_bFirstSpawnType[STATE_NODETECTED] == true)
	{
		m_pModelCom->Set_CurAnimIndex(CBoss_Golem_State::GOLEM_INTRO0);
	}

	return S_OK;
}

void CNormal_Boss::Tick(_double TimeDelta)
{
	__super::Tick(TimeDelta);
	

	// _float4x4 tmp = m_pModelCom->Get_BonePtr("Bip001")->Get_CombindMatrix();
	// // _float3 Dest;
	// // XMStoreFloat3(&Dest, XMVectorSet(116.f, 1.3f, 179.f, 1.f));
	// tmp._41 = 116.f;
	// tmp._42 = 1.3f;
	// tmp._43 = 179.f;
	// m_pModelCom->Get_BonePtr("Bip 001")->Set_TransformMatrix(tmp);

	if (m_bIsDead)
	{
		Set_Dead(true);
		return;
	}

	m_pState->Tick(TimeDelta);
	m_pGolem_State->Tick(TimeDelta);

	if (m_bPlayAnimation)
		m_pModelCom->Play_Animation(TimeDelta, m_eLerpType, L"Bip001");

	Collider_Tick(TimeDelta);

	_uint UISize = (_uint)m_vecMonsterUI.size();
	
	for (_uint i = 0; i < UISize; ++i)
	{
		m_vecMonsterUI[i]->Tick(TimeDelta);
	}

	if(CGameInstance::GetInstance()->Key_Down(DIK_F5))
	{
		m_tMonsterOption.MonsterDesc.m_iHP -= 30;
	}
}

void CNormal_Boss::Late_Tick(_double TimeDelta)
{
	__super::Late_Tick(TimeDelta);

	m_pGolem_State->Late_Tick(TimeDelta);

	_uint UISize = (_uint)m_vecMonsterUI.size();

	for (_uint i = 0; i < UISize; ++i)
	{
		m_vecMonsterUI[i]->Late_Tick(TimeDelta);
	}

	if (nullptr != m_pRendererCom &&
		true == CGameInstance::GetInstance()->isInFrustum_WorldSpace(m_pTransformCom->Get_State(CTransform::STATE_TRANSLATION), 2.f))
	{
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHABLEND, this);
		m_pRendererCom->Add_DebugRenderGroup(m_pColliderCom[COLLTYPE_DETECTED]);
		m_pRendererCom->Add_DebugRenderGroup(m_pColliderCom[COLLTYPE_HITBODY]);
		m_pRendererCom->Add_DebugRenderGroup(m_pColliderCom[COLLTYPE_HITHEAD]);
		m_pRendererCom->Add_DebugRenderGroup(m_pColliderCom[COLLTYPE_ATTPOS]);
		m_pRendererCom->Add_DebugRenderGroup(m_pColliderCom[COLLTYPE_ATTRANGE]);
		m_pRendererCom->Add_DebugRenderGroup(m_pColliderCom[COLLTYPE_ONAIM]);
		m_pRendererCom->Add_DebugRenderGroup(m_pLeftArmColliderCom);
		m_pRendererCom->Add_DebugRenderGroup(m_pRightArmColliderCom);
	}
}

HRESULT CNormal_Boss::Render()
{
	FAILED_CHECK_RETURN(__super::Render(), E_FAIL);

	FAILED_CHECK_RETURN(SetUp_ShaderResources(), E_FAIL);

	_uint iNumMeshes = m_pModelCom->Get_NumMeshes();

	// _uint UISize = (_uint)m_vecMonsterUI.size();
	//
	// for (_uint i = 0; i < UISize; ++i)
	// {
	// 	m_vecMonsterUI[i]->Render();
	// }

	for (_uint i = 0; i < iNumMeshes; ++i)
	{
		m_pModelCom->Bind_Material(m_pShaderCom, i, aiTextureType_DIFFUSE, L"g_DiffuseTexture");

		m_pModelCom->Render(m_pShaderCom, i, L"g_BoneMatrices", 3);
	}

	return S_OK;
}

HRESULT CNormal_Boss::Ready_UI()
{
	

	return S_OK;
}

void CNormal_Boss::Collider_Tick(_double TimeDelta)
{
	m_pColliderCom[COLLTYPE_DETECTED]->Update(m_pTransformCom->Get_WorldMatrix());
	m_pColliderCom[COLLTYPE_HITBODY]->Update(Get_BoneMatrix("Bip001 Spine") * CGameUtils::Get_PlayerPivotMatrix() * m_pTransformCom->Get_WorldMatrix());
	m_pColliderCom[COLLTYPE_HITHEAD]->Update(Get_BoneMatrix("Bip001 Head") * CGameUtils::Get_PlayerPivotMatrix() * m_pTransformCom->Get_WorldMatrix());
	m_pLeftArmColliderCom->Update(Get_BoneMatrix("Bip001 L UpperArm") * CGameUtils::Get_PlayerPivotMatrix() * m_pTransformCom->Get_WorldMatrix());
	m_pRightArmColliderCom->Update(Get_BoneMatrix("Bip001 R UpperArm") * CGameUtils::Get_PlayerPivotMatrix() * m_pTransformCom->Get_WorldMatrix());
	m_pColliderCom[COLLTYPE_ATTRANGE]->Update(m_pTransformCom->Get_WorldMatrix());
	// m_pColliderCom[COLLTYPE_ONAIM]->Update(m_pTransformCom->Get_WorldMatrix());
}

_bool CNormal_Boss::Collision_Detected(CCollider* pOtherCollider)
{
	return m_pColliderCom[COLLTYPE_DETECTED]->Collision(pOtherCollider);
}

_bool CNormal_Boss::Collider_AttRange(CCollider* pOtherCollider)
{
	return m_pColliderCom[COLLTYPE_HITBODY]->Collision(pOtherCollider);
}

void CNormal_Boss::Set_On_NaviMesh()
{
	_float4 MonsterPos = m_pTransformCom->Get_State(CTransform::STATE_TRANSLATION);
	MonsterPos = m_pNavigationCom->Get_CellHeight(MonsterPos);
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, MonsterPos);
}

void CNormal_Boss::Collision_Body(CBullet* pBullet)
{
	// CBullet::BULLETOPTION BulletDesc;
	// BulletDesc = pBullet->Get_BulletOption();
	//
	// if (m_tMonsterOption.MonsterDesc.m_iHP >= 0)
	// 	m_tMonsterOption.MonsterDesc.m_iHP -= BulletDesc.BulletDesc.m_iDamage;
	// else if (m_tMonsterOption.MonsterDesc.m_iHP <= 0)
	// 	Set_Dead(true);
	//
	// m_pGolem_State->Reset_Damaged();
	// m_pGolem_State->Set_DamagedState(CBoss_Golem_State::HIT);
	// m_pGolem_State->Set_DamagedState(CBoss_Golem_State::HITBODY);
}

void CNormal_Boss::Collision_Head(CBullet* pBullet)
{
	// CBullet::BULLETOPTION BulletDesc;
	// BulletDesc = pBullet->Get_BulletOption();
	//
	// if (m_tMonsterOption.MonsterDesc.m_iHP >= 0)
	// 	m_tMonsterOption.MonsterDesc.m_iHP -= BulletDesc.BulletDesc.m_iDamage * 2;
	// else if (m_tMonsterOption.MonsterDesc.m_iHP <= 0)
	// 	Set_Dead(true);
	//
	// m_pGolem_State->Reset_Damaged();
	// m_pGolem_State->Set_DamagedState(CBoss_Golem_State::HIT);
	// m_pGolem_State->Set_DamagedState(CBoss_Golem_State::HITHEAD);
}

void CNormal_Boss::Collision_Hide(CBullet* pBullet)
{
	
}

void CNormal_Boss::Collision_PlayerEyes()
{
}

void CNormal_Boss::Collision_Shield(CBullet* pBullet)
{
	// CBullet::BULLETOPTION BulletDesc;
	// BulletDesc = pBullet->Get_BulletOption();
	//
	// if (m_tMonsterOption.MonsterDesc.m_iHP >= 0)
	// 	m_tMonsterOption.MonsterDesc.m_iHP -= (_int)(BulletDesc.BulletDesc.m_iDamage * 0.5f);
	// else if (m_tMonsterOption.MonsterDesc.m_iHP <= 0)
	// 	Set_Dead(true);
	//
	// m_pGolem_State->Set_DamagedState(CBoss_Golem_State::HIT);
}

void CNormal_Boss::Collision_Armor(CBullet* pBullet)
{
	// CBullet::BULLETOPTION BulletDesc;
	// BulletDesc = pBullet->Get_BulletOption();
	//
	// if (m_tMonsterOption.MonsterDesc.m_iHP >= 0)
	// 	m_tMonsterOption.MonsterDesc.m_iHP -= (_int)(BulletDesc.BulletDesc.m_iDamage * 0.7);
	// else if (m_tMonsterOption.MonsterDesc.m_iHP <= 0)
	// 	Set_Dead(true);
	//
	// m_pGolem_State->Set_DamagedState(CBoss_Golem_State::HIT);
}

_bool CNormal_Boss::Collision_StonePillars()
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	// 몬스터 리스트를 가져와서 순회해야할것같음
	list<CGameObject*>* CloneStonePillars = CGameInstance::GetInstance()->Get_CloneObjectList(LEVEL_GAMEPLAY, L"Layer_Pillars");
	if (CloneStonePillars == nullptr || CloneStonePillars->size() == 0)
		return false;
	else
	{
		m_StonePillarList = *pGameInstance->Get_CloneObjectList(LEVEL_GAMEPLAY, L"Layer_Pillars");
		_int ListSize = 0;
		ListSize = (_int)m_StonePillarList.size();

		if (ListSize != 0)
		{
			auto iter = m_StonePillarList.begin();
			for (auto& iter = m_StonePillarList.begin(); iter != m_StonePillarList.end();)
			{
				if (iter == m_StonePillarList.end())
					return false;

				CCollider* pCollider = (CCollider*)(*iter)->Find_Component(L"Com_StonePillarColl");

				if (pCollider == nullptr)
					++iter;
				else if (m_pColliderCom[COLLTYPE_DETECTED]->Collision(pCollider))
				{
					CStonePillar* pStonePillar = (CStonePillar*)pCollider->Get_Owner();
					NULL_CHECK_RETURN(pStonePillar, false);

					pStonePillar->Collision_To_Golem(); // 총알이 어디 충돌했는지 판단하니까

					++iter;
					continue;
				}
				else
					++iter;
			}
			return false;
		}
		return false;

	}


}

HRESULT CNormal_Boss::SetUp_Components()
{
	FAILED_CHECK_RETURN(__super::Add_Component(CGameInstance::Get_StaticLevelIndex(), L"Prototype_Component_Renderer", L"Com_Renderer", (CComponent**)&m_pRendererCom, this), E_FAIL);
	FAILED_CHECK_RETURN(__super::Add_Component(CGameInstance::Get_StaticLevelIndex(), L"Prototype_Component_State", L"Com_State", (CComponent**)&m_pState, this), E_FAIL);
	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Shader_VtxAnimModel", L"Com_Shader", (CComponent**)&m_pShaderCom, this), E_FAIL);
	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Model_Normal_Boss", L"Com_Model", (CComponent**)&m_pModelCom, this), E_FAIL);


	// _float Xmin = (_float)SHRT_MAX, Xmax = (_float)SHRT_MIN, Ymin = (_float)SHRT_MAX, Ymax = (_float)SHRT_MIN, Zmin = (_float)SHRT_MAX, Zmax = (_float)SHRT_MIN;
	// FAILED_CHECK_RETURN(m_pModelCom->Check_MeshSize("CritterSmall_01_LOD0", Xmin, Xmax, Ymin, Ymax, Zmin, Zmax), E_FAIL);

	CCollider::COLLIDERDESC	ColliderDesc;

	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));
	ColliderDesc.vSize = _float3(165.f, 15.f, 15.f);
	ColliderDesc.vPosition = _float3(0.f, 0.f, 0.f);
	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Collider_SPHERE", L"Com_DetectedSphere", (CComponent**)&m_pColliderCom[COLLTYPE_DETECTED], this, &ColliderDesc), E_FAIL);

	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));
	ColliderDesc.vSize = _float3(0.01f, 0.01f, 0.01f);
	ColliderDesc.vPosition = _float3(0.25f, 0.f, 0.f);
	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Collider_SPHERE", L"Com_HitBodySphere", (CComponent**)&m_pColliderCom[COLLTYPE_HITBODY], this, &ColliderDesc), E_FAIL);

	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));
	ColliderDesc.vSize = _float3(0.01f, 0.5f, 0.5f);
	ColliderDesc.vPosition = _float3(0.1f, 0.f, 0.f);
	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Collider_SPHERE", L"Com_HitHeadSphere", (CComponent**)&m_pColliderCom[COLLTYPE_HITHEAD], this, &ColliderDesc), E_FAIL);

	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));
	ColliderDesc.vSize = _float3(0.f, 0.f, 0.3f);
	ColliderDesc.vPosition = _float3(0.f, 0.f, -1.f);
	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Collider_SPHERE", L"Com_AttackPosSphere", (CComponent**)&m_pColliderCom[COLLTYPE_ATTPOS], this, &ColliderDesc), E_FAIL);

	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));
	ColliderDesc.vSize = _float3(4.7f, 4.7f, 4.7f);
	ColliderDesc.vPosition = _float3(0.f, 0.f, 0.f);
	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Collider_SPHERE", L"Com_AttackRangeSphere", (CComponent**)&m_pColliderCom[COLLTYPE_ATTRANGE], this, &ColliderDesc), E_FAIL);

	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));
	ColliderDesc.vSize = _float3(2.f, 2.f, 2.f);
	ColliderDesc.vPosition = _float3(0.f, 1.5f, 0.f);
	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Collider_SPHERE", L"Com_OnAimSphere", (CComponent**)&m_pColliderCom[COLLTYPE_ONAIM], this, &ColliderDesc), E_FAIL);

	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));
	ColliderDesc.vSize = _float3(0.01f, 0.7f, 0.7f);
	ColliderDesc.vPosition = _float3(0.f, 0.f, 0.f);
	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Collider_SPHERE", L"Com_LArmSphere", (CComponent**)&m_pLeftArmColliderCom, this, &ColliderDesc), E_FAIL);

	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));
	ColliderDesc.vSize = _float3(0.01f, 0.7f, 0.7f);
	ColliderDesc.vPosition = _float3(0.f, 0.f, 0.f);
	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Collider_SPHERE", L"Com_RArmSphere", (CComponent**)&m_pRightArmColliderCom, this, &ColliderDesc), E_FAIL);

	// CNavigation::NAVIDESC		NaviDesc;
	// ZeroMemory(&NaviDesc, sizeof(CNavigation::NAVIDESC));
	//
	// NaviDesc.iCurrentIndex = m_tMonsterOption.m_iCellIndex;
	//
	// FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Navigation", L"Com_Navigation", (CComponent**)&m_pNavigationCom, this, &NaviDesc), E_FAIL);

	return S_OK;
}

HRESULT CNormal_Boss::SetUp_ShaderResources()
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

CNormal_Boss* CNormal_Boss::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CNormal_Boss*		pInstance = new CNormal_Boss(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Create : CNormal_Boss");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CNormal_Boss::Clone(const wstring& wstrPrototypeTag, void* pArg)
{
	CNormal_Boss*		pInstance = new CNormal_Boss(*this);

	if (FAILED(pInstance->Initialize_Clone(wstrPrototypeTag, pArg)))
	{
		MSG_BOX("Failed to Clone : CNormal_Boss");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CNormal_Boss::Free()
{
	__super::Free();

	Safe_Release(m_pGolem_State);
	Safe_Release(m_pNavigationCom);
	Safe_Release(m_pState);

	for (_uint i = 0; i < COLLTYPE_END; ++i)
	{
		Safe_Release(m_pColliderCom[i]);
	}
	Safe_Release(m_pLeftArmColliderCom);
	Safe_Release(m_pRightArmColliderCom);
	for (auto& pUI : m_vecMonsterUI)
	{
		Safe_Release(pUI);
	}

	Safe_Release(m_pModelCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pRendererCom);
}
