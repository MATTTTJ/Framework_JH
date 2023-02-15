#include "stdafx.h"
#include "..\public\Level_Logo.h"

#include "BackGround.h"
#include "Shader.h"
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
	// FAILED_CHECK_RETURN(Ready_Layer_Light(), E_FAIL);
	// // FAILED_CHECK_RETURN(Ready_Layer_Env(L"Layer_Env"), E_FAIL);
	// FAILED_CHECK_RETURN(Ready_Layer_Lai_Home(L"Layer_Player"), E_FAIL);
	CGameInstance::GetInstance()->Stop_All_Sound();

	FAILED_CHECK_RETURN(Ready_Layer_BackGround(L"Layer_BackGround"), E_FAIL);
	// FAILED_CHECK_RETURN(Ready_Layer_Camera(L"Layer_ZCamera"), E_FAIL);


	// CGameInstance::GetInstance()->Clear_ImguiObjects();
	// CGameInstance::GetInstance()->Add_ImguiWindowObject(CImgui_Setting::Create());
	CGameInstance::GetInstance()->Add_ImguiWindowObject(CImgui_LevelSwitcher::Create(m_pDevice, m_pContext));
	// CGameInstance::GetInstance()->Add_ImguiWindowObject(CImgui_ProtoMgr::Create(m_pDevice, m_pContext));
	// CGameInstance::GetInstance()->Add_ImguiWindowObject(CImgui_MapEditor::Create());
	// CGameInstance::GetInstance()->Add_ImguiWindowObject(CImgui_AnimationMgr::Create());
	// CGameInstance::GetInstance()->Add_ImguiWindowObject(CImgui_ModelSave::Create(m_pDevice, m_pContext));
	// CGameInstance::GetInstance()->Add_ImguiWindowObject(CImgui_NavigationEditor::Create(m_pDevice, m_pContext));
	CGameInstance::GetInstance()->Stop_All_Sound();

	m_fLogoTime = 0.f;
	// CGameInstance::GetInstance()->Play_Sound(L"Ready_Logo.mp3", 0.5f, true, false, 1);

	return S_OK;
}

void CLevel_Logo::Tick(_double TimeDelta)
{
	__super::Tick(TimeDelta);

	if (m_bLevelChange == false)
		m_fCurLogoTime += (_float)TimeDelta;



	// if(CGameInstance::GetInstance()->Key_Down(DIK_F5))
	// {
	// 	m_pShader->ReCompileShader();
	// }
}

void CLevel_Logo::Late_Tick(_double TimeDelta)
{
	__super::Late_Tick(TimeDelta);

	if (m_fCurLogoTime > m_fLogoTime && m_bLevelChange == false)
	{
		CGameInstance*		pGameInstance = CGameInstance::GetInstance();
		Safe_AddRef(pGameInstance);

		_uint Size = (_uint)m_vecLogo.size();

		for (_uint i = 0; i<Size; ++i)
		{
			m_vecLogo[i]->Set_Dead(true);
		}
		m_vecLogo.clear();

		if (pGameInstance->Key_Down(DIK_SPACE))
		{
			FAILED_CHECK_RETURN(pGameInstance->Open_Level(LEVEL_LOADING, CLevel_Loading::Create(m_pDevice, m_pContext, LEVEL_GAMEPLAY)), );
		}


		Safe_Release(pGameInstance);

		

		m_bLevelChange = true;
	}

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
	CGameObject* pGameObject = nullptr;
	pGameObject = pGameInstance->Clone_GameObjectReturnPtr(pGameInstance->Get_StaticLevelIndex(), wstrLayerTag, L"Prototype_GameObject_BackGround");
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	m_vecLogo.push_back(pGameObject);
	CGameObject::GAMEOBJECTDESC LogoDesc;
	ZeroMemory(&LogoDesc, sizeof(CGameObject::GAMEOBJECTDESC));
	LogoDesc.TransformDesc.vInitPos = _float3(1080.f, -480.f, 0.f);
	pGameObject = pGameInstance->Clone_GameObjectReturnPtr(pGameInstance->Get_StaticLevelIndex(), wstrLayerTag, L"Prototype_GameObject_Loading_Fire", &LogoDesc);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	m_vecLogo.push_back(pGameObject);

	return S_OK;
}

HRESULT CLevel_Logo::SetUp_Component()
{
	// m_pShader = dynamic_cast<CShader*>(CGameInstance::GetInstance()->Clone_Component(CGameInstance::Get_StaticLevelIndex(), L"Prototype_Component_Shader_VtxTex", nullptr));
	// NULL_CHECK_RETURN(m_pShader, E_FAIL);

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
