#include "stdafx.h"
#include "..\public\Weapon_State.h"
#include "Player.h"
#include "GameInstance.h"
#include "State.h"

CWeapon_State::CWeapon_State()
{
	m_tWeaponOption[DEFAULT_PISTOL].wstrWeaponName = L"WEAPON_DEFAULT";
	m_tWeaponOption[FLAME_BULLET].wstrWeaponName = L"WEAPON_FLAMEBULLET";
	m_tWeaponOption[FIRE_DRAGON].wstrWeaponName = L"WEAPON_FIREDRAGON";
	m_tWeaponOption[POISON].wstrWeaponName = L"WEAPON_POISON";
}

HRESULT CWeapon_State::Initialize(class CPlayer* pPlayer, CState* pStateMachineCom, CModel* pModel, CTransform* pTransform, CNavigation* pNavigation)
{
	m_pGameInstance = CGameInstance::GetInstance();
	m_pPlayer = pPlayer;
	m_pState = pStateMachineCom;
	m_pTransformCom = pTransform;
	m_pNavigationCom = pNavigation;
	m_tWeaponOption[DEFAULT_PISTOL].iCurBullet = m_tWeaponOption[DEFAULT_PISTOL].iMaxBullet = 50;
	m_tWeaponOption[DEFAULT_PISTOL].iAttack = 150;

	m_tWeaponOption[FLAME_BULLET].iCurBullet =	m_tWeaponOption[FLAME_BULLET].iMaxBullet = 30;
	m_tWeaponOption[FLAME_BULLET].iAttack = 300;

	m_tWeaponOption[FIRE_DRAGON].iCurBullet = m_tWeaponOption[FIRE_DRAGON].iMaxBullet = 60;
	m_tWeaponOption[FIRE_DRAGON].iAttack = 100;

	m_tWeaponOption[POISON].iCurBullet = m_tWeaponOption[POISON].iMaxBullet = 1;
	m_tWeaponOption[POISON].iAttack = 230;



	FAILED_CHECK_RETURN(SetUp_State_Weapon_Idle(), E_FAIL);
	FAILED_CHECK_RETURN(SetUp_State_Fire(), E_FAIL);
	FAILED_CHECK_RETURN(SetUp_state_Reload(), E_FAIL);
	FAILED_CHECK_RETURN(SetUp_State_Roar(), E_FAIL);
	FAILED_CHECK_RETURN(SetUp_State_Throw(), E_FAIL);

	m_pModelCom = m_pPlayer->m_pModelCom = m_pPlayer->m_tWeaponDesc[CPlayer::WEAPON_DEFAULT].m_pWeaponModelCom;
	m_pPlayer->m_PlayerOption.m_wstrCurWeaponName = m_pPlayer->m_tWeaponDesc[CPlayer::WEAPON_DEFAULT].m_wstrWeaponName;
	m_pPlayer->m_PlayerOption.m_wstrWeaponNumber = L"3";

	return S_OK;
}

void CWeapon_State::Tick(_double dTimeDelta)
{

	// 이동하기
	_float m_fHeight = m_pNavigationCom->Get_CellHeight();
	_float4 PlayerPos = m_pTransformCom->Get_State(CTransform::STATE_TRANSLATION);
	PlayerPos.y = m_fHeight + 1.5f; 
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, PlayerPos);

	if (m_pGameInstance->Get_DIKeyState(DIK_1))
	{
		m_pModelCom = m_pPlayer->m_pModelCom = m_pPlayer->m_tWeaponDesc[CPlayer::WEAPON_DEFAULT].m_pWeaponModelCom;
		m_pPlayer->m_PlayerOption.m_wstrCurWeaponName = L"";
		m_pPlayer->m_PlayerOption.m_wstrCurWeaponName = m_pPlayer->m_tWeaponDesc[CPlayer::WEAPON_DEFAULT].m_wstrWeaponName;
		m_pPlayer->m_PlayerOption.m_wstrWeaponNumber = L"";
		m_pPlayer->m_PlayerOption.m_wstrWeaponNumber = L"1";

	}
	else if (m_pGameInstance->Get_DIKeyState(DIK_2))
	{
		m_pModelCom = m_pPlayer->m_pModelCom = m_pPlayer->m_tWeaponDesc[CPlayer::WEAPON_FLAMEBULLET].m_pWeaponModelCom;
		m_pPlayer->m_PlayerOption.m_wstrCurWeaponName = L"";
		m_pPlayer->m_PlayerOption.m_wstrCurWeaponName = m_pPlayer->m_tWeaponDesc[CPlayer::WEAPON_FLAMEBULLET].m_wstrWeaponName;
		m_pPlayer->m_PlayerOption.m_wstrWeaponNumber = L"";
		m_pPlayer->m_PlayerOption.m_wstrWeaponNumber = L"2";
	}
	else if (m_pGameInstance->Get_DIKeyState(DIK_3))
	{
		m_pModelCom = m_pPlayer->m_pModelCom = m_pPlayer->m_tWeaponDesc[CPlayer::WEAPON_FIREDRAGON].m_pWeaponModelCom;
		m_pPlayer->m_PlayerOption.m_wstrCurWeaponName = L"";
		m_pPlayer->m_PlayerOption.m_wstrCurWeaponName = m_pPlayer->m_tWeaponDesc[CPlayer::WEAPON_FIREDRAGON].m_wstrWeaponName;
		m_pPlayer->m_PlayerOption.m_wstrWeaponNumber = L"";
		m_pPlayer->m_PlayerOption.m_wstrWeaponNumber = L"3";
	}
	else if (m_pGameInstance->Get_DIKeyState(DIK_4))
	{
		m_pModelCom = m_pPlayer->m_pModelCom = m_pPlayer->m_tWeaponDesc[CPlayer::WEAPON_POISON].m_pWeaponModelCom;
		m_pPlayer->m_PlayerOption.m_wstrCurWeaponName = L"";
		m_pPlayer->m_PlayerOption.m_wstrCurWeaponName = m_pPlayer->m_tWeaponDesc[CPlayer::WEAPON_POISON].m_wstrWeaponName;
	}

	if(m_pGameInstance->Get_DIKeyState(DIK_W))
	{
		m_pTransformCom->Go_Straight(dTimeDelta, m_pNavigationCom);
	}

	if (m_pGameInstance->Get_DIKeyState(DIK_S))
	{
		m_pTransformCom->Go_Backward(dTimeDelta);
	}
	if (m_pGameInstance->Get_DIKeyState(DIK_A))
	{
		m_pTransformCom->Go_Left(dTimeDelta);
	}
	if (m_pGameInstance->Get_DIKeyState(DIK_D))
	{
		m_pTransformCom->Go_Right(dTimeDelta);
	}
}

void CWeapon_State::Late_Tick(_double dTimeDelta)
{
}





HRESULT CWeapon_State::SetUp_State_Weapon_Idle()
{
	NULL_CHECK_RETURN(m_pPlayer->m_pState, E_FAIL);

	m_pPlayer->m_pState->Set_Root(L"STATE::WEAPON_IDLE")
		.Add_State(L"STATE::WEAPON_IDLE")
		.Init_Start(this, &CWeapon_State::Start_Weapon_Idle)
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
		.Init_Changer(L"STATE::WEAPON_THROW", this, &CWeapon_State::KeyInput_Q)
		.Init_Changer(L"STATE::WEAPON_ROAR", this, &CWeapon_State::KeyInput_E)
		.Init_Changer(L"STATE::WEAPON_IDLE", this, &CWeapon_State::Animation_Finish)

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
		.Init_End(this, &CWeapon_State::End_Reload)
		.Init_Changer(L"STATE::WEAPON_IDLE", this, &CWeapon_State::Animation_Finish)

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
		.Init_Changer(L"STATE::WEAPON_IDLE", this, &CWeapon_State::Animation_Finish)

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
		.Init_Changer(L"STATE::WEAPON_THROW", this, &CWeapon_State::KeyInput_Q)
		.Init_Changer(L"STATE::WEAPON_IDLE", this, &CWeapon_State::Animation_Finish)

		.Finish_Setting();

	return S_OK;
}

void CWeapon_State::Start_Weapon_Idle(_double TimeDelta)
{
	if (m_pPlayer->m_wstrCurWeaponName == m_tWeaponOption[DEFAULT_PISTOL].wstrWeaponName)
	{
		m_pModelCom->Set_CurAnimIndex(DEFAULT_PISTOL_IDLE);
	}

	else if (m_pPlayer->m_wstrCurWeaponName == m_tWeaponOption[FLAME_BULLET].wstrWeaponName)
	{
		m_pModelCom->Set_CurAnimIndex(FLAME_BULLET_IDLE);
	}

	else if (m_pPlayer->m_wstrCurWeaponName == m_tWeaponOption[FIRE_DRAGON].wstrWeaponName)
	{
		m_pModelCom->Set_CurAnimIndex(FIRE_DRAGON_IDLE);
	}

	else if (m_pPlayer->m_wstrCurWeaponName == m_tWeaponOption[POISON].wstrWeaponName)
	{
		m_pModelCom->Set_CurAnimIndex(POISON_IDLE);
	}
}

void CWeapon_State::Start_Fire(_double TimeDelta)
{
	if (m_pPlayer->m_wstrCurWeaponName == m_tWeaponOption[DEFAULT_PISTOL].wstrWeaponName)
	{
		m_pModelCom->Set_CurAnimIndex(DEFAULT_PISTOL_FIRE);
		m_tWeaponOption[DEFAULT_PISTOL].iCurBullet -= 1;
	}
	else if (m_pPlayer->m_wstrCurWeaponName == m_tWeaponOption[FLAME_BULLET].wstrWeaponName)
	{
		m_pModelCom->Set_CurAnimIndex(FLAME_BULLET_FIRE);
		m_tWeaponOption[FLAME_BULLET].iCurBullet -= 1;
	}
	else if (m_pPlayer->m_wstrCurWeaponName == m_tWeaponOption[FIRE_DRAGON].wstrWeaponName)
	{
		m_pModelCom->Set_CurAnimIndex(FIRE_DRAGON_FIRE);
		m_tWeaponOption[FIRE_DRAGON].iCurBullet -= 1;

	}
	else if (m_pPlayer->m_wstrCurWeaponName == m_tWeaponOption[POISON].wstrWeaponName)
	{
		if (m_pPlayer->m_iPoisonAttCnt == POISON_IDLE)
			m_pPlayer->m_iPoisonAttCnt = 0;

		m_pModelCom->Set_CurAnimIndex(POISON_FIRE_A + m_pPlayer->m_iPoisonAttCnt);
		m_tWeaponOption[POISON].iCurBullet -= 1;

		m_pPlayer->m_iPoisonAttCnt++;
	}
}

void CWeapon_State::Start_Reload(_double TimeDelta)
{
	if (m_pPlayer->m_wstrCurWeaponName == m_tWeaponOption[DEFAULT_PISTOL].wstrWeaponName)
	{
		m_pModelCom->Set_CurAnimIndex(DEFAULT_PISTOL_RELOAD);
	}
	else if (m_pPlayer->m_wstrCurWeaponName == m_tWeaponOption[FLAME_BULLET].wstrWeaponName)
	{
		m_pModelCom->Set_CurAnimIndex(FLAME_BULLET_RELOAD);
	}
	else if (m_pPlayer->m_wstrCurWeaponName == m_tWeaponOption[FIRE_DRAGON].wstrWeaponName)
	{
		m_pModelCom->Set_CurAnimIndex(FIRE_DRAGON_RELOAD);
	}
	else if (m_pPlayer->m_wstrCurWeaponName == m_tWeaponOption[POISON].wstrWeaponName)
		m_pModelCom->Set_CurAnimIndex(POISON_IDLE);
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
	if (m_pPlayer->m_wstrCurWeaponName == m_tWeaponOption[DEFAULT_PISTOL].wstrWeaponName)
	{
		m_pModelCom->Set_CurAnimIndex(DEFAULT_PISTOL_IDLE);
	}
	else if (m_pPlayer->m_wstrCurWeaponName == m_tWeaponOption[FLAME_BULLET].wstrWeaponName)
	{
		m_pModelCom->Set_CurAnimIndex(FLAME_BULLET_IDLE);
	}
	else if (m_pPlayer->m_wstrCurWeaponName == m_tWeaponOption[FIRE_DRAGON].wstrWeaponName)
	{
		m_pModelCom->Set_CurAnimIndex(FIRE_DRAGON_IDLE);
	}
	else if (m_pPlayer->m_wstrCurWeaponName == m_tWeaponOption[POISON].wstrWeaponName)
	{
		if(m_tWeaponOption[POISON].iCurBullet == 0 && m_pPlayer->m_PlayerOption.m_iPistol_BulletCnt != 0)
		{
			_int BulletCnt = m_pPlayer->m_PlayerOption.m_iPistol_BulletCnt;
			_int NeedBulletCnt = m_tWeaponOption[POISON].iMaxBullet - m_tWeaponOption[POISON].iCurBullet;
		
			if (BulletCnt > NeedBulletCnt)
			{
				BulletCnt -= NeedBulletCnt;
				m_tWeaponOption[POISON].iCurBullet = m_tWeaponOption[POISON].iMaxBullet;
				m_pPlayer->m_PlayerOption.m_iPistol_BulletCnt = BulletCnt;
			}
			else
			{
				m_tWeaponOption[POISON].iCurBullet += BulletCnt;
				m_pPlayer->m_PlayerOption.m_iPistol_BulletCnt = 0;
			}
		}
		m_pModelCom->Set_CurAnimIndex(POISON_IDLE);
	}
}


void CWeapon_State::Tick_Fire(_double TimeDelta)
{
	// 총알 생성 하면될듯
}

void CWeapon_State::Tick_Reload(_double TimeDelta)
{
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

void CWeapon_State::End_Reload(_double TimeDelta)
{
	if (m_pPlayer->m_wstrCurWeaponName == m_tWeaponOption[DEFAULT_PISTOL].wstrWeaponName)
	{
		if (0 != m_pPlayer->m_PlayerOption.m_iPistol_BulletCnt)
		{
			_int BulletCnt = m_pPlayer->m_PlayerOption.m_iPistol_BulletCnt;
			_int NeedBulletCnt = m_tWeaponOption[DEFAULT_PISTOL].iMaxBullet - m_tWeaponOption[DEFAULT_PISTOL].iCurBullet;

			if (BulletCnt > NeedBulletCnt)
			{
				BulletCnt -= NeedBulletCnt;
				m_tWeaponOption[DEFAULT_PISTOL].iCurBullet = m_tWeaponOption[DEFAULT_PISTOL].iMaxBullet;
				m_pPlayer->m_PlayerOption.m_iPistol_BulletCnt = BulletCnt;
			}
			else
			{
				m_tWeaponOption[DEFAULT_PISTOL].iCurBullet += BulletCnt;
				m_pPlayer->m_PlayerOption.m_iPistol_BulletCnt = 0;
			}
		}
	}
	else if (m_pPlayer->m_wstrCurWeaponName == m_tWeaponOption[FLAME_BULLET].wstrWeaponName)
	{
		if (0 != m_pPlayer->m_PlayerOption.m_iPistol_BulletCnt)
		{
			_int BulletCnt = m_pPlayer->m_PlayerOption.m_iPistol_BulletCnt;
			_int NeedBulletCnt = m_tWeaponOption[FLAME_BULLET].iMaxBullet - m_tWeaponOption[FLAME_BULLET].iCurBullet;

			if (BulletCnt > NeedBulletCnt)
			{
				BulletCnt -= NeedBulletCnt;
				m_tWeaponOption[FLAME_BULLET].iCurBullet = m_tWeaponOption[FLAME_BULLET].iMaxBullet;
				m_pPlayer->m_PlayerOption.m_iPistol_BulletCnt = BulletCnt;
			}
			else
			{
				m_tWeaponOption[FLAME_BULLET].iCurBullet += BulletCnt;
				m_pPlayer->m_PlayerOption.m_iPistol_BulletCnt = 0;
			}
		}
	}
	else if (m_pPlayer->m_wstrCurWeaponName == m_tWeaponOption[FIRE_DRAGON].wstrWeaponName)
	{
		m_pModelCom->Set_CurAnimIndex(FIRE_DRAGON_IDLE);
	}
	else if (m_pPlayer->m_wstrCurWeaponName == m_tWeaponOption[POISON].wstrWeaponName)
	{
		if (0 != m_pPlayer->m_PlayerOption.m_iPistol_BulletCnt)
		{
			_int BulletCnt = m_pPlayer->m_PlayerOption.m_iPistol_BulletCnt;
			_int NeedBulletCnt = m_tWeaponOption[POISON].iMaxBullet - m_tWeaponOption[POISON].iCurBullet;

			if (BulletCnt > NeedBulletCnt)
			{
				BulletCnt -= NeedBulletCnt;
				m_tWeaponOption[POISON].iCurBullet = m_tWeaponOption[POISON].iMaxBullet;
				m_pPlayer->m_PlayerOption.m_iPistol_BulletCnt = BulletCnt;
			}
			else
			{
				m_tWeaponOption[POISON].iCurBullet += BulletCnt;
				m_pPlayer->m_PlayerOption.m_iPistol_BulletCnt = 0;
			}
		}
	}
}

_bool CWeapon_State::None_Input()
{
	if (!KeyInput_Q() && !KeyInput_E() && !KeyInput_R() && !MouseInput_LB())
		return true;

	return false;
}

_bool CWeapon_State::KeyInput_None()
{
	_bool bResult =
		!m_pGameInstance->Key_Down(DIK_Q) &&
		!m_pGameInstance->Key_Down(DIK_R) &&
		!m_pGameInstance->Key_Down(DIK_E);

	return bResult;
}

_bool CWeapon_State::KeyInput_R()
{
	if (m_pGameInstance->Key_Down(DIK_R))
		return true;

	return false;
}

_bool CWeapon_State::KeyInput_Q()
{
	if (m_pGameInstance->Key_Down(DIK_Q) && !KeyInput_R() && !KeyInput_E())
		return true;

	return false;
}

_bool CWeapon_State::KeyInput_E()
{
	if (m_pGameInstance->Key_Down(DIK_E) && !KeyInput_R() && !KeyInput_Q())
		return true;

	return false;
}

_bool CWeapon_State::MouseInput_None()
{
	if (!MouseInput_LB())
		return true;

	return false;
}

_bool CWeapon_State::MouseInput_LB()
{
	if (m_pGameInstance->Mouse_Down(DIM_LB))
		return true;

	return false;
}

_bool CWeapon_State::Animation_Finish()
{
	return m_pModelCom->Get_AnimationFinish();
}

CWeapon_State* CWeapon_State::Create(CPlayer* pPlayer, CState* pStateMachineCom, CModel * pModel, CTransform * pTransform, CNavigation* pNavigation)
{
	CWeapon_State*		pInstance = new CWeapon_State();
	if (FAILED(pInstance->Initialize(pPlayer, pStateMachineCom, pModel, pTransform, pNavigation)))
	{
		MSG_BOX("Failed to Create : CWeapon_State");
		Safe_Release(pInstance);

		return nullptr;
	}

	return pInstance;
}

void CWeapon_State::Free()
{
	Safe_Release(m_pGameInstance);
}

