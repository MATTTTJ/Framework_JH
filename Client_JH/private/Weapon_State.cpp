#include "stdafx.h"
#include "..\public\Weapon_State.h"
#include "GameInstance.h"
#include "Bullet.h"
#include "Player.h"


CWeapon_State::CWeapon_State()
{
	m_tWeaponOption[DEFAULT_PISTOL].wstrWeaponName = L"WEAPON_DEFAULT";
	m_tWeaponOption[FLAME_BULLET].wstrWeaponName = L"WEAPON_FLAMEBULLET";
	m_tWeaponOption[FIRE_DRAGON].wstrWeaponName = L"WEAPON_FIREDRAGON";
	m_tWeaponOption[POISON].wstrWeaponName = L"WEAPON_POISON";
}

const _uint& CWeapon_State::Get_CurWeaponBulletCnt(wstring WeaponName)
{
	for (_int i = 0; i < WEAPONTYPE_END; ++i)
	{
		if (WeaponName == m_tWeaponOption[i].wstrWeaponName)
			return m_tWeaponOption[i].iCurBullet;
	}

	return 9999;
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

	m_tWeaponOption[FLAME_BULLET].iCurBullet = m_tWeaponOption[FLAME_BULLET].iMaxBullet = 30;
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



	for (auto iter = m_vecBullet.begin(); iter != m_vecBullet.end();)
	{
		if (CGameInstance::GetInstance()->Key_Down(DIK_F9) && !m_bDeadOnce)
		{
			m_vecBullet.front()->Set_Dead(true);
			m_bDeadOnce = true;
		}

		if (true == (*iter)->Check_Dead())
		{
			(*iter)->Set_Dead(false);

			Safe_Release(*iter);
			iter = m_vecBullet.erase(iter);
		}
		else
		{
			++iter;
		}
	}

	m_bDeadOnce = false;


	for (_uint i = 0; i < m_vecBullet.size(); ++i)
	{
		m_vecBullet[i]->Tick(dTimeDelta);
	}

	// 벡터의 프론트를 지우는 게 맞다 .

}

void CWeapon_State::Late_Tick(_double dTimeDelta)
{
	for (_uint i = 0; i < m_vecBullet.size(); ++i)
	{
		m_vecBullet[i]->Late_Tick(dTimeDelta);
	}
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
	if (m_pPlayer->m_PlayerOption.m_wstrCurWeaponName == m_tWeaponOption[DEFAULT_PISTOL].wstrWeaponName)
	{
		m_pModelCom->Set_CurAnimIndex(DEFAULT_PISTOL_IDLE);
	}

	else if (m_pPlayer->m_PlayerOption.m_wstrCurWeaponName == m_tWeaponOption[FLAME_BULLET].wstrWeaponName)
	{
		m_pModelCom->Set_CurAnimIndex(FLAME_BULLET_IDLE);
	}

	else if (m_pPlayer->m_PlayerOption.m_wstrCurWeaponName == m_tWeaponOption[FIRE_DRAGON].wstrWeaponName)
	{
		m_pModelCom->Set_CurAnimIndex(FIRE_DRAGON_IDLE);
	}

	else if (m_pPlayer->m_PlayerOption.m_wstrCurWeaponName == m_tWeaponOption[POISON].wstrWeaponName)
	{
		m_pModelCom->Set_CurAnimIndex(POISON_IDLE);
	}
}

void CWeapon_State::Start_Fire(_double TimeDelta)
{
	if (m_pPlayer->m_PlayerOption.m_wstrCurWeaponName == m_tWeaponOption[DEFAULT_PISTOL].wstrWeaponName &&
		m_pPlayer->Get_PistolBulletCnt() > 1)
	{
		m_pModelCom->Set_CurAnimIndex(DEFAULT_PISTOL_FIRE);
		m_tWeaponOption[DEFAULT_PISTOL].iCurBullet -= 1;

		// TODO:: 카메라 룩 설정할 수 있게 작업하기 

		if (nullptr != m_pPlayer->Collision_AimBox_To_Monster())
		{
			if (nullptr == m_pPlayer->m_pFirstAimColliderCom)
				return;

			_float4x4 fmatrix = m_pPlayer->Collision_AimBox_To_Monster()->Get_WorldFloat4x4();

			_vector MonsterPos = fmatrix.Translation();

			_vector FirstAimSpherePos = m_pPlayer->m_pFirstAimColliderCom->Get_SphereCenter();

			_vector SecondAimSpherePos = m_pPlayer->m_pSecondAimColliderCom->Get_SphereCenter();

			_float MonsterToFirstAim, MonsterToSecondAim;

			XMStoreFloat(&MonsterToFirstAim, XMVector3Length(MonsterPos - FirstAimSpherePos));
			XMStoreFloat(&MonsterToSecondAim, XMVector3Length(MonsterPos - SecondAimSpherePos));

			if (fabsf(MonsterToFirstAim) - fabsf(MonsterToSecondAim) < EPSILON)
			{
				CGameObject::GAMEOBJECTDESC		tmp;
				ZeroMemory(&tmp, sizeof(CGameObject::GAMEOBJECTDESC));

				_float4 Position;
				XMStoreFloat4(&Position, (m_pPlayer->Get_CurWeaponModelCom()->Get_BoneMatrix("Att") * CGameUtils::Get_PlayerPivotMatrix() *m_pPlayer->m_pTransformCom->Get_WorldMatrix()).r[3]);

				tmp.TransformDesc.vInitPos = _float3(Position.x, Position.y, Position.z);
				tmp.m_vBulletLook = XMVector4Normalize(m_pPlayer->m_pFirstAimColliderCom->Get_SphereCenter());
				CGameObject*		pBullet = nullptr;
				pBullet = CGameInstance::GetInstance()->Clone_GameObject(L"Prototype_GameObject_Player_Default_PistolTex", &tmp);
				NULL_CHECK_RETURN(pBullet, );

				m_vecBullet.push_back(pBullet);
			}
			else
			{
				CGameObject::GAMEOBJECTDESC		tmp;
				ZeroMemory(&tmp, sizeof(CGameObject::GAMEOBJECTDESC));
			
				_float4 Position;
				XMStoreFloat4(&Position, (m_pPlayer->Get_CurWeaponModelCom()->Get_BoneMatrix("Att") * CGameUtils::Get_PlayerPivotMatrix() *m_pPlayer->m_pTransformCom->Get_WorldMatrix()).r[3]);

				tmp.TransformDesc.vInitPos = _float3(Position.x, Position.y, Position.z);
				tmp.m_vBulletLook = XMVector4Normalize(m_pPlayer->m_pSecondAimColliderCom->Get_SphereCenter());
				CGameObject*		pBullet = nullptr;
				pBullet = CGameInstance::GetInstance()->Clone_GameObject(L"Prototype_GameObject_Player_Default_PistolTex", &tmp);
				NULL_CHECK_RETURN(pBullet, );

				m_vecBullet.push_back(pBullet);
			}
		}
		else
		{
			CGameObject::GAMEOBJECTDESC		tmp;
			ZeroMemory(&tmp, sizeof(CGameObject::GAMEOBJECTDESC));

			_float4 Position;
			XMStoreFloat4(&Position, (m_pPlayer->Get_CurWeaponModelCom()->Get_BoneMatrix("Att") * CGameUtils::Get_PlayerPivotMatrix() *m_pPlayer->m_pTransformCom->Get_WorldMatrix()).r[3]);

			tmp.TransformDesc.vInitPos = _float3(Position.x, Position.y, Position.z);
			tmp.m_vBulletLook = XMVector3Normalize(m_pGameInstance->Get_CamLook());
			CGameObject*		pBullet = nullptr;
			pBullet = CGameInstance::GetInstance()->Clone_GameObject(L"Prototype_GameObject_Player_Default_PistolTex", &tmp);
			NULL_CHECK_RETURN(pBullet, );

			m_vecBullet.push_back(pBullet);
		}
	}
	else if (m_pPlayer->m_PlayerOption.m_wstrCurWeaponName == m_tWeaponOption[FLAME_BULLET].wstrWeaponName)
	{
		m_pModelCom->Set_CurAnimIndex(FLAME_BULLET_FIRE);
		m_tWeaponOption[FLAME_BULLET].iCurBullet -= 1;
	}
	else if (m_pPlayer->m_PlayerOption.m_wstrCurWeaponName == m_tWeaponOption[FIRE_DRAGON].wstrWeaponName)
	{
		m_pModelCom->Set_CurAnimIndex(FIRE_DRAGON_FIRE);
		m_tWeaponOption[FIRE_DRAGON].iCurBullet -= 1;

	}
	else if (m_pPlayer->m_PlayerOption.m_wstrCurWeaponName == m_tWeaponOption[POISON].wstrWeaponName)
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
	if (m_pPlayer->m_PlayerOption.m_wstrCurWeaponName == m_tWeaponOption[DEFAULT_PISTOL].wstrWeaponName)
	{
		m_pModelCom->Set_CurAnimIndex(DEFAULT_PISTOL_RELOAD);
	}
	else if (m_pPlayer->m_PlayerOption.m_wstrCurWeaponName == m_tWeaponOption[FLAME_BULLET].wstrWeaponName)
	{
		m_pModelCom->Set_CurAnimIndex(FLAME_BULLET_RELOAD);
	}
	else if (m_pPlayer->m_PlayerOption.m_wstrCurWeaponName == m_tWeaponOption[FIRE_DRAGON].wstrWeaponName)
	{
		m_pModelCom->Set_CurAnimIndex(FIRE_DRAGON_RELOAD);
	}
	else if (m_pPlayer->m_PlayerOption.m_wstrCurWeaponName == m_tWeaponOption[POISON].wstrWeaponName)
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
	if (m_pPlayer->m_PlayerOption.m_wstrCurWeaponName == m_tWeaponOption[DEFAULT_PISTOL].wstrWeaponName)
	{
		m_pModelCom->Set_CurAnimIndex(DEFAULT_PISTOL_IDLE);
	}
	else if (m_pPlayer->m_PlayerOption.m_wstrCurWeaponName == m_tWeaponOption[FLAME_BULLET].wstrWeaponName)
	{
		m_pModelCom->Set_CurAnimIndex(FLAME_BULLET_IDLE);
	}
	else if (m_pPlayer->m_PlayerOption.m_wstrCurWeaponName == m_tWeaponOption[FIRE_DRAGON].wstrWeaponName)
	{
		m_pModelCom->Set_CurAnimIndex(FIRE_DRAGON_IDLE);
	}
	else if (m_pPlayer->m_PlayerOption.m_wstrCurWeaponName == m_tWeaponOption[POISON].wstrWeaponName)
	{
		if (m_tWeaponOption[POISON].iCurBullet == 0 && m_pPlayer->m_PlayerOption.m_iPistol_BulletCnt != 0)
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
	if (m_pPlayer->m_PlayerOption.m_wstrCurWeaponName == m_tWeaponOption[DEFAULT_PISTOL].wstrWeaponName)
	{
		if (0 < m_pPlayer->m_PlayerOption.m_iPistol_BulletCnt)
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
	else if (m_pPlayer->m_PlayerOption.m_wstrCurWeaponName == m_tWeaponOption[FLAME_BULLET].wstrWeaponName)
	{
		if (0 < m_pPlayer->m_PlayerOption.m_iPistol_BulletCnt)
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
	else if (m_pPlayer->m_PlayerOption.m_wstrCurWeaponName == m_tWeaponOption[FIRE_DRAGON].wstrWeaponName)
	{
		if (0 < m_pPlayer->m_PlayerOption.m_iInjector_BulletCnt)
		{
			_int BulletCnt = m_pPlayer->m_PlayerOption.m_iInjector_BulletCnt;
			_int NeedBulletCnt = m_tWeaponOption[FIRE_DRAGON].iMaxBullet - m_tWeaponOption[FIRE_DRAGON].iCurBullet;

			if (BulletCnt > NeedBulletCnt)
			{
				BulletCnt -= NeedBulletCnt;
				m_tWeaponOption[FIRE_DRAGON].iCurBullet = m_tWeaponOption[FIRE_DRAGON].iMaxBullet;
				m_pPlayer->m_PlayerOption.m_iInjector_BulletCnt = BulletCnt;
			}
			else
			{
				m_tWeaponOption[FIRE_DRAGON].iCurBullet += BulletCnt;
				m_pPlayer->m_PlayerOption.m_iInjector_BulletCnt = 0;
			}
		}
	}
	else if (m_pPlayer->m_PlayerOption.m_wstrCurWeaponName == m_tWeaponOption[POISON].wstrWeaponName)
	{
		if (0 < m_pPlayer->m_PlayerOption.m_iPistol_BulletCnt)
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
	for (_uint i = 0; i < m_vecBullet.size(); ++i)
	{
		Safe_Release(m_vecBullet[i]);
	}
	m_vecBullet.clear();
}

