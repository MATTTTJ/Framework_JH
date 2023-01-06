#include "stdafx.h"
#include "..\public\Level_GamePlay.h"
#include "GameInstance.h"

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
	FAILED_CHECK_RETURN(Ready_Layer_Effect(L"Layer_Effect"), E_FAIL);

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
	FAILED_CHECK_RETURN(pGameInstance->Clone_GameObject(LEVEL_GAMEPLAY, wstrLayerTag, L"Prototype_GameObject_Sky"), E_FAIL);
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

	// FAILED_CHECK_RETURN(pGameInstance->Clone_GameObject(LEVEL_GAMEPLAY, wstrLayerTag, L"Prototype_GameObject_Monster"), E_FAIL)



	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Player(const wstring wstrLayerTag)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	// {
	// 	if (FAILED(pGameInstance->Clone_GameObject(LEVEL_GAMEPLAY, wstrLayerTag, L"Prototype_GameObject_Player")))
	// 		return E_FAIL;
	// }

	if (FAILED(pGameInstance->Clone_GameObject(LEVEL_GAMEPLAY, wstrLayerTag, L"Prototype_GameObject_Player")))
		return E_FAIL;
	
	// if (FAILED(pGameInstance->Clone_GameObject(LEVEL_GAMEPLAY, wstrLayerTag, L"Prototype_GameObject_LaiHome")))
	// 	return E_FAIL;

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
