#include "stdafx.h"
#include "..\public\Human_Explode_State.h"
#include "Bullet.h"
#include "Player.h"
#include "GameInstance.h"

CHuman_Explode_State::CHuman_Explode_State()
{
}

HRESULT CHuman_Explode_State::Initialize(CHuman_Explode* pOwner, CState* pStateMachineCom, CModel* pModel, CTransform* pTransform, CNavigation* pNavigation)
{
	m_pGameInstance = CGameInstance::GetInstance();
	m_pMonster = pOwner;
	m_pModelCom = pModel;
	m_pState = pStateMachineCom;
	m_pTransformCom = pTransform;
	m_pNavigationCom = pNavigation;


	memcpy(&m_bFirstState, m_pMonster->m_tMonsterOption.m_bFirstSpawnType, sizeof(_bool) * CHuman_Explode::FIRSTSTATE_END);

	if (true == m_bFirstState[CHuman_Explode::STATE_NODETECTED])
	{
		FAILED_CHECK_RETURN(SetUp_State_Patrol(), E_FAIL);
		FAILED_CHECK_RETURN(SetUp_State_Idle(), E_FAIL);
	}
	else if (true == m_bFirstState[CHuman_Explode::STATE_ALREADYSPAWN])
	{
		FAILED_CHECK_RETURN(SetUp_State_JustStand(), E_FAIL);
		FAILED_CHECK_RETURN(SetUp_State_Idle(), E_FAIL);
	}

	return S_OK;
}

void CHuman_Explode_State::Tick(_double dTimeDelta)
{
	if (m_pMonster->m_tMonsterOption.MonsterDesc.m_iHP <= 0)
	{
		m_pMonster->Set_Dead(true);
		return;
	}

	m_pPlayer = m_pMonster->m_pPlayer;

	{
		if (!m_bCanHide)
			m_fCurHideCoolTime += (_float)dTimeDelta;

		if (m_bWalkSoundOnce == false && m_pState->Get_CurState() == L"STATE::RUN")
		{
			m_fCurWalkSoundTime += (_float)dTimeDelta;
		}

		m_fCurPatrolTurnTime += (_float)dTimeDelta;

	}

	if (m_fCurWalkSoundTime >= m_fWalkSoundTime)
	{
		m_fCurWalkSoundTime = 0.f;
		m_bWalkSoundOnce = true;
	}

	if (m_fCurPatrolTurnTime >= m_fPatrolTurnTime)
	{
		m_fCurPatrolTurnTime = 0.f;
		m_bTurnPatrolDirection = true;
	}

	if (m_fCurHideCoolTime >= m_fHideCoolTime && m_bCanHide == false)
	{
		m_fCurHideCoolTime = 0.f;
		m_bCanHide = true;
	}
}

void CHuman_Explode_State::Late_Tick(_double dTimeDelta)
{
	// _float4	fDir;
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
	// 	// m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, vPos);
	// }
	if (m_pPlayer != nullptr)
	{
		if (m_pPlayer->Collision_Detected(m_pMonster->m_pColliderCom[CMonster::COLLTYPE_DETECTED]))
		{
			m_pMonster->m_bPlayerDetected = true;
		}
		else
			m_pMonster->m_bPlayerDetected = false;

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

void CHuman_Explode_State::Reset_Damaged()
{
}

HRESULT CHuman_Explode_State::SetUp_State_Idle()
{
	NULL_CHECK_RETURN(m_pMonster->m_pState, E_FAIL);

	m_pMonster->m_pState->
		Add_State(L"STATE::IN_COMBAT_IDLE")
		.Init_Start(this, &CHuman_Explode_State::Start_Idle)
		.Init_Tick(this, &CHuman_Explode_State::Tick_Idle)
		.Init_End(this, &CHuman_Explode_State::End_Common)
		.Init_Changer(L"STATE::RUN", this, &CHuman_Explode_State::Player_DetectedAndFar)
		.Init_Changer(L"STATE::ATTACK_A", this, &CHuman_Explode_State::Player_CloseAndCanAttack)
		.Init_Changer(L"STATE::HIDE", this, &CHuman_Explode_State::Bullet_Hide_Collision)

		.Add_State(L"STATE::RUN")
		.Init_Start(this, &CHuman_Explode_State::Start_Run)
		.Init_Tick(this, &CHuman_Explode_State::Tick_Run)
		.Init_End(this, &CHuman_Explode_State::End_Common)
		.Init_Changer(L"STATE::HIDE", this, &CHuman_Explode_State::Bullet_Hide_Collision)
		.Init_Changer(L"STATE::IN_COMBAT_IDLE", this, &CHuman_Explode_State::Player_DetectedAndClose)

		.Add_State(L"STATE::HIDE")
		.Init_Start(this, &CHuman_Explode_State::Start_Hide)
		.Init_Tick(this, &CHuman_Explode_State::Tick_Hide)
		.Init_End(this, &CHuman_Explode_State::End_Hide)
		.Init_Changer(L"STATE::IN_COMBAT_IDLE", this, &CHuman_Explode_State::Animation_Finish)

		.Add_State(L"STATE::ATTACK_A")
		.Init_Start(this, &CHuman_Explode_State::Start_Attack_A)
		.Init_Tick(this, &CHuman_Explode_State::Tick_Attack_A)
		.Init_End(this, &CHuman_Explode_State::End_Attack_A)
		.Init_Changer(L"STATE::DEAD", this, &CHuman_Explode_State::Animation_Finish)

		.Add_State(L"STATE::DEAD")
		.Init_Start(this, &CHuman_Explode_State::Start_Dead)
		.Init_Tick(this, &CHuman_Explode_State::Tick_Dead)
		.Init_End(this, &CHuman_Explode_State::End_Dead)	
		.Finish_Setting();

	return S_OK;
}

HRESULT CHuman_Explode_State::SetUp_State_Run()
{
	return S_OK;
}

HRESULT CHuman_Explode_State::SetUp_State_JustStand()
{
	m_pMonster->m_pState->Set_Root(L"STATE::JUST_STAND")
		.Add_State(L"STATE::JUST_STAND")
		.Init_Start(this, &CHuman_Explode_State::Start_JustStand)
		.Init_Tick(this, &CHuman_Explode_State::Tick_JustStand)
		.Init_End(this, &CHuman_Explode_State::End_JustStand)
		.Init_Changer(L"STATE::IN_COMBAT_IDLE", this, &CHuman_Explode_State::Is_Damaged)
		.Init_Changer(L"STATE::IN_COMBAT_IDLE", this, &CHuman_Explode_State::Player_Detected)
		.Finish_Setting();

	return S_OK;
}

HRESULT CHuman_Explode_State::SetUp_State_Patrol()
{
	NULL_CHECK_RETURN(m_pMonster->m_pState, E_FAIL);
	m_pMonster->m_pState->Set_Root(L"STATE::PATROL")
		.Add_State(L"STATE::PATROL")
		.Init_Start(this, &CHuman_Explode_State::Start_Patrol)
		.Init_Tick(this, &CHuman_Explode_State::Tick_Patrol)
		.Init_End(this, &CHuman_Explode_State::End_Patrol)
		.Init_Changer(L"STATE::IN_COMBAT_IDLE", this, &CHuman_Explode_State::Player_Detected)
		.Init_Changer(L"STATE::IN_COMBAT_IDLE", this, &CHuman_Explode_State::Is_Damaged)
		.Finish_Setting();

	return S_OK;
}

HRESULT CHuman_Explode_State::SetUp_State_Attack_A()
{
	return S_OK;
}

HRESULT CHuman_Explode_State::SetUp_State_Hide()
{

	return S_OK;
}

void CHuman_Explode_State::Start_Idle(_double dTimeDelta)
{
	m_pModelCom->Set_LerpTime(0.2f);


	for (_uint i = 0; i < HIT_END; ++i)
	{
		memset(&m_bDamaged[i], false, sizeof(_bool)* HIT_END);
	}
	m_pModelCom->Set_CurAnimIndex(EXPLODE_IN_COMBAT_IDLE);
}

void CHuman_Explode_State::Start_Run(_double dTimeDelta)
{
	m_pModelCom->Set_LerpTime(0.2f);

	m_pModelCom->Set_CurAnimIndex(EXPLODE_RUN);
}

void CHuman_Explode_State::Start_JustStand(_double dTimeDelta)
{
	m_pModelCom->Set_LerpTime(0.2f);

	m_pModelCom->Set_CurAnimIndex(EXPLODE_IN_COMBAT_IDLE);
}

void CHuman_Explode_State::Start_Patrol(_double dTimeDelta)
{
	m_pModelCom->Set_LerpTime(0.2f);

	m_pModelCom->Set_CurAnimIndex(EXPLODE_PATROL);
}

void CHuman_Explode_State::Start_Attack_A(_double dTimeDelta)
{
	m_pModelCom->Set_LerpTime(0.2f);

	m_pModelCom->Set_CurAnimIndex(EXPLODE_EXPLODE);
}

void CHuman_Explode_State::Start_Hide(_double dTimeDelta)
{
	m_pModelCom->Set_LerpTime(0.13f);


	if (rand() % 3 % 2 == 0)
	{
		m_pModelCom->Set_CurAnimIndex(EXPLODE_HIDE_LEFT);
	}
	else
		m_pModelCom->Set_CurAnimIndex(EXPLODE_HIDE_RIGHT);

	CGameInstance::GetInstance()->Play_Sound(L"Explode_Hide.mp3", 1.f, false, false, 5);

}

void CHuman_Explode_State::Start_Dead(_double dTimeDelta)
{
	m_pModelCom->Set_LerpTime(0.2f);

	m_pModelCom->Set_CurAnimIndex(EXPLODE_EXPLODE);
}



void CHuman_Explode_State::Tick_Idle(_double dTimeDelta)
{
}

void CHuman_Explode_State::Tick_Run(_double dTimeDelta)
{
	m_pTransformCom->LookAt_Move_Monster(m_pPlayer->Get_TransformState(CTransform::STATE_TRANSLATION), dTimeDelta, 2.35f, m_pNavigationCom);

	if (m_bWalkSoundOnce == true)
	{
		CSound::SOUND_DESC SoundDesc;
		SoundDesc.fRange = 15.f;
		SoundDesc.bIs3D = true;
		SoundDesc.pStartTransform = m_pTransformCom;
		SoundDesc.pTargetTransform = m_pPlayer->Get_Transform();
		CGameInstance::GetInstance()->Set_SoundDesc(L"Explode_Walk.mp3", SoundDesc);

		m_pGameInstance->Play_Sound(L"Explode_Walk.mp3", 1.f, false, true);
		m_bWalkSoundOnce = false;

	}
}

void CHuman_Explode_State::Tick_JustStand(_double dTimeDelta)
{
}

void CHuman_Explode_State::Tick_Patrol(_double dTimeDelta)
{
	if (m_bTurnPatrolDirection == true)
	{
		_float4 vLook = m_pTransformCom->Get_State(CTransform::STATE_LOOK);
		m_pTransformCom->Set_State(CTransform::STATE_LOOK, -vLook);
		m_bTurnPatrolDirection = false;
	}
	m_pTransformCom->Go_Straight(dTimeDelta, CTransform::TRANS_MONSTER, m_pNavigationCom);
}

void CHuman_Explode_State::Tick_Attack_A(_double dTimeDelta)
{
	if (m_pMonster->m_pPlayer->Collision_Detected(m_pMonster->m_pColliderCom[CMonster::COLLTYPE_ATTPOS]))
	{
		m_pMonster->m_pPlayer->Collision_Event(m_pMonster);
		m_bAttackOnce = true;
	}
}

void CHuman_Explode_State::Tick_Hide(_double dTimeDelta)
{
	if (m_pModelCom->Get_CurAnimation() == EXPLODE_HIDE_LEFT)
	{
		if (m_pModelCom->Get_AnimationProgress() < 0.5f)
		{
			m_pTransformCom->Go_Left(dTimeDelta, CTransform::TRANS_MONSTER, m_pNavigationCom);
		}
	}
	else if (m_pModelCom->Get_CurAnimation() == EXPLODE_HIDE_RIGHT)
	{
		if (m_pModelCom->Get_AnimationProgress() < 0.5f)
		{
			m_pTransformCom->Go_Right(dTimeDelta, CTransform::TRANS_MONSTER, m_pNavigationCom);
		}
	}
}

void CHuman_Explode_State::Tick_Dead(_double dTimeDelta)
{
}



void CHuman_Explode_State::End_Idle(_double dTimeDelta)
{
}

void CHuman_Explode_State::End_Run(_double dTimeDelta)
{
}

void CHuman_Explode_State::End_JustStand(_double dTimeDelta)
{
}

void CHuman_Explode_State::End_Patrol(_double dTimeDelta)
{
	for (_uint i = 0; i < HIT_END; ++i)
		memset(&m_bDamaged[i], false, sizeof(_bool)* HIT_END);

	m_fCurHideCoolTime = 0.f;
	m_pMonster->Set_HideColl(false);
}

void CHuman_Explode_State::End_Attack_A(_double dTimeDelta)
{
	m_bAttackOnce = false;
	CGameInstance::GetInstance()->Play_Sound(L"Explode_Fire.mp3", 1.f, false, false, 5);

	m_pMonster->Set_Dead(true);
}

void CHuman_Explode_State::End_Hide(_double dTimeDelta)
{
	m_pMonster->Set_HideColl(false);
	m_bCanHide = false;
}

void CHuman_Explode_State::End_Dead(_double dTimeDelta)
{
}

void CHuman_Explode_State::End_Common(_double dTimeDelta)
{
	for (_uint i = 0; i < HIT_END; ++i)
	{
		memset(&m_bDamaged[i], false, sizeof(_bool)* HIT_END);
	}
}

_bool CHuman_Explode_State::Animation_Finish()
{
	return m_pModelCom->Get_AnimationFinish();
}

_bool CHuman_Explode_State::Player_Detected()
{
	return m_pMonster->m_bPlayerDetected;
}

_bool CHuman_Explode_State::Player_DetectedAndFar()
{
	return !m_pMonster->m_bPlayerDetectedClose;
}

_bool CHuman_Explode_State::Player_DetectedAndClose()
{
	return m_pMonster->m_bPlayerDetectedClose;
}

_bool CHuman_Explode_State::Player_CloseAndCanAttack()
{
	return m_pMonster->m_bPlayerDetectedClose;
}

_bool CHuman_Explode_State::Bullet_Hide_Collision()
{
	if (m_bCanHide && m_pState->Get_CurState() != L"STATE::NO_DETECTED")
	{
		return m_pMonster->m_bHideCollision;
	}

	return false;
}

_bool CHuman_Explode_State::Player_NotDetected()
{
	return false;
}

_bool CHuman_Explode_State::Is_Damaged()
{
	if(m_bDamaged[HIT] == true)
	{
		return true;
	}
	else
	{
		memset(&m_bDamaged, false, sizeof(_bool) * HIT_END);
		return false;
	}
}

_bool CHuman_Explode_State::IS_Dead()
{
	return false;
}

CHuman_Explode_State* CHuman_Explode_State::Create(CHuman_Explode* pOwner, CState* pStateMachineCom, CModel* pModel,
	CTransform* pTransform, CNavigation* pNavigation)
{
	CHuman_Explode_State*		pInstance = new CHuman_Explode_State();
	if (FAILED(pInstance->Initialize(pOwner, pStateMachineCom, pModel, pTransform, pNavigation)))
	{
		MSG_BOX("Failed to Create : CHuman_Explode_State");
		Safe_Release(pInstance);

		return nullptr;
	}

	return pInstance;
}

void CHuman_Explode_State::Free()
{
}
