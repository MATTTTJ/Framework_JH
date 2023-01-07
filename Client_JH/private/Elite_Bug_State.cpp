#include "stdafx.h"
#include "..\public\Elite_Bug_State.h"
#include "Bullet.h"
#include "Player.h"
#include "GameInstance.h"

CElite_Bug_State::CElite_Bug_State()
{
}

HRESULT CElite_Bug_State::Initialize(CElite_Bug* pOwner, CState* pStateMachineCom, CModel* pModel, CTransform* pTransform, CNavigation* pNavigation)
{
	return S_OK;
}

void CElite_Bug_State::Tick(_double dTimeDelta)
{
}

void CElite_Bug_State::Late_Tick(_double dTimeDelta)
{
}

HRESULT CElite_Bug_State::SetUp_State_No_Detected()
{
	return S_OK;

}

HRESULT CElite_Bug_State::SetUp_State_Idle()
{
	return S_OK;
}

HRESULT CElite_Bug_State::SetUp_State_Walk()
{
	return S_OK;
}

HRESULT CElite_Bug_State::SetUp_State_Run()
{
	return S_OK;
}

HRESULT CElite_Bug_State::SetUp_State_Attack_A()
{
	return S_OK;
}

HRESULT CElite_Bug_State::SetUp_State_Attack_B()
{
	return S_OK;
}

HRESULT CElite_Bug_State::SetUp_State_Hide()
{
	return S_OK;
}

HRESULT CElite_Bug_State::SetUp_State_Death()
{
	return S_OK;
}

void CElite_Bug_State::Start_No_Detected(_double dTimeDelta)
{
}

void CElite_Bug_State::Start_Idle(_double dTimeDelta)
{
}

void CElite_Bug_State::Start_Walk(_double dTimeDelta)
{
}

void CElite_Bug_State::Start_Run(_double dTimeDelta)
{
}

void CElite_Bug_State::Start_Attack_A(_double dTimeDelta)
{
}

void CElite_Bug_State::Start_Attack_B(_double dTimeDelta)
{
}

void CElite_Bug_State::Start_Hide(_double dTimeDelta)
{
}

void CElite_Bug_State::Start_Death(_double dTimeDelta)
{
}

void CElite_Bug_State::Tick_No_Detected(_double dTimeDelta)
{
}

void CElite_Bug_State::Tick_Idle(_double dTimeDelta)
{
}

void CElite_Bug_State::Tick_Walk(_double dTimeDelta)
{
}

void CElite_Bug_State::Tick_Run(_double dTimeDelta)
{
}

void CElite_Bug_State::Tick_Attack_A(_double dTimeDelta)
{
}

void CElite_Bug_State::Tick_Attack_B(_double dTimeDelta)
{
}

void CElite_Bug_State::Tick_Hide(_double dTimeDelta)
{
}

void CElite_Bug_State::Tick_Death(_double dTimeDelta)
{
}

void CElite_Bug_State::End_No_Detected(_double dTimeDelta)
{
}

void CElite_Bug_State::End_Idle(_double dTimeDelta)
{
}

void CElite_Bug_State::End_Walk(_double dTimeDelta)
{
}

void CElite_Bug_State::End_Run(_double dTimeDelta)
{
}

void CElite_Bug_State::End_Attack_A(_double dTimeDelta)
{
}

void CElite_Bug_State::End_Attack_B(_double dTimeDelta)
{
}

void CElite_Bug_State::End_Hide(_double dTimeDelta)
{
}

void CElite_Bug_State::End_Death(_double dTimeDelta)
{
}

_bool CElite_Bug_State::Player_Detected()
{
	return true;
}

_bool CElite_Bug_State::Player_DetectedAndFar()
{
	return true;
}

_bool CElite_Bug_State::Player_DetectedAndClose()
{
	return true;
}

_bool CElite_Bug_State::Player_NotDetected()
{
	return true;
}

CElite_Bug_State* CElite_Bug_State::Create(CElite_Bug* pOwner, CState* pStateMachineCom, CModel* pModel,
	CTransform* pTransform, CNavigation* pNavigation)
{
	return nullptr;
}

void CElite_Bug_State::Free()
{
}


