#include "stdafx.h"
#include "..\public\Imgui_ProtoMgr.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "GameUtils.h"
#include "CustomObject.h"
#include "Json/json.hpp"
#include <fstream>
#include "Monster.h"
#include "Player.h"

#define LEVEL_PUBLIC 3

CImgui_ProtoMgr::CImgui_ProtoMgr(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: m_pDevice(pDevice)
	, m_pContext(pContext)
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
}

HRESULT CImgui_ProtoMgr::Initialize(void* pArg)
{
	m_szWindowName = "Prototype Manager";

	m_iCurLevel = CGameInstance::GetInstance()->Get_CurLevelIndex();

	m_mapProtoComponenets = CGameInstance::GetInstance()->Get_PrototypeComponents();
	m_iProtoComponentCnt = (_uint)m_mapProtoComponenets->size();

	m_mapProtoObjects = CGameInstance::GetInstance()->Get_Prototypes();
	m_iProtoObjCnt = (_uint)m_mapProtoObjects->size();

	m_mapLayers = CGameInstance::GetInstance()->Get_Layers(m_iCurLevel);
	m_iLayerCnt = (_uint)m_mapLayers->size();

	return S_OK;
}

void CImgui_ProtoMgr::Imgui_RenderWindow()
{
	m_iCurLevel = CGameInstance::GetInstance()->Get_CurLevelIndex();

	ImGui::BeginTabBar("Prototype Manager");
	Component_Editor();
	GameObject_Editor();
	CloneObject_Editor();
	ImGui::EndTabBar();
}

void CImgui_ProtoMgr::Component_Editor()
{
	static _int	iSelectLevel = LEVEL_PUBLIC;
	static _int	iSelectComponent = -1;
	static _bool	bAddPrototype = false;
	static _bool	bSave = false;
	static _bool	bLoad = false;

	if (ImGui::BeginTabItem("Component"))
	{
		ImGui::BulletText("Prototype Component List");
		IMGUI_LEFT_LABEL(ImGui::Combo, "Select Level", &iSelectLevel, m_pLevelName, LEVEL_PUBLIC + 1);

		char**			ppProtoComponentTag = new char*[m_mapProtoComponenets[iSelectLevel].size()];
		_uint i = 0;
		for (auto Pair : m_mapProtoComponenets[iSelectLevel])
		{
			ppProtoComponentTag[i] = new char[Pair.first.length() + 1];
			CGameUtils::wc2c(Pair.first.c_str(), ppProtoComponentTag[i]);
			++i;
		}

		ImGui::ListBox(" ", &iSelectComponent, ppProtoComponentTag, (_int)m_mapProtoComponenets[iSelectLevel].size());

		if (ImGui::Button("Add"))
		{
			if (bSave)
				bSave = false;
			if (bLoad)
				bLoad = false;
			bAddPrototype = true;
		}
		ImGui::SameLine();
		if (ImGui::Button("Save"))
		{
			if (bAddPrototype)
				bAddPrototype = false;
			if (bLoad)
				bLoad = false;
			bSave = true;
		}
		ImGui::SameLine();
		if (ImGui::Button("Load"))
		{
			if (bAddPrototype)
				bAddPrototype = false;
			if (bSave)
				bSave = false;
			ImGuiFileDialog::Instance()->OpenDialog("Select Json", "Select Json", ".json", "../Bin/Save Data", ".", 1, nullptr, ImGuiFileDialogFlags_Modal);
		}

		if (bAddPrototype)
		{
			ImGui::Separator();

			iSelectComponent = -1;
			static	COMPONENTTYPE	eComponentType = COM_RENDERER;
			static	COMPONENTTYPE	eLastComponentType = eComponentType;
			char*	szTypeName[COMPONENTTYPE_END] = { "Renderer", "VIBuffer", "Shader", "Transform", "Texture", "Model", "Collider" };
			static _uint	iTargetLevel = 3;
			static _uint	iLastTargetLevel = iTargetLevel;

			IMGUI_LEFT_LABEL(ImGui::Combo, "Select Type ", (_int*)&eComponentType, szTypeName, COMPONENTTYPE_END);
			IMGUI_LEFT_LABEL(ImGui::Combo, "Target Level", (_int*)&iTargetLevel, m_pLevelName, LEVEL_END + 1);

			ImGui::NewLine();
			ImGui::BulletText("Options");

			/* For All Component */
			static char	szPrototypeTag[MAX_PATH] = "";
			static char	szFilePath[MAX_PATH] = "";
			const char*	Filter = "Image files (*.png *.dds *.jpg *.jpeg *.gif *.tga){.png,.dds,.jpg,.jpeg,.gif,.tga}";

			/* For Shader */
			static _int	iSelectDeclaration = 0;
			char*			szDeclarationType[5] = { "VTXPOS", "VTXTEX", "VTXNORTEX", "VTXMODEL", "VTXANIMMODEL" };
			static _int	iNumElements = 0;

			/* For Texture */
			static _uint	iTextureCnt = 1;

			/* For Model */
			static CModel::MODELTYPE		eModelType = CModel::MODEL_NONANIM;
			char*			szModelType[2] = { "Non-Animation", "Animation" };

			/* For Collider */
			static CCollider::COLLIDERTYPE	eColliderType = CCollider::COLLIDER_SPHERE;
			char*			szColliderType[3] = { "Sphere", "AABB", "OBB" };

			if (iTargetLevel != iLastTargetLevel || eComponentType != eLastComponentType)
			{
				iLastTargetLevel = iTargetLevel;
				eLastComponentType = eComponentType;

				strcpy_s(szPrototypeTag, "");
				strcpy_s(szFilePath, "");
				iSelectDeclaration = 0;
				iNumElements = 0;
				iTextureCnt = 1;
				eModelType = CModel::MODEL_NONANIM;
			}

			switch (eComponentType)
			{
			case COM_RENDERER:
				ImGui::Text("There is no Options for this Comopnent Type.");
				break;

			case COM_VIBUFFER:
				ImGui::Text("There is no Options for this Comopnent Type.");
				break;

			case COM_SHADER:
				/* Input Prototype Tag */
				IMGUI_LEFT_LABEL(ImGui::InputText, "Component Tag", szPrototypeTag, sizeof(char) * MAX_PATH);

				/* Input File Path */
				IMGUI_LEFT_LABEL(ImGui::InputTextWithHint, "File Path", "Input Path here or Choose from FileDialog.", szFilePath, sizeof(char) * MAX_PATH);
				ImGui::SameLine();
				if (ImGui::SmallButton("Open"))
					ImGuiFileDialog::Instance()->OpenDialog("Choose HLSL", "Choose .hlsl", ".hlsl", "../Bin/ShaderFiles", ".hlsl", 1, nullptr, ImGuiFileDialogFlags_Modal);

				/* Select Declaration Type */
				IMGUI_LEFT_LABEL(ImGui::Combo, "Select Declaration", &iSelectDeclaration, szDeclarationType, 5);

				if (iSelectDeclaration == 0)
					iNumElements = 1;
				else if (iSelectDeclaration == 1)
					iNumElements = 2;
				else if (iSelectDeclaration == 2)
					iNumElements = 3;
				else if (iSelectDeclaration == 3)
					iNumElements = 4;
				else if (iSelectDeclaration == 4)
					iNumElements = 6;

				IMGUI_LEFT_LABEL(ImGui::InputInt, "Elements Count", &iNumElements, 0, 0, ImGuiInputTextFlags_ReadOnly);

				if (ImGuiFileDialog::Instance()->Display("Choose HLSL"))
				{
					if (ImGuiFileDialog::Instance()->IsOk())
					{
						char	szRelativePath[MAX_PATH] = "";
						char	szDirectoryPath[MAX_PATH] = "";
						GetCurrentDirectoryA(MAX_PATH, szDirectoryPath);
						PathRelativePathToA(szRelativePath, szDirectoryPath, FILE_ATTRIBUTE_DIRECTORY, ImGuiFileDialog::Instance()->GetFilePathName().c_str(), FILE_ATTRIBUTE_DIRECTORY);
						strcpy_s(szFilePath, sizeof(char) * MAX_PATH, szRelativePath);

						ImGuiFileDialog::Instance()->Close();
					}
					if (!ImGuiFileDialog::Instance()->IsOk())
						ImGuiFileDialog::Instance()->Close();
				}
				break;

			case COM_TRANSFORM:
				ImGui::Text("There is no Options for this Comopnent Type.");
				break;

			case COM_TEXTURE:
				/* Input Prototype Tag */
				IMGUI_LEFT_LABEL(ImGui::InputText, "Component Tag", szPrototypeTag, sizeof(char) * MAX_PATH);

				/* Imput Texture Count */
				IMGUI_LEFT_LABEL(ImGui::InputInt, "Texture Count", (_int*)&iTextureCnt, 0, 0);

				/* Input File Path */
				IMGUI_LEFT_LABEL(ImGui::InputTextWithHint, "File Path", "Input Path here or Choose from FildDialog.", szFilePath, sizeof(char) * MAX_PATH);
				ImGui::SameLine();
				if (ImGui::SmallButton("Open"))
					ImGuiFileDialog::Instance()->OpenDialog("Choose Image", "Choose Image", Filter, "../Bin/Resources", ".", 1, nullptr, ImGuiFileDialogFlags_Modal);

				if (ImGuiFileDialog::Instance()->Display("Choose Image"))
				{
					if (ImGuiFileDialog::Instance()->IsOk())
					{
						string	strFilePath = ImGuiFileDialog::Instance()->GetFilePathName();
						char	szDrive[128] = "";
						char	szDir[MAX_PATH] = "";
						char	szFileName[MAX_PATH] = "";
						char	szExt[32] = "";
						char	szNewPath[MAX_PATH] = "";
						_splitpath_s(strFilePath.c_str(), szDrive, 128, szDir, MAX_PATH, szFileName, MAX_PATH, szExt, 32);
						CGameUtils::ConvertNumberTo_d(szFileName);
						_makepath_s(szNewPath, szDrive, szDir, szFileName, szExt);

						char	szRelativePath[MAX_PATH] = "";
						char	szDirectoryPath[MAX_PATH] = "";
						GetCurrentDirectoryA(MAX_PATH, szDirectoryPath);
						PathRelativePathToA(szRelativePath, szDirectoryPath, FILE_ATTRIBUTE_DIRECTORY, szNewPath, FILE_ATTRIBUTE_DIRECTORY);
						strcpy_s(szFilePath, sizeof(char) * MAX_PATH, szRelativePath);

						ImGuiFileDialog::Instance()->Close();
					}
					if (!ImGuiFileDialog::Instance()->IsOk())
						ImGuiFileDialog::Instance()->Close();
				}
				break;

			case COM_MODEL:
				/* Input Prototype Tag */
				IMGUI_LEFT_LABEL(ImGui::InputText, "Component Tag", szPrototypeTag, sizeof(char) * MAX_PATH);

				/* Input Model Type */
				IMGUI_LEFT_LABEL(ImGui::Combo, "Model Type", (_int*)&eModelType, szModelType, 2);

				/* Input File Path */
				IMGUI_LEFT_LABEL(ImGui::InputTextWithHint, "File Path", "Input Path here or Choose from FildDialog.", szFilePath, sizeof(char) * MAX_PATH);
				ImGui::SameLine();
				if (ImGui::SmallButton("Open"))
					ImGuiFileDialog::Instance()->OpenDialog("Choose Model", "Choose Model", ".fbx,.model", "../Bin/Resources/Meshes", ".", 1, nullptr, ImGuiFileDialogFlags_Modal);

				if (ImGuiFileDialog::Instance()->Display("Choose Model"))
				{
					if (ImGuiFileDialog::Instance()->IsOk())
					{
						char	szRelativePath[MAX_PATH] = "";
						char	szDirectoryPath[MAX_PATH] = "";
						GetCurrentDirectoryA(MAX_PATH, szDirectoryPath);
						PathRelativePathToA(szRelativePath, szDirectoryPath, FILE_ATTRIBUTE_DIRECTORY, ImGuiFileDialog::Instance()->GetFilePathName().c_str(), FILE_ATTRIBUTE_DIRECTORY);
						strcpy_s(szFilePath, sizeof(char) * MAX_PATH, szRelativePath);

						ImGuiFileDialog::Instance()->Close();
					}
					if (!ImGuiFileDialog::Instance()->IsOk())
						ImGuiFileDialog::Instance()->Close();
				}
				break;

			case COM_COLLIDER:
				IMGUI_LEFT_LABEL(ImGui::InputText, "Component Tag", szPrototypeTag, sizeof(char) * MAX_PATH);
				IMGUI_LEFT_LABEL(ImGui::Combo, "Collider Type", (_int*)&eColliderType, szColliderType, 3);
				break;
			}

			if (ImGui::Button("Create"))
			{
				for (size_t i = 0; i < m_mapProtoComponenets[iSelectLevel].size(); ++i)
					Safe_Delete_Array(ppProtoComponentTag[i]);
				Safe_Delete_Array(ppProtoComponentTag);

				switch (eComponentType)
				{
				case COM_RENDERER:
					break;

				case COM_VIBUFFER:
					break;

				case COM_SHADER:
				{
					_tchar		wszPrototypeTag[MAX_PATH] = L"";
					_tchar		wszFilePath[MAX_PATH] = L"";
					CGameUtils::ctwc(szPrototypeTag, wszPrototypeTag);
					CGameUtils::ctwc(szFilePath, wszFilePath);

					if (iSelectDeclaration == 0)
						CGameInstance::GetInstance()->Add_Prototype(iTargetLevel, wstring(wszPrototypeTag), CShader::Create(m_pDevice, m_pContext, wstring(wszFilePath), (CShader::DECLARATIONTYPE)iSelectDeclaration, VTXPOS_DECLARATION::Elements, VTXPOS_DECLARATION::iNumElements));
					else if (iSelectDeclaration == 1)
						CGameInstance::GetInstance()->Add_Prototype(iTargetLevel, wstring(wszPrototypeTag), CShader::Create(m_pDevice, m_pContext, wstring(wszFilePath), (CShader::DECLARATIONTYPE)iSelectDeclaration, VTXTEX_DECLARATION::Elements, VTXTEX_DECLARATION::iNumElements));
					else if (iSelectDeclaration == 2)
						CGameInstance::GetInstance()->Add_Prototype(iTargetLevel, wstring(wszPrototypeTag), CShader::Create(m_pDevice, m_pContext, wstring(wszFilePath), (CShader::DECLARATIONTYPE)iSelectDeclaration, VTXNORTEX_DECLARATION::Elements, VTXNORTEX_DECLARATION::iNumElements));
					else if (iSelectDeclaration == 3)
						CGameInstance::GetInstance()->Add_Prototype(iTargetLevel, wstring(wszPrototypeTag), CShader::Create(m_pDevice, m_pContext, wstring(wszFilePath), (CShader::DECLARATIONTYPE)iSelectDeclaration, VTXMODEL_DECLARATION::Elements, VTXMODEL_DECLARATION::iNumElements));
					else if (iSelectDeclaration == 4)
						CGameInstance::GetInstance()->Add_Prototype(iTargetLevel, wstring(wszPrototypeTag), CShader::Create(m_pDevice, m_pContext, wstring(wszFilePath), (CShader::DECLARATIONTYPE)iSelectDeclaration, VTXANIMMODEL_DECLARATION::Elements, VTXANIMMODEL_DECLARATION::iNumElements));

					break;
				}
				case COM_TRANSFORM:
					ImGui::Text("I am %d", COM_TRANSFORM);
					break;

				case COM_TEXTURE:
				{
					_tchar		wszPrototypeTag[MAX_PATH] = L"";
					_tchar		wszFilePath[MAX_PATH] = L"";
					CGameUtils::ctwc(szPrototypeTag, wszPrototypeTag);
					CGameUtils::ctwc(szFilePath, wszFilePath);

					CGameInstance::GetInstance()->Add_Prototype(iTargetLevel, wstring(wszPrototypeTag), CTexture::Create(m_pDevice, m_pContext, wstring(wszFilePath), iTextureCnt));

					break;
				}
				case COM_MODEL:
				{
					_tchar		wszPrototypeTag[MAX_PATH] = L"";
					CGameUtils::ctwc(szPrototypeTag, wszPrototypeTag);

					CGameInstance::GetInstance()->Add_Prototype(iTargetLevel, wstring(wszPrototypeTag), CModel::Create(m_pDevice, m_pContext, eModelType, szFilePath));

					break;
				}

				case COM_COLLIDER:
					_tchar		wszPrototypeTag[MAX_PATH] = L"";
					CGameUtils::ctwc(szPrototypeTag, wszPrototypeTag);

					CGameInstance::GetInstance()->Add_Prototype(iTargetLevel, wstring(wszPrototypeTag), CCollider::Create(m_pDevice, m_pContext, eColliderType));
					break;
				}
				ImGui::EndTabItem();

				bAddPrototype = false;
				strcpy_s(szPrototypeTag, "");
				strcpy_s(szFilePath, "");
				iSelectDeclaration = 0;
				iNumElements = 0;
				iTextureCnt = 1;
				eModelType = CModel::MODEL_NONANIM;
				eColliderType = CCollider::COLLIDER_SPHERE;

				return;
			}
			ImGui::SameLine();
			if (ImGui::Button("Cancel"))
			{
				bAddPrototype = false;
				strcpy_s(szPrototypeTag, "");
				strcpy_s(szFilePath, "");
				iSelectDeclaration = 0;
				iNumElements = 0;
				iTextureCnt = 1;
				eModelType = CModel::MODEL_NONANIM;
				eColliderType = CCollider::COLLIDER_SPHERE;
			}
			ImGui::Separator();
		}

		static char	szSaveFileName[MAX_PATH] = "";
		if (bSave)
		{
			iSelectComponent = -1;

			static _uint	iSaveLevel = 3;

			ImGui::Separator();

			IMGUI_LEFT_LABEL(ImGui::Combo, "Save Level", (_int*)&iSaveLevel, m_pLevelName, LEVEL_END + 1);
			IMGUI_LEFT_LABEL(ImGui::InputText, "File Name", szSaveFileName, MAX_PATH);

			if (ImGui::Button("Confirm"))
				ImGuiFileDialog::Instance()->OpenDialog("Select Folder", "Select Folder", ".json", "../Bin/Save Data", ".", 0, nullptr, ImGuiFileDialogFlags_Modal);

			ImGui::SameLine();
			if (ImGui::Button("Cancel"))
			{
				bSave = false;
				strcpy_s(szSaveFileName, "");
			}

			if (ImGuiFileDialog::Instance()->Display("Select Folder"))
			{
				if (ImGuiFileDialog::Instance()->IsOk())
				{
					for (size_t i = 0; i < m_mapProtoComponenets[iSelectLevel].size(); ++i)
						Safe_Delete_Array(ppProtoComponentTag[i]);
					Safe_Delete_Array(ppProtoComponentTag);

					bSave = false;

					Json jLevel;

					string		strLevelName = m_pLevelName[iSaveLevel];
					_tchar		wszLevelName[128] = L"";
					CGameUtils::ctwc(m_pLevelName[iSaveLevel], wszLevelName);
					jLevel["Level"] = m_pLevelName[iSaveLevel];

					Json jCom;

					for (auto& Pair : m_mapProtoComponenets[iSaveLevel])
					{
						COMPONENTTYPE eType = CheckComponentType(Pair.second);

						string	strComponentTag = CGameUtils::wstrTostr(Pair.first);
						string strFilePath = CGameUtils::wstrTostr(Pair.second->Get_FilePath());

						if (eType == COM_RENDERER)
						{
							jCom["Type"] = "Renderer";
							jCom["Tag"] = strComponentTag;
							jCom["File Path"] = strFilePath;
							jCom["Texture Count"] = 0;
							jCom["Vertex Declaration"] = "None";
							jCom["Vertex Elements Count"] = 0;
							jCom["Model Type"] = "None";
							jCom["Collider Type"] = "None";
						}
						else if (eType == COM_VIBUFFER)
						{
							jCom["Type"] = "VIBuffer";
							jCom["Tag"] = strComponentTag;
							jCom["File Path"] = strFilePath;
							jCom["Texture Count"] = 0;
							jCom["Vertex Declaration"] = "None";
							jCom["Vertex Elements Count"] = 0;
							jCom["Model Type"] = "None";
							jCom["Collider Type"] = "None";
						}
						else if (eType == COM_SHADER)
						{
							CShader::DECLARATIONTYPE	eDeclarationType = dynamic_cast<CShader*>(Pair.second)->Get_DeclarationType();
							if (eDeclarationType == CShader::DECLARATION_END)
								continue;

							jCom["Type"] = "Shader";
							jCom["Tag"] = strComponentTag;
							jCom["File Path"] = strFilePath;
							jCom["Texture Count"] = 0;

							if (eDeclarationType == CShader::DECLARATION_VTXPOS)
								jCom["Vertex Declaration"] = "Vtx_Position";
							else if (eDeclarationType == CShader::DECLARATION_VTXTEX)
								jCom["Vertex Declaration"] = "Vtx_Texture";
							else if (eDeclarationType == CShader::DECLARATION_VTXNORTEX)
								jCom["Vertex Declaration"] = "Vtx_NormalTexture";
							else if (eDeclarationType == CShader::DECLARATION_VTXMODEL)
								jCom["Vertex Declaration"] = "Vtx_NonAnimModel";
							else if (eDeclarationType == CShader::DECLARATION_VTXANIMMODEL)
								jCom["Vertex Declaration"] = "Vtx_AnimModel";

							jCom["Vertex Elements Count"] = dynamic_cast<CShader*>(Pair.second)->Get_ElementsCnt();
							jCom["Model Type"] = "None";
							jCom["Collider Type"] = "None";
						}
						else if (eType == COM_TRANSFORM)
						{
							jCom["Type"] = "Transform";
							jCom["Tag"] = strComponentTag;
							jCom["File Path"] = strFilePath;
							jCom["Texture Count"] = 0;
							jCom["Vertex Declaration"] = "None";
							jCom["Vertex Elements Count"] = 0;
							jCom["Model Type"] = "None";
							jCom["Collider Type"] = "None";
						}
						else if (eType == COM_TEXTURE)
						{
							jCom["Type"] = "Texture";
							jCom["Tag"] = strComponentTag;
							jCom["File Path"] = strFilePath;
							jCom["Texture Count"] = dynamic_cast<CTexture*>(Pair.second)->Get_TextureCnt();
							jCom["Vertex Declaration"] = "None";
							jCom["Vertex Elements Count"] = 0;
							jCom["Model Type"] = "None";
							jCom["Collider Type"] = "None";
						}
						else if (eType == COM_MODEL)
						{
							CModel::MODELTYPE		eModelType = dynamic_cast<CModel*>(Pair.second)->Get_ModelType();
							if (eModelType == CModel::MODELTYPE_END)
								continue;

							jCom["Type"] = "Model";
							jCom["Tag"] = strComponentTag;
							jCom["File Path"] = strFilePath;
							jCom["Texture Count"] = 0;
							jCom["Vertex Declaration"] = "None";
							jCom["Vertex Elements Count"] = 0;

							if (eModelType == CModel::MODEL_NONANIM)
								jCom["Model Type"] = "NonAnim";
							else if (eModelType == CModel::MODEL_ANIM)
								jCom["Model Type"] = "Anim";

							jCom["Collider Type"] = "None";
						}
						else if (eType == COM_COLLIDER)
						{
							CCollider::COLLIDERTYPE	eColliderType = dynamic_cast<CCollider*>(Pair.second)->Get_ColliderType();
							if (eColliderType == CCollider::COLLIDERTYPE_END)
								continue;

							jCom["Type"] = "Collider";
							jCom["Tag"] = strComponentTag;
							jCom["File Path"] = strFilePath;
							jCom["Texture Count"] = 0;
							jCom["Vectex Declaration"] = "None";
							jCom["Vertex Elements Count"] = 0;
							jCom["Model Type"] = "None";

							if (eColliderType == CCollider::COLLIDER_SPHERE)
								jCom["Collider Type"] = "Sphere";
							else if (eColliderType == CCollider::COLLIDER_AABB)
								jCom["Collider Type"] = "AABB";
							else if (eColliderType == CCollider::COLLIDER_OBB)
								jCom["Collider Type"] = "OBB";
						}

						jLevel["Components"].push_back(jCom);
					}

					string	strFolderDirectory = ImGuiFileDialog::Instance()->GetCurrentPath();
					char	szDash[128] = "\\";
					strcat_s(szDash, szSaveFileName);
					strFolderDirectory += string(szDash);
					strFolderDirectory += ".json";

					ofstream	file(strFolderDirectory.c_str());
					file << jLevel;
					file.close();

					strcpy_s(szSaveFileName, "");

					ImGuiFileDialog::Instance()->Close();
					ImGui::EndTabItem();
					return;
				}
				if (!ImGuiFileDialog::Instance()->IsOk())
				{
					bSave = false;
					ImGuiFileDialog::Instance()->Close();
				}
			}
		}

		if (ImGuiFileDialog::Instance()->Display("Select Json"))
		{
			iSelectComponent = -1;

			if (ImGuiFileDialog::Instance()->IsOk())
			{
				for (size_t i = 0; i < m_mapProtoComponenets[iSelectLevel].size(); ++i)
					Safe_Delete_Array(ppProtoComponentTag[i]);
				Safe_Delete_Array(ppProtoComponentTag);

				bLoad = false;

				string		strFilePath = ImGuiFileDialog::Instance()->GetFilePathName();
				Json		jLevel;

				ifstream	file(strFilePath.c_str());
				file >> jLevel;
				file.close();

				string	strLevel = jLevel["Level"];
				_uint	iLevelIndex = 1000;

				for (_uint i = 0; i < LEVEL_END + 1; ++i)
				{
					if (!strcmp(strLevel.c_str(), m_pLevelName[i]))
					{
						iLevelIndex = i;
						break;
					}
				}

				/* 툴은 동적이기 때문에 Level_Public 쪽은 건들지 않아줌. */
				if (iLevelIndex == 1000 || iLevelIndex == m_iCurLevel || iLevelIndex == LEVEL_PUBLIC)
				{
					ImGuiFileDialog::Instance()->Close();
					ImGui::EndTabItem();
					return;
				}

				for (auto& Pair : m_mapProtoComponenets[iLevelIndex])
					Safe_Release(Pair.second);
				m_mapProtoComponenets[iLevelIndex].clear();

				for (auto& Com : jLevel["Components"])
				{
					string		strComponentType = "", strComponentTag = "";
					wstring	wstrComponentTag = L"";

					Com["Type"].get_to<string>(strComponentType);
					Com["Tag"].get_to<string>(strComponentTag);
					wstrComponentTag.assign(strComponentTag.begin(), strComponentTag.end());

					if (strComponentType == "Renderer")
					{
						CGameInstance::GetInstance()->Add_Prototype(iLevelIndex, wstrComponentTag, CRenderer::Create(m_pDevice, m_pContext));
						continue;
					}
					else if (strComponentType == "VIBuffer")
					{
						CGameInstance::GetInstance()->Add_Prototype(iLevelIndex, wstrComponentTag, CVIBuffer_Rect::Create(m_pDevice, m_pContext));
						continue;
					}
					else if (strComponentType == "Shader")
					{
						string		strFilePath = "";
						wstring	wstrFilePath = L"";
						string		strDeclarationType = "";
						wstring	wstrDeclarationType = L"";
						_uint		iVertexElementsCount = 0;

						Com["File Path"].get_to<string>(strFilePath);
						wstrFilePath.assign(strFilePath.begin(), strFilePath.end());
						Com["Vertex Declaration"].get_to<string>(strDeclarationType);
						wstrDeclarationType.assign(strDeclarationType.begin(), strDeclarationType.end());
						Com["Vertex Elements Count"].get_to<_uint>(iVertexElementsCount);

						if (strDeclarationType == "Vtx_Position")
							CGameInstance::GetInstance()->Add_Prototype(iLevelIndex, wstrComponentTag, CShader::Create(m_pDevice, m_pContext, wstrFilePath, CShader::DECLARATION_VTXPOS, VTXPOS_DECLARATION::Elements, VTXPOS_DECLARATION::iNumElements));
						else if (strDeclarationType == "Vtx_Texture")
							CGameInstance::GetInstance()->Add_Prototype(iLevelIndex, wstrComponentTag, CShader::Create(m_pDevice, m_pContext, wstrFilePath, CShader::DECLARATION_VTXTEX, VTXTEX_DECLARATION::Elements, VTXTEX_DECLARATION::iNumElements));
						else if (strDeclarationType == "Vtx_NormalTexture")
							CGameInstance::GetInstance()->Add_Prototype(iLevelIndex, wstrComponentTag, CShader::Create(m_pDevice, m_pContext, wstrFilePath, CShader::DECLARATION_VTXNORTEX, VTXNORTEX_DECLARATION::Elements, VTXNORTEX_DECLARATION::iNumElements));
						else if (strDeclarationType == "Vtx_NonAnimModel")
							CGameInstance::GetInstance()->Add_Prototype(iLevelIndex, wstrComponentTag, CShader::Create(m_pDevice, m_pContext, wstrFilePath, CShader::DECLARATION_VTXMODEL, VTXMODEL_DECLARATION::Elements, VTXMODEL_DECLARATION::iNumElements));
						else if (strDeclarationType == "Vtx_AnimModel")
							CGameInstance::GetInstance()->Add_Prototype(iLevelIndex, wstrComponentTag, CShader::Create(m_pDevice, m_pContext, wstrFilePath, CShader::DECLARATION_VTXANIMMODEL, VTXANIMMODEL_DECLARATION::Elements, VTXANIMMODEL_DECLARATION::iNumElements));
						else
							continue;

						continue;
					}
					else if (strComponentType == "Transform")
					{
						CGameInstance::GetInstance()->Add_Prototype(iLevelIndex, wstrComponentTag, CTransform::Create(m_pDevice, m_pContext));
						continue;
					}
					else if (strComponentType == "Texture")
					{
						string		strFilePath = "";
						wstring	wstrFilePath = L"";
						_uint		iTextureCount = 0;

						Com["File Path"].get_to<string>(strFilePath);
						wstrFilePath.assign(strFilePath.begin(), strFilePath.end());
						Com["Texture Count"].get_to<_uint>(iTextureCount);

						CGameInstance::GetInstance()->Add_Prototype(iLevelIndex, wstrComponentTag, CTexture::Create(m_pDevice, m_pContext, wstrFilePath, iTextureCount));
						continue;
					}
					else if (strComponentType == "Model")
					{
						string		strFilePath = "";
						string		strModelType = "";
						wstring	wstrModelType = L"";

						Com["File Path"].get_to<string>(strFilePath);
						Com["Model Type"].get_to<string>(strModelType);
						wstrModelType.assign(strModelType.begin(), strModelType.end());

						if (strModelType == "NonAnim")
							CGameInstance::GetInstance()->Add_Prototype(iLevelIndex, wstrComponentTag, CModel::Create(m_pDevice, m_pContext, CModel::MODEL_NONANIM, strFilePath.c_str()));
						else if (strModelType == "Anim")
							CGameInstance::GetInstance()->Add_Prototype(iLevelIndex, wstrComponentTag, CModel::Create(m_pDevice, m_pContext, CModel::MODEL_ANIM, strFilePath.c_str()));
						else
							continue;
					}
					else if (strComponentType == "Collider")
					{
						string		strColliderType = "";
						wstring	wstrColliderType = L"";

						Com["Collider Type"].get_to<string>(strColliderType);
						wstrColliderType.assign(strColliderType.begin(), strColliderType.end());

						if (strColliderType == "Sphere")
							CGameInstance::GetInstance()->Add_Prototype(iLevelIndex, wstrComponentTag, CCollider::Create(m_pDevice, m_pContext, CCollider::COLLIDER_SPHERE));
						else if (strColliderType == "AABB")
							CGameInstance::GetInstance()->Add_Prototype(iLevelIndex, wstrComponentTag, CCollider::Create(m_pDevice, m_pContext, CCollider::COLLIDER_AABB));
						else if (strColliderType == "OBB")
							CGameInstance::GetInstance()->Add_Prototype(iLevelIndex, wstrComponentTag, CCollider::Create(m_pDevice, m_pContext, CCollider::COLLIDER_OBB));
						else
							continue;
					}
				}

				jLevel.clear();

				ImGuiFileDialog::Instance()->Close();
				ImGui::EndTabItem();
				return;
			}
			if (!ImGuiFileDialog::Instance()->IsOk())
			{
				bLoad = false;
				ImGuiFileDialog::Instance()->Close();
			}
		}

		if (iSelectComponent != -1)
		{
			ImGui::Separator();

			CComponent*		pComponent = nullptr;

			for (auto Pair : m_mapProtoComponenets[iSelectLevel])
			{
				_tchar		wszComponentTag[MAX_PATH] = L"";
				CGameUtils::ctwc(ppProtoComponentTag[iSelectComponent], wszComponentTag);
				if (!lstrcmp(Pair.first.c_str(), wszComponentTag))
				{
					pComponent = Pair.second;
					break;
				}
			}

			if (nullptr != pComponent)
				pComponent->Imgui_RenderProperty();


			/* 컴포넌트 별로 기능 수행하기 */
			switch (CheckComponentType(iSelectLevel, ppProtoComponentTag[iSelectComponent]))
			{
			case COM_RENDERER:
				ImGui::Text("I am %d", COM_RENDERER);
				break;

			case COM_VIBUFFER:
				ImGui::Text("I am %d", COM_VIBUFFER);
				break;

			case COM_SHADER:
				ImGui::Text("I am %d", COM_SHADER);
				break;

			case COM_TRANSFORM:
				ImGui::Text("I am %d", COM_TRANSFORM);
				break;

			case COM_TEXTURE:
				ImGui::Text("I am %d", COM_TEXTURE);
				break;

			case COM_MODEL:
				ImGui::Text("I am %d", COM_MODEL);
				break;

			case COM_COLLIDER:
				ImGui::Text("I am %d", COM_COLLIDER);
				break;

			case COMPONENTTYPE_END:
				return;
			}
		}

		for (size_t i = 0; i < m_mapProtoComponenets[iSelectLevel].size(); ++i)
			Safe_Delete_Array(ppProtoComponentTag[i]);
		Safe_Delete_Array(ppProtoComponentTag);

		ImGui::EndTabItem();
	}
}

void CImgui_ProtoMgr::GameObject_Editor()
{
	_int	iSelectObject = 1000;
	if (ImGui::BeginTabItem("GameObject"))
	{
		m_iProtoObjCnt = (_uint)m_mapProtoObjects->size();
		ImGui::BulletText("Prototype GameObject List");

		char**			ppProtoObjTag = new char*[m_iProtoObjCnt];
		_uint i = 0;
		for (auto Pair : *m_mapProtoObjects)
		{
			ppProtoObjTag[i] = new char[Pair.first.length() + 1];
			CGameUtils::wc2c(Pair.first.c_str(), ppProtoObjTag[i]);
			++i;
		}

		ImGui::ListBox(" ", &iSelectObject, ppProtoObjTag, (_int)m_iProtoObjCnt);

		static _bool	bSave = false;
		if (ImGui::Button("Save"))
			bSave = true;
		ImGui::SameLine();
		if (ImGui::Button("Load"))
		{
			if (bSave)
				bSave = false;

			ImGuiFileDialog::Instance()->OpenDialog("Load Json File", "Select Json File", ".json", "../Bin/Save Data", ".", 1, nullptr, ImGuiFileDialogFlags_Modal);
		}

		ImGui::Separator();

		if (bSave)
		{
			ImGui::BulletText("Save Prototype Objects");

			static	char	szSaveFileName[MAX_PATH] = "";
			ImGui::InputText("File Name", szSaveFileName, MAX_PATH);

			if (ImGui::Button("Confirm"))
				ImGuiFileDialog::Instance()->OpenDialog("Select Save Folder", "Select Save Folder", ".json", "../Bin/Save Data", ".", 0, nullptr, ImGuiFileDialogFlags_Modal);
			ImGui::SameLine();
			if (ImGui::Button("Cancel"))
			{
				bSave = false;
				strcpy_s(szSaveFileName, "");
			}
			if (ImGuiFileDialog::Instance()->Display("Select Save Folder"))
			{
				if (ImGuiFileDialog::Instance()->IsOk())
				{
					string		strSaveDirectory = ImGuiFileDialog::Instance()->GetCurrentPath();
					char	szDash[128] = "\\";
					strcat_s(szDash, szSaveFileName);
					strSaveDirectory += string(szDash);
					strSaveDirectory += ".json";

					Json	jPrototypeObjects;
					Json	jObject;
					Json	jComponents;

					for (auto& Pair : *m_mapProtoObjects)
					{
						CCustomObject*	pObject = dynamic_cast<CCustomObject*>(Pair.second);
						// 일단은 CustomGameObject만 저장 해보기.
						if (pObject != nullptr)
						{
							char	szPrototypeGameObjectTag[MAX_PATH] = "";
							CGameUtils::wc2c(Pair.first.c_str(), szPrototypeGameObjectTag);
							jObject["Prototype Object Tag"] = szPrototypeGameObjectTag;

							_uint	iTextureComCount = pObject->Get_TextureComCount();
							jObject["Texture Component Count"] = iTextureComCount;

							vector<pair<_uint, wstring>>	vecComponentInfo = pObject->Get_PrototypeSaveData();
							for (auto PairCom : vecComponentInfo)
							{
								char	szPrototypeComponentTag[MAX_PATH] = "";
								CGameUtils::wc2c(PairCom.second.c_str(), szPrototypeComponentTag);
								jComponents["Level"] = PairCom.first;
								jComponents["Component Tag"] = szPrototypeComponentTag;
								jObject["Components"].push_back(jComponents);
							}

							jPrototypeObjects["Objects"].push_back(jObject);
						}
					}

					ofstream		file(strSaveDirectory.c_str());
					file << jPrototypeObjects;
					file.close();

					bSave = false;
					strcpy_s(szSaveFileName, "");
					ImGuiFileDialog::Instance()->Close();

					for (size_t i = 0; i < m_iProtoObjCnt; ++i)
						Safe_Delete_Array(ppProtoObjTag[i]);
					Safe_Delete_Array(ppProtoObjTag);

					ImGui::EndTabItem();

					return;
				}
				if (!ImGuiFileDialog::Instance()->IsOk())
				{
					bSave = false;
					strcpy_s(szSaveFileName, "");
					ImGuiFileDialog::Instance()->Close();
				}
			}
		}

		if (ImGuiFileDialog::Instance()->Display("Load Json File"))
		{
			if (ImGuiFileDialog::Instance()->IsOk())
			{
				for (size_t i = 0; i < m_iProtoObjCnt; ++i)
					Safe_Delete_Array(ppProtoObjTag[i]);
				Safe_Delete_Array(ppProtoObjTag);

				string		strLoadFile = ImGuiFileDialog::Instance()->GetFilePathName();

				Json	jPrototypeObjects;

				ifstream	file(strLoadFile.c_str());
				file >> jPrototypeObjects;
				file.close();

				for (auto jObj : jPrototypeObjects["Objects"])
				{
					string		strObjectTag;
					wstring	wstrObjectTag;
					_uint		iTextureComCount = 1000;
					vector<pair<_uint, wstring>>	vecComponentInfo;

					jObj["Prototype Object Tag"].get_to<string>(strObjectTag);
					wstrObjectTag.assign(strObjectTag.begin(), strObjectTag.end());
					jObj["Texture Component Count"].get_to<_uint>(iTextureComCount);

					for (auto jCom : jObj["Components"])
					{
						_uint		iComponentLevelIndex = 1000;
						string		strComponentTag = "";
						wstring	wstrComponentTag = L"";

						jCom["Level"].get_to<_uint>(iComponentLevelIndex);
						jCom["Component Tag"].get_to<string>(strComponentTag);
						wstrComponentTag.assign(strComponentTag.begin(), strComponentTag.end());

						vecComponentInfo.push_back(pair<_uint, wstring>(iComponentLevelIndex, wstrComponentTag));
					}

					CGameInstance::GetInstance()->Add_Prototype(wstrObjectTag, CCustomObject::Create(m_pDevice, m_pContext, vecComponentInfo));
				}
				ImGuiFileDialog::Instance()->Close();

				ImGui::EndTabItem();

				return;
			}
			if (!ImGuiFileDialog::Instance()->IsOk())
				ImGuiFileDialog::Instance()->Close();
		}

		if (!bSave && !ImGuiFileDialog::Instance()->Display("Load Json File"))
		{
			ImGui::BulletText("Add Prototype GameObject");

			static char	szPrototypeTag[MAX_PATH] = "";
			char***		ppComponentTag = nullptr;
			_uint			iComponentCnt[COMPONENTTYPE_END];
			SortComponentByType(ppComponentTag, iComponentCnt);
			static _int	iSelectRender = 0, iSelectVIBuffer = 0, iSelectShader = 0, iSelectTransform = 0, iSelectModel = 0, iSelectCollider = 0;

			static _int	iTextureComCnt = 0;
			static _int	iLastTextureComCnt = iTextureComCnt;

			if (m_iSelectTextureCom == nullptr)
				m_iSelectTextureCom = new _int[iTextureComCnt];

			if (iTextureComCnt != iLastTextureComCnt)
			{
				_int*		iTemp = new _int[iTextureComCnt];
				ZeroMemory(iTemp, sizeof(_int) * iTextureComCnt);

				if (iTextureComCnt < iLastTextureComCnt)
					memcpy(iTemp, m_iSelectTextureCom, sizeof(_int) * iTextureComCnt);
				else
					memcpy(iTemp, m_iSelectTextureCom, sizeof(_int) * iLastTextureComCnt);
				Safe_Delete_Array(m_iSelectTextureCom);

				m_iSelectTextureCom = iTemp;
				iLastTextureComCnt = iTextureComCnt;
			}

			ImGui::BulletText("Object Tag");
			IMGUI_LEFT_LABEL(ImGui::InputText, "Object Tag", szPrototypeTag, sizeof(char) * MAX_PATH);


			ImGui::NewLine();
			ImGui::BulletText("Select Component");

			ImGui::Combo("Renderer", &iSelectRender, ppComponentTag[COM_RENDERER], iComponentCnt[COM_RENDERER]);
			ImGui::Combo("VIBuffer", &iSelectVIBuffer, ppComponentTag[COM_VIBUFFER], iComponentCnt[COM_VIBUFFER]);
			ImGui::Combo("Shader", &iSelectShader, ppComponentTag[COM_SHADER], iComponentCnt[COM_SHADER]);
			ImGui::Combo("Transform", &iSelectTransform, ppComponentTag[COM_TRANSFORM], iComponentCnt[COM_TRANSFORM]);
			ImGui::InputInt("Texture Component Count", &iTextureComCnt, 1, 0);
			for (_int i = 0; i < iTextureComCnt; ++i)
			{
				char	szTextureLabel[32] = "";
				sprintf(szTextureLabel, "Texture%d", i);
				const char*	pTextureLabel = szTextureLabel;
				ImGui::Combo(pTextureLabel, &m_iSelectTextureCom[i], ppComponentTag[COM_TEXTURE], iComponentCnt[COM_TEXTURE]);
			}
			ImGui::Combo("Model", &iSelectModel, ppComponentTag[COM_MODEL], iComponentCnt[COM_MODEL]);
			ImGui::Combo("Collider", &iSelectCollider, ppComponentTag[COM_COLLIDER], iComponentCnt[COM_COLLIDER]);

			if (ImGui::Button("Create"))
			{
				vector<pair<_uint, wstring>>	vecPrototypeInfo;
				vecPrototypeInfo.reserve(10);

				_tchar		wszComponentTag[MAX_PATH] = L"";
				_uint		iPrototypeLevel = 0;
				pair<_uint, wstring>		MyPair;
				CGameUtils::ctwc(ppComponentTag[COM_RENDERER][iSelectRender], wszComponentTag);
				iPrototypeLevel = FindPrototypeComponentLevel(wszComponentTag);
				if (iPrototypeLevel != 1000)
				{
					MyPair.first = iPrototypeLevel;
					MyPair.second = wstring(wszComponentTag);
					vecPrototypeInfo.push_back(MyPair);
				}
				CGameUtils::ctwc(ppComponentTag[COM_VIBUFFER][iSelectVIBuffer], wszComponentTag);
				iPrototypeLevel = FindPrototypeComponentLevel(wszComponentTag);
				if (iPrototypeLevel != 1000)
				{
					MyPair.first = iPrototypeLevel;
					MyPair.second = wstring(wszComponentTag);
					vecPrototypeInfo.push_back(MyPair);
				}
				CGameUtils::ctwc(ppComponentTag[COM_SHADER][iSelectShader], wszComponentTag);
				iPrototypeLevel = FindPrototypeComponentLevel(wszComponentTag);
				if (iPrototypeLevel != 1000)
				{
					MyPair.first = iPrototypeLevel;
					MyPair.second = wstring(wszComponentTag);
					vecPrototypeInfo.push_back(MyPair);
				}
				CGameUtils::ctwc(ppComponentTag[COM_TRANSFORM][iSelectTransform], wszComponentTag);
				iPrototypeLevel = FindPrototypeComponentLevel(wszComponentTag);
				if (iPrototypeLevel != 1000)
				{
					MyPair.first = iPrototypeLevel;
					MyPair.second = wstring(wszComponentTag);
					vecPrototypeInfo.push_back(MyPair);
				}
				for (_int i = 0; i < iTextureComCnt; ++i)
				{
					CGameUtils::ctwc(ppComponentTag[COM_TEXTURE][m_iSelectTextureCom[i]], wszComponentTag);
					iPrototypeLevel = FindPrototypeComponentLevel(wszComponentTag);
					if (iPrototypeLevel != 1000)
					{
						MyPair.first = iPrototypeLevel;
						MyPair.second = wstring(wszComponentTag);
						vecPrototypeInfo.push_back(MyPair);
					}
				}
				CGameUtils::ctwc(ppComponentTag[COM_MODEL][iSelectModel], wszComponentTag);
				iPrototypeLevel = FindPrototypeComponentLevel(wszComponentTag);
				if (iPrototypeLevel != 1000)
				{
					MyPair.first = iPrototypeLevel;
					MyPair.second = wstring(wszComponentTag);
					vecPrototypeInfo.push_back(MyPair);
				}
				CGameUtils::ctwc(ppComponentTag[COM_COLLIDER][iSelectCollider], wszComponentTag);
				iPrototypeLevel = FindPrototypeComponentLevel(wszComponentTag);
				if (iPrototypeLevel != 1000)
				{
					MyPair.first = iPrototypeLevel;
					MyPair.second = wstring(wszComponentTag);
					vecPrototypeInfo.push_back(MyPair);
				}

				_tchar		wszPrototypeTag[MAX_PATH] = L"";
				CGameUtils::ctwc(szPrototypeTag, wszPrototypeTag);
				CGameInstance::GetInstance()->Add_Prototype(wszPrototypeTag, CCustomObject::Create(m_pDevice, m_pContext, vecPrototypeInfo, iTextureComCnt));

				vecPrototypeInfo.clear();

				for (size_t i = 0; i < m_iProtoObjCnt; ++i)
					Safe_Delete_Array(ppProtoObjTag[i]);
				Safe_Delete_Array(ppProtoObjTag);

				for (_uint i = 0; i < (_uint)COMPONENTTYPE_END; ++i)
				{
					for (_uint j = 0; j < iComponentCnt[i]; ++j)
						Safe_Delete_Array(ppComponentTag[i][j]);
					Safe_Delete_Array(ppComponentTag[i]);
				}
				Safe_Delete_Array(ppComponentTag);

				strcpy_s(szPrototypeTag, "");
				iSelectRender = 0;
				iSelectVIBuffer = 0;
				iSelectShader = 0;
				iSelectTransform = 0;
				iSelectModel = 0;
				iTextureComCnt = 0;
				iLastTextureComCnt = iTextureComCnt;

				ImGui::EndTabItem();

				return;
			}
			ImGui::SameLine();
			if (ImGui::Button("Cancel"))
			{
				strcpy_s(szPrototypeTag, "");
				iSelectRender = 0;
				iSelectVIBuffer = 0;
				iSelectShader = 0;
				iSelectTransform = 0;
				iSelectModel = 0;
				iTextureComCnt = 0;
				iLastTextureComCnt = iTextureComCnt;
			}
			for (size_t i = 0; i < m_iProtoObjCnt; ++i)
				Safe_Delete_Array(ppProtoObjTag[i]);
			Safe_Delete_Array(ppProtoObjTag);

			for (_uint i = 0; i < (_uint)COMPONENTTYPE_END; ++i)
			{
				for (_uint j = 0; j < iComponentCnt[i]; ++j)
					Safe_Delete_Array(ppComponentTag[i][j]);
				Safe_Delete_Array(ppComponentTag[i]);
			}
			Safe_Delete_Array(ppComponentTag);
		}
		else
		{
			for (size_t i = 0; i < m_iProtoObjCnt; ++i)
				Safe_Delete_Array(ppProtoObjTag[i]);
			Safe_Delete_Array(ppProtoObjTag);
		}

		ImGui::EndTabItem();
	}
}

void CImgui_ProtoMgr::CloneObject_Editor()
{
	m_mapLayers = CGameInstance::GetInstance()->Get_Layers(m_iCurLevel);
	m_iLayerCnt = (_uint)m_mapLayers->size();

	static _uint		iSelectLayer = 0;
	static _int			iSelectObject = -1;
	static _bool		bSave = false;

	if(ImGui::BeginTabItem("CloneObject"))
	{
		if(m_iLayerCnt == 0)
		{
			ImGui::EndTabItem();
			return;
		}

		char** ppLayerTag = new char*[m_iLayerCnt];
		_uint i = 0;
		for(auto Pair : *m_mapLayers)
		{
			ppLayerTag[i] = new char[Pair.first.length() + 1];
			CGameUtils::wc2c(Pair.first.c_str(), ppLayerTag[i++]);
		}

		ImGui::BulletText("Current Level : %s", m_pLevelName[m_iCurLevel]);
		ImGui::BulletText("CloneObject List");
		ImGui::Combo("##", (_int*)&iSelectLayer, ppLayerTag, (_int)m_iLayerCnt);

		string	strSelectLayerTag = ppLayerTag[iSelectLayer];
		wstring wstrSelectLayerTag = L"";
		wstrSelectLayerTag.assign(strSelectLayerTag.begin(), strSelectLayerTag.end());

		list<class CGameObject*>*	CloneObjectList = CGameInstance::GetInstance()->Get_CloneObjectList(m_iCurLevel, wstrSelectLayerTag);
		char**		ppCloneObjectTag = new char*[CloneObjectList->size()];

		CGameObject*		pCloneObject = nullptr;
		_uint				j = 0;
		for(auto& Obj : *CloneObjectList)
		{
			wstring wstrPrototypeGameObjectTag = Obj->Get_PrototypeGameObjectTag();
			string	strPrototypeGameObjectTag = "";
			strPrototypeGameObjectTag.assign(wstrPrototypeGameObjectTag.begin(), wstrPrototypeGameObjectTag.end());

			ppCloneObjectTag[j] = new char[strPrototypeGameObjectTag.length() + 1];
			strcpy_s(ppCloneObjectTag[j], strPrototypeGameObjectTag.length() + 1, strPrototypeGameObjectTag.c_str());

			if (j++ == iSelectObject)
				pCloneObject = Obj;
		}

		ImGui::BulletText("CloneObjects List");
		ImGui::ListBox("##", &iSelectObject, ppCloneObjectTag, (_int)CloneObjectList->size());

		if (ImGui::Button("Save"))
			bSave = true;
		ImGui::SameLine();
		if(ImGui::Button("Load"))
		{
			if (bSave)
				bSave = false;

			ImGuiFileDialog::Instance()->OpenDialog("Load Layer", "Select Layer Json", ".json", "../Bin/Save Data", ".", 1, nullptr, ImGuiFileDialogFlags_Modal);
		}

		if(bSave)
		{
			static char szSaveFileName[MAX_PATH] = "";
			ImGui::Separator();
			ImGui::InputText("File Name", szSaveFileName, MAX_PATH);

			if (ImGui::Button("Confirm"))
				ImGuiFileDialog::Instance()->OpenDialog("Save Layer", "Select Folder Directory", ".json", "../Bin/Save Data", ".", 0, nullptr, ImGuiFileDialogFlags_Modal);

			ImGui::SameLine();
			if(ImGui::Button("Cancel"))
			{
				strcpy_s(szSaveFileName, "");
				bSave = false;
			}

			if (ImGuiFileDialog::Instance()->Display("Save Layer"))
			{
				if (ImGuiFileDialog::Instance()->IsOk())
				{
					string strSaveFileDirectory = ImGuiFileDialog::Instance()->GetCurrentPath();
					char	szDash[128] = "\\";
					strcat_s(szDash, szSaveFileName);
					strSaveFileDirectory += string(szDash);
					strSaveFileDirectory += ".json";

					Json jLayers;
					Json jLayer;
					Json jCloneObj;

					jLayers["Level"] = m_pLevelName[m_iCurLevel];

					for(auto& Pair : *m_mapLayers)
					{
						list<CGameObject*>* CloneObjects = CGameInstance::GetInstance()->Get_CloneObjectList(m_iCurLevel, Pair.first);
						if (CloneObjects->size() == 0 || CloneObjects == nullptr)
							continue;

						string	strLayerTag = "";
						strLayerTag.assign(Pair.first.begin(), Pair.first.end());
						jLayer["Layer Tag"] = strLayerTag;

						for(auto& Obj : *CloneObjects)
						{
							// 클론 오브젝트 저장 내용 : 원본 오브젝트 태그, 클론 오브젝트의 월드 행렬
							string	strPrototypeObjTag = "";
							strPrototypeObjTag.assign(Obj->Get_PrototypeGameObjectTag().begin(), Obj->Get_PrototypeGameObjectTag().end());
							jCloneObj["Prototype GameObject Tag"] = strPrototypeObjTag;

							_float4x4		WorldMatrix = Obj->Get_WorldFloat4x4();
							for(_uint i =0; i< 16; ++i)
							{
								_float	fElement = 0.f;
								memcpy(&fElement, ((_float*)&WorldMatrix) + i, sizeof(_float));
								jCloneObj["Transform State"].push_back(fElement);
							}
							jLayer["Clone Objects"].push_back(jCloneObj);
							jCloneObj.clear();
						}
						jLayers["Layers"].push_back(jLayer);
						jLayer.clear();
					}

					ofstream	file(strSaveFileDirectory.c_str());
					file << jLayers;
					file.close();

					strcpy_s(szSaveFileName, "");
					ImGuiFileDialog::Instance()->Close();
				}
				if (!ImGuiFileDialog::Instance()->IsOk())
					ImGuiFileDialog::Instance()->Close();
			}
		}
		if(ImGuiFileDialog::Instance()->Display("Load Layer"))
		{
			if(ImGuiFileDialog::Instance()->IsOk())
			{
				for (size_t i = 0; i < CloneObjectList->size(); ++i)
					Safe_Delete_Array(ppCloneObjectTag[i]);
				Safe_Delete_Array(ppCloneObjectTag);

				for (_uint i = 0; i < m_iLayerCnt; ++i)
					Safe_Delete_Array(ppLayerTag[i]);
				Safe_Delete_Array(ppLayerTag);
					
				string	strFilePath = ImGuiFileDialog::Instance()->GetFilePathName();

				CGameInstance*	pGameInstance = CGameInstance::GetInstance();
				Safe_AddRef(pGameInstance);
				pGameInstance->Clear_Level(m_iCurLevel);

				Json	jLayers;

				ifstream	file(strFilePath.c_str());
				file >> jLayers;
				file.close();

				CPlayer*	pPlayer = nullptr;

				for(auto jLayer : jLayers["Layers"])
				{
					string	strLayerTag = "";
					wstring	wstrLayerTag = L"";
					jLayer["Layer Tag"].get_to<string>(strLayerTag);

					if (strLayerTag == "Layer_Monster")
						continue;

					wstrLayerTag.assign(strLayerTag.begin(), strLayerTag.end());

					for(auto jCloneObj : jLayer["Clone Objects"])
					{
						string	strPrototypeObjTag = "";
						wstring wstrPrototypeObjTag = L"";
						jCloneObj["Prototype GameObject Tag"].get_to<string>(strPrototypeObjTag);

						wstrPrototypeObjTag.assign(strPrototypeObjTag.begin(), strPrototypeObjTag.end());

						_float4x4	matWorld;
						XMStoreFloat4x4(&matWorld, XMMatrixIdentity());

						_uint k = 0;
						for(_float fElement : jCloneObj["Transform State"])
							memcpy(((_float*)&matWorld) + (k++), &fElement, sizeof(_float));

						CGameObject* pGameObject = pGameInstance->Clone_GameObjectReturnPtr_M(m_iCurLevel, wstrLayerTag, wstrPrototypeObjTag, matWorld);
						if (dynamic_cast<CPlayer*>(pGameObject))
							pPlayer = dynamic_cast<CPlayer*>(pGameObject);
					}
				}
				for(auto jLayer : jLayers["Layers"])
				{
					string			strLayertag = "";
					wstring			wstrLayerTag = L"";
					jLayer["Layer_Tag"].get_to<string>(strLayertag);

					if (strLayertag != "Layer_Monster")
						continue;

					wstrLayerTag.assign(strLayertag.begin(), strLayertag.end());

					for(auto jCloneObj : jLayer["Clone Objects"])
					{
						string		strPrototypeObjTag = "";
						wstring	wstrPrototypeObjTag = L"";
						jCloneObj["Prototype GameObject Tag"].get_to<string>(strPrototypeObjTag);

						wstrPrototypeObjTag.assign(strPrototypeObjTag.begin(), strPrototypeObjTag.end());

						_float4x4		matWorld;
						XMStoreFloat4x4(&matWorld, XMMatrixIdentity());

						_uint	k = 0;
						for (_float fElement : jCloneObj["Transform State"])
							memcpy(((_float*)&matWorld) + (k++), &fElement, sizeof(_float));

						CGameObject*	pGameObject = pGameInstance->Clone_GameObjectReturnPtr_M(m_iCurLevel, wstrLayerTag, wstrPrototypeObjTag, matWorld);

						if (CMonster*	pEnemy = dynamic_cast<CMonster*>(pGameObject))
							pEnemy->Set_Player(pPlayer);
					}
				}
				Safe_Release(pGameInstance);

				ImGuiFileDialog::Instance()->Close();

				ImGui::EndTabItem();

				return;
			}
			if (!ImGuiFileDialog::Instance()->IsOk())
				ImGuiFileDialog::Instance()->Close();
		}

		if(!bSave && !ImGuiFileDialog::Instance()->Display("Load Layer") && pCloneObject != nullptr)
		{
			ImGui::Separator();

			wstring wstrPrototypeGameObjectTag = pCloneObject->Get_PrototypeGameObjectTag();
			string	strPrototypeGameObjectTag = "";
			strPrototypeGameObjectTag.assign(wstrPrototypeGameObjectTag.begin(), wstrPrototypeGameObjectTag.end());

			ImGui::BulletText("Prototype GameObject Tag : %s", strPrototypeGameObjectTag.c_str());

			pCloneObject->Imgui_RenderProperty();
				pCloneObject->Imgui_RenderComponentProperties();

			if(ImGui::Button("Move to"))
			{
				
			}
			ImGui::SameLine();
			if(ImGui::Button("Delete"))
			{
				for (size_t i = 0; i < CloneObjectList->size(); ++i)
					Safe_Delete_Array(ppCloneObjectTag[i]);
				Safe_Delete_Array(ppCloneObjectTag);

				for (_uint i = 0; i < m_iLayerCnt; ++i)
					Safe_Delete_Array(ppLayerTag[i]);
				Safe_Delete_Array(ppLayerTag);

				auto iter = CloneObjectList->begin();
				for (_int i = 0; i < iSelectObject; ++i)
					iter++;

				Safe_Release(*iter);
				CloneObjectList->erase(iter);

				iSelectObject = -1;

				ImGui::EndTabItem();

				return;				
			}
		}
		for (size_t i = 0; i < CloneObjectList->size(); ++i)
			Safe_Delete_Array(ppCloneObjectTag[i]);
		Safe_Delete_Array(ppCloneObjectTag);

		for (_uint i = 0; i < m_iLayerCnt; ++i)
			Safe_Delete_Array(ppLayerTag[i]);
		Safe_Delete_Array(ppLayerTag);

		ImGui::EndTabItem();

	}

}


COMPONENTTYPE CImgui_ProtoMgr::CheckComponentType(_int iSelectLevel, const char* pComponentTag)
{
	CComponent*		pComponent = nullptr;

	for (auto Pair : m_mapProtoComponenets[iSelectLevel])
	{
		_tchar		wszComponentTag[MAX_PATH] = L"";
		CGameUtils::ctwc(pComponentTag, wszComponentTag);
		if (!lstrcmp(Pair.first.c_str(), wszComponentTag))
		{
			pComponent = Pair.second;
			break;
		}
	}

	NULL_CHECK_RETURN(pComponent, COMPONENTTYPE_END);

	if (dynamic_cast<CRenderer*>(pComponent))
		return COM_RENDERER;
	if (dynamic_cast<CVIBuffer*>(pComponent))
		return COM_VIBUFFER;
	if (dynamic_cast<CShader*>(pComponent))
		return COM_SHADER;
	if (dynamic_cast<CTransform*>(pComponent))
		return COM_TRANSFORM;
	if (dynamic_cast<CTexture*>(pComponent))
		return COM_TEXTURE;
	if (dynamic_cast<CModel*>(pComponent))
		return COM_MODEL;
	if (dynamic_cast<CCollider*>(pComponent))
		return COM_COLLIDER;

	return COMPONENTTYPE_END;
}

COMPONENTTYPE CImgui_ProtoMgr::CheckComponentType(CComponent* pComponent)
{
	NULL_CHECK_RETURN(pComponent, COMPONENTTYPE_END);

	if (dynamic_cast<CRenderer*>(pComponent))
		return COM_RENDERER;
	if (dynamic_cast<CVIBuffer*>(pComponent))
		return COM_VIBUFFER;
	if (dynamic_cast<CShader*>(pComponent))
		return COM_SHADER;
	if (dynamic_cast<CTransform*>(pComponent))
		return COM_TRANSFORM;
	if (dynamic_cast<CTexture*>(pComponent))
		return COM_TEXTURE;
	if (dynamic_cast<CModel*>(pComponent))
		return COM_MODEL;
	if (dynamic_cast<CCollider*>(pComponent))
		return COM_COLLIDER;

	return COMPONENTTYPE_END;
}

void CImgui_ProtoMgr::SortComponentByType(char***& ppComponentTag, _uint* pComponentCnt)
{
	if (ppComponentTag != nullptr)
		return;

	_uint ComponentCnt[COMPONENTTYPE_END] = { 1, 1, 1, 1, 1, 1, 1 };

	for (_uint i = 0; i < LEVEL_END + 1; ++i)
	{
		for (auto& Pair : m_mapProtoComponenets[i])
		{
			NULL_CHECK_RETURN(Pair.second, );

			if (dynamic_cast<CRenderer*>(Pair.second))
			{
				ComponentCnt[COM_RENDERER]++;
				continue;
			}
			if (dynamic_cast<CVIBuffer*>(Pair.second))
			{
				ComponentCnt[COM_VIBUFFER]++;
				continue;
			}
			if (dynamic_cast<CShader*>(Pair.second))
			{
				ComponentCnt[COM_SHADER]++;
				continue;
			}
			if (dynamic_cast<CTransform*>(Pair.second))
			{
				ComponentCnt[COM_TRANSFORM]++;
				continue;
			}
			if (dynamic_cast<CTexture*>(Pair.second))
			{
				ComponentCnt[COM_TEXTURE]++;
				continue;
			}
			if (dynamic_cast<CModel*>(Pair.second))
			{
				ComponentCnt[COM_MODEL]++;
				continue;
			}
			if (dynamic_cast<CCollider*>(Pair.second))
				ComponentCnt[COM_COLLIDER]++;
		}
	}

	ppComponentTag = new char**[COMPONENTTYPE_END];
	for (_int i = 0; i < (_int)COMPONENTTYPE_END; ++i)
	{
		ppComponentTag[i] = new char*[ComponentCnt[i]];
		for (_uint j = 0; j < ComponentCnt[i]; ++j)
		{
			ppComponentTag[i][j] = new char[64];

			if (j == 0)
				strcpy_s(ppComponentTag[i][j], 7, "<None>");
		}
	}

	char	szPrototypeTag[MAX_PATH] = "";
	_uint	ComCnt[COMPONENTTYPE_END] = { 1, 1, 1, 1, 1, 1, 1 };

	for (_uint i = 0; i < LEVEL_END + 1; ++i)
	{
		for (auto& Pair : m_mapProtoComponenets[i])
		{
			NULL_CHECK_RETURN(Pair.second, );

			ZeroMemory(szPrototypeTag, sizeof(char) * MAX_PATH);
			CGameUtils::wc2c(Pair.first.c_str(), szPrototypeTag);
			_int	iLength = (_int)strlen(szPrototypeTag) + 1;

			if (dynamic_cast<CRenderer*>(Pair.second))
			{
				ZeroMemory(ppComponentTag[COM_RENDERER][ComCnt[COM_RENDERER]], 64);
				sprintf_s(ppComponentTag[COM_RENDERER][ComCnt[COM_RENDERER]++], iLength, szPrototypeTag);
				continue;
			}
			if (dynamic_cast<CVIBuffer*>(Pair.second))
			{
				ZeroMemory(ppComponentTag[COM_VIBUFFER][ComCnt[COM_VIBUFFER]], 64);
				sprintf_s(ppComponentTag[COM_VIBUFFER][ComCnt[COM_VIBUFFER]++], iLength, szPrototypeTag);
				continue;
			}
			if (dynamic_cast<CShader*>(Pair.second))
			{
				ZeroMemory(ppComponentTag[COM_SHADER][ComCnt[COM_SHADER]], 64);
				sprintf_s(ppComponentTag[COM_SHADER][ComCnt[COM_SHADER]++], iLength, szPrototypeTag);
				continue;
			}
			if (dynamic_cast<CTransform*>(Pair.second))
			{
				ZeroMemory(ppComponentTag[COM_TRANSFORM][ComCnt[COM_TRANSFORM]], 64);
				sprintf_s(ppComponentTag[COM_TRANSFORM][ComCnt[COM_TRANSFORM]++], iLength, szPrototypeTag);
				continue;
			}
			if (dynamic_cast<CTexture*>(Pair.second))
			{
				ZeroMemory(ppComponentTag[COM_TEXTURE][ComCnt[COM_TEXTURE]], 64);
				sprintf_s(ppComponentTag[COM_TEXTURE][ComCnt[COM_TEXTURE]++], iLength, szPrototypeTag);
				continue;
			}
			if (dynamic_cast<CModel*>(Pair.second))
			{
				ZeroMemory(ppComponentTag[COM_MODEL][ComCnt[COM_MODEL]], 64);
				sprintf_s(ppComponentTag[COM_MODEL][ComCnt[COM_MODEL]++], iLength, szPrototypeTag);
				continue;
			}
			if(dynamic_cast<CCollider*>(Pair.second))
			{
				ZeroMemory(ppComponentTag[COM_COLLIDER][ComCnt[COM_COLLIDER]], 64);
				sprintf_s(ppComponentTag[COM_COLLIDER][ComCnt[COM_COLLIDER]++], iLength, szPrototypeTag);
			}
		}
	}

	for (_int i = 0; i < (_int)COMPONENTTYPE_END; ++i)
		pComponentCnt[i] = ComponentCnt[i];
}

_uint CImgui_ProtoMgr::FindPrototypeComponentLevel(const _tchar* pComponentTag)
{
	_uint	iLevelIndex = 1000;

	for (_uint i = 0; i < LEVEL_END + 1; ++i)
	{
		for (auto& Pair : m_mapProtoComponenets[i])
		{
			if (!lstrcmp(Pair.first.c_str(), pComponentTag))
			{
				iLevelIndex = i;
				return iLevelIndex;
			}
		}
	}

	return iLevelIndex;
}

CImgui_ProtoMgr* CImgui_ProtoMgr::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, void* pArg)
{
	CImgui_ProtoMgr*	pInstance = new CImgui_ProtoMgr(pDevice, pContext);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Create : CTool_PrototypeMgr");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CImgui_ProtoMgr::Free()
{
	Safe_Delete_Array(m_iSelectTextureCom);

	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);
}
