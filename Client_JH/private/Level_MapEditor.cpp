#include "stdafx.h"
#include "Level_MapEditor.h"
#include "GameInstance.h"


CLevel_MapEditor::CLevel_MapEditor(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CLevel(pDevice,pContext)
{
}

HRESULT CLevel_MapEditor::Initialize()
{
	if (FAILED(__super::Initialize()))
		return E_FAIL;

	FAILED_CHECK_RETURN(Ready_Light(), E_FAIL);
	//
	// FAILED_CHECK_RETURN(Ready_Layer_Background(L"Layer_Terrain"), E_FAIL);
	//
	// FAILED_CHECK_RETURN(Ready_Layer_Camera(L"Layer_Camera"), E_FAIL);
	//
	// FAILED_CHECK_RETURN(Ready_Layer_StaticMesh(L"Layer_StaticMesh"), E_FAIL);

	return S_OK;
}

void CLevel_MapEditor::Tick(_double TimeDelta)
{
	__super::Tick(TimeDelta);
}

void CLevel_MapEditor::Late_Tick(_double TimeDelta)
{
	__super::Late_Tick(TimeDelta);
}

HRESULT CLevel_MapEditor::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	SetWindowText(g_hWnd, TEXT("Level : MAPTOOL "));

	return S_OK;
}

HRESULT CLevel_MapEditor::Ready_Light()
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

HRESULT CLevel_MapEditor::Ready_Layer_Background(const wstring wstrLayerTag)
{
	// CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	// if (FAILED(pGameInstance->Clone_GameObject(LEVEL_MAPEDITOR, wstrLayerTag, L"Prototype_GameObject_Terrain")))
	// 	return E_FAIL;

	// RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLevel_MapEditor::Ready_Layer_Camera(const wstring wstrLayerTag)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	// if (FAILED(pGameInstance->Clone_GameObject(LEVEL_MAPEDITOR, wstrLayerTag, L"Prototype_GameObject_Camera_Dynamic")))
	// 	return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}
CLevel_MapEditor* CLevel_MapEditor::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CLevel_MapEditor*		pInstance = new CLevel_MapEditor(pDevice, pContext);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created : CLevel_MapEditor");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CLevel_MapEditor::Free()
{
	__super::Free();
}


