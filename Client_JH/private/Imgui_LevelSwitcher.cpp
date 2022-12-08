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
	m_szWindowName = " [ Level Switcher ] ";

	return S_OK;
}

void CImgui_LevelSwitcher::Imgui_RenderWindow()
{
	m_iCurrentLevel = CGameInstance::GetInstance()->Get_CurLevelIndex();

	ImGui::Text("Current Level : %s", m_pLevelName[m_iCurrentLevel]);

	ImGui::NewLine();
	ImGui::BulletText("Change Level");
	ImGui::Combo( " ", &m_iNextLevel, m_pLevelName, LEVEL_END);
	
	ImGui::SameLine();
	if (ImGui::Button("Change"))
	{
		if (m_iNextLevel == LEVEL_LOADING || m_iCurrentLevel == m_iNextLevel)
			return;

		FAILED_CHECK_RETURN(CGameInstance::GetInstance()->Open_Level(LEVEL_LOADING, CLevel_Loading::Create(m_pDevice, m_pContext, (LEVEL)m_iNextLevel)), );
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
