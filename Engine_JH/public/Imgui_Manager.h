#pragma once
#include "Base.h"

BEGIN(Engine)
class CImguiObject;

class CImgui_Manager final : public CBase
{
	DECLARE_SINGLETON(CImgui_Manager)

private:
	CImgui_Manager();
	virtual ~CImgui_Manager() = default;

public:
	void Ready_Imgui(HWND hWnd, ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut);
	void Tick_Imgui();
	void Render_Imgui();
	void Render_Update_ImGui();

	// imgui object를 tab에 추가한다.
	void Add_ImguiTabObject(CImguiObject* ImguiObject);

	// imgui object를 새로운 window로 추가한다.
	void Add_ImguiWindowObject(CImguiObject* ImguiObject);

	// 현재 사용중인 imgui object를 모두 삭제한다.
	void Clear_ImguiObjects();

private:
	void RenderTab();
	void RenderWindow();

private:
	ID3D11Device*			m_pDevice = nullptr;
	ID3D11DeviceContext*	m_pDeviceContext = nullptr;

	vector<CImguiObject*> m_vecTab;
	vector<CImguiObject*> m_vecWin;

public:
	virtual void Free() override;
};

END