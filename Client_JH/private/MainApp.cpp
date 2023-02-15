#include "stdafx.h"
#include "..\public\MainApp.h"

#include "BackGround.h"
#include "GameInstance.h"
#include "Level_Loading.h"
#include "Dynamic_Camera.h"
#include "Static_Camera.h"
#include "DamageFont_Mgr.h"
#include "Loading_Fire.h"

CMainApp::CMainApp()
	: m_pGameInstance(CGameInstance::GetInstance())
{
	Safe_AddRef(m_pGameInstance);
}

HRESULT CMainApp::Initialize()
{
	NULL_CHECK_RETURN(m_pGameInstance, E_FAIL);


	/* 게임엔진 초기화 */
	GRAPHIC_DESC			GraphicDesc;
	ZeroMemory(&GraphicDesc, sizeof(GRAPHIC_DESC));
	GraphicDesc.hInst = g_hInst;
	GraphicDesc.hWnd = g_hWnd;
	GraphicDesc.iViewportSizeX = g_iWinSizeX;
	GraphicDesc.iViewportSizeY = g_iWinSizeY;
	GraphicDesc.eWindowMode = GRAPHIC_DESC::WINMODE_WIN;
	FAILED_CHECK_RETURN(m_pGameInstance->Initialize_Engine(LEVEL_END, GraphicDesc, &m_pDevice, &m_pContext), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Prototype_Component(), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Prototype_GameObject(), E_FAIL);
	FAILED_CHECK_RETURN(Start_Level(LEVEL_LOGO), E_FAIL);

	FAILED_CHECK_RETURN(m_pGameInstance->Add_Font(m_pDevice, m_pContext, TEXT("Font_Comic"), TEXT("../Bin/Resources/fonts/131.SpriteFont")), E_FAIL);
	// D3D11_BLEND_DESC
	return S_OK;
}
void CMainApp::Tick(_double TimeDelta)
{
	NULL_CHECK(m_pGameInstance)

	srand((unsigned)time(NULL));

#ifdef  _DEBUG
		// m_TimeAcc += TimeDelta;
#endif


	m_pGameInstance->Tick_Engine(TimeDelta);

	ShowCursor(false);

}

HRESULT CMainApp::Render()
{
	if (nullptr == m_pGameInstance ||	nullptr == m_pRenderer)
		return E_FAIL;

	m_pGameInstance->Render_ImGui();
	m_pGameInstance->Clear_Graphic_Device(&_float4(0.f, 0.f, 0.f, 1.f));

	m_pRenderer->Draw_RenderGroup();

	m_pGameInstance->Render_Update_ImGui();
	m_pGameInstance->Render_Level();

	

#ifdef _DEBUG
	// ++m_iNumCallDraw;
	// if(m_TimeAcc >= 1.f)
	// {
	// 	wsprintf(m_szFPS, TEXT("FPS : %d"), m_iNumCallDraw);
	//
	// 	m_iNumCallDraw = 0;
	// 	m_TimeAcc = 0;
	// }

	// m_pGameInstance->Render_Font(TEXT("Font_Comic"), m_szFPS, _float2(100.f, 0.f), 0.f, _float2(1.f, 1.f), XMVectorSet(1.f, 0.f, 0.f, 1.f));

#endif
	m_pGameInstance->Present();


	return S_OK;
}

HRESULT CMainApp::Resize_BackBuffer()
{
	GRAPHIC_DESC	tGraphicDesc;
	ZeroMemory(&tGraphicDesc, sizeof(GRAPHIC_DESC));

	tGraphicDesc.hWnd = g_hWnd;
	if (!g_bFullScreen)
	{
		tGraphicDesc.iViewportSizeX = g_iWinSizeX;
		tGraphicDesc.iViewportSizeY = g_iWinSizeY;
	}
	else
	{
		tGraphicDesc.iViewportSizeX = GetSystemMetrics(SM_CXSCREEN);
		tGraphicDesc.iViewportSizeY = GetSystemMetrics(SM_CYSCREEN);
	}

	if (FAILED(m_pGameInstance->Update_SwapChain(tGraphicDesc.hWnd, tGraphicDesc.iViewportSizeX, tGraphicDesc.iViewportSizeY, g_bFullScreen, g_bNeedResizeSwapChain)))
		return E_FAIL;

	return S_OK;
}

HRESULT CMainApp::Start_Level(LEVEL eLevelID)
{
	if (LEVEL_LOADING == eLevelID || nullptr == m_pGameInstance)
		return E_FAIL;

	FAILED_CHECK_RETURN(m_pGameInstance->Open_Level(LEVEL_LOADING, CLevel_Loading::Create(m_pDevice, m_pContext, eLevelID)), E_FAIL);

	return S_OK;
}

HRESULT CMainApp::Ready_Prototype_Component()
{
	NULL_CHECK_RETURN(m_pGameInstance, E_FAIL);

	/* For.Prototype_Component_Renderer */
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(CGameInstance::Get_StaticLevelIndex(), L"Prototype_Component_Renderer", m_pRenderer = CRenderer::Create(m_pDevice, m_pContext)), E_FAIL);
	Safe_AddRef(m_pRenderer);

	/* For.Prototype_Component_VIBuffer_Rect */
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(CGameInstance::Get_StaticLevelIndex(), L"Prototype_Component_VIBuffer_Rect", CVIBuffer_Rect::Create(m_pDevice, m_pContext)), E_FAIL);

	/* For.Prototype_Component_Shader_VtxTex */
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(CGameInstance::Get_StaticLevelIndex(), L"Prototype_Component_Shader_VtxTex",	CShader::Create(m_pDevice, m_pContext, L"../Bin/ShaderFiles/Shader_VtxTex.hlsl",CShader::DECLARATION_VTXNORTEX, VTXTEX_DECLARATION::Elements, VTXTEX_DECLARATION::iNumElements)), E_FAIL);
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(CGameInstance::Get_StaticLevelIndex(), L"Prototype_Component_Texture_Logo", CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/2DTexture/Loading/Logo_%d.png", 8)), E_FAIL);
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(CGameInstance::Get_StaticLevelIndex(), L"Prototype_Component_Texture_Logo_Fire", CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/2DTexture/Loading_Fire/img_logofire_%d.png", 30)), E_FAIL);
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(CGameInstance::Get_StaticLevelIndex(), L"Prototype_Component_State", CState::Create(m_pDevice, m_pContext)), E_FAIL);





	return S_OK;
}

HRESULT CMainApp::Ready_Prototype_GameObject()
{
	NULL_CHECK_RETURN(m_pGameInstance, E_FAIL);

	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(L"Prototype_GameObject_Camera_Dynamic", CDynamic_Camera::Create(m_pDevice, m_pContext)), E_FAIL);
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(L"Prototype_GameObject_Camera_Static", CStatic_Camera::Create(m_pDevice, m_pContext)), E_FAIL);

	CBackGround* pBack = nullptr;
	if(	FAILED(m_pGameInstance->Add_Prototype(L"Prototype_GameObject_BackGround", pBack = CBackGround::Create(m_pDevice, m_pContext))))
	{
		Safe_Release(pBack);
	}
	CLoading_Fire* pFire = nullptr;

	if (FAILED(m_pGameInstance->Add_Prototype(L"Prototype_GameObject_Loading_Fire", pFire = CLoading_Fire::Create(m_pDevice, m_pContext))))
	{
		Safe_Release(pFire);
	}
	// FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(L"Prototype_GameObject_Loading_Fire", CLoading_Fire::Create(m_pDevice, m_pContext)), E_FAIL);


	return S_OK;
}

CMainApp * CMainApp::Create()
{
	CMainApp*		pInstance = new CMainApp;

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created : CMainApp");
		Safe_Release<CMainApp*>(pInstance);
	}
	return pInstance;
}

void CMainApp::Free()
{
	// m_pGameInstance->Clear_ImguiObjects();

	Safe_Release(m_pRenderer);
	Safe_Release(m_pGameInstance);
	Safe_Release(m_pContext);
	Safe_Release(m_pDevice);

	CGameInstance::Release_Engine();
}
