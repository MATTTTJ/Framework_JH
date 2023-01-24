#include "stdafx.h"
#include "..\public\Level_Logo.h"

#include "Level_Loading.h"
#include "GameInstance.h"
#include "Imgui_AnimationMgr.h"
#include "Imgui_PropertyEditor.h"
#include "Imgui_LevelSwitcher.h"
#include "Imgui_MapEditor.h"
#include "Imgui_ProtoMgr.h"
#include "Imgui_Setting.h"
#include "Imgui_ModelSave.h"
#include "Imgui_NavigationEditor.h"

CLevel_Logo::CLevel_Logo(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CLevel(pDevice, pContext)
{
}

HRESULT CLevel_Logo::Initialize()
{
	if (FAILED(__super::Initialize()))
		return E_FAIL;

	// if (FAILED(Ready_Layer_BackGround(L"Layer_BackGround")))
	// 	return E_FAIL;
	// CGameInstance::GetInstance()->Clear_ImguiObjects();
	// CGameInstance::GetInstance()->Add_ImguiWindowObject(CImgui_Setting::Create());
	// CGameInstance::GetInstance()->Add_ImguiWindowObject(CImgui_LevelSwitcher::Create(m_pDevice, m_pContext));
	// CGameInstance::GetInstance()->Add_ImguiWindowObject(CImgui_ProtoMgr::Create(m_pDevice, m_pContext));
	CGameInstance::GetInstance()->Add_ImguiWindowObject(CImgui_MapEditor::Create());
	CGameInstance::GetInstance()->Add_ImguiWindowObject(CImgui_AnimationMgr::Create());
	CGameInstance::GetInstance()->Add_ImguiWindowObject(CImgui_ModelSave::Create(m_pDevice, m_pContext));
	CGameInstance::GetInstance()->Add_ImguiWindowObject(CImgui_NavigationEditor::Create(m_pDevice, m_pContext));

	return S_OK;
}

void CLevel_Logo::Tick(_double TimeDelta)
{
	__super::Tick(TimeDelta);

}

void CLevel_Logo::Late_Tick(_double TimeDelta)
{
	__super::Late_Tick(TimeDelta);

	if (GetKeyState(VK_SPACE) & 0x8000)
	{
		CGameInstance*		pGameInstance = CGameInstance::GetInstance();
		Safe_AddRef(pGameInstance);
	
		if (FAILED(pGameInstance->Open_Level(LEVEL_LOADING, CLevel_Loading::Create(m_pDevice, m_pContext, LEVEL_GAMEPLAY))))
			return;
	
		Safe_Release(pGameInstance);
	
	}

}

HRESULT CLevel_Logo::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	SetWindowText(g_hWnd, TEXT("Level : LOGO"));

	return S_OK;
}

HRESULT CLevel_Logo::Ready_Layer_BackGround(const wstring& wstrLayerTag)
{
	CGameInstance*		pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	if (FAILED(pGameInstance->Clone_GameObject(LEVEL_LOGO, wstrLayerTag, L"Prototype_GameObject_BackGround")))
		return E_FAIL;

	Safe_Release(pGameInstance);

	return S_OK;
}

CLevel_Logo * CLevel_Logo::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CLevel_Logo*		pInstance = new CLevel_Logo(pDevice, pContext);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created : CLevel_Logo");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CLevel_Logo::Free()
{
	__super::Free();


}
