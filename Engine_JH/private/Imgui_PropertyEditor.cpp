#include "stdafx.h"
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

	// todo: ���콺 ��ŷ���� ������Ʈ �����ϴ� ��� �߰�
	// todo: ���� �纻 object�� ������ �̸��� ���� ������ �ϴ� class�̸����� �س���. ���̵�� �ִ»���� �߰��ϱ�~

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
	CImgui_PropertyEditor* pInst = new CImgui_PropertyEditor;
	if (FAILED(pInst->Initialize(pArg)))
	{
		MSG_BOX("Failed to Create : CImgui_PropertyEditor");
		Safe_Release(pInst);
	}

	return pInst;
}
