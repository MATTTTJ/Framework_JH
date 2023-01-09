#include "stdafx.h"
#include "..\public\Human_Sword_State.h"
#include "Bullet.h"
#include "Player.h"
#include "Weapon_State.h"
#include "GameInstance.h"

CHuman_Sword_State::CHuman_Sword_State()
{
}

HRESULT CHuman_Sword_State::Initialize(CHuman_Sword* pOwner, CState* pStateMachineCom, CModel* pModel, CTransform* pTransform, CNavigation* pNavigation)
{
	m_pGameInstance = CGameInstance::GetInstance();
	m_pMonster = pOwner;
	m_pModelCom = pModel;
	m_pState = pStateMachineCom;
	m_pTransformCom = pTransform;
	m_pNavigationCom = pNavigation;


	memcpy(&m_bFirstState, m_pMonster->m_tMonsterOption.m_bFirstSpawnType, sizeof(_bool) * 3);

	if(true == m_bFirstState[CHuman_Sword::STATE_NODETECTED])
	{
		FAILED_CHECK_RETURN(SetUp_State_No_Detected(), E_FAIL);
	}
	else if( true == m_bFirstState[CHuman_Sword::STATE_GROUNDSPAWN])
	{
		FAILED_CHECK_RETURN(SetUp_State_GroundSpawn(), E_FAIL);
	}
	else
	{
		FAILED_CHECK_RETURN(SetUp_State_JustStand(), E_FAIL);
		m_pMonster->Set_PlayAnimation(false);
	}

	FAILED_CHECK_RETURN(SetUp_State_Idle(), E_FAIL);
	FAILED_CHECK_RETURN(SetUp_State_Run(), E_FAIL);
	FAILED_CHECK_RETURN(SetUp_State_JustStand(), E_FAIL);
	FAILED_CHECK_RETURN(SetUp_State_HitBody(), E_FAIL);
	FAILED_CHECK_RETURN(SetUp_State_No_Detected(), E_FAIL);
	FAILED_CHECK_RETURN(SetUp_State_Detected(), E_FAIL);
	FAILED_CHECK_RETURN(SetUp_State_GroundSpawn(), E_FAIL);
	FAILED_CHECK_RETURN(SetUp_State_Attack_A(), E_FAIL);
	FAILED_CHECK_RETURN(SetUp_State_Hide(), E_FAIL);
	FAILED_CHECK_RETURN(SetUp_State_Death(), E_FAIL);

	return S_OK;
}

void CHuman_Sword_State::Tick(_double dTimeDelta)
{
	// if (CGameInstance::GetInstance()->Key_Down(DIK_F9))
	// 	m_pMonster->Set_Dead(true);

	m_pPlayer = m_pMonster->m_pPlayer;
	m_pPlayerState = m_pPlayer->Get_WeaponStatePtr();
	m_vecLiveBullet = &m_pPlayerState->Get_LiveBullet();

	// ImGui::Begin("Test");
	// ImGui::Text(CurState);
	// ImGui::End();
}

void CHuman_Sword_State::Late_Tick(_double dTimeDelta)
{
	if (m_pPlayer->Collision_Body(m_pMonster->m_pColliderCom[CMonster::COLLTYPE_DETECTED]))
	{
		m_pMonster->m_bPlayerDetected = true;
	}

	if (m_pPlayer->Collision_Body(m_pMonster->m_pColliderCom[CMonster::COLLTYPE_ATTRANGE]))
	{
		m_pMonster->m_bPlayerDetectedClose = true;
	}
	else
	{
		m_pMonster->m_bPlayerDetectedClose = false;
	}

	

	//
	// _uint BulletCnt = (_uint)m_vecLiveBullet.size();
	//
	// for(_uint i = 0; i <BulletCnt; ++i)
	// {
	// 	if (m_vecLiveBullet[i]->Collision_To_Monster(m_pMonster->m_pColliderCom[CMonster::COLLTYPE_HITHEAD]))
	// 	{
	// 		if (m_vecLiveBullet[i] == nullptr)
	// 			continue;
	//
	// 		m_bDamaged[HIT] = true;
	// 		m_bDamaged[HITHEAD] = true;
	// 		m_pMonster->Collision_Event(m_vecLiveBullet[i]);
	// 	}
	//
	// 	else if (m_vecLiveBullet[i]->Collision_To_Monster(m_pMonster->m_pColliderCom[CMonster::COLLTYPE_HITBODY]))
	// 	{
	// 		if (m_vecLiveBullet[i] == nullptr)
	// 			continue;
	//
	// 		m_bDamaged[HIT] = true;
	// 		m_bDamaged[HITBODY] = true;
	// 		m_pMonster->Collision_Event(m_vecLiveBullet[i]);
	// 	}
	// }
	

}

void CHuman_Sword_State::Reset_Damaged()
{

	memset(m_bDamaged, false, sizeof(_bool) * 3);

	// for(_uint i =0; i < HIT_END; ++i)
	// {
	// 	m_bDamaged[i] = false;
	// }
}

HRESULT CHuman_Sword_State::SetUp_State_No_Detected()
{
	 /*처음부터 생성되어있고 플레이어를 감지하면 Detected 함수로 이동
	 if (true == m_bFirstState[CHuman_Sword::STATE_NODETECTED])
	{
		FAILED_CHECK_RETURN(SetUp_State_No_Detected(), E_FAIL);
	}*/
	NULL_CHECK_RETURN(m_pMonster->m_pState, E_FAIL);
	m_pMonster->m_pState->Set_Root(L"STATE::NO_DETECTED")
		.Add_State(L"STATE::NO_DETECTED")
		.Init_Start(this, &CHuman_Sword_State::Start_No_Detected)
		.Init_Tick(this, &CHuman_Sword_State::Tick_No_Detected)
		.Init_End(this, &CHuman_Sword_State::End_Common)
		.Init_Changer(L"STATE::DETECTED", this, &CHuman_Sword_State::Player_Detected)
		// .Init_Changer(L"STATE::DETECTED", this, &CHuman_Sword_State::Is_Damaged)

		// .Add_State(L"STATE::DETECTED")
		// .Init_Start(this, &CHuman_Sword_State::Start_Detected)
		// .Init_Tick(this, &CHuman_Sword_State::Tick_Detected)
		// .Init_End(this, &CHuman_Sword_State::End_Common)
		// .Init_Changer(L"STATE::IN_COMBAT_IDLE", this, &CHuman_Sword_State::Animation_Finish)
		.Finish_Setting();

	return S_OK;
}

HRESULT CHuman_Sword_State::SetUp_State_Detected()
{
	NULL_CHECK_RETURN(m_pMonster->m_pState, E_FAIL);

	m_pMonster->m_pState->
		Add_State(L"STATE::DETECTED")
		.Init_Start(this, &CHuman_Sword_State::Start_Detected)
		.Init_Tick(this, &CHuman_Sword_State::Tick_Detected)
		.Init_End(this, &CHuman_Sword_State::End_Common)
		.Init_Changer(L"STATE::IN_COMBAT_IDLE", this, &CHuman_Sword_State::Animation_Finish)
		.Finish_Setting();

	return S_OK;
}


HRESULT CHuman_Sword_State::SetUp_State_JustStand()
{
	/*처음부터 생성되어있고 플레이어를 감지하면 Idle 함수로 이동
	else
	{
		FAILED_CHECK_RETURN(SetUp_State_JustStand(), E_FAIL);
		m_pMonster->Set_PlayAnimation(false);
	}
	*/

	return S_OK;
}

HRESULT CHuman_Sword_State::SetUp_State_GroundSpawn()
{
	/*처음부터 생성되어있고 m_bPlayAnimation -> false 상태로 만들고 플레이어를 감지하면 애니메이션 재생 후
	  Idle 함수로 이동
	else if (true == m_bFirstState[CHuman_Sword::STATE_GROUNDSPAWN])
	{
		FAILED_CHECK_RETURN(SetUp_State_GroundSpawn(), E_FAIL);
	}
	*/

	return S_OK;
}

HRESULT CHuman_Sword_State::SetUp_State_Idle()
{
	// 플레이어 감지한 IDLE 상태  **JUSTSTAND 아님
	NULL_CHECK_RETURN(m_pMonster->m_pState, E_FAIL);

	m_pMonster->m_pState->
		Add_State(L"STATE::IN_COMBAT_IDLE")
		.Init_Start(this, &CHuman_Sword_State::Start_Idle)
		.Init_Tick(this, &CHuman_Sword_State::Tick_Idle)
		.Init_End(this, &CHuman_Sword_State::End_Common)
		.Init_Changer(L"STATE::RUN", this, &CHuman_Sword_State::Player_DetectedAndFar)
		.Init_Changer(L"STATE::ATTACK_A", this, &CHuman_Sword_State::Player_DetectedAndClose)
		.Init_Changer(L"STATE::DAMAGED", this, &CHuman_Sword_State::Is_Damaged)
		// .Init_Changer(L"STATE::DAMAGED_HEAD", this, &CHuman_Sword_State::Is_Damaged)
		// .Init_Changer(L"STATE::HIDE", this, &CHuman_Sword_State::Player_ShootOnHead)
		.Init_Changer(L"STATE::DEAD", this, &CHuman_Sword_State::IS_Dead)


		.Add_State(L"STATE::RUN")
		.Init_Start(this, &CHuman_Sword_State::Start_Run)
		.Init_Tick(this, &CHuman_Sword_State::Tick_Run)
		.Init_End(this, &CHuman_Sword_State::End_Common)
		.Init_Changer(L"STATE::DAMAGED", this, &CHuman_Sword_State::Is_Damaged)
		// .Init_Changer(L"STATE::HIDE", this, &CHuman_Sword_State::Player_ShootOnHead) // 한번만
		.Init_Changer(L"STATE::IN_COMBAT_IDLE", this, &CHuman_Sword_State::Player_DetectedAndClose)

		.Add_State(L"STATE::ATTACK_A")
		.Init_Start(this, &CHuman_Sword_State::Start_Attack_A)
		.Init_Tick(this, &CHuman_Sword_State::Tick_Attack_A)
		.Init_End(this, &CHuman_Sword_State::End_Attack_A)
		.Init_Changer(L"STATE::IN_COMBAT_IDLE", this, &CHuman_Sword_State::Animation_Finish)

		.Add_State(L"STATE::DAMAGED")
		.Init_Start(this, &CHuman_Sword_State::Start_Damaged)
		.Init_Tick(this, &CHuman_Sword_State::Tick_Damaged)
		.Init_End(this, &CHuman_Sword_State::End_Damaged)
		.Init_Changer(L"STATE::IN_COMBAT_IDLE", this, &CHuman_Sword_State::Animation_Finish)
		//
		// .Add_State(L"STATE::DAMAGED_HEAD")
		// .Init_Start(this, &CHuman_Sword_State::Start_HitHead)
		// .Init_Tick(this, &CHuman_Sword_State::Start_HitHead)
		// .Init_End(this, &CHuman_Sword_State::Start_HitHead)
		// .Init_Changer(L"STATE::IN_COMBAT_IDLE", this, &CHuman_Sword_State::Animation_Finish)

		// .Add_State(L"STATE::HIDE")
		// .Init_Start(this, &CHuman_Sword_State::Start_Hide)
		// .Init_Tick(this, &CHuman_Sword_State::Start_Hide)
		// .Init_End(this, &CHuman_Sword_State::Start_Hide)
		// .Init_Changer(L"STATE::IN_COMBAT_IDLE", this, &CHuman_Sword_State::Animation_Finish)

		.Finish_Setting();

	//
	// .Init_Changer(L"STATE::ATTACK_A", this, &CHuman_Sword_State::Player_DetectedAndClose)
	// .Init_Changer(L"STATE::DAMAGED_BODY", this, &CHuman_Sword_State::Is_Damaged) // 애니메이션은 재생하지말것
	// .Init_Changer(L"STATE::DAMAGED_HEAD", this, &CHuman_Sword_State::Is_Damaged)
	// .Init_Changer(L"STATE::HIDE_L", this, &CHuman_Sword_State::Player_ShootOnHead)
	// .Init_Changer(L"STATE::HIDE_R", this, &CHuman_Sword_State::Player_ShootOnHead)

	return S_OK;
}




HRESULT CHuman_Sword_State::SetUp_State_Run()
{
	NULL_CHECK_RETURN(m_pMonster->m_pState, E_FAIL);

	// m_pMonster->m_pState->
	// 	Add_State(L"STATE::RUN")
	// 	.Init_Start(this, &CHuman_Sword_State::Start_Run)
	// 	.Init_Tick(this, &CHuman_Sword_State::Tick_Run)
	// 	.Init_End(this, &CHuman_Sword_State::End_Common)
	// 	// .Init_Changer(L"STATE::HIDE", this, &CHuman_Sword_State::Player_ShootOnHead)
	// 	.Init_Changer(L"STATE::IN_COMBAT_IDLE", this, &CHuman_Sword_State::Player_Detected)
	//
	// 	// .Add_State(L"STATE::HIDE")
	// 	// .Init_Start(this, &CHuman_Sword_State::Start_Hide)
	// 	// .Init_Tick(this, &CHuman_Sword_State::Tick_Hide)
	// 	// .Init_End(this, &CHuman_Sword_State::End_Common)
	// 	// .Init_Changer(L"STATE::IN_COMBAT_IDLE", this, &CHuman_Sword_State::Player_DetectedAndClose)
	// 	.Finish_Setting();

	return S_OK;
}



HRESULT CHuman_Sword_State::SetUp_State_HitBody()
{
	NULL_CHECK_RETURN(m_pMonster->m_pState, E_FAIL);
	//
	// m_pMonster->m_pState->
	// 	Add_State(L"STATE::DAMAGED_BODY")
	// 	.Init_Start(this, &CHuman_Sword_State::Start_Damaged)
	// 	.Init_Tick(this, &CHuman_Sword_State::Tick_Damaged)
	// 	.Init_End(this, &CHuman_Sword_State::End_Damaged)
	// 	.Init_Changer(L"STATE::IN_COMBAT_IDLE", this, &CHuman_Sword_State::Animation_Finish)
	// 	.Finish_Setting();

	return S_OK;
}



HRESULT CHuman_Sword_State::SetUp_State_Attack_A()
{
	NULL_CHECK_RETURN(m_pMonster->m_pState, E_FAIL);

	// m_pMonster->m_pState->
	// 	Add_State(L"STATE::ATTACK_A")
	// 	.Init_Start(this, &CHuman_Sword_State::Start_Attack_A)
	// 	.Init_Tick(this, &CHuman_Sword_State::Tick_Attack_A)
	// 	.Init_End(this, &CHuman_Sword_State::End_Attack_A)
	// 	.Init_Changer(L"STATE::IN_COMBAT_IDLE", this, &CHuman_Sword_State::Animation_Finish)
	// 	.Finish_Setting();

	return S_OK;
}



HRESULT CHuman_Sword_State::SetUp_State_Hide()
{
	// NULL_CHECK_RETURN(m_pMonster->m_pState, E_FAIL);
	//
	// m_pMonster->m_pState->
	// Add_State(L"STATE::HIDE")
	// 	.Init_Start(this, &CHuman_Sword_State::Start_Hide)
	// 	.Init_Tick(this, &CHuman_Sword_State::Tick_Hide)
	// 	.Init_End(this, &CHuman_Sword_State::End_Common)
	// 	.Init_Changer(L"STATE::IN_COMBAT_IDLE", this, &CHuman_Sword_State::Player_DetectedAndClose)
	// 	.Finish_Setting();

	return S_OK;
}

HRESULT CHuman_Sword_State::SetUp_State_Death()
{
	// NULL_CHECK_RETURN(m_pMonster->m_pState, E_FAIL);
	//
	// m_pMonster->m_pState->
	// 	Add_State(L"STATE::HIDE")
	// 	.Init_Start(this, &CHuman_Sword_State::Start_Hide)
	// 	.Init_Tick(this, &CHuman_Sword_State::Tick_Hide)
	// 	.Init_End(this, &CHuman_Sword_State::End_Common)
	// 	.Init_Changer(L"STATE::IN_COMBAT_IDLE", this, &CHuman_Sword_State::Player_DetectedAndClose)
	// 	.Finish_Setting();

	return S_OK;
}

//START

void CHuman_Sword_State::Start_No_Detected(_double dTimeDelta)
{
	m_pModelCom->Set_CurAnimIndex(SWORD_NODETECTED);
}

void CHuman_Sword_State::Start_Idle(_double dTimeDelta)
{
	m_pModelCom->Set_CurAnimIndex(SWORD_INCOMBAT_IDLE);
}

void CHuman_Sword_State::Start_Run(_double dTimeDelta)
{
	if (m_bFirstState[CHuman_Sword::STATE_GROUNDSPAWN] == true)
	{
		m_pModelCom->Set_CurAnimIndex(SWORD_RUN_B);
	}
	else
		m_pModelCom->Set_CurAnimIndex(SWORD_RUN_A);

}

void CHuman_Sword_State::Start_JustStand(_double dTimeDelta)
{
	m_pModelCom->Set_CurAnimIndex(SWORD_JUSTSTAND);
}


void CHuman_Sword_State::Start_Damaged(_double dTimeDelta)
{
	if (m_bDamaged[HITBODY] == true)
		m_pModelCom->Set_CurAnimIndex(SWORD_HITBODY);
	else if(m_bDamaged[HITHEAD] == true)
		m_pModelCom->Set_CurAnimIndex(SWORD_HITHEAD);

	m_bDamaged[HIT] = false;
}

void CHuman_Sword_State::Start_Detected(_double dTimeDelta)
{
	m_pModelCom->Set_CurAnimIndex(SWORD_DECTED);
}

void CHuman_Sword_State::Start_GroundSpawn(_double dTimeDelta)
{
	m_pModelCom->Set_CurAnimIndex(SWORD_GROUNDSPAWN);
}

void CHuman_Sword_State::Start_Attack_A(_double dTimeDelta)
{
	if (rand() % 5 + 12 % 2 == 0)
	{
		m_pModelCom->Set_CurAnimIndex(SWORD_ATTACK_A);
	}
	else
		m_pModelCom->Set_CurAnimIndex(SWORD_ATTACK_B);


}



void CHuman_Sword_State::Start_Hide(_double dTimeDelta)
{
	 // m_pMonster->m_pColliderCom[CMonster::COLLTYPE_HITHEAD]->Get_SphereCenter();
	if (rand() % 5 + 12 % 2 == 0)
	{
		m_pModelCom->Set_CurAnimIndex(SWORD_HIDE_LEFT);
	}
	else 
		m_pModelCom->Set_CurAnimIndex(SWORD_HIDE_RIGHT);

}

void CHuman_Sword_State::Start_Death(_double dTimeDelta)
{
	m_pModelCom->Set_CurAnimIndex(SWORD_JUSTSTAND);
}

// TICK
void CHuman_Sword_State::Tick_No_Detected(_double dTimeDelta)
{
}

void CHuman_Sword_State::Tick_Idle(_double dTimeDelta)
{
}

void CHuman_Sword_State::Tick_Run(_double dTimeDelta)
{
}

void CHuman_Sword_State::Tick_JustStand(_double dTimeDelta)
{
}



void CHuman_Sword_State::Tick_Damaged(_double dTimeDelta)
{
}

void CHuman_Sword_State::Tick_Detected(_double dTimeDelta)
{
}

void CHuman_Sword_State::Tick_GroundSpawn(_double dTimeDelta)
{
}

void CHuman_Sword_State::Tick_Attack_A(_double dTimeDelta)
{
	if (m_pMonster->m_pPlayer->Collision_Body(m_pMonster->m_pColliderCom[CMonster::COLLTYPE_ATTPOS]))
	{
		m_pMonster->m_pPlayer->Collision_Event(m_pMonster);
	}
}


void CHuman_Sword_State::Tick_Hide(_double dTimeDelta)
{
}

void CHuman_Sword_State::Tick_Death(_double dTimeDelta)
{
}


//END

void CHuman_Sword_State::End_Common(_double TimeDelta)
{
}

void CHuman_Sword_State::End_No_Detected(_double dTimeDelta)
{
}

void CHuman_Sword_State::End_Idle(_double dTimeDelta)
{
}

void CHuman_Sword_State::End_Run(_double dTimeDelta)
{
}

void CHuman_Sword_State::End_JustStand(_double dTimeDelta)
{
}

void CHuman_Sword_State::End_Damaged(_double dTimeDelta)
{
}

void CHuman_Sword_State::End_Detected(_double dTimeDelta)
{
}

void CHuman_Sword_State::End_GroundSpawn(_double dTimeDelta)
{
}

void CHuman_Sword_State::End_Attack_A(_double dTimeDelta)
{
}



void CHuman_Sword_State::End_Hide(_double dTimeDelta)
{
}

void CHuman_Sword_State::End_Death(_double dTimeDelta)
{
}

_bool CHuman_Sword_State::Animation_Finish()
{
	return m_pModelCom->Get_AnimationFinish();
}

_bool CHuman_Sword_State::Player_Detected()
{
	return m_pMonster->m_bPlayerDetected;
}

_bool CHuman_Sword_State::Player_DetectedAndFar()
{

	return !m_pMonster->m_bPlayerDetectedClose;
}

_bool CHuman_Sword_State::Player_DetectedAndClose()
{
	return m_pMonster->m_bPlayerDetectedClose;
}

_bool CHuman_Sword_State::Player_ShootOnHead()
{
	return false;
}

_bool CHuman_Sword_State::Player_NotDetected()
{
	return false;
}

_bool CHuman_Sword_State::Is_Damaged()
{
	return m_bDamaged[HIT];
}

_bool CHuman_Sword_State::IS_Dead()
{
	return false;
}

CHuman_Sword_State* CHuman_Sword_State::Create(CHuman_Sword* pOwner, CState* pStateMachineCom, CModel* pModel,
                                               CTransform* pTransform, CNavigation* pNavigation)
{
	CHuman_Sword_State*		pInstance = new CHuman_Sword_State();
	if (FAILED(pInstance->Initialize(pOwner, pStateMachineCom, pModel, pTransform, pNavigation)))
	{
		MSG_BOX("Failed to Create : CHuman_Sword_State");
		Safe_Release(pInstance);

		return nullptr;
	}

	return pInstance;
}

void CHuman_Sword_State::Free()
{
}


