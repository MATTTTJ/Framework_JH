#include "stdafx.h"
#include "..\public\Elite_Bug_State.h"
#include "Bullet.h"
#include "Player.h"
#include "GameInstance.h"


CElite_Bug_State::CElite_Bug_State()
{
	m_bDamagedAnim = true;
	m_fRushTime = 3.f;
}

HRESULT CElite_Bug_State::Initialize(CElite_Bug* pOwner, CState* pStateMachineCom, CModel* pModel,
	CTransform* pTransform, CNavigation* pNavigation)
{
	m_pGameInstance = CGameInstance::GetInstance();
	m_pMonster = pOwner;
	m_pModelCom = pModel;
	m_pState = pStateMachineCom;
	m_pTransformCom = pTransform;
	m_pNavigationCom = pNavigation;


	memcpy(&m_bFirstState, m_pMonster->m_tMonsterOption.m_bFirstSpawnType, sizeof(_bool) * CElite_Bug::FIRSTSTATE_END);

	FAILED_CHECK_RETURN(SetUp_State_No_Detected(), E_FAIL);
	FAILED_CHECK_RETURN(SetUp_State_Idle(), E_FAIL);

	return S_OK;
}

void CElite_Bug_State::Tick(_double dTimeDelta)
{
	if (m_pMonster->m_tMonsterOption.MonsterDesc.m_iHP <= 0)
	{
		m_pMonster->Set_Dead(true);
		return;
	}

	m_pPlayer = m_pMonster->m_pPlayer;

	{
		if (m_bCanSpawnJunior == false)
		{
			m_fCurSpawnJuniorCoolTime += (_float)dTimeDelta;
		}
		m_fCurAttackCoolTime += (_float)dTimeDelta;
		m_fCurPatrolTurnTime += (_float)dTimeDelta;

		if (m_pState->Get_CurState() == L"STATE::READY_RUSH_LOOP")
		{
			m_fCurChargingTime += (_float)dTimeDelta;
		}

		if (m_pState->Get_CurState() == L"STATE::RUSH_LOOP")
		{
			m_fCurRushTime += (_float)dTimeDelta;
		}
	}


	if (m_fCurChargingTime >= m_fChargingTime)
	{
		m_fCurChargingTime = 0.f;
		m_bCanRushStart = true;
	}

	if (m_fCurRushTime >= m_fRushTime)
	{
		m_fCurRushTime = 0.f;
		m_bStopRush = true;
	}

	if (m_fCurSpawnJuniorCoolTime >= m_fSpawnJuniorCoolTime && m_bCanSpawnJunior == false)
	{
		m_fCurSpawnJuniorCoolTime = 0.f;
		m_bCanSpawnJunior = true;
	}

	if (m_fCurPatrolTurnTime >= m_fPatrolTurnTime)
	{
		m_fCurPatrolTurnTime = 0.f;
		m_bTurnPatrolDirection = true;
	}

	if (m_fCurAttackCoolTime >= m_fAttackCoolTime && m_bCanAttack == false)
	{
		m_fCurAttackCoolTime = 0.f;
		m_bCanAttack = true;
	}

	// if (m_pGameInstance->Key_Down(DIK_F7))
	// {
	// 	CMonster::MONSTEROPTION			MonsterDesc;
	// 	MonsterDesc.m_bFirstSpawnType[CMonster::STATE_ALREADYSPAWN] = true;
	// 	MonsterDesc.MonsterDesc.m_iHP = MonsterDesc.MonsterDesc.m_iMaxHP = 300;
	// 	MonsterDesc.MonsterDesc.m_iDamage = 15;
	// 	MonsterDesc.MonsterDesc.m_iShield = MonsterDesc.MonsterDesc.m_iMaxShield = 0;
	// 	MonsterDesc.MonsterDesc.TransformDesc.fSpeedPerSec = 2.5f;
	// 	MonsterDesc.MonsterDesc.TransformDesc.fRotationPerSec = XMConvertToRadians(180.f);
	// 	MonsterDesc.m_iCellIndex = m_pNavigationCom->Get_NaviDesc().iCurrentIndex;
	//
	// 	_matrix PivotMatrix = XMMatrixIdentity();
	// 	// PivotMatrix = PivotMatrix * XMMatrixScaling(0.4f, 0.4f, 0.4f);
	// 	PivotMatrix.r[3] = m_pTransformCom->Get_State(CTransform::STATE_TRANSLATION);
	// 	CMonster* pMonster = dynamic_cast<CMonster*>(m_pGameInstance->Clone_GameObjectReturnPtr_M(LEVEL_GAMEPLAY, L"Layer_Monster", L"Prototype_GameObject_Normal_Little_Bug", PivotMatrix, &MonsterDesc));
	// 	NULL_CHECK_RETURN(pMonster, );
	// 	pMonster->Set_Player(m_pPlayer);
	// }

}

void CElite_Bug_State::Late_Tick(_double dTimeDelta)
{
	




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

void CElite_Bug_State::Reset_Damaged()
{
}

HRESULT CElite_Bug_State::SetUp_State_Idle()
{
	NULL_CHECK_RETURN(m_pMonster->m_pState, E_FAIL);
	m_pMonster->m_pState->Set_Root(L"STATE::PATROL")
		.Add_State(L"STATE::PATROL")
		.Init_Start(this, &CElite_Bug_State::Start_No_Detected)
		.Init_Tick(this, &CElite_Bug_State::Tick_Patrol)
		.Init_End(this, &CElite_Bug_State::End_Patrol)
		.Init_Changer(L"STATE::IN_COMBAT_IDLE", this, &CElite_Bug_State::Player_Detected)
		.Init_Changer(L"STATE::IN_COMBAT_IDLE", this, &CElite_Bug_State::Is_Damaged)

		.Add_State(L"STATE::IN_COMBAT_IDLE")
		.Init_Start(this, &CElite_Bug_State::Start_Idle)
		.Init_Tick(this, &CElite_Bug_State::Tick_Idle)
		.Init_End(this, &CElite_Bug_State::End_Common)
		.Init_Changer(L"STATE::RUN", this, &CElite_Bug_State::Player_DetectedAndFar)
		.Init_Changer(L"STATE::READY_RUSH", this, &CElite_Bug_State::Player_CloseAndCanAttack)
		.Init_Changer(L"STATE::SPAWN_JUNIOR", this, &CElite_Bug_State::Is_CanSpawnJunior)

		.Add_State(L"STATE::RUN")
		.Init_Start(this, &CElite_Bug_State::Start_Run)
		.Init_Tick(this, &CElite_Bug_State::Tick_Run)
		.Init_End(this, &CElite_Bug_State::End_Common)
		.Init_Changer(L"STATE::IN_COMBAT_IDLE", this, &CElite_Bug_State::Player_DetectedAndClose)

		.Add_State(L"STATE::SPAWN_JUNIOR")
		.Init_Start(this, &CElite_Bug_State::Start_Spawn_Junior)
		.Init_Tick(this, &CElite_Bug_State::Tick_Spawn_Junior)
		.Init_End(this, &CElite_Bug_State::End_Spawn_Junior)
		.Init_Changer(L"STATE::IN_COMBAT_IDLE", this, &CElite_Bug_State::Animation_Finish)

		.Add_State(L"STATE::READY_RUSH")
		.Init_Start(this, &CElite_Bug_State::Start_Ready_Rush)
		.Init_Tick(this, &CElite_Bug_State::Tick_Ready_Rush)
		.Init_End(this, &CElite_Bug_State::End_Ready_Rush)
		.Init_Changer(L"STATE::READY_RUSH_LOOP", this, &CElite_Bug_State::Animation_Finish)

		.Add_State(L"STATE::READY_RUSH_LOOP")
		.Init_Start(this, &CElite_Bug_State::Start_Ready_Rush_Loop)
		.Init_Tick(this, &CElite_Bug_State::Tick_Ready_Rush_Loop)
		.Init_End(this, &CElite_Bug_State::End_Ready_Rush_Loop)
		.Init_Changer(L"STATE::START_RUSH", this, &CElite_Bug_State::Is_ChargingTimeForRush)

		.Add_State(L"STATE::START_RUSH")
		.Init_Start(this, &CElite_Bug_State::Start_Starting_Rush)
		.Init_Tick(this, &CElite_Bug_State::Tick_Starting_Rush)
		.Init_End(this, &CElite_Bug_State::End_Starting_Rush)
		.Init_Changer(L"STATE::RUSH_LOOP", this, &CElite_Bug_State::Animation_Finish)

		.Add_State(L"STATE::RUSH_LOOP")
		.Init_Start(this, &CElite_Bug_State::Start_Rush_Loop)
		.Init_Tick(this, &CElite_Bug_State::Tick_Rush_Loop)
		.Init_End(this, &CElite_Bug_State::End_Rush_Loop)
		.Init_Changer(L"STATE::RUSH_END", this, &CElite_Bug_State::Player_DetectedAndClose)
		.Init_Changer(L"STATE::RUSH_END", this, &CElite_Bug_State::Is_StopRush)

		.Add_State(L"STATE::RUSH_END")
		.Init_Start(this, &CElite_Bug_State::Start_Ending_Rush)
		.Init_Tick(this, &CElite_Bug_State::Tick_Ending_Rush)
		.Init_End(this, &CElite_Bug_State::End_Ending_Rush)
		.Init_Changer(L"STATE::IN_COMBAT_IDLE", this, &CElite_Bug_State::Animation_Finish)
		.Finish_Setting();

	return S_OK;
}

HRESULT CElite_Bug_State::SetUp_State_No_Detected()
{

	return S_OK;
}

HRESULT CElite_Bug_State::SetUp_State_Death()
{
	return S_OK;
}

void CElite_Bug_State::Start_Idle(_double dTimeDelta)
{
	m_pModelCom->Set_LerpTime(0.2f);


	for (_uint i = 0; i < HIT_END; ++i)
	{
		memset(&m_bDamaged[i], false, sizeof(_bool) * HIT_END);
	}

	m_pModelCom->Set_CurAnimIndex(BUG_IN_COMBAT_IDLE);
	m_bAttackOnce = false;
}

void CElite_Bug_State::Start_Run(_double dTimeDelta)
{
	m_pModelCom->Set_LerpTime(0.2f);

	m_pModelCom->Set_CurAnimIndex(BUG_RUN_F);
}

void CElite_Bug_State::Start_No_Detected(_double dTimeDelta)
{
	m_pModelCom->Set_LerpTime(0.2f);

	m_pModelCom->Set_CurAnimIndex(BUG_PATROL);
}

void CElite_Bug_State::Start_Death(_double dTimeDelta)
{
	m_pModelCom->Set_LerpTime(0.2f);


	m_pModelCom->Set_CurAnimIndex(BUG_STAND);
}

void CElite_Bug_State::Start_Ready_Rush(_double dTimeDelta)
{
	m_pModelCom->Set_LerpTime(0.8f);

	m_pModelCom->Set_CurAnimIndex(BUG_READYRUSH);
}

void CElite_Bug_State::Start_Ready_Rush_Loop(_double dTimeDelta)
{
	m_pModelCom->Set_LerpTime(0.1f);

	m_pModelCom->Set_CurAnimIndex(BUG_READYRUSH_LOOP);
}

void CElite_Bug_State::Start_Starting_Rush(_double dTimeDelta)
{
	m_pModelCom->Set_LerpTime(0.4f);

	m_pModelCom->Set_CurAnimIndex(BUG_STARTRUSH);
}

void CElite_Bug_State::Start_Rush_Loop(_double dTimeDelta)
{
	m_pModelCom->Set_LerpTime(0.1f);

	m_pModelCom->Set_CurAnimIndex(BUG_RUSH_LOOP);
}

void CElite_Bug_State::Start_Ending_Rush(_double dTimeDelta)
{
	m_pModelCom->Set_LerpTime(0.1f);

	m_pModelCom->Set_CurAnimIndex(BUG_RUSH_END);
}

void CElite_Bug_State::Start_Spawn_Junior(_double dTimeDelta)
{
	m_pModelCom->Set_LerpTime(0.2f);

	m_pModelCom->Set_CurAnimIndex(BUG_SPAWN_JUNIOR);
	m_fLastPos = m_pTransformCom->Get_State(CTransform::STATE_TRANSLATION);
}

void CElite_Bug_State::Tick_Ready_Rush(_double dTimeDelta)
{
}

void CElite_Bug_State::Tick_Ready_Rush_Loop(_double dTimeDelta)
{
	m_pModelCom->Set_LerpTime(0.1f);

	m_pTransformCom->LookAt_Monster(m_pPlayer->Get_TransformState(CTransform::STATE_TRANSLATION), dTimeDelta);

	m_pModelCom->Set_CurAnimIndex(BUG_READYRUSH_LOOP);
}

void CElite_Bug_State::Tick_Starting_Rush(_double dTimeDelta)
{
	if (m_pModelCom->Get_AnimationProgress() > 0.8f)
	{
		m_pTransformCom->Rush(dTimeDelta, CTransform::TRANS_MONSTER, m_pNavigationCom);
	}
}

void CElite_Bug_State::Tick_Rush_Loop(_double dTimeDelta)
{
	m_pTransformCom->Rush(dTimeDelta, CTransform::TRANS_MONSTER, m_pNavigationCom);
}

void CElite_Bug_State::Tick_Ending_Rush(_double dTimeDelta)
{
}
void CElite_Bug_State::Tick_Spawn_Junior(_double dTimeDelta)
{
	int i = 0;
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, m_fLastPos);
	CMonster::MONSTEROPTION			MonsterDesc;
	MonsterDesc.m_bFirstSpawnType[CMonster::STATE_ALREADYSPAWN] = true;
	MonsterDesc.MonsterDesc.m_iHP = MonsterDesc.MonsterDesc.m_iMaxHP = 100;
	MonsterDesc.MonsterDesc.m_iDamage = 15;
	MonsterDesc.MonsterDesc.m_iShield = MonsterDesc.MonsterDesc.m_iMaxShield = 0;
	MonsterDesc.MonsterDesc.TransformDesc.fSpeedPerSec = 2.5f;
	MonsterDesc.MonsterDesc.TransformDesc.fRotationPerSec = XMConvertToRadians(180.f);
	MonsterDesc.m_iCellIndex = m_pNavigationCom->Get_NaviDesc().iCurrentIndex;

	_matrix PivotMatrix = XMMatrixIdentity();
	// PivotMatrix = PivotMatrix * XMMatrixScaling(0.4f, 0.4f, 0.4f);
	PivotMatrix.r[3] = m_pTransformCom->Get_State(CTransform::STATE_TRANSLATION);

	if (m_pModelCom->Get_AnimationProgress() > 0.2f && m_pModelCom->Get_AnimationProgress() < 0.25f && m_bSpawnOnce == false)
	{
		CMonster* pMonster = dynamic_cast<CMonster*>(m_pGameInstance->Clone_GameObjectReturnPtr_M(LEVEL_GAMEPLAY, L"Layer_Monster", L"Prototype_GameObject_Normal_Little_Bug", PivotMatrix, &MonsterDesc));
		NULL_CHECK_RETURN(pMonster, );
		pMonster->Set_Player(m_pPlayer);

		m_bSpawnOnce = true;
	}
	else if (m_pModelCom->Get_AnimationProgress() > 0.4f && m_pModelCom->Get_AnimationProgress() < 0.45f && m_bSpawnOnce == true)
	{
		CMonster* pMonster = dynamic_cast<CMonster*>(m_pGameInstance->Clone_GameObjectReturnPtr_M(LEVEL_GAMEPLAY, L"Layer_Monster", L"Prototype_GameObject_Normal_Little_Bug", PivotMatrix, &MonsterDesc));
		NULL_CHECK_RETURN(pMonster, );
		pMonster->Set_Player(m_pPlayer);
		m_bSpawnOnce = false;
	}
	else if (m_pModelCom->Get_AnimationProgress() > 0.6f && m_pModelCom->Get_AnimationProgress() < 0.65f && m_bSpawnOnce == false)
	{
		CMonster* pMonster = dynamic_cast<CMonster*>(m_pGameInstance->Clone_GameObjectReturnPtr_M(LEVEL_GAMEPLAY, L"Layer_Monster", L"Prototype_GameObject_Normal_Little_Bug", PivotMatrix, &MonsterDesc));
		NULL_CHECK_RETURN(pMonster, );
		pMonster->Set_Player(m_pPlayer);
		m_bSpawnOnce = true;
	}
	else if (m_pModelCom->Get_AnimationProgress() > 0.8f && m_pModelCom->Get_AnimationProgress() < 0.85f && m_bSpawnOnce == true)
	{
		CMonster* pMonster = dynamic_cast<CMonster*>(m_pGameInstance->Clone_GameObjectReturnPtr_M(LEVEL_GAMEPLAY, L"Layer_Monster", L"Prototype_GameObject_Normal_Little_Bug", PivotMatrix, &MonsterDesc));
		NULL_CHECK_RETURN(pMonster, );
		pMonster->Set_Player(m_pPlayer);
		m_bSpawnOnce = false;
	}
}

void CElite_Bug_State::Tick_Idle(_double dTimeDelta)
{
	m_pMonster->m_pTransformCom->LookAt_Monster(m_pPlayer->Get_TransformState(CTransform::STATE_TRANSLATION), dTimeDelta);
}

void CElite_Bug_State::Tick_Run(_double dTimeDelta)
{
	m_pMonster->m_pTransformCom->LookAt_Move_Monster(m_pPlayer->Get_TransformState(CTransform::STATE_TRANSLATION), dTimeDelta, 2.35f, m_pNavigationCom);
}

void CElite_Bug_State::Tick_Patrol(_double dTimeDelta)
{
	if (m_bTurnPatrolDirection == true)
	{
		_float4 vLook = m_pMonster->m_pTransformCom->Get_State(CTransform::STATE_LOOK);
		m_pMonster->m_pTransformCom->Set_State(CTransform::STATE_LOOK, -vLook);
		m_bTurnPatrolDirection = false;
	}
	m_pMonster->m_pTransformCom->Go_Straight(dTimeDelta, CTransform::TRANS_MONSTER, m_pNavigationCom);
}

void CElite_Bug_State::Tick_Death(_double dTimeDelta)
{

}

void CElite_Bug_State::End_Common(_double TimeDelta)
{
}

void CElite_Bug_State::End_Idle(_double dTimeDelta)
{
}

void CElite_Bug_State::End_Run(_double dTimeDelta)
{
}

void CElite_Bug_State::End_Ready_Rush(_double dTimeDelta)
{
}

void CElite_Bug_State::End_Ready_Rush_Loop(_double dTimeDelta)
{
	m_bCanRushStart = false;
}

void CElite_Bug_State::End_Starting_Rush(_double dTimeDelta)
{
}

void CElite_Bug_State::End_Rush_Loop(_double dTimeDelta)
{
	m_bStopRush = false;
}

void CElite_Bug_State::End_Ending_Rush(_double dTimeDelta)
{
	m_bCanAttack = false;
}

void CElite_Bug_State::End_Spawn_Junior(_double dTimeDelta)
{
	m_bCanSpawnJunior = false;
}


void CElite_Bug_State::End_Patrol(_double dTimeDelta)
{
	for (_uint i = 0; i < HIT_END; ++i)
		memset(&m_bDamaged[i], false, sizeof(_bool)* HIT_END);
}

void CElite_Bug_State::End_Death(_double dTimeDelta)
{
}

_bool CElite_Bug_State::Animation_Finish()
{
	return m_pModelCom->Get_AnimationFinish();
}

_bool CElite_Bug_State::Player_Detected()
{
	return m_pMonster->m_bPlayerDetected;
}

_bool CElite_Bug_State::Player_DetectedAndFar()
{
	return !m_pMonster->m_bPlayerDetectedClose;
}

_bool CElite_Bug_State::Player_DetectedAndClose()
{
	return m_pMonster->m_bPlayerDetectedClose;
}

_bool CElite_Bug_State::Player_CloseAndCanAttack()
{
	if (m_bCanAttack)
	{
		return m_pMonster->m_bPlayerDetectedClose;
	}
	else
		return false;
}

_bool CElite_Bug_State::Player_NotDetected()
{
	return false;
}

_bool CElite_Bug_State::Is_Damaged()
{
	if (m_bDamaged[HIT] == true)
	{
		return true;
	}
	else
	{
		memset(&m_bDamaged[HIT_END], false, sizeof(_bool) * HIT_END);
		return false;
	}
}

_bool CElite_Bug_State::IS_Dead()
{
	return false;
}

_bool CElite_Bug_State::Is_CanSpawnJunior()
{
	return m_bCanSpawnJunior;
}

_bool CElite_Bug_State::Is_ChargingTimeForRush()
{
	return m_bCanRushStart;
}

_bool CElite_Bug_State::Is_StopRush()
{
	return m_bStopRush;
}

CElite_Bug_State* CElite_Bug_State::Create(CElite_Bug* pOwner, CState* pStateMachineCom, CModel* pModel,
	CTransform* pTransform, CNavigation* pNavigation)
{
	CElite_Bug_State*		pInstance = new CElite_Bug_State();
	if (FAILED(pInstance->Initialize(pOwner, pStateMachineCom, pModel, pTransform, pNavigation)))
	{
		MSG_BOX("Failed to Create : CElite_Bug_State");
		Safe_Release(pInstance);

		return nullptr;
	}
	return pInstance;
}

void CElite_Bug_State::Free()
{
}
