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

	// FAILED_CHECK_RETURN(pGameInstance->Clone_GameObject(LEVEL_GAMEPLAY, wstrLayerTag, L"Prototype_GameObject_Terrain"), E_FAIL);

	// FAILED_CHECK_RETURN(pGameInstance->Clone_GameObject(LEVEL_GAMEPLAY, wstrLayerTag, L"Prototype_GameObject_ForkLift"), E_FAIL);
	// FAILED_CHECK_RETURN(pGameInstance->Clone_GameObject(LEVEL_GAMEPLAY, wstrLayerTag, L"Prototype_GameObject_Sky"), E_FAIL);
	FAILED_CHECK_RETURN(pGameInstance->Clone_GameObject(LEVEL_GAMEPLAY, wstrLayerTag, L"Prototype_GameObject_NormalMap"), E_FAIL);
	FAILED_CHECK_RETURN(pGameInstance->Clone_GameObject(LEVEL_GAMEPLAY, wstrLayerTag, L"Prototype_GameObject_Normal_BossMap"), E_FAIL);

	// _matrix PivotMatrix = XMMatrixIdentity();


	// if (FAILED(pGameInstance->))
	// 	return E_FAIL;
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
	PivotMatrix.r[3] = XMVectorSet(116.f, 1.3f, 179.f, 1.f);
	// Boss 
	CMonster::MONSTEROPTION			MonsterDesc;
	MonsterDesc.m_bFirstSpawnType[CMonster::STATE_NODETECTED] = true;
	MonsterDesc.MonsterDesc.m_iHP = MonsterDesc.MonsterDesc.m_iMaxHP = 300;
	MonsterDesc.MonsterDesc.m_iDamage = 15;
	MonsterDesc.MonsterDesc.m_iShield = MonsterDesc.MonsterDesc.m_iMaxShield = 0;
	MonsterDesc.MonsterDesc.TransformDesc.fSpeedPerSec = 5.f;
	MonsterDesc.m_iCellIndex = 40;
	pMonster = dynamic_cast<CMonster*>(pGameInstance->Clone_GameObjectReturnPtr_M(LEVEL_GAMEPLAY, wstrLayerTag, L"Prototype_GameObject_Normal_Boss", PivotMatrix, &MonsterDesc));
	pMonster->Set_Player(pPlayer);

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
	//
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
	// ZeroMemory(&MonsterDesc, sizeof(CMonster::MONSTEROPTION));
	// PivotMatrix.r[3] = XMVectorSet(-44.7f, 0.f, 72.2f, 1.f);
	// MonsterDesc.m_bFirstSpawnType[CMonster::STATE_ALREADYSPAWN] = true;
	// MonsterDesc.MonsterDesc.m_iHP = MonsterDesc.MonsterDesc.m_iMaxHP = 300;
	// MonsterDesc.MonsterDesc.m_iDamage = 15;
	// MonsterDesc.MonsterDesc.m_iShield = MonsterDesc.MonsterDesc.m_iMaxShield = 0;
	// MonsterDesc.MonsterDesc.TransformDesc.fSpeedPerSec = 5.f;
	// MonsterDesc.m_iCellIndex = 294;
	// pMonster = dynamic_cast<CMonster*>(pGameInstance->Clone_GameObjectReturnPtr_M(LEVEL_GAMEPLAY, wstrLayerTag, L"Prototype_GameObject_Normal_Human_Sword", PivotMatrix, &MonsterDesc));
	// pMonster->Set_Player(pPlayer);
	//
	// // 그라운드 스폰
	// ZeroMemory(&MonsterDesc, sizeof(CMonster::MONSTEROPTION));
	// PivotMatrix.r[3] = XMVectorSet(-64.3f, -0.7f, 73.8f, 1.f);
	// MonsterDesc.m_bFirstSpawnType[CMonster::STATE_ALREADYSPAWN] = true;
	// MonsterDesc.MonsterDesc.m_iHP = MonsterDesc.MonsterDesc.m_iMaxHP = 300;
	// MonsterDesc.MonsterDesc.m_iDamage = 15;
	// MonsterDesc.MonsterDesc.m_iShield = MonsterDesc.MonsterDesc.m_iMaxShield = 0;
	// MonsterDesc.MonsterDesc.TransformDesc.fSpeedPerSec = 5.f;
	// MonsterDesc.m_iCellIndex = 446;
	// pMonster = dynamic_cast<CMonster*>(pGameInstance->Clone_GameObjectReturnPtr_M(LEVEL_GAMEPLAY, wstrLayerTag, L"Prototype_GameObject_Normal_Human_Sword", PivotMatrix, &MonsterDesc));
	// pMonster->Set_Player(pPlayer);
	// // ~그라운드 스폰
	//
	// ZeroMemory(&MonsterDesc, sizeof(CMonster::MONSTEROPTION));
	// PivotMatrix.r[3] = XMVectorSet(-64.5f, -0.9f, 69.2f, 1.f);
	// MonsterDesc.m_bFirstSpawnType[CMonster::STATE_NODETECTED] = true;
	// MonsterDesc.MonsterDesc.m_iHP = MonsterDesc.MonsterDesc.m_iMaxHP = 300;
	// MonsterDesc.MonsterDesc.m_iDamage = 15;
	// MonsterDesc.MonsterDesc.m_iShield = MonsterDesc.MonsterDesc.m_iMaxShield = 0;
	// MonsterDesc.MonsterDesc.TransformDesc.fSpeedPerSec = 5.f;
	// MonsterDesc.m_iCellIndex = 446;
	// pMonster = dynamic_cast<CMonster*>(pGameInstance->Clone_GameObjectReturnPtr_M(LEVEL_GAMEPLAY, wstrLayerTag, L"Prototype_GameObject_Normal_Human_Sword", PivotMatrix, &MonsterDesc));
	// pMonster->Set_Player(pPlayer);
	//
	// ZeroMemory(&MonsterDesc, sizeof(CMonster::MONSTEROPTION));
	// PivotMatrix.r[3] = XMVectorSet(-62.5f, -0.9f, 71.6f, 1.f);
	// MonsterDesc.m_bFirstSpawnType[CMonster::STATE_NODETECTED] = true;
	// MonsterDesc.MonsterDesc.m_iHP = MonsterDesc.MonsterDesc.m_iMaxHP = 300;
	// MonsterDesc.MonsterDesc.m_iDamage = 15;
	// MonsterDesc.MonsterDesc.m_iShield = MonsterDesc.MonsterDesc.m_iMaxShield = 0;
	// MonsterDesc.MonsterDesc.TransformDesc.fSpeedPerSec = 5.f;
	// MonsterDesc.m_iCellIndex = 446;
	// pMonster = dynamic_cast<CMonster*>(pGameInstance->Clone_GameObjectReturnPtr_M(LEVEL_GAMEPLAY, wstrLayerTag, L"Prototype_GameObject_Normal_Human_Sword", PivotMatrix, &MonsterDesc));
	// pMonster->Set_Player(pPlayer);
	//
	// ZeroMemory(&MonsterDesc, sizeof(CMonster::MONSTEROPTION));
	// PivotMatrix.r[3] = XMVectorSet(-56.2f, -0.9f, 71.98f, 1.f);
	// MonsterDesc.m_bFirstSpawnType[CMonster::STATE_NODETECTED] = true;
	// MonsterDesc.MonsterDesc.m_iHP = MonsterDesc.MonsterDesc.m_iMaxHP = 300;
	// MonsterDesc.MonsterDesc.m_iDamage = 15;
	// MonsterDesc.MonsterDesc.m_iShield = MonsterDesc.MonsterDesc.m_iMaxShield = 0;
	// MonsterDesc.MonsterDesc.TransformDesc.fSpeedPerSec = 5.f;
	// MonsterDesc.m_iCellIndex = 448;
	// pMonster = dynamic_cast<CMonster*>(pGameInstance->Clone_GameObjectReturnPtr_M(LEVEL_GAMEPLAY, wstrLayerTag, L"Prototype_GameObject_Normal_Human_Sword", PivotMatrix, &MonsterDesc));
	// pMonster->Set_Player(pPlayer);
	//
	// ZeroMemory(&MonsterDesc, sizeof(CMonster::MONSTEROPTION));
	// PivotMatrix.r[3] = XMVectorSet(-56.8f, -1.f, 76.5f, 1.f);
	// MonsterDesc.m_bFirstSpawnType[CMonster::STATE_NODETECTED] = true;
	// MonsterDesc.MonsterDesc.m_iHP = MonsterDesc.MonsterDesc.m_iMaxHP = 300;
	// MonsterDesc.MonsterDesc.m_iDamage = 15;
	// MonsterDesc.MonsterDesc.m_iShield = MonsterDesc.MonsterDesc.m_iMaxShield = 0;
	// MonsterDesc.MonsterDesc.TransformDesc.fSpeedPerSec = 5.f;
	// MonsterDesc.m_iCellIndex = 449;
	// pMonster = dynamic_cast<CMonster*>(pGameInstance->Clone_GameObjectReturnPtr_M(LEVEL_GAMEPLAY, wstrLayerTag, L"Prototype_GameObject_Normal_Human_Sword", PivotMatrix, &MonsterDesc));
	// pMonster->Set_Player(pPlayer);
	//
	// ZeroMemory(&MonsterDesc, sizeof(CMonster::MONSTEROPTION));
	// PivotMatrix.r[3] = XMVectorSet(-59.2f, -0.9f, 78.5f, 1.f);
	// MonsterDesc.m_bFirstSpawnType[CMonster::STATE_NODETECTED] = true;
	// MonsterDesc.MonsterDesc.m_iHP = MonsterDesc.MonsterDesc.m_iMaxHP = 300;
	// MonsterDesc.MonsterDesc.m_iDamage = 15;
	// MonsterDesc.MonsterDesc.m_iShield = MonsterDesc.MonsterDesc.m_iMaxShield = 0;
	// MonsterDesc.MonsterDesc.TransformDesc.fSpeedPerSec = 5.f;
	// MonsterDesc.m_iCellIndex = 449;
	// pMonster = dynamic_cast<CMonster*>(pGameInstance->Clone_GameObjectReturnPtr_M(LEVEL_GAMEPLAY, wstrLayerTag, L"Prototype_GameObject_Normal_Human_Sword", PivotMatrix, &MonsterDesc));
	// pMonster->Set_Player(pPlayer);
	//
	// ZeroMemory(&MonsterDesc, sizeof(CMonster::MONSTEROPTION));
	// PivotMatrix.r[3] = XMVectorSet(-68.f, -0.9f, 85.6f, 1.f);
	// MonsterDesc.m_bFirstSpawnType[CMonster::STATE_NODETECTED] = true;
	// MonsterDesc.MonsterDesc.m_iHP = MonsterDesc.MonsterDesc.m_iMaxHP = 300;
	// MonsterDesc.MonsterDesc.m_iDamage = 15;
	// MonsterDesc.MonsterDesc.m_iShield = MonsterDesc.MonsterDesc.m_iMaxShield = 0;
	// MonsterDesc.MonsterDesc.TransformDesc.fSpeedPerSec = 5.f;
	// MonsterDesc.m_iCellIndex = 455;
	// pMonster = dynamic_cast<CMonster*>(pGameInstance->Clone_GameObjectReturnPtr_M(LEVEL_GAMEPLAY, wstrLayerTag, L"Prototype_GameObject_Normal_Human_Sword", PivotMatrix, &MonsterDesc));
	// pMonster->Set_Player(pPlayer);
	//
	// ZeroMemory(&MonsterDesc, sizeof(CMonster::MONSTEROPTION));
	// PivotMatrix.r[3] = XMVectorSet(-63.3f, -0.97f, 87.2f, 1.f);
	// MonsterDesc.m_bFirstSpawnType[CMonster::STATE_NODETECTED] = true;
	// MonsterDesc.MonsterDesc.m_iHP = MonsterDesc.MonsterDesc.m_iMaxHP = 300;
	// MonsterDesc.MonsterDesc.m_iDamage = 15;
	// MonsterDesc.MonsterDesc.m_iShield = MonsterDesc.MonsterDesc.m_iMaxShield = 0;
	// MonsterDesc.MonsterDesc.TransformDesc.fSpeedPerSec = 5.f;
	// MonsterDesc.m_iCellIndex = 453;
	// pMonster = dynamic_cast<CMonster*>(pGameInstance->Clone_GameObjectReturnPtr_M(LEVEL_GAMEPLAY, wstrLayerTag, L"Prototype_GameObject_Normal_Human_Sword", PivotMatrix, &MonsterDesc));
	// pMonster->Set_Player(pPlayer);
	//
	// ZeroMemory(&MonsterDesc, sizeof(CMonster::MONSTEROPTION));
	// PivotMatrix.r[3] = XMVectorSet(-57.1f, -0.97f, 83.7f, 1.f);
	// MonsterDesc.m_bFirstSpawnType[CMonster::STATE_NODETECTED] = true;
	// MonsterDesc.MonsterDesc.m_iHP = MonsterDesc.MonsterDesc.m_iMaxHP = 300;
	// MonsterDesc.MonsterDesc.m_iDamage = 15;
	// MonsterDesc.MonsterDesc.m_iShield = MonsterDesc.MonsterDesc.m_iMaxShield = 0;
	// MonsterDesc.MonsterDesc.TransformDesc.fSpeedPerSec = 5.f;
	// MonsterDesc.m_iCellIndex = 453;
	// pMonster = dynamic_cast<CMonster*>(pGameInstance->Clone_GameObjectReturnPtr_M(LEVEL_GAMEPLAY, wstrLayerTag, L"Prototype_GameObject_Normal_Human_Sword", PivotMatrix, &MonsterDesc));
	// pMonster->Set_Player(pPlayer);
	//
	// ZeroMemory(&MonsterDesc, sizeof(CMonster::MONSTEROPTION));
	// PivotMatrix.r[3] = XMVectorSet(-63.2f, 0.95f, 99.3f, 1.f);
	// MonsterDesc.m_bFirstSpawnType[CMonster::STATE_NODETECTED] = true;
	// MonsterDesc.MonsterDesc.m_iHP = MonsterDesc.MonsterDesc.m_iMaxHP = 300;
	// MonsterDesc.MonsterDesc.m_iDamage = 15;
	// MonsterDesc.MonsterDesc.m_iShield = MonsterDesc.MonsterDesc.m_iMaxShield = 0;
	// MonsterDesc.MonsterDesc.TransformDesc.fSpeedPerSec = 5.f;
	// MonsterDesc.m_iCellIndex = 466;
	// pMonster = dynamic_cast<CMonster*>(pGameInstance->Clone_GameObjectReturnPtr_M(LEVEL_GAMEPLAY, wstrLayerTag, L"Prototype_GameObject_Normal_Human_Sword", PivotMatrix, &MonsterDesc));
	// pMonster->Set_Player(pPlayer);
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
	// ZeroMemory(&MonsterDesc, sizeof(CMonster::MONSTEROPTION));
	// PivotMatrix.r[3] = XMVectorSet(-13.3f, 0.f, 48.4f, 1.f);
	// MonsterDesc.m_bFirstSpawnType[CMonster::STATE_ALREADYSPAWN] = true;
	// MonsterDesc.MonsterDesc.m_iHP = MonsterDesc.MonsterDesc.m_iMaxHP = 300;
	// MonsterDesc.MonsterDesc.m_iDamage = 15;
	// MonsterDesc.MonsterDesc.m_iShield = MonsterDesc.MonsterDesc.m_iMaxShield = 0;
	// MonsterDesc.MonsterDesc.TransformDesc.fSpeedPerSec = 5.f;
	// MonsterDesc.m_iCellIndex = 205;
	// pMonster = dynamic_cast<CMonster*>(pGameInstance->Clone_GameObjectReturnPtr_M(LEVEL_GAMEPLAY, wstrLayerTag, L"Prototype_GameObject_Normal_Human_Spear", PivotMatrix, &MonsterDesc));
	// pMonster->Set_Player(pPlayer);
	//
	// ZeroMemory(&MonsterDesc, sizeof(CMonster::MONSTEROPTION));
	// PivotMatrix.r[3] = XMVectorSet(-15.8f, 0.18f, 55.5f, 1.f);
	// MonsterDesc.m_bFirstSpawnType[CMonster::STATE_ALREADYSPAWN] = true;
	// MonsterDesc.MonsterDesc.m_iHP = MonsterDesc.MonsterDesc.m_iMaxHP = 300;
	// MonsterDesc.MonsterDesc.m_iDamage = 15;
	// MonsterDesc.MonsterDesc.m_iShield = MonsterDesc.MonsterDesc.m_iMaxShield = 0;
	// MonsterDesc.MonsterDesc.TransformDesc.fSpeedPerSec = 5.f;
	// MonsterDesc.m_iCellIndex = 221;
	// pMonster = dynamic_cast<CMonster*>(pGameInstance->Clone_GameObjectReturnPtr_M(LEVEL_GAMEPLAY, wstrLayerTag, L"Prototype_GameObject_Normal_Human_Spear", PivotMatrix, &MonsterDesc));
	// pMonster->Set_Player(pPlayer);
	//
	// ZeroMemory(&MonsterDesc, sizeof(CMonster::MONSTEROPTION));
	// PivotMatrix.r[3] = XMVectorSet(-23.7f, 1.4f, 45.7f, 1.f);
	// MonsterDesc.m_bFirstSpawnType[CMonster::STATE_ALREADYSPAWN] = true;
	// MonsterDesc.MonsterDesc.m_iHP = MonsterDesc.MonsterDesc.m_iMaxHP = 300;
	// MonsterDesc.MonsterDesc.m_iDamage = 15;
	// MonsterDesc.MonsterDesc.m_iShield = MonsterDesc.MonsterDesc.m_iMaxShield = 0;
	// MonsterDesc.MonsterDesc.TransformDesc.fSpeedPerSec = 5.f;
	// MonsterDesc.m_iCellIndex = 237;
	// pMonster = dynamic_cast<CMonster*>(pGameInstance->Clone_GameObjectReturnPtr_M(LEVEL_GAMEPLAY, wstrLayerTag, L"Prototype_GameObject_Normal_Human_Spear", PivotMatrix, &MonsterDesc));
	// pMonster->Set_Player(pPlayer);
	//
	// ZeroMemory(&MonsterDesc, sizeof(CMonster::MONSTEROPTION));
	// PivotMatrix.r[3] = XMVectorSet(-32.3f, 1.4f, 52.8f, 1.f);
	// MonsterDesc.m_bFirstSpawnType[CMonster::STATE_ALREADYSPAWN] = true;
	// MonsterDesc.MonsterDesc.m_iHP = MonsterDesc.MonsterDesc.m_iMaxHP = 300;
	// MonsterDesc.MonsterDesc.m_iDamage = 15;
	// MonsterDesc.MonsterDesc.m_iShield = MonsterDesc.MonsterDesc.m_iMaxShield = 0;
	// MonsterDesc.MonsterDesc.TransformDesc.fSpeedPerSec = 5.f;
	// MonsterDesc.m_iCellIndex = 270;
	// pMonster = dynamic_cast<CMonster*>(pGameInstance->Clone_GameObjectReturnPtr_M(LEVEL_GAMEPLAY, wstrLayerTag, L"Prototype_GameObject_Normal_Human_Spear", PivotMatrix, &MonsterDesc));
	// pMonster->Set_Player(pPlayer);
	//
	// ZeroMemory(&MonsterDesc, sizeof(CMonster::MONSTEROPTION));
	// PivotMatrix.r[3] = XMVectorSet(-37.7f, -1.4f, 80.7f, 1.f);
	// MonsterDesc.m_bFirstSpawnType[CMonster::STATE_ALREADYSPAWN] = true;
	// MonsterDesc.MonsterDesc.m_iHP = MonsterDesc.MonsterDesc.m_iMaxHP = 300;
	// MonsterDesc.MonsterDesc.m_iDamage = 15;
	// MonsterDesc.MonsterDesc.m_iShield = MonsterDesc.MonsterDesc.m_iMaxShield = 0;
	// MonsterDesc.MonsterDesc.TransformDesc.fSpeedPerSec = 5.f;
	// MonsterDesc.m_iCellIndex = 324;
	// pMonster = dynamic_cast<CMonster*>(pGameInstance->Clone_GameObjectReturnPtr_M(LEVEL_GAMEPLAY, wstrLayerTag, L"Prototype_GameObject_Normal_Human_Spear", PivotMatrix, &MonsterDesc));
	// pMonster->Set_Player(pPlayer);
	//
	// ZeroMemory(&MonsterDesc, sizeof(CMonster::MONSTEROPTION));
	// PivotMatrix.r[3] = XMVectorSet(11.9f, 0.9f, 43.3f, 1.f);
	// MonsterDesc.m_bFirstSpawnType[CMonster::STATE_ALREADYSPAWN] = true;
	// MonsterDesc.MonsterDesc.m_iHP = MonsterDesc.MonsterDesc.m_iMaxHP = 300;
	// MonsterDesc.MonsterDesc.m_iDamage = 15;
	// MonsterDesc.MonsterDesc.m_iShield = MonsterDesc.MonsterDesc.m_iMaxShield = 0;
	// MonsterDesc.MonsterDesc.TransformDesc.fSpeedPerSec = 5.f;
	// MonsterDesc.m_iCellIndex = 386;
	// pMonster = dynamic_cast<CMonster*>(pGameInstance->Clone_GameObjectReturnPtr_M(LEVEL_GAMEPLAY, wstrLayerTag, L"Prototype_GameObject_Normal_Human_Spear", PivotMatrix, &MonsterDesc));
	// pMonster->Set_Player(pPlayer);
	//
	// ZeroMemory(&MonsterDesc, sizeof(CMonster::MONSTEROPTION));
	// PivotMatrix.r[3] = XMVectorSet(-60.4f, -0.99f, 73.75f, 1.f);
	// MonsterDesc.m_bFirstSpawnType[CMonster::STATE_NODETECTED] = true;
	// MonsterDesc.MonsterDesc.m_iHP = MonsterDesc.MonsterDesc.m_iMaxHP = 300;
	// MonsterDesc.MonsterDesc.m_iDamage = 15;
	// MonsterDesc.MonsterDesc.m_iShield = MonsterDesc.MonsterDesc.m_iMaxShield = 0;
	// MonsterDesc.MonsterDesc.TransformDesc.fSpeedPerSec = 5.f;
	// MonsterDesc.m_iCellIndex = 447;
	// pMonster = dynamic_cast<CMonster*>(pGameInstance->Clone_GameObjectReturnPtr_M(LEVEL_GAMEPLAY, wstrLayerTag, L"Prototype_GameObject_Normal_Human_Spear", PivotMatrix, &MonsterDesc));
	// pMonster->Set_Player(pPlayer);
	//
	// ZeroMemory(&MonsterDesc, sizeof(CMonster::MONSTEROPTION));
	// PivotMatrix.r[3] = XMVectorSet(-57.4f, -0.9f, 67.2f, 1.f);
	// MonsterDesc.m_bFirstSpawnType[CMonster::STATE_NODETECTED] = true;
	// MonsterDesc.MonsterDesc.m_iHP = MonsterDesc.MonsterDesc.m_iMaxHP = 300;
	// MonsterDesc.MonsterDesc.m_iDamage = 15;
	// MonsterDesc.MonsterDesc.m_iShield = MonsterDesc.MonsterDesc.m_iMaxShield = 0;
	// MonsterDesc.MonsterDesc.TransformDesc.fSpeedPerSec = 5.f;
	// MonsterDesc.m_iCellIndex = 443;
	// pMonster = dynamic_cast<CMonster*>(pGameInstance->Clone_GameObjectReturnPtr_M(LEVEL_GAMEPLAY, wstrLayerTag, L"Prototype_GameObject_Normal_Human_Spear", PivotMatrix, &MonsterDesc));
	// pMonster->Set_Player(pPlayer);
	//
	// ZeroMemory(&MonsterDesc, sizeof(CMonster::MONSTEROPTION));
	// PivotMatrix.r[3] = XMVectorSet(-67.2f, 0.96f, 93.4f, 1.f);
	// MonsterDesc.m_bFirstSpawnType[CMonster::STATE_NODETECTED] = true;
	// MonsterDesc.MonsterDesc.m_iHP = MonsterDesc.MonsterDesc.m_iMaxHP = 300;
	// MonsterDesc.MonsterDesc.m_iDamage = 15;
	// MonsterDesc.MonsterDesc.m_iShield = MonsterDesc.MonsterDesc.m_iMaxShield = 0;
	// MonsterDesc.MonsterDesc.TransformDesc.fSpeedPerSec = 5.f;
	// MonsterDesc.m_iCellIndex = 460;
	// pMonster = dynamic_cast<CMonster*>(pGameInstance->Clone_GameObjectReturnPtr_M(LEVEL_GAMEPLAY, wstrLayerTag, L"Prototype_GameObject_Normal_Human_Spear", PivotMatrix, &MonsterDesc));
	// pMonster->Set_Player(pPlayer);
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
	ZeroMemory(&MonsterDesc, sizeof(CMonster::MONSTEROPTION));
	PivotMatrix.r[3] = XMVectorSet(-12.2f, 0.f, 57.4f, 1.f);
	MonsterDesc.m_bFirstSpawnType[CMonster::STATE_ALREADYSPAWN] = true;
	MonsterDesc.MonsterDesc.m_iHP = MonsterDesc.MonsterDesc.m_iMaxHP = 300;
	MonsterDesc.MonsterDesc.m_iDamage = 15;
	MonsterDesc.MonsterDesc.m_iShield = MonsterDesc.MonsterDesc.m_iMaxShield = 0;
	MonsterDesc.MonsterDesc.TransformDesc.fSpeedPerSec = 2.5f;
	MonsterDesc.MonsterDesc.TransformDesc.fRotationPerSec = XMConvertToRadians(180.f);
	MonsterDesc.m_iCellIndex = 220;
	pMonster = dynamic_cast<CMonster*>(pGameInstance->Clone_GameObjectReturnPtr_M(LEVEL_GAMEPLAY, wstrLayerTag, L"Prototype_GameObject_Normal_Human_Bow", PivotMatrix, &MonsterDesc));
	pMonster->Set_Player(pPlayer);
	
	NULL_CHECK_RETURN(pMonster, E_FAIL);
	//
	// ZeroMemory(&MonsterDesc, sizeof(CMonster::MONSTEROPTION));
	// PivotMatrix.r[3] = XMVectorSet(-34.2f, 0.f, 37.8f, 1.f);
	// MonsterDesc.m_bFirstSpawnType[CMonster::STATE_ALREADYSPAWN] = true;
	// MonsterDesc.MonsterDesc.m_iHP = MonsterDesc.MonsterDesc.m_iMaxHP = 300;
	// MonsterDesc.MonsterDesc.m_iDamage = 15;
	// MonsterDesc.MonsterDesc.m_iShield = MonsterDesc.MonsterDesc.m_iMaxShield = 0;
	// MonsterDesc.MonsterDesc.TransformDesc.fSpeedPerSec = 2.5f;
	// MonsterDesc.MonsterDesc.TransformDesc.fRotationPerSec = XMConvertToRadians(180.f);
	// MonsterDesc.m_iCellIndex = 256;
	// pMonster = dynamic_cast<CMonster*>(pGameInstance->Clone_GameObjectReturnPtr_M(LEVEL_GAMEPLAY, wstrLayerTag, L"Prototype_GameObject_Normal_Human_Bow", PivotMatrix, &MonsterDesc));
	// pMonster->Set_Player(pPlayer);
	//
	// NULL_CHECK_RETURN(pMonster, E_FAIL);
	//
	// ZeroMemory(&MonsterDesc, sizeof(CMonster::MONSTEROPTION));
	// PivotMatrix.r[3] = XMVectorSet(-21.6f, 0.f, 65.7f, 1.f);
	// MonsterDesc.m_bFirstSpawnType[CMonster::STATE_ALREADYSPAWN] = true;
	// MonsterDesc.MonsterDesc.m_iHP = MonsterDesc.MonsterDesc.m_iMaxHP = 300;
	// MonsterDesc.MonsterDesc.m_iDamage = 15;
	// MonsterDesc.MonsterDesc.m_iShield = MonsterDesc.MonsterDesc.m_iMaxShield = 0;
	// MonsterDesc.MonsterDesc.TransformDesc.fSpeedPerSec = 2.5f;
	// MonsterDesc.MonsterDesc.TransformDesc.fRotationPerSec = XMConvertToRadians(180.f);
	// MonsterDesc.m_iCellIndex = 282;
	// pMonster = dynamic_cast<CMonster*>(pGameInstance->Clone_GameObjectReturnPtr_M(LEVEL_GAMEPLAY, wstrLayerTag, L"Prototype_GameObject_Normal_Human_Bow", PivotMatrix, &MonsterDesc));
	// pMonster->Set_Player(pPlayer);
	//
	// NULL_CHECK_RETURN(pMonster, E_FAIL);
	//
	// ZeroMemory(&MonsterDesc, sizeof(CMonster::MONSTEROPTION));
	// PivotMatrix.r[3] = XMVectorSet(-45.3f, 0.f, 66.4f, 1.f);
	// MonsterDesc.m_bFirstSpawnType[CMonster::STATE_ALREADYSPAWN] = true;
	// MonsterDesc.MonsterDesc.m_iHP = MonsterDesc.MonsterDesc.m_iMaxHP = 300;
	// MonsterDesc.MonsterDesc.m_iDamage = 15;
	// MonsterDesc.MonsterDesc.m_iShield = MonsterDesc.MonsterDesc.m_iMaxShield = 0;
	// MonsterDesc.MonsterDesc.TransformDesc.fSpeedPerSec = 2.5f;
	// MonsterDesc.MonsterDesc.TransformDesc.fRotationPerSec = XMConvertToRadians(180.f);
	// MonsterDesc.m_iCellIndex = 291;
	// pMonster = dynamic_cast<CMonster*>(pGameInstance->Clone_GameObjectReturnPtr_M(LEVEL_GAMEPLAY, wstrLayerTag, L"Prototype_GameObject_Normal_Human_Bow", PivotMatrix, &MonsterDesc));
	// pMonster->Set_Player(pPlayer);
	//
	// NULL_CHECK_RETURN(pMonster, E_FAIL);
	//
	// ZeroMemory(&MonsterDesc, sizeof(CMonster::MONSTEROPTION));
	// PivotMatrix.r[3] = XMVectorSet(17.f, 0.f, 44.2f, 1.f);
	// MonsterDesc.m_bFirstSpawnType[CMonster::STATE_ALREADYSPAWN] = true;
	// MonsterDesc.MonsterDesc.m_iHP = MonsterDesc.MonsterDesc.m_iMaxHP = 300;
	// MonsterDesc.MonsterDesc.m_iDamage = 15;
	// MonsterDesc.MonsterDesc.m_iShield = MonsterDesc.MonsterDesc.m_iMaxShield = 0;
	// MonsterDesc.MonsterDesc.TransformDesc.fSpeedPerSec = 2.5f;
	// MonsterDesc.MonsterDesc.TransformDesc.fRotationPerSec = XMConvertToRadians(180.f);
	// MonsterDesc.m_iCellIndex = 403;
	// pMonster = dynamic_cast<CMonster*>(pGameInstance->Clone_GameObjectReturnPtr_M(LEVEL_GAMEPLAY, wstrLayerTag, L"Prototype_GameObject_Normal_Human_Bow", PivotMatrix, &MonsterDesc));
	// pMonster->Set_Player(pPlayer);
	//
	// NULL_CHECK_RETURN(pMonster, E_FAIL);
	//
	// ZeroMemory(&MonsterDesc, sizeof(CMonster::MONSTEROPTION));
	// PivotMatrix.r[3] = XMVectorSet(-62.4f, -0.9f, 79.65f, 1.f);
	// MonsterDesc.m_bFirstSpawnType[CMonster::STATE_ALREADYSPAWN] = true;
	// MonsterDesc.MonsterDesc.m_iHP = MonsterDesc.MonsterDesc.m_iMaxHP = 300;
	// MonsterDesc.MonsterDesc.m_iDamage = 15;
	// MonsterDesc.MonsterDesc.m_iShield = MonsterDesc.MonsterDesc.m_iMaxShield = 0;
	// MonsterDesc.MonsterDesc.TransformDesc.fSpeedPerSec = 2.5f;
	// MonsterDesc.MonsterDesc.TransformDesc.fRotationPerSec = XMConvertToRadians(180.f);
	// MonsterDesc.m_iCellIndex = 449;
	// pMonster = dynamic_cast<CMonster*>(pGameInstance->Clone_GameObjectReturnPtr_M(LEVEL_GAMEPLAY, wstrLayerTag, L"Prototype_GameObject_Normal_Human_Bow", PivotMatrix, &MonsterDesc));
	// pMonster->Set_Player(pPlayer);
	//
	// NULL_CHECK_RETURN(pMonster, E_FAIL);
	//
	// ZeroMemory(&MonsterDesc, sizeof(CMonster::MONSTEROPTION));
	// PivotMatrix.r[3] = XMVectorSet(-63.2f, 0.95f, 99.3f, 1.f);
	// MonsterDesc.m_bFirstSpawnType[CMonster::STATE_ALREADYSPAWN] = true;
	// MonsterDesc.MonsterDesc.m_iHP = MonsterDesc.MonsterDesc.m_iMaxHP = 300;
	// MonsterDesc.MonsterDesc.m_iDamage = 15;
	// MonsterDesc.MonsterDesc.m_iShield = MonsterDesc.MonsterDesc.m_iMaxShield = 0;
	// MonsterDesc.MonsterDesc.TransformDesc.fSpeedPerSec = 2.5f;
	// MonsterDesc.MonsterDesc.TransformDesc.fRotationPerSec = XMConvertToRadians(180.f);
	// MonsterDesc.m_iCellIndex = 466;
	// pMonster = dynamic_cast<CMonster*>(pGameInstance->Clone_GameObjectReturnPtr_M(LEVEL_GAMEPLAY, wstrLayerTag, L"Prototype_GameObject_Normal_Human_Bow", PivotMatrix, &MonsterDesc));
	// pMonster->Set_Player(pPlayer);
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
	// ZeroMemory(&MonsterDesc, sizeof(CMonster::MONSTEROPTION));
	// PivotMatrix.r[3] = XMVectorSet(-20.1f, 0.f, 37.6f, 1.f);
	// MonsterDesc.m_bFirstSpawnType[CMonster::STATE_ALREADYSPAWN] = true;			// No Detected == Patrol
	// MonsterDesc.MonsterDesc.m_iHP = MonsterDesc.MonsterDesc.m_iMaxHP = 300;
	// MonsterDesc.MonsterDesc.m_iDamage = 15;
	// MonsterDesc.MonsterDesc.m_iShield = MonsterDesc.MonsterDesc.m_iMaxShield = 0;
	// MonsterDesc.MonsterDesc.TransformDesc.fSpeedPerSec = 2.5f;
	// MonsterDesc.MonsterDesc.TransformDesc.fRotationPerSec = XMConvertToRadians(180.f);
	// MonsterDesc.m_iCellIndex = 243;
	// pMonster = dynamic_cast<CMonster*>(pGameInstance->Clone_GameObjectReturnPtr_M(LEVEL_GAMEPLAY, wstrLayerTag, L"Prototype_GameObject_Normal_Human_Granade", PivotMatrix, &MonsterDesc));
	// pMonster->Set_Player(pPlayer);
	//
	// NULL_CHECK_RETURN(pMonster, E_FAIL);
	//
	// ZeroMemory(&MonsterDesc, sizeof(CMonster::MONSTEROPTION));
	// PivotMatrix.r[3] = XMVectorSet(-30.1f, 1.4f, 47.0f, 1.f);
	// MonsterDesc.m_bFirstSpawnType[CMonster::STATE_ALREADYSPAWN] = true;			// No Detected == Patrol
	// MonsterDesc.MonsterDesc.m_iHP = MonsterDesc.MonsterDesc.m_iMaxHP = 300;
	// MonsterDesc.MonsterDesc.m_iDamage = 15;
	// MonsterDesc.MonsterDesc.m_iShield = MonsterDesc.MonsterDesc.m_iMaxShield = 0;
	// MonsterDesc.MonsterDesc.TransformDesc.fSpeedPerSec = 2.5f;
	// MonsterDesc.MonsterDesc.TransformDesc.fRotationPerSec = XMConvertToRadians(180.f);
	// MonsterDesc.m_iCellIndex = 265;
	// pMonster = dynamic_cast<CMonster*>(pGameInstance->Clone_GameObjectReturnPtr_M(LEVEL_GAMEPLAY, wstrLayerTag, L"Prototype_GameObject_Normal_Human_Granade", PivotMatrix, &MonsterDesc));
	// pMonster->Set_Player(pPlayer);
	//
	// NULL_CHECK_RETURN(pMonster, E_FAIL);

	// ZeroMemory(&MonsterDesc, sizeof(CMonster::MONSTEROPTION)); // 위치 확인 필요
	// PivotMatrix.r[3] = XMVectorSet(-30.1f, 1.4f, 47.0f, 1.f);
	// MonsterDesc.m_bFirstSpawnType[CMonster::STATE_ALREADYSPAWN] = true;			// No Detected == Patrol
	// MonsterDesc.MonsterDesc.m_iHP = MonsterDesc.MonsterDesc.m_iMaxHP = 300;
	// MonsterDesc.MonsterDesc.m_iDamage = 15;
	// MonsterDesc.MonsterDesc.m_iShield = MonsterDesc.MonsterDesc.m_iMaxShield = 0;
	// MonsterDesc.MonsterDesc.TransformDesc.fSpeedPerSec = 2.5f;
	// MonsterDesc.MonsterDesc.TransformDesc.fRotationPerSec = XMConvertToRadians(180.f);
	// MonsterDesc.m_iCellIndex = 269;
	// pMonster = dynamic_cast<CMonster*>(pGameInstance->Clone_GameObjectReturnPtr_M(LEVEL_GAMEPLAY, wstrLayerTag, L"Prototype_GameObject_Normal_Human_Granade", PivotMatrix, &MonsterDesc));
	// pMonster->Set_Player(pPlayer);
	//
	// NULL_CHECK_RETURN(pMonster, E_FAIL);

	// ZeroMemory(&MonsterDesc, sizeof(CMonster::MONSTEROPTION));
	// PivotMatrix.r[3] = XMVectorSet(-51.5f, -0.94f, 85.2f, 1.f);
	// MonsterDesc.m_bFirstSpawnType[CMonster::STATE_ALREADYSPAWN] = true;			// No Detected == Patrol
	// MonsterDesc.MonsterDesc.m_iHP = MonsterDesc.MonsterDesc.m_iMaxHP = 300;
	// MonsterDesc.MonsterDesc.m_iDamage = 15;
	// MonsterDesc.MonsterDesc.m_iShield = MonsterDesc.MonsterDesc.m_iMaxShield = 0;
	// MonsterDesc.MonsterDesc.TransformDesc.fSpeedPerSec = 2.5f;
	// MonsterDesc.MonsterDesc.TransformDesc.fRotationPerSec = XMConvertToRadians(180.f);
	// MonsterDesc.m_iCellIndex = 452;
	// pMonster = dynamic_cast<CMonster*>(pGameInstance->Clone_GameObjectReturnPtr_M(LEVEL_GAMEPLAY, wstrLayerTag, L"Prototype_GameObject_Normal_Human_Granade", PivotMatrix, &MonsterDesc));
	// pMonster->Set_Player(pPlayer);
	//
	// NULL_CHECK_RETURN(pMonster, E_FAIL);

	// ZeroMemory(&MonsterDesc, sizeof(CMonster::MONSTEROPTION));
	// PivotMatrix.r[3] = XMVectorSet(-70.f, 1.49f, 102.3f, 1.f);
	// MonsterDesc.m_bFirstSpawnType[CMonster::STATE_ALREADYSPAWN] = true;			// No Detected == Patrol
	// MonsterDesc.MonsterDesc.m_iHP = MonsterDesc.MonsterDesc.m_iMaxHP = 300;
	// MonsterDesc.MonsterDesc.m_iDamage = 15;
	// MonsterDesc.MonsterDesc.m_iShield = MonsterDesc.MonsterDesc.m_iMaxShield = 0;
	// MonsterDesc.MonsterDesc.TransformDesc.fSpeedPerSec = 2.5f;
	// MonsterDesc.MonsterDesc.TransformDesc.fRotationPerSec = XMConvertToRadians(180.f);
	// MonsterDesc.m_iCellIndex = 480;
	// pMonster = dynamic_cast<CMonster*>(pGameInstance->Clone_GameObjectReturnPtr_M(LEVEL_GAMEPLAY, wstrLayerTag, L"Prototype_GameObject_Normal_Human_Granade", PivotMatrix, &MonsterDesc));
	// pMonster->Set_Player(pPlayer);
	//
	// NULL_CHECK_RETURN(pMonster, E_FAIL);

	// ZeroMemory(&MonsterDesc, sizeof(CMonster::MONSTEROPTION));
	// PivotMatrix.r[3] = XMVectorSet(13.f, 0.f, 35.1f, 1.f);
	// MonsterDesc.m_bFirstSpawnType[CMonster::STATE_ALREADYSPAWN] = true;			// No Detected == Patrol
	// MonsterDesc.MonsterDesc.m_iHP = MonsterDesc.MonsterDesc.m_iMaxHP = 300;
	// MonsterDesc.MonsterDesc.m_iDamage = 15;
	// MonsterDesc.MonsterDesc.m_iShield = MonsterDesc.MonsterDesc.m_iMaxShield = 0;
	// MonsterDesc.MonsterDesc.TransformDesc.fSpeedPerSec = 2.5f;
	// MonsterDesc.MonsterDesc.TransformDesc.fRotationPerSec = XMConvertToRadians(180.f);
	// MonsterDesc.m_iCellIndex = 388;
	// pMonster = dynamic_cast<CMonster*>(pGameInstance->Clone_GameObjectReturnPtr_M(LEVEL_GAMEPLAY, wstrLayerTag, L"Prototype_GameObject_Normal_Human_Granade", PivotMatrix, &MonsterDesc));
	// pMonster->Set_Player(pPlayer);
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
	// ZeroMemory(&MonsterDesc, sizeof(CMonster::MONSTEROPTION));
	// PivotMatrix.r[3] = XMVectorSet(-7.7f, -0.85f, 42.f, 1.f);
	// MonsterDesc.m_bFirstSpawnType[CMonster::STATE_ALREADYSPAWN] = true;
	// MonsterDesc.MonsterDesc.m_iHP = MonsterDesc.MonsterDesc.m_iMaxHP = 300;
	// MonsterDesc.MonsterDesc.m_iDamage = 15;
	// MonsterDesc.MonsterDesc.m_iShield = MonsterDesc.MonsterDesc.m_iMaxShield = 0;
	// MonsterDesc.MonsterDesc.TransformDesc.fSpeedPerSec = 2.5f;
	// MonsterDesc.MonsterDesc.TransformDesc.fRotationPerSec = XMConvertToRadians(180.f);
	// MonsterDesc.m_iCellIndex = 201;
	// pMonster = dynamic_cast<CMonster*>(pGameInstance->Clone_GameObjectReturnPtr_M(LEVEL_GAMEPLAY, wstrLayerTag, L"Prototype_GameObject_Normal_Human_Explode", PivotMatrix, &MonsterDesc));
	// pMonster->Set_Player(pPlayer);
	//
	// NULL_CHECK_RETURN(pMonster, E_FAIL);
	//
	// ZeroMemory(&MonsterDesc, sizeof(CMonster::MONSTEROPTION));
	// PivotMatrix.r[3] = XMVectorSet(-20.7f, 0.f, 51.8f, 1.f);
	// MonsterDesc.m_bFirstSpawnType[CMonster::STATE_ALREADYSPAWN] = true;
	// MonsterDesc.MonsterDesc.m_iHP = MonsterDesc.MonsterDesc.m_iMaxHP = 300;
	// MonsterDesc.MonsterDesc.m_iDamage = 15;
	// MonsterDesc.MonsterDesc.m_iShield = MonsterDesc.MonsterDesc.m_iMaxShield = 0;
	// MonsterDesc.MonsterDesc.TransformDesc.fSpeedPerSec = 2.5f;
	// MonsterDesc.MonsterDesc.TransformDesc.fRotationPerSec = XMConvertToRadians(180.f);
	// MonsterDesc.m_iCellIndex = 232;
	// pMonster = dynamic_cast<CMonster*>(pGameInstance->Clone_GameObjectReturnPtr_M(LEVEL_GAMEPLAY, wstrLayerTag, L"Prototype_GameObject_Normal_Human_Explode", PivotMatrix, &MonsterDesc));
	// pMonster->Set_Player(pPlayer);
	//
	// NULL_CHECK_RETURN(pMonster, E_FAIL);
	//
	// ZeroMemory(&MonsterDesc, sizeof(CMonster::MONSTEROPTION));
	// PivotMatrix.r[3] = XMVectorSet(-29.8f, -1.4f, 81.1f, 1.f);
	// MonsterDesc.m_bFirstSpawnType[CMonster::STATE_ALREADYSPAWN] = true;
	// MonsterDesc.MonsterDesc.m_iHP = MonsterDesc.MonsterDesc.m_iMaxHP = 300;
	// MonsterDesc.MonsterDesc.m_iDamage = 15;
	// MonsterDesc.MonsterDesc.m_iShield = MonsterDesc.MonsterDesc.m_iMaxShield = 0;
	// MonsterDesc.MonsterDesc.TransformDesc.fSpeedPerSec = 2.5f;
	// MonsterDesc.MonsterDesc.TransformDesc.fRotationPerSec = XMConvertToRadians(180.f);
	// MonsterDesc.m_iCellIndex = 328;
	// pMonster = dynamic_cast<CMonster*>(pGameInstance->Clone_GameObjectReturnPtr_M(LEVEL_GAMEPLAY, wstrLayerTag, L"Prototype_GameObject_Normal_Human_Explode", PivotMatrix, &MonsterDesc));
	// pMonster->Set_Player(pPlayer);
	//
	// NULL_CHECK_RETURN(pMonster, E_FAIL);
	//
	// ZeroMemory(&MonsterDesc, sizeof(CMonster::MONSTEROPTION));
	// PivotMatrix.r[3] = XMVectorSet(7.38f, 0.f, 33.9f, 1.f);
	// MonsterDesc.m_bFirstSpawnType[CMonster::STATE_ALREADYSPAWN] = true;
	// MonsterDesc.MonsterDesc.m_iHP = MonsterDesc.MonsterDesc.m_iMaxHP = 300;
	// MonsterDesc.MonsterDesc.m_iDamage = 15;
	// MonsterDesc.MonsterDesc.m_iShield = MonsterDesc.MonsterDesc.m_iMaxShield = 0;
	// MonsterDesc.MonsterDesc.TransformDesc.fSpeedPerSec = 2.5f;
	// MonsterDesc.MonsterDesc.TransformDesc.fRotationPerSec = XMConvertToRadians(180.f);
	// MonsterDesc.m_iCellIndex = 378;
	// pMonster = dynamic_cast<CMonster*>(pGameInstance->Clone_GameObjectReturnPtr_M(LEVEL_GAMEPLAY, wstrLayerTag, L"Prototype_GameObject_Normal_Human_Explode", PivotMatrix, &MonsterDesc));
	// pMonster->Set_Player(pPlayer);
	//
	// NULL_CHECK_RETURN(pMonster, E_FAIL);
	//
	// ZeroMemory(&MonsterDesc, sizeof(CMonster::MONSTEROPTION));
	// PivotMatrix.r[3] = XMVectorSet(-68.9f, -0.9f, 83.5f, 1.f);
	// MonsterDesc.m_bFirstSpawnType[CMonster::STATE_ALREADYSPAWN] = true;
	// MonsterDesc.MonsterDesc.m_iHP = MonsterDesc.MonsterDesc.m_iMaxHP = 300;
	// MonsterDesc.MonsterDesc.m_iDamage = 15;
	// MonsterDesc.MonsterDesc.m_iShield = MonsterDesc.MonsterDesc.m_iMaxShield = 0;
	// MonsterDesc.MonsterDesc.TransformDesc.fSpeedPerSec = 2.5f;
	// MonsterDesc.MonsterDesc.TransformDesc.fRotationPerSec = XMConvertToRadians(180.f);
	// MonsterDesc.m_iCellIndex = 455;
	// pMonster = dynamic_cast<CMonster*>(pGameInstance->Clone_GameObjectReturnPtr_M(LEVEL_GAMEPLAY, wstrLayerTag, L"Prototype_GameObject_Normal_Human_Explode", PivotMatrix, &MonsterDesc));
	// pMonster->Set_Player(pPlayer);
	//
	// NULL_CHECK_RETURN(pMonster, E_FAIL);
	// ~Normal Explode

	// Elite Monster_Bug
	// ZeroMemory(&MonsterDesc, sizeof(CMonster::MONSTEROPTION));
	// PivotMatrix.r[3] = XMVectorSet(-70.7f, 1.49f, 102.3f, 1.f);
	// MonsterDesc.m_bFirstSpawnType[CMonster::STATE_NODETECTED] = true;
	// MonsterDesc.MonsterDesc.m_iHP = MonsterDesc.MonsterDesc.m_iMaxHP = 300;
	// MonsterDesc.MonsterDesc.m_iDamage = 15;
	// MonsterDesc.MonsterDesc.m_iShield = MonsterDesc.MonsterDesc.m_iMaxShield = 0;
	// MonsterDesc.MonsterDesc.TransformDesc.fSpeedPerSec = 2.5f;
	// MonsterDesc.MonsterDesc.TransformDesc.fRotationPerSec = XMConvertToRadians(180.f);
	// MonsterDesc.m_iCellIndex = 451;
	// // MonsterDesc.MonsterDesc.TransformDesc.vInitPos = _float3( -14.f, 0.f, 0.19f );
	// pMonster = dynamic_cast<CMonster*>(pGameInstance->Clone_GameObjectReturnPtr_M(LEVEL_GAMEPLAY, wstrLayerTag, L"Prototype_GameObject_Normal_Elite_Bug", PivotMatrix, &MonsterDesc));
	// pMonster->Set_Player(pPlayer);
	// NULL_CHECK_RETURN(pMonster, E_FAIL);

	// Little_Bug
	// ZeroMemory(&MonsterDesc, sizeof(CMonster::MONSTEROPTION));
	// PivotMatrix.r[3] = XMVectorSet(-22.85f, -1.4f, 79.5f, 1.f);
	// MonsterDesc.m_bFirstSpawnType[CMonster::STATE_NODETECTED] = true;
	// MonsterDesc.MonsterDesc.m_iHP = MonsterDesc.MonsterDesc.m_iMaxHP = 300;
	// MonsterDesc.MonsterDesc.m_iDamage = 15;
	// MonsterDesc.MonsterDesc.m_iShield = MonsterDesc.MonsterDesc.m_iMaxShield = 0;
	// MonsterDesc.MonsterDesc.TransformDesc.fSpeedPerSec = 2.5f;
	// MonsterDesc.MonsterDesc.TransformDesc.fRotationPerSec = XMConvertToRadians(180.f);
	// MonsterDesc.m_iCellIndex = 313;
	// // MonsterDesc.MonsterDesc.TransformDesc.vInitPos = _float3( -14.f, 0.f, 0.19f );
	// pMonster = dynamic_cast<CMonster*>(pGameInstance->Clone_GameObjectReturnPtr_M(LEVEL_GAMEPLAY, wstrLayerTag, L"Prototype_GameObject_Normal_Little_Bug", PivotMatrix, &MonsterDesc));
	// pMonster->Set_Player(pPlayer);
	// NULL_CHECK_RETURN(pMonster, E_FAIL);

	// ZeroMemory(&MonsterDesc, sizeof(CMonster::MONSTEROPTION));
	// PivotMatrix.r[3] = XMVectorSet(-20.6f, -1.4f, 79.5f, 1.f);
	// MonsterDesc.m_bFirstSpawnType[CMonster::STATE_NODETECTED] = true;
	// MonsterDesc.MonsterDesc.m_iHP = MonsterDesc.MonsterDesc.m_iMaxHP = 300;
	// MonsterDesc.MonsterDesc.m_iDamage = 15;
	// MonsterDesc.MonsterDesc.m_iShield = MonsterDesc.MonsterDesc.m_iMaxShield = 0;
	// MonsterDesc.MonsterDesc.TransformDesc.fSpeedPerSec = 2.5f;
	// MonsterDesc.MonsterDesc.TransformDesc.fRotationPerSec = XMConvertToRadians(180.f);
	// MonsterDesc.m_iCellIndex = 313;
	// // MonsterDesc.MonsterDesc.TransformDesc.vInitPos = _float3( -14.f, 0.f, 0.19f );
	// pMonster = dynamic_cast<CMonster*>(pGameInstance->Clone_GameObjectReturnPtr_M(LEVEL_GAMEPLAY, wstrLayerTag, L"Prototype_GameObject_Normal_Little_Bug", PivotMatrix, &MonsterDesc));
	// pMonster->Set_Player(pPlayer);
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
