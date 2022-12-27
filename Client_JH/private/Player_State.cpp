#include "stdafx.h"
#include "..\public\Player_State.h"
#include "Player.h"
#include "GameInstance.h"
#include "State.h"

CPlayer_State::CPlayer_State()
{
}

HRESULT CPlayer_State::Initialize(CPlayer* pPlayer)
{
	m_pPlayer = pPlayer;

	FAILED_CHECK_RETURN(SetUp_State_Idle(), E_FAIL);
	FAILED_CHECK_RETURN(SetUp_State_Walk(), E_FAIL);
	// FAILED_CHECK_RETURN(SetUp_State_Fire(), E_FAIL);
	// FAILED_CHECK_RETURN(SetUp_State_Reload(), E_FAIL);
	FAILED_CHECK_RETURN(SetUp_State_Dash(), E_FAIL);
	FAILED_CHECK_RETURN(SetUp_State_Damaged(), E_FAIL);

	return S_OK;
}
#pragma region SetUp_State

HRESULT CPlayer_State::SetUp_State_Idle()
{
	NULL_CHECK_RETURN(m_pPlayer->m_pState, E_FAIL);
	
	m_pPlayer->m_pState->Set_Root(L"STATE::IDLE")

		.Add_State(L"STATE::IDLE")
		.Init_Start(this, &CPlayer_State::Start_Idle)
		.Init_Tick(this, &CPlayer_State::Tick_Idle)
		.Init_End(this, &CPlayer_State::End_Common)
		.Init_Changer(L"STATE::WALK_F", this, &CPlayer_State::KeyInput_W)
		.Init_Changer(L"STATE::WALK_B", this, &CPlayer_State::KeyInput_S)
		.Init_Changer(L"STATE::WALK_L", this, &CPlayer_State::KeyInput_A)
		.Init_Changer(L"STATE::WALK_R", this, &CPlayer_State::KeyInput_D)
		.Init_Changer(L"STATE::WALK_FL", this, &CPlayer_State::KeyInput_WA)
		.Init_Changer(L"STATE::WALK_FR", this, &CPlayer_State::KeyInput_WD)
		.Init_Changer(L"STATE::WALK_BL", this, &CPlayer_State::KeyInput_SA)
		.Init_Changer(L"STATE::WALK_BR", this, &CPlayer_State::KeyInput_SD)
		.Init_Changer(L"STATE::DASH_F", this, &CPlayer_State::KeyInput_Shift)
		// .Init_Changer(L"STATE::FIRE", this, &CPlayer_State::MouseDown_LB)
		// .Init_Changer(L"STATE::RELOAD", this, &CPlayer_State::KeyInput_R)

		.Finish_Setting();

	return S_OK;
}

HRESULT CPlayer_State::SetUp_State_Walk()
{
	NULL_CHECK_RETURN(m_pPlayer->m_pState, E_FAIL);

	m_pPlayer->m_pState->Add_State(L"STATE::WALK_F")
		.Init_Start(this, &CPlayer_State::Start_Walk_F)
		.Init_Tick(this, &CPlayer_State::Tick_Walk_F)
		.Init_End(this, &CPlayer_State::End_Common)
		.Init_Changer(L"STATE::WALK_B", this, &CPlayer_State::KeyInput_S)
		.Init_Changer(L"STATE::WALK_L", this, &CPlayer_State::KeyInput_A)
		.Init_Changer(L"STATE::WALK_R", this, &CPlayer_State::KeyInput_D)
		.Init_Changer(L"STATE::WALK_FL", this, &CPlayer_State::KeyInput_WA)
		.Init_Changer(L"STATE::WALK_FR", this, &CPlayer_State::KeyInput_WD)
		.Init_Changer(L"STATE::WALK_BL", this, &CPlayer_State::KeyInput_SA)
		.Init_Changer(L"STATE::WALK_BR", this, &CPlayer_State::KeyInput_SD)
		.Init_Changer(L"STATE::DASH_F", this, &CPlayer_State::KeyInput_Shift)
		// .Init_Changer(L"STATE::FIRE", this, &CPlayer_State::MouseDown_LB)
		// .Init_Changer(L"STATE::RELOAD", this, &CPlayer_State::KeyInput_R)
		.Init_Changer(L"STATE::IDLE", this, &CPlayer_State::KeyInput_None)

		.Add_State(L"STATE::WALK_B")
		.Init_Start(this, &CPlayer_State::Start_Walk_B)
		.Init_Tick(this, &CPlayer_State::Tick_Walk_B)
		.Init_End(this, &CPlayer_State::End_Common)
		.Init_Changer(L"STATE::WALK_F", this, &CPlayer_State::KeyInput_W)
		.Init_Changer(L"STATE::WALK_L", this, &CPlayer_State::KeyInput_A)
		.Init_Changer(L"STATE::WALK_R", this, &CPlayer_State::KeyInput_D)
		.Init_Changer(L"STATE::WALK_FL", this, &CPlayer_State::KeyInput_WA)
		.Init_Changer(L"STATE::WALK_FR", this, &CPlayer_State::KeyInput_WD)
		.Init_Changer(L"STATE::WALK_BL", this, &CPlayer_State::KeyInput_SA)
		.Init_Changer(L"STATE::WALK_BR", this, &CPlayer_State::KeyInput_SD)
		.Init_Changer(L"STATE::DASH_B", this, &CPlayer_State::KeyInput_Shift)
		// .Init_Changer(L"STATE::FIRE", this, &CPlayer_State::MouseDown_LB)
		// .Init_Changer(L"STATE::RELOAD", this, &CPlayer_State::KeyInput_R)
		.Init_Changer(L"STATE::IDLE", this, &CPlayer_State::KeyInput_None)

		.Add_State(L"STATE::WALK_L")
		.Init_Start(this, &CPlayer_State::Start_Walk_L)
		.Init_Tick(this, &CPlayer_State::Tick_Walk_L)
		.Init_End(this, &CPlayer_State::End_Common)
		.Init_Changer(L"STATE::WALK_F", this, &CPlayer_State::KeyInput_W)
		.Init_Changer(L"STATE::WALK_B", this, &CPlayer_State::KeyInput_S)
		.Init_Changer(L"STATE::WALK_R", this, &CPlayer_State::KeyInput_D)
		.Init_Changer(L"STATE::WALK_FL", this, &CPlayer_State::KeyInput_WA)
		.Init_Changer(L"STATE::WALK_FR", this, &CPlayer_State::KeyInput_WD)
		.Init_Changer(L"STATE::WALK_BL", this, &CPlayer_State::KeyInput_SA)
		.Init_Changer(L"STATE::WALK_BR", this, &CPlayer_State::KeyInput_SD)
		.Init_Changer(L"STATE::DASH_L", this, &CPlayer_State::KeyInput_Shift)
		// .Init_Changer(L"STATE::FIRE", this, &CPlayer_State::MouseDown_LB)
		// .Init_Changer(L"STATE::RELOAD", this, &CPlayer_State::KeyInput_R)
		.Init_Changer(L"STATE::IDLE", this, &CPlayer_State::KeyInput_None)

		.Add_State(L"STATE::WALK_R")
		.Init_Start(this, &CPlayer_State::Start_Walk_R)
		.Init_Tick(this, &CPlayer_State::Tick_Walk_R)
		.Init_End(this, &CPlayer_State::End_Common)
		.Init_Changer(L"STATE::WALK_F", this, &CPlayer_State::KeyInput_W)
		.Init_Changer(L"STATE::WALK_B", this, &CPlayer_State::KeyInput_S)
		.Init_Changer(L"STATE::WALK_L", this, &CPlayer_State::KeyInput_A)
		.Init_Changer(L"STATE::WALK_FL", this, &CPlayer_State::KeyInput_WA)
		.Init_Changer(L"STATE::WALK_FR", this, &CPlayer_State::KeyInput_WD)
		.Init_Changer(L"STATE::WALK_BL", this, &CPlayer_State::KeyInput_SA)
		.Init_Changer(L"STATE::WALK_BR", this, &CPlayer_State::KeyInput_SD)
		.Init_Changer(L"STATE::DASH_R", this, &CPlayer_State::KeyInput_Shift)
		// .Init_Changer(L"STATE::FIRE", this, &CPlayer_State::MouseDown_LB)
		// .Init_Changer(L"STATE::RELOAD", this, &CPlayer_State::KeyInput_R)
		.Init_Changer(L"STATE::IDLE", this, &CPlayer_State::KeyInput_None)

		.Add_State(L"STATE::WALK_FL")
		.Init_Start(this, &CPlayer_State::Start_Walk_FL)
		.Init_Tick(this, &CPlayer_State::Tick_Walk_FL)
		.Init_End(this, &CPlayer_State::End_Common)
		.Init_Changer(L"STATE::WALK_F", this, &CPlayer_State::KeyInput_W)
		.Init_Changer(L"STATE::WALK_B", this, &CPlayer_State::KeyInput_S)
		.Init_Changer(L"STATE::WALK_L", this, &CPlayer_State::KeyInput_A)
		.Init_Changer(L"STATE::WALK_R", this, &CPlayer_State::KeyInput_D)
		.Init_Changer(L"STATE::WALK_FR", this, &CPlayer_State::KeyInput_WD)
		.Init_Changer(L"STATE::WALK_BL", this, &CPlayer_State::KeyInput_SA)
		.Init_Changer(L"STATE::WALK_BR", this, &CPlayer_State::KeyInput_SD)
		.Init_Changer(L"STATE::DASH_FL", this, &CPlayer_State::KeyInput_Shift)
		// .Init_Changer(L"STATE::FIRE", this, &CPlayer_State::MouseDown_LB)
		// .Init_Changer(L"STATE::RELOAD", this, &CPlayer_State::KeyInput_R)
		.Init_Changer(L"STATE::IDLE", this, &CPlayer_State::KeyInput_None)

		.Add_State(L"STATE::WALK_FR")
		.Init_Start(this, &CPlayer_State::Start_Walk_FR)
		.Init_Tick(this, &CPlayer_State::Tick_Walk_FR)
		.Init_End(this, &CPlayer_State::End_Common)
		.Init_Changer(L"STATE::WALK_F", this, &CPlayer_State::KeyInput_W)
		.Init_Changer(L"STATE::WALK_B", this, &CPlayer_State::KeyInput_S)
		.Init_Changer(L"STATE::WALK_L", this, &CPlayer_State::KeyInput_A)
		.Init_Changer(L"STATE::WALK_R", this, &CPlayer_State::KeyInput_D)
		.Init_Changer(L"STATE::WALK_FL", this, &CPlayer_State::KeyInput_WA)
		.Init_Changer(L"STATE::WALK_BL", this, &CPlayer_State::KeyInput_SA)
		.Init_Changer(L"STATE::WALK_BR", this, &CPlayer_State::KeyInput_SD)
		.Init_Changer(L"STATE::DASH_FR", this, &CPlayer_State::KeyInput_Shift)
		// .Init_Changer(L"STATE::FIRE", this, &CPlayer_State::MouseDown_LB)
		// .Init_Changer(L"STATE::RELOAD", this, &CPlayer_State::KeyInput_R)
		.Init_Changer(L"STATE::IDLE", this, &CPlayer_State::KeyInput_None)

		.Add_State(L"STATE::WALK_BL")
		.Init_Start(this, &CPlayer_State::Start_Walk_BL)
		.Init_Tick(this, &CPlayer_State::Tick_Walk_BL)
		.Init_End(this, &CPlayer_State::End_Common)
		.Init_Changer(L"STATE::WALK_F", this, &CPlayer_State::KeyInput_W)
		.Init_Changer(L"STATE::WALK_B", this, &CPlayer_State::KeyInput_S)
		.Init_Changer(L"STATE::WALK_L", this, &CPlayer_State::KeyInput_A)
		.Init_Changer(L"STATE::WALK_R", this, &CPlayer_State::KeyInput_D)
		.Init_Changer(L"STATE::WALK_FL", this, &CPlayer_State::KeyInput_WA)
		.Init_Changer(L"STATE::WALK_FR", this, &CPlayer_State::KeyInput_WD)
		.Init_Changer(L"STATE::WALK_BR", this, &CPlayer_State::KeyInput_SD)
		.Init_Changer(L"STATE::DASH_BL", this, &CPlayer_State::KeyInput_Shift)
		// .Init_Changer(L"STATE::FIRE", this, &CPlayer_State::MouseDown_LB)
		// .Init_Changer(L"STATE::RELOAD", this, &CPlayer_State::KeyInput_R)
		.Init_Changer(L"STATE::IDLE", this, &CPlayer_State::KeyInput_None)

		.Add_State(L"STATE::WALK_BR")
		.Init_Start(this, &CPlayer_State::Start_Walk_BR)
		.Init_Tick(this, &CPlayer_State::Tick_Walk_BR)
		.Init_End(this, &CPlayer_State::End_Common)
		.Init_Changer(L"STATE::WALK_F", this, &CPlayer_State::KeyInput_W)
		.Init_Changer(L"STATE::WALK_B", this, &CPlayer_State::KeyInput_S)
		.Init_Changer(L"STATE::WALK_L", this, &CPlayer_State::KeyInput_A)
		.Init_Changer(L"STATE::WALK_R", this, &CPlayer_State::KeyInput_D)
		.Init_Changer(L"STATE::WALK_FL", this, &CPlayer_State::KeyInput_WA)
		.Init_Changer(L"STATE::WALK_FR", this, &CPlayer_State::KeyInput_WD)
		.Init_Changer(L"STATE::WALK_BL", this, &CPlayer_State::KeyInput_SA)
		.Init_Changer(L"STATE::DASH_BR", this, &CPlayer_State::KeyInput_Shift)
		// .Init_Changer(L"STATE::FIRE", this, &CPlayer_State::MouseDown_LB)
		// .Init_Changer(L"STATE::RELOAD", this, &CPlayer_State::KeyInput_R)
		.Init_Changer(L"STATE::IDLE", this, &CPlayer_State::KeyInput_None)

		.Finish_Setting();

	return S_OK;
}

HRESULT CPlayer_State::SetUp_State_Dash()
{
	NULL_CHECK_RETURN(m_pPlayer->m_pState, E_FAIL);

	m_pPlayer->m_pState->
		Add_State(L"STATE::DASH_F")
		.Init_Start(this, &CPlayer_State::Start_Dash_F)
		.Init_Tick(this, &CPlayer_State::Tick_Dash_F)
		.Init_End(this, &CPlayer_State::End_Common)
		.Init_Changer(L"STATE::WALK_F", this, &CPlayer_State::KeyInput_W)
		.Init_Changer(L"STATE::WALK_B", this, &CPlayer_State::KeyInput_S)
		.Init_Changer(L"STATE::WALK_L", this, &CPlayer_State::KeyInput_A)
		.Init_Changer(L"STATE::WALK_R", this, &CPlayer_State::KeyInput_D)
		.Init_Changer(L"STATE::WALK_FL", this, &CPlayer_State::KeyInput_WA)
		.Init_Changer(L"STATE::WALK_FR", this, &CPlayer_State::KeyInput_WD)
		.Init_Changer(L"STATE::WALK_BL", this, &CPlayer_State::KeyInput_SA)
		.Init_Changer(L"STATE::WALK_BR", this, &CPlayer_State::KeyInput_SD)
		// .Init_Changer(L"STATE::FIRE", this, &CPlayer_State::MouseDown_LB)
		.Init_Changer(L"STATE::RELOAD", this, &CPlayer_State::KeyInput_R)
		.Init_Changer(L"STATE::IDLE", this, &CPlayer_State::isFinish)

		.Add_State(L"STATE::DASH_B")
		.Init_Start(this, &CPlayer_State::Start_Dash_B)
		.Init_Tick(this, &CPlayer_State::Tick_Dash_B)
		.Init_End(this, &CPlayer_State::End_Common)
		.Init_Changer(L"STATE::WALK_F", this, &CPlayer_State::KeyInput_W)
		.Init_Changer(L"STATE::WALK_B", this, &CPlayer_State::KeyInput_S)
		.Init_Changer(L"STATE::WALK_L", this, &CPlayer_State::KeyInput_A)
		.Init_Changer(L"STATE::WALK_R", this, &CPlayer_State::KeyInput_D)
		.Init_Changer(L"STATE::WALK_FL", this, &CPlayer_State::KeyInput_WA)
		.Init_Changer(L"STATE::WALK_FR", this, &CPlayer_State::KeyInput_WD)
		.Init_Changer(L"STATE::WALK_BL", this, &CPlayer_State::KeyInput_SA)
		.Init_Changer(L"STATE::WALK_BR", this, &CPlayer_State::KeyInput_SD)
		// .Init_Changer(L"STATE::FIRE", this, &CPlayer_State::MouseDown_LB)
		.Init_Changer(L"STATE::RELOAD", this, &CPlayer_State::KeyInput_R)
		.Init_Changer(L"STATE::IDLE", this, &CPlayer_State::isFinish)

		.Add_State(L"STATE::DASH_L")
		.Init_Start(this, &CPlayer_State::Start_Dash_L)
		.Init_Tick(this, &CPlayer_State::Tick_Dash_L)
		.Init_End(this, &CPlayer_State::End_Common)
		.Init_Changer(L"STATE::WALK_F", this, &CPlayer_State::KeyInput_W)
		.Init_Changer(L"STATE::WALK_B", this, &CPlayer_State::KeyInput_S)
		.Init_Changer(L"STATE::WALK_L", this, &CPlayer_State::KeyInput_A)
		.Init_Changer(L"STATE::WALK_R", this, &CPlayer_State::KeyInput_D)
		.Init_Changer(L"STATE::WALK_FL", this, &CPlayer_State::KeyInput_WA)
		.Init_Changer(L"STATE::WALK_FR", this, &CPlayer_State::KeyInput_WD)
		.Init_Changer(L"STATE::WALK_BL", this, &CPlayer_State::KeyInput_SA)
		.Init_Changer(L"STATE::WALK_BR", this, &CPlayer_State::KeyInput_SD)
		// .Init_Changer(L"STATE::FIRE", this, &CPlayer_State::MouseDown_LB)
		.Init_Changer(L"STATE::RELOAD", this, &CPlayer_State::KeyInput_R)
		.Init_Changer(L"STATE::IDLE", this, &CPlayer_State::isFinish)

		.Add_State(L"STATE::DASH_R")
		.Init_Start(this, &CPlayer_State::Start_Dash_R)
		.Init_Tick(this, &CPlayer_State::Tick_Dash_R)
		.Init_End(this, &CPlayer_State::End_Common)
		.Init_Changer(L"STATE::WALK_F", this, &CPlayer_State::KeyInput_W)
		.Init_Changer(L"STATE::WALK_B", this, &CPlayer_State::KeyInput_S)
		.Init_Changer(L"STATE::WALK_L", this, &CPlayer_State::KeyInput_A)
		.Init_Changer(L"STATE::WALK_R", this, &CPlayer_State::KeyInput_D)
		.Init_Changer(L"STATE::WALK_FL", this, &CPlayer_State::KeyInput_WA)
		.Init_Changer(L"STATE::WALK_FR", this, &CPlayer_State::KeyInput_WD)
		.Init_Changer(L"STATE::WALK_BL", this, &CPlayer_State::KeyInput_SA)
		.Init_Changer(L"STATE::WALK_BR", this, &CPlayer_State::KeyInput_SD)
		// .Init_Changer(L"STATE::FIRE", this, &CPlayer_State::MouseDown_LB)
		.Init_Changer(L"STATE::RELOAD", this, &CPlayer_State::KeyInput_R)
		.Init_Changer(L"STATE::IDLE", this, &CPlayer_State::isFinish)

		.Add_State(L"STATE::DASH_FL")
		.Init_Start(this, &CPlayer_State::Start_Dash_FL)
		.Init_Tick(this, &CPlayer_State::Tick_Dash_FL)
		.Init_End(this, &CPlayer_State::End_Common)
		.Init_Changer(L"STATE::WALK_F", this, &CPlayer_State::KeyInput_W)
		.Init_Changer(L"STATE::WALK_B", this, &CPlayer_State::KeyInput_S)
		.Init_Changer(L"STATE::WALK_L", this, &CPlayer_State::KeyInput_A)
		.Init_Changer(L"STATE::WALK_R", this, &CPlayer_State::KeyInput_D)
		.Init_Changer(L"STATE::WALK_FL", this, &CPlayer_State::KeyInput_WA)
		.Init_Changer(L"STATE::WALK_FR", this, &CPlayer_State::KeyInput_WD)
		.Init_Changer(L"STATE::WALK_BL", this, &CPlayer_State::KeyInput_SA)
		.Init_Changer(L"STATE::WALK_BR", this, &CPlayer_State::KeyInput_SD)
		// .Init_Changer(L"STATE::FIRE", this, &CPlayer_State::MouseDown_LB)
		.Init_Changer(L"STATE::RELOAD", this, &CPlayer_State::KeyInput_R)
		.Init_Changer(L"STATE::IDLE", this, &CPlayer_State::isFinish)

		.Add_State(L"STATE::DASH_FR")
		.Init_Start(this, &CPlayer_State::Start_Dash_FR)
		.Init_Tick(this, &CPlayer_State::Tick_Dash_FR)
		.Init_End(this, &CPlayer_State::End_Common)
		.Init_Changer(L"STATE::WALK_F", this, &CPlayer_State::KeyInput_W)
		.Init_Changer(L"STATE::WALK_B", this, &CPlayer_State::KeyInput_S)
		.Init_Changer(L"STATE::WALK_L", this, &CPlayer_State::KeyInput_A)
		.Init_Changer(L"STATE::WALK_R", this, &CPlayer_State::KeyInput_D)
		.Init_Changer(L"STATE::WALK_FL", this, &CPlayer_State::KeyInput_WA)
		.Init_Changer(L"STATE::WALK_FR", this, &CPlayer_State::KeyInput_WD)
		.Init_Changer(L"STATE::WALK_BL", this, &CPlayer_State::KeyInput_SA)
		.Init_Changer(L"STATE::WALK_BR", this, &CPlayer_State::KeyInput_SD)
		// .Init_Changer(L"STATE::FIRE", this, &CPlayer_State::MouseDown_LB)
		.Init_Changer(L"STATE::RELOAD", this, &CPlayer_State::KeyInput_R)
		.Init_Changer(L"STATE::IDLE", this, &CPlayer_State::isFinish)

		.Add_State(L"STATE::DASH_BL")
		.Init_Start(this, &CPlayer_State::Start_Dash_BL)
		.Init_Tick(this, &CPlayer_State::Tick_Dash_BL)
		.Init_End(this, &CPlayer_State::End_Common)
		.Init_Changer(L"STATE::WALK_F", this, &CPlayer_State::KeyInput_W)
		.Init_Changer(L"STATE::WALK_B", this, &CPlayer_State::KeyInput_S)
		.Init_Changer(L"STATE::WALK_L", this, &CPlayer_State::KeyInput_A)
		.Init_Changer(L"STATE::WALK_R", this, &CPlayer_State::KeyInput_D)
		.Init_Changer(L"STATE::WALK_FL", this, &CPlayer_State::KeyInput_WA)
		.Init_Changer(L"STATE::WALK_FR", this, &CPlayer_State::KeyInput_WD)
		.Init_Changer(L"STATE::WALK_BL", this, &CPlayer_State::KeyInput_SA)
		.Init_Changer(L"STATE::WALK_BR", this, &CPlayer_State::KeyInput_SD)
		// .Init_Changer(L"STATE::FIRE", this, &CPlayer_State::MouseDown_LB)
		.Init_Changer(L"STATE::RELOAD", this, &CPlayer_State::KeyInput_R)
		.Init_Changer(L"STATE::IDLE", this, &CPlayer_State::isFinish)

		.Add_State(L"STATE::DASH_BR")
		.Init_Start(this, &CPlayer_State::Start_Dash_BR)
		.Init_Tick(this, &CPlayer_State::Tick_Dash_BR)
		.Init_End(this, &CPlayer_State::End_Common)
		.Init_Changer(L"STATE::WALK_F", this, &CPlayer_State::KeyInput_W)
		.Init_Changer(L"STATE::WALK_B", this, &CPlayer_State::KeyInput_S)
		.Init_Changer(L"STATE::WALK_L", this, &CPlayer_State::KeyInput_A)
		.Init_Changer(L"STATE::WALK_R", this, &CPlayer_State::KeyInput_D)
		.Init_Changer(L"STATE::WALK_FL", this, &CPlayer_State::KeyInput_WA)
		.Init_Changer(L"STATE::WALK_FR", this, &CPlayer_State::KeyInput_WD)
		.Init_Changer(L"STATE::WALK_BL", this, &CPlayer_State::KeyInput_SA)
		.Init_Changer(L"STATE::WALK_BR", this, &CPlayer_State::KeyInput_SD)
		// .Init_Changer(L"STATE::FIRE", this, &CPlayer_State::MouseDown_LB)
		.Init_Changer(L"STATE::RELOAD", this, &CPlayer_State::KeyInput_R)
		.Init_Changer(L"STATE::IDLE", this, &CPlayer_State::isFinish)

		.Finish_Setting();

	return S_OK;

}

HRESULT CPlayer_State::SetUp_State_Fire()
{

	return S_OK;
}

HRESULT CPlayer_State::SetUp_State_Reload()
{

	return S_OK;

}

HRESULT CPlayer_State::SetUp_State_Damaged()
{

	return S_OK;

}

HRESULT CPlayer_State::SetUp_State_SelectPage()
{

	return S_OK;

}

#pragma endregion


void CPlayer_State::Start_Idle(_double TimeDelta)
{
	// if (m_pPlayer->m_wstrCurWeaponName == L"WEAPON_DEFAULT")
	// {
	//
	// 	m_pPlayer->m_pModelCom = m_pPlayer->m_tWeaponDesc[CPlayer::WEAPON_DEFAULT].m_pWeaponModelCom;
	//
	// 	m_pPlayer->m_pModelCom->Set_CurAnimIndex(DEFAULT_PISTOL_IDLE);
	// }
	// else if (m_pPlayer->m_wstrCurWeaponName == L"WEAPON_FLAMEBULLET")
	// {
	// 	m_pPlayer->m_pModelCom = m_pPlayer->m_tWeaponDesc[CPlayer::WEAPON_FLAMEBULLET].m_pWeaponModelCom;
	//
	// 	m_pPlayer->m_pModelCom->Set_CurAnimIndex(FLAME_BULLET_IDLE);
	// }
	// else if (m_pPlayer->m_wstrCurWeaponName == L"WEAPON_FIREDRAGON")
	// {
	// 	m_pPlayer->m_pModelCom = m_pPlayer->m_tWeaponDesc[CPlayer::WEAPON_FIREDRAGON].m_pWeaponModelCom;
	//
	// 	m_pPlayer->m_pModelCom->Set_CurAnimIndex(FIRE_DRAGON_IDLE);
	// }
	// else if (m_pPlayer->m_wstrCurWeaponName == L"WEAPON_POISON")
	// {
	// 	m_pPlayer->m_pModelCom = m_pPlayer->m_tWeaponDesc[CPlayer::WEAPON_POISON].m_pWeaponModelCom;
	//
	// 	m_pPlayer->m_pModelCom->Set_CurAnimIndex(POISON_IDLE);
	// }
}

void CPlayer_State::Start_Fire(_double TimeDelta)
{
	if (m_pPlayer->m_wstrCurWeaponName == L"WEAPON_DEFAULT")
	{
		m_pPlayer->m_pModelCom->Set_CurAnimIndex(DEFAULT_PISTOL_FIRE);
	}
	else if (m_pPlayer->m_wstrCurWeaponName == L"WEAPON_FLAMEBULLET")
	{
		m_pPlayer->m_pModelCom->Set_CurAnimIndex(FLAME_BULLET_FIRE);
	}
	else if (m_pPlayer->m_wstrCurWeaponName == L"WEAPON_FIREDRAGON")
	{
		m_pPlayer->m_pModelCom->Set_CurAnimIndex(FIRE_DRAGON_FIRE);
	}
	else if (m_pPlayer->m_wstrCurWeaponName == L"WEAPON_POISON")
	{
		if (m_pPlayer->m_iPoisonAttCnt == 2)
			m_pPlayer->m_iPoisonAttCnt = 0;

		m_pPlayer->m_pModelCom->Set_CurAnimIndex(POISON_FIRE_A + m_pPlayer->m_iPoisonAttCnt);
		m_pPlayer->m_iPoisonAttCnt++;
	}
}

void CPlayer_State::Strat_Reload(_double TimeDelta)
{
	if (m_pPlayer->m_wstrCurWeaponName == L"WEAPON_DEFAULT")
	{
		m_pPlayer->m_pModelCom->Set_CurAnimIndex(DEFAULT_PISTOL_RELOAD);
	}
	else if (m_pPlayer->m_wstrCurWeaponName == L"WEAPON_FLAMEBULLET")
	{
		m_pPlayer->m_pModelCom->Set_CurAnimIndex(FLAME_BULLET_RELOAD);
	}
	else if (m_pPlayer->m_wstrCurWeaponName == L"WEAPON_FIREDRAGON")
	{
		m_pPlayer->m_pModelCom->Set_CurAnimIndex(FIRE_DRAGON_RELOAD);
	}
	else if (m_pPlayer->m_wstrCurWeaponName == L"WEAPON_POISON")
		m_pPlayer->m_pModelCom->Set_CurAnimIndex(POISON_IDLE);
}

#pragma region Walk / Dash (Have no Anim)
void CPlayer_State::Start_Walk_F(_double TimeDelta)
{
}

void CPlayer_State::Start_Walk_B(_double TimeDelta)
{
}

void CPlayer_State::Start_Walk_L(_double TimeDelta)
{
}

void CPlayer_State::Start_Walk_R(_double TimeDelta)
{
}

void CPlayer_State::Start_Walk_FL(_double TimeDelta)
{
}

void CPlayer_State::Start_Walk_FR(_double TimeDelta)
{
}

void CPlayer_State::Start_Walk_BL(_double TimeDelta)
{
}

void CPlayer_State::Start_Walk_BR(_double TimeDelta)
{
}

void CPlayer_State::Start_Dash_F(_double TimeDelta)
{
}

void CPlayer_State::Start_Dash_B(_double TimeDelta)
{
}

void CPlayer_State::Start_Dash_L(_double TimeDelta)
{
}

void CPlayer_State::Start_Dash_R(_double TimeDelta)
{
}

void CPlayer_State::Start_Dash_FL(_double TimeDelta)
{
}

void CPlayer_State::Start_Dash_FR(_double TimeDelta)
{
}

void CPlayer_State::Start_Dash_BL(_double TimeDelta)
{
}

void CPlayer_State::Start_Dash_BR(_double TimeDelta)
{
}
#pragma endregion

void CPlayer_State::Tick_Idle(_double TimeDelta)
{
}

void CPlayer_State::Tick_Fire(_double TimeDelta)
{
	// 총알 생성
}

void CPlayer_State::Tick_Reload(_double TimeDelta)
{
	// 플레이어가 들고 있는 총알에서 총에 필요한 탄창만큼 빼기 
}

void CPlayer_State::Tick_Damaged(_double TimeDelta)
{
	// 데미지 입을 때 
}

void CPlayer_State::Tick_Walk_F(_double TimeDelta)
{
	m_pPlayer->m_pTransformCom->Go_Straight(TimeDelta);
}

void CPlayer_State::Tick_Walk_B(_double TimeDelta)
{
	m_pPlayer->m_pTransformCom->Go_Backward(TimeDelta);
}

void CPlayer_State::Tick_Walk_L(_double TimeDelta)
{
	m_pPlayer->m_pTransformCom->Go_Left(TimeDelta);
}

void CPlayer_State::Tick_Walk_R(_double TimeDelta)
{
	m_pPlayer->m_pTransformCom->Go_Right(TimeDelta);
}

void CPlayer_State::Tick_Walk_FL(_double TimeDelta)
{
	m_pPlayer->m_pTransformCom->Go_Straight(TimeDelta * 0.5);
	m_pPlayer->m_pTransformCom->Go_Left(TimeDelta * 0.5);
}

void CPlayer_State::Tick_Walk_FR(_double TimeDelta)
{
	m_pPlayer->m_pTransformCom->Go_Straight(TimeDelta * 0.5);
	m_pPlayer->m_pTransformCom->Go_Right(TimeDelta * 0.5);
}

void CPlayer_State::Tick_Walk_BL(_double TimeDelta)
{
	m_pPlayer->m_pTransformCom->Go_Backward(TimeDelta * 0.5);
	m_pPlayer->m_pTransformCom->Go_Left(TimeDelta * 0.5);
}

void CPlayer_State::Tick_Walk_BR(_double TimeDelta)
{
	m_pPlayer->m_pTransformCom->Go_Backward(TimeDelta * 0.5);
	m_pPlayer->m_pTransformCom->Go_Right(TimeDelta * 0.5);
}

void CPlayer_State::Tick_Dash_F(_double TimeDelta)
{
	// m_pRootPart->pTrans->m_vInfo[INFO_POS] += m_pRootPart->pTrans->m_vInfo[INFO_LOOK] * m_fRollSpeed * fTimeDelta;

	// 시프트 쿨타임을 만들어서 키 입력하면 쿨타임 돌게 해야함 

	// _vector tmp = m_pPlayer->m_pTransformCom->Get_State(CTransform::STATE_TRANSLATION);

	// m_pPlayer->m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, tmp += m_pPlayer->m_pTransformCom->Get_State(CTransform::STATE_LOOK) * 7.f * TimeDelta);

	m_pPlayer->m_pTransformCom->Go_Straight(TimeDelta * 3.0);

}

void CPlayer_State::Tick_Dash_B(_double TimeDelta)
{
	// _vector tmp = m_pPlayer->m_pTransformCom->Get_State(CTransform::STATE_TRANSLATION);

	// m_pPlayer->m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, tmp -= m_pPlayer->m_pTransformCom->Get_State(CTransform::STATE_LOOK) * 7.f * TimeDelta);

	m_pPlayer->m_pTransformCom->Go_Backward(TimeDelta * 3.0);

}

void CPlayer_State::Tick_Dash_L(_double TimeDelta)
{
	// _vector tmp = m_pPlayer->m_pTransformCom->Get_State(CTransform::STATE_TRANSLATION);

	// m_pPlayer->m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, tmp += m_pPlayer->m_pTransformCom->Get_State(CTransform::STATE_LOOK) * 7.f * TimeDelta);

	m_pPlayer->m_pTransformCom->Go_Left(TimeDelta * 3.0);
}

void CPlayer_State::Tick_Dash_R(_double TimeDelta)
{
	m_pPlayer->m_pTransformCom->Go_Right(TimeDelta * 3.0);
}

void CPlayer_State::Tick_Dash_FL(_double TimeDelta)
{
	m_pPlayer->m_pTransformCom->Go_Straight(TimeDelta * 1.5);
	m_pPlayer->m_pTransformCom->Go_Left(TimeDelta * 1.5);
}

void CPlayer_State::Tick_Dash_FR(_double TimeDelta)
{
	m_pPlayer->m_pTransformCom->Go_Straight(TimeDelta * 1.5);
	m_pPlayer->m_pTransformCom->Go_Right(TimeDelta * 1.5);
}

void CPlayer_State::Tick_Dash_BL(_double TimeDelta)
{
	m_pPlayer->m_pTransformCom->Go_Backward(TimeDelta * 1.5);
	m_pPlayer->m_pTransformCom->Go_Left(TimeDelta * 1.5);
}

void CPlayer_State::Tick_Dash_BR(_double TimeDelta)
{
	m_pPlayer->m_pTransformCom->Go_Backward(TimeDelta * 1.5);
	m_pPlayer->m_pTransformCom->Go_Right(TimeDelta * 1.5);
}

void CPlayer_State::End_Common(_double TimeDelta)
{
}

_bool CPlayer_State::KeyInput_None()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	_bool bResult =
		!pGameInstance->Key_Pressing(DIK_W) &&
		!pGameInstance->Key_Pressing(DIK_A) &&
		!pGameInstance->Key_Pressing(DIK_S) &&
		!pGameInstance->Key_Pressing(DIK_D);

	RELEASE_INSTANCE(CGameInstance);

	return bResult;
}

_bool CPlayer_State::KeyInput_R()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	_bool	bResult = pGameInstance->Key_Pressing(DIK_R);

	RELEASE_INSTANCE(CGameInstance);
	return bResult;
}

_bool CPlayer_State::KeyInput_W()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	_bool bResult =
		pGameInstance->Key_Pressing(DIK_W) &&
		!pGameInstance->Key_Pressing(DIK_A) &&
		!pGameInstance->Key_Pressing(DIK_S) &&
		!pGameInstance->Key_Pressing(DIK_D) &&
		!KeyInput_WA() && !KeyInput_WD();

	RELEASE_INSTANCE(CGameInstance);
	return bResult;
}

_bool CPlayer_State::KeyInput_A()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	_bool bResult =
		!pGameInstance->Key_Pressing(DIK_W) &&
		pGameInstance->Key_Pressing(DIK_A) &&
		!pGameInstance->Key_Pressing(DIK_S) &&
		!pGameInstance->Key_Pressing(DIK_D) &&
		!KeyInput_WA() && !KeyInput_SA();

	RELEASE_INSTANCE(CGameInstance);
	return bResult;
}

_bool CPlayer_State::KeyInput_S()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	_bool bResult =
		!pGameInstance->Key_Pressing(DIK_W) &&
		!pGameInstance->Key_Pressing(DIK_A) &&
		pGameInstance->Key_Pressing(DIK_S) &&
		!pGameInstance->Key_Pressing(DIK_D) &&
		!KeyInput_SA() && !KeyInput_SD();

	RELEASE_INSTANCE(CGameInstance);
	return bResult;
}

_bool CPlayer_State::KeyInput_D()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	_bool bResult =
		!pGameInstance->Key_Pressing(DIK_W) &&
		!pGameInstance->Key_Pressing(DIK_A) &&
		!pGameInstance->Key_Pressing(DIK_S) &&
		pGameInstance->Key_Pressing(DIK_D) &&
		!KeyInput_WD() && !KeyInput_SD();

	RELEASE_INSTANCE(CGameInstance);
	return bResult;
}

_bool CPlayer_State::KeyInput_WA()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	_bool	bResult =
		pGameInstance->Key_Pressing(DIK_W) &&
		pGameInstance->Key_Pressing(DIK_A) &&
		!pGameInstance->Key_Pressing(DIK_S) &&
		!pGameInstance->Key_Pressing(DIK_D);

	RELEASE_INSTANCE(CGameInstance);
	return bResult;
}

_bool CPlayer_State::KeyInput_WD()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	_bool	bResult =
		pGameInstance->Key_Pressing(DIK_W) &&
		!pGameInstance->Key_Pressing(DIK_A) &&
		!pGameInstance->Key_Pressing(DIK_S) &&
		pGameInstance->Key_Pressing(DIK_D);

	RELEASE_INSTANCE(CGameInstance);
	return bResult;
}

_bool CPlayer_State::KeyInput_SA()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	_bool	bResult =
		!pGameInstance->Key_Pressing(DIK_W) &&
		pGameInstance->Key_Pressing(DIK_A) &&
		pGameInstance->Key_Pressing(DIK_S) &&
		!pGameInstance->Key_Pressing(DIK_D);

	RELEASE_INSTANCE(CGameInstance);
	return bResult;
}

_bool CPlayer_State::KeyInput_SD()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	_bool	bResult =
		!pGameInstance->Key_Pressing(DIK_W) &&
		!pGameInstance->Key_Pressing(DIK_A) &&
		pGameInstance->Key_Pressing(DIK_S) &&
		pGameInstance->Key_Pressing(DIK_D);

	RELEASE_INSTANCE(CGameInstance);
	return bResult;
}

_bool CPlayer_State::KeyInput_Shift()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	_bool	bResult = pGameInstance->Key_Pressing(DIK_LSHIFT);

	RELEASE_INSTANCE(CGameInstance);
	return bResult;
}

_bool CPlayer_State::KeyUp_Shift()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	_bool	bResult = pGameInstance->Key_Up(DIK_LSHIFT);

	RELEASE_INSTANCE(CGameInstance);
	return bResult;
}

_bool CPlayer_State::KeyInput_Space()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	_bool	bResult = pGameInstance->Key_Pressing(DIK_SPACE);

	RELEASE_INSTANCE(CGameInstance);
	return bResult;
}

_bool CPlayer_State::MouseDown_LB()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	_bool	bResult = pGameInstance->Mouse_Down(DIM_LB);


	RELEASE_INSTANCE(CGameInstance);
	return bResult;
}

_bool CPlayer_State::MouseCharge_LB()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	_bool	bResult = pGameInstance->Get_DIMouseState(DIM_LB);

	RELEASE_INSTANCE(CGameInstance);

	return bResult;
}

_bool CPlayer_State::isFinish()
{
	// _bool bResult = m_pPlayer->m_pModelCom->Get_IsAnimFinished();
	// m_pPlayer->m_pModelCom->Reset_IsFinished();
	//
	// m_pPlayer->m_bIsAnimaFinished = bResult;

	return true;
}

CPlayer_State* CPlayer_State::Create(CPlayer* pPlayer)
{
	CPlayer_State*		pInstance = new CPlayer_State();

	if (FAILED(pInstance->Initialize(pPlayer)))
	{
		MSG_BOX("Failed to Create : CPlayer_State");
		Safe_Release(pInstance);

		return nullptr;
	}

	return pInstance;
}

void CPlayer_State::Free()
{
}
