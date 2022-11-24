#pragma once
#include "Base.h"

BEGIN(Engine)

// imgui�ڵ带 �ۼ��ϰ� CImgui_Manager�� �����ϱ� ���� �θ�Ŭ����
class ENGINE_DLL CImguiObject abstract : public CBase
{
protected:
	CImguiObject() = default;

public:
	virtual HRESULT Initialize(void* pArg);

	virtual void Imgui_RenderTab(){}
	virtual void Imgui_RenderWindow(){}

	const char* GetTabName() const { return m_szTabname; }
	const char* GetWindowName() const { return m_szWindowName; }
	virtual void Free() override {}

protected:
	const char* m_szTabname = "";
	const char* m_szWindowName = "";
};

END