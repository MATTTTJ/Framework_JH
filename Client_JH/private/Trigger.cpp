#include "stdafx.h"
#include "..\public\Trigger.h"
#include "GameInstance.h"
#include "Monster.h"
#include "Player.h"

CTrigger::CTrigger(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CGameObject(pDevice, pContext)
{
}

CTrigger::CTrigger(const CTrigger& rhs)
	: CGameObject(rhs)
{
}

HRESULT CTrigger::Initialize_Prototype()
{
	m_bHasModel = true;

	FAILED_CHECK_RETURN(__super::Initialize_Prototype(), E_FAIL);

	return S_OK;
}

HRESULT CTrigger::Initialize_Clone(const wstring& wstrPrototypeTag, void* pArg)
{
	ZeroMemory(&m_tTriggerOption, sizeof(TRIGGERDESC));

	if (nullptr != pArg)
	{
		m_tTriggerOption = *(TRIGGERDESC*)pArg;
	}
	else
	{
		m_tTriggerOption.m_eType = TRIGGERTYPE_END;
	}

	FAILED_CHECK_RETURN(__super::Initialize_Clone(wstrPrototypeTag, pArg), E_FAIL);

	FAILED_CHECK_RETURN(SetUp_Components(), E_FAIL);

	return S_OK;
}

void CTrigger::Tick(_double TimeDelta)
{
	__super::Tick(TimeDelta);

	if(m_tTriggerOption.m_eType == TRIGGER_A_TO_KNIGHT)
	{
		Room_A_To_Knight();
	}
	else if (m_tTriggerOption.m_eType == TRIGGER_KNIGHT_TO_B)
	{
		Knight_To_Room_B();
	}
	else if (m_tTriggerOption.m_eType == TRIGGER_B_TO_BUG)
	{
		Room_B_To_Bug();
	}
	else if (m_tTriggerOption.m_eType == TRIGGER_BUG_TO_C)
	{
		Bug_To_Room_C();
	}
	else if (m_tTriggerOption.m_eType == TRIGGER_C_TO_BOSS)
	{
		Room_C_To_Boss();
	}

	if(m_bSpawnTrigger == true)
	{
		CTrigger* pTrigger = nullptr;
		_matrix PivotMatrix = XMMatrixIdentity();
		CTrigger::TRIGGERDESC TriggerDesc;

		if (m_tTriggerOption.m_eType == TRIGGER_A_TO_KNIGHT)
		{
			PivotMatrix.r[3] = XMVectorSet(25.232f, 0.02f, 59.929f, 1.f);
			TriggerDesc.m_eType = CTrigger::TRIGGER_KNIGHT_TO_B;
			pTrigger = (CTrigger*)CGameInstance::GetInstance()->Clone_GameObjectReturnPtr_M(LEVEL_GAMEPLAY, L"Layer_Trigger", L"Prototype_GameObject_Trigger", PivotMatrix, &TriggerDesc);
			pTrigger->Set_Owner(m_pPlayer);
			m_bSpawnTrigger = false;
			Set_Dead(true);
		}
		else if (m_tTriggerOption.m_eType == TRIGGER_KNIGHT_TO_B)
		{
			PivotMatrix.r[3] = XMVectorSet(-31.863f, 0.013f, 61.017f, 1.f);
			TriggerDesc.m_eType = CTrigger::TRIGGER_B_TO_BUG;
			pTrigger = (CTrigger*)CGameInstance::GetInstance()->Clone_GameObjectReturnPtr_M(LEVEL_GAMEPLAY, L"Layer_Trigger", L"Prototype_GameObject_Trigger", PivotMatrix, &TriggerDesc);
			pTrigger->Set_Owner(m_pPlayer);
			m_bSpawnTrigger = false;
			Set_Dead(true);
		}
		else if (m_tTriggerOption.m_eType == TRIGGER_B_TO_BUG)
		{
			PivotMatrix.r[3] = XMVectorSet(-60.485f, 2.031f, 110.085f, 1.f);
			TriggerDesc.m_eType = CTrigger::TRIGGER_BUG_TO_C;
			pTrigger = (CTrigger*)CGameInstance::GetInstance()->Clone_GameObjectReturnPtr_M(LEVEL_GAMEPLAY, L"Layer_Trigger", L"Prototype_GameObject_Trigger", PivotMatrix, &TriggerDesc);
			pTrigger->Set_Owner(m_pPlayer);
			m_bSpawnTrigger = false;
			Set_Dead(true);
		}
		else if (m_tTriggerOption.m_eType == TRIGGER_BUG_TO_C)
		{
			PivotMatrix.r[3] = XMVectorSet(-26.953f, -1.484f, 102.057f, 1.f);
			TriggerDesc.m_eType = CTrigger::TRIGGER_C_TO_BOSS;
			pTrigger = (CTrigger*)CGameInstance::GetInstance()->Clone_GameObjectReturnPtr_M(LEVEL_GAMEPLAY, L"Layer_Trigger", L"Prototype_GameObject_Trigger", PivotMatrix, &TriggerDesc);
			pTrigger->Set_Owner(m_pPlayer);
			m_bSpawnTrigger = false;
			Set_Dead(true);
		}
	}

	m_pTriggerCollCom->Update(m_pTransformCom->Get_WorldMatrix());
}

void CTrigger::Late_Tick(_double TimeDelta)
{
	__super::Late_Tick(TimeDelta);

	if (nullptr != m_pRendererCom)
	{
		m_pRendererCom->Add_DebugRenderGroup(m_pTriggerCollCom);
	}
}

HRESULT CTrigger::Render()
{
	FAILED_CHECK_RETURN(__super::Render(), E_FAIL);

	return S_OK;
}

HRESULT CTrigger::Room_A_To_Knight()
{
	CCollider* pCollider = (CCollider*)m_pPlayer->Find_Component(L"Com_BODYSPHERE");
	NULL_CHECK_RETURN(pCollider, E_FAIL);
	if (m_pTriggerCollCom->Collision(pCollider))
	{
		m_pPlayer->Set_PortalUI(true);

		if(CGameInstance::GetInstance()->Key_Down(DIK_F))
		{
			m_pPlayer->Set_RoomType(CPlayer::ROOM_KIGHT);
			_matrix PlayerMat = m_pPlayer->Get_WorldFloat4x4();
			PlayerMat.r[3] = XMVectorSet(0.7f, 0.36f, 36.5f, 1.f);
			m_pPlayer->Set_CellIndex(360);
			m_pPlayer->Set_WorldMatrix(PlayerMat);

			CMonster::MONSTEROPTION MonsterDesc;
			CMonster* pMonster = nullptr;
			_matrix PivotMatrix = XMMatrixIdentity();
			ZeroMemory(&MonsterDesc, sizeof(CMonster::MONSTEROPTION));
			PivotMatrix.r[3] = XMVectorSet(24.5f, 0.4f, 47.37f, 1.f);
			MonsterDesc.m_bFirstSpawnType[CMonster::STATE_UPSPAWN] = true;
			MonsterDesc.MonsterDesc.m_iHP = MonsterDesc.MonsterDesc.m_iMaxHP = 300;
			MonsterDesc.MonsterDesc.m_iDamage = 15;
			MonsterDesc.MonsterDesc.m_iShield = MonsterDesc.MonsterDesc.m_iMaxShield = 0;
			MonsterDesc.MonsterDesc.TransformDesc.fSpeedPerSec = 2.5f;
			MonsterDesc.MonsterDesc.TransformDesc.fRotationPerSec = XMConvertToRadians(180.f);
			MonsterDesc.m_iCellIndex = 420;
			pMonster = dynamic_cast<CMonster*>(CGameInstance::GetInstance()->Clone_GameObjectReturnPtr_M(LEVEL_GAMEPLAY, L"Layer_Monster", L"Prototype_GameObject_Normal_Elite_Knight", PivotMatrix, &MonsterDesc));
			pMonster->Set_Player(m_pPlayer);

			ZeroMemory(&MonsterDesc, sizeof(CMonster::MONSTEROPTION));
			PivotMatrix.r[3] = XMVectorSet(11.9f, 0.9f, 43.3f, 1.f);
			MonsterDesc.m_bFirstSpawnType[CMonster::STATE_ALREADYSPAWN] = true;
			MonsterDesc.MonsterDesc.m_iHP = MonsterDesc.MonsterDesc.m_iMaxHP = 300;
			MonsterDesc.MonsterDesc.m_iDamage = 15;
			MonsterDesc.MonsterDesc.m_iShield = MonsterDesc.MonsterDesc.m_iMaxShield = 0;
			MonsterDesc.MonsterDesc.TransformDesc.fSpeedPerSec = 5.f;
			MonsterDesc.m_iCellIndex = 385;
			pMonster = dynamic_cast<CMonster*>(CGameInstance::GetInstance()->Clone_GameObjectReturnPtr_M(LEVEL_GAMEPLAY, L"Layer_Monster", L"Prototype_GameObject_Normal_Human_Spear", PivotMatrix, &MonsterDesc));
			pMonster->Set_Player(m_pPlayer);

			ZeroMemory(&MonsterDesc, sizeof(CMonster::MONSTEROPTION));
			PivotMatrix.r[3] = XMVectorSet(7.38f, 0.f, 33.9f, 1.f);
			MonsterDesc.m_bFirstSpawnType[CMonster::STATE_ALREADYSPAWN] = true;
			MonsterDesc.MonsterDesc.m_iHP = MonsterDesc.MonsterDesc.m_iMaxHP = 300;
			MonsterDesc.MonsterDesc.m_iDamage = 15;
			MonsterDesc.MonsterDesc.m_iShield = MonsterDesc.MonsterDesc.m_iMaxShield = 0;
			MonsterDesc.MonsterDesc.TransformDesc.fSpeedPerSec = 2.5f;
			MonsterDesc.MonsterDesc.TransformDesc.fRotationPerSec = XMConvertToRadians(180.f);
			MonsterDesc.m_iCellIndex = 377;
			pMonster = dynamic_cast<CMonster*>(CGameInstance::GetInstance()->Clone_GameObjectReturnPtr_M(LEVEL_GAMEPLAY, L"Layer_Monster", L"Prototype_GameObject_Normal_Human_Explode", PivotMatrix, &MonsterDesc));
			pMonster->Set_Player(m_pPlayer);

			ZeroMemory(&MonsterDesc, sizeof(CMonster::MONSTEROPTION));
			PivotMatrix.r[3] = XMVectorSet(13.f, 0.f, 35.1f, 1.f);
			MonsterDesc.m_bFirstSpawnType[CMonster::STATE_ALREADYSPAWN] = true;			// No Detected == Patrol
			MonsterDesc.MonsterDesc.m_iHP = MonsterDesc.MonsterDesc.m_iMaxHP = 300;
			MonsterDesc.MonsterDesc.m_iDamage = 15;
			MonsterDesc.MonsterDesc.m_iShield = MonsterDesc.MonsterDesc.m_iMaxShield = 0;
			MonsterDesc.MonsterDesc.TransformDesc.fSpeedPerSec = 2.5f;
			MonsterDesc.MonsterDesc.TransformDesc.fRotationPerSec = XMConvertToRadians(180.f);
			MonsterDesc.m_iCellIndex = 388;
			pMonster = dynamic_cast<CMonster*>(CGameInstance::GetInstance()->Clone_GameObjectReturnPtr_M(LEVEL_GAMEPLAY, L"Layer_Monster", L"Prototype_GameObject_Normal_Human_Granade", PivotMatrix, &MonsterDesc));
			pMonster->Set_Player(m_pPlayer);

			ZeroMemory(&MonsterDesc, sizeof(CMonster::MONSTEROPTION));
			PivotMatrix.r[3] = XMVectorSet(17.f, 0.f, 44.2f, 1.f);
			MonsterDesc.m_bFirstSpawnType[CMonster::STATE_ALREADYSPAWN] = true;
			MonsterDesc.MonsterDesc.m_iHP = MonsterDesc.MonsterDesc.m_iMaxHP = 300;
			MonsterDesc.MonsterDesc.m_iDamage = 15;
			MonsterDesc.MonsterDesc.m_iShield = MonsterDesc.MonsterDesc.m_iMaxShield = 0;
			MonsterDesc.MonsterDesc.TransformDesc.fSpeedPerSec = 2.5f;
			MonsterDesc.MonsterDesc.TransformDesc.fRotationPerSec = XMConvertToRadians(180.f);
			MonsterDesc.m_iCellIndex = 402;
			pMonster = dynamic_cast<CMonster*>(CGameInstance::GetInstance()->Clone_GameObjectReturnPtr_M(LEVEL_GAMEPLAY, L"Layer_Monster", L"Prototype_GameObject_Normal_Human_Bow", PivotMatrix, &MonsterDesc));
			pMonster->Set_Player(m_pPlayer);

			m_bSpawnTrigger = true;
		}
		return S_OK;
	}
	else
	{
		m_pPlayer->Set_PortalUI(false);
	}

	return S_OK;
}


// Elite_Knight
// m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(0.7f, 0.36f, 36.5f, 1.f));
// m_pNavigationCom->Set_CellIndex(360);

HRESULT CTrigger::Knight_To_Room_B()
{
	CCollider* pCollider = (CCollider*)m_pPlayer->Find_Component(L"Com_BODYSPHERE");
	NULL_CHECK_RETURN(pCollider, E_FAIL);
	if (m_pTriggerCollCom->Collision(pCollider))
	{
		m_pPlayer->Set_PortalUI(true);

		if (CGameInstance::GetInstance()->Key_Down(DIK_F))
		{
			m_pPlayer->Set_RoomType(CPlayer::ROOM_B);
			_matrix PlayerMat = m_pPlayer->Get_WorldFloat4x4();
			PlayerMat.r[3] = XMVectorSet(-10.018f, 0.032f, 33.904f, 1.f);
			m_pPlayer->Set_CellIndex(194);
			m_pPlayer->Set_WorldMatrix(PlayerMat);

			CMonster::MONSTEROPTION MonsterDesc;
			CMonster* pMonster = nullptr;
			_matrix PivotMatrix = XMMatrixIdentity();

			ZeroMemory(&MonsterDesc, sizeof(CMonster::MONSTEROPTION));
			PivotMatrix.r[3] = XMVectorSet(-13.3f, 0.f, 48.4f, 1.f);
			MonsterDesc.m_bFirstSpawnType[CMonster::STATE_ALREADYSPAWN] = true;
			MonsterDesc.MonsterDesc.m_iHP = MonsterDesc.MonsterDesc.m_iMaxHP = 300;
			MonsterDesc.MonsterDesc.m_iDamage = 15;
			MonsterDesc.MonsterDesc.m_iShield = MonsterDesc.MonsterDesc.m_iMaxShield = 0;
			MonsterDesc.MonsterDesc.TransformDesc.fSpeedPerSec = 5.f;
			MonsterDesc.m_iCellIndex = 205;
			pMonster = dynamic_cast<CMonster*>(CGameInstance::GetInstance()->Clone_GameObjectReturnPtr_M(LEVEL_GAMEPLAY, L"Layer_Monster", L"Prototype_GameObject_Normal_Human_Spear", PivotMatrix, &MonsterDesc));
			pMonster->Set_Player(m_pPlayer);

			ZeroMemory(&MonsterDesc, sizeof(CMonster::MONSTEROPTION));
			PivotMatrix.r[3] = XMVectorSet(-15.8f, 0.18f, 55.5f, 1.f);
			MonsterDesc.m_bFirstSpawnType[CMonster::STATE_ALREADYSPAWN] = true;
			MonsterDesc.MonsterDesc.m_iHP = MonsterDesc.MonsterDesc.m_iMaxHP = 300;
			MonsterDesc.MonsterDesc.m_iDamage = 15;
			MonsterDesc.MonsterDesc.m_iShield = MonsterDesc.MonsterDesc.m_iMaxShield = 0;
			MonsterDesc.MonsterDesc.TransformDesc.fSpeedPerSec = 5.f;
			MonsterDesc.m_iCellIndex = 221;
			pMonster = dynamic_cast<CMonster*>(CGameInstance::GetInstance()->Clone_GameObjectReturnPtr_M(LEVEL_GAMEPLAY, L"Layer_Monster", L"Prototype_GameObject_Normal_Human_Spear", PivotMatrix, &MonsterDesc));
			pMonster->Set_Player(m_pPlayer);

			ZeroMemory(&MonsterDesc, sizeof(CMonster::MONSTEROPTION));
			PivotMatrix.r[3] = XMVectorSet(-23.7f, 1.4f, 45.7f, 1.f);
			MonsterDesc.m_bFirstSpawnType[CMonster::STATE_ALREADYSPAWN] = true;
			MonsterDesc.MonsterDesc.m_iHP = MonsterDesc.MonsterDesc.m_iMaxHP = 300;
			MonsterDesc.MonsterDesc.m_iDamage = 15;
			MonsterDesc.MonsterDesc.m_iShield = MonsterDesc.MonsterDesc.m_iMaxShield = 0;
			MonsterDesc.MonsterDesc.TransformDesc.fSpeedPerSec = 5.f;
			MonsterDesc.m_iCellIndex = 237;
			pMonster = dynamic_cast<CMonster*>(CGameInstance::GetInstance()->Clone_GameObjectReturnPtr_M(LEVEL_GAMEPLAY, L"Layer_Monster", L"Prototype_GameObject_Normal_Human_Spear", PivotMatrix, &MonsterDesc));
			pMonster->Set_Player(m_pPlayer);

			ZeroMemory(&MonsterDesc, sizeof(CMonster::MONSTEROPTION));
			PivotMatrix.r[3] = XMVectorSet(-32.3f, 1.4f, 52.8f, 1.f);
			MonsterDesc.m_bFirstSpawnType[CMonster::STATE_ALREADYSPAWN] = true;
			MonsterDesc.MonsterDesc.m_iHP = MonsterDesc.MonsterDesc.m_iMaxHP = 300;
			MonsterDesc.MonsterDesc.m_iDamage = 15;
			MonsterDesc.MonsterDesc.m_iShield = MonsterDesc.MonsterDesc.m_iMaxShield = 0;
			MonsterDesc.MonsterDesc.TransformDesc.fSpeedPerSec = 5.f;
			MonsterDesc.m_iCellIndex = 270;
			pMonster = dynamic_cast<CMonster*>(CGameInstance::GetInstance()->Clone_GameObjectReturnPtr_M(LEVEL_GAMEPLAY, L"Layer_Monster", L"Prototype_GameObject_Normal_Human_Spear", PivotMatrix, &MonsterDesc));
			pMonster->Set_Player(m_pPlayer);

			ZeroMemory(&MonsterDesc, sizeof(CMonster::MONSTEROPTION));
			PivotMatrix.r[3] = XMVectorSet(-12.2f, 0.f, 57.4f, 1.f);
			MonsterDesc.m_bFirstSpawnType[CMonster::STATE_ALREADYSPAWN] = true;
			MonsterDesc.MonsterDesc.m_iHP = MonsterDesc.MonsterDesc.m_iMaxHP = 300;
			MonsterDesc.MonsterDesc.m_iDamage = 15;
			MonsterDesc.MonsterDesc.m_iShield = MonsterDesc.MonsterDesc.m_iMaxShield = 0;
			MonsterDesc.MonsterDesc.TransformDesc.fSpeedPerSec = 2.5f;
			MonsterDesc.MonsterDesc.TransformDesc.fRotationPerSec = XMConvertToRadians(180.f);
			MonsterDesc.m_iCellIndex = 220;
			pMonster = dynamic_cast<CMonster*>(CGameInstance::GetInstance()->Clone_GameObjectReturnPtr_M(LEVEL_GAMEPLAY, L"Layer_Monster", L"Prototype_GameObject_Normal_Human_Bow", PivotMatrix, &MonsterDesc));
			pMonster->Set_Player(m_pPlayer);

			ZeroMemory(&MonsterDesc, sizeof(CMonster::MONSTEROPTION));
			PivotMatrix.r[3] = XMVectorSet(-34.2f, 0.f, 37.8f, 1.f);
			MonsterDesc.m_bFirstSpawnType[CMonster::STATE_ALREADYSPAWN] = true;
			MonsterDesc.MonsterDesc.m_iHP = MonsterDesc.MonsterDesc.m_iMaxHP = 300;
			MonsterDesc.MonsterDesc.m_iDamage = 15;
			MonsterDesc.MonsterDesc.m_iShield = MonsterDesc.MonsterDesc.m_iMaxShield = 0;
			MonsterDesc.MonsterDesc.TransformDesc.fSpeedPerSec = 2.5f;
			MonsterDesc.MonsterDesc.TransformDesc.fRotationPerSec = XMConvertToRadians(180.f);
			MonsterDesc.m_iCellIndex = 256;
			pMonster = dynamic_cast<CMonster*>(CGameInstance::GetInstance()->Clone_GameObjectReturnPtr_M(LEVEL_GAMEPLAY, L"Layer_Monster", L"Prototype_GameObject_Normal_Human_Bow", PivotMatrix, &MonsterDesc));
			pMonster->Set_Player(m_pPlayer);

			ZeroMemory(&MonsterDesc, sizeof(CMonster::MONSTEROPTION));
			PivotMatrix.r[3] = XMVectorSet(-20.1f, 0.f, 37.6f, 1.f);
			MonsterDesc.m_bFirstSpawnType[CMonster::STATE_ALREADYSPAWN] = true;			// No Detected == Patrol
			MonsterDesc.MonsterDesc.m_iHP = MonsterDesc.MonsterDesc.m_iMaxHP = 300;
			MonsterDesc.MonsterDesc.m_iDamage = 15;
			MonsterDesc.MonsterDesc.m_iShield = MonsterDesc.MonsterDesc.m_iMaxShield = 0;
			MonsterDesc.MonsterDesc.TransformDesc.fSpeedPerSec = 2.5f;
			MonsterDesc.MonsterDesc.TransformDesc.fRotationPerSec = XMConvertToRadians(180.f);
			MonsterDesc.m_iCellIndex = 243;
			pMonster = dynamic_cast<CMonster*>(CGameInstance::GetInstance()->Clone_GameObjectReturnPtr_M(LEVEL_GAMEPLAY, L"Layer_Monster", L"Prototype_GameObject_Normal_Human_Granade", PivotMatrix, &MonsterDesc));
			pMonster->Set_Player(m_pPlayer);

			ZeroMemory(&MonsterDesc, sizeof(CMonster::MONSTEROPTION));
			PivotMatrix.r[3] = XMVectorSet(-30.1f, 1.4f, 47.0f, 1.f);
			MonsterDesc.m_bFirstSpawnType[CMonster::STATE_ALREADYSPAWN] = true;			// No Detected == Patrol
			MonsterDesc.MonsterDesc.m_iHP = MonsterDesc.MonsterDesc.m_iMaxHP = 300;
			MonsterDesc.MonsterDesc.m_iDamage = 15;
			MonsterDesc.MonsterDesc.m_iShield = MonsterDesc.MonsterDesc.m_iMaxShield = 0;
			MonsterDesc.MonsterDesc.TransformDesc.fSpeedPerSec = 2.5f;
			MonsterDesc.MonsterDesc.TransformDesc.fRotationPerSec = XMConvertToRadians(180.f);
			MonsterDesc.m_iCellIndex = 265;
			pMonster = dynamic_cast<CMonster*>(CGameInstance::GetInstance()->Clone_GameObjectReturnPtr_M(LEVEL_GAMEPLAY, L"Layer_Monster", L"Prototype_GameObject_Normal_Human_Granade", PivotMatrix, &MonsterDesc));
			pMonster->Set_Player(m_pPlayer);

			ZeroMemory(&MonsterDesc, sizeof(CMonster::MONSTEROPTION)); // ��ġ Ȯ�� �ʿ�
			PivotMatrix.r[3] = XMVectorSet(-30.1f, 1.4f, 47.0f, 1.f);
			MonsterDesc.m_bFirstSpawnType[CMonster::STATE_ALREADYSPAWN] = true;			// No Detected == Patrol
			MonsterDesc.MonsterDesc.m_iHP = MonsterDesc.MonsterDesc.m_iMaxHP = 300;
			MonsterDesc.MonsterDesc.m_iDamage = 15;
			MonsterDesc.MonsterDesc.m_iShield = MonsterDesc.MonsterDesc.m_iMaxShield = 0;
			MonsterDesc.MonsterDesc.TransformDesc.fSpeedPerSec = 2.5f;
			MonsterDesc.MonsterDesc.TransformDesc.fRotationPerSec = XMConvertToRadians(180.f);
			MonsterDesc.m_iCellIndex = 269;
			pMonster = dynamic_cast<CMonster*>(CGameInstance::GetInstance()->Clone_GameObjectReturnPtr_M(LEVEL_GAMEPLAY, L"Layer_Monster", L"Prototype_GameObject_Normal_Human_Granade", PivotMatrix, &MonsterDesc));
			pMonster->Set_Player(m_pPlayer);

			ZeroMemory(&MonsterDesc, sizeof(CMonster::MONSTEROPTION));
			PivotMatrix.r[3] = XMVectorSet(-7.7f, -0.85f, 42.f, 1.f);
			MonsterDesc.m_bFirstSpawnType[CMonster::STATE_ALREADYSPAWN] = true;
			MonsterDesc.MonsterDesc.m_iHP = MonsterDesc.MonsterDesc.m_iMaxHP = 300;
			MonsterDesc.MonsterDesc.m_iDamage = 15;
			MonsterDesc.MonsterDesc.m_iShield = MonsterDesc.MonsterDesc.m_iMaxShield = 0;
			MonsterDesc.MonsterDesc.TransformDesc.fSpeedPerSec = 2.5f;
			MonsterDesc.MonsterDesc.TransformDesc.fRotationPerSec = XMConvertToRadians(180.f);
			MonsterDesc.m_iCellIndex = 201;
			pMonster = dynamic_cast<CMonster*>(CGameInstance::GetInstance()->Clone_GameObjectReturnPtr_M(LEVEL_GAMEPLAY, L"Layer_Monster", L"Prototype_GameObject_Normal_Human_Explode", PivotMatrix, &MonsterDesc));
			pMonster->Set_Player(m_pPlayer);

			ZeroMemory(&MonsterDesc, sizeof(CMonster::MONSTEROPTION));
			PivotMatrix.r[3] = XMVectorSet(-20.7f, 0.f, 51.8f, 1.f);
			MonsterDesc.m_bFirstSpawnType[CMonster::STATE_ALREADYSPAWN] = true;
			MonsterDesc.MonsterDesc.m_iHP = MonsterDesc.MonsterDesc.m_iMaxHP = 300;
			MonsterDesc.MonsterDesc.m_iDamage = 15;
			MonsterDesc.MonsterDesc.m_iShield = MonsterDesc.MonsterDesc.m_iMaxShield = 0;
			MonsterDesc.MonsterDesc.TransformDesc.fSpeedPerSec = 2.5f;
			MonsterDesc.MonsterDesc.TransformDesc.fRotationPerSec = XMConvertToRadians(180.f);
			MonsterDesc.m_iCellIndex = 232;
			pMonster = dynamic_cast<CMonster*>(CGameInstance::GetInstance()->Clone_GameObjectReturnPtr_M(LEVEL_GAMEPLAY, L"Layer_Monster", L"Prototype_GameObject_Normal_Human_Explode", PivotMatrix, &MonsterDesc));
			pMonster->Set_Player(m_pPlayer);

			m_bSpawnTrigger = true;

		}
		return S_OK;
	}
	else
	{
		m_pPlayer->Set_PortalUI(false);
	}

	return S_OK;
}

//Elite_Bug
// m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(-59.f, -0.9f, 64.6f, 1.f));
// m_pNavigationCom->Set_CellIndex(442);


HRESULT CTrigger::Room_B_To_Bug()
{
	CCollider* pCollider = (CCollider*)m_pPlayer->Find_Component(L"Com_BODYSPHERE");
	NULL_CHECK_RETURN(pCollider, E_FAIL);
	if (m_pTriggerCollCom->Collision(pCollider))
	{
		m_pPlayer->Set_PortalUI(true);

		if (CGameInstance::GetInstance()->Key_Down(DIK_F))
		{
			m_pPlayer->Set_RoomType(CPlayer::ROOM_BUG);
			_matrix PlayerMat = m_pPlayer->Get_WorldFloat4x4();
			PlayerMat.r[3] = XMVectorSet(-59.f, -0.9f, 64.6f, 1.f);
			m_pPlayer->Set_CellIndex(442);
			m_pPlayer->Set_WorldMatrix(PlayerMat);

			CMonster::MONSTEROPTION MonsterDesc;
			CMonster* pMonster = nullptr;
			_matrix PivotMatrix = XMMatrixIdentity();
			ZeroMemory(&MonsterDesc, sizeof(CMonster::MONSTEROPTION));
			PivotMatrix.r[3] = XMVectorSet(-64.3f, -0.7f, 73.8f, 1.f);
			MonsterDesc.m_bFirstSpawnType[CMonster::STATE_ALREADYSPAWN] = true;
			MonsterDesc.MonsterDesc.m_iHP = MonsterDesc.MonsterDesc.m_iMaxHP = 300;
			MonsterDesc.MonsterDesc.m_iDamage = 15;
			MonsterDesc.MonsterDesc.m_iShield = MonsterDesc.MonsterDesc.m_iMaxShield = 0;
			MonsterDesc.MonsterDesc.TransformDesc.fSpeedPerSec = 5.f;
			MonsterDesc.m_iCellIndex = 445;
			pMonster = dynamic_cast<CMonster*>(CGameInstance::GetInstance()->Clone_GameObjectReturnPtr_M(LEVEL_GAMEPLAY, L"Layer_Monster", L"Prototype_GameObject_Normal_Human_Sword", PivotMatrix, &MonsterDesc));
			pMonster->Set_Player(m_pPlayer);

			ZeroMemory(&MonsterDesc, sizeof(CMonster::MONSTEROPTION));
			PivotMatrix.r[3] = XMVectorSet(-64.5f, -0.9f, 69.2f, 1.f);
			MonsterDesc.m_bFirstSpawnType[CMonster::STATE_NODETECTED] = true;
			MonsterDesc.MonsterDesc.m_iHP = MonsterDesc.MonsterDesc.m_iMaxHP = 300;
			MonsterDesc.MonsterDesc.m_iDamage = 15;
			MonsterDesc.MonsterDesc.m_iShield = MonsterDesc.MonsterDesc.m_iMaxShield = 0;
			MonsterDesc.MonsterDesc.TransformDesc.fSpeedPerSec = 5.f;
			MonsterDesc.m_iCellIndex = 446;
			pMonster = dynamic_cast<CMonster*>(CGameInstance::GetInstance()->Clone_GameObjectReturnPtr_M(LEVEL_GAMEPLAY, L"Layer_Monster", L"Prototype_GameObject_Normal_Human_Sword", PivotMatrix, &MonsterDesc));
			pMonster->Set_Player(m_pPlayer);

			ZeroMemory(&MonsterDesc, sizeof(CMonster::MONSTEROPTION));
			PivotMatrix.r[3] = XMVectorSet(-62.5f, -0.9f, 71.6f, 1.f);
			MonsterDesc.m_bFirstSpawnType[CMonster::STATE_NODETECTED] = true;
			MonsterDesc.MonsterDesc.m_iHP = MonsterDesc.MonsterDesc.m_iMaxHP = 300;
			MonsterDesc.MonsterDesc.m_iDamage = 15;
			MonsterDesc.MonsterDesc.m_iShield = MonsterDesc.MonsterDesc.m_iMaxShield = 0;
			MonsterDesc.MonsterDesc.TransformDesc.fSpeedPerSec = 5.f;
			MonsterDesc.m_iCellIndex = 446;
			pMonster = dynamic_cast<CMonster*>(CGameInstance::GetInstance()->Clone_GameObjectReturnPtr_M(LEVEL_GAMEPLAY, L"Layer_Monster", L"Prototype_GameObject_Normal_Human_Sword", PivotMatrix, &MonsterDesc));
			pMonster->Set_Player(m_pPlayer);

			ZeroMemory(&MonsterDesc, sizeof(CMonster::MONSTEROPTION));
			PivotMatrix.r[3] = XMVectorSet(-56.2f, -0.9f, 71.98f, 1.f);
			MonsterDesc.m_bFirstSpawnType[CMonster::STATE_NODETECTED] = true;
			MonsterDesc.MonsterDesc.m_iHP = MonsterDesc.MonsterDesc.m_iMaxHP = 300;
			MonsterDesc.MonsterDesc.m_iDamage = 15;
			MonsterDesc.MonsterDesc.m_iShield = MonsterDesc.MonsterDesc.m_iMaxShield = 0;
			MonsterDesc.MonsterDesc.TransformDesc.fSpeedPerSec = 5.f;
			MonsterDesc.m_iCellIndex = 448;
			pMonster = dynamic_cast<CMonster*>(CGameInstance::GetInstance()->Clone_GameObjectReturnPtr_M(LEVEL_GAMEPLAY, L"Layer_Monster", L"Prototype_GameObject_Normal_Human_Sword", PivotMatrix, &MonsterDesc));
			pMonster->Set_Player(m_pPlayer);

			ZeroMemory(&MonsterDesc, sizeof(CMonster::MONSTEROPTION));
			PivotMatrix.r[3] = XMVectorSet(-56.8f, -1.f, 76.5f, 1.f);
			MonsterDesc.m_bFirstSpawnType[CMonster::STATE_NODETECTED] = true;
			MonsterDesc.MonsterDesc.m_iHP = MonsterDesc.MonsterDesc.m_iMaxHP = 300;
			MonsterDesc.MonsterDesc.m_iDamage = 15;
			MonsterDesc.MonsterDesc.m_iShield = MonsterDesc.MonsterDesc.m_iMaxShield = 0;
			MonsterDesc.MonsterDesc.TransformDesc.fSpeedPerSec = 5.f;
			MonsterDesc.m_iCellIndex = 449;
			pMonster = dynamic_cast<CMonster*>(CGameInstance::GetInstance()->Clone_GameObjectReturnPtr_M(LEVEL_GAMEPLAY, L"Layer_Monster", L"Prototype_GameObject_Normal_Human_Sword", PivotMatrix, &MonsterDesc));
			pMonster->Set_Player(m_pPlayer);

			ZeroMemory(&MonsterDesc, sizeof(CMonster::MONSTEROPTION));
			PivotMatrix.r[3] = XMVectorSet(-59.2f, -0.9f, 78.5f, 1.f);
			MonsterDesc.m_bFirstSpawnType[CMonster::STATE_NODETECTED] = true;
			MonsterDesc.MonsterDesc.m_iHP = MonsterDesc.MonsterDesc.m_iMaxHP = 300;
			MonsterDesc.MonsterDesc.m_iDamage = 15;
			MonsterDesc.MonsterDesc.m_iShield = MonsterDesc.MonsterDesc.m_iMaxShield = 0;
			MonsterDesc.MonsterDesc.TransformDesc.fSpeedPerSec = 5.f;
			MonsterDesc.m_iCellIndex = 449;
			pMonster = dynamic_cast<CMonster*>(CGameInstance::GetInstance()->Clone_GameObjectReturnPtr_M(LEVEL_GAMEPLAY, L"Layer_Monster", L"Prototype_GameObject_Normal_Human_Sword", PivotMatrix, &MonsterDesc));
			pMonster->Set_Player(m_pPlayer);

			ZeroMemory(&MonsterDesc, sizeof(CMonster::MONSTEROPTION));
			PivotMatrix.r[3] = XMVectorSet(-68.f, -0.9f, 85.6f, 1.f);
			MonsterDesc.m_bFirstSpawnType[CMonster::STATE_NODETECTED] = true;
			MonsterDesc.MonsterDesc.m_iHP = MonsterDesc.MonsterDesc.m_iMaxHP = 300;
			MonsterDesc.MonsterDesc.m_iDamage = 15;
			MonsterDesc.MonsterDesc.m_iShield = MonsterDesc.MonsterDesc.m_iMaxShield = 0;
			MonsterDesc.MonsterDesc.TransformDesc.fSpeedPerSec = 5.f;
			MonsterDesc.m_iCellIndex = 455;
			pMonster = dynamic_cast<CMonster*>(CGameInstance::GetInstance()->Clone_GameObjectReturnPtr_M(LEVEL_GAMEPLAY, L"Layer_Monster", L"Prototype_GameObject_Normal_Human_Sword", PivotMatrix, &MonsterDesc));
			pMonster->Set_Player(m_pPlayer);

			ZeroMemory(&MonsterDesc, sizeof(CMonster::MONSTEROPTION));
			PivotMatrix.r[3] = XMVectorSet(-63.3f, -0.97f, 87.2f, 1.f);
			MonsterDesc.m_bFirstSpawnType[CMonster::STATE_NODETECTED] = true;
			MonsterDesc.MonsterDesc.m_iHP = MonsterDesc.MonsterDesc.m_iMaxHP = 300;
			MonsterDesc.MonsterDesc.m_iDamage = 15;
			MonsterDesc.MonsterDesc.m_iShield = MonsterDesc.MonsterDesc.m_iMaxShield = 0;
			MonsterDesc.MonsterDesc.TransformDesc.fSpeedPerSec = 5.f;
			MonsterDesc.m_iCellIndex = 453;
			pMonster = dynamic_cast<CMonster*>(CGameInstance::GetInstance()->Clone_GameObjectReturnPtr_M(LEVEL_GAMEPLAY, L"Layer_Monster", L"Prototype_GameObject_Normal_Human_Sword", PivotMatrix, &MonsterDesc));
			pMonster->Set_Player(m_pPlayer);

			ZeroMemory(&MonsterDesc, sizeof(CMonster::MONSTEROPTION));
			PivotMatrix.r[3] = XMVectorSet(-57.1f, -0.97f, 83.7f, 1.f);
			MonsterDesc.m_bFirstSpawnType[CMonster::STATE_NODETECTED] = true;
			MonsterDesc.MonsterDesc.m_iHP = MonsterDesc.MonsterDesc.m_iMaxHP = 300;
			MonsterDesc.MonsterDesc.m_iDamage = 15;
			MonsterDesc.MonsterDesc.m_iShield = MonsterDesc.MonsterDesc.m_iMaxShield = 0;
			MonsterDesc.MonsterDesc.TransformDesc.fSpeedPerSec = 5.f;
			MonsterDesc.m_iCellIndex = 453;
			pMonster = dynamic_cast<CMonster*>(CGameInstance::GetInstance()->Clone_GameObjectReturnPtr_M(LEVEL_GAMEPLAY, L"Layer_Monster", L"Prototype_GameObject_Normal_Human_Sword", PivotMatrix, &MonsterDesc));
			pMonster->Set_Player(m_pPlayer);

			ZeroMemory(&MonsterDesc, sizeof(CMonster::MONSTEROPTION));
			PivotMatrix.r[3] = XMVectorSet(-63.2f, 0.95f, 99.3f, 1.f);
			MonsterDesc.m_bFirstSpawnType[CMonster::STATE_NODETECTED] = true;
			MonsterDesc.MonsterDesc.m_iHP = MonsterDesc.MonsterDesc.m_iMaxHP = 300;
			MonsterDesc.MonsterDesc.m_iDamage = 15;
			MonsterDesc.MonsterDesc.m_iShield = MonsterDesc.MonsterDesc.m_iMaxShield = 0;
			MonsterDesc.MonsterDesc.TransformDesc.fSpeedPerSec = 5.f;
			MonsterDesc.m_iCellIndex = 466;
			pMonster = dynamic_cast<CMonster*>(CGameInstance::GetInstance()->Clone_GameObjectReturnPtr_M(LEVEL_GAMEPLAY, L"Layer_Monster", L"Prototype_GameObject_Normal_Human_Sword", PivotMatrix, &MonsterDesc));
			pMonster->Set_Player(m_pPlayer);

			ZeroMemory(&MonsterDesc, sizeof(CMonster::MONSTEROPTION));
			PivotMatrix.r[3] = XMVectorSet(-59.566f, -0.961f, 74.761f, 1.f);
			MonsterDesc.m_bFirstSpawnType[CMonster::STATE_NODETECTED] = true;
			MonsterDesc.MonsterDesc.m_iHP = MonsterDesc.MonsterDesc.m_iMaxHP = 300;
			MonsterDesc.MonsterDesc.m_iDamage = 15;
			MonsterDesc.MonsterDesc.m_iShield = MonsterDesc.MonsterDesc.m_iMaxShield = 0;
			MonsterDesc.MonsterDesc.TransformDesc.fSpeedPerSec = 5.f;
			MonsterDesc.m_iCellIndex = 448;
			pMonster = dynamic_cast<CMonster*>(CGameInstance::GetInstance()->Clone_GameObjectReturnPtr_M(LEVEL_GAMEPLAY, L"Layer_Monster", L"Prototype_GameObject_Normal_Human_Spear", PivotMatrix, &MonsterDesc));
			pMonster->Set_Player(m_pPlayer);

			ZeroMemory(&MonsterDesc, sizeof(CMonster::MONSTEROPTION));
			PivotMatrix.r[3] = XMVectorSet(-57.4f, -0.9f, 67.2f, 1.f);
			MonsterDesc.m_bFirstSpawnType[CMonster::STATE_NODETECTED] = true;
			MonsterDesc.MonsterDesc.m_iHP = MonsterDesc.MonsterDesc.m_iMaxHP = 300;
			MonsterDesc.MonsterDesc.m_iDamage = 15;
			MonsterDesc.MonsterDesc.m_iShield = MonsterDesc.MonsterDesc.m_iMaxShield = 0;
			MonsterDesc.MonsterDesc.TransformDesc.fSpeedPerSec = 5.f;
			MonsterDesc.m_iCellIndex = 443;
			pMonster = dynamic_cast<CMonster*>(CGameInstance::GetInstance()->Clone_GameObjectReturnPtr_M(LEVEL_GAMEPLAY, L"Layer_Monster", L"Prototype_GameObject_Normal_Human_Spear", PivotMatrix, &MonsterDesc));
			pMonster->Set_Player(m_pPlayer);

			ZeroMemory(&MonsterDesc, sizeof(CMonster::MONSTEROPTION));
			PivotMatrix.r[3] = XMVectorSet(-67.2f, 0.96f, 93.4f, 1.f);
			MonsterDesc.m_bFirstSpawnType[CMonster::STATE_NODETECTED] = true;
			MonsterDesc.MonsterDesc.m_iHP = MonsterDesc.MonsterDesc.m_iMaxHP = 300;
			MonsterDesc.MonsterDesc.m_iDamage = 15;
			MonsterDesc.MonsterDesc.m_iShield = MonsterDesc.MonsterDesc.m_iMaxShield = 0;
			MonsterDesc.MonsterDesc.TransformDesc.fSpeedPerSec = 5.f;
			MonsterDesc.m_iCellIndex = 460;
			pMonster = dynamic_cast<CMonster*>(CGameInstance::GetInstance()->Clone_GameObjectReturnPtr_M(LEVEL_GAMEPLAY, L"Layer_Monster", L"Prototype_GameObject_Normal_Human_Spear", PivotMatrix, &MonsterDesc));
			pMonster->Set_Player(m_pPlayer);

			ZeroMemory(&MonsterDesc, sizeof(CMonster::MONSTEROPTION));
			PivotMatrix.r[3] = XMVectorSet(-62.4f, -0.9f, 79.65f, 1.f);
			MonsterDesc.m_bFirstSpawnType[CMonster::STATE_ALREADYSPAWN] = true;
			MonsterDesc.MonsterDesc.m_iHP = MonsterDesc.MonsterDesc.m_iMaxHP = 300;
			MonsterDesc.MonsterDesc.m_iDamage = 15;
			MonsterDesc.MonsterDesc.m_iShield = MonsterDesc.MonsterDesc.m_iMaxShield = 0;
			MonsterDesc.MonsterDesc.TransformDesc.fSpeedPerSec = 2.5f;
			MonsterDesc.MonsterDesc.TransformDesc.fRotationPerSec = XMConvertToRadians(180.f);
			MonsterDesc.m_iCellIndex = 449;
			pMonster = dynamic_cast<CMonster*>(CGameInstance::GetInstance()->Clone_GameObjectReturnPtr_M(LEVEL_GAMEPLAY, L"Layer_Monster", L"Prototype_GameObject_Normal_Human_Bow", PivotMatrix, &MonsterDesc));
			pMonster->Set_Player(m_pPlayer);

			ZeroMemory(&MonsterDesc, sizeof(CMonster::MONSTEROPTION));
			PivotMatrix.r[3] = XMVectorSet(-63.2f, 0.95f, 99.3f, 1.f);
			MonsterDesc.m_bFirstSpawnType[CMonster::STATE_ALREADYSPAWN] = true;
			MonsterDesc.MonsterDesc.m_iHP = MonsterDesc.MonsterDesc.m_iMaxHP = 300;
			MonsterDesc.MonsterDesc.m_iDamage = 15;
			MonsterDesc.MonsterDesc.m_iShield = MonsterDesc.MonsterDesc.m_iMaxShield = 0;
			MonsterDesc.MonsterDesc.TransformDesc.fSpeedPerSec = 2.5f;
			MonsterDesc.MonsterDesc.TransformDesc.fRotationPerSec = XMConvertToRadians(180.f);
			MonsterDesc.m_iCellIndex = 466;
			pMonster = dynamic_cast<CMonster*>(CGameInstance::GetInstance()->Clone_GameObjectReturnPtr_M(LEVEL_GAMEPLAY, L"Layer_Monster", L"Prototype_GameObject_Normal_Human_Bow", PivotMatrix, &MonsterDesc));
			pMonster->Set_Player(m_pPlayer);

			ZeroMemory(&MonsterDesc, sizeof(CMonster::MONSTEROPTION));
			PivotMatrix.r[3] = XMVectorSet(-51.5f, -0.94f, 85.2f, 1.f);
			MonsterDesc.m_bFirstSpawnType[CMonster::STATE_ALREADYSPAWN] = true;			// No Detected == Patrol
			MonsterDesc.MonsterDesc.m_iHP = MonsterDesc.MonsterDesc.m_iMaxHP = 300;
			MonsterDesc.MonsterDesc.m_iDamage = 15;
			MonsterDesc.MonsterDesc.m_iShield = MonsterDesc.MonsterDesc.m_iMaxShield = 0;
			MonsterDesc.MonsterDesc.TransformDesc.fSpeedPerSec = 2.5f;
			MonsterDesc.MonsterDesc.TransformDesc.fRotationPerSec = XMConvertToRadians(180.f);
			MonsterDesc.m_iCellIndex = 452;
			pMonster = dynamic_cast<CMonster*>(CGameInstance::GetInstance()->Clone_GameObjectReturnPtr_M(LEVEL_GAMEPLAY, L"Layer_Monster", L"Prototype_GameObject_Normal_Human_Granade", PivotMatrix, &MonsterDesc));
			pMonster->Set_Player(m_pPlayer);

			ZeroMemory(&MonsterDesc, sizeof(CMonster::MONSTEROPTION));
			PivotMatrix.r[3] = XMVectorSet(-70.f, 1.49f, 102.3f, 1.f);
			MonsterDesc.m_bFirstSpawnType[CMonster::STATE_ALREADYSPAWN] = true;			// No Detected == Patrol
			MonsterDesc.MonsterDesc.m_iHP = MonsterDesc.MonsterDesc.m_iMaxHP = 300;
			MonsterDesc.MonsterDesc.m_iDamage = 15;
			MonsterDesc.MonsterDesc.m_iShield = MonsterDesc.MonsterDesc.m_iMaxShield = 0;
			MonsterDesc.MonsterDesc.TransformDesc.fSpeedPerSec = 2.5f;
			MonsterDesc.MonsterDesc.TransformDesc.fRotationPerSec = XMConvertToRadians(180.f);
			MonsterDesc.m_iCellIndex = 480;
			pMonster = dynamic_cast<CMonster*>(CGameInstance::GetInstance()->Clone_GameObjectReturnPtr_M(LEVEL_GAMEPLAY, L"Layer_Monster", L"Prototype_GameObject_Normal_Human_Granade", PivotMatrix, &MonsterDesc));
			pMonster->Set_Player(m_pPlayer);

			ZeroMemory(&MonsterDesc, sizeof(CMonster::MONSTEROPTION));
			PivotMatrix.r[3] = XMVectorSet(-68.9f, -0.9f, 83.5f, 1.f);
			MonsterDesc.m_bFirstSpawnType[CMonster::STATE_ALREADYSPAWN] = true;
			MonsterDesc.MonsterDesc.m_iHP = MonsterDesc.MonsterDesc.m_iMaxHP = 300;
			MonsterDesc.MonsterDesc.m_iDamage = 15;
			MonsterDesc.MonsterDesc.m_iShield = MonsterDesc.MonsterDesc.m_iMaxShield = 0;
			MonsterDesc.MonsterDesc.TransformDesc.fSpeedPerSec = 2.5f;
			MonsterDesc.MonsterDesc.TransformDesc.fRotationPerSec = XMConvertToRadians(180.f);
			MonsterDesc.m_iCellIndex = 455;
			pMonster = dynamic_cast<CMonster*>(CGameInstance::GetInstance()->Clone_GameObjectReturnPtr_M(LEVEL_GAMEPLAY, L"Layer_Monster", L"Prototype_GameObject_Normal_Human_Explode", PivotMatrix, &MonsterDesc));
			pMonster->Set_Player(m_pPlayer);

			ZeroMemory(&MonsterDesc, sizeof(CMonster::MONSTEROPTION));
			PivotMatrix.r[3] = XMVectorSet(-70.7f, 1.49f, 102.3f, 1.f);
			MonsterDesc.m_bFirstSpawnType[CMonster::STATE_NODETECTED] = true;
			MonsterDesc.MonsterDesc.m_iHP = MonsterDesc.MonsterDesc.m_iMaxHP = 300;
			MonsterDesc.MonsterDesc.m_iDamage = 15;
			MonsterDesc.MonsterDesc.m_iShield = MonsterDesc.MonsterDesc.m_iMaxShield = 0;
			MonsterDesc.MonsterDesc.TransformDesc.fSpeedPerSec = 2.5f;
			MonsterDesc.MonsterDesc.TransformDesc.fRotationPerSec = XMConvertToRadians(180.f);
			MonsterDesc.m_iCellIndex = 479;
			pMonster = dynamic_cast<CMonster*>(CGameInstance::GetInstance()->Clone_GameObjectReturnPtr_M(LEVEL_GAMEPLAY, L"Layer_Monster", L"Prototype_GameObject_Normal_Elite_Bug", PivotMatrix, &MonsterDesc));
			pMonster->Set_Player(m_pPlayer);

			m_bSpawnTrigger = true;

		}
		return S_OK;
	}
	else
	{
		m_pPlayer->Set_PortalUI(false);
	}

	return S_OK;
}

HRESULT CTrigger::Bug_To_Room_C()
{
	CCollider* pCollider = (CCollider*)m_pPlayer->Find_Component(L"Com_BODYSPHERE");
	NULL_CHECK_RETURN(pCollider, E_FAIL);
	if (m_pTriggerCollCom->Collision(pCollider))
	{
		m_pPlayer->Set_PortalUI(true);

		if (CGameInstance::GetInstance()->Key_Down(DIK_F))
		{
			m_pPlayer->Set_RoomType(CPlayer::ROOM_C);
			_matrix PlayerMat = m_pPlayer->Get_WorldFloat4x4();
			PlayerMat.r[3] = XMVectorSet( -32.028f, 0.03f, 62.767f, 1.f);
			m_pPlayer->Set_CellIndex(279);
			m_pPlayer->Set_WorldMatrix(PlayerMat);

			CMonster::MONSTEROPTION MonsterDesc;
			CMonster* pMonster = nullptr;
			_matrix PivotMatrix = XMMatrixIdentity();

			ZeroMemory(&MonsterDesc, sizeof(CMonster::MONSTEROPTION));
			PivotMatrix.r[3] = XMVectorSet(-44.7f, 0.f, 72.2f, 1.f);
			MonsterDesc.m_bFirstSpawnType[CMonster::STATE_ALREADYSPAWN] = true;
			MonsterDesc.MonsterDesc.m_iHP = MonsterDesc.MonsterDesc.m_iMaxHP = 300;
			MonsterDesc.MonsterDesc.m_iDamage = 15;
			MonsterDesc.MonsterDesc.m_iShield = MonsterDesc.MonsterDesc.m_iMaxShield = 0;
			MonsterDesc.MonsterDesc.TransformDesc.fSpeedPerSec = 5.f;
			MonsterDesc.m_iCellIndex = 294;
			pMonster = dynamic_cast<CMonster*>(CGameInstance::GetInstance()->Clone_GameObjectReturnPtr_M(LEVEL_GAMEPLAY, L"Layer_Monster", L"Prototype_GameObject_Normal_Human_Sword", PivotMatrix, &MonsterDesc));
			pMonster->Set_Player(m_pPlayer);

			ZeroMemory(&MonsterDesc, sizeof(CMonster::MONSTEROPTION));
			PivotMatrix.r[3] = XMVectorSet(-37.7f, -1.4f, 80.7f, 1.f);
			MonsterDesc.m_bFirstSpawnType[CMonster::STATE_ALREADYSPAWN] = true;
			MonsterDesc.MonsterDesc.m_iHP = MonsterDesc.MonsterDesc.m_iMaxHP = 300;
			MonsterDesc.MonsterDesc.m_iDamage = 15;
			MonsterDesc.MonsterDesc.m_iShield = MonsterDesc.MonsterDesc.m_iMaxShield = 0;
			MonsterDesc.MonsterDesc.TransformDesc.fSpeedPerSec = 5.f;
			MonsterDesc.m_iCellIndex = 323;
			pMonster = dynamic_cast<CMonster*>(CGameInstance::GetInstance()->Clone_GameObjectReturnPtr_M(LEVEL_GAMEPLAY, L"Layer_Monster", L"Prototype_GameObject_Normal_Human_Spear", PivotMatrix, &MonsterDesc));
			pMonster->Set_Player(m_pPlayer);

			ZeroMemory(&MonsterDesc, sizeof(CMonster::MONSTEROPTION));
			PivotMatrix.r[3] = XMVectorSet(-21.6f, 0.f, 65.7f, 1.f);
			MonsterDesc.m_bFirstSpawnType[CMonster::STATE_ALREADYSPAWN] = true;
			MonsterDesc.MonsterDesc.m_iHP = MonsterDesc.MonsterDesc.m_iMaxHP = 300;
			MonsterDesc.MonsterDesc.m_iDamage = 15;
			MonsterDesc.MonsterDesc.m_iShield = MonsterDesc.MonsterDesc.m_iMaxShield = 0;
			MonsterDesc.MonsterDesc.TransformDesc.fSpeedPerSec = 2.5f;
			MonsterDesc.MonsterDesc.TransformDesc.fRotationPerSec = XMConvertToRadians(180.f);
			MonsterDesc.m_iCellIndex = 282;
			pMonster = dynamic_cast<CMonster*>(CGameInstance::GetInstance()->Clone_GameObjectReturnPtr_M(LEVEL_GAMEPLAY, L"Layer_Monster", L"Prototype_GameObject_Normal_Human_Bow", PivotMatrix, &MonsterDesc));
			pMonster->Set_Player(m_pPlayer);

			ZeroMemory(&MonsterDesc, sizeof(CMonster::MONSTEROPTION));
			PivotMatrix.r[3] = XMVectorSet(-45.3f, 0.f, 66.4f, 1.f);
			MonsterDesc.m_bFirstSpawnType[CMonster::STATE_ALREADYSPAWN] = true;
			MonsterDesc.MonsterDesc.m_iHP = MonsterDesc.MonsterDesc.m_iMaxHP = 300;
			MonsterDesc.MonsterDesc.m_iDamage = 15;
			MonsterDesc.MonsterDesc.m_iShield = MonsterDesc.MonsterDesc.m_iMaxShield = 0;
			MonsterDesc.MonsterDesc.TransformDesc.fSpeedPerSec = 2.5f;
			MonsterDesc.MonsterDesc.TransformDesc.fRotationPerSec = XMConvertToRadians(180.f);
			MonsterDesc.m_iCellIndex = 291;
			pMonster = dynamic_cast<CMonster*>(CGameInstance::GetInstance()->Clone_GameObjectReturnPtr_M(LEVEL_GAMEPLAY, L"Layer_Monster", L"Prototype_GameObject_Normal_Human_Bow", PivotMatrix, &MonsterDesc));
			pMonster->Set_Player(m_pPlayer);

			ZeroMemory(&MonsterDesc, sizeof(CMonster::MONSTEROPTION));
			PivotMatrix.r[3] = XMVectorSet(-29.8f, -1.4f, 81.1f, 1.f);
			MonsterDesc.m_bFirstSpawnType[CMonster::STATE_ALREADYSPAWN] = true;
			MonsterDesc.MonsterDesc.m_iHP = MonsterDesc.MonsterDesc.m_iMaxHP = 300;
			MonsterDesc.MonsterDesc.m_iDamage = 15;
			MonsterDesc.MonsterDesc.m_iShield = MonsterDesc.MonsterDesc.m_iMaxShield = 0;
			MonsterDesc.MonsterDesc.TransformDesc.fSpeedPerSec = 2.5f;
			MonsterDesc.MonsterDesc.TransformDesc.fRotationPerSec = XMConvertToRadians(180.f);
			MonsterDesc.m_iCellIndex = 328;
			pMonster = dynamic_cast<CMonster*>(CGameInstance::GetInstance()->Clone_GameObjectReturnPtr_M(LEVEL_GAMEPLAY, L"Layer_Monster", L"Prototype_GameObject_Normal_Human_Explode", PivotMatrix, &MonsterDesc));
			pMonster->Set_Player(m_pPlayer);

			ZeroMemory(&MonsterDesc, sizeof(CMonster::MONSTEROPTION));
			PivotMatrix.r[3] = XMVectorSet(-22.85f, -1.4f, 79.5f, 1.f);
			MonsterDesc.m_bFirstSpawnType[CMonster::STATE_NODETECTED] = true;
			MonsterDesc.MonsterDesc.m_iHP = MonsterDesc.MonsterDesc.m_iMaxHP = 300;
			MonsterDesc.MonsterDesc.m_iDamage = 15;
			MonsterDesc.MonsterDesc.m_iShield = MonsterDesc.MonsterDesc.m_iMaxShield = 0;
			MonsterDesc.MonsterDesc.TransformDesc.fSpeedPerSec = 2.5f;
			MonsterDesc.MonsterDesc.TransformDesc.fRotationPerSec = XMConvertToRadians(180.f);
			MonsterDesc.m_iCellIndex = 313;
			pMonster = dynamic_cast<CMonster*>(CGameInstance::GetInstance()->Clone_GameObjectReturnPtr_M(LEVEL_GAMEPLAY, L"Layer_Monster", L"Prototype_GameObject_Normal_Little_Bug", PivotMatrix, &MonsterDesc));
			pMonster->Set_Player(m_pPlayer);

			PivotMatrix.r[3] = XMVectorSet(-20.6f, -1.4f, 79.5f, 1.f);
			MonsterDesc.m_bFirstSpawnType[CMonster::STATE_NODETECTED] = true;
			MonsterDesc.MonsterDesc.m_iHP = MonsterDesc.MonsterDesc.m_iMaxHP = 300;
			MonsterDesc.MonsterDesc.m_iDamage = 15;
			MonsterDesc.MonsterDesc.m_iShield = MonsterDesc.MonsterDesc.m_iMaxShield = 0;
			MonsterDesc.MonsterDesc.TransformDesc.fSpeedPerSec = 2.5f;
			MonsterDesc.MonsterDesc.TransformDesc.fRotationPerSec = XMConvertToRadians(180.f);
			MonsterDesc.m_iCellIndex = 313;
			pMonster = dynamic_cast<CMonster*>(CGameInstance::GetInstance()->Clone_GameObjectReturnPtr_M(LEVEL_GAMEPLAY, L"Layer_Monster", L"Prototype_GameObject_Normal_Little_Bug", PivotMatrix, &MonsterDesc));
			pMonster->Set_Player(m_pPlayer);

			m_bSpawnTrigger = true;

		}
		return S_OK;
	}
	else
	{
		m_pPlayer->Set_PortalUI(false);
	}

	return S_OK;
}

// Normal_Boss
// m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(113.1f, 0.104f, 63.115f, 1.f));
// m_pNavigationCom->Set_CellIndex(504);
HRESULT CTrigger::Room_C_To_Boss()
{
	CCollider* pCollider = (CCollider*)m_pPlayer->Find_Component(L"Com_BODYSPHERE");
	NULL_CHECK_RETURN(pCollider, E_FAIL);
	if (m_pTriggerCollCom->Collision(pCollider))
	{
		m_pPlayer->Set_PortalUI(true);

		if (CGameInstance::GetInstance()->Key_Down(DIK_F))
		{
			CGameInstance::GetInstance()->Get_CloneObjectList(LEVEL_GAMEPLAY, L"Layer_BackGround")->front()->Set_Dead(true);

			FAILED_CHECK_RETURN(CGameInstance::GetInstance()->Clone_GameObject(LEVEL_GAMEPLAY, L"Layer_BackGround", L"Prototype_GameObject_Normal_BossMap"), E_FAIL);
			CGameInstance::GetInstance()->Set_LightRange(0, 100.f);
			_float4 vDirection;
			_matrix	RotationMatrix = XMMatrixRotationAxis(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(-221.33f));
			XMStoreFloat4(&vDirection, XMVector3TransformNormal(XMVectorSet(-cosf(XMConvertToRadians(60.f)), -sinf(XMConvertToRadians(60.f)), 0.f, 0.f), RotationMatrix));

			CGameInstance::GetInstance()->Set_LightDirection(1, vDirection);

			m_pPlayer->Set_RoomType(CPlayer::ROOM_BOSS);
			_matrix PlayerMat = m_pPlayer->Get_WorldFloat4x4();
			PlayerMat.r[3] = XMVectorSet(113.1f, 0.104f, 63.115f, 1.f);
			m_pPlayer->Set_CellIndex(504);
			m_pPlayer->Set_WorldMatrix(PlayerMat);

			CMonster::MONSTEROPTION MonsterDesc;
			CMonster* pMonster = nullptr;
			_matrix PivotMatrix = XMMatrixIdentity() * XMMatrixRotationAxis(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(180.f));
			PivotMatrix.r[3] = XMVectorSet(110.f, 1.3f, 179.f, 1.f);
			MonsterDesc.m_bFirstSpawnType[CMonster::STATE_NODETECTED] = true;
			MonsterDesc.MonsterDesc.m_iHP = MonsterDesc.MonsterDesc.m_iMaxHP = 1000;
			MonsterDesc.MonsterDesc.m_iDamage = 15;
			MonsterDesc.MonsterDesc.m_iShield = MonsterDesc.MonsterDesc.m_iMaxShield = 0;
			MonsterDesc.MonsterDesc.TransformDesc.fSpeedPerSec = 5.f;
			MonsterDesc.m_iCellIndex = 0;
			pMonster = dynamic_cast<CMonster*>(CGameInstance::GetInstance()->Clone_GameObjectReturnPtr_M(LEVEL_GAMEPLAY, L"Layer_ZBossMonster", L"Prototype_GameObject_Normal_Boss", PivotMatrix, &MonsterDesc));
			pMonster->Set_Player(m_pPlayer);

			

		}
		return S_OK;
	}
	else
	{
		m_pPlayer->Set_PortalUI(false);
	}

	return S_OK;
}

HRESULT CTrigger::SetUp_Components()
{
	FAILED_CHECK_RETURN(__super::Add_Component(CGameInstance::Get_StaticLevelIndex(), L"Prototype_Component_Renderer", L"Com_Renderer", (CComponent**)&m_pRendererCom, this), E_FAIL);

	CCollider::COLLIDERDESC			ColliderDesc;
	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));
	ColliderDesc.vSize = _float3(3.f, 3.f, 3.f);
	ColliderDesc.vPosition = _float3(0.f, 0.f, 0.f);
	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Collider_SPHERE", L"Com_ExplodeSPHERE", (CComponent**)&m_pTriggerCollCom, this, &ColliderDesc), E_FAIL);
	return S_OK;
}

CTrigger* CTrigger::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CTrigger*		pInstance = new CTrigger(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CTrigger");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CTrigger::Clone(const wstring& wstrPrototypeTag, void* pArg)
{
	CTrigger*		pInstance = new CTrigger(*this);

	if (FAILED(pInstance->Initialize_Clone(wstrPrototypeTag, pArg)))
	{
		MSG_BOX("Failed to Created : CTrigger");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CTrigger::Free()
{
	

	__super::Free();

	Safe_Release(m_pTriggerCollCom);
	Safe_Release(m_pRendererCom);
}