#include "stdafx.h"
#include "MainApp.h"
#include "GameInstance.h"
#include "Level_Loading.h"

CMainApp::CMainApp()
	:m_pGameInstance(CGameInstance::GetInstance())
{
	Safe_AddRef(m_pGameInstance);
}

HRESULT CMainApp::Initialize()
{
	if (nullptr == m_pGameInstance)
		return E_FAIL;

	/* 게임엔진 초기화 */
	GRAPHIC_DESC			GraphicDesc;
	ZeroMemory(&GraphicDesc, sizeof(GRAPHIC_DESC));

	GraphicDesc.hWnd = g_hWnd;
	GraphicDesc.iViewportSizeX = g_iWinSizeX;
	GraphicDesc.iViewportSizeY = g_iWinSizeY;
	GraphicDesc.eWindowMode = GRAPHIC_DESC::WINMODE_WIN;

	if (FAILED(m_pGameInstance->Initialize_Engine(g_hInst, LEVEL_END, GraphicDesc, &m_pDevice, &m_pContext)))
		return E_FAIL;

	if (FAILED(Ready_Prototype_Component()))
		return E_FAIL;

	if (FAILED(Ready_Prototype_GameObject()))
		return E_FAIL;

	if (FAILED(Start_Level(LEVEL_LOGO)))			// 무엇을 넣든 로딩을 거쳐서 가게 된다. 
		return E_FAIL;

	return S_OK;
}

void CMainApp::Tick(_double TimeDelta)
{
	if (m_pGameInstance == nullptr)
		return;

	m_pGameInstance->Tick_Engine(TimeDelta);
}

HRESULT CMainApp::Render()
{
	if (nullptr == m_pGameInstance ||
		nullptr == m_pRenderer)
		return E_FAIL;

	m_pGameInstance->Clear_Graphic_Device(&_float4(0.0f, 1.f, 1.f, 1.f));

	m_pRenderer->Draw_RenderGroup();

	m_pGameInstance->Render_Level();

	m_pGameInstance->Present();

	return S_OK;
}

HRESULT CMainApp::Start_Level(LEVEL eLevelID)
{
	if (LEVEL_LOADING == eLevelID ||
		m_pGameInstance == nullptr)
		return E_FAIL;

	if (FAILED(m_pGameInstance->Open_Level(LEVEL_LOADING,CLevel_Loading::Create(m_pDevice, m_pContext, eLevelID))))
		return E_FAIL;

	return S_OK;
}

HRESULT CMainApp::Ready_Prototype_Component()
{
	if (nullptr == m_pGameInstance)
		return E_FAIL;
	
	/* For.Prototype_Component_Renderer */
	 if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"),
	 	m_pRenderer = CRenderer::Create(m_pDevice, m_pContext))))
	 	return E_FAIL;
	
	 Safe_AddRef(m_pRenderer);

	return S_OK;
}

HRESULT CMainApp::Ready_Prototype_GameObject()
{
	return S_OK;
}

CMainApp* CMainApp::Create()
{
	CMainApp*		pInstance = new CMainApp;

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created : CMainApp");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CMainApp::Free()
{
	Safe_Release(m_pGameInstance);
	Safe_Release(m_pRenderer);
	Safe_Release(m_pContext);
	Safe_Release(m_pDevice);

	CGameInstance::Release_Engine();
}
