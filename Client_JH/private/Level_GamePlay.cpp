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
	ZeroMemory(&LightDesc, sizeof(LightDesc));

	LightDesc.eType = LIGHTDESC::LIGHT_DIRECTIONAL;
	LightDesc.isEnable = true;
	LightDesc.vDirection = _float4(1.f, -1.f, 1.f, 1.f);
	LightDesc.vDiffuse = _float4(1.f, 1.f, 1.f, 1.f);
	LightDesc.vAmbient = _float4(1.f, 1.f, 1.f, 1.f);
	LightDesc.vSpecular = _float4(1.f, 1.f, 1.f, 1.f);

	if (FAILED(pGameInstance->Add_Light(m_pDevice, m_pContext, LightDesc)))
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_BackGround(const wstring wstrLayerTag)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	// FAILED_CHECK_RETURN(pGameInstance->Clone_GameObject(LEVEL_GAMEPLAY, wstrLayerTag, L"Prototype_GameObject_Terrain"), E_FAIL);

	// FAILED_CHECK_RETURN(pGameInstance->Clone_GameObject(LEVEL_GAMEPLAY, wstrLayerTag, L"Prototype_GameObject_ForkLift"), E_FAIL);
	// FAILED_CHECK_RETURN(pGameInstance->Clone_GameObject(LEVEL_GAMEPLAY, wstrLayerTag, L"Prototype_GameObject_Sky"), E_FAIL);
	FAILED_CHECK_RETURN(pGameInstance->Clone_GameObject(LEVEL_GAMEPLAY, wstrLayerTag, L"Prototype_GameObject_Home"), E_FAIL);
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

	_matrix PivotMatrix = XMMatrixIdentity();
	PivotMatrix.r[3] = XMVectorSet(-14.f, 0.f, 0.19f, 1.f);

	CMonster::MONSTEROPTION			MonsterDesc;
	MonsterDesc.m_bFirstSpawnType[CMonster::STATE_NODETECTED] = true;
	MonsterDesc.MonsterDesc.m_iHP = MonsterDesc.MonsterDesc.m_iMaxHP = 300;
	MonsterDesc.MonsterDesc.m_iDamage = 15;
	MonsterDesc.MonsterDesc.m_iShield = MonsterDesc.MonsterDesc.m_iMaxShield = 0;
	MonsterDesc.MonsterDesc.TransformDesc.fSpeedPerSec = 5.f;
	pMonster = dynamic_cast<CMonster*>(pGameInstance->Clone_GameObjectReturnPtr_M(LEVEL_GAMEPLAY, wstrLayerTag, L"Prototype_GameObject_Normal_Human_Sword", PivotMatrix, &MonsterDesc));
	pMonster->Set_Player(pPlayer);
	NULL_CHECK_RETURN(pMonster, E_FAIL);



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
