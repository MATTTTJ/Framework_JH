#include "stdafx.h"
#include "..\public\Human_Granade_State.h"
#include "Bullet.h"
#include "Player.h"
#include "GameInstance.h"

CHuman_Granade_State::CHuman_Granade_State()
{
	m_bDamagedAnim = true;
	m_fAttackCoolTime = 8.f;
	m_fPatrolTurnTime = 4.f;
}

HRESULT CHuman_Granade_State::Initialize(CHuman_Granade* pOwner, CState* pStateMachineCom, CModel* pModel,
	CTransform* pTransform, CNavigation* pNavigation)
{
	m_pGameInstance = CGameInstance::GetInstance();
	m_pMonster = pOwner;
	m_pModelCom = pModel;
	m_pState = pStateMachineCom;
	m_pTransformCom = pTransform;
	m_pNavigationCom = pNavigation;

	memcpy(&m_bFirstState, m_pMonster->m_tMonsterOption.m_bFirstSpawnType, sizeof(_bool) * CHuman_Granade::FIRSTSTATE_END);

	if (true == m_bFirstState[CHuman_Granade::STATE_NODETECTED])
	{
		FAILED_CHECK_RETURN(SetUp_State_Patrol(), E_FAIL);
		FAILED_CHECK_RETURN(SetUp_State_Idle(), E_FAIL);
	}
	else if (true == m_bFirstState[CHuman_Granade::STATE_ALREADYSPAWN])
	{
		FAILED_CHECK_RETURN(SetUp_State_JustStand(), E_FAIL);
		FAILED_CHECK_RETURN(SetUp_State_Idle(), E_FAIL);
	}
	return S_OK;
}

void CHuman_Granade_State::Tick(_double dTimeDelta)
{
	if (m_pMonster->m_tMonsterOption.MonsterDesc.m_iHP <= 0)
	{
		m_pMonster->Set_Dead(true);
		return;
	}

	m_pPlayer = m_pMonster->m_pPlayer;

	{
		if (m_bCanHide == false)
		{
			m_fCurHideCoolTime += (_float)dTimeDelta;
		}
		m_fCurAttackCoolTime += (_float)dTimeDelta;
		m_fCurDamagedAnimCoolTime += (_float)dTimeDelta;


		m_fCurPatrolTurnTime += (_float)dTimeDelta;

	}

	if (m_fCurHideCoolTime >= m_fHideCoolTime && m_bCanHide == false)
	{
		m_fCurHideCoolTime = 0.f;
		m_bCanHide = true;
	}

	if (m_fCurAttackCoolTime >= m_fAttackCoolTime && m_bCanAttack == false)
	{
		m_fCurAttackCoolTime = 0.f;
		m_bCanAttack = true;
	}

	if (m_fCurDamagedAnimCoolTime >= m_fDamagedAnimCoolTime && m_bDamagedAnim == false)
	{
		m_fCurDamagedAnimCoolTime = 0.f;
		m_bDamagedAnim = true;
	}

	if (m_fCurPatrolTurnTime >= m_fPatrolTurnTime)
	{
		m_fCurPatrolTurnTime = 0.f;
		m_bTurnPatrolDirection = true;
		m_iTurnint = -1;
	}
	else
	{
		m_iTurnint = 1;
	}

	// if (m_pGameInstance->Key_Down(DIK_F7))
	// {
	// 		
	// }
}

void CHuman_Granade_State::Late_Tick(_double dTimeDelta)
{
	_float4	fDir;
	// if (CGameUtils::CollisionSphereSphere(m_pMonster->m_pPlayer->m_pColliderCom[CPlayer::COLLIDER_SPHERE], m_pMonster->m_pColliderCom[CMonster::COLLTYPE_HITBODY], fDir))
	// {
	// 	_vector vPos = m_pTransformCom->Get_State(CTransform::STATE_TRANSLATION);
	// 	fDir.y = 0.f;
	// 	_vector	vMovePos = XMVectorAdd(vPos, fDir);
	//
	// 	_float4 vBlockedLine = { 0.f, 0.f, 0.f, 0.f };
	// 	_float4 vBlockedLineNormal = { 0.f ,0.f, 0.f, 0.f };
	//
	// 	if (true == m_pNavigationCom->IsMove_OnNavigation(vPos, vBlockedLine, vBlockedLineNormal))
	// 		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, vMovePos);
	// 	else
	// 	{
	// 		_vector vInDir = vMovePos - vPos;
	// 		_vector vOutDir = vPos - vMovePos;
	// 		_float	fLength = XMVectorGetX(XMVector3Dot(vOutDir, vBlockedLineNormal));
	//
	// 		_vector vSlidingDir = vInDir + XMLoadFloat4(&vBlockedLineNormal) * fLength;
	//
	// 		vMovePos = vPos + vSlidingDir;
	//
	// 		if (m_pNavigationCom->IsMove_OnNavigation(vMovePos, vBlockedLine, vBlockedLineNormal))
	// 		{
	// 			m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, vMovePos);
	// 		}
	// 	}
	// }

	if (m_pPlayer != nullptr)
	{
		if (m_pPlayer->Collision_Detected(m_pMonster->m_pColliderCom[CMonster::COLLTYPE_DETECTED]))
		{
			m_pMonster->m_bPlayerDetected = true;
		}
		else
		{
			m_pMonster->m_bPlayerDetected = false;
		}

		if (m_pPlayer->Collision_Detected(m_pMonster->m_pColliderCom[CMonster::COLLTYPE_ATTRANGE]))
		{
			m_pMonster->m_bPlayerDetectedClose = true;
		}
		else
		{
			m_pMonster->m_bPlayerDetectedClose = false;
		}
	}
}

void CHuman_Granade_State::Reset_Damaged()
{
}

HRESULT CHuman_Granade_State::SetUp_State_Idle()
{
	NULL_CHECK_RETURN(m_pMonster->m_pState, E_FAIL);

	m_pMonster->m_pState->
		Add_State(L"STATE::IN_COMBAT_IDLE")
		.Init_Start(this, &CHuman_Granade_State::Start_Idle)
		.Init_Tick(this, &CHuman_Granade_State::Tick_Idle)
		.Init_End(this, &CHuman_Granade_State::End_Common)
		.Init_Changer(L"STATE::RUN", this, &CHuman_Granade_State::Player_DetectedAndFar)
		.Init_Changer(L"STATE::MELEE_ATTACK", this, &CHuman_Granade_State::Player_CloseAndCanAttack)
		.Init_Changer(L"STATE::ATTACK_A", this, &CHuman_Granade_State::Player_DetectedAndInBowRange)
		.Init_Changer(L"STATE::DAMAGED", this, &CHuman_Granade_State::Is_Damaged)
		.Init_Changer(L"STATE::HIDE", this, &CHuman_Granade_State::Bullet_Hide_Collision)

		.Add_State(L"STATE::RUN")
		.Init_Start(this, &CHuman_Granade_State::Start_Run)
		.Init_Tick(this, &CHuman_Granade_State::Tick_Run)
		.Init_End(this, &CHuman_Granade_State::End_Common)
		.Init_Changer(L"STATE::DAMAGED", this, &CHuman_Granade_State::Is_Damaged)
		.Init_Changer(L"STATE::HIDE", this, &CHuman_Granade_State::Bullet_Hide_Collision)
		.Init_Changer(L"STATE::IN_COMBAT_IDLE", this, &CHuman_Granade_State::Player_Detected)

		.Add_State(L"STATE::MELEE_ATTACK")
		.Init_Start(this, &CHuman_Granade_State::Start_Melee_Attack)
		.Init_Tick(this, &CHuman_Granade_State::Tick_Melee_Attack)
		.Init_End(this, &CHuman_Granade_State::END_Melee_Attack)
		.Init_Changer(L"STATE::IN_COMBAT_IDLE", this, &CHuman_Granade_State::Animation_Finish)

		.Add_State(L"STATE::ATTACK_A")
		.Init_Start(this, &CHuman_Granade_State::Start_Attack_A)
		.Init_Tick(this, &CHuman_Granade_State::Tick_Attack_A)
		.Init_End(this, &CHuman_Granade_State::End_Attack_A)
		.Init_Changer(L"STATE::RELOAD", this, &CHuman_Granade_State::Animation_Finish)

		.Add_State(L"STATE::RELOAD")
		.Init_Start(this, &CHuman_Granade_State::Start_Reload)
		.Init_Tick(this, &CHuman_Granade_State::Tick_Reload)
		.Init_End(this, &CHuman_Granade_State::End_Common)
		.Init_Changer(L"STATE::IN_COMBAT_IDLE", this, &CHuman_Granade_State::Animation_Finish)

		.Add_State(L"STATE::DAMAGED")
		.Init_Start(this, &CHuman_Granade_State::Start_Damaged)
		.Init_Tick(this, &CHuman_Granade_State::Tick_Damaged)
		.Init_End(this, &CHuman_Granade_State::End_Damaged)
		.Init_Changer(L"STATE::IN_COMBAT_IDLE", this, &CHuman_Granade_State::Animation_Finish)

		.Add_State(L"STATE::HIDE")
		.Init_Start(this, &CHuman_Granade_State::Start_Hide)
		.Init_Tick(this, &CHuman_Granade_State::Tick_Hide)
		.Init_End(this, &CHuman_Granade_State::End_Hide)
		.Init_Changer(L"STATE::IN_COMBAT_IDLE", this, &CHuman_Granade_State::Animation_Finish)
		.Finish_Setting();

	return S_OK;
}

HRESULT CHuman_Granade_State::SetUp_State_Run()
{
	return S_OK;
}

HRESULT CHuman_Granade_State::SetUp_State_JustStand()
{
	NULL_CHECK_RETURN(m_pMonster->m_pState, E_FAIL);
	m_pMonster->m_pState->Set_Root(L"STATE::JUST_STAND")
		.Add_State(L"STATE::JUST_STAND")
		.Init_Start(this, &CHuman_Granade_State::Start_JustStand)
		.Init_Tick(this, &CHuman_Granade_State::Tick_JustStand)
		.Init_End(this, &CHuman_Granade_State::End_JustStand)
		.Init_Changer(L"STATE::DAMAGED", this, &CHuman_Granade_State::Is_Damaged)
		.Init_Changer(L"STATE::IN_COMBAT_IDLE", this, &CHuman_Granade_State::Player_Detected)
		.Finish_Setting();

	return S_OK;
}

HRESULT CHuman_Granade_State::SetUp_State_HitBody()
{
	return S_OK;
}

HRESULT CHuman_Granade_State::SetUp_State_Patrol()
{
	NULL_CHECK_RETURN(m_pMonster->m_pState, E_FAIL);
	m_pMonster->m_pState->Set_Root(L"STATE::PATROL")
		.Add_State(L"STATE::PATROL")
		.Init_Start(this, &CHuman_Granade_State::Start_Patrol)
		.Init_Tick(this, &CHuman_Granade_State::Tick_Patrol)
		.Init_End(this, &CHuman_Granade_State::End_Patrol)
		.Init_Changer(L"STATE::DAMAGED", this, &CHuman_Granade_State::Is_Damaged)
		.Init_Changer(L"STATE::IN_COMBAT_IDLE", this, &CHuman_Granade_State::Player_Detected)
		.Finish_Setting();

	return S_OK;
}

HRESULT CHuman_Granade_State::SetUp_State_Detected()
{
	return S_OK;
}

HRESULT CHuman_Granade_State::SetUp_State_UpSpawn()
{
	return S_OK;
}

HRESULT CHuman_Granade_State::SetUp_State_Attack_A()
{
	return S_OK;
}

HRESULT CHuman_Granade_State::SetUp_State_Hide()
{
	return S_OK;
}

HRESULT CHuman_Granade_State::SetUp_State_Death()
{
	return S_OK;
}

void CHuman_Granade_State::Start_Idle(_double dTimeDelta)
{
	m_pModelCom->Set_LerpTime(0.2f);

	for (_uint i = 0; i < HIT_END; ++i)
	{
		memset(&m_bDamaged[i], false, sizeof(_bool) * HIT_END);
	}

	m_pModelCom->Set_CurAnimIndex(BOOM_IN_COMBAT_IDLE);
	m_bAttackOnce = false;
}

void CHuman_Granade_State::Start_Run(_double dTimeDelta)
{
	m_pModelCom->Set_LerpTime(0.2f);

	m_pModelCom->Set_CurAnimIndex(BOOM_RUN_F);
}

void CHuman_Granade_State::Start_JustStand(_double dTimeDelta)
{
	m_pModelCom->Set_LerpTime(0.2f);

	m_pModelCom->Set_CurAnimIndex(BOOM_JUSTSTAND);
}

void CHuman_Granade_State::Start_Patrol(_double dTimeDelta)
{
	m_pModelCom->Set_LerpTime(0.2f);

	m_pModelCom->Set_CurAnimIndex(BOOM_PATROL);
}

void CHuman_Granade_State::Start_Damaged(_double dTimeDelta)
{
	m_pModelCom->Set_LerpTime(0.1f);
	m_pMonster->Set_HideColl(false);

	if (m_bDamaged[HITBODY] == true)
		m_pModelCom->Set_CurAnimIndex(BOOM_HITBODY);
	else if (m_bDamaged[HITHEAD] == true)
		m_pModelCom->Set_CurAnimIndex(BOOM_HITHEAD);

	m_bDamaged[HIT] = false;
}

void CHuman_Granade_State::Start_Detected(_double dTimeDelta)
{
	m_pModelCom->Set_LerpTime(0.2f);
	m_pModelCom->Set_CurAnimIndex(BOOM_IN_COMBAT_IDLE);
}

void CHuman_Granade_State::Start_Attack_A(_double dTimeDelta)
{
	m_pModelCom->Set_LerpTime(0.05f);

	m_pModelCom->Set_CurAnimIndex(BOOM_THROW);
}

void CHuman_Granade_State::Start_Melee_Attack(_double dTimeDelta)
{
}

void CHuman_Granade_State::Start_Hide(_double dTimeDelta)
{
	m_pModelCom->Set_LerpTime(0.13f);


	if ((rand() % 3) % 2 == 0)
	{
		m_pModelCom->Set_CurAnimIndex(BOOM_HIDE_LEFT);
	}
	else
		m_pModelCom->Set_CurAnimIndex(BOOM_HIDE_RIGHT);
}

void CHuman_Granade_State::Start_Death(_double dTimeDelta)
{
	m_pModelCom->Set_LerpTime(0.2f);


	m_pModelCom->Set_CurAnimIndex(BOOM_JUSTSTAND);
}

void CHuman_Granade_State::Start_Reload(_double dTimeDelta)
{
}

void CHuman_Granade_State::Tick_Idle(_double dTimeDelta)
{
	m_pTransformCom->LookAt_Monster(m_pPlayer->Get_TransformState(CTransform::STATE_TRANSLATION), dTimeDelta);
}

void CHuman_Granade_State::Tick_Run(_double dTimeDelta)
{
	m_pTransformCom->LookAt_Move_Monster(m_pPlayer->Get_TransformState(CTransform::STATE_TRANSLATION), dTimeDelta, 2.35f, m_pNavigationCom);
}

void CHuman_Granade_State::Tick_JustStand(_double dTimeDelta)
{
}

void CHuman_Granade_State::Tick_Patrol(_double dTimeDelta)
{
	if (m_bTurnPatrolDirection == true)
	{
		_float4 vLook = m_pTransformCom->Get_State(CTransform::STATE_LOOK);
		m_pTransformCom->Set_State(CTransform::STATE_LOOK, -vLook);
		m_bTurnPatrolDirection = false;
	}
	m_pTransformCom->Go_Straight(dTimeDelta, CTransform::TRANS_MONSTER, m_pNavigationCom);
}

void CHuman_Granade_State::Tick_Damaged(_double dTimeDelta)
{
	m_pMonster->Set_HideColl(false);
}

void CHuman_Granade_State::Tick_Detected(_double dTimeDelta)
{
}

void CHuman_Granade_State::Tick_Melee_Attack(_double dTimeDelta)
{
}

void CHuman_Granade_State::Tick_Reload(_double dTimeDelta)
{
}

void CHuman_Granade_State::Tick_Attack_A(_double dTimeDelta)
{
	//다이너마이트 작업하기 
	if (!m_bShotOnce && m_pModelCom->Get_AnimationProgress() > 0.3f)
	{
		CBullet::BULLETOPTION BulletDesc;
		_float4 Position;
		_matrix pivot = XMMatrixIdentity();
		pivot = XMMatrixRotationZ(XMConvertToRadians(180.f));

		XMStoreFloat4(&Position, (m_pMonster->Get_BoneMatrix("Bip001 Prop1") * CGameUtils::Get_PlayerPivotMatrix() * m_pMonster->m_pTransformCom->Get_WorldMatrix()).r[3]);

		BulletDesc.BulletDesc.TransformDesc.vInitPos = _float3(Position.x, Position.y, Position.z);
		_float4 PlayerPos = m_pPlayer->Get_TransformState(CTransform::STATE_TRANSLATION);
		PlayerPos = _float4(PlayerPos.x, PlayerPos.y, PlayerPos.z, PlayerPos.w);
		// BulletDesc.BulletDesc.m_vBulletLook = XMVector4Normalize(PlayerPos - Position);
		BulletDesc.BulletDesc.m_vBulletLook = XMVector4Normalize(m_pMonster->Get_TransformState(CTransform::STATE_LOOK));
		BulletDesc.m_eOwner = CBullet::BULLETOWNERTYPE::OWNER_MONSTER;
		BulletDesc.m_pOwner = m_pMonster;
		CBullet*		pBullet = nullptr;
		pBullet = (CBullet*)(m_pGameInstance->Clone_GameObjectReturnPtr(LEVEL_GAMEPLAY, L"Layer_Bullet", L"Prototype_GameObject_Normal_Human_Granade_Boom", &BulletDesc));
		NULL_CHECK(pBullet);

		m_bShotOnce = true;
	}
}

void CHuman_Granade_State::Tick_Hide(_double dTimeDelta)
{
	if (m_pModelCom->Get_CurAnimation() == BOOM_HIDE_LEFT)
	{
		if (m_pModelCom->Get_AnimationProgress() < 0.5f)
		{
			m_pTransformCom->Go_Left(dTimeDelta, CTransform::TRANS_MONSTER, m_pNavigationCom);
		}
	}
	else if (m_pModelCom->Get_CurAnimation() == BOOM_HIDE_RIGHT)
	{
		if (m_pModelCom->Get_AnimationProgress() < 0.5f)
		{
			m_pTransformCom->Go_Right(dTimeDelta, CTransform::TRANS_MONSTER, m_pNavigationCom);
		}
	}
}

void CHuman_Granade_State::Tick_Death(_double dTimeDelta)
{
}

void CHuman_Granade_State::End_Common(_double TimeDelta)
{
}

void CHuman_Granade_State::End_Idle(_double dTimeDelta)
{
}

void CHuman_Granade_State::End_Run(_double dTimeDelta)
{
}

void CHuman_Granade_State::End_JustStand(_double dTimeDelta)
{
}

void CHuman_Granade_State::End_Patrol(_double dTimeDelta)
{
}

void CHuman_Granade_State::End_Damaged(_double dTimeDelta)
{
	for (_uint i = 0; i < HIT_END; ++i)
	{
		memset(&m_bDamaged[i], false, sizeof(_bool)* HIT_END);
	}

	m_bDamagedAnim = false;
}

void CHuman_Granade_State::End_Detected(_double dTimeDelta)
{
	for (_uint i = 0; i < HIT_END; ++i)
		memset(&m_bDamaged[i], false, sizeof(_bool) * HIT_END);
}

void CHuman_Granade_State::END_Melee_Attack(_double dTimeDelta)
{
}

void CHuman_Granade_State::End_Attack_A(_double dTimeDelta)
{
	m_bCanAttack = false;
	m_bAttackOnce = false;
	m_bShotOnce = false;
}

void CHuman_Granade_State::End_Hide(_double dTimeDelta)
{
	m_pMonster->Set_HideColl(false);
	m_bCanHide = false;
}

void CHuman_Granade_State::End_Death(_double dTimeDelta)
{
}

_bool CHuman_Granade_State::Animation_Finish()
{
	return m_pModelCom->Get_AnimationFinish();
}

_bool CHuman_Granade_State::Player_Detected()
{
	if (m_pMonster->m_bPlayerDetected)
	{
		return m_pMonster->m_bPlayerDetected;
	}

	return false;
}

_bool CHuman_Granade_State::Player_DetectedAndFar()
{
	if (!m_pMonster->m_bPlayerDetected)
	{
		return !m_pMonster->m_bPlayerDetected;
	}

	return false;
}

_bool CHuman_Granade_State::Player_DetectedAndInBowRange()
{
	if (m_bCanAttack)
	{
		return m_pMonster->m_bPlayerDetected;
	}
	return false;
}

_bool CHuman_Granade_State::Player_DetectedAndClose()
{
	return false;
}

_bool CHuman_Granade_State::Player_CloseAndCanAttack()
{
	if (m_bCanAttack)
	{
		return m_pMonster->m_bPlayerDetectedClose;
	}
	else
		return false;
}

_bool CHuman_Granade_State::Bullet_Hide_Collision()
{
	if (m_bCanHide && m_pState->Get_CurState() != L"STATE::NO_DETECTED")
	{
		return m_pMonster->m_bHideCollision;
	}

	return false;
}

_bool CHuman_Granade_State::Player_NotDetected()
{
	return false;
}

_bool CHuman_Granade_State::Is_Damaged()
{
	if (m_bDamagedAnim)
	{
		return m_bDamaged[HIT];
	}
	else
	{
		memset(&m_bDamaged[HIT_END], false, sizeof(_bool) * HIT_END);
		return false;
	}

	return false;
}

_bool CHuman_Granade_State::IS_Dead()
{
	return false;
}

CHuman_Granade_State* CHuman_Granade_State::Create(CHuman_Granade* pOwner, CState* pStateMachineCom, CModel* pModel,
	CTransform* pTransform, CNavigation* pNavigation)
{
	CHuman_Granade_State*		pInstance = new CHuman_Granade_State();
	if (FAILED(pInstance->Initialize(pOwner, pStateMachineCom, pModel, pTransform, pNavigation)))
	{
		MSG_BOX("Failed to Create : CHuman_Granade_State");
		Safe_Release(pInstance);

		return nullptr;
	}
	return pInstance;
}

void CHuman_Granade_State::Free()
{
}
