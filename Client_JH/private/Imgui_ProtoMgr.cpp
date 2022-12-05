#include "stdafx.h"
#include "..\public\Imgui_ProtoMgr.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "GameUtils.h"
#include "CustomObject.h"

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

	return S_OK;
}

void CImgui_ProtoMgr::Imgui_RenderWindow()
{
	ImGui::BeginTabBar("Prototype Manager");
	Component_Editor();
	GameObject_Editor();
	ImGui::EndTabBar();
}

void CImgui_ProtoMgr::Component_Editor()
{
	static _int iSelectLevel = LEVEL_PUBLIC;
	static _int iSelectComponent = -1;
	static _bool bAddPrototype = false;

	if (ImGui::BeginTabItem("Component"))
	{

		ImGui::BulletText("Prototype Component List");
		IMGUI_LEFT_LABEL(ImGui::Combo, "Select Level", &iSelectLevel, m_pLevelName, LEVEL_PUBLIC + 1);

		char**	ppPrototypeComponentTag = new char*[m_mapProtoComponenets[iSelectLevel].size()];
		_uint i = 0;
		for (auto Pair : m_mapProtoComponenets[iSelectLevel])
		{
			ppPrototypeComponentTag[i] = new char[Pair.first.length() + 1];
			CGameUtils::wc2c(Pair.first.c_str(), ppPrototypeComponentTag[i]);
			++i;
		}

		ImGui::ListBox(" ", &iSelectComponent, ppPrototypeComponentTag, (_int)m_mapProtoComponenets[iSelectLevel].size());

		if (ImGui::Button("Add"))
			bAddPrototype = true;
		ImGui::SameLine();
		if (ImGui::Button("Save"))
		{

		}
		ImGui::SameLine();
		if (ImGui::Button("Load"))
		{

		}

		if (bAddPrototype)
		{
			ImGui::Separator();

			iSelectComponent = -1;
			static COMPONENTTYPE	eComponentType = COM_RENDERER;
			static COMPONENTTYPE	eLastComponentType = eComponentType;

			char*	szTypeName[COMPONENTTYPE_END] = { "Renderer", "VIBuffer", "Shader", "Transform", "Texture", "Model" };
			static _uint	iTargetLevel = 3;
			static _uint	iLastTargetLevel = iTargetLevel;

			IMGUI_LEFT_LABEL(ImGui::Combo, "Select Type", (_int*)&eComponentType, szTypeName, COMPONENTTYPE_END);
			IMGUI_LEFT_LABEL(ImGui::Combo, "Select Level", (_int*)&iTargetLevel, m_pLevelName, LEVEL_END + 1);

			ImGui::NewLine();
			ImGui::BulletText("Options");

			// For All Component
			static char szPrototypeTag[MAX_PATH] = "";
			static char szFilePath[MAX_PATH] = "";
			const char* Filter = "Image files (*.png *.dds *.jpg *.jpeg *.gif *.tga){.png,.dds,.jpg,.jpeg,.gif,.tga}";

			// For Shader
			static _int iSelectDeclaration = 0;
			char*		szDeclarationType[4] = { "VTXTEX", "VTXNORTEX", "VTXMODEL", "VTXANIMMODEL" };
			static _int iNumElements = 0;

			// For Texture
			static _uint	iTextureCnt = 1;

			// For Model
			static CModel::MODELTYPE	eModelType = CModel::MODEL_NONANIM;
			char*						szModelType[2] = { "Non-Animation", "Animation" };

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
				ImGui::Text("There is no Options for this Component Type");
				break;
			case COM_VIBUFFER:
				ImGui::Text("There is no Options for this Component Type");
				break;
			case COM_SHADER:
				// Input Prototype Tag
				IMGUI_LEFT_LABEL(ImGui::InputText, "Component Tag", szPrototypeTag, sizeof(char) * MAX_PATH);

				// Input File Path
				IMGUI_LEFT_LABEL(ImGui::InputTextWithHint, "File Path", "Input Path here or Choose from FileDialog.", szFilePath, sizeof(char) * MAX_PATH);
				ImGui::SameLine();
				if (ImGui::SmallButton("Open"))
					ImGuiFileDialog::Instance()->OpenDialog("Choose HLSL", "Choose .hlsl", ".hlsl", "../Bin/ShaderFiles", ".hlsl", 1, nullptr, ImGuiFileDialogFlags_Modal);
				IMGUI_LEFT_LABEL(ImGui::Combo, "Select Declaration", &iSelectDeclaration, szDeclarationType, 4);

				if (iSelectDeclaration == 0)
					iNumElements = 2;
				else if (iSelectDeclaration == 1)
					iNumElements = 3;
				else if (iSelectDeclaration == 2)
					iNumElements = 4;
				else if (iSelectDeclaration == 3)
					iNumElements = 6;

				IMGUI_LEFT_LABEL(ImGui::InputInt, "Elements Count", &iNumElements, 0, 0, ImGuiInputTextFlags_ReadOnly);

				if (ImGuiFileDialog::Instance()->Display("Choose HLSL"))
				{
					if (ImGuiFileDialog::Instance()->IsOk())
					{
						string tmp = ImGuiFileDialog::Instance()->GetFilePathName();
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
			}
			if (ImGui::Button("Create"))
			{
				for (size_t i = 0; i < m_mapProtoComponenets[iSelectLevel].size(); ++i)
					Safe_Delete_Array(ppPrototypeComponentTag[i]);
				Safe_Delete_Array(ppPrototypeComponentTag);

				switch (eComponentType)
				{
				case COM_RENDERER:
					break;

				case COM_VIBUFFER:
					break;

				case COM_SHADER:
				{
					_tchar	wszPrototypeTag[MAX_PATH] = L"";
					_tchar	wszFilePath[MAX_PATH] = L"";
					CGameUtils::ctwc(szPrototypeTag, wszPrototypeTag);
					CGameUtils::ctwc(szFilePath, wszFilePath);

					if (iSelectDeclaration == 0)
						CGameInstance::GetInstance()->Add_Prototype(iTargetLevel, wstring(wszPrototypeTag), CShader::Create(m_pDevice, m_pContext, wstring(wszFilePath), VTXTEX_DECLARATION::Elements, VTXTEX_DECLARATION::iNumElements));
					else if (iSelectDeclaration == 1)
						CGameInstance::GetInstance()->Add_Prototype(iTargetLevel, wstring(wszPrototypeTag), CShader::Create(m_pDevice, m_pContext, wstring(wszFilePath), VTXNORTEX_DECLARATION::Elements, VTXNORTEX_DECLARATION::iNumElements));
					else if (iSelectDeclaration == 2)
						CGameInstance::GetInstance()->Add_Prototype(iTargetLevel, wstring(wszPrototypeTag), CShader::Create(m_pDevice, m_pContext, wstring(wszFilePath), VTXMODEL_DECLARATION::Elements, VTXMODEL_DECLARATION::iNumElements));
					else if (iSelectDeclaration == 3)
						CGameInstance::GetInstance()->Add_Prototype(iTargetLevel, wstring(wszPrototypeTag), CShader::Create(m_pDevice, m_pContext, wstring(wszFilePath), VTXANIMMODEL_DECLARATION::Elements, VTXANIMMODEL_DECLARATION::iNumElements));
					break;
				}
				case COM_TRANSFORM:
					ImGui::Text("I am %d", COM_TRANSFORM);
					break;

				case COM_TEXTURE:
				{
					_tchar	wszPrototypeTag[MAX_PATH] = L"";
					_tchar wszFilePath[MAX_PATH] = L"";
					CGameUtils::ctwc(szPrototypeTag, wszPrototypeTag);
					CGameUtils::ctwc(szFilePath, wszFilePath);

					CGameInstance::GetInstance()->Add_Prototype(iTargetLevel, wstring(wszPrototypeTag), CTexture::Create(m_pDevice, m_pContext, wstring(wszFilePath), iTextureCnt));

					break;
				}
				case COM_MODEL:
				{
					_tchar	wszPrototypeTag[MAX_PATH] = L"";
					CGameUtils::ctwc(szPrototypeTag, wszPrototypeTag);
					CGameInstance::GetInstance()->Add_Prototype(iTargetLevel, wstring(wszPrototypeTag), CModel::Create(m_pDevice, m_pContext, eModelType, szFilePath));
					break;
				}
				}
				ImGui::EndTabItem();

				bAddPrototype = false;
				strcpy_s(szPrototypeTag, "");
				strcpy_s(szFilePath, "");
				iSelectDeclaration = 0;
				iNumElements = 0;
				iTextureCnt = 1;
				eModelType = CModel::MODEL_NONANIM;

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
			}
			ImGui::Separator();
		}

		if (iSelectComponent != -1)
		{
			ImGui::Separator();

			CComponent*		pComponent = nullptr;

			for (auto Pair : m_mapProtoComponenets[iSelectLevel])
			{
				_tchar	wszComponentTag[MAX_PATH] = L"";
				CGameUtils::ctwc(ppPrototypeComponentTag[iSelectComponent], wszComponentTag);

				if (!lstrcmp(Pair.first.c_str(), wszComponentTag))
				{
					pComponent = Pair.second;
					break;
				}
			}

			if (nullptr != pComponent)
				pComponent->Imgui_RenderProperty();

			switch (CheckComponentType(iSelectLevel, ppPrototypeComponentTag[iSelectComponent]))
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

			case COMPONENTTYPE_END:
				return;
			}
		}

		for (size_t i = 0; i < m_mapProtoComponenets[iSelectLevel].size(); ++i)
			Safe_Delete_Array(ppPrototypeComponentTag[i]);
		Safe_Delete_Array(ppPrototypeComponentTag);

		ImGui::EndTabItem();
	}
}

void CImgui_ProtoMgr::GameObject_Editor()
{
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

	return COMPONENTTYPE_END;
}

void CImgui_ProtoMgr::SortComponentByType(char***& ppComponentTag, _uint* pComponentCnt)
{
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
