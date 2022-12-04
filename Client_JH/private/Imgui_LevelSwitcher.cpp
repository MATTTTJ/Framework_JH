#include "stdafx.h"
#include "Imgui_LevelSwitcher.h"
#include "GameInstance.h"
#include "Level_Loading.h"
CImgui_LevelSwitcher::CImgui_LevelSwitcher(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: m_pDevice(pDevice)
	, m_pContext(pContext)
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
}

HRESULT CImgui_LevelSwitcher::Initialize(void* pArg)
{
	m_szWindowName = "LevelSwitcher";

	m_mapLevels.emplace("Level_Gameplay", LEVEL_GAMEPLAY);
	m_mapLevels.emplace("Level_Logo", LEVEL_LOGO);
	m_mapLevels.emplace("Level_MapEditor", LEVEL_MAPEDITOR);

	return S_OK;
}

void CImgui_LevelSwitcher::Imgui_RenderWindow()
{
	m_iCurrentLevel = CGameInstance::GetInstance()->Get_CurLevelIndex();

	static int iCurSceneTagIdx = 0;
	static int iLevelIdx = 0;
	// ImGui::ShowDemoWindow();

	ImGui::BulletText("Level Switcher");
	ImGui::BulletText("Current Level  [ %s ]", m_pLevelName[m_iCurrentLevel]);
	if(ImGui::BeginListBox("##"))
	{
		_int iIdx = 0;
		const bool bSelected = iCurSceneTagIdx == iIdx;

		for (auto& pLevel : m_mapLevels)
		{
			if(ImGui::Selectable(pLevel.first.c_str(), bSelected))
			{
				iLevelIdx = pLevel.second;

				if(pLevel.second >= LEVEL_END)
					return;
				if (0 == iLevelIdx)
					break;
				if (1 == iLevelIdx)
					m_Level = LEVEL_LOGO;
				if (2 == iLevelIdx)
					m_Level = LEVEL_GAMEPLAY;
				if (3 == iLevelIdx)
					m_Level = LEVEL_MAPEDITOR;
			}

			if (bSelected)
				ImGui::SetItemDefaultFocus();
			++iIdx;
		}
		ImGui::EndListBox();
	}

	ImGui::SameLine();

	if(ImGui::Button("Change"))
	{
		CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

		FAILED_CHECK_RETURN(pGameInstance->Open_Level(LEVEL_LOADING, CLevel_Loading::Create(m_pDevice, m_pContext, m_Level)), );

		RELEASE_INSTANCE(CGameInstance);

	}
}

CImgui_LevelSwitcher* CImgui_LevelSwitcher::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, void* pArg)
{
	CImgui_LevelSwitcher*		pInstance = new CImgui_LevelSwitcher(pDevice, pContext);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Create : CImgui_LevelSwitcher");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CImgui_LevelSwitcher::Free()
{
	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);

	// for (auto& Pair : m_mapLevels)
	// 	Safe_Release(Pair.second);
	//
	// m_mapLevels.clear();
}
