#include "stdafx.h"
#include "..\public\Level_GamePlay.h"

#include "Button_Play.h"
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
#include "Camera.h"
#include "Fire_Light.h"
#include "Level_Loading.h"

CLevel_GamePlay::CLevel_GamePlay(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CLevel(pDevice, pContext)
{

}

HRESULT CLevel_GamePlay::Initialize()
{
	FAILED_CHECK_RETURN(__super::Initialize(), E_FAIL);
	CGameInstance::GetInstance()->Clear_Lights();
	CGameInstance::GetInstance()->Clear_Camera();

	// CGameInstance::GetInstance()->Clear_();

	FAILED_CHECK_RETURN(Ready_Light(), E_FAIL);
	// FAILED_CHECK_RETURN(Ready_Layer_BackGround(L"Layer_BackGround"), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Layer_Camera(L"Layer_ZCamera"), E_FAIL);
	// FAILED_CHECK_RETURN(Ready_Layer_Monster(L"Layer_Monster"), E_FAIL);
	// FAILED_CHECK_RETURN(Ready_Layer_Effect(L"Layer_Smoke"), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Layer_Env(L"Layer_Env"), E_FAIL);

	FAILED_CHECK_RETURN(Ready_Layer_LaiHome(L"Layer_LaiHome"), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Layer_Lobby(L"Layer_LobbyMap"), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Layer_LobbyButton(L"Layer_Button"), E_FAIL);
	// FAILED_CHECK_RETURN(Ready_Layer_Player(L"Layer_Player"), E_FAIL);

	// FAILED_CHECK_RETURN(Ready_Layer_BossMonster(L"Layer_ZBossMonster"), E_FAIL);
	
	// FAILED_CHECK_RETURN(Ready_Layer_Trigger(L"Layer_Trigger"), E_FAIL);

	// FAILED_CHECK_RETURN(Ready_Layer_UI(L"Layer_UI"), E_FAIL);

	return S_OK;
}

void CLevel_GamePlay::Tick(_double TimeDelta)
{
	__super::Tick(TimeDelta);

	if(m_bButton == true)
	{
		m_fCurClickedDelay += (_float)TimeDelta;
	}

	// if (CGameInstance::GetInstance()->Key_Down(DIK_END))
	// {
	// 	// m_bPlayFinish = true;
	//
	// 	FAILED_CHECK_RETURN(CGameInstance::GetInstance()->Open_Level(LEVEL_LOADING, CLevel_Loading::Create(m_pDevice, m_pContext, (LEVEL)LEVEL_LOGO)), );
	//
	// 	return;
	// }

	if(m_fCurClickedDelay >= m_fClickedDelay)
	{
		m_fCurClickedDelay = 0.f;
		m_bIsClicked = true;
		m_bButton = false;
	}

	if(m_pPlayer != nullptr)
	{
		if(m_pPlayer->Get_CurRoomType(CPlayer::ROOM_KIGHT) == true  && m_bRenderLoading ==false)
		{
			CGameInstance* pGameInstance = CGameInstance::GetInstance();
			CGameObject* pGameObject = nullptr;
			CGameObject::GAMEOBJECTDESC	GameObjectDesc;
			GameObjectDesc.m_iCountType = 2;
			pGameObject = pGameInstance->Clone_GameObjectReturnPtr(pGameInstance->Get_StaticLevelIndex(), L"Layer_Logo", L"Prototype_GameObject_BackGround", &GameObjectDesc);
			NULL_CHECK_RETURN(pGameObject, );
			m_bRenderLoading = true;
		}
		else if (m_pPlayer->Get_CurRoomType(CPlayer::ROOM_B) == true && m_bRenderLoading == true)
		{
			CGameInstance* pGameInstance = CGameInstance::GetInstance();
			CGameObject* pGameObject = nullptr;
			CGameObject::GAMEOBJECTDESC	GameObjectDesc;
			GameObjectDesc.m_iCountType = 3;
			pGameObject = pGameInstance->Clone_GameObjectReturnPtr(pGameInstance->Get_StaticLevelIndex(), L"Layer_Logo", L"Prototype_GameObject_BackGround", &GameObjectDesc);
			NULL_CHECK_RETURN(pGameObject, );
			m_bRenderLoading = false;
		}
		else if (m_pPlayer->Get_CurRoomType(CPlayer::ROOM_BUG) == true && m_bRenderLoading == false)
		{
			CGameInstance* pGameInstance = CGameInstance::GetInstance();
			CGameObject* pGameObject = nullptr;
			CGameObject::GAMEOBJECTDESC	GameObjectDesc;
			GameObjectDesc.m_iCountType = 4;
			pGameObject = pGameInstance->Clone_GameObjectReturnPtr(pGameInstance->Get_StaticLevelIndex(), L"Layer_Logo", L"Prototype_GameObject_BackGround", &GameObjectDesc);
			NULL_CHECK_RETURN(pGameObject, );
			m_bRenderLoading = true;
		}
		else if (m_pPlayer->Get_CurRoomType(CPlayer::ROOM_C) == true && m_bRenderLoading == true)
		{
			CGameInstance* pGameInstance = CGameInstance::GetInstance();
			CGameObject* pGameObject = nullptr;
			CGameObject::GAMEOBJECTDESC	GameObjectDesc;
			GameObjectDesc.m_iCountType = 5;
			pGameObject = pGameInstance->Clone_GameObjectReturnPtr(pGameInstance->Get_StaticLevelIndex(), L"Layer_Logo", L"Prototype_GameObject_BackGround", &GameObjectDesc);
			NULL_CHECK_RETURN(pGameObject, );
			m_bRenderLoading = false;
		}
		else if (m_pPlayer->Get_CurRoomType(CPlayer::ROOM_BOSS) == true && m_bRenderLoading == false)
		{
			CGameInstance* pGameInstance = CGameInstance::GetInstance();
			CGameObject* pGameObject = nullptr;
			CGameObject::GAMEOBJECTDESC	GameObjectDesc;
			GameObjectDesc.m_iCountType = 6;
			pGameObject = pGameInstance->Clone_GameObjectReturnPtr(pGameInstance->Get_StaticLevelIndex(), L"Layer_Logo", L"Prototype_GameObject_BackGround", &GameObjectDesc);
			NULL_CHECK_RETURN(pGameObject, );
			m_bRenderLoading = true;
		}
	}


	if (m_bButton == false && m_bIsClicked == false)
	{
		_uint ButtonSize = (_uint)m_vecButton.size();

		for (_uint i = 0; i < ButtonSize; ++i)
		{
			if (dynamic_cast<CButton_Play*>(m_vecButton[i])->Check_Clicked())
			{
				if (i == 0)
				{
					m_bButton = true;

					CGameInstance* pGameInstance = CGameInstance::GetInstance();
					CGameObject* pGameObject = nullptr;
					CGameObject::GAMEOBJECTDESC	GameObjectDesc;
					GameObjectDesc.m_iCountType = 1;
					pGameObject = pGameInstance->Clone_GameObjectReturnPtr(pGameInstance->Get_StaticLevelIndex(), L"Layer_Logo", L"Prototype_GameObject_BackGround", &GameObjectDesc);
					NULL_CHECK_RETURN(pGameObject, );
				}
				else if (i == 1)
				{
					// 상점 창 띄우기
					CGameInstance* pGameInstance = CGameInstance::GetInstance();
					CGameObject* pGameObject = nullptr;
					CGameObject::GAMEOBJECTDESC	GameObjectDesc;
					GameObjectDesc.m_iCountType = 7;
					pGameObject = pGameInstance->Clone_GameObjectReturnPtr(pGameInstance->Get_StaticLevelIndex(), L"Layer_Logo", L"Prototype_GameObject_BackGround", &GameObjectDesc);
					NULL_CHECK_RETURN(pGameObject, );

				}
			}
		}
	}

	if(m_bIsClicked == true)
	{
		_uint vecsize = (_uint)m_vecLobbyObject.size();
	
		for(_uint i = 0; i < vecsize; ++i)
		{
			m_vecLobbyObject[i]->Set_Dead(true);
		}
		m_vecLobbyObject.clear();

		vecsize = (_uint)m_vecButton.size();

		for (_uint i = 0; i < vecsize; ++i)
		{
			m_vecButton[i]->Set_Dead(true);
		}
		m_vecButton.clear();

		Ready_Layer_Player(L"Layer_Player");
		Ready_Layer_BackGround(L"Layer_BackGround");
		Ready_Layer_Monster(L"Layer_Monster");
		Ready_Layer_Effect(L"Layer_Smoke");
		Ready_Layer_Trigger(L"Layer_Trigger");

		CGameInstance::GetInstance()->Set_LightPos(2, XMVectorSet(-11.08f, 1.734f, -3.234f, 1.f));
		

		CGameInstance::GetInstance()->Set_LightPos(3, XMVectorSet(-6.242f, 1.088f, 19.687f, 1.f));

		CGameObject::GAMEOBJECTDESC	tFireLightDesc;
		ZeroMemory(&tFireLightDesc, sizeof(CGameObject::GAMEOBJECTDESC));
		tFireLightDesc.TransformDesc.vInitPos = _float3(-11.08f, 1.335f, -3.234f);
		tFireLightDesc.m_iNumber = 2;
		tFireLightDesc.m_vTexSize = _float2(1.f, 2.f);
		CFire_Light*	pFire = nullptr;
		pFire = (CFire_Light*)CGameInstance::GetInstance()->Clone_GameObjectReturnPtr(LEVEL_GAMEPLAY, L"Layer_FireLight", L"Prototype_GameObject_Effect_Fire_Light", &tFireLightDesc);
		pFire->Set_Lobby(false);

		ZeroMemory(&tFireLightDesc, sizeof(CGameObject::GAMEOBJECTDESC));
		tFireLightDesc.TransformDesc.vInitPos = _float3(-6.242f, 1.335f, 19.687f);
		tFireLightDesc.m_iNumber = 3;
		tFireLightDesc.m_vTexSize = _float2(1.f, 2.f);
		pFire = (CFire_Light*)CGameInstance::GetInstance()->Clone_GameObjectReturnPtr(LEVEL_GAMEPLAY, L"Layer_FireLight", L"Prototype_GameObject_Effect_Fire_Light", &tFireLightDesc);
		pFire->Set_Lobby(false);

		_bool&		DynamicCamera = dynamic_cast<CCamera*>(CGameInstance::GetInstance()->Get_CloneObjectList(LEVEL_GAMEPLAY, L"Layer_ZCamera")->front())->Get_RenderState();
		_bool&		StaticCamera = dynamic_cast<CCamera*>(CGameInstance::GetInstance()->Get_CloneObjectList(LEVEL_GAMEPLAY, L"Layer_ZCamera")->back())->Get_RenderState();
		DynamicCamera = !DynamicCamera;
		StaticCamera = !StaticCamera;
		// CGameInstance::GetInstance()->Set_Far(50.f);
		CGameInstance::GetInstance()->Change_Camera();

		m_bButton = false;
		m_bIsClicked = false;

	}

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

HRESULT CLevel_GamePlay::Ready_Layer_Env(const wstring wstrLayerTag)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);
	FAILED_CHECK_RETURN(pGameInstance->Clone_GameObject(LEVEL_GAMEPLAY, wstrLayerTag, L"Prototype_GameObject_SkySphere"), E_FAIL);
	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Light()
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	LIGHTDESC			LightDesc;
	ZeroMemory(&LightDesc, sizeof LightDesc);

	LightDesc.eType = LIGHTDESC::LIGHT_FOV;
	LightDesc.isEnable = true;
	LightDesc.vPosition = _float4(0.f, 0.f, 0.f, 1.f);
	LightDesc.fRange = 15.f;
	LightDesc.vDirection = _float4(-1.f, -3.f, 1.0f, 0.f);
	LightDesc.vDiffuse = _float4(0.7f, 0.7f, 0.7f, 1.f);
	LightDesc.vAmbient = _float4(0.3f, 0.3f, 0.3f, 1.f);
	LightDesc.vSpecular = _float4(0.1f, 0.1f, 0.1f, 1.f);
	FAILED_CHECK_RETURN(pGameInstance->Add_Light(m_pDevice, m_pContext, LightDesc), E_FAIL);

	ZeroMemory(&LightDesc, sizeof(LIGHTDESC));

	LightDesc.eType = LIGHTDESC::LIGHT_DIRECTIONAL;
	LightDesc.isEnable = true;
	LightDesc.vDirection = _float4(-cosf(XMConvertToRadians(60.f)), -sinf(XMConvertToRadians(60.f)), 0.0f, 0.f);
	// LightDesc.vDirection = _float4(1.f, -1.f, 1.0f, 0.f);
	LightDesc.vDiffuse = _float4(1.f, 1.f, 1.f, 1.f);
	LightDesc.vAmbient = _float4(0.3f, 0.3f, 0.3f, 1.f);
	LightDesc.vSpecular = _float4(0.1f, 0.1f, 0.1f, 1.f);
	FAILED_CHECK_RETURN(pGameInstance->Add_Light(m_pDevice, m_pContext, LightDesc), E_FAIL);


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


	// Prototype_GameObject_Effect_Fire_Light

#pragma region LIGHT_2
	CGameObject::GAMEOBJECTDESC	tFireLightDesc;
	// ZeroMemory(&tFireLightDesc, sizeof(CGameObject::GAMEOBJECTDESC));
	// tFireLightDesc.TransformDesc.vInitPos = _float3(-11.08f, 1.335f, -3.234f);
	// tFireLightDesc.m_iNumber = 2;
	// tFireLightDesc.m_vTexSize = _float2(1.f, 2.f);
	// FAILED_CHECK_RETURN(pGameInstance->Clone_GameObject(LEVEL_GAMEPLAY, L"Layer_FireLight", L"Prototype_GameObject_Effect_Fire_Light", &tFireLightDesc), E_FAIL);
	//
	// ZeroMemory(&tFireLightDesc, sizeof(CGameObject::GAMEOBJECTDESC));
	// tFireLightDesc.TransformDesc.vInitPos = _float3(-6.242f, 1.335f, 19.687f);
	// tFireLightDesc.m_iNumber = 3;
	// tFireLightDesc.m_vTexSize = _float2(1.f, 2.f);
	// FAILED_CHECK_RETURN(pGameInstance->Clone_GameObject(LEVEL_GAMEPLAY, L"Layer_FireLight", L"Prototype_GameObject_Effect_Fire_Light", &tFireLightDesc), E_FAIL);

	ZeroMemory(&LightDesc, sizeof LightDesc);
	
	LightDesc.eType = LIGHTDESC::LIGHT_POINT;
	LightDesc.isEnable = true;
	/*LightDesc.vDirection = _float4(1.f, -1.f, 1.0f, 0.f);*/
	LightDesc.vPosition = _float4(-11.08f, 1.734f, -3.234f, 1.f);
	LightDesc.fRange = 8.0f;
	LightDesc.vDiffuse = _float4(0.9f, 0.3f, 0.15f, 1.f);
	LightDesc.vAmbient = _float4(0.2f, 0.2f, 0.2f, 0.2f);
	LightDesc.vSpecular = LightDesc.vDiffuse;
	FAILED_CHECK_RETURN(pGameInstance->Add_Light(m_pDevice, m_pContext, LightDesc), E_FAIL);
#pragma endregion

#pragma region LIGHT_3
	// ZeroMemory(&tFireLightDesc, sizeof(CGameObject::GAMEOBJECTDESC));
	// tFireLightDesc.TransformDesc.vInitPos = _float3(-6.242f, 1.335f, 19.687f);
	// tFireLightDesc.m_iNumber = 3;
	// tFireLightDesc.m_vTexSize = _float2(1.f, 2.f);
	// FAILED_CHECK_RETURN(pGameInstance->Clone_GameObject(LEVEL_GAMEPLAY, L"Layer_FireLight", L"Prototype_GameObject_Effect_Fire_Light", &tFireLightDesc), E_FAIL);

	ZeroMemory(&LightDesc, sizeof LightDesc);

	LightDesc.eType = LIGHTDESC::LIGHT_POINT;
	LightDesc.isEnable = true;
	/*LightDesc.vDirection = _float4(1.f, -1.f, 1.0f, 0.f);*/
	LightDesc.vPosition = _float4(-6.242f, 1.088f, 19.687f, 1.f);
	LightDesc.fRange = 8.0f;
	LightDesc.vDiffuse = _float4(0.9f, 0.3f, 0.15f, 1.f);
	LightDesc.vAmbient = _float4(0.2f, 0.2f, 0.2f, 0.2f);
	LightDesc.vSpecular = LightDesc.vDiffuse;
	FAILED_CHECK_RETURN(pGameInstance->Add_Light(m_pDevice, m_pContext, LightDesc), E_FAIL);
#pragma endregion

#pragma region LIGHT_4


	ZeroMemory(&tFireLightDesc, sizeof(CGameObject::GAMEOBJECTDESC));
	tFireLightDesc.TransformDesc.vInitPos = _float3(1.652f, 3.439f, 24.013f);
	tFireLightDesc.m_iNumber = 4;
	tFireLightDesc.m_vTexSize = _float2(0.7f, 2.f);
	for(_uint i =0; i<15; ++i)
		FAILED_CHECK_RETURN(pGameInstance->Clone_GameObject(LEVEL_GAMEPLAY, L"Layer_FireLight", L"Prototype_GameObject_BlueLight", &tFireLightDesc), E_FAIL);

	ZeroMemory(&LightDesc, sizeof LightDesc);

	LightDesc.eType = LIGHTDESC::LIGHT_POINT;
	LightDesc.isEnable = true;


	LightDesc.vPosition = _float4(1.652f, 3.87f, 24.013f, 1.f);
	LightDesc.fRange = 8.0f;
	LightDesc.vDiffuse = _float4(0.6f, 0.87f, 0.9f, 1.f);
	LightDesc.vAmbient = _float4(0.2f, 0.2f, 0.2f, 0.2f);
	LightDesc.vSpecular = LightDesc.vDiffuse;
	FAILED_CHECK_RETURN(pGameInstance->Add_Light(m_pDevice, m_pContext, LightDesc), E_FAIL);
#pragma endregion

#pragma region LIGHT_5
	ZeroMemory(&tFireLightDesc, sizeof(CGameObject::GAMEOBJECTDESC));
	tFireLightDesc.TransformDesc.vInitPos = _float3(1.628f, 3.439f, 28.932f);
	tFireLightDesc.m_iNumber = 5;
	tFireLightDesc.m_vTexSize = _float2(0.7f, 2.f);
	for (_uint i = 0; i<15; ++i)
		FAILED_CHECK_RETURN(pGameInstance->Clone_GameObject(LEVEL_GAMEPLAY, L"Layer_FireLight", L"Prototype_GameObject_BlueLight", &tFireLightDesc), E_FAIL);

	ZeroMemory(&LightDesc, sizeof LightDesc);

	LightDesc.eType = LIGHTDESC::LIGHT_POINT;
	LightDesc.isEnable = true;
	/*LightDesc.vDirection = _float4(1.f, -1.f, 1.0f, 0.f);*/
	LightDesc.vPosition = _float4(1.628f, 3.87f, 28.932f, 1.f);
	LightDesc.fRange = 8.0f;
	LightDesc.vDiffuse = _float4(0.6f, 0.87f, 0.9f, 1.f);
	LightDesc.vAmbient = _float4(0.2f, 0.2f, 0.2f, 0.2f);
	LightDesc.vSpecular = LightDesc.vDiffuse;
	FAILED_CHECK_RETURN(pGameInstance->Add_Light(m_pDevice, m_pContext, LightDesc), E_FAIL);
#pragma endregion

#pragma region LIGHT_6
	ZeroMemory(&tFireLightDesc, sizeof(CGameObject::GAMEOBJECTDESC));
	tFireLightDesc.TransformDesc.vInitPos = _float3(-15.879f, 1.335f, 29.835f);
	tFireLightDesc.m_iNumber = 6;
	tFireLightDesc.m_vTexSize = _float2(1.f, 2.f);
	FAILED_CHECK_RETURN(pGameInstance->Clone_GameObject(LEVEL_GAMEPLAY, L"Layer_FireLight", L"Prototype_GameObject_Effect_Fire_Light", &tFireLightDesc), E_FAIL);

	ZeroMemory(&LightDesc, sizeof LightDesc);

	LightDesc.eType = LIGHTDESC::LIGHT_POINT;
	LightDesc.isEnable = true;
	/*LightDesc.vDirection = _float4(1.f, -1.f, 1.0f, 0.f);*/
	LightDesc.vPosition = _float4(-15.879f, 1.08f, 29.835f, 1.f);
	LightDesc.fRange = 8.0f;
	LightDesc.vDiffuse = _float4(0.9f, 0.3f, 0.15f, 1.f);
	LightDesc.vAmbient = _float4(0.2f, 0.2f, 0.2f, 0.2f);
	LightDesc.vSpecular = LightDesc.vDiffuse;
	FAILED_CHECK_RETURN(pGameInstance->Add_Light(m_pDevice, m_pContext, LightDesc), E_FAIL);
#pragma endregion

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

	CCamera::CAMERADESC			CameraDesc;
	ZeroMemory(&CameraDesc, sizeof(CCamera::CAMERADESC));

	CameraDesc.vEye = _float4(1.f, 3.f, -1.f, 1.f);
	CameraDesc.vAt = _float4(0.f, 0.f, 0.f, 1.f);
	CameraDesc.vUp = _float4(0.f, 1.f, 0.f, 0.f);

	CameraDesc.fFovy = XMConvertToRadians(60.0f);
	CameraDesc.fAspect = 2560.f / 1369.f;
	CameraDesc.fNear = 0.2f;
	CameraDesc.fFar = 300.f;
	CameraDesc.TransformDesc.fSpeedPerSec = 10.f;
	CameraDesc.TransformDesc.fRotationPerSec = XMConvertToRadians(90.f);

	FAILED_CHECK_RETURN(pGameInstance->Add_Camera(LEVEL_GAMEPLAY, wstrLayerTag, L"Prototype_GameObject_Camera_Dynamic", CameraDesc), E_FAIL);
	FAILED_CHECK_RETURN(pGameInstance->Add_Camera(LEVEL_GAMEPLAY, wstrLayerTag, L"Prototype_GameObject_Camera_Static", CameraDesc), E_FAIL);

	RELEASE_INSTANCE(CGameInstance);
	
	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Monster(const wstring wstrLayerTag)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);
	CMonster*			pMonster = nullptr;
	CPlayer*			pPlayer = dynamic_cast<CPlayer*>(pGameInstance->Get_CloneObjectList(LEVEL_GAMEPLAY, L"Layer_Player")->front());
	NULL_CHECK_RETURN(pPlayer, E_FAIL);

	_matrix PivotMatrix = XMMatrixIdentity(); //* XMMatrixRotationAxis(XMVectorSet(0.f,1.f,0.f, 0.f),XMConvertToRadians(180.f));
	// PivotMatrix.r[3] = XMVectorSet(110.f, 1.3f, 179.f, 1.f);
	// // SWORD
	PivotMatrix.r[3] = XMVectorSet(-14.f, 0.f, 0.8f, 1.f);
	CMonster::MONSTEROPTION			MonsterDesc;
	ZeroMemory(&MonsterDesc, sizeof(CMonster::MONSTEROPTION));
	MonsterDesc.m_bFirstSpawnType[CMonster::STATE_ALREADYSPAWN] = true;
	MonsterDesc.MonsterDesc.m_iHP = MonsterDesc.MonsterDesc.m_iMaxHP = 300;
	MonsterDesc.MonsterDesc.m_iDamage = 15;
	MonsterDesc.MonsterDesc.m_iShield = MonsterDesc.MonsterDesc.m_iMaxShield = 0;
	MonsterDesc.MonsterDesc.TransformDesc.fSpeedPerSec = 5.f;
	MonsterDesc.m_iCellIndex = 40;
	pMonster = dynamic_cast<CMonster*>(pGameInstance->Clone_GameObjectReturnPtr_M(LEVEL_GAMEPLAY, wstrLayerTag, L"Prototype_GameObject_Normal_Human_Sword", PivotMatrix, &MonsterDesc));
	pMonster->Set_Player(pPlayer);

	////이펙트 확인용
	// CGameObject::GAMEOBJECTDESC GameObjectDesc;
	// GameObjectDesc.TransformDesc.vInitPos = _float3(-14.f, 0.f, 0.8f);
	// (CGameInstance::GetInstance()->Clone_GameObjectReturnPtr(LEVEL_GAMEPLAY, L"Layer_StoneLight", L"Prototype_GameObject_Normal_Boss_StonePillar_Light", &GameObjectDesc));




	//////////////////


	// //
	ZeroMemory(&MonsterDesc, sizeof(CMonster::MONSTEROPTION));
	PivotMatrix.r[3] = XMVectorSet(-11.8f, 0.f, 8.8f, 1.f);
	MonsterDesc.m_bFirstSpawnType[CMonster::STATE_ALREADYSPAWN] = true;
	MonsterDesc.MonsterDesc.m_iHP = MonsterDesc.MonsterDesc.m_iMaxHP = 300;
	MonsterDesc.MonsterDesc.m_iDamage = 15;
	MonsterDesc.MonsterDesc.m_iShield = MonsterDesc.MonsterDesc.m_iMaxShield = 0;
	MonsterDesc.MonsterDesc.TransformDesc.fSpeedPerSec = 5.f;
	MonsterDesc.m_iCellIndex = 50;
	pMonster = dynamic_cast<CMonster*>(pGameInstance->Clone_GameObjectReturnPtr_M(LEVEL_GAMEPLAY, wstrLayerTag, L"Prototype_GameObject_Normal_Human_Sword", PivotMatrix, &MonsterDesc));
	pMonster->Set_Player(pPlayer);
	//
	ZeroMemory(&MonsterDesc, sizeof(CMonster::MONSTEROPTION));
	PivotMatrix.r[3] = XMVectorSet(-4.45f, 0.f, 17.8f, 1.f);
	MonsterDesc.m_bFirstSpawnType[CMonster::STATE_ALREADYSPAWN] = true;
	MonsterDesc.MonsterDesc.m_iHP = MonsterDesc.MonsterDesc.m_iMaxHP = 300;
	MonsterDesc.MonsterDesc.m_iDamage = 15;
	MonsterDesc.MonsterDesc.m_iShield = MonsterDesc.MonsterDesc.m_iMaxShield = 0;
	MonsterDesc.MonsterDesc.TransformDesc.fSpeedPerSec = 5.f;
	MonsterDesc.m_iCellIndex = 91;
	pMonster = dynamic_cast<CMonster*>(pGameInstance->Clone_GameObjectReturnPtr_M(LEVEL_GAMEPLAY, wstrLayerTag, L"Prototype_GameObject_Normal_Human_Sword", PivotMatrix, &MonsterDesc));
	pMonster->Set_Player(pPlayer);
	// // //
	ZeroMemory(&MonsterDesc, sizeof(CMonster::MONSTEROPTION));
	PivotMatrix.r[3] = XMVectorSet(-5.7f, 1.1f, 7.5f, 1.f);
	MonsterDesc.m_bFirstSpawnType[CMonster::STATE_ALREADYSPAWN] = true;
	MonsterDesc.MonsterDesc.m_iHP = MonsterDesc.MonsterDesc.m_iMaxHP = 300;
	MonsterDesc.MonsterDesc.m_iDamage = 15;
	MonsterDesc.MonsterDesc.m_iShield = MonsterDesc.MonsterDesc.m_iMaxShield = 0;
	MonsterDesc.MonsterDesc.TransformDesc.fSpeedPerSec = 5.f;
	MonsterDesc.m_iCellIndex = 100;
	pMonster = dynamic_cast<CMonster*>(pGameInstance->Clone_GameObjectReturnPtr_M(LEVEL_GAMEPLAY, wstrLayerTag, L"Prototype_GameObject_Normal_Human_Sword", PivotMatrix, &MonsterDesc));
	pMonster->Set_Player(pPlayer);
	// // 그라운드 스폰
	ZeroMemory(&MonsterDesc, sizeof(CMonster::MONSTEROPTION));
	PivotMatrix.r[3] = XMVectorSet(-10.1f, 0.28f, 24.f, 1.f);
	MonsterDesc.m_bFirstSpawnType[CMonster::STATE_ALREADYSPAWN] = true;
	MonsterDesc.MonsterDesc.m_iHP = MonsterDesc.MonsterDesc.m_iMaxHP = 300;
	MonsterDesc.MonsterDesc.m_iDamage = 15;
	MonsterDesc.MonsterDesc.m_iShield = MonsterDesc.MonsterDesc.m_iMaxShield = 0;
	MonsterDesc.MonsterDesc.TransformDesc.fSpeedPerSec = 5.f;
	MonsterDesc.m_iCellIndex = 186;
	pMonster = dynamic_cast<CMonster*>(pGameInstance->Clone_GameObjectReturnPtr_M(LEVEL_GAMEPLAY, wstrLayerTag, L"Prototype_GameObject_Normal_Human_Sword", PivotMatrix, &MonsterDesc));
	pMonster->Set_Player(pPlayer);
	// // ~그라운드 스폰
	
	
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
	// CMonster::MONSTEROPTION			MonsterDesc;
	
	ZeroMemory(&MonsterDesc, sizeof(CMonster::MONSTEROPTION));
	PivotMatrix.r[3] = XMVectorSet(-15.7f, 0.f, 26.f, 1.f);
	MonsterDesc.m_bFirstSpawnType[CMonster::STATE_ALREADYSPAWN] = true;
	MonsterDesc.MonsterDesc.m_iHP = MonsterDesc.MonsterDesc.m_iMaxHP = 300;
	MonsterDesc.MonsterDesc.m_iDamage = 15;
	MonsterDesc.MonsterDesc.m_iShield = MonsterDesc.MonsterDesc.m_iMaxShield = 0;
	MonsterDesc.MonsterDesc.TransformDesc.fSpeedPerSec = 2.5f;
	MonsterDesc.MonsterDesc.TransformDesc.fRotationPerSec = XMConvertToRadians(180.f);
	MonsterDesc.m_iCellIndex = 188;
	pMonster = dynamic_cast<CMonster*>(pGameInstance->Clone_GameObjectReturnPtr_M(LEVEL_GAMEPLAY, wstrLayerTag, L"Prototype_GameObject_Normal_Human_Bow", PivotMatrix, &MonsterDesc));
	pMonster->Set_Player(pPlayer);
	//
	ZeroMemory(&MonsterDesc, sizeof(CMonster::MONSTEROPTION));
	PivotMatrix.r[3] = XMVectorSet(-15.3f, 0.f, 15.7f, 1.f);
	MonsterDesc.m_bFirstSpawnType[CMonster::STATE_NODETECTED] = true;			// No Detected == Patrol
	MonsterDesc.MonsterDesc.m_iHP = MonsterDesc.MonsterDesc.m_iMaxHP = 300;
	MonsterDesc.MonsterDesc.m_iDamage = 15;
	MonsterDesc.MonsterDesc.m_iShield = MonsterDesc.MonsterDesc.m_iMaxShield = 0;
	MonsterDesc.MonsterDesc.TransformDesc.fSpeedPerSec = 2.5f;
	MonsterDesc.MonsterDesc.TransformDesc.fRotationPerSec = XMConvertToRadians(180.f);
	MonsterDesc.m_iCellIndex = 71;
	pMonster = dynamic_cast<CMonster*>(pGameInstance->Clone_GameObjectReturnPtr_M(LEVEL_GAMEPLAY, wstrLayerTag, L"Prototype_GameObject_Normal_Human_Granade", PivotMatrix, &MonsterDesc));
	pMonster->Set_Player(pPlayer);
	//
	ZeroMemory(&MonsterDesc, sizeof(CMonster::MONSTEROPTION));
	PivotMatrix.r[3] = XMVectorSet(4.f, 0.f, 13.7f, 1.f);
	MonsterDesc.m_bFirstSpawnType[CMonster::STATE_NODETECTED] = true;			// No Detected == Patrol
	MonsterDesc.MonsterDesc.m_iHP = MonsterDesc.MonsterDesc.m_iMaxHP = 300;
	MonsterDesc.MonsterDesc.m_iDamage = 15;
	MonsterDesc.MonsterDesc.m_iShield = MonsterDesc.MonsterDesc.m_iMaxShield = 0;
	MonsterDesc.MonsterDesc.TransformDesc.fSpeedPerSec = 2.5f;
	MonsterDesc.MonsterDesc.TransformDesc.fRotationPerSec = XMConvertToRadians(180.f);
	MonsterDesc.m_iCellIndex = 135;
	pMonster = dynamic_cast<CMonster*>(pGameInstance->Clone_GameObjectReturnPtr_M(LEVEL_GAMEPLAY, wstrLayerTag, L"Prototype_GameObject_Normal_Human_Granade", PivotMatrix, &MonsterDesc));
	pMonster->Set_Player(pPlayer);
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
	// // // Normal Explode
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

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Player(const wstring wstrLayerTag)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);
	CGameObject* pGameObject = nullptr;
	pGameObject = pGameInstance->Clone_GameObjectReturnPtr(LEVEL_GAMEPLAY, wstrLayerTag, L"Prototype_GameObject_Player");
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	m_pPlayer = (CPlayer*)pGameObject;


	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_LaiHome(const wstring wstrLayerTag)
{
	CGameInstance*	pGameInstance = GET_INSTANCE(CGameInstance);
	_matrix PivotMatrix = XMMatrixIdentity();
	_float	vPos[3], vScale[3], vAngle[3];
	vPos[0] = -24.316f;	vPos[1] = 0.f; vPos[2] = -24.043f;
	vScale[0] = 1.f;	vScale[1] = 1.f;	vScale[2] = 1.0f;
	vAngle[0] = 0.f;	vAngle[1] = -0.f;	vAngle[2] = 0.f;
	ImGuizmo::RecomposeMatrixFromComponents(vPos, vAngle, vScale, (_float*)&PivotMatrix);
	CGameObject::GAMEOBJECTDESC	tmp;
	ZeroMemory(&tmp, sizeof(CGameObject::GAMEOBJECTDESC));
	CGameObject* pObject = nullptr;
	pObject = pGameInstance->Clone_GameObjectReturnPtr_M(LEVEL_GAMEPLAY, wstrLayerTag, L"Prototype_GameObject_LaiHome", PivotMatrix, &tmp);
	NULL_CHECK_RETURN(pObject, E_FAIL);
	m_vecLobbyObject.push_back(pObject);

	vPos[0] = -21.986f;	vPos[1] = 1.598f;  vPos[2] = -23.475f;
	vScale[0] = 1.f;	vScale[1] = 1.f;	vScale[2] = 1.0f;
	vAngle[0] = 180.f;	vAngle[1] = 1.003f;	vAngle[2] = 180.f;
	ImGuizmo::RecomposeMatrixFromComponents(vPos, vAngle, vScale, (_float*)&PivotMatrix);
	pObject = pGameInstance->Clone_GameObjectReturnPtr_M(LEVEL_GAMEPLAY, wstrLayerTag, L"Prototype_GameObject_Lobby_Owner", PivotMatrix, &tmp);
	NULL_CHECK_RETURN(pObject, E_FAIL);
	m_vecLobbyObject.push_back(pObject);

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Lobby(const wstring wstrLayerTag)
{
	CGameInstance*	pGameInstance = GET_INSTANCE(CGameInstance);
	_matrix PivotMatrix = XMMatrixIdentity();
	_float	vPos[3], vScale[3], vAngle[3];
	ImGuizmo::DecomposeMatrixToComponents((_float*)&PivotMatrix, vPos, vAngle, vScale);
	vPos[0] = -50.f;	vPos[1] = 0.f; vPos[2] = -50.f;
	vScale[0] = 1.f;	vScale[1] = 1.f;	vScale[2] = 1.f;
	vAngle[0] = 0.f;	vAngle[1] = -0.f;	vAngle[2] = 0.f;
	ImGuizmo::RecomposeMatrixFromComponents(vPos, vAngle, vScale, (_float*)&PivotMatrix);
	CGameObject::GAMEOBJECTDESC	tmp;
	ZeroMemory(&tmp, sizeof(CGameObject::GAMEOBJECTDESC));
	CGameObject* pObject = nullptr;
	pObject = pGameInstance->Clone_GameObjectReturnPtr_M(LEVEL_GAMEPLAY, wstrLayerTag, L"Prototype_GameObject_Home", PivotMatrix, &tmp);
	NULL_CHECK_RETURN(pObject, E_FAIL);
	m_vecLobbyObject.push_back(pObject);

	CGameObject::GAMEOBJECTDESC	tFireLightDesc;
	// -24.316f;	vPos[1] = 0.f; vPos[2] = -24.043f

	pGameInstance->Set_LightPos(2, XMVectorSet(-21.837f, 3.311f, -25.421f, 1.f));
	pGameInstance->Set_LightRange(2, 3.f);

	ZeroMemory(&tFireLightDesc, sizeof(CGameObject::GAMEOBJECTDESC));
	tFireLightDesc.TransformDesc.vInitPos = _float3(-21.837f, 3.311f, -25.421f);
	tFireLightDesc.m_iNumber = 2;
	tFireLightDesc.m_vTexSize = _float2(0.2f, 0.5f);
	pObject = pGameInstance->Clone_GameObjectReturnPtr(LEVEL_GAMEPLAY, L"Layer_FireLight", L"Prototype_GameObject_Effect_Fire_Light", &tFireLightDesc);
	NULL_CHECK_RETURN(pObject, E_FAIL);
	CFire_Light* pFire = nullptr;
	pFire = (CFire_Light*)pObject;
	pFire->Set_Lobby(true);
	m_vecLobbyObject.push_back(pObject);

	pGameInstance->Set_LightPos(3, XMVectorSet(-33.580f, 5.57f, -20.514f, 1.f));
	pGameInstance->Set_LightRange(3, 3.f);
	

	ZeroMemory(&tFireLightDesc, sizeof(CGameObject::GAMEOBJECTDESC));
	tFireLightDesc.TransformDesc.vInitPos = _float3(-33.580f, 5.57f, -20.514f);
	tFireLightDesc.m_iNumber = 3;
	tFireLightDesc.m_vTexSize = _float2(0.2f, 0.5f);
	pObject = pGameInstance->Clone_GameObjectReturnPtr(LEVEL_GAMEPLAY, L"Layer_FireLight", L"Prototype_GameObject_Effect_Fire_Light", &tFireLightDesc);
	NULL_CHECK_RETURN(pObject, E_FAIL);
	pFire = (CFire_Light*)pObject;
	pFire->Set_Lobby(true);
	m_vecLobbyObject.push_back(pObject);

	CGameObject::GAMEOBJECTDESC	CursorDesc;
	ZeroMemory(&CursorDesc, sizeof(CGameObject::GAMEOBJECTDESC));
	CursorDesc.m_iCountType = 1;
	pObject = pGameInstance->Clone_GameObjectReturnPtr(LEVEL_GAMEPLAY, L"Layer_Cursor", L"Prototype_GameObject_Battle_Cursor", &CursorDesc);
	NULL_CHECK_RETURN(pObject, E_FAIL);
	m_vecLobbyObject.push_back(pObject);

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_LobbyButton(const wstring wstrLayerTag)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	CGameObject::GAMEOBJECTDESC ButtonDesc;

	ZeroMemory(&ButtonDesc, sizeof(CGameObject::GAMEOBJECTDESC));
	ButtonDesc.TransformDesc.vInitPos = _float3( -700.f, 300.f,0.f);
	ButtonDesc.m_iCountType = 0;
	CGameObject* pObject = nullptr;
	pObject = pGameInstance->Clone_GameObjectReturnPtr(LEVEL_GAMEPLAY, wstrLayerTag, L"Prototype_GameObject_Button", &ButtonDesc);
	NULL_CHECK_RETURN(pObject, E_FAIL);
	m_vecButton.push_back(pObject);

	ZeroMemory(&ButtonDesc, sizeof(CGameObject::GAMEOBJECTDESC));
	ButtonDesc.TransformDesc.vInitPos = _float3(-700.f, 0.f, 0.f);
	ButtonDesc.m_iCountType = 1;
	pObject = pGameInstance->Clone_GameObjectReturnPtr(LEVEL_GAMEPLAY, wstrLayerTag, L"Prototype_GameObject_Button", &ButtonDesc);
	NULL_CHECK_RETURN(pObject, E_FAIL);
	m_vecButton.push_back(pObject);

	ZeroMemory(&ButtonDesc, sizeof(CGameObject::GAMEOBJECTDESC));
	ButtonDesc.TransformDesc.vInitPos = _float3(-700.f, -300.f, 0.f);
	ButtonDesc.m_iCountType = 2;
	pObject = pGameInstance->Clone_GameObjectReturnPtr(LEVEL_GAMEPLAY, wstrLayerTag, L"Prototype_GameObject_Button", &ButtonDesc);
	NULL_CHECK_RETURN(pObject, E_FAIL);
	m_vecButton.push_back(pObject);

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Effect(const wstring wstrLayerTag)
{
	CGameInstance*	pGameInstance = GET_INSTANCE(CGameInstance);

	CGameObject::GAMEOBJECTDESC	EffectDesc;
	ZeroMemory(&EffectDesc, sizeof(CGameObject::GAMEOBJECTDESC));
	EffectDesc.TransformDesc.vInitPos = _float3(-14.f, 3.f, 0.f);
	EffectDesc.TransformDesc.fRotationPerSec = -68.454f;
	for(_uint i =0; i< 2; ++i)
		FAILED_CHECK_RETURN(pGameInstance->Clone_GameObject(LEVEL_GAMEPLAY, wstrLayerTag, L"Prototype_GameObject_Effect_Smoke", &EffectDesc), E_FAIL);


	EffectDesc.TransformDesc.vInitPos = _float3(-9.629f, 0.729f, 43.134f);
	EffectDesc.TransformDesc.fRotationPerSec = -28.f;
	for (_uint i = 0; i< 2; ++i)
		FAILED_CHECK_RETURN(pGameInstance->Clone_GameObject(LEVEL_GAMEPLAY, wstrLayerTag, L"Prototype_GameObject_Effect_Smoke", &EffectDesc), E_FAIL);

	// EffectDesc.TransformDesc.vInitPos = _float3(-9.629f, 0.729f, 43.134f);
	// EffectDesc.TransformDesc.fRotationPerSec = -28.f;
	// for (_uint i = 0; i< 2; ++i)
	// 	FAILED_CHECK_RETURN(pGameInstance->Clone_GameObject(LEVEL_GAMEPLAY, wstrLayerTag, L"Prototype_GameObject_Effect_Smoke", &EffectDesc), E_FAIL);

	EffectDesc.TransformDesc.vInitPos = _float3(-34.f, 1.2f, 52.7f);
	EffectDesc.TransformDesc.fRotationPerSec = 17.f;
	for (_uint i = 0; i< 2; ++i)
		FAILED_CHECK_RETURN(pGameInstance->Clone_GameObject(LEVEL_GAMEPLAY, wstrLayerTag, L"Prototype_GameObject_Effect_Smoke", &EffectDesc), E_FAIL);

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

	// CMonster*			pMonster = nullptr;
	// CPlayer*			pPlayer = dynamic_cast<CPlayer*>(pGameInstance->Get_CloneObjectList(LEVEL_GAMEPLAY, L"Layer_Player")->front());
	// NULL_CHECK_RETURN(pPlayer, E_FAIL);

	
	// Boss 
	// CMonster::MONSTEROPTION			MonsterDesc;


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
	// PivotMatrix.r[0] = XMVectorSet(-10.815f, 0.015f, 31.333f, 1.f);
	// PivotMatrix.r[1] = XMVectorSet(-10.815f, 0.015f, 31.333f, 1.f);
	// PivotMatrix.r[2] = XMVectorSet(-10.815f, 0.015f, 31.333f, 1.f);
	// PivotMatrix.r[3] = XMVectorSet(-10.815f, 0.015f, 31.333f, 1.f);

	_float	vPos[3], vScale[3], vAngle[3];
	ImGuizmo::DecomposeMatrixToComponents((_float*)&PivotMatrix, vPos, vAngle, vScale);
	vPos[0] = -10.815f;	vPos[1] = 2.579f; vPos[2] = 30.469f;
	// vScale[0] = 6.397f;	vScale[1] = 8.087f;	vScale[2] = 1.0f;
	vScale[0] = 4.5f;	vScale[1] = 4.5f;	vScale[2] = 1.0f;

	vAngle[0] = 0.f;	vAngle[1] = -0.f;	vAngle[2] = 0.f;
	ImGuizmo::RecomposeMatrixFromComponents(vPos, vAngle, vScale, (_float*)&PivotMatrix);

	TriggerDesc.m_eType = CTrigger::TRIGGER_A_TO_KNIGHT;
	pTrigger = (CTrigger*)pGameInstance->Clone_GameObjectReturnPtr_M(LEVEL_GAMEPLAY, wstrLayerTag, L"Prototype_GameObject_Trigger", PivotMatrix, &TriggerDesc);
	NULL_CHECK_RETURN(pTrigger, E_FAIL);
	pTrigger->Set_Owner(pPlayer);

	// PivotMatrix.r[3] = XMVectorSet(-10.815f, 0.015f, 31.333f, 1.f);
	// TriggerDesc.m_eType = CTrigger::TRIGGER_C_TO_BOSS;
	// pTrigger = (CTrigger*)pGameInstance->Clone_GameObjectReturnPtr_M(LEVEL_GAMEPLAY, wstrLayerTag, L"Prototype_GameObject_Trigger", PivotMatrix, &TriggerDesc);
	// NULL_CHECK_RETURN(pTrigger, E_FAIL);
	// pTrigger->Set_Owner(pPlayer);

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
