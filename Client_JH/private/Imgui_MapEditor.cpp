#include "stdafx.h"
#include "..\public\Imgui_MapEditor.h"


CImgui_MapEditor::CImgui_MapEditor(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
}

HRESULT CImgui_MapEditor::Initialize(void* pArg)
{
	return S_OK;
}

void CImgui_MapEditor::Imgui_RenderWindow()
{
	CImguiObject::Imgui_RenderWindow();
}

CImgui_MapEditor* CImgui_MapEditor::Create(void* pArg)
{
	return nullptr;
}

void CImgui_MapEditor::Free()
{
	CImguiObject::Free();
}
