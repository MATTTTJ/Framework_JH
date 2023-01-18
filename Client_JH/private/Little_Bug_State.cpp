#include "stdafx.h"
#include "..\public\Little_Bug_State.h"
#include "GameInstance.h"
#include "Player.h"
#include "Human_Sword_State.h"
#include "Bullet.h"
#include "UI.h"

CLittle_Bug_State::CLittle_Bug_State()
{
	m_bDamagedAnim = true;
}

HRESULT CLittle_Bug_State::Initialize(CLittle_Bug* pOwner, CState* pStateMachineCom, CModel* pModel,	CTransform* pTransform, CNavigation* pNavigation)
{
	m_pGameInstance = CGameInstance::GetInstance();
	m_pMonster = pOwner;
	m_pModelCom = pModel;
	m_pState = pStateMachineCom;
	m_pTransformCom = pTransform;
	m_pNavigationCom = pNavigation;

	memcpy(&m_bFirstState, m_pMonster->m_tMonsterOption.m_bFirstSpawnType, sizeof(_bool) * CLittle_Bug::FIRSTSTATE_END);
	// Already Spawn
	FAILED_CHECK_RETURN(SetUp_State_Idle(), E_FAIL);
	
	return S_OK;
}

void CLittle_Bug_State::Tick(_double dTimeDelta)
{
	if (m_pMonster->m_tMonsterOption.MonsterDesc.m_iHP <= 0)
	{
		m_pMonster->Set_Dead(true);
		return;
	}

	m_pPlayer = m_pMonster->m_pPlayer;

	{
		m_fCurAttackCoolTime += (_float)dTimeDelta;
		m_fCurDamagedAnimCoolTime += (_float)dTimeDelta;
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

void CLittle_Bug_State::Late_Tick(_double dTimeDelta)
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

void CLittle_Bug_State::Reset_Damaged()
{
}

HRESULT CLittle_Bug_State::SetUp_State_Idle()
{
	NULL_CHECK_RETURN(m_pMonster->m_pState, E_FAIL);

	m_pMonster->m_pState->Set_Root(L"STATE::IN_COMBAT_IDLE")
		.Add_State(L"STATE::IN_COMBAT_IDLE")
		.Init_Start(this, &CLittle_Bug_State::Start_Idle)
		.Init_Tick(this, &CLittle_Bug_State::Tick_Idle)
		.Init_End(this, &CLittle_Bug_State::End_Common)
		.Init_Changer(L"STATE::RUN", this, &CLittle_Bug_State::Player_DetectedAndFar)
		.Init_Changer(L"STATE::DAMAGED", this, &CLittle_Bug_State::Is_Damaged)
		.Init_Changer(L"STATE::ATTACK_A", this, &CLittle_Bug_State::Player_CloseAndCanAttack)

		.Add_State(L"STATE::RUN")
		.Init_Start(this, &CLittle_Bug_State::Start_Run)
		.Init_Tick(this, &CLittle_Bug_State::Tick_Run)
		.Init_End(this, &CLittle_Bug_State::End_Common)
		.Init_Changer(L"STATE::IN_COMBAT_IDLE", this, &CLittle_Bug_State::Player_DetectedAndClose)
		.Init_Changer(L"STATE::DAMAGED", this, &CLittle_Bug_State::Is_Damaged)

		.Add_State(L"STATE::ATTACK_A")
		.Init_Start(this, &CLittle_Bug_State::Start_Attack_A)
		.Init_Tick(this, &CLittle_Bug_State::Tick_Attack_A)
		.Init_End(this, &CLittle_Bug_State::End_Attack_A)
		.Init_Changer(L"STATE::DAMAGED", this, &CLittle_Bug_State::Is_Damaged)
		.Init_Changer(L"STATE::IN_COMBAT_IDLE", this, &CLittle_Bug_State::Animation_Finish)
	
		.Add_State(L"STATE::DAMAGED")
		.Init_Start(this, &CLittle_Bug_State::Start_Damaged)
		.Init_Tick(this, &CLittle_Bug_State::Tick_Damaged)
		.Init_End(this, &CLittle_Bug_State::End_Damaged)
		.Init_Changer(L"STATE::IN_COMBAT_IDLE", this, &CLittle_Bug_State::Animation_Finish)
	
		.Finish_Setting();

	return S_OK;
}

HRESULT CLittle_Bug_State::SetUp_State_Death()
{
	return S_OK;
}

void CLittle_Bug_State::Start_Idle(_double dTimeDelta)
{
	m_pModelCom->Set_LerpTime(0.2f);


	for (_uint i = 0; i < HIT_END; ++i)
	{
		memset(&m_bDamaged[i], false, sizeof(_bool) * HIT_END);
	}

	m_pModelCom->Set_CurAnimIndex(LITTLE_IN_COMBAT_IDLE);
	m_bAttackOnce = false;
}

void CLittle_Bug_State::Start_Run(_double dTimeDelta)
{
	m_pModelCom->Set_LerpTime(0.2f);

	m_pModelCom->Set_CurAnimIndex(LITTLE_RUN);
}

void CLittle_Bug_State::Start_Damaged(_double dTimeDelta)
{
	m_pModelCom->Set_LerpTime(0.1f);
	m_pMonster->Set_HideColl(false);
	m_pModelCom->Set_CurAnimIndex(LITTLE_HITBODY);
	m_bDamaged[HIT] = false;
}

void CLittle_Bug_State::Start_Attack_A(_double dTimeDelta)
{
	m_pModelCom->Set_LerpTime(0.2f);
	m_pModelCom->Set_CurAnimIndex(LITTLE_ATTACK);
}

void CLittle_Bug_State::Start_Death(_double dTimeDelta)
{
	m_pModelCom->Set_LerpTime(0.2f);

	
	m_pModelCom->Set_CurAnimIndex(LITTLE_STAND);
}

void CLittle_Bug_State::Tick_Death(_double dTimeDelta)
{
}

void CLittle_Bug_State::Tick_Damaged(_double dTimeDelta)
{
	m_pMonster->Set_HideColl(false);
}

void CLittle_Bug_State::Tick_Attack_A(_double dTimeDelta)
{
	if (m_pMonster->m_pPlayer->Collision_Detected(m_pMonster->m_pColliderCom[CMonster::COLLTYPE_ATTPOS]))
	{
		m_pMonster->m_pPlayer->Collision_Event(m_pMonster);
		m_bAttackOnce = true;
	}
}
void CLittle_Bug_State::Tick_Idle(_double dTimeDelta)
{
	m_pTransformCom->LookAt_Monster(m_pPlayer->Get_TransformState(CTransform::STATE_TRANSLATION), dTimeDelta);
}

void CLittle_Bug_State::Tick_Run(_double dTimeDelta)
{
	m_pTransformCom->LookAt_Move_Monster(m_pPlayer->Get_TransformState(CTransform::STATE_TRANSLATION), dTimeDelta, 1.5f, m_pNavigationCom);
}


void CLittle_Bug_State::End_Common(_double TimeDelta)
{
}

void CLittle_Bug_State::End_Idle(_double dTimeDelta)
{
}

void CLittle_Bug_State::End_Damaged(_double dTimeDelta)
{
	for (_uint i = 0; i < HIT_END; ++i)
	{
		memset(&m_bDamaged[i], false, sizeof(_bool)* HIT_END);
	}

	m_bDamagedAnim = false;
}

void CLittle_Bug_State::End_Attack_A(_double dTimeDelta)
{
	m_bCanAttack = false;
	m_bAttackOnce = false;
}

void CLittle_Bug_State::End_Death(_double dTimeDelta)
{
}

_bool CLittle_Bug_State::Animation_Finish()
{
	return m_pModelCom->Get_AnimationFinish();
}

_bool CLittle_Bug_State::Player_Detected()
{
	return m_pMonster->m_bPlayerDetected;
}

_bool CLittle_Bug_State::Player_DetectedAndFar()
{
	return !m_pMonster->m_bPlayerDetectedClose;
}

_bool CLittle_Bug_State::Player_DetectedAndClose()
{
	return m_pMonster->m_bPlayerDetectedClose;
}

_bool CLittle_Bug_State::Player_CloseAndCanAttack()
{
	if (m_bCanAttack)
	{
		return m_pMonster->m_bPlayerDetectedClose;
	}
	else
		return false;
}

_bool CLittle_Bug_State::Player_NotDetected()
{
	return false;
}

_bool CLittle_Bug_State::Is_Damaged()
{
	if(m_bDamaged[HIT]== true)
	{
		return true;
	}
	else
	{
		memset(&m_bDamaged[HIT_END], false, sizeof(_bool) * HIT_END);
		return false;
	}
}

_bool CLittle_Bug_State::IS_Dead()
{
	return false;
}

CLittle_Bug_State* CLittle_Bug_State::Create(CLittle_Bug* pOwner, CState* pStateMachineCom, CModel* pModel,
                                           CTransform* pTransform, CNavigation* pNavigation)
{
	CLittle_Bug_State*		pInstance = new CLittle_Bug_State();
	if (FAILED(pInstance->Initialize(pOwner, pStateMachineCom, pModel, pTransform, pNavigation)))
	{
		MSG_BOX("Failed to Create : CLittle_Bug_State");
		Safe_Release(pInstance);

		return nullptr;
	}
	return pInstance;
}

void CLittle_Bug_State::Free()
{
}
