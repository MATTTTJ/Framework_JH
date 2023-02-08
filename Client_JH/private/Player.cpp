#include "stdafx.h"
#include "..\public\Player.h"

#include "Dynamic_Camera.h"
#include "GameInstance.h"
#include "Monster.h"
#include "Static_Camera.h"
#include "State.h"
#include "UI.h"

CPlayer::CPlayer(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CGameObject(pDevice, pContext)
	, m_fGravity(0.02f)
	, m_fInitJumpSpeed(1.5f)
	, m_fCurJumpSpeed(0.f)
	, m_fDashCoolTime(3.f)
	, m_fCurDashCoolTime(0.f)
	, m_fDashTime(0.17f)
	, m_fCurDashTime(0.f)
	, m_bCanDash(true)
{
	
}

CPlayer::CPlayer(const CPlayer & rhs)
	: CGameObject(rhs)
	, m_fGravity(rhs.m_fGravity)
	, m_fInitJumpSpeed(rhs.m_fInitJumpSpeed)
	, m_fCurJumpSpeed(rhs.m_fCurJumpSpeed)
	, m_fDashCoolTime(rhs.m_fDashCoolTime)
	, m_fCurDashCoolTime(rhs.m_fCurDashCoolTime)  
	, m_fDashTime(rhs.m_fDashTime)
	, m_fCurDashTime(rhs.m_fCurDashTime)
	, m_bCanDash(rhs.m_bCanDash)
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
	m_PlayerOption.m_iPistol_BulletCnt = 300;
	m_PlayerOption.m_iRifle_BulletCnt = 50;
	m_PlayerOption.m_iInjector_BulletCnt = 50;
	m_PlayerOption.m_iThrowCnt = 4;

	memset(&m_bCurRoomType, false, sizeof(_bool) * ROOMTYPE_END);
}

void CPlayer::Set_CellIndex(_uint iCellIdx)
{
	// CNavigation::NAVIDESC& NaviDesc = m_pNavigationCom->Get_NaviDescRef();
	// NaviDesc.iCurrentIndex = iCellIdx;
	m_pNavigationCom->Set_CellIndex(iCellIdx);
}

#pragma region Utils
 _uint CPlayer::Get_RifleBulletCnt()
{
	 if (m_PlayerOption.m_iRifle_BulletCnt <= 0)
		 m_PlayerOption.m_iRifle_BulletCnt = 0;

	  return m_PlayerOption.m_iRifle_BulletCnt; 
}

_uint CPlayer::Get_PistolBulletCnt()
{
	if (m_PlayerOption.m_iPistol_BulletCnt <= 0)
		m_PlayerOption.m_iPistol_BulletCnt = 0;

	 return m_PlayerOption.m_iPistol_BulletCnt; 
}

_uint CPlayer::Get_InjectorBulletCnt()
{
	if (m_PlayerOption.m_iInjector_BulletCnt <= 0)
		m_PlayerOption.m_iInjector_BulletCnt = 0;

	return m_PlayerOption.m_iInjector_BulletCnt;
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
#pragma endregion 

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


	m_pModelCom->Set_CurAnimIndex(CWeapon_State::DEFAULT_PISTOL_IDLE);

	m_pTransformCom->Set_Scaled(_float3(0.5f, 0.5f, 0.5f));
	_float4 PlayerPos = m_pTransformCom->Get_State(CTransform::STATE_TRANSLATION);
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(PlayerPos.x, PlayerPos.y + 1.5f, PlayerPos.z, 1.f));


	m_pWeaponState = CWeapon_State::Create(this, m_pState, m_pModelCom, m_pTransformCom, m_pNavigationCom);
	NULL_CHECK_RETURN(m_pWeaponState, E_FAIL);
	FAILED_CHECK_RETURN(Ready_UI(), E_FAIL);

	// m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(-14.f, 0.f, 0.8f, 1.f));
	// m_pNavigationCom->Set_CellIndex(40);

	dynamic_cast<CDynamic_Camera*>(CGameInstance::GetInstance()->Get_CloneObjectList(LEVEL_GAMEPLAY, L"Layer_ZCamera")->front())->Set_LobbyCam(false);

	m_bCurRoomType[ROOM_A] = true;
	
	return S_OK;
}

void CPlayer::Set_On_NaviMesh()
{
	_float4 PlayerPos = m_pTransformCom->Get_State(CTransform::STATE_TRANSLATION);
	_float4 tmp = _float4(PlayerPos.x, PlayerPos.y - 1.5f, PlayerPos.z, 1.f);
	_float4 CellPos = m_pNavigationCom->Get_CellHeight(tmp);

	// PlayerPos.y  = PlayerPos.y + 1.5f;
	// 2.644
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(CellPos.x, CellPos.y +1.5f, CellPos.z, 1.f));
}

void CPlayer::Tick(_double dTimeDelta)
{
	__super::Tick(dTimeDelta);
	if(m_bCurRoomType[ROOM_BOSS] == true)
	{
		_vector vPos = XMVectorSet(109.66f, 1.52f, 129.f, 1.f);
		CGameInstance::GetInstance()->Set_LightPos(0, vPos);
		CGameInstance::GetInstance()->Set_Far(50.f);

	}
	else
	{
		_vector vPos = m_pTransformCom->Get_State(CTransform::STATE_TRANSLATION);

		CGameInstance::GetInstance()->Set_LightPos(0, vPos);
		CGameInstance::GetInstance()->Set_Far(6.f);

	}
	
	
	if (m_PlayerOption.m_iHp <= 0)
		m_PlayerOption.m_iHp = 0;
	if (m_PlayerOption.m_iShieldPoint <= 0)
		m_PlayerOption.m_iShieldPoint = 0;

	m_PlayerOption.m_iShieldPoint += (_int)(dTimeDelta * 30);

	if ((_int)m_PlayerOption.m_iMaxShieldPoint <= m_PlayerOption.m_iShieldPoint)
		m_PlayerOption.m_iShieldPoint = m_PlayerOption.m_iMaxShieldPoint;

	Set_On_NaviMesh();
	{
		
		m_fCurDashCoolTime += (_float)dTimeDelta;

		if(m_bNowIsDash == true)
		{
			m_fCurDashTime += (_float)dTimeDelta;
		}
	}

	if(m_fCurDashCoolTime >= m_fDashCoolTime)
	{
		m_fCurDashCoolTime = 0.f;
		m_bCanDash = true;
	}
	if(m_fCurDashTime >= m_fDashTime && m_bNowIsDash == true)
	{
		m_fCurDashTime = 0.f;
		m_bNowIsDash = false;
	}


	if (_int TurnX = CGameInstance::GetInstance()->Get_DIMouseMove(DIMS_X))
	{
		m_pTransformCom->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), dTimeDelta * TurnX * 0.1f);
	}

	if (_int TurnY  = CGameInstance::GetInstance()->Get_DIMouseMove(DIMS_Y))
	{
		m_pTransformCom->Turn(m_pTransformCom->Get_State(CTransform::STATE_RIGHT), dTimeDelta * TurnY * 0.1f);
	}

	if (CGameInstance::GetInstance()->Key_Down(DIK_F2))
	{
		m_pRendererCom->Switch_Collider_Render();
	}

	if (CGameInstance::GetInstance()->Get_DIKeyState(DIK_F1))
	{
		static_cast<CStatic_Camera*>(CGameInstance::GetInstance()->Get_CloneObjectList(LEVEL_GAMEPLAY, L"Layer_ZCamera")->back())->Set_FixControl();
	}

	if (CGameInstance::GetInstance()->Key_Pressing(DIK_W))
	{
		m_pTransformCom->Go_Straight(dTimeDelta, CTransform::TRANS_PLAYER, m_pNavigationCom);
	}

	if (CGameInstance::GetInstance()->Key_Pressing(DIK_S))
	{
		m_pTransformCom->Go_Backward(dTimeDelta, CTransform::TRANS_PLAYER, m_pNavigationCom);
	}
	if (CGameInstance::GetInstance()->Key_Pressing(DIK_A))
	{
		m_pTransformCom->Go_Left(dTimeDelta, CTransform::TRANS_PLAYER, m_pNavigationCom);
	}
	if (CGameInstance::GetInstance()->Key_Pressing(DIK_D))
	{
		m_pTransformCom->Go_Right(dTimeDelta, CTransform::TRANS_PLAYER, m_pNavigationCom);
	}
	if (CGameInstance::GetInstance()->Key_Down(DIK_LSHIFT) && m_bCanDash && m_bNowIsDash == false)
	{
		m_bNowIsDash = true;
		m_bCanDash = false;
	}
	// if (CGameInstance::GetInstance()->Key_Down(DIK_F6))
	// {
	// 	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);
	// 	CMonster*			pMonster = nullptr;
	//
	// 	_matrix PivotMatrix = XMMatrixIdentity();
	// 	PivotMatrix.r[3] = XMVectorSet(-14.f, 0.f, 0.19f, 1.f);
	//
	// 	CMonster::MONSTEROPTION			MonsterDesc;
	// 	MonsterDesc.m_bFirstSpawnType[CMonster::STATE_UPSPAWN] = true;
	// 	MonsterDesc.MonsterDesc.m_iHP = MonsterDesc.MonsterDesc.m_iMaxHP = 300;
	// 	MonsterDesc.MonsterDesc.m_iDamage = 15;
	// 	MonsterDesc.MonsterDesc.m_iShield = MonsterDesc.MonsterDesc.m_iMaxShield = 0;
	// 	MonsterDesc.MonsterDesc.TransformDesc.fSpeedPerSec = 5.f;
	// 	MonsterDesc.m_iCellIndex = 40;
	// 	pMonster = dynamic_cast<CMonster*>(pGameInstance->Clone_GameObjectReturnPtr_M(LEVEL_GAMEPLAY, L"Layer_Monster", L"Prototype_GameObject_Normal_Human_Spear", PivotMatrix, &MonsterDesc));
	// 	pMonster->Set_Player(this);
	//
	// 	RELEASE_INSTANCE(CGameInstance);
	// }

	_bool&		DynamicCamera = dynamic_cast<CCamera*>(CGameInstance::GetInstance()->Get_CloneObjectList(LEVEL_GAMEPLAY, L"Layer_ZCamera")->front())->Get_RenderState();
	_bool&		StaticCamera = dynamic_cast<CCamera*>(CGameInstance::GetInstance()->Get_CloneObjectList(LEVEL_GAMEPLAY, L"Layer_ZCamera")->back())->Get_RenderState();

	if (CGameInstance::GetInstance()->Key_Down(DIK_F4))
	{
		DynamicCamera = !DynamicCamera;
		StaticCamera = !StaticCamera;
		CGameInstance::GetInstance()->Change_Camera();
	}

	if(m_bNowIsDash)
	{
		m_pTransformCom->Dash(dTimeDelta, CTransform::TRANS_PLAYER, m_pNavigationCom);
	}

	if(CGameInstance::GetInstance()->Key_Pressing(DIK_F3))
	{
		// m_fDegree -=  dTimeDelta * 10.f;
		// _float4 vDirection;
		// _matrix	RotationMatrix = XMMatrixRotationAxis(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(-221.33f));
		// XMStoreFloat4(&vDirection, XMVector3TransformNormal(XMVectorSet(-cosf(XMConvertToRadians(60.f)), -sinf(XMConvertToRadians(60.f)), 0.f, 0.f), RotationMatrix));
		//
		// CGameInstance::GetInstance()->Set_LightDirection(1, vDirection); 
	}

	if(m_pState != nullptr)
		m_pState->Tick(dTimeDelta);

	if(m_pWeaponState != nullptr)
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
				m_pColliderCom[i]->Update(Get_BoneMatrix("Att") * CGameUtils::Get_PlayerPivotMatrix() * m_pTransformCom->Get_WorldMatrix());
			else if (m_PlayerOption.m_wstrCurWeaponName == L"WEAPON_FLAMEBULLET")
			{
				m_pColliderCom[i]->Update(Get_BoneMatrix("Att001") * CGameUtils::Get_PlayerPivotMatrix() * m_pTransformCom->Get_WorldMatrix());
			}
		}
		else if (i == COLLIDER_OBB)
			m_pColliderCom[i]->Update(_matrix(R, U, L, P));
		else 
			m_pColliderCom[i]->Update(m_pTransformCom->Get_WorldMatrix());
	}
	m_vCamPos = (Get_BoneMatrix("Bip001 Head") * CGameUtils::Get_PlayerPivotMatrix() * m_pTransformCom->Get_WorldMatrix()).r[3];
	m_pFirstAimColliderCom->Update(_matrix(R, U, L, P));
	m_pSecondAimColliderCom->Update(_matrix(R, U, L, P));
	// static_cast<CStatic_Camera*>(CGameInstance::GetInstance()->Get_CloneObjectList(LEVEL_GAMEPLAY, L"Layer_ZCamera")->back())->Late_Tick(dTimeDelta);

	// if (CGameInstance::GetInstance()->Key_Down(DIK_F5))
	// {
	// 	_int tmp = m_PlayerOption.m_iHp + m_PlayerOption.m_iShieldPoint;
	// 	_int Damage = 30;
	// 	_int ShieldPoint = m_PlayerOption.m_iShieldPoint;
	//
	// 	_int result = m_PlayerOption.m_iShieldPoint - Damage;
	//
	// 	if (result < 0)
	// 	{
	// 		m_PlayerOption.m_iShieldPoint = 0;
	//
	// 		m_PlayerOption.m_iHp += result;
	// 	}
	// 	else if (result >= 0)
	// 	{
	// 		m_PlayerOption.m_iShieldPoint = result;
	// 	}
	// }
	Set_Camera(dTimeDelta);

}

void CPlayer::Late_Tick(_double dTimeDelta)
{
	__super::Late_Tick(dTimeDelta);
	// Collision_AimBox_To_Monster();

	if(m_pWeaponState != nullptr)
		m_pWeaponState->Late_Tick(dTimeDelta);
	
	static_cast<CStatic_Camera*>(CGameInstance::GetInstance()->Get_CloneObjectList(LEVEL_GAMEPLAY, L"Layer_ZCamera")->back())->Tick(dTimeDelta);

	for (_uint i = 0; i < m_vecPlayerUI.size(); ++i)
	{
		m_vecPlayerUI[i]->Late_Tick(dTimeDelta);
	}

	if (nullptr != m_pRendererCom)
	{
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHABLEND, this);

		for (_uint i = 0; i < COLLIDERTYPE_END; ++i)
		{
			if (nullptr != m_pColliderCom[i])
				m_pRendererCom->Add_DebugRenderGroup(m_pColliderCom[i]);
		}
		m_pRendererCom->Add_DebugRenderGroup(m_pFirstAimColliderCom);
		m_pRendererCom->Add_DebugRenderGroup(m_pSecondAimColliderCom);
		m_pRendererCom->Add_DebugRenderGroup(m_pNavigationCom);
	}
}

HRESULT CPlayer::Render()
{
	
	FAILED_CHECK_RETURN(__super::Render(), E_FAIL);

	_uint iNumMeshes = m_pModelCom->Get_NumMeshes();

	for (_uint i = 0; i < iNumMeshes; ++i)
	{
		HRESULT hNormal = m_pModelCom->Bind_Material(m_pShaderCom, i, aiTextureType_NORMALS, L"g_NormalTexture");
		if(hNormal == S_FALSE)
		{
			m_pModelCom->Bind_Material(m_pShaderCom, i, aiTextureType_DIFFUSE, L"g_DiffuseTexture");
			m_pSpecularMap_Arm->Bind_ShaderResource(m_pShaderCom, L"g_ModelSpecularTexture");
			m_bNormalTexOn = false;
		}
		else if (hNormal == S_OK)
		{
			m_pModelCom->Bind_Material(m_pShaderCom, i, aiTextureType_DIFFUSE, L"g_DiffuseTexture");

			// m_pSpecularMap_Weapon->Bind_ShaderResource(m_pShaderCom, L"g_ModelSpecularTexture");
			m_bNormalTexOn = true;
		}

		HRESULT hSpecular = m_pModelCom->Bind_Material(m_pShaderCom, i, aiTextureType_OPACITY, L"g_ModelSpecularTexture");
		if(hSpecular == S_FALSE)
		{
			m_bSpecularTexOn = false;
		}
		if(hSpecular== S_OK)
		{
			m_bSpecularTexOn = true;
		}
		FAILED_CHECK_RETURN(SetUp_ShaderResources(), E_FAIL);

		m_pModelCom->Render(m_pShaderCom, i, L"g_BoneMatrices",1);
		m_pModelCom->Render(m_pShaderCom, i, L"g_BoneMatrices");
	}

	return S_OK;
}


void CPlayer::Set_Camera(_double dTimeDelta)
{
	static_cast<CStatic_Camera*>(CGameInstance::GetInstance()->Get_CloneObjectList(LEVEL_GAMEPLAY, L"Layer_ZCamera")->
		back())->Camera_Update(	dTimeDelta, m_pTransformCom->Get_WorldMatrix());
}

_bool CPlayer::Collision_Detected(CCollider* pOtherCollider)
{
	NULL_CHECK_RETURN(pOtherCollider, false);

	return m_pColliderCom[COLLIDER_SPHERE]->Collision(pOtherCollider);
}

CGameObject* CPlayer::Collision_AimBox_To_Monster()
{
	// CCollider*		pTargetCollider = (CCollider*)CGameInstance::GetInstance()->Get_ComponentPtr(LEVEL_GAMEPLAY, L"Layer_Monster", L"Com_SPHERE");

	// if (nullptr == pTargetCollider)
		// return nullptr;


	return	nullptr;//m_pColliderCom[COLLIDER_OBB]->CollisionReturnObj(pTargetCollider);
}

void CPlayer::Collision_Event(CGameObject* pMonster)
{
	NULL_CHECK_RETURN(pMonster, );

	CMonster::MONSTEROPTION		MonsterOption;
	// MonsterOption.
	// TODO :: 몬스터 옵션 지역변수로 저장하고 플레이어에 적용하기 
}

HRESULT CPlayer::SetUp_Components()
{
	FAILED_CHECK_RETURN(__super::Add_Component(CGameInstance::Get_StaticLevelIndex(), L"Prototype_Component_Renderer", L"Com_Renderer",	(CComponent**)&m_pRendererCom, this), E_FAIL);
	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Shader_VtxAnimModel", L"Com_Shader",	(CComponent**)&m_pShaderCom, this), E_FAIL);

	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Model_Default_Pistol", L"Com_Default_Pistol_Model", (CComponent**)&m_tWeaponDesc[WEAPON_DEFAULT].m_pWeaponModelCom, this), E_FAIL);
	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Model_Fire_Dragon", L"Com_Fire_Dragon_Model", (CComponent**)&m_tWeaponDesc[WEAPON_FIREDRAGON].m_pWeaponModelCom, this), E_FAIL);
	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Model_Flame_Bullet", L"Com_Flame_Bullet_Model", (CComponent**)&m_tWeaponDesc[WEAPON_FLAMEBULLET].m_pWeaponModelCom, this), E_FAIL);
	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Model_Poison", L"Com_Poison_Model", (CComponent**)&m_tWeaponDesc[WEAPON_POISON].m_pWeaponModelCom, this), E_FAIL);

	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Player_Arm_s", L"Com_ArmSTextureCom", (CComponent**)&m_pSpecularMap_Arm, this), E_FAIL);
	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Player_Default_Pistol_s", L"Com_WeaponSTextureCom", (CComponent**)&m_pSpecularMap_Weapon, this), E_FAIL);


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
	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Collider_OBB", L"Com_EyesOBB", (CComponent**)&m_pColliderCom[COLLIDER_OBB], this, &ColliderDesc), E_FAIL);

	// /* For.Com_SPHERE */
	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));
	ColliderDesc.vSize = _float3(3.f, 3.f, 3.f);
	ColliderDesc.vPosition = _float3(0.f, 0.f, 0.f);
	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Collider_SPHERE", L"Com_BODYSPHERE", (CComponent**)&m_pColliderCom[COLLIDER_SPHERE], this, &ColliderDesc), E_FAIL);

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
	
	m_vCamPos = static_cast<CStatic_Camera*>(CGameInstance::GetInstance()->Get_CloneObjectList(LEVEL_GAMEPLAY, L"Layer_ZCamera")->back())->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	FAILED_CHECK_RETURN(m_pShaderCom->Set_RawValue(L"g_vCameraPos", &XMVector3Normalize(_float3(m_vCamPos.x, m_vCamPos.y, m_vCamPos.z)), sizeof(_float3)), E_FAIL);
	FAILED_CHECK_RETURN(m_pShaderCom->Set_RawValue(L"g_OutLineColor", &XMVectorSet(0.f,0.f,0.f,1.f), sizeof(_vector)), E_FAIL);
	FAILED_CHECK_RETURN(m_pShaderCom->Set_RawValue(L"g_bHit", &m_bHitColor, sizeof(_bool)), E_FAIL);
	FAILED_CHECK_RETURN(m_pShaderCom->Set_RawValue(L"g_Outline_Offset", &m_fOutLineOffset, sizeof(_float)), E_FAIL);
	FAILED_CHECK_RETURN(m_pShaderCom->Set_RawValue(L"g_bNormalTexOn", &m_bNormalTexOn, sizeof(_bool)), E_FAIL);

	const LIGHTDESC* pLightDesc = pGameInstance->Get_LightDesc(0);
	if (nullptr == pLightDesc)
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CPlayer::Ready_UI()
{
	CUI*		pPlayerUI = nullptr;

	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	pPlayerUI = dynamic_cast<CUI*>(pGameInstance->Clone_GameObject(L"Prototype_GameObject_PlayerUI_Enter"));
	NULL_CHECK_RETURN(pPlayerUI, E_FAIL);
	pPlayerUI->Set_Owner(this);
	m_vecPlayerUI.push_back(pPlayerUI);

	pPlayerUI = dynamic_cast<CUI*>(pGameInstance->Clone_GameObject(L"Prototype_GameObject_PlayerUI_Base"));
	NULL_CHECK_RETURN(pPlayerUI, E_FAIL);
	pPlayerUI->Set_Owner(this);
	pPlayerUI->Set_Weapon_State(m_pWeaponState);
	m_vecPlayerUI.push_back(pPlayerUI);

	pPlayerUI = dynamic_cast<CUI*>(pGameInstance->Clone_GameObject(L"Prototype_GameObject_PlayerUI_Hp"));
	NULL_CHECK_RETURN(pPlayerUI, E_FAIL);
	pPlayerUI->Set_Owner(this);
	pPlayerUI->Set_Weapon_State(m_pWeaponState);
	m_vecPlayerUI.push_back(pPlayerUI);
	
	pPlayerUI = dynamic_cast<CUI*>(pGameInstance->Clone_GameObject(L"Prototype_GameObject_PlayerUI_Shield"));
	NULL_CHECK_RETURN(pPlayerUI, E_FAIL);
	pPlayerUI->Set_Owner(this);
	pPlayerUI->Set_Weapon_State(m_pWeaponState);
	m_vecPlayerUI.push_back(pPlayerUI);

	pPlayerUI = dynamic_cast<CUI*>(pGameInstance->Clone_GameObject(L"Prototype_GameObject_Player_Skill_BaseTex"));
	NULL_CHECK_RETURN(pPlayerUI, E_FAIL);
	pPlayerUI->Set_Owner(this);
	pPlayerUI->Set_Weapon_State(m_pWeaponState);
	m_vecPlayerUI.push_back(pPlayerUI);

	pPlayerUI = dynamic_cast<CUI*>(pGameInstance->Clone_GameObject(L"Prototype_GameObject_PlayerUI_Skill"));
	NULL_CHECK_RETURN(pPlayerUI, E_FAIL);
	pPlayerUI->Set_Owner(this);
	pPlayerUI->Set_Weapon_State(m_pWeaponState);
	m_vecPlayerUI.push_back(pPlayerUI);

	pPlayerUI = dynamic_cast<CUI*>(pGameInstance->Clone_GameObject(L"Prototype_GameObject_Player_GoldTex"));
	NULL_CHECK_RETURN(pPlayerUI, E_FAIL);
	pPlayerUI->Set_Owner(this);
	pPlayerUI->Set_Weapon_State(m_pWeaponState);
	m_vecPlayerUI.push_back(pPlayerUI);

	pPlayerUI = dynamic_cast<CUI*>(pGameInstance->Clone_GameObject(L"Prototype_GameObject_Player_EmeraldTex"));
	NULL_CHECK_RETURN(pPlayerUI, E_FAIL);
	pPlayerUI->Set_Owner(this);
	pPlayerUI->Set_Weapon_State(m_pWeaponState);
	m_vecPlayerUI.push_back(pPlayerUI);

	pPlayerUI = dynamic_cast<CUI*>(pGameInstance->Clone_GameObject(L"Prototype_GameObject_PlayerUI_Throw"));
	NULL_CHECK_RETURN(pPlayerUI, E_FAIL);
	pPlayerUI->Set_Owner(this);
	pPlayerUI->Set_Weapon_State(m_pWeaponState);
	m_vecPlayerUI.push_back(pPlayerUI);

	pPlayerUI = dynamic_cast<CUI*>(pGameInstance->Clone_GameObject(L"Prototype_GameObject_PlayerUI_Dash"));
	NULL_CHECK_RETURN(pPlayerUI, E_FAIL);
	pPlayerUI->Set_Owner(this);
	pPlayerUI->Set_Weapon_State(m_pWeaponState);
	m_vecPlayerUI.push_back(pPlayerUI);

	pPlayerUI = dynamic_cast<CUI*>(pGameInstance->Clone_GameObject(L"Prototype_GameObject_PlayerUI_Weapon"));
	NULL_CHECK_RETURN(pPlayerUI, E_FAIL);
	pPlayerUI->Set_Owner(this);
	pPlayerUI->Set_Weapon_State(m_pWeaponState);
	m_vecPlayerUI.push_back(pPlayerUI);

	pPlayerUI = dynamic_cast<CUI*>(pGameInstance->Clone_GameObject(L"Prototype_GameObject_PlayerUI_Number_1"));
	NULL_CHECK_RETURN(pPlayerUI, E_FAIL);
	pPlayerUI->Set_Owner(this);
	pPlayerUI->Set_Weapon_State(m_pWeaponState);
	m_vecPlayerUI.push_back(pPlayerUI);

	pPlayerUI = dynamic_cast<CUI*>(pGameInstance->Clone_GameObject(L"Prototype_GameObject_PlayerUI_WeaponPic"));
	NULL_CHECK_RETURN(pPlayerUI, E_FAIL);
	pPlayerUI->Set_Owner(this);
	pPlayerUI->Set_Weapon_State(m_pWeaponState);
	m_vecPlayerUI.push_back(pPlayerUI);
	
	pPlayerUI = dynamic_cast<CUI*>(pGameInstance->Clone_GameObject(L"Prototype_GameObject_PlayerUI_BulletPic"));
	NULL_CHECK_RETURN(pPlayerUI, E_FAIL);
	pPlayerUI->Set_Owner(this);
	pPlayerUI->Set_Weapon_State(m_pWeaponState);
	m_vecPlayerUI.push_back(pPlayerUI);



	CUI::COUNTDESC eType;
	eType.m_eType = CUI::CNT_BULLET;

	pPlayerUI = dynamic_cast<CUI*>(pGameInstance->Clone_GameObject(L"Prototype_GameObject_PlayerUI_BulletCnt", &eType));
	NULL_CHECK_RETURN(pPlayerUI, E_FAIL);
	pPlayerUI->Set_Owner(this);
	pPlayerUI->Set_Weapon_State(m_pWeaponState);
	m_vecPlayerUI.push_back(pPlayerUI);

	eType.m_eType = CUI::CNT_GOLD;
	pPlayerUI = dynamic_cast<CUI*>(pGameInstance->Clone_GameObject(L"Prototype_GameObject_PlayerUI_GoldCnt", &eType));
	NULL_CHECK_RETURN(pPlayerUI, E_FAIL);
	pPlayerUI->Set_Owner(this);
	pPlayerUI->Set_Weapon_State(m_pWeaponState);
	m_vecPlayerUI.push_back(pPlayerUI);

	eType.m_eType = CUI::CNT_THROW;
	pPlayerUI = dynamic_cast<CUI*>(pGameInstance->Clone_GameObject(L"Prototype_GameObject_PlayerUI_ThrowCnt", &eType));
	NULL_CHECK_RETURN(pPlayerUI, E_FAIL);
	pPlayerUI->Set_Owner(this);
	pPlayerUI->Set_Weapon_State(m_pWeaponState);
	m_vecPlayerUI.push_back(pPlayerUI);

	eType.m_eType = CUI::CNT_EMERALD;
	pPlayerUI = dynamic_cast<CUI*>(pGameInstance->Clone_GameObject(L"Prototype_GameObject_PlayerUI_EmeraldCnt", &eType));
	NULL_CHECK_RETURN(pPlayerUI, E_FAIL);
	pPlayerUI->Set_Owner(this);
	pPlayerUI->Set_Weapon_State(m_pWeaponState);
	m_vecPlayerUI.push_back(pPlayerUI);

	pPlayerUI = dynamic_cast<CUI*>(pGameInstance->Clone_GameObject(L"Prototype_GameObject_Battle_Cursor"));
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
	Safe_Release(m_pSpecularMap_Arm);
	Safe_Release(m_pSpecularMap_Weapon);

	Safe_Release(m_pState);
	Safe_Release(m_pNavigationCom);

	if (m_bIsClone)
	{
		for (_uint i = 0; i < WEAPON_END; ++i)
			Safe_Release(m_tWeaponDesc[i].m_pWeaponModelCom);
	}
	Safe_Release(m_pRendererCom);

}
