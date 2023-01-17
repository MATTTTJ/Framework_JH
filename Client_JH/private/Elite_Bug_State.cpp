#include "stdafx.h"
#include "..\public\Elite_Bug_State.h"
#include "Bullet.h"
#include "Player.h"
#include "GameInstance.h"


CElite_Bug_State::CElite_Bug_State()
{
	m_bDamagedAnim = true;
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

void CElite_Bug_State::Late_Tick(_double dTimeDelta)
{
	_float4	fDir;
	if (CGameUtils::CollisionSphereSphere(m_pMonster->m_pPlayer->m_pColliderCom[CPlayer::COLLIDER_SPHERE], m_pMonster->m_pColliderCom[CMonster::COLLTYPE_HITBODY], fDir))
	{
		_vector vPos = m_pTransformCom->Get_State(CTransform::STATE_TRANSLATION);
		fDir.y = 0.f;
		_vector	vMovePos = XMVectorAdd(vPos, fDir);

		_float4 vBlockedLine = { 0.f, 0.f, 0.f, 0.f };
		_float4 vBlockedLineNormal = { 0.f ,0.f, 0.f, 0.f };

		if (true == m_pNavigationCom->IsMove_OnNavigation(vPos, vBlockedLine, vBlockedLineNormal))
			m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, vMovePos);
		else
		{
			_vector vInDir = vMovePos - vPos;
			_vector vOutDir = vPos - vMovePos;
			_float	fLength = XMVectorGetX(XMVector3Dot(vOutDir, vBlockedLineNormal));

			_vector vSlidingDir = vInDir + XMLoadFloat4(&vBlockedLineNormal) * fLength;

			vMovePos = vPos + vSlidingDir;

			if (m_pNavigationCom->IsMove_OnNavigation(vMovePos, vBlockedLine, vBlockedLineNormal))
			{
				m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, vMovePos);
			}
		}
	}

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

	m_pMonster->m_pState->
		Add_State(L"STATE::IN_COMBAT_IDLE")
		.Init_Start(this, &CElite_Bug_State::Start_Idle)
		.Init_Tick(this, &CElite_Bug_State::Tick_Idle)
		.Init_End(this, &CElite_Bug_State::End_Common)
		.Init_Changer(L"STATE::RUN", this, &CElite_Bug_State::Player_DetectedAndFar)
		.Init_Changer(L"STATE::ATTACK_A", this, &CElite_Bug_State::Player_CloseAndCanAttack)
		.Init_Changer(L"STATE::DAMAGED", this, &CElite_Bug_State::Is_Damaged)

		.Add_State(L"STATE::RUN")
		.Init_Start(this, &CElite_Bug_State::Start_Run)
		.Init_Tick(this, &CElite_Bug_State::Tick_Run)
		.Init_End(this, &CElite_Bug_State::End_Common)
		.Init_Changer(L"STATE::DAMAGED", this, &CElite_Bug_State::Is_Damaged)
		.Init_Changer(L"STATE::IN_COMBAT_IDLE", this, &CElite_Bug_State::Player_DetectedAndClose)

		.Add_State(L"STATE::ATTACK_A")
		.Init_Start(this, &CElite_Bug_State::Start_Attack_A)
		.Init_Tick(this, &CElite_Bug_State::Tick_Attack_A)
		.Init_End(this, &CElite_Bug_State::End_Attack_A)
		.Init_Changer(L"STATE::DAMAGED", this, &CElite_Bug_State::Is_Damaged)
		.Init_Changer(L"STATE::IN_COMBAT_IDLE", this, &CElite_Bug_State::Animation_Finish)

		.Add_State(L"STATE::DAMAGED")
		.Init_Start(this, &CElite_Bug_State::Start_Damaged)
		.Init_Tick(this, &CElite_Bug_State::Tick_Damaged)
		.Init_End(this, &CElite_Bug_State::End_Damaged)
		.Init_Changer(L"STATE::IN_COMBAT_IDLE", this, &CElite_Bug_State::Animation_Finish)

		
		.Finish_Setting();

	return S_OK;
}

HRESULT CElite_Bug_State::SetUp_State_No_Detected()
{
	NULL_CHECK_RETURN(m_pMonster->m_pState, E_FAIL);
	m_pMonster->m_pState->Set_Root(L"STATE::NO_DETECTED")
		.Add_State(L"STATE::NO_DETECTED")
		.Init_Start(this, &CElite_Bug_State::Start_No_Detected)
		.Init_Tick(this, &CElite_Bug_State::Tick_No_Detected)
		.Init_End(this, &CElite_Bug_State::End_No_Detected)
		.Init_Changer(L"STATE::IN_COMBAT_IDLE", this, &CElite_Bug_State::Player_Detected)
		.Init_Changer(L"STATE::IN_COMBAT_IDLE", this, &CElite_Bug_State::Is_Damaged)

		.Finish_Setting();

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

	m_pModelCom->Set_CurAnimIndex(SPEAR_INCOMBAT_IDLE);
	m_bAttackOnce = false;
}

void CElite_Bug_State::Start_Run(_double dTimeDelta)
{
	m_pModelCom->Set_LerpTime(0.2f);

	if (m_bFirstState[CElite_Bug::STATE_ALREADYSPAWN] == true)
	{
		m_pModelCom->Set_CurAnimIndex(SPEAR_RUN_B);
	}
	else
		m_pModelCom->Set_CurAnimIndex(SPEAR_RUN_A);
}

void CElite_Bug_State::Start_No_Detected(_double dTimeDelta)
{
	m_pModelCom->Set_LerpTime(0.2f);

	m_pModelCom->Set_CurAnimIndex(SPEAR_NODETECTED);
}

void CElite_Bug_State::Start_Death(_double dTimeDelta)
{
}

void CElite_Bug_State::Start_Prepare_Rush(_double dTimeDelta)
{
}

void CElite_Bug_State::Start_Ready_Rush(_double dTimeDelta)
{
}

void CElite_Bug_State::Start_Starting_Rush(_double dTimeDelta)
{
}

void CElite_Bug_State::Start_Rush_Loop(_double dTimeDelta)
{
}

void CElite_Bug_State::Start_Ending_Rush(_double dTimeDelta)
{
}

void CElite_Bug_State::Start_Spawn_Junior(_double dTimeDelta)
{
}



void CElite_Bug_State::Tick_Prepare_Rush(_double dTimeDelta)
{
}

void CElite_Bug_State::Tick_Ready_Rush(_double dTimeDelta)
{
}

void CElite_Bug_State::Tick_Starting_Rush(_double dTimeDelta)
{
}

void CElite_Bug_State::Tick_Rush_Loop(_double dTimeDelta)
{
}

void CElite_Bug_State::Tick_Ending_Rush(_double dTimeDelta)
{
}
void CElite_Bug_State::Tick_Spawn_Junior(_double dTimeDelta)
{
}

void CElite_Bug_State::Tick_Idle(_double dTimeDelta)
{
}

void CElite_Bug_State::Tick_Run(_double dTimeDelta)
{
	m_pTransformCom->LookAt_Move_Monster(m_pPlayer->Get_TransformState(CTransform::STATE_TRANSLATION), dTimeDelta, 2.35f, m_pNavigationCom);
}

void CElite_Bug_State::Tick_No_Detected(_double dTimeDelta)
{
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

void CElite_Bug_State::End_Prepare_Rush(_double dTimeDelta)
{
}

void CElite_Bug_State::End_Ready_Rush(_double dTimeDelta)
{
}

void CElite_Bug_State::End_Starting_Rush(_double dTimeDelta)
{
}

void CElite_Bug_State::End_Rush_Loop(_double dTimeDelta)
{
}

void CElite_Bug_State::End_Ending_Rush(_double dTimeDelta)
{
}

void CElite_Bug_State::End_Spawn_Junior(_double dTimeDelta)
{
}


void CElite_Bug_State::End_No_Detected(_double dTimeDelta)
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

_bool CElite_Bug_State::IS_Dead()
{
	return false;
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
