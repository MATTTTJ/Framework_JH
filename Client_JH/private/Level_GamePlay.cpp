#include "stdafx.h"
#include "..\public\Level_GamePlay.h"
#include "GameInstance.h"
#include "Player.h"
#include "Monster.h"
#include "Imgui_AnimationMgr.h"
#include "Imgui_PropertyEditor.h"
#include "Imgui_LevelSwitcher.h"
#include "Imgui_MapEditor.h"
#include "Imgui_ProtoMgr.h"
#include "Imgui_Setting.h"
#include "Imgui_ModelSave.h"
#include "Imgui_NavigationEditor.h"
#include "Trigger.h"

CLevel_GamePlay::CLevel_GamePlay(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CLevel(pDevice, pContext)
{

}

HRESULT CLevel_GamePlay::Initialize()
{
	FAILED_CHECK_RETURN(__super::Initialize(), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Light(), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Layer_Player(L"Layer_Player"), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Layer_BackGround(L"Layer_BackGround"), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Layer_Camera(L"Layer_ZCamera"), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Layer_Monster(L"Layer_Monster"), E_FAIL);
	// FAILED_CHECK_RETURN(Ready_Layer_Effect(L"Layer_Effect"), E_FAIL);
	// FAILED_CHECK_RETURN(Ready_Layer_BossMonster(L"Layer_ZBossMonster"), E_FAIL);
	
		FAILED_CHECK_RETURN(Ready_Layer_Trigger(L"Layer_Trigger"), E_FAIL);

	// FAILED_CHECK_RETURN(Ready_Layer_UI(L"Layer_UI"), E_FAIL);

	return S_OK;
}

void CLevel_GamePlay::Tick(_double TimeDelta)
{
	__super::Tick(TimeDelta);
}

void CLevel_GamePlay::Late_Tick(_double TimeDelta)
{
	__super::Late_Tick(TimeDelta);

	// 여기서 오픈레벨 구현해야함

}

HRESULT CLevel_GamePlay::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	SetWindowText(g_hWnd, TEXT("Level : GAMEPLAY"));

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Light()
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	LIGHTDESC			LightDesc;
	ZeroMemory(&LightDesc, sizeof LightDesc);

	LightDesc.eType = LIGHTDESC::LIGHT_DIRECTIONAL;
	LightDesc.isEnable = true;
	LightDesc.vDirection = _float4(1.f, -1.f, 1.0f, 0.f);
	LightDesc.vDiffuse = _float4(1.f, 1.f, 1.f, 1.f);
	LightDesc.vAmbient = _float4(0.4f, 0.4f, 0.4f, 1.f);
	LightDesc.vSpecular = _float4(1.f, 1.f, 1.f, 1.f);

	if (FAILED(pGameInstance->Add_Light(m_pDevice, m_pContext, LightDesc)))
		return E_FAIL;

	// ZeroMemory(&LightDesc, sizeof LightDesc);
	//
	// LightDesc.eType = LIGHTDESC::LIGHT_POINT;
	// LightDesc.isEnable = true;
	// /*LightDesc.vDirection = _float4(1.f, -1.f, 1.0f, 0.f);*/
	// LightDesc.vPosition = _float4(5.f, 3.f, 5.f, 1.f);
	// LightDesc.fRange = 10.0f;
	// LightDesc.vDiffuse = _float4(1.f, 0.f, 0.f, 1.f);
	// LightDesc.vAmbient = _float4(0.4f, 0.2f, 0.2f, 0.2f);
	// LightDesc.vSpecular = LightDesc.vDiffuse;
	//
	// if (FAILED(pGameInstance->Add_Light(m_pDevice, m_pContext, LightDesc)))
	// 	return E_FAIL;
	//
	//
	// ZeroMemory(&LightDesc, sizeof LightDesc);
	//
	// LightDesc.eType = LIGHTDESC::LIGHT_POINT;
	// LightDesc.isEnable = true;
	// /*LightDesc.vDirection = _float4(1.f, -1.f, 1.0f, 0.f);*/
	// LightDesc.vPosition = _float4(10.f, 3.f, 5.f, 1.f);
	// LightDesc.fRange = 10.0f;
	// LightDesc.vDiffuse = _float4(0.f, 1.f, 0.f, 1.f);
	// LightDesc.vAmbient = _float4(0.2f, 0.4f, 0.2f, 0.2f);
	// LightDesc.vSpecular = LightDesc.vDiffuse;
	//
	// if (FAILED(pGameInstance->Add_Light(m_pDevice, m_pContext, LightDesc)))
	// 	return E_FAIL;


	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_BackGround(const wstring wstrLayerTag)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);
	FAILED_CHECK_RETURN(pGameInstance->Clone_GameObject(LEVEL_GAMEPLAY, wstrLayerTag, L"Prototype_GameObject_NormalMap"), E_FAIL);
	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Camera(const wstring wstrLayerTag)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	FAILED_CHECK_RETURN(pGameInstance->Clone_GameObject(LEVEL_GAMEPLAY, wstrLayerTag, L"Prototype_GameObject_Camera_Dynamic"), E_FAIL);
	FAILED_CHECK_RETURN(pGameInstance->Clone_GameObject(LEVEL_GAMEPLAY, wstrLayerTag, L"Prototype_GameObject_Camera_Static"), E_FAIL);

	RELEASE_INSTANCE(CGameInstance);
	
	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Monster(const wstring wstrLayerTag)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);
	CMonster*			pMonster = nullptr;
	CPlayer*			pPlayer = dynamic_cast<CPlayer*>(pGameInstance->Get_CloneObjectList(LEVEL_GAMEPLAY, L"Layer_Player")->front());
	NULL_CHECK_RETURN(pPlayer, E_FAIL);

	_matrix PivotMatrix = XMMatrixIdentity() * XMMatrixRotationAxis(XMVectorSet(0.f,1.f,0.f, 0.f),XMConvertToRadians(180.f));
	// PivotMatrix.r[3] = XMVectorSet(110.f, 1.3f, 179.f, 1.f);
	// // SWORD
	// PivotMatrix.r[3] = XMVectorSet(-14.f, 0.f, 0.8f, 1.f);
	// CMonster::MONSTEROPTION			MonsterDesc;
	// ZeroMemory(&MonsterDesc, sizeof(CMonster::MONSTEROPTION));
	// MonsterDesc.m_bFirstSpawnType[CMonster::STATE_ALREADYSPAWN] = true;
	// MonsterDesc.MonsterDesc.m_iHP = MonsterDesc.MonsterDesc.m_iMaxHP = 300;
	// MonsterDesc.MonsterDesc.m_iDamage = 15;
	// MonsterDesc.MonsterDesc.m_iShield = MonsterDesc.MonsterDesc.m_iMaxShield = 0;
	// MonsterDesc.MonsterDesc.TransformDesc.fSpeedPerSec = 5.f;
	// MonsterDesc.m_iCellIndex = 40;
	// pMonster = dynamic_cast<CMonster*>(pGameInstance->Clone_GameObjectReturnPtr_M(LEVEL_GAMEPLAY, wstrLayerTag, L"Prototype_GameObject_Normal_Human_Sword", PivotMatrix, &MonsterDesc));
	// pMonster->Set_Player(pPlayer);
	//
	// ZeroMemory(&MonsterDesc, sizeof(CMonster::MONSTEROPTION));
	// PivotMatrix.r[3] = XMVectorSet(-11.8f, 0.f, 8.8f, 1.f);
	// MonsterDesc.m_bFirstSpawnType[CMonster::STATE_ALREADYSPAWN] = true;
	// MonsterDesc.MonsterDesc.m_iHP = MonsterDesc.MonsterDesc.m_iMaxHP = 300;
	// MonsterDesc.MonsterDesc.m_iDamage = 15;
	// MonsterDesc.MonsterDesc.m_iShield = MonsterDesc.MonsterDesc.m_iMaxShield = 0;
	// MonsterDesc.MonsterDesc.TransformDesc.fSpeedPerSec = 5.f;
	// MonsterDesc.m_iCellIndex = 50;
	// pMonster = dynamic_cast<CMonster*>(pGameInstance->Clone_GameObjectReturnPtr_M(LEVEL_GAMEPLAY, wstrLayerTag, L"Prototype_GameObject_Normal_Human_Sword", PivotMatrix, &MonsterDesc));
	// pMonster->Set_Player(pPlayer);
	//
	// ZeroMemory(&MonsterDesc, sizeof(CMonster::MONSTEROPTION));
	// PivotMatrix.r[3] = XMVectorSet(-4.45f, 0.f, 17.8f, 1.f);
	// MonsterDesc.m_bFirstSpawnType[CMonster::STATE_ALREADYSPAWN] = true;
	// MonsterDesc.MonsterDesc.m_iHP = MonsterDesc.MonsterDesc.m_iMaxHP = 300;
	// MonsterDesc.MonsterDesc.m_iDamage = 15;
	// MonsterDesc.MonsterDesc.m_iShield = MonsterDesc.MonsterDesc.m_iMaxShield = 0;
	// MonsterDesc.MonsterDesc.TransformDesc.fSpeedPerSec = 5.f;
	// MonsterDesc.m_iCellIndex = 91;
	// pMonster = dynamic_cast<CMonster*>(pGameInstance->Clone_GameObjectReturnPtr_M(LEVEL_GAMEPLAY, wstrLayerTag, L"Prototype_GameObject_Normal_Human_Sword", PivotMatrix, &MonsterDesc));
	// pMonster->Set_Player(pPlayer);
	// //
	// ZeroMemory(&MonsterDesc, sizeof(CMonster::MONSTEROPTION));
	// PivotMatrix.r[3] = XMVectorSet(-5.7f, 1.1f, 7.5f, 1.f);
	// MonsterDesc.m_bFirstSpawnType[CMonster::STATE_ALREADYSPAWN] = true;
	// MonsterDesc.MonsterDesc.m_iHP = MonsterDesc.MonsterDesc.m_iMaxHP = 300;
	// MonsterDesc.MonsterDesc.m_iDamage = 15;
	// MonsterDesc.MonsterDesc.m_iShield = MonsterDesc.MonsterDesc.m_iMaxShield = 0;
	// MonsterDesc.MonsterDesc.TransformDesc.fSpeedPerSec = 5.f;
	// MonsterDesc.m_iCellIndex = 100;
	// pMonster = dynamic_cast<CMonster*>(pGameInstance->Clone_GameObjectReturnPtr_M(LEVEL_GAMEPLAY, wstrLayerTag, L"Prototype_GameObject_Normal_Human_Sword", PivotMatrix, &MonsterDesc));
	// pMonster->Set_Player(pPlayer);
	// // 그라운드 스폰
	// ZeroMemory(&MonsterDesc, sizeof(CMonster::MONSTEROPTION));
	// PivotMatrix.r[3] = XMVectorSet(-10.1f, 0.28f, 24.f, 1.f);
	// MonsterDesc.m_bFirstSpawnType[CMonster::STATE_ALREADYSPAWN] = true;
	// MonsterDesc.MonsterDesc.m_iHP = MonsterDesc.MonsterDesc.m_iMaxHP = 300;
	// MonsterDesc.MonsterDesc.m_iDamage = 15;
	// MonsterDesc.MonsterDesc.m_iShield = MonsterDesc.MonsterDesc.m_iMaxShield = 0;
	// MonsterDesc.MonsterDesc.TransformDesc.fSpeedPerSec = 5.f;
	// MonsterDesc.m_iCellIndex = 186;
	// pMonster = dynamic_cast<CMonster*>(pGameInstance->Clone_GameObjectReturnPtr_M(LEVEL_GAMEPLAY, wstrLayerTag, L"Prototype_GameObject_Normal_Human_Sword", PivotMatrix, &MonsterDesc));
	// pMonster->Set_Player(pPlayer);
	// // ~그라운드 스폰
	//

	//
	// // 그라운드 스폰
	
	// // ~그라운드 스폰
	//
	
	//
	
	//
	
	//
	
	//
	
	//
	
	//
	
	//
	
	//
	
	// //
	// // CMonster::MONSTEROPTION			MonsterDesc;
	// // MonsterDesc.m_bFirstSpawnType[CMonster::STATE_GROUNDSPAWN] = true;
	// // MonsterDesc.MonsterDesc.m_iHP = MonsterDesc.MonsterDesc.m_iMaxHP = 300;
	// // MonsterDesc.MonsterDesc.m_iDamage = 15;
	// // MonsterDesc.MonsterDesc.m_iShield = MonsterDesc.MonsterDesc.m_iMaxShield = 0;
	// // MonsterDesc.MonsterDesc.TransformDesc.fSpeedPerSec = 5.f;
	// // MonsterDesc.m_iCellIndex = 40;
	// // pMonster = dynamic_cast<CMonster*>(pGameInstance->Clone_GameObjectReturnPtr_M(LEVEL_GAMEPLAY, wstrLayerTag, L"Prototype_GameObject_Normal_Human_Sword", PivotMatrix, &MonsterDesc));
	// // pMonster->Set_Player(pPlayer);
	//
	// // CMonster::MONSTEROPTION			MonsterDesc;
	// // MonsterDesc.m_bFirstSpawnType[CMonster::STATE_NODETECTED] = true;
	// // MonsterDesc.MonsterDesc.m_iHP = MonsterDesc.MonsterDesc.m_iMaxHP = 300;
	// // MonsterDesc.MonsterDesc.m_iDamage = 15;
	// // MonsterDesc.MonsterDesc.m_iShield = MonsterDesc.MonsterDesc.m_iMaxShield = 0;
	// // MonsterDesc.MonsterDesc.TransformDesc.fSpeedPerSec = 5.f;
	// // MonsterDesc.m_iCellIndex = 40;
	// // pMonster = dynamic_cast<CMonster*>(pGameInstance->Clone_GameObjectReturnPtr_M(LEVEL_GAMEPLAY, wstrLayerTag, L"Prototype_GameObject_Normal_Human_Sword", PivotMatrix, &MonsterDesc));
	// // pMonster->Set_Player(pPlayer);
	// // ~SWORD
	// ////////////////////////////
	// // SPEAR
	// // CMonster::MONSTEROPTION			MonsterDesc;
	// // MonsterDesc.m_bFirstSpawnType[CMonster::STATE_NODETECTED] = true;
	// // MonsterDesc.MonsterDesc.m_iHP = MonsterDesc.MonsterDesc.m_iMaxHP = 300;
	// // MonsterDesc.MonsterDesc.m_iDamage = 15;
	// // MonsterDesc.MonsterDesc.m_iShield = MonsterDesc.MonsterDesc.m_iMaxShield = 0;
	// // MonsterDesc.MonsterDesc.TransformDesc.fSpeedPerSec = 5.f;
	// // MonsterDesc.m_iCellIndex = 40;
	// // pMonster = dynamic_cast<CMonster*>(pGameInstance->Clone_GameObjectReturnPtr_M(LEVEL_GAMEPLAY, wstrLayerTag, L"Prototype_GameObject_Normal_Human_Spear", PivotMatrix, &MonsterDesc));
	// // pMonster->Set_Player(pPlayer);
	//
	// // CMonster::MONSTEROPTION			MonsterDesc;
	// // MonsterDesc.m_bFirstSpawnType[CMonster::STATE_UPSPAWN] = true;
	// // MonsterDesc.MonsterDesc.m_iHP = MonsterDesc.MonsterDesc.m_iMaxHP = 300;
	// // MonsterDesc.MonsterDesc.m_iDamage = 15;
	// // MonsterDesc.MonsterDesc.m_iShield = MonsterDesc.MonsterDesc.m_iMaxShield = 0;
	// // MonsterDesc.MonsterDesc.TransformDesc.fSpeedPerSec = 5.f;
	// // MonsterDesc.m_iCellIndex = 40;
	// // pMonster = dynamic_cast<CMonster*>(pGameInstance->Clone_GameObjectReturnPtr_M(LEVEL_GAMEPLAY, wstrLayerTag, L"Prototype_GameObject_Normal_Human_Spear", PivotMatrix, &MonsterDesc));
	// // pMonster->Set_Player(pPlayer);
	// ZeroMemory(&MonsterDesc, sizeof(CMonster::MONSTEROPTION));
	// PivotMatrix.r[3] = XMVectorSet(-5.8f, 0.f, 29.2f, 1.f);
	// MonsterDesc.m_bFirstSpawnType[CMonster::STATE_ALREADYSPAWN] = true;
	// MonsterDesc.MonsterDesc.m_iHP = MonsterDesc.MonsterDesc.m_iMaxHP = 300;
	// MonsterDesc.MonsterDesc.m_iDamage = 15;
	// MonsterDesc.MonsterDesc.m_iShield = MonsterDesc.MonsterDesc.m_iMaxShield = 0;
	// MonsterDesc.MonsterDesc.TransformDesc.fSpeedPerSec = 5.f;
	// MonsterDesc.m_iCellIndex = 184;
	// pMonster = dynamic_cast<CMonster*>(pGameInstance->Clone_GameObjectReturnPtr_M(LEVEL_GAMEPLAY, wstrLayerTag, L"Prototype_GameObject_Normal_Human_Spear", PivotMatrix, &MonsterDesc));
	// pMonster->Set_Player(pPlayer);
	//

	//
	
	//
	
	//
	
	//
	
	//
	
	//
	
	//
	
	//
	
	// // ~SPEAR







	/////////////////////////
	// BOW
	// CMonster::MONSTEROPTION			MonsterDesc;
	// MonsterDesc.m_bFirstSpawnType[CMonster::STATE_NODETECTED] = true;
	// MonsterDesc.MonsterDesc.m_iHP = MonsterDesc.MonsterDesc.m_iMaxHP = 300;
	// MonsterDesc.MonsterDesc.m_iDamage = 15;
	// MonsterDesc.MonsterDesc.m_iShield = MonsterDesc.MonsterDesc.m_iMaxShield = 0;
	// MonsterDesc.MonsterDesc.TransformDesc.fSpeedPerSec = 2.5f;
	// MonsterDesc.MonsterDesc.TransformDesc.fRotationPerSec = XMConvertToRadians(180.f);
	// MonsterDesc.m_iCellIndex = 40;
	// pMonster = dynamic_cast<CMonster*>(pGameInstance->Clone_GameObjectReturnPtr_M(LEVEL_GAMEPLAY, wstrLayerTag, L"Prototype_GameObject_Normal_Human_Bow", PivotMatrix, &MonsterDesc));
	// pMonster->Set_Player(pPlayer);
	//
	//
	// CMonster::MONSTEROPTION			MonsterDesc;

	// ZeroMemory(&MonsterDesc, sizeof(CMonster::MONSTEROPTION));
	// PivotMatrix.r[3] = XMVectorSet(-15.7f, 0.f, 26.f, 1.f);
	// MonsterDesc.m_bFirstSpawnType[CMonster::STATE_ALREADYSPAWN] = true;
	// MonsterDesc.MonsterDesc.m_iHP = MonsterDesc.MonsterDesc.m_iMaxHP = 300;
	// MonsterDesc.MonsterDesc.m_iDamage = 15;
	// MonsterDesc.MonsterDesc.m_iShield = MonsterDesc.MonsterDesc.m_iMaxShield = 0;
	// MonsterDesc.MonsterDesc.TransformDesc.fSpeedPerSec = 2.5f;
	// MonsterDesc.MonsterDesc.TransformDesc.fRotationPerSec = XMConvertToRadians(180.f);
	// MonsterDesc.m_iCellIndex = 188;
	// pMonster = dynamic_cast<CMonster*>(pGameInstance->Clone_GameObjectReturnPtr_M(LEVEL_GAMEPLAY, wstrLayerTag, L"Prototype_GameObject_Normal_Human_Bow", PivotMatrix, &MonsterDesc));
	// pMonster->Set_Player(pPlayer);
	//
	// NULL_CHECK_RETURN(pMonster, E_FAIL);
	//
	
	//
	// NULL_CHECK_RETURN(pMonster, E_FAIL);
	//
	
	//
	// NULL_CHECK_RETURN(pMonster, E_FAIL);
	//
	
	//
	// NULL_CHECK_RETURN(pMonster, E_FAIL);
	//
	
	//
	// NULL_CHECK_RETURN(pMonster, E_FAIL);
	//

	//
	// NULL_CHECK_RETURN(pMonster, E_FAIL);
	//
	
	//
	// NULL_CHECK_RETURN(pMonster, E_FAIL);
	//
	
	//
	// NULL_CHECK_RETURN(pMonster, E_FAIL);
	// ~BOW
	////////////////////
	// Granade

	// CMonster::MONSTEROPTION			MonsterDesc;
	// MonsterDesc.m_bFirstSpawnType[CMonster::STATE_ALREADYSPAWN] = true;
	// MonsterDesc.MonsterDesc.m_iHP = MonsterDesc.MonsterDesc.m_iMaxHP = 300;
	// MonsterDesc.MonsterDesc.m_iDamage = 15;
	// MonsterDesc.MonsterDesc.m_iShield = MonsterDesc.MonsterDesc.m_iMaxShield = 0;
	// MonsterDesc.MonsterDesc.TransformDesc.fSpeedPerSec = 2.5f;
	// MonsterDesc.MonsterDesc.TransformDesc.fRotationPerSec = XMConvertToRadians(180.f);
	// MonsterDesc.m_iCellIndex = 40;
	// pMonster = dynamic_cast<CMonster*>(pGameInstance->Clone_GameObjectReturnPtr_M(LEVEL_GAMEPLAY, wstrLayerTag, L"Prototype_GameObject_Normal_Human_Granade", PivotMatrix, &MonsterDesc));
	// pMonster->Set_Player(pPlayer);
	//
	// NULL_CHECK_RETURN(pMonster, E_FAIL);
	//
	//
	// ZeroMemory(&MonsterDesc, sizeof(CMonster::MONSTEROPTION));
	// PivotMatrix.r[3] = XMVectorSet(-15.3f, 0.f, 15.7f, 1.f);
	// MonsterDesc.m_bFirstSpawnType[CMonster::STATE_NODETECTED] = true;			// No Detected == Patrol
	// MonsterDesc.MonsterDesc.m_iHP = MonsterDesc.MonsterDesc.m_iMaxHP = 300;
	// MonsterDesc.MonsterDesc.m_iDamage = 15;
	// MonsterDesc.MonsterDesc.m_iShield = MonsterDesc.MonsterDesc.m_iMaxShield = 0;
	// MonsterDesc.MonsterDesc.TransformDesc.fSpeedPerSec = 2.5f;
	// MonsterDesc.MonsterDesc.TransformDesc.fRotationPerSec = XMConvertToRadians(180.f);
	// MonsterDesc.m_iCellIndex = 71;
	// pMonster = dynamic_cast<CMonster*>(pGameInstance->Clone_GameObjectReturnPtr_M(LEVEL_GAMEPLAY, wstrLayerTag, L"Prototype_GameObject_Normal_Human_Granade", PivotMatrix, &MonsterDesc));
	// pMonster->Set_Player(pPlayer);
	//
	// NULL_CHECK_RETURN(pMonster, E_FAIL);
	//
	// ZeroMemory(&MonsterDesc, sizeof(CMonster::MONSTEROPTION));
	// PivotMatrix.r[3] = XMVectorSet(4.f, 0.f, 13.7f, 1.f);
	// MonsterDesc.m_bFirstSpawnType[CMonster::STATE_NODETECTED] = true;			// No Detected == Patrol
	// MonsterDesc.MonsterDesc.m_iHP = MonsterDesc.MonsterDesc.m_iMaxHP = 300;
	// MonsterDesc.MonsterDesc.m_iDamage = 15;
	// MonsterDesc.MonsterDesc.m_iShield = MonsterDesc.MonsterDesc.m_iMaxShield = 0;
	// MonsterDesc.MonsterDesc.TransformDesc.fSpeedPerSec = 2.5f;
	// MonsterDesc.MonsterDesc.TransformDesc.fRotationPerSec = XMConvertToRadians(180.f);
	// MonsterDesc.m_iCellIndex = 135;
	// pMonster = dynamic_cast<CMonster*>(pGameInstance->Clone_GameObjectReturnPtr_M(LEVEL_GAMEPLAY, wstrLayerTag, L"Prototype_GameObject_Normal_Human_Granade", PivotMatrix, &MonsterDesc));
	// pMonster->Set_Player(pPlayer);
	//
	// NULL_CHECK_RETURN(pMonster, E_FAIL);
	//
	// ZeroMemory(&MonsterDesc, sizeof(CMonster::MONSTEROPTION));
	// PivotMatrix.r[3] = XMVectorSet(11.f, 1.6f, 22.1f, 1.f);
	// MonsterDesc.m_bFirstSpawnType[CMonster::STATE_ALREADYSPAWN] = true;			// No Detected == Patrol
	// MonsterDesc.MonsterDesc.m_iHP = MonsterDesc.MonsterDesc.m_iMaxHP = 300;
	// MonsterDesc.MonsterDesc.m_iDamage = 15;
	// MonsterDesc.MonsterDesc.m_iShield = MonsterDesc.MonsterDesc.m_iMaxShield = 0;
	// MonsterDesc.MonsterDesc.TransformDesc.fSpeedPerSec = 2.5f;
	// MonsterDesc.MonsterDesc.TransformDesc.fRotationPerSec = XMConvertToRadians(180.f);
	// MonsterDesc.m_iCellIndex = 158;
	// pMonster = dynamic_cast<CMonster*>(pGameInstance->Clone_GameObjectReturnPtr_M(LEVEL_GAMEPLAY, wstrLayerTag, L"Prototype_GameObject_Normal_Human_Granade", PivotMatrix, &MonsterDesc));
	// pMonster->Set_Player(pPlayer);
	//
	// NULL_CHECK_RETURN(pMonster, E_FAIL);
	//
	// ZeroMemory(&MonsterDesc, sizeof(CMonster::MONSTEROPTION));
	// PivotMatrix.r[3] = XMVectorSet(12.9f, 1.6f, 23.6f, 1.f);
	// MonsterDesc.m_bFirstSpawnType[CMonster::STATE_ALREADYSPAWN] = true;			// No Detected == Patrol
	// MonsterDesc.MonsterDesc.m_iHP = MonsterDesc.MonsterDesc.m_iMaxHP = 300;
	// MonsterDesc.MonsterDesc.m_iDamage = 15;
	// MonsterDesc.MonsterDesc.m_iShield = MonsterDesc.MonsterDesc.m_iMaxShield = 0;
	// MonsterDesc.MonsterDesc.TransformDesc.fSpeedPerSec = 2.5f;
	// MonsterDesc.MonsterDesc.TransformDesc.fRotationPerSec = XMConvertToRadians(180.f);
	// MonsterDesc.m_iCellIndex = 159;
	// pMonster = dynamic_cast<CMonster*>(pGameInstance->Clone_GameObjectReturnPtr_M(LEVEL_GAMEPLAY, wstrLayerTag, L"Prototype_GameObject_Normal_Human_Granade", PivotMatrix, &MonsterDesc));
	// pMonster->Set_Player(pPlayer);
	//
	// NULL_CHECK_RETURN(pMonster, E_FAIL);
	//
	// ZeroMemory(&MonsterDesc, sizeof(CMonster::MONSTEROPTION));
	// PivotMatrix.r[3] = XMVectorSet(3.9f, 1.6f, 26.7f, 1.f);
	// MonsterDesc.m_bFirstSpawnType[CMonster::STATE_ALREADYSPAWN] = true;			// No Detected == Patrol
	// MonsterDesc.MonsterDesc.m_iHP = MonsterDesc.MonsterDesc.m_iMaxHP = 300;
	// MonsterDesc.MonsterDesc.m_iDamage = 15;
	// MonsterDesc.MonsterDesc.m_iShield = MonsterDesc.MonsterDesc.m_iMaxShield = 0;
	// MonsterDesc.MonsterDesc.TransformDesc.fSpeedPerSec = 2.5f;
	// MonsterDesc.MonsterDesc.TransformDesc.fRotationPerSec = XMConvertToRadians(180.f);
	// MonsterDesc.m_iCellIndex = 172;
	// pMonster = dynamic_cast<CMonster*>(pGameInstance->Clone_GameObjectReturnPtr_M(LEVEL_GAMEPLAY, wstrLayerTag, L"Prototype_GameObject_Normal_Human_Granade", PivotMatrix, &MonsterDesc));
	// pMonster->Set_Player(pPlayer);
	//
	// NULL_CHECK_RETURN(pMonster, E_FAIL);
	//

	//
	// NULL_CHECK_RETURN(pMonster, E_FAIL);
	//
	
	//
	// NULL_CHECK_RETURN(pMonster, E_FAIL);

	
	//
	// NULL_CHECK_RETURN(pMonster, E_FAIL);

	
	//
	// NULL_CHECK_RETURN(pMonster, E_FAIL);

	
	//
	// NULL_CHECK_RETURN(pMonster, E_FAIL);


	//
	// NULL_CHECK_RETURN(pMonster, E_FAIL);
	// ~ Granade
	///////////////////
	// // Normal Explode
	//
	// CMonster::MONSTEROPTION			MonsterDesc;
	// MonsterDesc.m_bFirstSpawnType[CMonster::STATE_NODETECTED] = true;
	// MonsterDesc.MonsterDesc.m_iHP = MonsterDesc.MonsterDesc.m_iMaxHP = 300;
	// MonsterDesc.MonsterDesc.m_iDamage = 15;
	// MonsterDesc.MonsterDesc.m_iShield = MonsterDesc.MonsterDesc.m_iMaxShield = 0;
	// MonsterDesc.MonsterDesc.TransformDesc.fSpeedPerSec = 2.5f;
	// MonsterDesc.MonsterDesc.TransformDesc.fRotationPerSec = XMConvertToRadians(180.f);
	// MonsterDesc.m_iCellIndex = 40;
	// pMonster = dynamic_cast<CMonster*>(pGameInstance->Clone_GameObjectReturnPtr_M(LEVEL_GAMEPLAY, wstrLayerTag, L"Prototype_GameObject_Normal_Human_Explode", PivotMatrix, &MonsterDesc));
	// pMonster->Set_Player(pPlayer);
	//
	// NULL_CHECK_RETURN(pMonster, E_FAIL);
	//
	// ZeroMemory(&MonsterDesc, sizeof(CMonster::MONSTEROPTION));
	// PivotMatrix.r[3] = XMVectorSet(11.4f, 1.f, 12.9f, 1.f);
	// MonsterDesc.m_bFirstSpawnType[CMonster::STATE_ALREADYSPAWN] = true;
	// MonsterDesc.MonsterDesc.m_iHP = MonsterDesc.MonsterDesc.m_iMaxHP = 300;
	// MonsterDesc.MonsterDesc.m_iDamage = 15;
	// MonsterDesc.MonsterDesc.m_iShield = MonsterDesc.MonsterDesc.m_iMaxShield = 0;
	// MonsterDesc.MonsterDesc.TransformDesc.fSpeedPerSec = 2.5f;
	// MonsterDesc.MonsterDesc.TransformDesc.fRotationPerSec = XMConvertToRadians(180.f);
	// MonsterDesc.m_iCellIndex = 140;
	// pMonster = dynamic_cast<CMonster*>(pGameInstance->Clone_GameObjectReturnPtr_M(LEVEL_GAMEPLAY, wstrLayerTag, L"Prototype_GameObject_Normal_Human_Explode", PivotMatrix, &MonsterDesc));
	// pMonster->Set_Player(pPlayer);
	//
	// NULL_CHECK_RETURN(pMonster, E_FAIL);
	//
	// ZeroMemory(&MonsterDesc, sizeof(CMonster::MONSTEROPTION));
	// PivotMatrix.r[3] = XMVectorSet(13.6f, 1.f, 17.f, 1.f);
	// MonsterDesc.m_bFirstSpawnType[CMonster::STATE_ALREADYSPAWN] = true;
	// MonsterDesc.MonsterDesc.m_iHP = MonsterDesc.MonsterDesc.m_iMaxHP = 300;
	// MonsterDesc.MonsterDesc.m_iDamage = 15;
	// MonsterDesc.MonsterDesc.m_iShield = MonsterDesc.MonsterDesc.m_iMaxShield = 0;
	// MonsterDesc.MonsterDesc.TransformDesc.fSpeedPerSec = 2.5f;
	// MonsterDesc.MonsterDesc.TransformDesc.fRotationPerSec = XMConvertToRadians(180.f);
	// MonsterDesc.m_iCellIndex = 151;
	// pMonster = dynamic_cast<CMonster*>(pGameInstance->Clone_GameObjectReturnPtr_M(LEVEL_GAMEPLAY, wstrLayerTag, L"Prototype_GameObject_Normal_Human_Explode", PivotMatrix, &MonsterDesc));
	// pMonster->Set_Player(pPlayer);
	//
	// NULL_CHECK_RETURN(pMonster, E_FAIL);
	//
	// ZeroMemory(&MonsterDesc, sizeof(CMonster::MONSTEROPTION));
	// PivotMatrix.r[3] = XMVectorSet(2.2f, 0.f, 22.1f, 1.f);
	// MonsterDesc.m_bFirstSpawnType[CMonster::STATE_ALREADYSPAWN] = true;
	// MonsterDesc.MonsterDesc.m_iHP = MonsterDesc.MonsterDesc.m_iMaxHP = 300;
	// MonsterDesc.MonsterDesc.m_iDamage = 15;
	// MonsterDesc.MonsterDesc.m_iShield = MonsterDesc.MonsterDesc.m_iMaxShield = 0;
	// MonsterDesc.MonsterDesc.TransformDesc.fSpeedPerSec = 2.5f;
	// MonsterDesc.MonsterDesc.TransformDesc.fRotationPerSec = XMConvertToRadians(180.f);
	// MonsterDesc.m_iCellIndex = 163;
	// pMonster = dynamic_cast<CMonster*>(pGameInstance->Clone_GameObjectReturnPtr_M(LEVEL_GAMEPLAY, wstrLayerTag, L"Prototype_GameObject_Normal_Human_Explode", PivotMatrix, &MonsterDesc));
	// pMonster->Set_Player(pPlayer);
	//
	// NULL_CHECK_RETURN(pMonster, E_FAIL);
	//
	
	//
	// NULL_CHECK_RETURN(pMonster, E_FAIL);
	//
	
	//
	// NULL_CHECK_RETURN(pMonster, E_FAIL);
	//
	
	//
	// NULL_CHECK_RETURN(pMonster, E_FAIL);
	//

	
	//
	
	//
	// NULL_CHECK_RETURN(pMonster, E_FAIL);
	// ~Normal Explode

	// Elite Monster_Bug
	
	// NULL_CHECK_RETURN(pMonster, E_FAIL);

	// Little_Bug
	
	// NULL_CHECK_RETURN(pMonster, E_FAIL);

	// ZeroMemory(&MonsterDesc, sizeof(CMonster::MONSTEROPTION));
	
	// NULL_CHECK_RETURN(pMonster, E_FAIL);
	//~Little_BUG

	////////
	//Elite_Knight
	// ZeroMemory(&MonsterDesc, sizeof(CMonster::MONSTEROPTION));
	// PivotMatrix.r[3] = XMVectorSet(24.f, 0.f, 46.f, 1.f);
	// MonsterDesc.m_bFirstSpawnType[CMonster::STATE_UPSPAWN] = true;
	// MonsterDesc.MonsterDesc.m_iHP = MonsterDesc.MonsterDesc.m_iMaxHP = 300;
	// MonsterDesc.MonsterDesc.m_iDamage = 15;
	// MonsterDesc.MonsterDesc.m_iShield = MonsterDesc.MonsterDesc.m_iMaxShield = 0;
	// MonsterDesc.MonsterDesc.TransformDesc.fSpeedPerSec = 2.5f;
	// MonsterDesc.MonsterDesc.TransformDesc.fRotationPerSec = XMConvertToRadians(180.f);
	// MonsterDesc.m_iCellIndex =421;
	// // MonsterDesc.MonsterDesc.TransformDesc.vInitPos = _float3(-14.f, 0.f, 0.19f);
	// pMonster = dynamic_cast<CMonster*>(pGameInstance->Clone_GameObjectReturnPtr_M(LEVEL_GAMEPLAY, wstrLayerTag, L"Prototype_GameObject_Normal_Elite_Knight", PivotMatrix, &MonsterDesc));
	// pMonster->Set_Player(pPlayer);
	//~ Elite Knight
	/////////////////////////
	// NULL_CHECK_RETURN(pMonster, E_FAIL);

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Player(const wstring wstrLayerTag)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);
	FAILED_CHECK_RETURN(pGameInstance->Clone_GameObject(LEVEL_GAMEPLAY, wstrLayerTag, L"Prototype_GameObject_Player"), E_FAIL);
	

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Effect(const wstring wstrLayerTag)
{
	CGameInstance*	pGameInstance = GET_INSTANCE(CGameInstance);

	FAILED_CHECK_RETURN(pGameInstance->Clone_GameObject(LEVEL_GAMEPLAY, wstrLayerTag, L"Prototype_GameObject_Effect_Point_Instancing"), E_FAIL);

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_UI(const wstring wstrLayerTag)
{
	CGameInstance*	pGameInstance = GET_INSTANCE(CGameInstance);

	FAILED_CHECK_RETURN(pGameInstance->Clone_GameObject(LEVEL_GAMEPLAY, wstrLayerTag, L"Prototype_GameObject_PlayerUI_Base"), E_FAIL);

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_BossMonster(const wstring wstrLayerTag)
{
	CGameInstance*	pGameInstance = GET_INSTANCE(CGameInstance);

	CMonster*			pMonster = nullptr;
	CPlayer*			pPlayer = dynamic_cast<CPlayer*>(pGameInstance->Get_CloneObjectList(LEVEL_GAMEPLAY, L"Layer_Player")->front());
	NULL_CHECK_RETURN(pPlayer, E_FAIL);

	
	// Boss 
	CMonster::MONSTEROPTION			MonsterDesc;


	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Trigger(const wstring wstrLayerTag)
{
	CGameInstance*	pGameInstance = GET_INSTANCE(CGameInstance);
	CPlayer*			pPlayer = dynamic_cast<CPlayer*>(pGameInstance->Get_CloneObjectList(LEVEL_GAMEPLAY, L"Layer_Player")->front());
	CTrigger* pTrigger = nullptr;
	_matrix PivotMatrix = XMMatrixIdentity();
	CTrigger::TRIGGERDESC TriggerDesc;
	// A->Knight
	PivotMatrix.r[3] = XMVectorSet(-10.815f, 0.015f, 31.333f, 1.f);
	TriggerDesc.m_eType = CTrigger::TRIGGER_A_TO_KNIGHT;
	pTrigger = (CTrigger*)pGameInstance->Clone_GameObjectReturnPtr_M(LEVEL_GAMEPLAY, wstrLayerTag, L"Prototype_GameObject_Trigger", PivotMatrix, &TriggerDesc);
	NULL_CHECK_RETURN(pTrigger, E_FAIL);
	pTrigger->Set_Owner(pPlayer);

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

CLevel_GamePlay * CLevel_GamePlay::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CLevel_GamePlay*		pInstance = new CLevel_GamePlay(pDevice, pContext);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created : CLevel_GamePlay");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CLevel_GamePlay::Free()
{
	__super::Free();
}
