#include "stdafx.h"
#include "..\public\Human_Explode_State.h"
#include "Bullet.h"
#include "Player.h"
#include "GameInstance.h"

CHuman_Explode_State::CHuman_Explode_State()
{
}

HRESULT CHuman_Explode_State::Initialize(CHuman_Explode* pOwner, CState* pStateMachineCom, CModel* pModel, CTransform* pTransform, CNavigation* pNavigation)
{
	return S_OK;
}

void CHuman_Explode_State::Tick(_double dTimeDelta)
{
}

void CHuman_Explode_State::Late_Tick(_double dTimeDelta)
{
}

HRESULT CHuman_Explode_State::SetUp_State_No_Detected()
{
	return S_OK;

}

HRESULT CHuman_Explode_State::SetUp_State_Idle()
{
	return S_OK;
}

HRESULT CHuman_Explode_State::SetUp_State_Walk()
{
	return S_OK;
}

HRESULT CHuman_Explode_State::SetUp_State_Run()
{
	return S_OK;
}

HRESULT CHuman_Explode_State::SetUp_State_Attack_A()
{
	return S_OK;
}

HRESULT CHuman_Explode_State::SetUp_State_Attack_B()
{
	return S_OK;
}

HRESULT CHuman_Explode_State::SetUp_State_Hide()
{
	return S_OK;
}

HRESULT CHuman_Explode_State::SetUp_State_Death()
{
	return S_OK;
}

void CHuman_Explode_State::Start_No_Detected(_double dTimeDelta)
{
}

void CHuman_Explode_State::Start_Idle(_double dTimeDelta)
{
}

void CHuman_Explode_State::Start_Walk(_double dTimeDelta)
{
}

void CHuman_Explode_State::Start_Run(_double dTimeDelta)
{
}

void CHuman_Explode_State::Start_Attack_A(_double dTimeDelta)
{
}

void CHuman_Explode_State::Start_Attack_B(_double dTimeDelta)
{
}

void CHuman_Explode_State::Start_Hide(_double dTimeDelta)
{
}

void CHuman_Explode_State::Start_Death(_double dTimeDelta)
{
}

void CHuman_Explode_State::Tick_No_Detected(_double dTimeDelta)
{
}

void CHuman_Explode_State::Tick_Idle(_double dTimeDelta)
{
}

void CHuman_Explode_State::Tick_Walk(_double dTimeDelta)
{
}

void CHuman_Explode_State::Tick_Run(_double dTimeDelta)
{
}

void CHuman_Explode_State::Tick_Attack_A(_double dTimeDelta)
{
}

void CHuman_Explode_State::Tick_Attack_B(_double dTimeDelta)
{
}

void CHuman_Explode_State::Tick_Hide(_double dTimeDelta)
{
}

void CHuman_Explode_State::Tick_Death(_double dTimeDelta)
{
}

void CHuman_Explode_State::End_No_Detected(_double dTimeDelta)
{
}

void CHuman_Explode_State::End_Idle(_double dTimeDelta)
{
}

void CHuman_Explode_State::End_Walk(_double dTimeDelta)
{
}

void CHuman_Explode_State::End_Run(_double dTimeDelta)
{
}

void CHuman_Explode_State::End_Attack_A(_double dTimeDelta)
{
}

void CHuman_Explode_State::End_Attack_B(_double dTimeDelta)
{
}

void CHuman_Explode_State::End_Hide(_double dTimeDelta)
{
}

void CHuman_Explode_State::End_Death(_double dTimeDelta)
{
}

_bool CHuman_Explode_State::Player_Detected()
{
	return true;
}

_bool CHuman_Explode_State::Player_DetectedAndFar()
{
	return true;
}

_bool CHuman_Explode_State::Player_DetectedAndClose()
{
	return true;
}

_bool CHuman_Explode_State::Player_NotDetected()
{
	return true;
}

CHuman_Explode_State* CHuman_Explode_State::Create(CHuman_Explode* pOwner, CState* pStateMachineCom, CModel* pModel,
	CTransform* pTransform, CNavigation* pNavigation)
{
	return nullptr;
}

void CHuman_Explode_State::Free()
{
}


