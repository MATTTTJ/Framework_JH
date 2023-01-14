#include "stdafx.h"
#include "..\public\Human_Spear.h"

#include "Bullet.h"
#include "GameInstance.h"
#include "Player.h"
#include "Human_Spear_State.h"
#include "UI.h"

CHuman_Spear::CHuman_Spear(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CMonster(pDevice, pContext)
{
}

CHuman_Spear::CHuman_Spear(const CHuman_Spear& rhs)
	:CMonster(rhs)
{
	m_fHitColorOnTime = 0.15f;
}

HRESULT CHuman_Spear::Initialize_Prototype()
{
	m_bHasModel = true;

	FAILED_CHECK_RETURN(__super::Initialize_Prototype(), E_FAIL);

	return S_OK;
}

HRESULT CHuman_Spear::Initialize_Clone(const wstring& wstrPrototypeTag, void* pArg)
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

	m_pHuman_Spear_State = CHuman_Spear_State::Create(this, m_pState, m_pModelCom, m_pTransformCom, m_pNavigationCom);
	FAILED_CHECK_RETURN(Ready_UI(), E_FAIL);

	if (m_tMonsterOption.m_bFirstSpawnType[STATE_ALREADYSPAWN] == true)
	{
		m_pModelCom->Set_CurAnimIndex(CHuman_Spear_State::SPEAR_JUSTSTAND);
	}
	else if (m_tMonsterOption.m_bFirstSpawnType[STATE_NODETECTED] == true)
	{
		m_pModelCom->Set_CurAnimIndex(CHuman_Spear_State::SPEAR_NODETECTED);
	}
	else if (m_tMonsterOption.m_bFirstSpawnType[STATE_UPSPAWN] == true)
	{
		m_pModelCom->Set_CurAnimIndex(CHuman_Spear_State::SPEAR_UPSPAWN);
	}

	// m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(0.f, 0.f, 10.f, 1.f));

	return S_OK;
}

void CHuman_Spear::Tick(_double TimeDelta)
{
	__super::Tick(TimeDelta);

	if (m_bIsDead)
	{
		Set_Dead(true);
		return;
	}

	{
		if (m_bHitColor)
			m_fCurHitColorOnTime += (_float)TimeDelta;
	}

	m_pState->Tick(TimeDelta);
	m_pHuman_Spear_State->Tick(TimeDelta);

	if (m_fCurHitColorOnTime >= m_fHitColorOnTime && m_bHitColor == true)
	{
		m_fCurHitColorOnTime = 0.f;
		m_bHitColor = false;
	}

	if (m_bPlayAnimation)
		m_pModelCom->Play_Animation(TimeDelta);

	Collider_Tick(TimeDelta);

	Collision_PlayerEyes(); // When Have Hide Anim


	_uint UISize = (_uint)m_vecMonsterUI.size();

	for (_uint i = 0; i < UISize; ++i)
	{
		m_vecMonsterUI[i]->Tick(TimeDelta);
	}
}

void CHuman_Spear::Late_Tick(_double TimeDelta)
{
	__super::Late_Tick(TimeDelta);

	m_pHuman_Spear_State->Late_Tick(TimeDelta);



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
		m_pRendererCom->Add_DebugRenderGroup(m_pNavigationCom);
	}
}

HRESULT CHuman_Spear::Render()
{
	FAILED_CHECK_RETURN(__super::Render(), E_FAIL);

	FAILED_CHECK_RETURN(SetUp_ShaderResources(), E_FAIL);

	_uint iNumMeshes = m_pModelCom->Get_NumMeshes();

	_uint UISize = (_uint)m_vecMonsterUI.size();

	if (m_bCanUIRender)
	{
		for (_uint i = 0; i < UISize; ++i)
		{
			m_vecMonsterUI[i]->Render();
		}
	}

	for (_uint i = 0; i < iNumMeshes; ++i)
	{
		m_pModelCom->Bind_Material(m_pShaderCom, i, aiTextureType_DIFFUSE, L"g_DiffuseTexture");
		// m_pTextureCom[TEXTURE_NORMAL]->Bind_ShaderResource(m_pShaderCom, L"g_NormalTexture");

		// m_pModelCom->Render_2Pass(m_pShaderCom, i, L"monster_body_2001_020", L"g_BoneMatrices", 1);

		m_pModelCom->Render(m_pShaderCom, i, L"g_BoneMatrices", 1);
		m_pModelCom->Render(m_pShaderCom, i, L"g_BoneMatrices", 2);
	}

	return S_OK;
}

HRESULT CHuman_Spear::Ready_UI()
{
	CUI* pMonsterUI = nullptr;

	CGameInstance* pInst = CGameInstance::GetInstance();

	CUI::MONSTERUIDESC MonsterUIDesc;
	MonsterUIDesc.pMonster = this;
	MonsterUIDesc.pModel = m_pModelCom;

	pMonsterUI = dynamic_cast<CUI*>(pInst->Clone_GameObject(L"Prototype_GameObject_MonsterUI_Base", &MonsterUIDesc));
	NULL_CHECK_RETURN(pMonsterUI, E_FAIL);
	pMonsterUI->Set_Monster(this, m_pModelCom);
	m_vecMonsterUI.push_back(pMonsterUI);

	pMonsterUI = dynamic_cast<CUI*>(pInst->Clone_GameObject(L"Prototype_GameObject_MonsterUI_HP_Red", &MonsterUIDesc));
	NULL_CHECK_RETURN(pMonsterUI, E_FAIL);
	pMonsterUI->Set_Monster(this, m_pModelCom);
	m_vecMonsterUI.push_back(pMonsterUI);

	return S_OK;
}


void CHuman_Spear::Collider_Tick(_double TimeDelta)
{
	m_pColliderCom[COLLTYPE_DETECTED]->Update(m_pTransformCom->Get_WorldMatrix());
	m_pColliderCom[COLLTYPE_HITBODY]->Update(Get_BoneMatrix("Bip001 Spine") * CGameUtils::Get_PlayerPivotMatrix() * m_pTransformCom->Get_WorldMatrix());
	m_pColliderCom[COLLTYPE_HITHEAD]->Update(Get_BoneMatrix("Bip001 Head") * CGameUtils::Get_PlayerPivotMatrix() * m_pTransformCom->Get_WorldMatrix());
	m_pColliderCom[COLLTYPE_ATTPOS]->Update(Get_BoneMatrix("Bip001 Prop1") * CGameUtils::Get_PlayerPivotMatrix() * m_pTransformCom->Get_WorldMatrix());
	m_pColliderCom[COLLTYPE_ATTRANGE]->Update(m_pTransformCom->Get_WorldMatrix());
	m_pColliderCom[COLLTYPE_ONAIM]->Update(m_pTransformCom->Get_WorldMatrix());
}

_bool CHuman_Spear::Collision_Detected(CCollider* pOtherCollider)
{
	return m_pColliderCom[COLLTYPE_DETECTED]->Collision(pOtherCollider);
}

_bool CHuman_Spear::Collider_AttRange(CCollider* pOtherCollider)
{
	return m_pColliderCom[COLLTYPE_HITBODY]->Collision(pOtherCollider);
}

void CHuman_Spear::Collision_Head(CBullet* pBullet)
{
	CBullet::BULLETOPTION BulletDesc;
	BulletDesc = pBullet->Get_BulletOption();

	if (m_tMonsterOption.MonsterDesc.m_iHP >= 0)
		m_tMonsterOption.MonsterDesc.m_iHP -= BulletDesc.BulletDesc.m_iDamage * 2;
	else if (m_tMonsterOption.MonsterDesc.m_iHP <= 0)
		Set_Dead(true);

	m_pHuman_Spear_State->Reset_Damaged();
	m_pHuman_Spear_State->Set_DamagedState(CHuman_Spear_State::HIT);
	m_pHuman_Spear_State->Set_DamagedState(CHuman_Spear_State::HITHEAD);
}

void CHuman_Spear::Collision_Hide(CBullet* pBullet)
{
	m_bHideCollision = true;
}

void CHuman_Spear::Collision_PlayerEyes()
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();

	CCollider* pCollider = (CCollider*)pGameInstance->Get_ComponentPtr(LEVEL_GAMEPLAY, L"Layer_Player", L"Com_EyesOBB");
	NULL_CHECK(pCollider);
	if (m_pColliderCom[COLLTYPE_ONAIM]->Collision(pCollider))
	{
		m_bIsOnPlayerEyes = true;
		m_bCanUIRender = true;
	}
	else
	{
		m_bIsOnPlayerEyes = false;
		m_bCanUIRender = false;
	};
}


void CHuman_Spear::Set_On_NaviMesh()
{
	_float fCellHeight = m_pNavigationCom->Get_CellHeight();

	_vector vMonsterPos = m_pTransformCom->Get_State(CTransform::STATE_TRANSLATION);

	vMonsterPos = vMonsterPos + XMVectorSet(0.f, fCellHeight, 0.f, 0.f);

	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, vMonsterPos);
}

void CHuman_Spear::Collision_Body(CBullet* pBullet)
{
	CBullet::BULLETOPTION BulletDesc;
	BulletDesc = pBullet->Get_BulletOption();

	if (m_tMonsterOption.MonsterDesc.m_iHP >= 0)
		m_tMonsterOption.MonsterDesc.m_iHP -= BulletDesc.BulletDesc.m_iDamage;
	else if (m_tMonsterOption.MonsterDesc.m_iHP <= 0)
		Set_Dead(true);

	m_pHuman_Spear_State->Reset_Damaged();
	m_pHuman_Spear_State->Set_DamagedState(CHuman_Spear_State::HIT);
	m_pHuman_Spear_State->Set_DamagedState(CHuman_Spear_State::HITBODY);
}



HRESULT CHuman_Spear::SetUp_Components()
{
	FAILED_CHECK_RETURN(__super::Add_Component(CGameInstance::Get_StaticLevelIndex(), L"Prototype_Component_Renderer", L"Com_Renderer", (CComponent**)&m_pRendererCom, this), E_FAIL);
	FAILED_CHECK_RETURN(__super::Add_Component(CGameInstance::Get_StaticLevelIndex(), L"Prototype_Component_State", L"Com_State", (CComponent**)&m_pState, this), E_FAIL);
	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Shader_VtxAnimModel", L"Com_Shader", (CComponent**)&m_pShaderCom, this), E_FAIL);
	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Model_Human_Spear", L"Com_Model", (CComponent**)&m_pModelCom, this), E_FAIL);
	// FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Sword_NormalTex", L"Com_NormalTextrueCom", (CComponent**)&m_pTextureCom[TEXTURE_NORMAL], this), E_FAIL);


	// _float Xmin = (_float)SHRT_MAX, Xmax = (_float)SHRT_MIN, Ymin = (_float)SHRT_MAX, Ymax = (_float)SHRT_MIN, Zmin = (_float)SHRT_MAX, Zmax = (_float)SHRT_MIN;
	// FAILED_CHECK_RETURN(m_pModelCom->Check_MeshSize("CritterSmall_01_LOD0", Xmin, Xmax, Ymin, Ymax, Zmin, Zmax), E_FAIL);

	CCollider::COLLIDERDESC	ColliderDesc;

	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));
	ColliderDesc.vSize = _float3(17.f, 17.f, 17.f);
	ColliderDesc.vPosition = _float3(0.f, 0.f, 0.f);
	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Collider_SPHERE", L"Com_DetectedSphere", (CComponent**)&m_pColliderCom[COLLTYPE_DETECTED], this, &ColliderDesc), E_FAIL);

	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));
	ColliderDesc.vSize = _float3(0.7f, 0.1f, 0.1f);
	ColliderDesc.vPosition = _float3(0.25f, 0.f, 0.f);
	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Collider_SPHERE", L"Com_HitBodySphere", (CComponent**)&m_pColliderCom[COLLTYPE_HITBODY], this, &ColliderDesc), E_FAIL);

	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));
	ColliderDesc.vSize = _float3(0.5f, 0.5f, 0.5f);
	ColliderDesc.vPosition = _float3(0.3f, 0.f, 0.f);
	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Collider_SPHERE", L"Com_HitHeadSphere", (CComponent**)&m_pColliderCom[COLLTYPE_HITHEAD], this, &ColliderDesc), E_FAIL);

	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));
	ColliderDesc.vSize = _float3(0.f, 0.f, 0.3f);
	ColliderDesc.vPosition = _float3(0.f, 0.f, -1.2f);
	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Collider_SPHERE", L"Com_AttackPosSphere", (CComponent**)&m_pColliderCom[COLLTYPE_ATTPOS], this, &ColliderDesc), E_FAIL);

	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));
	ColliderDesc.vSize = _float3(5.5f, 5.5f, 5.5f);
	ColliderDesc.vPosition = _float3(0.f, 0.f, 0.f);
	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Collider_SPHERE", L"Com_AttackRangeSphere", (CComponent**)&m_pColliderCom[COLLTYPE_ATTRANGE], this, &ColliderDesc), E_FAIL);

	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));
	ColliderDesc.vSize = _float3(2.f, 2.f, 2.f);
	ColliderDesc.vPosition = _float3(0.f, 1.5f, 0.f);
	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Collider_SPHERE", L"Com_OnAimSphere", (CComponent**)&m_pColliderCom[COLLTYPE_ONAIM], this, &ColliderDesc), E_FAIL);

	CNavigation::NAVIDESC		NaviDesc;
	ZeroMemory(&NaviDesc, sizeof(CNavigation::NAVIDESC));

	NaviDesc.iCurrentIndex = m_tMonsterOption.m_iCellIndex;

	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Navigation", L"Com_Navigation", (CComponent**)&m_pNavigationCom, this, &NaviDesc), E_FAIL);

	return S_OK;
}

HRESULT CHuman_Spear::SetUp_ShaderResources()
{
	NULL_CHECK_RETURN(m_pShaderCom, E_FAIL);

	CGameInstance*		pGameInstance = CGameInstance::GetInstance();

	FAILED_CHECK_RETURN(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, L"g_WorldMatrix"), E_FAIL);

	m_pShaderCom->Set_Matrix(L"g_ViewMatrix", &pGameInstance->Get_TransformFloat4x4(CPipeLine::D3DTS_VIEW));
	m_pShaderCom->Set_Matrix(L"g_ProjMatrix", &pGameInstance->Get_TransformFloat4x4(CPipeLine::D3DTS_PROJ));
	FAILED_CHECK_RETURN(m_pShaderCom->Set_RawValue(L"g_Outline_Offset", &m_fOutLineOffset, sizeof(_float)), E_FAIL);

	FAILED_CHECK_RETURN(m_pShaderCom->Set_RawValue(L"g_bHit", &m_bHitColor, sizeof(_bool)), E_FAIL);

	if (m_bIsOnPlayerEyes)
	{
		FAILED_CHECK_RETURN(m_pShaderCom->Set_RawValue(L"g_OutLineColor", &m_vOnAimOutLineColor, sizeof(_float)), E_FAIL);
	}
	else
	{
		FAILED_CHECK_RETURN(m_pShaderCom->Set_RawValue(L"g_OutLineColor", &m_vDefaultOutLineColor, sizeof(_float)), E_FAIL);
	}

	return S_OK;
}


CHuman_Spear* CHuman_Spear::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CHuman_Spear*		pInstance = new CHuman_Spear(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Create : CHuman_Spear");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CHuman_Spear::Clone(const wstring& wstrPrototypeTag, void* pArg)
{
	CHuman_Spear*		pInstance = new CHuman_Spear(*this);

	if (FAILED(pInstance->Initialize_Clone(wstrPrototypeTag, pArg)))
	{
		MSG_BOX("Failed to Clone : CHuman_Spear");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CHuman_Spear::Free()
{
	__super::Free();

	Safe_Release(m_pHuman_Spear_State);
	Safe_Release(m_pNavigationCom);
	Safe_Release(m_pState);

	for (_uint i = 0; i<COLLTYPE_END; ++i)
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
