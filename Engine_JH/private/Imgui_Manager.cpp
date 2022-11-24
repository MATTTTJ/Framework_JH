#include "..\public\Imgui_Manager.h"
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
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;		// Enable Gamepad Controls
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
}


void CImgui_Manager::Tick_Imgui()
{
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
}

void CImgui_Manager::Render_Imgui()
{
	RenderTab();
	RenderWindow();

	ImGui::Render();
}

void CImgui_Manager::Render_Update_ImGui()
{
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

	if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
	}
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
