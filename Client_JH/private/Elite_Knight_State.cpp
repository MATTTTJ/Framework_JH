#include "stdafx.h"
#include "..\public\Elite_Knight_State.h"
#include "Bullet.h"
#include "Player.h"
#include "GameInstance.h"

CElite_Knight_State::CElite_Knight_State()
{
	m_bDamagedAnim = true;

	m_fAttackCoolTime = 10.f;
}

HRESULT CElite_Knight_State::Initialize(CElite_Knight* pOwner, CState* pStateMachineCom, CModel* pModel,
	CTransform* pTransform, CNavigation* pNavigation)
{
	m_pGameInstance = CGameInstance::GetInstance();
	m_pMonster = pOwner;
	m_pModelCom = pModel;
	m_pState = pStateMachineCom;
	m_pTransformCom = pTransform;
	m_pNavigationCom = pNavigation;


	memcpy(&m_bFirstState, m_pMonster->m_tMonsterOption.m_bFirstSpawnType, sizeof(_bool) * CElite_Knight::FIRSTSTATE_END);

	if (true == m_bFirstState[CElite_Knight::STATE_UPSPAWN])
	{
		// 생성 조건은 GAMEPLAY가 아닌 특정 콜라이더에 부딪혔을 때
		FAILED_CHECK_RETURN(SetUp_State_UpSpawn(), E_FAIL);
		FAILED_CHECK_RETURN(SetUp_State_Idle(), E_FAIL);
	}
	m_fWalkSoundTime = 0.6f;
	return S_OK;
}

void CElite_Knight_State::Tick(_double dTimeDelta)
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

		m_fCurAttackCoolTime += (_float)dTimeDelta;
		m_fCurDamagedAnimCoolTime += (_float)dTimeDelta;
	}

	if (m_bWalkSoundOnce == false && m_pState->Get_CurState() == L"STATE::RUN")
	{
		m_fCurWalkSoundTime += (_float)dTimeDelta;
	}

	if (m_fCurWalkSoundTime >= m_fWalkSoundTime)
	{
		m_fCurWalkSoundTime = 0.f;
		m_bWalkSoundOnce = true;
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

	// if(m_pGameInstance->Key_Down(DIK_F7))
	// {
	// 	CBullet::BULLETOPTION BulletDesc;
	// 	_float4 Position;
	// 	_matrix pivot = XMMatrixIdentity();
	// 	pivot = XMMatrixRotationZ(XMConvertToRadians(180.f));
	//
	// 	XMStoreFloat4(&Position, (m_pMonster->Get_BoneMatrix("Bip001 Prop1") * CGameUtils::Get_PlayerPivotMatrix() * m_pMonster->m_pTransformCom->Get_WorldMatrix()).r[3]);
	//
	// 	BulletDesc.BulletDesc.TransformDesc.vInitPos = _float3(Position.x, Position.y, Position.z);
	// 	_float4 PlayerPos = m_pPlayer->Get_TransformState(CTransform::STATE_TRANSLATION);
	// 	PlayerPos = _float4(PlayerPos.x, PlayerPos.y, PlayerPos.z, PlayerPos.w);
	// 	BulletDesc.BulletDesc.m_vBulletLook = XMVector4Normalize(PlayerPos - Position);
	// 	// BulletDesc.BulletDesc.m_vBulletLook = m_pMonster->m_pTransformCom->Get_State(CTransform::STATE_LOOK);
	//
	// 	BulletDesc.m_eOwner = CBullet::BULLETOWNERTYPE::OWNER_MONSTER;
	// 	BulletDesc.m_pOwner = m_pMonster;
	// 	CBullet*		pBullet = nullptr;
	// 	pBullet = (CBullet*)(m_pGameInstance->Clone_GameObjectReturnPtr(LEVEL_GAMEPLAY, L"Layer_Bullet", L"Prototype_GameObject_Normal_Elite_Knight_Blade", &BulletDesc));
	// 	NULL_CHECK(pBullet);
	//
	// 	m_bAttackOnce = true;
	// }

}

void CElite_Knight_State::Late_Tick(_double dTimeDelta)
{
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

void CElite_Knight_State::Reset_Damaged()
{
}

HRESULT CElite_Knight_State::SetUp_State_UpSpawn()
{
	NULL_CHECK_RETURN(m_pMonster->m_pState, E_FAIL);
	m_pMonster->m_pState->Set_Root(L"STATE::NO_DETECTED")
		.Add_State(L"STATE::NO_DETECTED")
		.Init_Start(this, &CElite_Knight_State::Start_UpSpawn)
		.Init_Tick(this, &CElite_Knight_State::Tick_UpSpawn)
		.Init_End(this, &CElite_Knight_State::End_UpSpawn)
		.Init_Changer(L"STATE::IN_COMBAT_IDLE", this, &CElite_Knight_State::Animation_Finish)
		.Finish_Setting();

	return S_OK;
}

HRESULT CElite_Knight_State::SetUp_State_Idle()
{
	NULL_CHECK_RETURN(m_pMonster->m_pState, E_FAIL);

	m_pMonster->m_pState->
		Add_State(L"STATE::IN_COMBAT_IDLE")
		.Init_Start(this, &CElite_Knight_State::Start_Idle)
		.Init_Tick(this, &CElite_Knight_State::Tick_Idle)
		.Init_End(this, &CElite_Knight_State::End_Common)
		.Init_Changer(L"STATE::RUN", this, &CElite_Knight_State::Player_DetectedAndFar)
		.Init_Changer(L"STATE::ATTACK_MELEE", this, &CElite_Knight_State::Player_CloseAndCanAttack)
		.Init_Changer(L"STATE::ATTACK_FAR", this, &CElite_Knight_State::Player_FarAndCanAttack)
		.Init_Changer(L"STATE::DAMAGED", this, &CElite_Knight_State::Is_Damaged)
		// .Init_Changer(L"STATE::TURNLEFT_180", this, &CElite_Knight_State::Turn_To_Player_Left_180)
		// .Init_Changer(L"STATE::TURNLEFT_90", this, &CElite_Knight_State::Turn_To_Player_Left_90)
		// .Init_Changer(L"STATE::TURNRIGHT_90", this, &CElite_Knight_State::Turn_To_Player_Right_90)
		// .Init_Changer(L"STATE::TURNRIGHT_180", this, &CElite_Knight_State::Turn_To_Player_Right_180)

		.Add_State(L"STATE::RUN")
		.Init_Start(this, &CElite_Knight_State::Start_Run)
		.Init_Tick(this, &CElite_Knight_State::Tick_Run)
		.Init_End(this, &CElite_Knight_State::End_Common)
		.Init_Changer(L"STATE::DAMAGED", this, &CElite_Knight_State::Is_Damaged)
		.Init_Changer(L"STATE::ATTACK_FAR", this, &CElite_Knight_State::Player_FarAndCanAttack)
		.Init_Changer(L"STATE::IN_COMBAT_IDLE", this, &CElite_Knight_State::Player_DetectedAndClose)

		.Add_State(L"STATE::ATTACK_MELEE")
		.Init_Start(this, &CElite_Knight_State::Start_Attack_IN_SHIELD)
		.Init_Tick(this, &CElite_Knight_State::Tick_Attack_IN_SHIELD)
		.Init_End(this, &CElite_Knight_State::End_Attack_IN_SHIELD)
		.Init_Changer(L"STATE::IN_COMBAT_IDLE", this, &CElite_Knight_State::Animation_Finish)

		.Add_State(L"STATE::ATTACK_FAR")
		.Init_Start(this, &CElite_Knight_State::Start_Attack_WIND)
		.Init_Tick(this, &CElite_Knight_State::Tick_Attack_WIND)
		.Init_End(this, &CElite_Knight_State::End_Attack_WIND)
		.Init_Changer(L"STATE::IN_COMBAT_IDLE", this, &CElite_Knight_State::Animation_Finish)

		.Add_State(L"STATE::DAMAGED")
		.Init_Start(this, &CElite_Knight_State::Start_Damaged)
		.Init_Tick(this, &CElite_Knight_State::Tick_Damaged)
		.Init_End(this, &CElite_Knight_State::End_Damaged)
		.Init_Changer(L"STATE::IN_COMBAT_IDLE", this, &CElite_Knight_State::Animation_Finish)

		// .Add_State(L"STATE::TURNLEFT_180")
		// .Init_Start(this, &CElite_Knight_State::Start_Turn)
		// .Init_Tick(this, &CElite_Knight_State::Tick_Turn)
		// .Init_End(this, &CElite_Knight_State::End_Turn)
		// .Init_Changer(L"STATE::IN_COMBAT_IDLE", this, &CElite_Knight_State::Animation_Finish)
		//
		// .Add_State(L"STATE::TURNLEFT_90")
		// .Init_Start(this, &CElite_Knight_State::Start_Turn)
		// .Init_Tick(this, &CElite_Knight_State::Tick_Turn)
		// .Init_End(this, &CElite_Knight_State::End_Turn)
		// .Init_Changer(L"STATE::IN_COMBAT_IDLE", this, &CElite_Knight_State::Animation_Finish)
		//
		// .Add_State(L"STATE::TURNRIGHT_90")
		// .Init_Start(this, &CElite_Knight_State::Start_Turn)
		// .Init_Tick(this, &CElite_Knight_State::Tick_Turn)
		// .Init_End(this, &CElite_Knight_State::End_Turn)
		// .Init_Changer(L"STATE::IN_COMBAT_IDLE", this, &CElite_Knight_State::Animation_Finish)
		//
		// .Add_State(L"STATE::TURNRIGHT_180")
		// .Init_Start(this, &CElite_Knight_State::Start_Turn)
		// .Init_Tick(this, &CElite_Knight_State::Tick_Turn)
		// .Init_End(this, &CElite_Knight_State::End_Turn)
		// .Init_Changer(L"STATE::IN_COMBAT_IDLE", this, &CElite_Knight_State::Animation_Finish)

		.Add_State(L"STATE::DEAD")
		.Init_Start(this, &CElite_Knight_State::Start_Death)
		.Init_Tick(this, &CElite_Knight_State::Tick_Death)
		.Init_End(this, &CElite_Knight_State::End_Death)

		.Finish_Setting();

	return S_OK;
}

HRESULT CElite_Knight_State::SetUp_State_Attack_WIND()
{
	return S_OK;
}

HRESULT CElite_Knight_State::SetUp_State_Attack_IN_SHIELD()
{
	return S_OK;
}

HRESULT CElite_Knight_State::SetUp_State_Attack_USE_SHIELD()
{
	return S_OK;
}

HRESULT CElite_Knight_State::SetUp_State_Hide()
{
	return S_OK;
}

HRESULT CElite_Knight_State::SetUp_State_Death()
{
	return S_OK;
}

void CElite_Knight_State::Start_UpSpawn(_double dTimeDelta)
{
	m_pModelCom->Set_LerpTime(0.2f);


	m_pModelCom->Set_CurAnimIndex(KNIGHT_SPAWN);
}

void CElite_Knight_State::Start_Idle(_double dTimeDelta)
{
	m_pModelCom->Set_LerpTime(0.3f);


	for (_uint i = 0; i < HIT_END; ++i)
	{
		memset(&m_bDamaged[i], false, sizeof(_bool)* HIT_END);
	}
	m_bDamagedAnim = false;
	m_pModelCom->Set_CurAnimIndex(KNIGHT_IN_COMBAT_IDLE);
}

void CElite_Knight_State::Start_Run(_double dTimeDelta)
{
	m_pModelCom->Set_LerpTime(0.2f);
	// Walk_F 랑 Run 섞어서 // 현재 스테이트가 뭔지에 따라 재생 애니메이션 바꾸기 
	if (m_pState->Get_CurState() == L"STATE::RUN")
	{
		m_pModelCom->Set_CurAnimIndex(KNIGHT_RUN);
	}
	else
		m_pModelCom->Set_CurAnimIndex(KNIGHT_WALK_F);
}

void CElite_Knight_State::Start_Attack_WIND(_double dTimeDelta)
{
	m_pModelCom->Set_LerpTime(0.2f);
	// 거리가 멀면 (Bow 활 쏘는 조건) 

	m_pModelCom->Set_CurAnimIndex(KNIGHT_ATTACK_WIND);
	m_bAttackSoundOnce = false;
}

void CElite_Knight_State::Start_Attack_IN_SHIELD(_double dTimeDelta)
{
	m_pModelCom->Set_LerpTime(0.2f);

	if ((rand() % 3) % 2 == 0)
	{
		m_pModelCom->Set_CurAnimIndex(KNIGHT_ATTACK_HIDE);
	}
	else
		m_pModelCom->Set_CurAnimIndex(KNIGHT_SHIELD_SHIELD);

	m_bAttackSoundOnce = false;
}

void CElite_Knight_State::Start_Attack_USE_SHIELD(_double dTimeDelta)
{
}

void CElite_Knight_State::Start_Hide(_double dTimeDelta)
{
}

void CElite_Knight_State::Start_Death(_double dTimeDelta)
{
	m_pModelCom->Set_LerpTime(0.2f);

	m_pModelCom->Set_CurAnimIndex(KNIGHT_STAND);
}

void CElite_Knight_State::Start_Damaged(_double dTimeDelta)
{
	m_pModelCom->Set_LerpTime(0.1f);
	m_pMonster->Set_HideColl(false);

	if (m_bDamaged[HITHEAD] == true)
		m_pModelCom->Set_CurAnimIndex(KNIGHT_HITHEAD);

	m_pGameInstance->Play_Sound(L"Knight_Hit.mp3", 0.5f, false, true);

	m_bDamaged[HIT] = false;
}

void CElite_Knight_State::Start_Detected(_double dTimeDelta)
{
}

void CElite_Knight_State::Start_Turn(_double dTimeDelta)
{
	// STATE에 따라 애니메이션 다르게 Rotation
	m_pModelCom->Set_LerpTime(0.2f);

	if(m_pState->Get_CurState() == L"STATE::TURNRIGHT_180")
	{
		m_pModelCom->Set_CurAnimIndex(KNIGHT_TURNRIGHT_180);

	}
	else if(m_pState->Get_CurState() == L"STATE::TURNRIGHT_90")
	{
		m_pModelCom->Set_CurAnimIndex(KNIGHT_TURNRIGHT_90);

	}
	else if (m_pState->Get_CurState() == L"STATE::TURNLEFT_90")
	{
		m_pModelCom->Set_CurAnimIndex(KNIGHT_TURNLEFT_90);

	}
	else if (m_pState->Get_CurState() == L"STATE::TURNLEFT_180")
	{
		m_pModelCom->Set_CurAnimIndex(KNIGHT_TURNLEFT_180);
	}
}

void CElite_Knight_State::Tick_UpSpawn(_double dTimeDelta)
{
}

void CElite_Knight_State::Tick_Idle(_double dTimeDelta)
{
	m_pTransformCom->LookAt_Monster(m_pPlayer->Get_TransformState(CTransform::STATE_TRANSLATION), dTimeDelta);
}

void CElite_Knight_State::Tick_Run(_double dTimeDelta)
{
	m_pTransformCom->LookAt_Move_Monster(m_pPlayer->Get_TransformState(CTransform::STATE_TRANSLATION), dTimeDelta, 3.f, m_pNavigationCom);

	if (m_bWalkSoundOnce == true)
	{
		CSound::SOUND_DESC SoundDesc;
		SoundDesc.fRange = 10.f;
		SoundDesc.bIs3D = true;
		SoundDesc.pStartTransform = m_pTransformCom;
		SoundDesc.pTargetTransform = m_pPlayer->Get_Transform();
		CGameInstance::GetInstance()->Set_SoundDesc(L"Knight_Walk.mp3", SoundDesc);

		m_pGameInstance->Play_Sound(L"Knight_Walk.mp3", 1.0f, false, true);
		m_bWalkSoundOnce = false;
	}
}

void CElite_Knight_State::Tick_Attack_WIND(_double dTimeDelta)
{
	// BOW 화살 쏘는거랑 똑같음 

	if (m_pModelCom->Get_AnimationProgress() > 0.2f && m_bAttackOnce == false)
	{
		CBullet::BULLETOPTION BulletDesc;
		_float4 Position;
		_matrix pivot = XMMatrixIdentity();
		pivot = XMMatrixRotationZ(XMConvertToRadians(180.f));

		XMStoreFloat4(&Position, (m_pMonster->Get_BoneMatrix("Bip001 Prop1") * CGameUtils::Get_PlayerPivotMatrix() * m_pMonster->m_pTransformCom->Get_WorldMatrix()).r[3]);

		BulletDesc.BulletDesc.TransformDesc.vInitPos = _float3(Position.x, Position.y, Position.z);
		_float4 PlayerPos = m_pPlayer->Get_TransformState(CTransform::STATE_TRANSLATION);
		PlayerPos = _float4(PlayerPos.x, PlayerPos.y, PlayerPos.z, PlayerPos.w);
		BulletDesc.BulletDesc.m_vBulletLook = XMVector4Normalize(PlayerPos - Position);
		// BulletDesc.BulletDesc.m_vBulletLook = m_pMonster->m_pTransformCom->Get_State(CTransform::STATE_LOOK);

		BulletDesc.m_eOwner = CBullet::BULLETOWNERTYPE::OWNER_MONSTER;
		BulletDesc.m_pOwner = m_pMonster;
		CBullet*		pBullet = nullptr;
		pBullet = (CBullet*)(m_pGameInstance->Clone_GameObjectReturnPtr(LEVEL_GAMEPLAY, L"Layer_Bullet", L"Prototype_GameObject_Normal_Elite_Knight_Blade", &BulletDesc));
		NULL_CHECK(pBullet);

		CSound::SOUND_DESC SoundDesc;
		SoundDesc.fRange = 15.f;
		SoundDesc.bIs3D = true;
		SoundDesc.pStartTransform = m_pTransformCom;
		SoundDesc.pTargetTransform = m_pPlayer->Get_Transform();
		CGameInstance::GetInstance()->Set_SoundDesc(L"Knight_Wind.mp3", SoundDesc);

		m_pGameInstance->Play_Sound(L"Knight_Wind.mp3", 1.f, false, true);

		m_bAttackOnce = true;
	}


}

void CElite_Knight_State::Tick_Attack_IN_SHIELD(_double dTimeDelta)
{
	if (m_pMonster->m_pPlayer->Collision_Detected(m_pMonster->m_pColliderCom[CMonster::COLLTYPE_ATTPOS]))
	{
		m_pMonster->m_pPlayer->Collision_Event(m_pMonster);
		m_bAttackOnce = true;
	}

	if (m_pModelCom->Get_AnimationProgress() > 0.3f && m_bAttackSoundOnce == false)
	{
		m_pGameInstance->Play_Sound(L"Knight_HideAttack.mp3", 0.8f, false, false);
		m_bAttackSoundOnce = true;
	}
}

void CElite_Knight_State::Tick_Attack_USE_SHIELD(_double dTimeDelta)
{
	// if (m_pMonster->m_pPlayer->Collision_Detected(m_pMonster->m_pColliderCom[CMonster::COLLTYPE_ATTPOS]))
	// {
	// 	m_pMonster->m_pPlayer->Collision_Event(m_pMonster);
	// 	m_bAttackOnce = true;
	// }
	//
	// if (m_pModelCom->Get_AnimationProgress() > 0.3f && m_bAttackSoundOnce == false)
	// {
	// 	m_pGameInstance->Play_Sound(L"Knight_ShieldAttack.mp3", 0.8f, false, false);
	// 	m_bAttackSoundOnce = true;
	// }
}

void CElite_Knight_State::Tick_Hide(_double dTimeDelta)
{
}

void CElite_Knight_State::Tick_Death(_double dTimeDelta)
{
}

void CElite_Knight_State::Tick_Damaged(_double dTimeDelta)
{
	m_pMonster->Set_HideColl(false);
}

void CElite_Knight_State::Tick_Detected(_double dTimeDelta)
{
}

void CElite_Knight_State::Tick_Turn(_double dTimeDelta)
{
}

void CElite_Knight_State::End_Common(_double TimeDelta)
{
}

void CElite_Knight_State::End_UpSpawn(_double dTimeDelta)
{
	for (_uint i = 0; i < HIT_END; ++i)
		memset(&m_bDamaged[i], false, sizeof(_bool)* HIT_END);

	m_fCurHideCoolTime = 0.f;
	m_pMonster->Set_HideColl(false);
}

void CElite_Knight_State::End_Idle(_double dTimeDelta)
{
}

void CElite_Knight_State::End_Run(_double dTimeDelta)
{
}

void CElite_Knight_State::End_Attack_WIND(_double dTimeDelta)
{
	m_bCanAttack = false;
	m_bAttackOnce = false;
}

void CElite_Knight_State::End_Attack_IN_SHIELD(_double dTimeDelta)
{
	m_bCanAttack = false;
	m_bAttackOnce = false;
}

void CElite_Knight_State::End_Attack_USE_SHIELD(_double dTimeDelta)
{
	m_bCanAttack = false;
	m_bAttackOnce = false;
}

void CElite_Knight_State::End_Hide(_double dTimeDelta)
{
}

void CElite_Knight_State::End_Death(_double dTimeDelta)
{
}

void CElite_Knight_State::End_Damaged(_double dTimeDelta)
{
	for (_uint i = 0; i < HIT_END; ++i)
	{
		memset(&m_bDamaged[i], false, sizeof(_bool)* HIT_END);
	}

	m_bDamagedAnim = false;
}

void CElite_Knight_State::End_Detected(_double dTimeDelta)
{
}

void CElite_Knight_State::End_Turn(_double dTimeDelta)
{
}

_bool CElite_Knight_State::Animation_Finish()
{
	return m_pModelCom->Get_AnimationFinish();
}

_bool CElite_Knight_State::Player_Detected()
{
	return m_pMonster->m_bPlayerDetected;
}

_bool CElite_Knight_State::Player_DetectedAndFar()
{
	return !m_pMonster->m_bPlayerDetectedClose;
}

_bool CElite_Knight_State::Player_DetectedAndClose()
{
	if (m_bCanAttack == false)
		return m_pMonster->m_bPlayerDetectedClose;
	else
		false;

	return false;
}

_bool CElite_Knight_State::Player_CloseAndCanAttack()
{
	if (m_bCanAttack)
	{
		return m_pMonster->m_bPlayerDetectedClose;
	}
	else
		return false;
}

_bool CElite_Knight_State::Player_FarAndCanAttack()
{
	if (m_bCanAttack)
	{
		return !m_pMonster->m_bPlayerDetectedClose;
	}
	else
		return false;
}

_bool CElite_Knight_State::Turn_To_Player_Right_180()
{
	_vector vLook = (m_pMonster->m_pTransformCom->Get_State(CTransform::STATE_LOOK) * 2.f);
	_vector vMonsterPos = m_pMonster->Get_TransformState(CTransform::STATE_TRANSLATION);
	_vector NormMonsterLook = XMVector3Normalize(vLook - vMonsterPos);


	_vector vTargetPos = m_pPlayer->Get_TransformState(CTransform::STATE_TRANSLATION);
	_vector NormMonsterToPlayer = XMVector3Normalize(vTargetPos - vMonsterPos);

	_float fAngle = XMVectorGetX(XMVector3Dot(NormMonsterLook, NormMonsterToPlayer));

	if (fAngle > 0.5 && fAngle < 1.f)
	{
		return true;
	}
	else
	{
		return false;
	}
}

_bool CElite_Knight_State::Turn_To_Player_Right_90()
{
	_vector vLook = (m_pMonster->m_pTransformCom->Get_State(CTransform::STATE_LOOK) * 2.f);
	_vector vMonsterPos = m_pMonster->Get_TransformState(CTransform::STATE_TRANSLATION);
	_vector NormMonsterLook = XMVector3Normalize(vLook - vMonsterPos);


	_vector vTargetPos = m_pPlayer->Get_TransformState(CTransform::STATE_TRANSLATION);
	_vector NormMonsterToPlayer = XMVector3Normalize(vTargetPos - vMonsterPos);

	_float fAngle = XMVectorGetX(XMVector3Dot(NormMonsterLook, NormMonsterToPlayer));

	if (fAngle > -0.5f && fAngle < 0.f)
	{
		return true;
	}
	else
	{
		return false;
	}
}

_bool CElite_Knight_State::Turn_To_Player_Left_180()
{
	_vector vLook = (m_pMonster->m_pTransformCom->Get_State(CTransform::STATE_LOOK) * 2.f);
	_vector vMonsterPos = m_pMonster->Get_TransformState(CTransform::STATE_TRANSLATION);
	_vector NormMonsterLook = XMVector3Normalize(vLook - vMonsterPos);


	_vector vTargetPos = m_pPlayer->Get_TransformState(CTransform::STATE_TRANSLATION);
	_vector NormMonsterToPlayer = XMVector3Normalize(vTargetPos - vMonsterPos);

	_float fAngle = XMVectorGetX(XMVector3Dot(NormMonsterLook, NormMonsterToPlayer));

	if (fAngle > 0.5 && fAngle < 1.f)
	{
		return true;
	}
	else
	{
		return false;
	}


	
}

_bool CElite_Knight_State::Turn_To_Player_Left_90()
{

	_vector vLook = (m_pMonster->m_pTransformCom->Get_State(CTransform::STATE_LOOK) * 2.f);
	_vector vMonsterPos = m_pMonster->Get_TransformState(CTransform::STATE_TRANSLATION);
	_vector NormMonsterLook = XMVector3Normalize(vLook - vMonsterPos);


	_vector vTargetPos = m_pPlayer->Get_TransformState(CTransform::STATE_TRANSLATION);
	_vector NormMonsterToPlayer = XMVector3Normalize(vTargetPos - vMonsterPos);

	_float fAngle = XMVectorGetX(XMVector3Dot(NormMonsterLook, NormMonsterToPlayer));

	if (fAngle > -0.5f && fAngle < 0.f)
	{
		return true;
	}
	else
	{
		return false;
	}


}

_bool CElite_Knight_State::Bullet_Hide_Collision()
{
	return false;
}

_bool CElite_Knight_State::Player_NotDetected()
{
	return false;
}

_bool CElite_Knight_State::Is_Damaged()
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

_bool CElite_Knight_State::IS_Dead()
{
	return false;
}

CElite_Knight_State* CElite_Knight_State::Create(CElite_Knight* pOwner, CState* pStateMachineCom, CModel* pModel,
	CTransform* pTransform, CNavigation* pNavigation)
{
	CElite_Knight_State*		pInstance = new CElite_Knight_State();
	if (FAILED(pInstance->Initialize(pOwner, pStateMachineCom, pModel, pTransform, pNavigation)))
	{
		MSG_BOX("Failed to Create : CElite_Knight_State");
		Safe_Release(pInstance);

		return nullptr;
	}

	return pInstance;
}

void CElite_Knight_State::Free()
{
}
