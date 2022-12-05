#include "stdafx.h"
#include "Imgui_Setting.h"


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