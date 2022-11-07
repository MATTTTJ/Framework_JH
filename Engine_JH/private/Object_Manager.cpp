#include "Object_Manager.h"

#include "GameObject.h"
#include "Layer.h"

IMPLEMENT_SINGLETON(CObject_Manager)

CObject_Manager::CObject_Manager()
{
}

HRESULT CObject_Manager::Reserve_Manager(_uint iNumLevels)
{
	// 레이어가 nullptr라면 실행
	if (nullptr != m_pLayers)
		return E_FAIL;

	// 레이어에 map 컨데이너를 레벨 배열에 맞게 동적할당함 
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

HRESULT CObject_Manager::Add_Prototype(const _tchar* pPrototypeTag, CGameObject* pPrototype)
{
	if (nullptr != Find_Prototype(pPrototypeTag))
		return E_FAIL;

	m_Prototypes.emplace(pPrototypeTag, pPrototype);

	return S_OK;
}

HRESULT CObject_Manager::Clone_GameObject(_uint iLevelIndex, const _tchar*  pLayerTag, const _tchar* pPrototypeTag,
                                          void* pArg)
{
	CGameObject*	pPrototype = Find_Prototype(pPrototypeTag);

	if (nullptr == pPrototype)
		return E_FAIL;

	CGameObject*		pGameObject = pPrototype->Clone(pArg);

	CLayer*				pLayer = Find_Layer(iLevelIndex,pLayerTag);

	if (pLayer == nullptr)
	{
		pLayer = CLayer::Create();
		if (pLayer == nullptr)
			return E_FAIL;
	
		if (FAILED(pLayer->Add_GameObject(pGameObject)))
			return E_FAIL;
	
		m_pLayers[iLevelIndex].emplace(pLayerTag, pLayer);
	}
	else
		pLayer->Add_GameObject(pPrototype);

	return S_OK;
}

void CObject_Manager::Tick(_double TimeDelta)
{
	for(_uint i = 0; i < m_iNumLevels; ++i)
	{
		for(auto& Pair : m_pLayers[i])
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

CGameObject* CObject_Manager::Find_Prototype(const _tchar* pPrototypeTag)
{
	auto	iter = find_if(m_Prototypes.begin(), m_Prototypes.end(), CTag_Finder(pPrototypeTag));
	if (iter == m_Prototypes.end())
		return nullptr;

	return iter->second;
}

CLayer* CObject_Manager::Find_Layer(_uint iLevelIndex, const _tchar* pLayer)
{
	auto	iter = find_if(m_pLayers[iLevelIndex].begin(), m_pLayers[iLevelIndex].end(), CTag_Finder(pLayer));
	if (iter == m_pLayers[iLevelIndex].end())
		return nullptr;
	
	return iter->second;
}

void CObject_Manager::Free()
{
	for(_uint i = 0; i<m_iNumLevels; ++i)
	{
		for(auto& Pair : m_pLayers[i])
			Safe_Release(Pair.second);

		m_pLayers[i].clear();
	}

	Safe_Delete_Array(m_pLayers);

	for (auto& Pair : m_Prototypes)
		Safe_Release(Pair.second);

	m_Prototypes.clear();
}
