#include "stdafx.h"
#include "..\public\Player.h"
#include "GameInstance.h"
#include "Weapon.h"
#include "Bone.h"
#include "Static_Camera.h"
#include "State.h"
#include "UI.h"

CPlayer::CPlayer(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CGameObject(pDevice, pContext)
	, m_fGravity(0.02f)
	, m_fInitJumpSpeed(1.5f)
	, m_fCurJumpSpeed(0.f)
	, m_fMaxDashTickCount(1.f)
	, m_fCurDashTickCount(0.f)
{
	
}

CPlayer::CPlayer(const CPlayer & rhs)
	: CGameObject(rhs)
	, m_fGravity(rhs.m_fGravity)
	, m_fInitJumpSpeed(rhs.m_fInitJumpSpeed)
	, m_fCurJumpSpeed(rhs.m_fCurJumpSpeed)
	, m_fMaxDashTickCount(rhs.m_fMaxDashTickCount)
	, m_fCurDashTickCount(rhs.m_fCurDashTickCount)
{
	m_tWeaponDesc[WEAPON_DEFAULT].m_wstrWeaponName = L"WEAPON_DEFAULT";
	m_tWeaponDesc[WEAPON_FLAMEBULLET].m_wstrWeaponName = L"WEAPON_FLAMEBULLET";
	m_tWeaponDesc[WEAPON_FIREDRAGON].m_wstrWeaponName = L"WEAPON_FIREDRAGON";
	m_tWeaponDesc[WEAPON_POISON].m_wstrWeaponName = L"WEAPON_POISON";


	m_PlayerOption.m_iMaxHp = 100;
	m_PlayerOption.m_iHp = m_PlayerOption.m_iMaxHp;
	m_PlayerOption.m_iGold = 500;
	m_PlayerOption.m_iEmeraldCnt = 0;
	m_PlayerOption.m_iMaxShieldPoint = 100;
	m_PlayerOption.m_iShieldPoint = m_PlayerOption.m_iMaxShieldPoint;
	m_PlayerOption.m_iPistol_BulletCnt = 50;
	m_PlayerOption.m_iRifle_BulletCnt = 50;
	m_PlayerOption.m_iInjector_BulletCnt = 50;
	m_PlayerOption.m_iThrowCnt = 4;
}

_matrix CPlayer::Get_BoneMatrix(const string& strBoneName)
{
	if (nullptr == m_pModelCom)
		return XMMatrixIdentity();

	return m_pModelCom->Get_BoneMatrix(strBoneName);
}

_matrix CPlayer::Get_OffsetMatrix(const string& strBoneName)
{
	if (nullptr == m_pModelCom)
		return XMMatrixIdentity();

	return m_pModelCom->Get_OffsetMatrix(strBoneName);
}

_matrix CPlayer::Get_PivotMatrix()
{
	if (nullptr == m_pModelCom)
		return XMMatrixIdentity();

	return m_pModelCom->Get_PivotMatrix();
}

_vector CPlayer::Get_TransformState(CTransform::STATE eState)
{
	if (m_pModelCom == nullptr)
		return XMVectorSet(0.f, 1.f, 0.f, 0.f);

	return m_pTransformCom->Get_State(eState);
}

_matrix CPlayer::Get_CombindMatrix(const string& strBoneName)
{
	if (nullptr == m_pModelCom)
		return XMMatrixIdentity();

	return m_pModelCom->Get_CombindMatrix(strBoneName);
}

CModel* CPlayer::Get_CurWeaponModelCom()
{
	for(_uint i = 0; i < WEAPON_END; ++i)
	{
		if (m_PlayerOption.m_wstrCurWeaponName == m_tWeaponDesc[i].m_wstrWeaponName)
		{
			return m_tWeaponDesc[i].m_pWeaponModelCom;
		}
		else
			return nullptr;
	}

	return nullptr;
}

_float4x4 CPlayer::Get_HeadModelCom()
{
	if (m_tWeaponDesc[WEAPON_DEFAULT].m_pWeaponModelCom == nullptr)
		return XMMatrixIdentity();

	return m_tWeaponDesc[WEAPON_DEFAULT].m_pWeaponModelCom->Get_BoneMatrix("Bip001 Head");
}


HRESULT CPlayer::Initialize_Prototype()
{
	m_bHasModel = true;

	FAILED_CHECK_RETURN(__super::Initialize_Prototype(), E_FAIL);

	return S_OK;
}

HRESULT CPlayer::Initialize_Clone(const wstring& wstrPrototypeTag, void * pArg)
{
	CGameObject::GAMEOBJECTDESC		GameObjectDesc;
	ZeroMemory(&GameObjectDesc, sizeof(GAMEOBJECTDESC));

	GameObjectDesc.TransformDesc.fSpeedPerSec = 7.0;
	GameObjectDesc.TransformDesc.fRotationPerSec = XMConvertToRadians(90.f);

	FAILED_CHECK_RETURN(__super::Initialize_Clone(wstrPrototypeTag, &GameObjectDesc), E_FAIL);
	FAILED_CHECK_RETURN(SetUp_Components(), E_FAIL);
	m_pWeaponState = CWeapon_State::Create(this, m_pState, m_pModelCom, m_pTransformCom, m_pNavigationCom);
	NULL_CHECK_RETURN(m_pWeaponState, E_FAIL);

	m_pModelCom->Set_CurAnimIndex(CWeapon_State::DEFAULT_PISTOL_IDLE);

	FAILED_CHECK_RETURN(Ready_UI(), E_FAIL);

	m_pTransformCom->Set_Scaled(_float3(0.3f, 0.3f, 0.3f));
	


	return S_OK;
}

void CPlayer::Tick(_double dTimeDelta)
{
	__super::Tick(dTimeDelta);

	if (_int TurnX = CGameInstance::GetInstance()->Get_DIMouseMove(DIMS_X))
	{
		m_pTransformCom->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), dTimeDelta * TurnX * 0.1f);
	}

	if (_int TurnY  = CGameInstance::GetInstance()->Get_DIMouseMove(DIMS_Y))
	{
		m_pTransformCom->Turn(m_pTransformCom->Get_State(CTransform::STATE_RIGHT), dTimeDelta * TurnY * 0.1f);
	}

	if (CGameInstance::GetInstance()->Get_DIKeyState(DIK_F1))
	{
		dynamic_cast<CStatic_Camera*>(CGameInstance::GetInstance()->Get_CloneObjectList(LEVEL_GAMEPLAY, L"Layer_ZCamera")->back())->Set_FixControl();
	}

	if (CGameInstance::GetInstance()->Key_Pressing(DIK_W))
	{
		m_pTransformCom->Go_Straight(dTimeDelta, CTransform::TRANS_PLAYER, m_pNavigationCom);
	}

	if (CGameInstance::GetInstance()->Key_Pressing(DIK_S))
	{
		m_pTransformCom->Go_Backward(dTimeDelta);
	}
	if (CGameInstance::GetInstance()->Key_Pressing(DIK_A))
	{
		m_pTransformCom->Go_Left(dTimeDelta);
	}
	if (CGameInstance::GetInstance()->Key_Pressing(DIK_D))
	{
		m_pTransformCom->Go_Right(dTimeDelta);
	}

	_matrix matpivot;
	matpivot = XMMatrixIdentity();
	matpivot = XMMatrixRotationY(XMConvertToRadians(180.f));

	_float m_fHeight = m_pNavigationCom->Get_CellHeight();
	_float4 PlayerPos = m_pTransformCom->Get_State(CTransform::STATE_TRANSLATION);
	PlayerPos.y = m_fHeight + 2.644f;

	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, PlayerPos);

	if (nullptr != m_pState)
		m_pState->Tick(dTimeDelta);

	m_pWeaponState->Tick(dTimeDelta);
	m_pModelCom->Play_Animation(dTimeDelta, m_eLerpType);

	for (_uint i = 0; i < m_vecPlayerUI.size(); ++i)
	{
		m_vecPlayerUI[i]->Tick(dTimeDelta);
	}

	_float4 R, U, L, P;
	_matrix FakeWorldMatrix;
	R = m_pTransformCom->Get_WorldMatrix().r[0];
	U = m_pTransformCom->Get_WorldMatrix().r[1];
	L = m_pTransformCom->Get_WorldMatrix().r[2];
	P = m_pTransformCom->Get_WorldMatrix().r[3];
	P = P + _float4(0.f, 0.f, 0.f, 0.f);

	for (_uint i = 0; i < COLLIDERTYPE_END; ++i)
	{
		if (m_pColliderCom[i] == nullptr)
			continue;

		if (i == COLLIDER_MUZZLE)
		{
			if(m_PlayerOption.m_wstrCurWeaponName == L"WEAPON_DEFAULT")
				m_pColliderCom[i]->Update(Get_BoneMatrix("Att") * matpivot * m_pTransformCom->Get_WorldMatrix());
			else if (m_PlayerOption.m_wstrCurWeaponName == L"WEAPON_FLAMEBULLET")
			{
				m_pColliderCom[i]->Update(Get_BoneMatrix("Att001") * matpivot * m_pTransformCom->Get_WorldMatrix());
			}
		}
		else if (i == COLLIDER_OBB)
			m_pColliderCom[i]->Update(_matrix(R, U, L, P));
		else 
			m_pColliderCom[i]->Update(m_pTransformCom->Get_WorldMatrix());
	}

	m_pFirstAimColliderCom->Update(_matrix(R, U, L, P));
	m_pSecondAimColliderCom->Update(_matrix(R, U, L, P));
	dynamic_cast<CStatic_Camera*>(CGameInstance::GetInstance()->Get_CloneObjectList(LEVEL_GAMEPLAY, L"Layer_ZCamera")->back())->Late_Tick(dTimeDelta);
}

void CPlayer::Late_Tick(_double dTimeDelta)
{
	__super::Late_Tick(dTimeDelta);
	// Collision_AimBox_To_Monster();
	m_pWeaponState->Late_Tick(dTimeDelta);

	Set_Camera(dTimeDelta);
	dynamic_cast<CStatic_Camera*>(CGameInstance::GetInstance()->Get_CloneObjectList(LEVEL_GAMEPLAY, L"Layer_ZCamera")->back())->Tick(dTimeDelta);

	for (_uint i = 0; i < m_vecPlayerUI.size(); ++i)
	{
		m_vecPlayerUI[i]->Late_Tick(dTimeDelta);
	}

	if (nullptr != m_pRendererCom)
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHABLEND, this);
}

HRESULT CPlayer::Render()
{

	FAILED_CHECK_RETURN(__super::Render(), E_FAIL);
	FAILED_CHECK_RETURN(SetUp_ShaderResources(), E_FAIL);

	_uint iNumMeshes = m_pModelCom->Get_NumMeshes();

	for (_uint i = 0; i < iNumMeshes; ++i)
	{
		m_pModelCom->Bind_Material(m_pShaderCom, i, aiTextureType_DIFFUSE, L"g_DiffuseTexture");
		m_pModelCom->Bind_Material(m_pShaderCom, i, aiTextureType_NORMALS, L"g_NormalTexture");

		m_pModelCom->Render(m_pShaderCom, i, L"g_BoneMatrices");
	}

#ifdef _DEBUG
	for (_uint i = 0; i < COLLIDERTYPE_END; ++i)
	{
		if (nullptr != m_pColliderCom[i])
			m_pColliderCom[i]->Render();
	}
	m_pFirstAimColliderCom->Render();
	m_pSecondAimColliderCom->Render();

	m_pNavigationCom->Render();
#endif

	return S_OK;
}


void CPlayer::Set_Camera(_double dTimeDelta)
{
	// m_vCamLook = *dynamic_cast<CStatic_Camera*>(CGameInstance::GetInstance()->Get_CloneObjectList(LEVEL_GAMEPLAY, L"Layer_ZCamera")->back())->Get_CamLook();
	//
	// // _vector		vCamLook = XMLoadFloat4(&m_vCamLook);
	// _float4		vPos = m_pTransformCom->Get_State(CTransform::STATE_TRANSLATION);
	// m_vCamLookAt = vPos + m_vCamLook;
	// m_pTransformCom->LookAt(m_vCamLookAt);

	// dynamic_cast<CStatic_Camera*>(CGameInstance::GetInstance()->Get_CloneObjectList(LEVEL_GAMEPLAY, L"Layer_ZCamera")->back())->Set_CamPos(vPos, XMVectorSet(0.f,0.f,1.f,0.f));

	dynamic_cast<CStatic_Camera*>(CGameInstance::GetInstance()->Get_CloneObjectList(LEVEL_GAMEPLAY, L"Layer_ZCamera")->
		back())->Camera_Update(	dTimeDelta, m_pTransformCom->Get_WorldMatrix());

}

CGameObject* CPlayer::Collision_AimBox_To_Monster()
{
	CCollider*		pTargetCollider = (CCollider*)CGameInstance::GetInstance()->Get_ComponentPtr(LEVEL_GAMEPLAY, L"Layer_Monster", L"Com_SPHERE");

	if (nullptr == pTargetCollider)
		return nullptr;

	return	m_pColliderCom[COLLIDER_OBB]->CollisionReturnObj(pTargetCollider);
}

HRESULT CPlayer::SetUp_Components()
{
	FAILED_CHECK_RETURN(__super::Add_Component(CGameInstance::Get_StaticLevelIndex(), L"Prototype_Component_Renderer", L"Com_Renderer",	(CComponent**)&m_pRendererCom, this), E_FAIL);
	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Shader_VtxAnimModel", L"Com_Shader",	(CComponent**)&m_pShaderCom, this), E_FAIL);

	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Model_Default_Pistol", L"Com_Default_Pistol_Model", (CComponent**)&m_tWeaponDesc[WEAPON_DEFAULT].m_pWeaponModelCom, this), E_FAIL);
	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Model_Fire_Dragon", L"Com_Fire_Dragon_Model", (CComponent**)&m_tWeaponDesc[WEAPON_FIREDRAGON].m_pWeaponModelCom, this), E_FAIL);
	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Model_Flame_Bullet", L"Com_Flame_Bullet_Model", (CComponent**)&m_tWeaponDesc[WEAPON_FLAMEBULLET].m_pWeaponModelCom, this), E_FAIL);
	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Model_Poison", L"Com_Poison_Model", (CComponent**)&m_tWeaponDesc[WEAPON_POISON].m_pWeaponModelCom, this), E_FAIL);

	FAILED_CHECK_RETURN(__super::Add_Component(CGameInstance::Get_StaticLevelIndex(), L"Prototype_Component_State", L"Com_State", (CComponent**)&m_pState, this), E_FAIL);


	m_pModelCom = m_tWeaponDesc[WEAPON_DEFAULT].m_pWeaponModelCom;
	m_PlayerOption.m_wstrCurWeaponName = L"";
	m_PlayerOption.m_wstrCurWeaponName = m_tWeaponDesc[WEAPON_DEFAULT].m_wstrWeaponName;

	CCollider::COLLIDERDESC			ColliderDesc;

	/* For.Com_AABB */
	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));
	ColliderDesc.vSize = _float3(0.7f, 1.5f, 0.7f);
	ColliderDesc.vPosition = _float3(0.f, 0.f, 0.f);
	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Collider_AABB", L"Com_AABB", (CComponent**)&m_pColliderCom[COLLIDER_AABB], this, &ColliderDesc), E_FAIL);

	/* For.Com_OBB */
	XMStoreFloat3(&m_vAimColliderPos, m_pTransformCom->Get_State(CTransform::STATE_LOOK));
	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));
	ColliderDesc.vSize = _float3(0.5f, 0.5f, 30.f);
	ColliderDesc.vRotation = _float3(0.f, 0.f, 0.f);
	ColliderDesc.vPosition = _float3(m_vAimColliderPos.x * 15.f, m_vAimColliderPos.y *15.f, m_vAimColliderPos.z * 15.f);
	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Collider_OBB", L"Com_OBB", (CComponent**)&m_pColliderCom[COLLIDER_OBB], this, &ColliderDesc), E_FAIL);

	// /* For.Com_SPHERE */
	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));
	ColliderDesc.vSize = _float3(2.f, 2.f, 2.f);
	ColliderDesc.vPosition = _float3(0.f, 0.f, 0.f);
	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Collider_SPHERE", L"Com_SPHERE", (CComponent**)&m_pColliderCom[COLLIDER_SPHERE], this, &ColliderDesc), E_FAIL);

	// For First Aim Sphere
	XMStoreFloat3(&m_vAimColliderPos, m_pTransformCom->Get_State(CTransform::STATE_LOOK));
	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));
	ColliderDesc.vSize = _float3(0.5f, 0.5f, 0.5f);
	ColliderDesc.vRotation = _float3(0.f, 0.f, 0.f);
	ColliderDesc.vPosition = ColliderDesc.vPosition = _float3(m_vAimColliderPos.x * 15.f, m_vAimColliderPos.y * 15.f, m_vAimColliderPos.z * 15.f);
	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Collider_SPHERE", L"Com_BulletFirstAim", (CComponent**)&m_pFirstAimColliderCom, this, &ColliderDesc), E_FAIL);

	// For Second Aim Sphere
	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));
	ColliderDesc.vSize = _float3(0.5f, 0.5f, 0.5f);
	ColliderDesc.vRotation = _float3(0.f, 0.f, 0.f);
	ColliderDesc.vPosition = ColliderDesc.vPosition = _float3(m_vAimColliderPos.x * 30.f, m_vAimColliderPos.y * 30.f, m_vAimColliderPos.z * 30.f);
	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Collider_SPHERE", L"Com_BulletSecondAim", (CComponent**)&m_pSecondAimColliderCom, this, &ColliderDesc), E_FAIL);
	// For Muzzle Coll

	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));
	ColliderDesc.vSize = _float3(0.3f, 0.3f, 0.3f);
	ColliderDesc.vPosition = _float3(0.f, 0.f, 0.f);
	// ColliderDesc.vRotation = _float3(-1.f, 0.f, -1.f);
	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Collider_Muzzle_SPHERE", L"Com_MuzzleSPHERE", (CComponent**)&m_pColliderCom[COLLIDER_MUZZLE], this, &ColliderDesc), E_FAIL);

	//NavigationCom
	CNavigation::NAVIDESC		NaviDesc;
	ZeroMemory(&NaviDesc, sizeof(CNavigation::NAVIDESC));

	NaviDesc.iCurrentIndex = 0;

	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Navigation", L"Com_Navigation", (CComponent**)&m_pNavigationCom, this, &NaviDesc), E_FAIL);

	return S_OK;
}

HRESULT CPlayer::SetUp_ShaderResources()
{
	NULL_CHECK_RETURN(m_pShaderCom, E_FAIL);


	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	_float3 tmp;
	XMStoreFloat3(&tmp, m_pTransformCom->Get_State(CTransform::STATE_LOOK));

	FAILED_CHECK_RETURN(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, L"g_WorldMatrix"), E_FAIL);
	FAILED_CHECK_RETURN(m_pShaderCom->Set_Matrix(L"g_ViewMatrix", &pGameInstance->Get_TransformFloat4x4(CPipeLine::D3DTS_VIEW)), E_FAIL);
	FAILED_CHECK_RETURN(m_pShaderCom->Set_RawValue(L"g_PixelOffset", &(_float4(1 / (_float)g_iWinSizeX, 1 / (_float)g_iWinSizeY, 0.f, 0.f)) , sizeof(_float4)), E_FAIL);
	FAILED_CHECK_RETURN(m_pShaderCom->Set_Matrix(L"g_ProjMatrix", &pGameInstance->Get_TransformFloat4x4(CPipeLine::D3DTS_PROJ)), E_FAIL);
	FAILED_CHECK_RETURN(m_pShaderCom->Set_RawValue(L"g_vCameraPos", &CGameInstance::GetInstance()->Get_CamPos(), sizeof(_float3)), E_FAIL);

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CPlayer::Ready_UI()
{
	CGameObject*		pPlayerUI = nullptr;

	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	pPlayerUI = pGameInstance->Clone_GameObject(L"Prototype_GameObject_PlayerUI_Base");
	NULL_CHECK_RETURN(pPlayerUI, E_FAIL);

	m_vecPlayerUI.push_back(pPlayerUI);

	pPlayerUI = pGameInstance->Clone_GameObject(L"Prototype_GameObject_Player_Skill_BaseTex");
	NULL_CHECK_RETURN(pPlayerUI, E_FAIL);

	m_vecPlayerUI.push_back(pPlayerUI);

	pPlayerUI = pGameInstance->Clone_GameObject(L"Prototype_GameObject_PlayerUI_Skill");
	NULL_CHECK_RETURN(pPlayerUI, E_FAIL);

	m_vecPlayerUI.push_back(pPlayerUI);

	pPlayerUI = pGameInstance->Clone_GameObject(L"Prototype_GameObject_Player_GoldTex");
	NULL_CHECK_RETURN(pPlayerUI, E_FAIL);

	m_vecPlayerUI.push_back(pPlayerUI);

	pPlayerUI = pGameInstance->Clone_GameObject(L"Prototype_GameObject_Player_EmeraldTex");
	NULL_CHECK_RETURN(pPlayerUI, E_FAIL);

	m_vecPlayerUI.push_back(pPlayerUI);

	pPlayerUI = pGameInstance->Clone_GameObject(L"Prototype_GameObject_PlayerUI_Throw");
	NULL_CHECK_RETURN(pPlayerUI, E_FAIL);

	m_vecPlayerUI.push_back(pPlayerUI);

	pPlayerUI = pGameInstance->Clone_GameObject(L"Prototype_GameObject_PlayerUI_Dash");
	NULL_CHECK_RETURN(pPlayerUI, E_FAIL);

	m_vecPlayerUI.push_back(pPlayerUI);

	pPlayerUI = pGameInstance->Clone_GameObject(L"Prototype_GameObject_PlayerUI_Weapon");
	NULL_CHECK_RETURN(pPlayerUI, E_FAIL);

	m_vecPlayerUI.push_back(pPlayerUI);

	pPlayerUI = pGameInstance->Clone_GameObject(L"Prototype_GameObject_PlayerUI_Number_1");
	NULL_CHECK_RETURN(pPlayerUI, E_FAIL);

	m_vecPlayerUI.push_back(pPlayerUI);

	pPlayerUI = pGameInstance->Clone_GameObject(L"Prototype_GameObject_PlayerUI_WeaponPic");
	NULL_CHECK_RETURN(pPlayerUI, E_FAIL);

	m_vecPlayerUI.push_back(pPlayerUI);
	
	pPlayerUI = pGameInstance->Clone_GameObject(L"Prototype_GameObject_PlayerUI_BulletPic");
	NULL_CHECK_RETURN(pPlayerUI, E_FAIL);

	m_vecPlayerUI.push_back(pPlayerUI);

	CUI::COUNTDESC eType;
	eType.m_eType = CUI::CNT_BULLET;

	pPlayerUI = pGameInstance->Clone_GameObject(L"Prototype_GameObject_PlayerUI_BulletCnt", &eType);
	NULL_CHECK_RETURN(pPlayerUI, E_FAIL);

	m_vecPlayerUI.push_back(pPlayerUI);

	eType.m_eType = CUI::CNT_GOLD;
	pPlayerUI = pGameInstance->Clone_GameObject(L"Prototype_GameObject_PlayerUI_GoldCnt", &eType);
	NULL_CHECK_RETURN(pPlayerUI, E_FAIL);

	m_vecPlayerUI.push_back(pPlayerUI);

	eType.m_eType = CUI::CNT_THROW;
	pPlayerUI = pGameInstance->Clone_GameObject(L"Prototype_GameObject_PlayerUI_ThrowCnt", &eType);
	NULL_CHECK_RETURN(pPlayerUI, E_FAIL);

	m_vecPlayerUI.push_back(pPlayerUI);

	eType.m_eType = CUI::CNT_EMERALD;
	pPlayerUI = pGameInstance->Clone_GameObject(L"Prototype_GameObject_PlayerUI_EmeraldCnt", &eType);
	NULL_CHECK_RETURN(pPlayerUI, E_FAIL);

	m_vecPlayerUI.push_back(pPlayerUI);

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

CPlayer * CPlayer::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CPlayer*		pInstance = new CPlayer(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CPlayer");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CPlayer::Clone(const wstring& wstrPrototypeTag, void * pArg)
{
	CPlayer*		pInstance = new CPlayer(*this);

	if (FAILED(pInstance->Initialize_Clone(wstrPrototypeTag, pArg)))
	{
		MSG_BOX("Failed to Cloned : CPlayer");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CPlayer::Free()
{
	__super::Free();

	Safe_Release(m_pWeaponState);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pModelCom);

	for (_uint i = 0; i < COLLIDERTYPE_END; ++i)
		Safe_Release(m_pColliderCom[i]);

	Safe_Release(m_pFirstAimColliderCom);
	Safe_Release(m_pSecondAimColliderCom);

	for (auto& pUI : m_vecPlayerUI)
		Safe_Release(pUI);

	Safe_Release(m_pRendererCom);
	Safe_Release(m_pState);
	Safe_Release(m_pNavigationCom);

	if (m_bIsClone)
	{
		for (_uint i = 0; i < WEAPON_END; ++i)
			Safe_Release(m_tWeaponDesc[i].m_pWeaponModelCom);
	}
}
