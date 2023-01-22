#include "stdafx.h"
#include "..\public\Boss_Golem_State.h"
#include "Bullet.h"
#include "FadeInOut.h"
#include "Player.h"
#include "GameInstance.h"
#include "Dynamic_Camera.h"

CBoss_Golem_State::CBoss_Golem_State()
{
	m_bDamagedAnim = true;

	m_fAttackCoolTime = 10.f;
}

HRESULT CBoss_Golem_State::Initialize(CNormal_Boss* pOwner, CState* pStateMachineCom, CModel* pModel,
	CTransform* pTransform, CNavigation* pNavigation)
{
	m_pGameInstance = CGameInstance::GetInstance();
	m_pMonster = pOwner;
	m_pModelCom = pModel;
	m_pState = pStateMachineCom;
	m_pTransformCom = pTransform;
	m_pNavigationCom = pNavigation;

	m_pDynamic_Camera = static_cast<CDynamic_Camera*>(CGameInstance::GetInstance()->Get_CloneObjectList(LEVEL_GAMEPLAY, L"Layer_ZCamera")->front());
	memcpy(&m_bFirstState, m_pMonster->m_tMonsterOption.m_bFirstSpawnType, sizeof(_bool) * CNormal_Boss::FIRSTSTATE_END);

	if (true == m_bFirstState[CNormal_Boss::STATE_NODETECTED])
	{
		// 생성 조건은 GAMEPLAY가 아닌 특정 콜라이더에 부딪혔을 때
		FAILED_CHECK_RETURN(SetUp_State_No_Detected(), E_FAIL);
		FAILED_CHECK_RETURN(SetUp_State_Idle(), E_FAIL);
	}

	return S_OK;
}

void CBoss_Golem_State::Tick(_double dTimeDelta)
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

	if(m_pGameInstance->Key_Down(DIK_F7))
	{
		m_pMonster->m_bPlayerDetected = true;
		m_pMonster->Set_PlayAnimation(true);
		m_pFadeInOut = dynamic_cast<CFadeInOut*>(m_pGameInstance->Clone_GameObject(L"Prototype_GameObject_Fade_In_Out"));
		NULL_CHECK_RETURN(m_pFadeInOut, );
		// CBullet::BULLETOPTION BulletDesc;
		// _float4 Position;
		// _matrix pivot = XMMatrixIdentity();
		// pivot = XMMatrixRotationZ(XMConvertToRadians(180.f));
		//
		// XMStoreFloat4(&Position, (m_pMonster->Get_BoneMatrix("Bip001 Prop1") * CGameUtils::Get_PlayerPivotMatrix() * m_pMonster->m_pTransformCom->Get_WorldMatrix()).r[3]);
		//
		// BulletDesc.BulletDesc.TransformDesc.vInitPos = _float3(Position.x, Position.y, Position.z);
		// _float4 PlayerPos = m_pPlayer->Get_TransformState(CTransform::STATE_TRANSLATION);
		// PlayerPos = _float4(PlayerPos.x, PlayerPos.y, PlayerPos.z, PlayerPos.w);
		// BulletDesc.BulletDesc.m_vBulletLook = XMVector4Normalize(PlayerPos - Position);
		// // BulletDesc.BulletDesc.m_vBulletLook = m_pMonster->m_pTransformCom->Get_State(CTransform::STATE_LOOK);
		//
		// BulletDesc.m_eOwner = CBullet::BULLETOWNERTYPE::OWNER_MONSTER;
		// BulletDesc.m_pOwner = m_pMonster;
		// CBullet*		pBullet = nullptr;
		// pBullet = (CBullet*)(m_pGameInstance->Clone_GameObjectReturnPtr(LEVEL_GAMEPLAY, L"Layer_Bullet", L"Prototype_GameObject_Normal_Elite_Knight_Blade", &BulletDesc));
		// NULL_CHECK(pBullet);
		//
		// m_bAttackOnce = true;
	}

	if (m_pGameInstance->Key_Down(DIK_F3))
	{
		_float scale[3], Rot[3], Pos[3];
		Pos[0] = {111.4f}; Pos[1] = { 11.47f }; Pos[2] = { 129.6f };
		Rot[0] = { -12.84f }; Rot[1] = { -34.574f }; Rot[2] = { 0.0f };
		scale[0] = { 1.f }; scale[1] = { 1.f }; scale[2] = { 1.f };
		_matrix camWorld;
		ImGuizmo::RecomposeMatrixFromComponents(Pos, Rot, scale, (_float*)&camWorld);
		m_pDynamic_Camera->Set_Boss_IntroCam(camWorld);
	}
	// if (m_pGameInstance->Key_Down(DIK_F4))
	// {
	// 	m_pFadeInOut->Fade_Out(true);
	// }

	if(m_pFadeInOut != nullptr)
	{
		m_pFadeInOut->Tick(dTimeDelta);
	}

}

void CBoss_Golem_State::Late_Tick(_double dTimeDelta)
{
	if (m_pPlayer != nullptr)
	{
		// if (m_pPlayer->Collision_Detected(m_pMonster->m_pColliderCom[CMonster::COLLTYPE_DETECTED]))
		// {
		// 	m_pMonster->m_bPlayerDetected = true;
		// }
		// else
		// 	m_pMonster->m_bPlayerDetected = false;
		//
		// if (m_pPlayer->Collision_Detected(m_pMonster->m_pColliderCom[CMonster::COLLTYPE_ATTRANGE]))
		// {
		// 	m_pMonster->m_bPlayerDetectedClose = true;
		// }
		// else
		// {
		// 	m_pMonster->m_bPlayerDetectedClose = false;
		// }
	}

	if (m_pFadeInOut != nullptr)
	{
		m_pFadeInOut->Late_Tick(dTimeDelta);
	}
}

void CBoss_Golem_State::Reset_Damaged()
{
}

HRESULT CBoss_Golem_State::SetUp_State_No_Detected()
{
	NULL_CHECK_RETURN(m_pMonster->m_pState, E_FAIL);
	m_pMonster->m_pState->Set_Root(L"STATE::NO_DETECTED")
		.Add_State(L"STATE::NO_DETECTED")
		.Init_Start(this, &CBoss_Golem_State::Start_Intro0)
		.Init_Tick(this, &CBoss_Golem_State::Tick_Intro0)
		.Init_End(this, &CBoss_Golem_State::End_Intro0)
		.Init_Changer(L"STATE::INTRO1", this, &CBoss_Golem_State::Player_Detected)

		.Add_State(L"STATE::INTRO1")
		.Init_Start(this, &CBoss_Golem_State::Start_Intro1)
		.Init_Tick(this, &CBoss_Golem_State::Tick_Intro1)
		.Init_End(this, &CBoss_Golem_State::End_Intro1)
		.Init_Changer(L"STATE::INTRO2", this, &CBoss_Golem_State::Animation_Finish)
		//
		.Add_State(L"STATE::INTRO2")
		.Init_Start(this, &CBoss_Golem_State::Start_Intro2)
		.Init_Tick(this, &CBoss_Golem_State::Tick_Intro2)
		.Init_End(this, &CBoss_Golem_State::End_Intro2)
		.Init_Changer(L"STATE::IN_COMBAT_IDLE", this, &CBoss_Golem_State::Animation_Finish)
	// 인트로 끝날 때 돌기둥 생성

		.Finish_Setting();

	return S_OK;
}

HRESULT CBoss_Golem_State::SetUp_State_Idle()
{
	NULL_CHECK_RETURN(m_pMonster->m_pState, E_FAIL);
	
	m_pMonster->m_pState->
		Add_State(L"STATE::IN_COMBAT_IDLE")
		.Init_Start(this, &CBoss_Golem_State::Start_Idle)
		.Init_Tick(this, &CBoss_Golem_State::Tick_Idle)
		.Init_End(this, &CBoss_Golem_State::End_Common)
		.Init_Changer(L"STATE::MELEE_AND_READY_SPAWN", this, &CBoss_Golem_State::Is_Can_Spawn_Pillars)
		.Init_Changer(L"STATE::READY_LAZER", this, &CBoss_Golem_State::Is_Can_Fire_Lazer)
		.Init_Changer(L"STATE::READY_ARMFIRE", this, &CBoss_Golem_State::Is_Can_Fire_Arm)
		.Init_Changer(L"STATE::SPAWN_MAGICSTONE", this, &CBoss_Golem_State::Is_Can_Spawn_MagicStone)
		.Init_Changer(L"STATE::SPAWN_MONSTER", this, &CBoss_Golem_State::Is_Can_Spawn_Monster)
		.Init_Changer(L"STATE::DEAD", this, &CBoss_Golem_State::IS_Dead)

	// 땅 내리찍으며 돌기둥 파괴 후 
		.Add_State(L"STATE::MELEE_AND_READY_SPAWN")
		.Init_Start(this, &CBoss_Golem_State::Start_Melee_And_Ready_SpawnPillars)
		.Init_Tick(this, &CBoss_Golem_State::Tick_Melee_And_Ready_SpawnPillars)
		.Init_End(this, &CBoss_Golem_State::End_Melee_And_Ready_SpawnPillars)
		.Init_Changer(L"STATE::START_SPAWNPILLARS", this, &CBoss_Golem_State::Is_Pillars_Empty)
	// 돌기둥 전부 파괴되면 스테이트 넘어옴
		.Add_State(L"STATE::START_SPAWNPILLARS")
		.Init_Start(this, &CBoss_Golem_State::Start_Start_SpawnPillars)
		.Init_Tick(this, &CBoss_Golem_State::Tick_Start_SpawnPillars)
		.Init_End(this, &CBoss_Golem_State::End_Start_SpawnPillars)
		.Init_Changer(L"STATE::IN_COMBAT_IDLE", this, &CBoss_Golem_State::Animation_Finish)
	// ~돌기둥 생성

	// 레이저 발사준비
		.Add_State(L"STATE::READY_LAZER")
		.Init_Start(this, &CBoss_Golem_State::Start_Ready_Lazer)
		.Init_Tick(this, &CBoss_Golem_State::Tick_Ready_Lazer)
		.Init_End(this, &CBoss_Golem_State::End_Ready_Lazer)
		.Init_Changer(L"STATE::FIRE_LAZER", this, &CBoss_Golem_State::Animation_Finish)

		.Add_State(L"STATE::FIRE_LAZER")
		.Init_Start(this, &CBoss_Golem_State::Start_Fire_Lazer)
		.Init_Tick(this, &CBoss_Golem_State::Tick_Fire_Lazer)
		.Init_End(this, &CBoss_Golem_State::End_Fire_Lazer)
		.Init_Changer(L"STATE::IN_COMBAT_IDLE", this, &CBoss_Golem_State::Animation_Finish)
	// ~레이저 발사준비

	// 로켓 발사준비
		.Add_State(L"STATE::READY_ARMFIRE")
		.Init_Start(this, &CBoss_Golem_State::Start_Ready_Arm_Fire)
		.Init_Tick(this, &CBoss_Golem_State::Tick_Ready_Arm_Fire)
		.Init_End(this, &CBoss_Golem_State::End_Ready_Arm_Fire)
		.Init_Changer(L"STATE::START_FIRE_LEFTARM", this, &CBoss_Golem_State::Animation_Finish)

		.Add_State(L"STATE::FIRE_LEFTARM")
		.Init_Start(this, &CBoss_Golem_State::Start_Fire_Arm_LeftArm)
		.Init_Tick(this, &CBoss_Golem_State::Tick_Fire_Arm_LeftArm)
		.Init_End(this, &CBoss_Golem_State::End_Fire_Arm_LeftArm)
		.Init_Changer(L"STATE::FIRE_RIGHTARM", this, &CBoss_Golem_State::Animation_Finish)

		.Add_State(L"STATE::FIRE_RIGHTARM")
		.Init_Start(this, &CBoss_Golem_State::Start_Fire_Arm_RightArm)
		.Init_Tick(this, &CBoss_Golem_State::Tick_Fire_Arm_RightArm)
		.Init_End(this, &CBoss_Golem_State::End_Fire_Arm_RightArm)
		.Init_Changer(L"STATE::END_FIREARM", this, &CBoss_Golem_State::Animation_Finish)

		.Add_State(L"STATE::END_FIREARM")
		.Init_Start(this, &CBoss_Golem_State::Start_End_Arm_Fire)
		.Init_Tick(this, &CBoss_Golem_State::Tick_End_Arm_Fire)
		.Init_End(this, &CBoss_Golem_State::End_End_Arm_Fire)
		.Init_Changer(L"STATE::IN_COMBAT_IDLE", this, &CBoss_Golem_State::Animation_Finish)
	// ~로켓 발사준비


	// 마법석 준비
		.Add_State(L"STATE::READY_MAGICSTONE")
		.Init_Start(this, &CBoss_Golem_State::Start_Ready_MagicStone)
		.Init_Tick(this, &CBoss_Golem_State::Tick_Ready_MagicStone)
		.Init_End(this, &CBoss_Golem_State::End_Ready_MagicStone)
		.Init_Changer(L"STATE::START_MAGICSTONE", this, &CBoss_Golem_State::Animation_Finish)


		.Add_State(L"STATE::START_MAGICSTONE")
		.Init_Start(this, &CBoss_Golem_State::Start_End_MagicStone)
		.Init_Tick(this, &CBoss_Golem_State::Tick_End_MagicStone)
		.Init_End(this, &CBoss_Golem_State::End_End_MagicStone)
		.Init_Changer(L"STATE::IN_COMBAT_IDLE", this, &CBoss_Golem_State::Animation_Finish)
	// ~마법석 준비





		.Add_State(L"STATE::DEAD")
		.Init_Start(this, &CBoss_Golem_State::Start_Dead)
		.Init_Tick(this, &CBoss_Golem_State::Tick_Dead)
		.Init_End(this, &CBoss_Golem_State::End_Dead)
		.Init_Changer(L"STATE::DEAD", this, &CBoss_Golem_State::Animation_Finish)


		.Finish_Setting();

	return S_OK;

}

void CBoss_Golem_State::Start_Intro0(_double dTimeDelta)
{
	m_pModelCom->Set_CurAnimIndex(GOLEM_INTRO0);
	m_pModelCom->Reset_Animation();
}

void CBoss_Golem_State::Start_Intro1(_double dTimeDelta)
{
	m_pModelCom->Set_CurAnimIndex(GOLEM_INTRO1);


	// _bool&		DynamicCamera = dynamic_cast<CCamera*>(CGameInstance::GetInstance()->Get_CloneObjectList(LEVEL_GAMEPLAY, L"Layer_ZCamera")->front())->Get_RenderState();
	// _bool&		StaticCamera = dynamic_cast<CCamera*>(CGameInstance::GetInstance()->Get_CloneObjectList(LEVEL_GAMEPLAY, L"Layer_ZCamera")->back())->Get_RenderState();
	//
	//
	// DynamicCamera = !DynamicCamera;
	// StaticCamera = !StaticCamera;
	//
	//
	// _float scale[3], Rot[3], Pos[3];
	// Pos[0] = { 111.4f }; Pos[1] = { 11.47f }; Pos[2] = { 129.6f };
	// Rot[0] = { -12.84f }; Rot[1] = { -34.574f }; Rot[2] = { 0.0f };
	// scale[0] = { 1.f }; scale[1] = { 1.f }; scale[2] = { 1.f };
	// _matrix camWorld;
	// ImGuizmo::RecomposeMatrixFromComponents(Pos, Rot, scale, (_float*)&camWorld);
	// m_pDynamic_Camera->Set_Boss_IntroCam(camWorld);
}

void CBoss_Golem_State::Start_Intro2(_double dTimeDelta)
{
	_float scale[3]{1.f, 1.f, 1.f}, Rot[3]{ -6.549f,3.192f,0.0f }, Pos[3]{ 116.f ,10.77f,124.94f };
	_matrix camWorld;
	ImGuizmo::RecomposeMatrixFromComponents(Pos, Rot, scale, (_float*)&camWorld);
	m_pDynamic_Camera->Set_Boss_IntroCam(camWorld);


	m_pModelCom->Set_LerpTime(0.4f);

	m_pModelCom->Set_CurAnimIndex(GOLEM_INTRO2);
	m_pMonster->Set_PlayAnimation(true);
}

void CBoss_Golem_State::Start_Idle(_double dTimeDelta)
{
	m_pModelCom->Set_LerpTime(0.6f);

	m_pModelCom->Set_CurAnimIndex(GOLEM_INCOMBAT_IDLE);
	m_pModelCom->Reset_Animation();
}

void CBoss_Golem_State::Start_Ready_Arm_Fire(_double dTimeDelta)
{
	m_pModelCom->Set_LerpTime(0.2f);


	m_pModelCom->Set_CurAnimIndex(GOLEM_READY_ARMFIRE);
}

void CBoss_Golem_State::Start_Fire_Arm_RightArm(_double dTimeDelta)
{
	m_pModelCom->Set_LerpTime(0.2f);


	m_pModelCom->Set_CurAnimIndex(GOLEM_FIRE_RIGHTARM);
}

void CBoss_Golem_State::Start_Fire_Arm_LeftArm(_double dTimeDelta)
{
	m_pModelCom->Set_LerpTime(0.2f);


	m_pModelCom->Set_CurAnimIndex(GOLEM_FIRE_LEFTARM);
}

void CBoss_Golem_State::Start_End_Arm_Fire(_double dTimeDelta)
{
	m_pModelCom->Set_LerpTime(0.2f);


	m_pModelCom->Set_CurAnimIndex(GOLEM_END_FIREARM);
}

void CBoss_Golem_State::Start_Ready_Lazer(_double dTimeDelta)
{
	m_pModelCom->Set_LerpTime(0.2f);

	m_pModelCom->Set_CurAnimIndex(GOLEM_READY_LAZER);
}

void CBoss_Golem_State::Start_Fire_Lazer(_double dTimeDelta)
{
	m_pModelCom->Set_LerpTime(0.2f);

	m_pModelCom->Set_CurAnimIndex(GOLEM_FIRE_LAZER);
}

void CBoss_Golem_State::Start_Ready_MagicStone(_double dTimeDelta)
{
	m_pModelCom->Set_LerpTime(0.2f);

	m_pModelCom->Set_CurAnimIndex(GOLEM_START_MAGICSTONE);
}

void CBoss_Golem_State::Start_End_MagicStone(_double dTimeDelta)
{
	m_pModelCom->Set_LerpTime(0.2f);

	m_pModelCom->Set_CurAnimIndex(GOLEM_END_MAGICSTONE);
}

void CBoss_Golem_State::Start_Start_SpawnPillars(_double dTimeDelta)
{
	m_pModelCom->Set_LerpTime(0.2f);
	m_pModelCom->Set_CurAnimIndex(GOLEM_START_SPAWNPILLARS);
}

void CBoss_Golem_State::Start_Melee_And_Ready_SpawnPillars(_double dTimeDelta)
{
	m_pModelCom->Set_LerpTime(0.2f);
	m_pModelCom->Set_CurAnimIndex(GOLEM_MELEE_N_SPAWNPILLARS);
}

void CBoss_Golem_State::Start_Dead(_double dTimeDelta)
{
	m_pModelCom->Set_LerpTime(0.2f);
	m_pModelCom->Set_CurAnimIndex(GOLEM_DIE);

	if(m_pModelCom->Get_LastAnimationIndex() == GOLEM_DIE)
	{
		m_pMonster->Set_Dead(true);
	}
}

void CBoss_Golem_State::Tick_Common(_double dTimeDelta)
{
}

void CBoss_Golem_State::Tick_Intro0(_double dTimeDelta)
{
	// // m_pModelCom->Set_CurAnimIndex(GOLEM_INTRO0);
	// if (m_pModelCom->Get_LastAnimationIndex() == GOLEM_INTRO0)
	// 	m_pMonster->m_eLerpType = CModel::LERP_CONTINUE;
	// else
	// 	m_pMonster->m_eLerpType = CModel::LERP_BEGIN;
}

void CBoss_Golem_State::Tick_Intro1(_double dTimeDelta)
{
	if(m_pModelCom->Get_AnimationProgress() > 0.2f && m_pModelCom->Get_AnimationProgress() < 0.22f && m_bCamSet == false)
	{
		_bool&		DynamicCamera = dynamic_cast<CCamera*>(CGameInstance::GetInstance()->Get_CloneObjectList(LEVEL_GAMEPLAY, L"Layer_ZCamera")->front())->Get_RenderState();
		_bool&		StaticCamera = dynamic_cast<CCamera*>(CGameInstance::GetInstance()->Get_CloneObjectList(LEVEL_GAMEPLAY, L"Layer_ZCamera")->back())->Get_RenderState();
		DynamicCamera = !DynamicCamera;
		StaticCamera = !StaticCamera;
		_float scale[3]{1.f, 1.f, 1.f}, Rot[3]{ -12.84f ,-34.574f , 0.0f }, Pos[3]{ 111.4f ,11.47f, 129.6f };
		_matrix camWorld;
		ImGuizmo::RecomposeMatrixFromComponents(Pos, Rot, scale, (_float*)&camWorld);
		m_pDynamic_Camera->Set_Boss_IntroCam(camWorld);
		m_bCamSet = true;
		m_pFadeInOut->Fade_Out(true);
	}

	if(m_pModelCom->Get_AnimationProgress() > 0.95f)
	{
		m_pFadeInOut->Fade_Out(false);
	}
}

void CBoss_Golem_State::Tick_Intro2(_double dTimeDelta)
{
	if(m_pModelCom->Get_AnimationProgress() > 0.01f)
	{
		m_pFadeInOut->Fade_Out(true);
	}

	if(m_pModelCom->Get_AnimationProgress() > 0.975f)
	{
		m_pFadeInOut->Fade_Out(false);
	}
}

void CBoss_Golem_State::Tick_Idle(_double dTimeDelta)
{
	// m_pModelCom->Set_CurAnimIndex(GOLEM_INCOMBAT_IDLE);

	// 자폭병, 애기 벌레 소환
	// if (m_pModelCom->Get_LastAnimationIndex() == GOLEM_INCOMBAT_IDLE)
	// 	m_pMonster->m_eLerpType = CModel::LERP_CONTINUE;
	// else
	// 	m_pMonster->m_eLerpType = CModel::LERP_BEGIN;

	if(m_pModelCom->Get_AnimationProgress() > 0.1f)
	{
		m_pFadeInOut->Fade_Out(true);
	}

	//~ 자폭병, 애기 벌레 소환
}

void CBoss_Golem_State::Tick_Ready_Arm_Fire(_double dTimeDelta)
{
}

void CBoss_Golem_State::Tick_Fire_Arm_RightArm(_double dTimeDelta)
{
}

void CBoss_Golem_State::Tick_Fire_Arm_LeftArm(_double dTimeDelta)
{
}

void CBoss_Golem_State::Tick_End_Arm_Fire(_double dTimeDelta)
{
}

void CBoss_Golem_State::Tick_Ready_Lazer(_double dTimeDelta)
{
}

void CBoss_Golem_State::Tick_Fire_Lazer(_double dTimeDelta)
{
}

void CBoss_Golem_State::Tick_Ready_MagicStone(_double dTimeDelta)
{
}

void CBoss_Golem_State::Tick_End_MagicStone(_double dTimeDelta)
{
}

void CBoss_Golem_State::Tick_Start_SpawnPillars(_double dTimeDelta)
{
}

void CBoss_Golem_State::Tick_Melee_And_Ready_SpawnPillars(_double dTimeDelta)
{
}

void CBoss_Golem_State::Tick_Dead(_double dTimeDelta)
{
}

void CBoss_Golem_State::End_Common(_double TimeDelta)
{
}

void CBoss_Golem_State::End_Intro0(_double TimeDelta)
{
	// m_pFadeInOut->Fade_Out(false);
	// 페이드아웃
}

void CBoss_Golem_State::End_Intro1(_double dTimeDelta)
{
}

void CBoss_Golem_State::End_Intro2(_double dTimeDelta)
{
	_bool&		DynamicCamera = dynamic_cast<CCamera*>(CGameInstance::GetInstance()->Get_CloneObjectList(LEVEL_GAMEPLAY, L"Layer_ZCamera")->front())->Get_RenderState();
	_bool&		StaticCamera = dynamic_cast<CCamera*>(CGameInstance::GetInstance()->Get_CloneObjectList(LEVEL_GAMEPLAY, L"Layer_ZCamera")->back())->Get_RenderState();


	DynamicCamera = !DynamicCamera;
	StaticCamera = !StaticCamera;

	m_pMonster->Render_UI();
}

void CBoss_Golem_State::End_Idle(_double dTimeDelta)
{
	m_pModelCom->Reset_IsFinished();
}

void CBoss_Golem_State::End_Ready_Arm_Fire(_double dTimeDelta)
{
}

void CBoss_Golem_State::End_Fire_Arm_RightArm(_double dTimeDelta)
{
}

void CBoss_Golem_State::End_Fire_Arm_LeftArm(_double dTimeDelta)
{
}

void CBoss_Golem_State::End_End_Arm_Fire(_double dTimeDelta)
{
}

void CBoss_Golem_State::End_Ready_Lazer(_double dTimeDelta)
{
}

void CBoss_Golem_State::End_Fire_Lazer(_double dTimeDelta)
{
}

void CBoss_Golem_State::End_Ready_MagicStone(_double dTimeDelta)
{
}

void CBoss_Golem_State::End_End_MagicStone(_double dTimeDelta)
{
}

void CBoss_Golem_State::End_Start_SpawnPillars(_double dTimeDelta)
{
}

void CBoss_Golem_State::End_Melee_And_Ready_SpawnPillars(_double dTimeDelta)
{
}

void CBoss_Golem_State::End_Dead(_double dTimeDelta)
{
}





_bool CBoss_Golem_State::Animation_Finish()
{
	return m_pModelCom->Get_AnimationFinish();
}

_bool CBoss_Golem_State::Player_Detected()
{
	return m_pMonster->m_bPlayerDetected;
}

_bool CBoss_Golem_State::Player_DetectedAndFar()
{
	return !m_pMonster->m_bPlayerDetectedClose;
}

_bool CBoss_Golem_State::Player_DetectedAndClose()
{
	if (m_bCanAttack == false)
		return m_pMonster->m_bPlayerDetectedClose;
	else
		false;

	return false;
}

_bool CBoss_Golem_State::Player_CloseAndCanAttack()
{
	if (m_bCanAttack)
	{
		return m_pMonster->m_bPlayerDetectedClose;
	}
	else
		return false;
}

_bool CBoss_Golem_State::Player_FarAndCanAttack()
{
	if (m_bCanAttack)
	{
		return !m_pMonster->m_bPlayerDetectedClose;
	}
	else
		return false;
}



_bool CBoss_Golem_State::IS_Dead()
{
	if (m_pMonster->m_bIsDead == true)
		return true;
	else 
		return false;
}

_bool CBoss_Golem_State::Is_Can_Spawn_Pillars()
{
	return false;
}

_bool CBoss_Golem_State::Is_Can_Spawn_MagicStone()
{
	return false;
}

_bool CBoss_Golem_State::Is_Can_Fire_Lazer()
{
	return false;
}

_bool CBoss_Golem_State::Is_Can_Fire_Arm()
{
	return false;
}

_bool CBoss_Golem_State::Is_Can_Spawn_Monster()
{
	return false;
}

_bool CBoss_Golem_State::Is_Pillars_Empty()
{
	return false;
}

CBoss_Golem_State* CBoss_Golem_State::Create(CNormal_Boss* pOwner, CState* pStateMachineCom, CModel* pModel,
                                             CTransform* pTransform, CNavigation* pNavigation)
{
	CBoss_Golem_State*		pInstance = new CBoss_Golem_State();
	if (FAILED(pInstance->Initialize(pOwner, pStateMachineCom, pModel, pTransform, pNavigation)))
	{
		MSG_BOX("Failed to Create : CBoss_Golem_State");
		Safe_Release(pInstance);

		return nullptr;
	}

	return pInstance;
}

void CBoss_Golem_State::Free()
{
	Safe_Release(m_pFadeInOut);
}
