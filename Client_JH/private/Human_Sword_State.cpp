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
	FAILED_CHECK_RETURN(SetUp_State_No_Detected(), E_FAIL);
	FAILED_CHECK_RETURN(SetUp_State_Idle(), E_FAIL);
	// FAILED_CHECK_RETURN(SetUp_State_Run(), E_FAIL);
	// FAILED_CHECK_RETURN(SetUp_State_JustStand(), E_FAIL);
	// FAILED_CHECK_RETURN(SetUp_State_HitBody(), E_FAIL);
	// FAILED_CHECK_RETURN(SetUp_State_Detected(), E_FAIL);
	// FAILED_CHECK_RETURN(SetUp_State_GroundSpawn(), E_FAIL);
	// FAILED_CHECK_RETURN(SetUp_State_Attack_A(), E_FAIL);
	// FAILED_CHECK_RETURN(SetUp_State_Hide(), E_FAIL);
	// FAILED_CHECK_RETURN(SetUp_State_Death(), E_FAIL);

	return S_OK;
}

void CHuman_Sword_State::Tick(_double dTimeDelta)
{
	if(m_pMonster->m_tMonsterOption.MonsterDesc.m_iHP <= 0)
	{
		m_pMonster->Set_Dead(true);
		return;
	}

	m_pPlayer = m_pMonster->m_pPlayer;
	m_pPlayerState = m_pPlayer->Get_WeaponStatePtr();

	{
		m_fCurHideCoolTime += (_float)dTimeDelta;
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
	
	// if(m_pGameInstance->Key_Down(DIK_F5))
	// {
	// 	m_pMonster->m_tMonsterOption.MonsterDesc.m_iHP -= 50;
	//
	// 	if (m_pMonster->m_tMonsterOption.MonsterDesc.m_iHP - 1 < EPSILON)
	// 	{
	// 		m_pMonster->m_tMonsterOption.MonsterDesc.m_iHP = 0;
	// 	}
	// }

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
}

void CHuman_Sword_State::Reset_Damaged()
{
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
		.Init_End(this, &CHuman_Sword_State::End_No_Detected)
		.Init_Changer(L"STATE::DETECTED", this, &CHuman_Sword_State::Player_Detected)
		.Init_Changer(L"STATE::DETECTED", this, &CHuman_Sword_State::Is_Damaged)

		.Add_State(L"STATE::DETECTED")
		.Init_Start(this, &CHuman_Sword_State::Start_Detected)
		.Init_Tick(this, &CHuman_Sword_State::Tick_Detected)
		.Init_End(this, &CHuman_Sword_State::End_Detected)
		.Init_Changer(L"STATE::IN_COMBAT_IDLE", this, &CHuman_Sword_State::Animation_Finish)
		.Finish_Setting();

	return S_OK;
}

HRESULT CHuman_Sword_State::SetUp_State_Detected()
{
	return S_OK;
}
HRESULT CHuman_Sword_State::SetUp_State_JustStand()
{
	return S_OK;
}
HRESULT CHuman_Sword_State::SetUp_State_GroundSpawn()
{
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
		.Init_Changer(L"STATE::HIDE", this, &CHuman_Sword_State::Bullet_Hide_Collision)

		.Add_State(L"STATE::RUN")
		.Init_Start(this, &CHuman_Sword_State::Start_Run)
		.Init_Tick(this, &CHuman_Sword_State::Tick_Run)
		.Init_End(this, &CHuman_Sword_State::End_Common)
		.Init_Changer(L"STATE::DAMAGED", this, &CHuman_Sword_State::Is_Damaged)
		.Init_Changer(L"STATE::IN_COMBAT_IDLE", this, &CHuman_Sword_State::Player_DetectedAndClose)
		.Init_Changer(L"STATE::HIDE", this, &CHuman_Sword_State::Bullet_Hide_Collision)


		.Add_State(L"STATE::ATTACK_A")
		.Init_Start(this, &CHuman_Sword_State::Start_Attack_A)
		.Init_Tick(this, &CHuman_Sword_State::Tick_Attack_A)
		.Init_End(this, &CHuman_Sword_State::End_Attack_A)
		.Init_Changer(L"STATE::DAMAGED", this, &CHuman_Sword_State::Is_Damaged)
		.Init_Changer(L"STATE::IN_COMBAT_IDLE", this, &CHuman_Sword_State::Animation_Finish)

		.Add_State(L"STATE::DAMAGED")
		.Init_Start(this, &CHuman_Sword_State::Start_Damaged)
		.Init_Tick(this, &CHuman_Sword_State::Tick_Damaged)
		.Init_End(this, &CHuman_Sword_State::End_Damaged)
		.Init_Changer(L"STATE::IN_COMBAT_IDLE", this, &CHuman_Sword_State::Animation_Finish)

		.Add_State(L"STATE::HIDE")
		.Init_Start(this, &CHuman_Sword_State::Start_Hide)
		.Init_Tick(this, &CHuman_Sword_State::Tick_Hide)
		.Init_End(this, &CHuman_Sword_State::End_Hide)
		.Init_Changer(L"STATE::IN_COMBAT_IDLE", this, &CHuman_Sword_State::Animation_Finish)
		.Finish_Setting();

	return S_OK;
}




HRESULT CHuman_Sword_State::SetUp_State_Run()
{
	return S_OK;
}

HRESULT CHuman_Sword_State::SetUp_State_HitBody()
{
	return S_OK;
}

HRESULT CHuman_Sword_State::SetUp_State_Attack_A()
{
	return S_OK;
}

HRESULT CHuman_Sword_State::SetUp_State_Hide()
{
	return S_OK;
}

HRESULT CHuman_Sword_State::SetUp_State_Death()
{
	return S_OK;
}

//START

void CHuman_Sword_State::Start_No_Detected(_double dTimeDelta)
{
	m_pModelCom->Set_LerpTime(0.2f);


	m_pModelCom->Set_CurAnimIndex(SWORD_NODETECTED);
}

void CHuman_Sword_State::Start_Idle(_double dTimeDelta)
{
	m_pModelCom->Set_LerpTime(0.2f);


	for (_uint i = 0; i < HIT_END; ++i)
	{
		memset(&m_bDamaged[i], false, sizeof(_bool));
	}

	m_pModelCom->Set_CurAnimIndex(SWORD_INCOMBAT_IDLE);
}

void CHuman_Sword_State::Start_Run(_double dTimeDelta)
{
	m_pModelCom->Set_LerpTime(0.2f);

	if (m_bFirstState[CHuman_Sword::STATE_GROUNDSPAWN] == true)
	{
		m_pModelCom->Set_CurAnimIndex(SWORD_RUN_B);
	}
	else
		m_pModelCom->Set_CurAnimIndex(SWORD_RUN_A);
}

void CHuman_Sword_State::Start_JustStand(_double dTimeDelta)
{
	m_pModelCom->Set_LerpTime(0.2f);

	m_pModelCom->Set_CurAnimIndex(SWORD_JUSTSTAND);
}


void CHuman_Sword_State::Start_Damaged(_double dTimeDelta)
{
	// m_pModelCom->Set
	m_pModelCom->Set_LerpTime(0.1f);
	m_pMonster->Set_HideColl(false);


	if (m_bDamaged[HITBODY] == true)
		m_pModelCom->Set_CurAnimIndex(SWORD_HITBODY);
	else if(m_bDamaged[HITHEAD] == true)
		m_pModelCom->Set_CurAnimIndex(SWORD_HITHEAD);

	m_bDamaged[HIT] = false;
}

void CHuman_Sword_State::Start_Detected(_double dTimeDelta)
{
	m_pModelCom->Set_LerpTime(0.2f);





	m_pModelCom->Set_CurAnimIndex(SWORD_DECTED);
}

void CHuman_Sword_State::Start_GroundSpawn(_double dTimeDelta)
{
	m_pModelCom->Set_LerpTime(0.2f);

	m_pModelCom->Set_CurAnimIndex(SWORD_GROUNDSPAWN);
}

void CHuman_Sword_State::Start_Attack_A(_double dTimeDelta)
{
	m_pModelCom->Set_LerpTime(0.2f);

	if (rand() % 5 + 12 % 2 == 0)
	{
		m_pModelCom->Set_CurAnimIndex(SWORD_ATTACK_A);
	}
	else
		m_pModelCom->Set_CurAnimIndex(SWORD_ATTACK_B);


}



void CHuman_Sword_State::Start_Hide(_double dTimeDelta)
{
	m_pModelCom->Set_LerpTime(0.13f);


	 // m_pMonster->m_pColliderCom[CMonster::COLLTYPE_HITHEAD]->Get_SphereCenter();
	if (rand() % 3 % 2 == 0)
	{
		m_pModelCom->Set_CurAnimIndex(SWORD_HIDE_LEFT);
	}
	else 
		m_pModelCom->Set_CurAnimIndex(SWORD_HIDE_RIGHT);

}

void CHuman_Sword_State::Start_Death(_double dTimeDelta)
{
	m_pModelCom->Set_LerpTime(0.2f);


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
	m_pTransformCom->LookAt_Monster(m_pPlayer->Get_TransformState(CTransform::STATE_TRANSLATION), dTimeDelta, 2.f);
	m_pTransformCom->Go_Straight(dTimeDelta, CTransform::TRANS_MONSTER, m_pMonster->m_pNavigationCom);
	// m_pTransformCom->Chase(m_pPlayer->Get_TransformState(CTransform::STATE_TRANSLATION), dTimeDelta, 2.f);
	//
	// _float3 vLook = m_pPlayer->Get_TransformState(CTransform::STATE_TRANSLATION) - m_pTransformCom->Get_State(CTransform::STATE_TRANSLATION);
	// vLook = XMVector3Normalize(vLook);
	//
	// const _float2 v2Look { 0.f, 1.f };
	// _float2 v2ToDest{ vLook.x, vLook.z };
	//
	// const _float fDot = v2Look.Dot(v2ToDest);
	//
	// m_pTransformCom->Turn(m_pTransformCom->Get_State(CTransform::STATE_UP), XMConvertToRadians(-acosf(fDot)));
}

void CHuman_Sword_State::Tick_JustStand(_double dTimeDelta)
{
}



void CHuman_Sword_State::Tick_Damaged(_double dTimeDelta)
{
	m_pMonster->Set_HideColl(false);
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
	if (m_pModelCom->Get_CurAnimation() == SWORD_HIDE_LEFT)
	{
		if(m_pModelCom->Get_AnimationProgress() < 0.5f)
		{
			m_pTransformCom->Go_Left(dTimeDelta, CTransform::TRANS_MONSTER, m_pNavigationCom);
		}
	}
	else if(m_pModelCom->Get_CurAnimation() == SWORD_HIDE_RIGHT)
	{
		if (m_pModelCom->Get_AnimationProgress() < 0.5f)
		{
			m_pTransformCom->Go_Right(dTimeDelta, CTransform::TRANS_MONSTER, m_pNavigationCom);
		}
	}
	
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
	for (_uint i = 0; i < HIT_END; ++i)
		memset(&m_bDamaged[i], false, sizeof(_bool));

	m_fCurHideCoolTime = 0.f;
	m_pMonster->Set_HideColl(false);
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
	for(_uint i =0; i < HIT_END; ++i)
	{
		memset(&m_bDamaged[i], false, sizeof(_bool));
	}

	m_bDamagedAnim = false; 
}

void CHuman_Sword_State::End_Detected(_double dTimeDelta)
{
	for (_uint i = 0; i < HIT_END; ++i)
		memset(&m_bDamaged[i], false, sizeof(_bool));
}

void CHuman_Sword_State::End_GroundSpawn(_double dTimeDelta)
{
}

void CHuman_Sword_State::End_Attack_A(_double dTimeDelta)
{
	m_bCanAttack = false; 
}

void CHuman_Sword_State::End_Hide(_double dTimeDelta)
{
	m_pMonster->Set_HideColl(false);
	m_bCanHide = false;
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
	if (m_bCanAttack)
	{
		return m_pMonster->m_bPlayerDetectedClose;
	}

	return false;
}

_bool CHuman_Sword_State::Bullet_Hide_Collision()
{

	if (m_bCanHide && m_pState->Get_CurState() != L"STATE::NO_DETECTED")
	{
		return m_pMonster->m_bHideCollision;
	}

	return false;
}

_bool CHuman_Sword_State::Player_NotDetected()
{
	return false;
}

_bool CHuman_Sword_State::Is_Damaged()
{
	if(m_bDamagedAnim)
	{
		return m_bDamaged[HIT];
	}

	return false;
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


