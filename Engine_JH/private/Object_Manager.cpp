#include "stdafx.h"
#include "..\public\Object_Manager.h"
#include "Layer.h"
#include "GameObject.h"
#include "GameUtils.h"

IMPLEMENT_SINGLETON(CObject_Manager)

CObject_Manager::CObject_Manager()
{
}

CComponent* CObject_Manager::Get_ComponentPtr(_uint iLevelIndex, const wstring& wstrLayerTag, const wstring& pComponentTag,
	_uint iIndex)
{
	if (iLevelIndex >= m_iNumLevels)
		return nullptr;

	CLayer*		pLayer = Find_Layer(iLevelIndex, wstrLayerTag);
	NULL_CHECK_RETURN(pLayer, nullptr);

	return pLayer->Get_ComponentPtr(pComponentTag, iIndex);
}

list<CGameObject*>* CObject_Manager::Get_CloneObjectList(_uint iLevelIndex, const wstring& wstrLayerTag)
{
	CLayer*		pLayer = nullptr;

	pLayer = Find_Layer(iLevelIndex, wstrLayerTag);

	if (pLayer == nullptr)
	{
		return nullptr;
	}
	else
	{
		return pLayer->Get_GameObject();
	}
}

HRESULT CObject_Manager::Reserve_Manager(_uint iNumLevels)
{
	if (nullptr != m_pLayers)
		return E_FAIL;

	m_pLayers = new LAYERS[iNumLevels];

	m_iNumLevels = iNumLevels;

	return S_OK;
}

HRESULT CObject_Manager::Clear(_uint iLevelIndex)
{
	if (iLevelIndex >= m_iNumLevels)
		return E_FAIL;

	for (auto& Pair : m_pLayers[iLevelIndex])
		Safe_Release(Pair.second);

	m_pLayers[iLevelIndex].clear();

	return S_OK;
}

HRESULT CObject_Manager::Add_Layer(_uint iLevelIndex, const wstring& wstrLayerTag)
{
	CLayer*		pLayer = Find_Layer(iLevelIndex, wstrLayerTag);

	if(nullptr == pLayer)
	{
		pLayer = CLayer::Create();
		NULL_CHECK_RETURN(pLayer, E_FAIL);

		m_pLayers[iLevelIndex].emplace(wstrLayerTag, pLayer);
	}

	return S_OK;
}

HRESULT CObject_Manager::Add_Prototype(const wstring& pPrototypeTag, CGameObject * pPrototype)
{
	if (nullptr != Find_Prototype(pPrototypeTag))
	{
		Safe_Release(pPrototype);
		return S_OK;
	}
	m_mapPrototypes.emplace(pPrototypeTag, pPrototype);

	return S_OK;
}

HRESULT CObject_Manager::Add_AnimObject(CGameObject* pAnimObject)
{
	NULL_CHECK_RETURN(pAnimObject, E_FAIL);
	m_vecAnimObjects.push_back(pAnimObject);
	return S_OK;
}

HRESULT CObject_Manager::Clone_GameObject(_uint iLevelIndex, const wstring& pLayerTag, const wstring& wstrPrototypeTag, void * pArg)
{
	CGameObject*		pPrototype = Find_Prototype(wstrPrototypeTag);
	NULL_CHECK_RETURN(pPrototype, E_FAIL);

	CGameObject*		pGameObject = pPrototype->Clone(wstrPrototypeTag, pArg);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);


	CLayer*		pLayer = Find_Layer(iLevelIndex, pLayerTag);

	if (nullptr == pLayer)
	{
		pLayer = CLayer::Create();
		NULL_CHECK_RETURN(pLayer, E_FAIL);
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(pGameObject), E_FAIL);

		m_pLayers[iLevelIndex].emplace(pLayerTag, pLayer);
	}
	else
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(pGameObject), E_FAIL);

	return S_OK;
}

HRESULT CObject_Manager::Clone_GameObject(_uint iLevelIndex, const wstring& pLayerTag, const wstring& wstrPrototypeTag,
	_float4x4 WorldMatrix, void* pArg)
{
	CGameObject*		pPrototypeObject = Find_Prototype(wstrPrototypeTag);
	NULL_CHECK_RETURN(pPrototypeObject, E_FAIL);

	CGameObject*		pCloneObject = pPrototypeObject->Clone(wstrPrototypeTag, pArg);

	pCloneObject->Set_WorldMatrix(WorldMatrix);

	CLayer*				pLayer = Find_Layer(iLevelIndex, pLayerTag);

	if (nullptr == pLayer)
	{
		pLayer = CLayer::Create();

		NULL_CHECK_RETURN(pLayer, E_FAIL);
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(pCloneObject), E_FAIL);

		m_pLayers[iLevelIndex].emplace(pLayerTag, pLayer);
	}
	else
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(pCloneObject), E_FAIL);

	return S_OK;
}

CGameObject* CObject_Manager::Clone_GameObject(const wstring& wstrPrototypeTag, void* pArg)
{
	CGameObject*		pPrototype = Find_Prototype(wstrPrototypeTag);
	if (nullptr == pPrototype)
		return nullptr;

	CGameObject*		pGameObject = pPrototype->Clone(wstrPrototypeTag, pArg);
	if (nullptr == pGameObject)
		return nullptr;

	// 필요하면 Add_GameObject로 게임오브젝트 리스트에 넣어서 사용가능함, 레이어태그도 당연히 필요하겠지

	return pGameObject;
}

CGameObject* CObject_Manager::Clone_GameObjectReturnPtr_M(_uint iLevelIndex, const wstring& wstrLayerTag,
	const wstring& wstrPrototypeTag, _float4x4 matWorld , void* pArg)
{
	CGameObject*	pPrototype = Find_Prototype(wstrPrototypeTag);
	NULL_CHECK_RETURN(pPrototype, nullptr);

	CGameObject*	pCloneObject = pPrototype->Clone(wstrPrototypeTag, pArg);
	NULL_CHECK_RETURN(pCloneObject, nullptr);

	pCloneObject->Set_WorldMatrix(matWorld);

	CLayer*			pLayer = Find_Layer(iLevelIndex, wstrLayerTag);

	if (nullptr == pLayer)
	{
		pLayer = CLayer::Create();

		NULL_CHECK_RETURN(pLayer, nullptr);
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(pCloneObject), nullptr);

		m_pLayers[iLevelIndex].emplace(wstrLayerTag, pLayer);
	}
	else
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(pCloneObject), nullptr);

	return pCloneObject;
}

CGameObject* CObject_Manager::Clone_GameObjectReturnPtr(_uint iLevelIndex, const wstring& wstrLayerTag,
	const wstring& wstrPrototypeTag,void* pArg)
{
	CGameObject*	pPrototype = Find_Prototype(wstrPrototypeTag);
	NULL_CHECK_RETURN(pPrototype, nullptr);

	CGameObject*	pCloneObject = pPrototype->Clone(wstrPrototypeTag, pArg);
	NULL_CHECK_RETURN(pCloneObject, nullptr);

	CLayer*			pLayer = Find_Layer(iLevelIndex, wstrLayerTag);

	if (nullptr == pLayer)
	{
		pLayer = CLayer::Create();

		NULL_CHECK_RETURN(pLayer, nullptr);
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(pCloneObject), nullptr);

		m_pLayers[iLevelIndex].emplace(wstrLayerTag, pLayer);
	}
	else
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(pCloneObject), nullptr);

	return pCloneObject;
}

void CObject_Manager::Tick(_double TimeDelta)
{
	for (_uint i = 0; i < m_iNumLevels; ++i)
	{
		for (auto& Pair : m_pLayers[i])
		{
			if (nullptr != Pair.second)
				Pair.second->Tick(TimeDelta);
		}
	}
}

void CObject_Manager::Late_Tick(_double TimeDelta)
{
	for (_uint i = 0; i < m_iNumLevels; ++i)
	{
		for (auto& Pair : m_pLayers[i])
		{
			if (nullptr != Pair.second)
				Pair.second->Late_Tick(TimeDelta);
		}
	}
}

void CObject_Manager::Imgui_ProtoViewer(const _tchar*& szSelectedProto)
{
}

void CObject_Manager::Imgui_ObjectViewer(_uint iLevel, CGameObject*& pSelectedObject)
{
	if (m_iNumLevels <= iLevel)
		return;

	_bool	bFound = false;

	const LAYERS&	targetLevel = m_pLayers[iLevel];

	if (ImGui::TreeNode("Object Viewer"))
	{
		for (auto& Pair : targetLevel)
		{
			char szLayerTag[MAX_PATH];
			CGameUtils::wc2c(Pair.first.c_str(), szLayerTag);
			if (ImGui::TreeNode(szLayerTag))
			{
				if (ImGui::BeginListBox("##", ImVec2(250.f, 50.f)))
				{
					for (auto& Obj : *Pair.second->Get_GameObject())
					{
						const _bool bSelected = pSelectedObject == Obj;
						if (bSelected)
						{
							ImGui::SetItemDefaultFocus();
							bFound = true;
						}
						if (ImGui::Selectable(typeid(*Obj).name(), bSelected))
						{
							pSelectedObject = Obj;
							bFound = true;
						}
					}
					ImGui::EndListBox();
				}
				ImGui::TreePop();
			}
		}
		ImGui::TreePop();
	}

	if (bFound == false)
		pSelectedObject = nullptr;
}

CGameObject * CObject_Manager::Find_Prototype(const wstring& pPrototypeTag)
{
	auto	iter = find_if(m_mapPrototypes.begin(), m_mapPrototypes.end(), CTag_Finder(pPrototypeTag));
	if (iter == m_mapPrototypes.end())
		return nullptr;

	return iter->second;
}

CLayer * CObject_Manager::Find_Layer(_uint iLevelIndex, const wstring& pLayerTag)
{
	auto	iter = find_if(m_pLayers[iLevelIndex].begin(), m_pLayers[iLevelIndex].end(), CTag_Finder(pLayerTag));
	if (iter == m_pLayers[iLevelIndex].end())
		return nullptr;

	return iter->second;
}

void CObject_Manager::Free()
{
	for (_uint i = 0; i < m_iNumLevels; ++i)
	{
		for (auto& Pair : m_pLayers[i])
			Safe_Release(Pair.second);

		m_pLayers[i].clear();
	}

	Safe_Delete_Array(m_pLayers);


	for (auto& Pair : m_mapPrototypes)
		Safe_Release(Pair.second);

	m_mapPrototypes.clear();


}

