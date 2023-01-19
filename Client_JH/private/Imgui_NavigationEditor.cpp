#include "stdafx.h"
#include "..\public\Imgui_NavigationEditor.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "GameUtils.h"
#include "Json/json.hpp"
#include <fstream>
#include "Cell.h"
#include "Home.h"
#include "Player.h"
#include "Static_Camera.h"

CImgui_NavigationEditor::CImgui_NavigationEditor(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: m_pDevice(pDevice), m_pContext(pContext)
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
}

HRESULT CImgui_NavigationEditor::Initialize(void* pArg)
{
	m_pGameInstance = CGameInstance::GetInstance();
	m_szWindowName = "Navigation Editor";

	m_pNavigationCom = CNavigation::Create(m_pDevice, m_pContext);

	return S_OK;
}

void CImgui_NavigationEditor::Imgui_RenderWindow()
{
	char**			ppCellTag = nullptr;
	static _bool	bSave = false;
	static char	szSaveFileName[MAX_PATH] = "";

	NULL_CHECK_RETURN(m_pNavigationCom, );

#ifdef _DEBUG
	for (_uint i = 0; i < (_uint)POINT_END; ++i)
	{
		if (m_pVIBufferCircleCom[i] != nullptr)
			Safe_Release(m_pVIBufferCircleCom[i]);

		if (!XMVector3Equal(XMVectorSetW(XMLoadFloat3(&m_vPoint[i]), 1.f), XMVectorSet(0.f, 0.f, 0.f, 1.f)))
			m_pVIBufferCircleCom[i] = CVIBuffer_Cell_Circle::Create(m_pDevice, m_pContext, m_vPoint[i]);
	}
#endif // _DEBUG

	m_iNumCell = m_pNavigationCom->Get_CellCount();
	ImGui::BulletText("Cell Count : %d", m_iNumCell);
	ImGui::BulletText("Cell List");

	if (m_iNumCell == 0)
		ImGui::Text("There is No Cell.");
	else
	{
		ppCellTag = new char*[m_iNumCell];
		for (_uint i = 0; i < m_iNumCell; ++i)
		{
			char		pCellTag[128] = "";
			sprintf_s(pCellTag, "Cell_%d", i);
			ppCellTag[i] = new char[strlen(pCellTag) + 1];
			strcpy_s(ppCellTag[i], strlen(pCellTag) + 1, pCellTag);
		}

		ImGui::ListBox("Cell List", &m_iSelectedCell, ppCellTag, m_iNumCell);

		if ((ImGui::Button("Delete") || m_pGameInstance->Key_Down(DIK_DELETE)) && m_iSelectedCell != -1)
		{
			for (_uint i = 0; i < m_iNumCell; ++i)
				Safe_Delete_Array(ppCellTag[i]);
			Safe_Delete_Array(ppCellTag);

			m_pNavigationCom->Delete_Cell(m_iSelectedCell);

			m_iSelectedCell = -1;

			return;
		}
	}

	ImGui::SameLine();
	if (ImGui::Button("Save") && m_iNumCell != 0)
		bSave = true;
	ImGui::SameLine();
	if (ImGui::Button("Load"))
		ImGuiFileDialog::Instance()->OpenDialog("Load Navigation", "Load Navigation", ".json", "../Bin/Save Data/Navigation", ".", 1, nullptr, ImGuiFileDialogFlags_Modal);

	if (bSave)
	{
		ImGui::InputText("File Name", szSaveFileName, MAX_PATH);
		ImGui::SameLine();
		if (ImGui::Button("Confirm"))
			ImGuiFileDialog::Instance()->OpenDialog("Save Navigation", "Save Navigation", ".json", "../Bin/Save Data/Navigation", strcat(szSaveFileName, ".json"), 0, nullptr, ImGuiFileDialogFlags_Modal);

		if (ImGuiFileDialog::Instance()->Display("Save Navigation"))
		{
			if (ImGuiFileDialog::Instance()->IsOk())
			{
				string		strSaveFileDirectory = ImGuiFileDialog::Instance()->GetCurrentPath();
				char	szDash[128] = "\\";
				strcat_s(szDash, szSaveFileName);
				strSaveFileDirectory += string(szDash);

				m_pNavigationCom->Save_Cell(strSaveFileDirectory);

				strcpy_s(szSaveFileName, "");
				bSave = false;

				ImGuiFileDialog::Instance()->Close();
			}
			if (!ImGuiFileDialog::Instance()->IsOk())
				ImGuiFileDialog::Instance()->Close();
		}
	}

	if (ImGuiFileDialog::Instance()->Display("Load Navigation"))
	{
		if (ImGuiFileDialog::Instance()->IsOk())
		{
			for (_uint i = 0; i < m_iNumCell; ++i)
				Safe_Delete_Array(ppCellTag[i]);
			Safe_Delete_Array(ppCellTag);

			Safe_Release(m_pNavigationCom);

			string		strFilePath = ImGuiFileDialog::Instance()->GetFilePathName();;
			wstring	wstrFilePath = L"";
			wstrFilePath.assign(strFilePath.begin(), strFilePath.end());

			_splitpath_s(strFilePath.c_str(), nullptr, 0, nullptr, 0, szSaveFileName, MAX_PATH, nullptr, 0);

			m_pNavigationCom = CNavigation::Create(m_pDevice, m_pContext, wstrFilePath);

			m_iSelectedCell = -1;

			ImGuiFileDialog::Instance()->Close();

			return;
		}
		if (!ImGuiFileDialog::Instance()->IsOk())
			ImGuiFileDialog::Instance()->Close();
	}

	ImGui::Separator();
	static _bool	bDrawCell = false;
	ImGui::Checkbox("Draw Cell", &bDrawCell);

	if (m_pGameInstance->Get_CurLevelIndex() != LEVEL_GAMEPLAY)
		bDrawCell = false;

	if (bDrawCell && m_pGameInstance->Get_CurLevelIndex() == LEVEL_GAMEPLAY)
	{
		// Mesh Picking
		if (CGameInstance::GetInstance()->Get_DIKeyState(DIK_SPACE))
		{
			list<CGameObject*>*	MeshList = m_pGameInstance->Get_CloneObjectList(LEVEL_GAMEPLAY, L"Layer_BackGround");

			for (auto& pMesh : *MeshList)
			{
				pair<_bool, _float3>		PickInfo = pMesh->Picking_Mesh();

				if (PickInfo.first == true)
				{
					m_vPoint[m_iCurPoint] = PickInfo.second;
					m_pNavigationCom->Find_NearBy_Point(m_vPoint[m_iCurPoint]);
					m_iCurPoint++;

					if (m_iCurPoint == (_uint)POINT_END)
					{
						// CGameUtils::SortPointsByCW(m_vPoint);
						Cell_Sorting();
						m_pNavigationCom->Add_Cell(m_vPoint);
						

						m_iCurPoint = (_uint)POINT_A;
						ZeroMemory(m_vPoint, sizeof(_float3) * POINT_END);
					}
				}
			}
		}

		// else if (m_pGameInstance->Mouse_Down(DIM_LB))
		// {
		// 	CNavigation* pNavigation = dynamic_cast<CNavigation*>(m_pGameInstance->Get_ComponentPtr(LEVEL_GAMEPLAY, L"Layer_BackGround", L"Com_Navigation"));
		// 	vector<CCell*>* vecCell = pNavigation->Get_vecCell();
		// 	CHome*			pHome = dynamic_cast<CHome*>(m_pGameInstance->Get_CloneObjectList(LEVEL_GAMEPLAY, L"Layer_BackGround")->front());
		//
		// 	D3D11_VIEWPORT		ViewPort;
		// 	ZeroMemory(&ViewPort, sizeof(D3D11_VIEWPORT));
		// 	_uint	iNumViewport = 1;
		//
		// 	m_pContext->RSGetViewports(&iNumViewport, &ViewPort);
		//
		// 	pair<_bool, _int> tmp =  CGameUtils::Picking_Cell(g_hWnd, ViewPort.Width, ViewPort.Height, pHome->Get_Transform(), pNavigation);
		//
		// 	m_iSelectedCell = m_iCellIndex = tmp.second;
		// 	
		// }


		// Undo
		else if (m_pGameInstance->Key_Down(DIK_Z) && m_pGameInstance->Get_DIKeyState(DIK_LCONTROL) & 0x80)
		{
			if (m_iCurPoint == (_uint)POINT_A)
			{
				CCell*		pCell = m_pNavigationCom->Get_Cell(m_iNumCell - 1);
				NULL_CHECK_RETURN(pCell, );

				m_vPoint[POINT_A] = pCell->Get_Point(CCell::POINT_A);
				m_vPoint[POINT_B] = pCell->Get_Point(CCell::POINT_B);

				m_pNavigationCom->Delete_Cell(m_iNumCell - 1);

				m_iCurPoint = (_uint)POINT_C;
			}
			else
				m_iCurPoint--;

			m_vPoint[m_iCurPoint] = { 0.f, 0.f, 0.f };
		}

		ImGui::InputFloat3("POINT_A", &m_vPoint[POINT_A].x);
		ImGui::InputFloat3("POINT_B", &m_vPoint[POINT_B].x);
		ImGui::InputFloat3("POINT_C", &m_vPoint[POINT_C].x);
		ImGui::InputInt("PickingCell", &m_iCellIndex);

	}

	

	if (m_iSelectedCell != -1)
	{
		ImGui::Separator();

		CCell*		pCell = m_pNavigationCom->Get_Cell(m_iSelectedCell);
		NULL_CHECK_RETURN(pCell, );

		pCell->ImGui_RenderProperty();
	}

	for (_uint i = 0; i < m_iNumCell; ++i)
		Safe_Delete_Array(ppCellTag[i]);
	Safe_Delete_Array(ppCellTag);
}

void CImgui_NavigationEditor::Render()
{
#ifdef _DEBUG
	m_pNavigationCom->Render();

	if (m_iSelectedCell != -1)
		m_pNavigationCom->Render_Selected_Cell(m_iSelectedCell);

	for (_uint i = 0; i < (_uint)POINT_END; ++i)
	{

		if (m_pVIBufferCircleCom[i] != nullptr)
		{
			m_pVIBufferCircleCom[i]->Render();
		}
	}


#endif // _DEBUG
}

void CImgui_NavigationEditor::Cell_Sorting()
{
	_vector vA = m_vPoint[0];
	_vector vB = m_vPoint[1];
	_vector vC = m_vPoint[2];

	_vector vAB = vB - vA;
	_vector vBC = vC - vB;

	_vector vCross = XMVector3Cross(vAB, vBC);

	assert(XMVectorGetY(vCross) != 0.f);

	if (XMVectorGetY(vCross) < 0.f)
	{
		_float3 tmp = m_vPoint[1];
		m_vPoint[1] = m_vPoint[2];
		m_vPoint[2] = tmp;
	}
}

CImgui_NavigationEditor* CImgui_NavigationEditor::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext,
                                                         void* pArg)
{
	CImgui_NavigationEditor*	pInstance = new CImgui_NavigationEditor(pDevice, pContext);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Create : CImgui_NavigationEditor");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CImgui_NavigationEditor::Free()
{
#ifdef _DEBUG
	for (_uint i = 0; i < (_uint)POINT_END; ++i)
		Safe_Release(m_pVIBufferCircleCom[i]);
#endif // _DEBUG

	Safe_Release(m_pNavigationCom);
	Safe_Release(m_pContext);
	Safe_Release(m_pDevice);
}
