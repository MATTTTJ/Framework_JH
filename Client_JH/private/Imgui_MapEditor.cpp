#include "stdafx.h"
#include "Imgui_MapEditor.h"
#include "Layer.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "GameUtils.h"


CImgui_MapEditor::CImgui_MapEditor()
{
}

HRESULT CImgui_MapEditor::Initialize(void* pArg)
{
	m_szWindowName = "Map Editor";

	m_mapPrototypes = CGameInstance::GetInstance()->Get_Prototypes();
	m_iProtypeCnt = (_uint)m_mapPrototypes->size();

	m_iCurLevel = CGameInstance::GetInstance()->Get_CurLevelIndex();
	m_mapLayers = CGameInstance::GetInstance()->Get_Layers(m_iCurLevel);

	for(auto Pair : *m_mapPrototypes)
	{
		if (Pair.second->Get_HasModel())
			m_mapPrototypes->emplace(Pair.first, Pair.second);
	}

	return S_OK;
}

void CImgui_MapEditor::Imgui_RenderWindow()
{
	CheckCurrentLevel();
	CheckNewPrototype();

	static	_int iSelectObject = -1;
	size_t i2 = m_mapPrototypeModels.size();

	if(m_mapPrototypeModels.size())
	{
		char** ppProtoModleTag = new char*[m_mapPrototypeModels.size()];
		_uint		i = 0;
		for(auto Pair : m_mapPrototypeModels)
		{
			ppProtoModleTag[i] = new char[Pair.first.length() + 1];
			CGameUtils::wc2c(Pair.first.c_str(), ppProtoModleTag[i]);
			i++;
		}

		ImGui::BulletText("Prototype Model List");
		ImGui::ListBox("Prototype Model List", &iSelectObject, ppProtoModleTag, (_int)m_mapPrototypeModels.size());

		if (iSelectObject != -1)
		{
			static _int iSelectLayer = 0;
			char**		ppLayerTags = new char*[m_mapLayers->size()];
			_uint j = 0;
			for (auto Pair : *m_mapLayers)
			{
				ppLayerTags[j] = new char[Pair.first.length() + 1];
				CGameUtils::wc2c(Pair.first.c_str(), ppLayerTags[j]);
				j++;
			}


			IMGUI_LEFT_LABEL(ImGui::Combo, "Target Layer", &iSelectObject, ppLayerTags, (_int)m_mapLayers->size());

			ImGui::SameLine();

			if(ImGui::Button("Clone"))
			{
				_tchar	wszBuff[MAX_PATH] = L"";
				CGameUtils::ctwc(ppProtoModleTag[iSelectObject], wszBuff);
				wstring wstrPrototypeTag = wszBuff;
				CGameUtils::ctwc(ppLayerTags[iSelectLayer], wszBuff);
				wstring	wstrLayerTag = wszBuff;

				CGameInstance::GetInstance()->Clone_GameObject(m_iCurLevel, wstrLayerTag, wstrPrototypeTag);
			}
		}
	}
}

void CImgui_MapEditor::CheckNewPrototype()
{
	if (m_iProtypeCnt == m_mapPrototypes->size())
		return;

	m_iProtypeCnt = (_uint)m_mapPrototypes->size();

	for(auto Pair : *m_mapPrototypes)
	{
		if (!Pair.second->Get_HasModel())
			continue;
		
		auto iter = find_if(m_mapPrototypeModels.begin(), m_mapPrototypeModels.end(), CTag_Finder(Pair.first));

		if (iter == m_mapPrototypeModels.end())
			m_mapPrototypeModels.emplace(Pair.first, Pair.second);
	}
}

void CImgui_MapEditor::CheckCurrentLevel()
{
	if(m_iCurLevel != CGameInstance::GetInstance()->Get_CurLevelIndex())
	{
		m_iCurLevel = CGameInstance::GetInstance()->Get_CurLevelIndex();
		m_mapLayers = CGameInstance::GetInstance()->Get_Layers(m_iCurLevel);
	}
}

CImgui_MapEditor* CImgui_MapEditor::Create(void* pArg)
{
	CImgui_MapEditor*	pInstance = new CImgui_MapEditor();

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Create : CImgui_MapEditor");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CImgui_MapEditor::Free()
{
	CImguiObject::Free();

	m_mapPrototypeModels.clear();
}
