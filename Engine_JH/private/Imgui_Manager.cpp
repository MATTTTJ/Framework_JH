#include "stdafx.h"
#include "..\public\Imgui_Manager.h"

#include "GameInstance.h"
#include "Graphic_Device.h"
#include "ImGuiFileDialog.h"
#include "ImguiObject.h"

IMPLEMENT_SINGLETON(CImgui_Manager)

CImgui_Manager::CImgui_Manager()
{
}

void CImgui_Manager::Ready_Imgui(HWND hWnd, ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContextOut)
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;		// Enable Gamepad Controls
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
	//io.ConfigViewportsNoAutoMerge = true;
	io.ConfigViewportsNoTaskBarIcon = true;
	ImGui::StyleColorsDark();
	
	ImGuiStyle& style = ImGui::GetStyle();
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		style.WindowRounding = 0.0f;
		style.Colors[ImGuiCol_WindowBg].w = 1.0f;
	}

	m_pDevice = pDevice;
	m_pDeviceContext = pDeviceContextOut;
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pDeviceContext);

	ImGui_ImplWin32_Init(hWnd);
	ImGui_ImplDX11_Init(m_pDevice, m_pDeviceContext);

	ImGuiStyle * pstyle = &ImGui::GetStyle();

	pstyle->WindowMinSize = ImVec2(160, 20);
	pstyle->FramePadding = ImVec2(10, 6);
	pstyle->WindowPadding = ImVec2(20, 20);
	
	pstyle->ItemSpacing = ImVec2(10, 6);
	pstyle->ItemInnerSpacing = ImVec2(6, 4);
	pstyle->Alpha = 0.95f;
	pstyle->WindowRounding = 4.0f;
	pstyle->FrameRounding = 2.0f;
	pstyle->IndentSpacing = 6.0f;
	pstyle->ItemInnerSpacing = ImVec2(10, 4);
	pstyle->ColumnsMinSpacing = 50.0f;
	pstyle->GrabMinSize = 14.0f;
	pstyle->GrabRounding = 16.0f;
	pstyle->ScrollbarSize = 12.0f;
	pstyle->ScrollbarRounding = 16.0f;
	pstyle->TabBorderSize = 1.f;
	pstyle->FrameBorderSize = 1.f;

	// pstyle->WindowPadding = ImVec2(15, 15);
	// pstyle->WindowRounding = 5.0f;
	// pstyle->FramePadding = ImVec2(5, 5);
	// pstyle->FrameRounding = 4.0f;
	// pstyle->ItemSpacing = ImVec2(12, 8);
	// pstyle->ItemInnerSpacing = ImVec2(8, 6);
	// pstyle->IndentSpacing = 25.0f;
	// pstyle->ScrollbarSize = 15.0f;
	// pstyle->ScrollbarRounding = 9.0f;
	// pstyle->GrabMinSize = 5.0f;
	// pstyle->GrabRounding = 3.0f;
	// pstyle->WindowRounding = 4.0f;
	// pstyle->FrameRounding = 2.0f;

	pstyle->Colors[ImGuiCol_Text] = ImVec4(0.80f, 0.80f, 0.83f, 1.00f);
	pstyle->Colors[ImGuiCol_TextDisabled] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
	pstyle->Colors[ImGuiCol_WindowBg] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
	pstyle->Colors[ImGuiCol_ChildBg] = ImVec4(0.07f, 0.07f, 0.09f, 1.00f);
	pstyle->Colors[ImGuiCol_PopupBg] = ImVec4(0.07f, 0.07f, 0.09f, 1.00f);
	pstyle->Colors[ImGuiCol_Border] = ImVec4(0.37f, 0.28f, 0.20f, 0.88f);
	pstyle->Colors[ImGuiCol_Tab] = ImVec4(0.30f, 0.30f, 0.33f, 1.f);
	pstyle->Colors[ImGuiCol_TabHovered] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
	pstyle->Colors[ImGuiCol_TabActive] = ImVec4(0.36f, 0.36f, 0.38f, 1.00f);
	pstyle->Colors[ImGuiCol_TabUnfocused] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
	pstyle->Colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.46f, 0.46f, 0.48f, 1.00f);
	pstyle->Alpha = 1.f;

	pstyle->Colors[ImGuiCol_ResizeGrip] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	pstyle->Colors[ImGuiCol_BorderShadow] = ImVec4(0.92f, 0.91f, 0.88f, 0.00f);
	pstyle->Colors[ImGuiCol_FrameBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
	pstyle->Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
	pstyle->Colors[ImGuiCol_FrameBgActive] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	pstyle->Colors[ImGuiCol_TitleBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
	pstyle->Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(1.00f, 0.98f, 0.95f, 0.75f);
	pstyle->Colors[ImGuiCol_TitleBgActive] = ImVec4(0.07f, 0.07f, 0.09f, 1.00f);
	pstyle->Colors[ImGuiCol_MenuBarBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
	pstyle->Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
	pstyle->Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
	pstyle->Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	pstyle->Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
	pstyle->Colors[ImGuiCol_CheckMark] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
	pstyle->Colors[ImGuiCol_SliderGrab] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
	pstyle->Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
	pstyle->Colors[ImGuiCol_Button] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
	pstyle->Colors[ImGuiCol_ButtonHovered] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
	pstyle->Colors[ImGuiCol_ButtonActive] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	pstyle->Colors[ImGuiCol_Header] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
	pstyle->Colors[ImGuiCol_HeaderHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	pstyle->Colors[ImGuiCol_HeaderActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
	pstyle->Colors[ImGuiCol_ResizeGrip] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	pstyle->Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	pstyle->Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
	pstyle->Colors[ImGuiCol_PlotLines] = ImVec4(0.40f, 0.39f, 0.38f, 0.63f);
	pstyle->Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.25f, 1.00f, 0.00f, 1.00f);
	pstyle->Colors[ImGuiCol_PlotHistogram] = ImVec4(0.40f, 0.39f, 0.38f, 0.63f);
	pstyle->Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.25f, 1.00f, 0.00f, 1.00f);
	pstyle->Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.25f, 1.00f, 0.00f, 0.43f);
	pstyle->Colors[ImGuiCol_ModalWindowDimBg] = ImVec4(1.00f, 0.98f, 0.95f, 0.73f);
}


void CImgui_Manager::Tick_Imgui()
{

	if (CGameInstance::GetInstance()->Key_Down(DIK_T) && CGameInstance::GetInstance()->Get_DIKeyState(DIK_LCONTROL) & 0x80)
	{
		m_bDrawImGui = !m_bDrawImGui;
	}

	if (!m_bDrawImGui)
	{
		return;
	}


	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	// ImGui::ShowStyleEditor();

	ImGui_DockSpace();

	RenderTab();
	RenderWindow();
	
}

void CImgui_Manager::Render_Imgui()
{
	if (!m_bDrawImGui)
		return;



	ImGui::Render();
}

void CImgui_Manager::Render_Update_ImGui()
{
	if (!m_bDrawImGui)
		return;

	for (const auto& ImWinObj : m_vecWin)
		ImWinObj->Render();

	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

	if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
	}
}

void CImgui_Manager::ImGui_DockSpace()
{
	ImGuiWindowFlags			WindowFlag = ImGuiWindowFlags_NoDocking;

	const ImGuiViewport*	Viewport = ImGui::GetMainViewport();
	ImGui::SetNextWindowPos(Viewport->WorkPos);
	ImGui::SetNextWindowSize(Viewport->WorkSize);
	ImGui::SetNextWindowViewport(Viewport->ID);
	ImGui::SetNextWindowBgAlpha(0.f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.f, 0.f));

	WindowFlag |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
	WindowFlag |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
	WindowFlag |= ImGuiDockNodeFlags_PassthruCentralNode;
	WindowFlag |= ImGuiWindowFlags_NoBackground;

	_bool	bIsShow = true;

	ImGui::Begin("DockSpace", &bIsShow, WindowFlag);
	ImGui::PopStyleVar(1);
	ImGui::PopStyleVar(2);

	ImGuiIO&	io = ImGui::GetIO();
	if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
	{
		ImGuiID	DockSpaceID = ImGui::GetID("DockSpace");
		ImGuiDockNodeFlags Flag = ImGuiDockNodeFlags_PassthruCentralNode;
		ImGui::DockSpace(DockSpaceID, ImVec2(0.f, 0.f), Flag);
	}

	ImGui::End();
}

void CImgui_Manager::Add_ImguiTabObject(CImguiObject* ImguiObject)
{
	if (ImguiObject == nullptr)
		return;

	// 여기서 바로 ImguiObject삭제하기 위해서 add ref 생략
	m_vecTab.push_back(ImguiObject);
}

void CImgui_Manager::Add_ImguiWindowObject(CImguiObject* ImguiObject)
{
	if (ImguiObject == nullptr)
		return;

	// 여기서 바로 ImguiObject삭제하기 위해서 add ref 생략
	m_vecWin.push_back(ImguiObject);
}

void CImgui_Manager::Clear_ImguiObjects()
{
	for (auto& e : m_vecTab)
		Safe_Release(e);
	m_vecTab.clear();

	for (auto& e : m_vecWin)
		Safe_Release(e);
	m_vecWin.clear();
}

void CImgui_Manager::RenderTab()
{
	if (m_vecTab.empty()) 
		return;

	ImGui::Begin("TabWindow");
    if (ImGui::BeginTabBar("Manager_Tab", ImGuiTabBarFlags_None))
    {
		for (const auto& imTabObj : m_vecTab)
		{
	        if (ImGui::BeginTabItem(imTabObj->GetTabName()))
	        {
				imTabObj->Imgui_RenderTab();
	            ImGui::EndTabItem();
	        }
		}
        ImGui::EndTabBar();
    }
	ImGui::End();
}

void CImgui_Manager::RenderWindow()
{
	for (const auto& imWinObj : m_vecWin)
	{
		ImGui::Begin(imWinObj->GetWindowName());
		imWinObj->Imgui_RenderWindow();
		ImGui::End();
	}
}

void CImgui_Manager::Free()
{
	Clear_ImguiObjects();

	Safe_Release(m_pDevice);
	Safe_Release(m_pDeviceContext);
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}
