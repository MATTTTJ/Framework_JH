#pragma once
#include "ImguiObject.h"

BEGIN(Engine)

// �纻 object�� �����ϰ�, ������ object�� ������ Ȯ���ϰ� �����ϴ� ����� ���� imgui object
// ������ object�� component�� ���� Imgui_PropertyRender�Լ��� �����Ѵ�.
class ENGINE_DLL CImgui_PropertyEditor final : public CImguiObject
{
private:
	CImgui_PropertyEditor() = default;

public:
	virtual HRESULT Initialize(void* pArg) override;

	virtual void Imgui_RenderTab() override;
	virtual void Imgui_RenderWindow() override;
	static CImgui_PropertyEditor* Create(void* pArg = nullptr);

private:
	class CGameObject* m_pSelectedObject = nullptr;
};

END