#include "stdafx.h"
#include "Imgui_Setting.h"
#include "GameInstance.h"
#include "Camera.h"

HRESULT CImgui_Setting::Initialize(void * pArg)
{
	m_szWindowName = " [ ImGui Setting ] ";
	m_pAlpha = &ImGui::GetStyle().Colors[ImGuiCol_WindowBg].w;
	
	return S_OK;
}

void CImgui_Setting::Imgui_RenderWindow()
{
	IMGUI_LEFT_LABEL(ImGui::SliderFloat, "Alpha", m_pAlpha, 0.f, 1.f);
	ImGui::Text("(CTRL+Click to Input Directly)");

	_uint		iCurrentLevel = CGameInstance::GetInstance()->Get_CurLevelIndex();

	if (iCurrentLevel == 2)
	{
		_bool&		DynamicCamera = dynamic_cast<CCamera*>(CGameInstance::GetInstance()->Get_CloneObjectList(iCurrentLevel, L"Layer_ZCamera")->front())->Get_RenderState();
		_bool&		StaticCamera = dynamic_cast<CCamera*>(CGameInstance::GetInstance()->Get_CloneObjectList(iCurrentLevel, L"Layer_ZCamera")->back())->Get_RenderState();
		_bool		bSelect = false;


		if (ImGui::RadioButton("Editor Cam", bSelect))
		{
			bSelect = !bSelect;
			DynamicCamera = true;
			StaticCamera = false;
		}
		ImGui::SameLine();
		if(ImGui::RadioButton("FPS Cam", !bSelect))
		{
			bSelect = !bSelect;
			DynamicCamera = false;
			StaticCamera = true;
		}
	}
}

CImgui_Setting * CImgui_Setting::Create(void * pArg)
{
	CImgui_Setting*		pInstance = new CImgui_Setting();

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Create : CTool_Settings");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CImgui_Setting::Free()
{
}