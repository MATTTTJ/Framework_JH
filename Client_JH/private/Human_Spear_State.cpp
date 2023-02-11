#include "stdafx.h"
#include "..\public\Human_Spear_State.h"
#include "Bullet.h"
#include "Player.h"
#include "GameInstance.h"

CHuman_Spear_State::CHuman_Spear_State()
{
	m_bDamagedAnim = true;
}

HRESULT CHuman_Spear_State::Initialize(CHuman_Spear* pOwner, CState* pStateMachineCom, CModel* pModel, CTransform* pTransform, CNavigation* pNavigation)
{
	m_pGameInstance = CGameInstance::GetInstance();
	m_pMonster = pOwner;
	m_pModelCom = pModel;
	m_pState = pStateMachineCom;
	m_pTransformCom = pTransform;
	m_pNavigationCom = pNavigation;


	memcpy(&m_bFirstState, m_pMonster->m_tMonsterOption.m_bFirstSpawnType, sizeof(_bool) * CHuman_Spear::FIRSTSTATE_END);

	if (true == m_bFirstState[CHuman_Spear::STATE_NODETECTED])
	{
		FAILED_CHECK_RETURN(SetUp_State_No_Detected(), E_FAIL);
		FAILED_CHECK_RETURN(SetUp_State_Idle(), E_FAIL);
	}
	else if (true == m_bFirstState[CHuman_Spear::STATE_UPSPAWN])
	{
		FAILED_CHECK_RETURN(SetUp_State_UpSpawn(), E_FAIL);
		FAILED_CHECK_RETURN(SetUp_State_Idle(), E_FAIL);
	}
	else if (true == m_bFirstState[CHuman_Spear::STATE_ALREADYSPAWN])
	{
		FAILED_CHECK_RETURN(SetUp_State_JustStand(), E_FAIL);
		FAILED_CHECK_RETURN(SetUp_State_Idle(), E_FAIL);
	}
	// FAILED_CHECK_RETURN(SetUp_State_No_Detected(), E_FAIL);
	// FAILED_CHECK_RETURN(SetUp_State_Idle(), E_FAIL);

	return S_OK;
}

void CHuman_Spear_State::Tick(_double dTimeDelta)
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
}

void CHuman_Spear_State::Late_Tick(_double dTimeDelta)
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

void CHuman_Spear_State::Reset_Damaged()
{
}

HRESULT CHuman_Spear_State::SetUp_State_Idle()
{
	NULL_CHECK_RETURN(m_pMonster->m_pState, E_FAIL);

	m_pMonster->m_pState->
		Add_State(L"STATE::IN_COMBAT_IDLE")
		.Init_Start(this, &CHuman_Spear_State::Start_Idle)
		.Init_Tick(this, &CHuman_Spear_State::Tick_Idle)
		.Init_End(this, &CHuman_Spear_State::End_Common)
		.Init_Changer(L"STATE::RUN", this, &CHuman_Spear_State::Player_DetectedAndFar)
		.Init_Changer(L"STATE::ATTACK_A", this, &CHuman_Spear_State::Player_CloseAndCanAttack)
		.Init_Changer(L"STATE::DAMAGED", this, &CHuman_Spear_State::Is_Damaged)
		.Init_Changer(L"STATE::HIDE", this, &CHuman_Spear_State::Bullet_Hide_Collision)

		.Add_State(L"STATE::RUN")
		.Init_Start(this, &CHuman_Spear_State::Start_Run)
		.Init_Tick(this, &CHuman_Spear_State::Tick_Run)
		.Init_End(this, &CHuman_Spear_State::End_Common)
		.Init_Changer(L"STATE::DAMAGED", this, &CHuman_Spear_State::Is_Damaged)
		.Init_Changer(L"STATE::HIDE", this, &CHuman_Spear_State::Bullet_Hide_Collision)
		.Init_Changer(L"STATE::IN_COMBAT_IDLE", this, &CHuman_Spear_State::Player_DetectedAndClose)

		.Add_State(L"STATE::ATTACK_A")
		.Init_Start(this, &CHuman_Spear_State::Start_Attack_A)
		.Init_Tick(this, &CHuman_Spear_State::Tick_Attack_A)
		.Init_End(this, &CHuman_Spear_State::End_Attack_A)
		.Init_Changer(L"STATE::DAMAGED", this, &CHuman_Spear_State::Is_Damaged)
		.Init_Changer(L"STATE::IN_COMBAT_IDLE", this, &CHuman_Spear_State::Animation_Finish)

		.Add_State(L"STATE::DAMAGED")
		.Init_Start(this, &CHuman_Spear_State::Start_Damaged)
		.Init_Tick(this, &CHuman_Spear_State::Tick_Damaged)
		.Init_End(this, &CHuman_Spear_State::End_Damaged)
		.Init_Changer(L"STATE::IN_COMBAT_IDLE", this, &CHuman_Spear_State::Animation_Finish)

		.Add_State(L"STATE::HIDE")
		.Init_Start(this, &CHuman_Spear_State::Start_Hide)
		.Init_Tick(this, &CHuman_Spear_State::Tick_Hide)
		.Init_End(this, &CHuman_Spear_State::End_Hide)
		.Init_Changer(L"STATE::IN_COMBAT_IDLE", this, &CHuman_Spear_State::Animation_Finish)
		.Finish_Setting();

	return S_OK;
}

HRESULT CHuman_Spear_State::SetUp_State_Run()
{
	return S_OK;
}

HRESULT CHuman_Spear_State::SetUp_State_JustStand()
{
	NULL_CHECK_RETURN(m_pMonster->m_pState, E_FAIL);
	m_pMonster->m_pState->Set_Root(L"STATE::JUST_STAND")
		.Add_State(L"STATE::JUST_STAND")
		.Init_Start(this, &CHuman_Spear_State::Start_JustStand)
		.Init_Tick(this, &CHuman_Spear_State::Tick_JustStand)
		.Init_End(this, &CHuman_Spear_State::End_JustStand)
		.Init_Changer(L"STATE::DETECTED_GUARD", this, &CHuman_Spear_State::Is_Damaged)
		.Init_Changer(L"STATE::DETECTED_GUARD", this, &CHuman_Spear_State::Player_Detected)

		.Add_State(L"STATE::DETECTED_GUARD")
		.Init_Start(this, &CHuman_Spear_State::Start_Detected_Guard)
		.Init_Tick(this, &CHuman_Spear_State::Tick_Detected_Guard)
		.Init_End(this, &CHuman_Spear_State::End_Detected_Guard)
		.Init_Changer(L"STATE::IN_COMBAT_IDLE", this, &CHuman_Spear_State::Animation_Finish)

		.Finish_Setting();

	return S_OK;
}

HRESULT CHuman_Spear_State::SetUp_State_HitBody()
{
	return S_OK;
}

HRESULT CHuman_Spear_State::SetUp_State_No_Detected()
{
	NULL_CHECK_RETURN(m_pMonster->m_pState, E_FAIL);
	m_pMonster->m_pState->Set_Root(L"STATE::NO_DETECTED")
		.Add_State(L"STATE::NO_DETECTED")
		.Init_Start(this, &CHuman_Spear_State::Start_No_Detected)
		.Init_Tick(this, &CHuman_Spear_State::Tick_No_Detected)
		.Init_End(this, &CHuman_Spear_State::End_No_Detected)
		.Init_Changer(L"STATE::DETECTED", this, &CHuman_Spear_State::Is_Damaged)

		.Add_State(L"STATE::DETECTED")
		.Init_Start(this, &CHuman_Spear_State::Start_Detected)
		.Init_Tick(this, &CHuman_Spear_State::Tick_Detected)
		.Init_End(this, &CHuman_Spear_State::End_Detected)
		.Init_Changer(L"STATE::IN_COMBAT_IDLE", this, &CHuman_Spear_State::Animation_Finish)
		.Finish_Setting();

	return S_OK;
}

HRESULT CHuman_Spear_State::SetUp_State_Detected()
{
	return S_OK;
}

HRESULT CHuman_Spear_State::SetUp_State_UpSpawn()
{
	NULL_CHECK_RETURN(m_pMonster->m_pState, E_FAIL);
	m_pMonster->m_pState->Set_Root(L"STATE::UP_SPAWN")
		.Add_State(L"STATE::UP_SPAWN")
		.Init_Start(this, &CHuman_Spear_State::Start_UpSpawn)
		.Init_Tick(this, &CHuman_Spear_State::Tick_UpSpawn)
		.Init_End(this, &CHuman_Spear_State::End_UpSpawn)
		.Init_Changer(L"STATE::IN_COMBAT_IDLE", this, &CHuman_Spear_State::Animation_Finish)
		.Finish_Setting();

	return S_OK;
}

HRESULT CHuman_Spear_State::SetUp_State_Attack_A()
{
	return S_OK;
}

HRESULT CHuman_Spear_State::SetUp_State_Hide()
{
	return S_OK;
}

HRESULT CHuman_Spear_State::SetUp_State_Death()
{
	return S_OK;
}

void CHuman_Spear_State::Start_Idle(_double dTimeDelta)
{
	m_pModelCom->Set_LerpTime(0.2f);


	for (_uint i = 0; i < HIT_END; ++i)
	{
		memset(&m_bDamaged[i], false, sizeof(_bool) * HIT_END);
	}

	m_pModelCom->Set_CurAnimIndex(SPEAR_INCOMBAT_IDLE);
	m_bAttackOnce = false; 
}

void CHuman_Spear_State::Start_Run(_double dTimeDelta)
{
	m_pModelCom->Set_LerpTime(0.2f);

	if (m_bFirstState[CHuman_Spear::STATE_ALREADYSPAWN] == true)
	{
		m_pModelCom->Set_CurAnimIndex(SPEAR_RUN_B);
	}
	else
		m_pModelCom->Set_CurAnimIndex(SPEAR_RUN_A);
}

void CHuman_Spear_State::Start_JustStand(_double dTimeDelta)
{
	m_pModelCom->Set_LerpTime(0.2f);

	m_pModelCom->Set_CurAnimIndex(SPEAR_JUSTSTAND);
}

void CHuman_Spear_State::Start_Damaged(_double dTimeDelta)
{// m_pModelCom->Set
	m_pModelCom->Set_LerpTime(0.1f);
	m_pMonster->Set_HideColl(false);

	if (m_bDamaged[HITBODY] == true)
		m_pModelCom->Set_CurAnimIndex(SPEAR_HITBODY);
	else if (m_bDamaged[HITHEAD] == true)
		m_pModelCom->Set_CurAnimIndex(SPEAR_HITHEAD);

	m_bDamaged[HIT] = false;
}

void CHuman_Spear_State::Start_No_Detected(_double dTimeDelta)
{
	m_pModelCom->Set_LerpTime(0.2f);

	m_pModelCom->Set_CurAnimIndex(SPEAR_NODETECTED);
}

void CHuman_Spear_State::Start_Detected(_double dTimeDelta)
{
	m_pModelCom->Set_LerpTime(0.2f);
	m_pModelCom->Set_CurAnimIndex(SPEAR_DETECTED);
}

void CHuman_Spear_State::Start_Detected_Guard(_double dTimeDelta)
{
	m_pModelCom->Set_LerpTime(0.2f);
	m_pTransformCom->LookAt(m_pPlayer->Get_TransformState(CTransform::STATE_TRANSLATION));
	m_pModelCom->Set_CurAnimIndex(SPEAR_GUARD);
}

void CHuman_Spear_State::Start_UpSpawn(_double dTimeDelta)
{
	m_pModelCom->Set_LerpTime(0.5f);
	
	m_pModelCom->Set_CurAnimIndex(SPEAR_UPSPAWN);
}

void CHuman_Spear_State::Start_Attack_A(_double dTimeDelta)
{
	m_pModelCom->Set_LerpTime(0.2f);

	if ((rand() % 4) % 2 == 0)
	{
		m_pModelCom->Set_CurAnimIndex(SPEAR_ATTACK_A);
	}
	else
		m_pModelCom->Set_CurAnimIndex(SPEAR_ATTACK_B);
}

void CHuman_Spear_State::Start_Hide(_double dTimeDelta)
{
	m_pModelCom->Set_LerpTime(0.13f);


	// m_pMonster->m_pColliderCom[CMonster::COLLTYPE_HITHEAD]->Get_SphereCenter();
	if ((rand() % 3) % 2 == 0)
	{
		m_pModelCom->Set_CurAnimIndex(SPEAR_HIDE_LEFT);
	}
	else
		m_pModelCom->Set_CurAnimIndex(SPEAR_HIDE_RIGHT);
}

void CHuman_Spear_State::Start_Death(_double dTimeDelta)
{
	m_pModelCom->Set_LerpTime(0.2f);


	m_pModelCom->Set_CurAnimIndex(SPEAR_JUSTSTAND);
}

void CHuman_Spear_State::Tick_Idle(_double dTimeDelta)
{
}

void CHuman_Spear_State::Tick_Run(_double dTimeDelta)
{
	m_pTransformCom->LookAt_Move_Monster(m_pPlayer->Get_TransformState(CTransform::STATE_TRANSLATION), dTimeDelta, 2.35f, m_pNavigationCom);
}

void CHuman_Spear_State::Tick_JustStand(_double dTimeDelta)
{
}

void CHuman_Spear_State::Tick_Damaged(_double dTimeDelta)
{
	m_pMonster->Set_HideColl(false);
}

void CHuman_Spear_State::Tick_No_Detected(_double dTimeDelta)
{
}

void CHuman_Spear_State::Tick_Detected(_double dTimeDelta)
{
}

void CHuman_Spear_State::Tick_Detected_Guard(_double dTimeDelta)
{
}



void CHuman_Spear_State::Tick_UpSpawn(_double dTimeDelta)
{
}

void CHuman_Spear_State::Tick_Attack_A(_double dTimeDelta)
{
	if (m_pMonster->m_pPlayer->Collision_Detected(m_pMonster->m_pColliderCom[CMonster::COLLTYPE_ATTPOS]) && m_bAttackOnce == false)
	{
		m_pMonster->m_pPlayer->Collision_Event(m_pMonster);
		m_bAttackOnce = true; 
	}

	if (m_pModelCom->Get_CurAnimation() == SPEAR_ATTACK_B)
	{
		if (m_pModelCom->Get_AnimationProgress() > 0.5f &&m_pModelCom->Get_AnimationProgress() < 0.68f)
		{
			m_pTransformCom->Go_Straight(dTimeDelta, CTransform::TRANS_MONSTER, m_pNavigationCom);
			m_bAttackOnce = true;
		}
	}
}

void CHuman_Spear_State::Tick_Hide(_double dTimeDelta)
{
	if (m_pModelCom->Get_CurAnimation() == SPEAR_HIDE_LEFT)
	{
		if (m_pModelCom->Get_AnimationProgress() < 0.5f)
		{
			m_pTransformCom->Go_Left(dTimeDelta, CTransform::TRANS_MONSTER, m_pNavigationCom);
		}
	}
	else if (m_pModelCom->Get_CurAnimation() == SPEAR_HIDE_RIGHT)
	{
		if (m_pModelCom->Get_AnimationProgress() < 0.5f)
		{
			m_pTransformCom->Go_Right(dTimeDelta, CTransform::TRANS_MONSTER, m_pNavigationCom);
		}
	}
}

void CHuman_Spear_State::Tick_Death(_double dTimeDelta)
{
}

void CHuman_Spear_State::End_Common(_double TimeDelta)
{
}

void CHuman_Spear_State::End_Idle(_double dTimeDelta)
{
}

void CHuman_Spear_State::End_Run(_double dTimeDelta)
{
}

void CHuman_Spear_State::End_JustStand(_double dTimeDelta)
{
}

void CHuman_Spear_State::End_Damaged(_double dTimeDelta)
{
	for (_uint i = 0; i < HIT_END; ++i)
	{
		memset(&m_bDamaged[i], false, sizeof(_bool)* HIT_END);
	}

	m_bDamagedAnim = false;
}

void CHuman_Spear_State::End_No_Detected(_double dTimeDelta)
{
	for (_uint i = 0; i < HIT_END; ++i)
		memset(&m_bDamaged[i], false, sizeof(_bool)* HIT_END);

	m_fCurHideCoolTime = 0.f;
	m_pMonster->Set_HideColl(false);
}

void CHuman_Spear_State::End_Detected(_double dTimeDelta)
{
	for (_uint i = 0; i < HIT_END; ++i)
		memset(&m_bDamaged[i], false, sizeof(_bool) * HIT_END);
}

void CHuman_Spear_State::End_Detected_Guard(_double dTimeDelta)
{
}

void CHuman_Spear_State::End_UpSpawn(_double dTimeDelta)
{
}

void CHuman_Spear_State::End_Attack_A(_double dTimeDelta)
{
	m_bCanAttack = false;
	m_bAttackOnce = false;
}

void CHuman_Spear_State::End_Hide(_double dTimeDelta)
{
	m_pMonster->Set_HideColl(false);
	m_bCanHide = false;
}

void CHuman_Spear_State::End_Death(_double dTimeDelta)
{
}

_bool CHuman_Spear_State::Animation_Finish()
{
	return m_pModelCom->Get_AnimationFinish();
}

_bool CHuman_Spear_State::Player_Detected()
{
	return m_pMonster->m_bPlayerDetected;
}

_bool CHuman_Spear_State::Player_DetectedAndFar()
{
	return !m_pMonster->m_bPlayerDetectedClose;
}

_bool CHuman_Spear_State::Player_DetectedAndClose()
{
	return m_pMonster->m_bPlayerDetectedClose;
}

_bool CHuman_Spear_State::Player_CloseAndCanAttack()
{
	if (m_bCanAttack)
	{
		return m_pMonster->m_bPlayerDetectedClose;
	}
	else
		return false;
}

_bool CHuman_Spear_State::Bullet_Hide_Collision()
{
	if (m_bCanHide && m_pState->Get_CurState() != L"STATE::NO_DETECTED")
	{
		return m_pMonster->m_bHideCollision;
	}

	return false;
}

_bool CHuman_Spear_State::Player_NotDetected()
{
	return false;
}

_bool CHuman_Spear_State::Is_Damaged()
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

_bool CHuman_Spear_State::IS_Dead()
{
	return false;
}

CHuman_Spear_State* CHuman_Spear_State::Create(CHuman_Spear* pOwner, CState* pStateMachineCom, CModel* pModel,
	CTransform* pTransform, CNavigation* pNavigation)
{
	CHuman_Spear_State*		pInstance = new CHuman_Spear_State();
	if (FAILED(pInstance->Initialize(pOwner, pStateMachineCom, pModel, pTransform, pNavigation)))
	{
		MSG_BOX("Failed to Create : CHuman_Spear_State");
		Safe_Release(pInstance);

		return nullptr;
	}
	return pInstance;
}

void CHuman_Spear_State::Free()
{
}
