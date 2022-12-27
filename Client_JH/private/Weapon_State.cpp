#include "stdafx.h"
#include "..\public\Weapon_State.h"
#include "Player.h"
#include "GameInstance.h"
#include "State.h"

CWeapon_State::CWeapon_State()
{
}

HRESULT CWeapon_State::Initialize(CPlayer* pPlayer)
{
	m_pPlayer = pPlayer;
	FAILED_CHECK_RETURN(SetUp_State_Idle(), E_FAIL);
	FAILED_CHECK_RETURN(SetUp_State_Fire(), E_FAIL);
	FAILED_CHECK_RETURN(SetUp_State_Roar(), E_FAIL);
	FAILED_CHECK_RETURN(SetUp_State_Throw(), E_FAIL);

	return S_OK;
}

HRESULT CWeapon_State::SetUp_State_Idle()
{
	NULL_CHECK_RETURN(m_pPlayer->m_pState, E_FAIL);

	m_pPlayer->m_pState->Set_Root(L"STATE::WEAPON_IDLE")

		.Add_State(L"STATE::WEAPON_IDLE")
		.Init_Start(this, &CWeapon_State::Start_Idle)
		.Init_Tick(this, &CWeapon_State::Tick_Idle)
		.Init_End(this, &CWeapon_State::End_Common)
		.Init_Changer(L"STATE::WEAPON_FIRE", this, &CWeapon_State::MouseInput_LB)
		.Init_Changer(L"STATE::WEAPON_RELOAD", this, &CWeapon_State::KeyInput_R)
		.Init_Changer(L"STATE::WEAPON_THROW", this, &CWeapon_State::KeyInput_Q)
		.Init_Changer(L"STATE::WEAPON_ROAR", this, &CWeapon_State::KeyInput_E)

		.Finish_Setting();

	return S_OK;
}



HRESULT CWeapon_State::SetUp_State_Fire()
{
	NULL_CHECK_RETURN(m_pPlayer->m_pState, E_FAIL);

	m_pPlayer->m_pState->
		Add_State(L"STATE::WEAPON_FIRE")
		.Init_Start(this, &CWeapon_State::Start_Fire)
		.Init_Tick(this, &CWeapon_State::Tick_Fire)
		.Init_End(this, &CWeapon_State::End_Common)
		.Init_Changer(L"STATE::WEAPON_RELOAD", this, &CWeapon_State::KeyInput_R)
		.Init_Changer(L"STATE::WEAPON_THROW", this, &CWeapon_State::KeyInput_Q)
		.Init_Changer(L"STATE::WEAPON_ROAR", this, &CWeapon_State::KeyInput_E)
		.Init_Changer(L"STATE::WEAPON_IDLE", this, &CWeapon_State::None_Input)

		.Finish_Setting();

	return S_OK;	
}

HRESULT CWeapon_State::SetUp_state_Reload()
{
	NULL_CHECK_RETURN(m_pPlayer->m_pState, E_FAIL);

	m_pPlayer->m_pState->
		Add_State(L"STATE::WEAPON_RELOAD")
		.Init_Start(this, &CWeapon_State::Start_Reload)
		.Init_Tick(this, &CWeapon_State::Tick_Reload)
		.Init_End(this, &CWeapon_State::End_Common)
		.Init_Changer(L"STATE::WEAPON_FIRE", this, &CWeapon_State::MouseInput_LB)
		.Init_Changer(L"STATE::WEAPON_THROW", this, &CWeapon_State::KeyInput_Q)
		.Init_Changer(L"STATE::WEAPON_ROAR", this, &CWeapon_State::KeyInput_E)
		.Init_Changer(L"STATE::WEAPON_IDLE", this, &CWeapon_State::None_Input)

		.Finish_Setting();

	return S_OK;
}

HRESULT CWeapon_State::SetUp_State_Throw()
{
	NULL_CHECK_RETURN(m_pPlayer->m_pState, E_FAIL);

	m_pPlayer->m_pState->
		Add_State(L"STATE::WEAPON_THROW")
		.Init_Start(this, &CWeapon_State::Start_Throw)
		.Init_Tick(this, &CWeapon_State::Tick_Throw)
		.Init_End(this, &CWeapon_State::End_Common)
		.Init_Changer(L"STATE::WEAPON_FIRE", this, &CWeapon_State::MouseInput_LB)
		.Init_Changer(L"STATE::WEAPON_ROAR", this, &CWeapon_State::KeyInput_E)
		.Init_Changer(L"STATE::WEAPON_RELOAD", this, &CWeapon_State::KeyInput_R)
		.Init_Changer(L"STATE::WEAPON_IDLE", this, &CWeapon_State::None_Input)

		.Finish_Setting();

	return S_OK;
}

HRESULT CWeapon_State::SetUp_State_Roar()
{
	NULL_CHECK_RETURN(m_pPlayer->m_pState, E_FAIL);

	m_pPlayer->m_pState->
		Add_State(L"STATE::WEAPON_ROAR")
		.Init_Start(this, &CWeapon_State::Start_Roar)
		.Init_Tick(this, &CWeapon_State::Tick_Roar)
		.Init_End(this, &CWeapon_State::End_Common)
		.Init_Changer(L"STATE::WEAPON_FIRE", this, &CWeapon_State::MouseInput_LB)
		.Init_Changer(L"STATE::WEAPON_RELOAD", this, &CWeapon_State::KeyInput_R)
		.Init_Changer(L"STATE::WEAPON_THROW", this, &CWeapon_State::KeyInput_Q)
		.Init_Changer(L"STATE::WEAPON_IDLE", this, &CWeapon_State::None_Input)

		.Finish_Setting();

	return S_OK;
}

void CWeapon_State::Start_Idle(_double TimeDelta)
{
	if (m_pPlayer->m_wstrCurWeaponName == L"WEAPON_DEFAULT")
	{
	
		m_pPlayer->m_pModelCom = m_pPlayer->m_tWeaponDesc[CPlayer::WEAPON_DEFAULT].m_pWeaponModelCom;
	
		m_pPlayer->m_pModelCom->Set_CurAnimIndex(DEFAULT_PISTOL_IDLE);
	}
	else if (m_pPlayer->m_wstrCurWeaponName == L"WEAPON_FLAMEBULLET")
	{
		m_pPlayer->m_pModelCom = m_pPlayer->m_tWeaponDesc[CPlayer::WEAPON_FLAMEBULLET].m_pWeaponModelCom;
	
		m_pPlayer->m_pModelCom->Set_CurAnimIndex(FLAME_BULLET_IDLE);
	}
	else if (m_pPlayer->m_wstrCurWeaponName == L"WEAPON_FIREDRAGON")
	{
		m_pPlayer->m_pModelCom = m_pPlayer->m_tWeaponDesc[CPlayer::WEAPON_FIREDRAGON].m_pWeaponModelCom;
	
		m_pPlayer->m_pModelCom->Set_CurAnimIndex(FIRE_DRAGON_IDLE);
	}
	else if (m_pPlayer->m_wstrCurWeaponName == L"WEAPON_POISON")
	{
		m_pPlayer->m_pModelCom = m_pPlayer->m_tWeaponDesc[CPlayer::WEAPON_POISON].m_pWeaponModelCom;
	
		m_pPlayer->m_pModelCom->Set_CurAnimIndex(POISON_IDLE);
	}
}

void CWeapon_State::Start_Fire(_double TimeDelta)
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

void CWeapon_State::Start_Reload(_double TimeDelta)
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

void CWeapon_State::Start_Throw(_double TimeDelta)
{
	// 플레이어 전용 스킬 갖고있는 모델 애니메이션 재생 시키도록 
}

void CWeapon_State::Start_Roar(_double TimeDelta)
{
	// 플레이어 전용 스킬 갖고있는 모델 애니메이션 재생 시키도록 
}

void CWeapon_State::Tick_Idle(_double TimeDelta)
{
}

void CWeapon_State::Tick_Fire(_double TimeDelta)
{
	// 총알 생성 하면될듯
}

void CWeapon_State::Tick_Reload(_double TimeDelta)
{
	// 플레이어가 들고있는 탄약에서 총에 넣을 수 잇는 만큼 옮기기 
}

void CWeapon_State::Tick_Throw(_double TimeDelta)
{
}

void CWeapon_State::Tick_Roar(_double TimeDelta)
{
}

void CWeapon_State::End_Common(_double TimeDelta)
{
}

inline _bool CWeapon_State::None_Input()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	_bool bResult =
		!pGameInstance->Key_Pressing(DIK_Q) &&
		!pGameInstance->Key_Pressing(DIK_R) &&
		!pGameInstance->Key_Pressing(DIK_E) &&
		!pGameInstance->Mouse_Down(DIM_LB);

	RELEASE_INSTANCE(CGameInstance);

	return bResult;
}

_bool CWeapon_State::KeyInput_None()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	_bool bResult =
		!pGameInstance->Key_Pressing(DIK_Q) &&
		!pGameInstance->Key_Pressing(DIK_R) &&
		!pGameInstance->Key_Pressing(DIK_E);

	RELEASE_INSTANCE(CGameInstance);

	return bResult;
}

_bool CWeapon_State::KeyInput_R()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	_bool	bResult = pGameInstance->Key_Pressing(DIK_R);

	RELEASE_INSTANCE(CGameInstance);
	return bResult;
}

_bool CWeapon_State::KeyInput_Q()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	_bool	bResult = pGameInstance->Key_Pressing(DIK_Q);

	RELEASE_INSTANCE(CGameInstance);
	return bResult;
}

_bool CWeapon_State::KeyInput_E()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	_bool	bResult = pGameInstance->Key_Pressing(DIK_E);

	RELEASE_INSTANCE(CGameInstance);
	return bResult;
}

_bool CWeapon_State::MouseInput_None()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	_bool bResult = !pGameInstance->Mouse_Down(DIM_LB);

	RELEASE_INSTANCE(CGameInstance);

	return bResult;
}

_bool CWeapon_State::MouseInput_LB()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	_bool	bResult = pGameInstance->Mouse_Down(DIM_LB);


	RELEASE_INSTANCE(CGameInstance);
	return bResult;
}

_bool CWeapon_State::IsFinish()
{
	// _bool bResult = m_pPlayer->m_pModelCom->Get_IsAnimFinished();
	// m_pPlayer->m_pModelCom->Reset_IsFinished();
	//
	// m_pPlayer->m_bIsAnimaFinished = bResult;
	//
	// return bResult;
	return true;
}

CWeapon_State* CWeapon_State::Create(CPlayer* pPlayer)
{
	CWeapon_State*		pInstance = new CWeapon_State();

	if (FAILED(pInstance->Initialize(pPlayer)))
	{
		MSG_BOX("Failed to Create : CWeapon_State");
		Safe_Release(pInstance);

		return nullptr;
	}

	return pInstance;
}

void CWeapon_State::Free()
{
}

