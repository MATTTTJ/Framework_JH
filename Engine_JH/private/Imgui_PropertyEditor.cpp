#include "..\public\Imgui_PropertyEditor.h"

#include "GameObject.h"
#include "Object_Manager.h"
#include "Level_Manager.h"

HRESULT CImgui_PropertyEditor::Initialize(void* pArg)
{
	m_szTabname = "PropertyEditor";
	return S_OK;
}

void CImgui_PropertyEditor::Imgui_RenderTab()
{
	CObject_Manager::GetInstance()->Imgui_ObjectViewer(
		CLevel_Manager::GetInstance()->GetCurLevelIdx(), 
		m_pSelectedObject);

	// todo: 마우스 피킹으로 오브젝트 선택하는 기능 추가
	// todo: 현재 사본 object를 구분할 이름이 없기 때문에 일단 class이름으로 해놓음. 아이디어 있는사람이 추가하기~

	if (m_pSelectedObject)
	{
		ImGui::Separator();
		ImGui::Text("%s", typeid(*m_pSelectedObject).name());
		m_pSelectedObject->Imgui_RenderProperty();
		m_pSelectedObject->Imgui_RenderComponentProperties();
	}
}

void CImgui_PropertyEditor::Imgui_RenderWindow()
{
	assert("dont use as window");
}

CImgui_PropertyEditor* CImgui_PropertyEditor::Create(void* pArg)
{
	auto inst = new CImgui_PropertyEditor;
	if (FAILED(inst->Initialize(pArg)))
	{
		return nullptr;
	}

	return inst;
}
