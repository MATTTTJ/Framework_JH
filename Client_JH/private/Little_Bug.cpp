#include "stdafx.h"
#include "..\public\Little_Bug.h"
#include "Bullet.h"
#include "GameInstance.h"
#include "Player.h"
#include "UI.h"
#include "Little_Bug_State.h"

CLittle_Bug::CLittle_Bug(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CMonster(pDevice, pContext)
{
}

CLittle_Bug::CLittle_Bug(const CLittle_Bug& rhs)
	: CMonster(rhs)
{
	m_fHitColorOnTime = 0.15f;
}

HRESULT CLittle_Bug::Initialize_Prototype()
{
	m_bHasModel = true;

	FAILED_CHECK_RETURN(__super::Initialize_Prototype(), E_FAIL);

	return S_OK;
}

HRESULT CLittle_Bug::Initialize_Clone(const wstring& wstrPrototypeTag, void* pArg)
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

	m_pLittle_Bug_State = CLittle_Bug_State::Create(this, m_pState, m_pModelCom, m_pTransformCom, m_pNavigationCom);
	FAILED_CHECK_RETURN(Ready_UI(), E_FAIL);

	m_pTransformCom->Set_Scaled(_float3(0.1f, 0.1f, 0.1f));

	if (m_tMonsterOption.m_bFirstSpawnType[STATE_ALREADYSPAWN] == true)
	{
		m_pModelCom->Set_CurAnimIndex(CLittle_Bug_State::LITTLE_IN_COMBAT_IDLE);
	}

	return S_OK;
}

void CLittle_Bug::Tick(_double TimeDelta)
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
	m_pLittle_Bug_State->Tick(TimeDelta);

	if (m_fCurHitColorOnTime >= m_fHitColorOnTime && m_bHitColor == true)
	{
		m_fCurHitColorOnTime = 0.f;
		m_bHitColor = false;
	}

	if (m_bPlayAnimation)
		m_pModelCom->Play_Animation(TimeDelta);

	Collider_Tick(TimeDelta);
	Set_On_NaviMesh();
	Collision_PlayerEyes(); // When Have Hide Anim


	_uint UISize = (_uint)m_vecMonsterUI.size();

	for (_uint i = 0; i < UISize; ++i)
	{
		m_vecMonsterUI[i]->Tick(TimeDelta);
	}
}

void CLittle_Bug::Late_Tick(_double TimeDelta)
{
	__super::Late_Tick(TimeDelta);

	m_pLittle_Bug_State->Late_Tick(TimeDelta);

	_uint UISize = (_uint)m_vecMonsterUI.size();

	for (_uint i = 0; i < UISize; ++i)
	{
		m_vecMonsterUI[i]->Late_Tick(TimeDelta);
	}

	if (nullptr != m_pRendererCom &&
		true == CGameInstance::GetInstance()->isInFrustum_WorldSpace(m_pTransformCom->Get_State(CTransform::STATE_TRANSLATION), 2.f))
	{
		if (m_bIsOnPlayerEyes)
			m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_OUTLINE, this);
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_DYNAMIC_SHADOWDEPTH, this);
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

HRESULT CLittle_Bug::Render()
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

		m_pModelCom->Render(m_pShaderCom, i, L"g_BoneMatrices", 2);
	}

	return S_OK;
}

HRESULT CLittle_Bug::Ready_UI()
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

void CLittle_Bug::Collider_Tick(_double TimeDelta)
{
	m_pColliderCom[COLLTYPE_DETECTED]->Update(m_pTransformCom->Get_WorldMatrix());
	m_pColliderCom[COLLTYPE_HITBODY]->Update(Get_BoneMatrix("Bip001 Spine") * CGameUtils::Get_PlayerPivotMatrix() * m_pTransformCom->Get_WorldMatrix());
	m_pColliderCom[COLLTYPE_HITHEAD]->Update(Get_BoneMatrix("Bip001 Head") * CGameUtils::Get_PlayerPivotMatrix() * m_pTransformCom->Get_WorldMatrix());
	m_pColliderCom[COLLTYPE_ATTPOS]->Update(Get_BoneMatrix("Bip001 Head") * CGameUtils::Get_PlayerPivotMatrix() * m_pTransformCom->Get_WorldMatrix());
	m_pColliderCom[COLLTYPE_ATTRANGE]->Update(m_pTransformCom->Get_WorldMatrix());
	m_pColliderCom[COLLTYPE_ONAIM]->Update(m_pTransformCom->Get_WorldMatrix());
}

HRESULT CLittle_Bug::Render_ShadowDepth()
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

HRESULT CLittle_Bug::Render_OutLineFlag()
{
	if (nullptr == m_pShaderCom ||
		nullptr == m_pModelCom)
		return E_FAIL;

	/* 셰이더 전역변수에 값을 던진다. */
	FAILED_CHECK_RETURN(SetUp_ShaderResources(), E_FAIL);

	FAILED_CHECK_RETURN(m_pShaderCom->Set_RawValue(L"g_vColor", &m_vOnAimOutLineColor, sizeof(_float)), E_FAIL);

	_uint iNumMeshes = m_pModelCom->Get_NumMeshes();

	for (_uint i = 0; i < iNumMeshes; ++i)
	{
		m_pModelCom->Render(m_pShaderCom, i, L"g_BoneMatrices", 6);
	}

	return S_OK;
}

_bool CLittle_Bug::Collision_Detected(CCollider* pOtherCollider)
{
	return m_pColliderCom[COLLTYPE_DETECTED]->Collision(pOtherCollider);
}

_bool CLittle_Bug::Collider_AttRange(CCollider* pOtherCollider)
{
	return m_pColliderCom[COLLTYPE_HITBODY]->Collision(pOtherCollider);
}

void CLittle_Bug::Set_On_NaviMesh()
{
	_float4 MonsterPos = m_pTransformCom->Get_State(CTransform::STATE_TRANSLATION);
	MonsterPos = m_pNavigationCom->Get_CellHeight(MonsterPos);
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, MonsterPos);
}

void CLittle_Bug::Collision_Body(CBullet* pBullet)
{
	CBullet::BULLETOPTION BulletDesc;
	BulletDesc = pBullet->Get_BulletOption();

	if (m_tMonsterOption.MonsterDesc.m_iHP >= 0)
		m_tMonsterOption.MonsterDesc.m_iHP -= BulletDesc.BulletDesc.m_iDamage;
	else if (m_tMonsterOption.MonsterDesc.m_iHP <= 0)
		Set_Dead(true);

	m_pLittle_Bug_State->Reset_Damaged();
	m_pLittle_Bug_State->Set_DamagedState(CLittle_Bug_State::HIT);
	m_pLittle_Bug_State->Set_DamagedState(CLittle_Bug_State::HITBODY);
}

void CLittle_Bug::Collision_Head(CBullet* pBullet)
{
	CBullet::BULLETOPTION BulletDesc;
	BulletDesc = pBullet->Get_BulletOption();

	if (m_tMonsterOption.MonsterDesc.m_iHP >= 0)
		m_tMonsterOption.MonsterDesc.m_iHP -= BulletDesc.BulletDesc.m_iDamage;
	else if (m_tMonsterOption.MonsterDesc.m_iHP <= 0)
		Set_Dead(true);

	m_pLittle_Bug_State->Reset_Damaged();
	m_pLittle_Bug_State->Set_DamagedState(CLittle_Bug_State::HIT);
	m_pLittle_Bug_State->Set_DamagedState(CLittle_Bug_State::HITBODY);
}

void CLittle_Bug::Collision_Hide(CBullet* pBullet)
{
	CMonster::Collision_Hide(pBullet);
}

void CLittle_Bug::Collision_PlayerEyes()
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

HRESULT CLittle_Bug::SetUp_Components()
{
	FAILED_CHECK_RETURN(__super::Add_Component(CGameInstance::Get_StaticLevelIndex(), L"Prototype_Component_Renderer", L"Com_Renderer", (CComponent**)&m_pRendererCom, this), E_FAIL);
	FAILED_CHECK_RETURN(__super::Add_Component(CGameInstance::Get_StaticLevelIndex(), L"Prototype_Component_State", L"Com_State", (CComponent**)&m_pState, this), E_FAIL);
	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Shader_VtxAnimModel", L"Com_Shader", (CComponent**)&m_pShaderCom, this), E_FAIL);
	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Model_Little_Bug", L"Com_Model", (CComponent**)&m_pModelCom, this), E_FAIL);
	// FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Sword_NormalTex", L"Com_NormalTextrueCom", (CComponent**)&m_pTextureCom[TEXTURE_NORMAL], this), E_FAIL);


	// _float Xmin = (_float)SHRT_MAX, Xmax = (_float)SHRT_MIN, Ymin = (_float)SHRT_MAX, Ymax = (_float)SHRT_MIN, Zmin = (_float)SHRT_MAX, Zmax = (_float)SHRT_MIN;
	// FAILED_CHECK_RETURN(m_pModelCom->Check_MeshSize("CritterSmall_01_LOD0", Xmin, Xmax, Ymin, Ymax, Zmin, Zmax), E_FAIL);

	CCollider::COLLIDERDESC	ColliderDesc;

	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));
	ColliderDesc.vSize = _float3(30.f, 17.f, 17.f);
	ColliderDesc.vPosition = _float3(0.f, 0.f, 0.f);
	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Collider_SPHERE", L"Com_DetectedSphere", (CComponent**)&m_pColliderCom[COLLTYPE_DETECTED], this, &ColliderDesc), E_FAIL);

	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));
	ColliderDesc.vSize = _float3(0.5f, 0.1f, 0.1f);
	ColliderDesc.vPosition = _float3(0.f, 0.f, 0.f);
	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Collider_SPHERE", L"Com_HitBodySphere", (CComponent**)&m_pColliderCom[COLLTYPE_HITBODY], this, &ColliderDesc), E_FAIL);

	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));
	ColliderDesc.vSize = _float3(0.5f, 0.5f, 0.5f);
	ColliderDesc.vPosition = _float3(0.f, 0.f, 0.f);
	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Collider_SPHERE", L"Com_HitHeadSphere", (CComponent**)&m_pColliderCom[COLLTYPE_HITHEAD], this, &ColliderDesc), E_FAIL);

	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));
	ColliderDesc.vSize = _float3(0.f, 0.f, 0.3f);
	ColliderDesc.vPosition = _float3(0.f, 0.f, 0.f);
	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Collider_SPHERE", L"Com_AttackPosSphere", (CComponent**)&m_pColliderCom[COLLTYPE_ATTPOS], this, &ColliderDesc), E_FAIL);

	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));
	ColliderDesc.vSize = _float3(3.f, 3.f, 3.f);
	ColliderDesc.vPosition = _float3(0.f, 0.f, 0.f);
	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Collider_SPHERE", L"Com_AttackRangeSphere", (CComponent**)&m_pColliderCom[COLLTYPE_ATTRANGE], this, &ColliderDesc), E_FAIL);

	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));
	ColliderDesc.vSize = _float3(3.f, 3.f, 3.f);
	ColliderDesc.vPosition = _float3(0.f, 1.5f, 0.f);
	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Collider_SPHERE", L"Com_OnAimSphere", (CComponent**)&m_pColliderCom[COLLTYPE_ONAIM], this, &ColliderDesc), E_FAIL);

	CNavigation::NAVIDESC		NaviDesc;
	ZeroMemory(&NaviDesc, sizeof(CNavigation::NAVIDESC));

	NaviDesc.iCurrentIndex = m_tMonsterOption.m_iCellIndex;

	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Navigation", L"Com_Navigation", (CComponent**)&m_pNavigationCom, this, &NaviDesc), E_FAIL);

	return S_OK;
}

HRESULT CLittle_Bug::SetUp_ShaderResources()
{
	NULL_CHECK_RETURN(m_pShaderCom, E_FAIL);

	CGameInstance*		pGameInstance = CGameInstance::GetInstance();

	FAILED_CHECK_RETURN(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, L"g_WorldMatrix"), E_FAIL);

	m_pShaderCom->Set_Matrix(L"g_ViewMatrix", &pGameInstance->Get_TransformFloat4x4(CPipeLine::D3DTS_VIEW));
	m_pShaderCom->Set_Matrix(L"g_ProjMatrix", &pGameInstance->Get_TransformFloat4x4(CPipeLine::D3DTS_PROJ));

	return S_OK;
}

CLittle_Bug* CLittle_Bug::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CLittle_Bug*		pInstance = new CLittle_Bug(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Create : CLittle_Bug");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CLittle_Bug::Clone(const wstring& wstrPrototypeTag, void* pArg)
{
	CLittle_Bug*		pInstance = new CLittle_Bug(*this);

	if (FAILED(pInstance->Initialize_Clone(wstrPrototypeTag, pArg)))
	{
		MSG_BOX("Failed to Clone : CLittle_Bug");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLittle_Bug::Free()
{
	__super::Free();

	Safe_Release(m_pLittle_Bug_State);
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
