#include "stdafx.h"
#include "..\public\Player_State.h"
#include "Player.h"
#include "GameInstance.h"

CPlayer_State::CPlayer_State()
{
}

HRESULT CPlayer_State::Initialize(CPlayer* pPlayer)
{
	m_pPlayer = pPlayer;

	FAILED_CHECK_RETURN(SetUp_State_Idle(), E_FAIL);
	FAILED_CHECK_RETURN(SetUp_State_Walk(), E_FAIL);
	FAILED_CHECK_RETURN(SetUp_State_Fire(), E_FAIL);
	FAILED_CHECK_RETURN(SetUp_State_Reload(), E_FAIL);
	FAILED_CHECK_RETURN(SetUp_State_Dash(), E_FAIL);
	FAILED_CHECK_RETURN(SetUp_State_Damaged(), E_FAIL);

	return S_OK;
}
#pragma region SetUp_State

HRESULT CPlayer_State::SetUp_State_Idle()
{
}

HRESULT CPlayer_State::SetUp_State_Walk()
{
}

HRESULT CPlayer_State::SetUp_State_Dash()
{
}

HRESULT CPlayer_State::SetUp_State_Fire()
{
}

HRESULT CPlayer_State::SetUp_State_Reload()
{
}

HRESULT CPlayer_State::SetUp_State_Damaged()
{
}

HRESULT CPlayer_State::SetUp_State_SelectPage()
{
}

#pragma endregion


void CPlayer_State::Start_Idle(_double TimeDelta)
{
	if (m_pPlayer->m_wstrCurWeaponName == L"WEAPON_DEFAULT")
	{
		m_pPlayer->m_pModelCom->Set_CurAnimIndex(DEFAULT_PISTOL_IDLE);
	}
	else if (m_pPlayer->m_wstrCurWeaponName == L"WEAPON_FLAMEBULLET")
	{
		m_pPlayer->m_pModelCom->Set_CurAnimIndex(FLAME_BULLET_IDLE);
	}
	else if (m_pPlayer->m_wstrCurWeaponName == L"WEAPON_FIREDRAGON")
	{
		m_pPlayer->m_pModelCom->Set_CurAnimIndex(FIRE_DRAGON_IDLE);
	}
	else if (m_pPlayer->m_wstrCurWeaponName == L"WEAPON_POISON")
		m_pPlayer->m_pModelCom->Set_CurAnimIndex(POISON_IDLE);
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

	m_pPlayer->m_pTransformCom->Go_Straight(TimeDelta * 7.0);

}

void CPlayer_State::Tick_Dash_B(_double TimeDelta)
{
	// _vector tmp = m_pPlayer->m_pTransformCom->Get_State(CTransform::STATE_TRANSLATION);

	// m_pPlayer->m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, tmp -= m_pPlayer->m_pTransformCom->Get_State(CTransform::STATE_LOOK) * 7.f * TimeDelta);

	m_pPlayer->m_pTransformCom->Go_Backward(TimeDelta * 7.0);

}

void CPlayer_State::Tick_Dash_L(_double TimeDelta)
{
	// _vector tmp = m_pPlayer->m_pTransformCom->Get_State(CTransform::STATE_TRANSLATION);

	// m_pPlayer->m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, tmp += m_pPlayer->m_pTransformCom->Get_State(CTransform::STATE_LOOK) * 7.f * TimeDelta);

	m_pPlayer->m_pTransformCom->Go_Left(TimeDelta * 7.0);
}

void CPlayer_State::Tick_Dash_R(_double TimeDelta)
{
	m_pPlayer->m_pTransformCom->Go_Right(TimeDelta * 7.0);
}

void CPlayer_State::Tick_Dash_FL(_double TimeDelta)
{
	m_pPlayer->m_pTransformCom->Go_Straight(TimeDelta * 3.5);
	m_pPlayer->m_pTransformCom->Go_Left(TimeDelta * 3.5);
}

void CPlayer_State::Tick_Dash_FR(_double TimeDelta)
{
	m_pPlayer->m_pTransformCom->Go_Straight(TimeDelta * 3.5);
	m_pPlayer->m_pTransformCom->Go_Right(TimeDelta * 3.5);
}

void CPlayer_State::Tick_Dash_BL(_double TimeDelta)
{
	m_pPlayer->m_pTransformCom->Go_Backward(TimeDelta * 3.5);
	m_pPlayer->m_pTransformCom->Go_Left(TimeDelta * 3.5);
}

void CPlayer_State::Tick_Dash_BR(_double TimeDelta)
{
	m_pPlayer->m_pTransformCom->Go_Backward(TimeDelta * 3.5);
	m_pPlayer->m_pTransformCom->Go_Right(TimeDelta * 3.5);
}

void CPlayer_State::End_Common(_double TimeDelta)
{
}

_bool CPlayer_State::KeyInput_None()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	_bool bResult =
		!pGameInstance->Get_DIKeyState(DIK_W) &&
		!pGameInstance->Get_DIKeyState(DIK_A) &&
		!pGameInstance->Get_DIKeyState(DIK_S) &&
		!pGameInstance->Get_DIKeyState(DIK_D);

	RELEASE_INSTANCE(CGameInstance);

	return bResult;
}

_bool CPlayer_State::KeyInput_W()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	_bool bResult =
		pGameInstance->Get_DIKeyState(DIK_W) &&
		!pGameInstance->Get_DIKeyState(DIK_A) &&
		!pGameInstance->Get_DIKeyState(DIK_S) &&
		!pGameInstance->Get_DIKeyState(DIK_D) &&
		!KeyInput_WA() && !KeyInput_WD();

	RELEASE_INSTANCE(CGameInstance);
	return bResult;
}

_bool CPlayer_State::KeyInput_A()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	_bool bResult =
		!pGameInstance->Get_DIKeyState(DIK_W) &&
		pGameInstance->Get_DIKeyState(DIK_A) &&
		!pGameInstance->Get_DIKeyState(DIK_S) &&
		!pGameInstance->Get_DIKeyState(DIK_D) &&
		!KeyInput_WA() && !KeyInput_SA();

	RELEASE_INSTANCE(CGameInstance);
	return bResult;
}

_bool CPlayer_State::KeyInput_S()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	_bool bResult =
		!pGameInstance->Get_DIKeyState(DIK_W) &&
		!pGameInstance->Get_DIKeyState(DIK_A) &&
		pGameInstance->Get_DIKeyState(DIK_S) &&
		!pGameInstance->Get_DIKeyState(DIK_D) &&
		!KeyInput_SA() && !KeyInput_SD();

	RELEASE_INSTANCE(CGameInstance);
	return bResult;
}

_bool CPlayer_State::KeyInput_D()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	_bool bResult =
		!pGameInstance->Get_DIKeyState(DIK_W) &&
		!pGameInstance->Get_DIKeyState(DIK_A) &&
		!pGameInstance->Get_DIKeyState(DIK_S) &&
		pGameInstance->Get_DIKeyState(DIK_D) &&
		!KeyInput_WD() && !KeyInput_SD();

	RELEASE_INSTANCE(CGameInstance);
	return bResult;
}

_bool CPlayer_State::KeyInput_WA()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	_bool	bResult =
		pGameInstance->Get_DIKeyState(DIK_W) &&
		pGameInstance->Get_DIKeyState(DIK_A) &&
		!pGameInstance->Get_DIKeyState(DIK_S) &&
		!pGameInstance->Get_DIKeyState(DIK_D);

	RELEASE_INSTANCE(CGameInstance);
	return bResult;
}

_bool CPlayer_State::KeyInput_WD()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	_bool	bResult =
		pGameInstance->Get_DIKeyState(DIK_W) &&
		!pGameInstance->Get_DIKeyState(DIK_A) &&
		!pGameInstance->Get_DIKeyState(DIK_S) &&
		pGameInstance->Get_DIKeyState(DIK_D);

	RELEASE_INSTANCE(CGameInstance);
	return bResult;
}

_bool CPlayer_State::KeyInput_SA()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	_bool	bResult =
		!pGameInstance->Get_DIKeyState(DIK_W) &&
		pGameInstance->Get_DIKeyState(DIK_A) &&
		pGameInstance->Get_DIKeyState(DIK_S) &&
		!pGameInstance->Get_DIKeyState(DIK_D);

	RELEASE_INSTANCE(CGameInstance);
	return bResult;
}

_bool CPlayer_State::KeyInput_SD()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	_bool	bResult =
		!pGameInstance->Get_DIKeyState(DIK_W) &&
		!pGameInstance->Get_DIKeyState(DIK_A) &&
		pGameInstance->Get_DIKeyState(DIK_S) &&
		pGameInstance->Get_DIKeyState(DIK_D);

	RELEASE_INSTANCE(CGameInstance);
	return bResult;
}

_bool CPlayer_State::KeyInput_Shift()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	_bool	bResult = pGameInstance->Get_DIKeyState(DIK_LSHIFT);

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


	RELEASE_INSTANCE(CGameInstance);
	return bResult;
}

_bool CPlayer_State::MouseDown_LB()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);


	RELEASE_INSTANCE(CGameInstance);
	return bResult;
}

_bool CPlayer_State::MouseCharge_LB()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);


	RELEASE_INSTANCE(CGameInstance);
	return bResult;
}

_bool CPlayer_State::isFinish()
{
}

CPlayer_State* CPlayer_State::Create(CPlayer* pPlayer)
{
}

void CPlayer_State::Free()
{
}
