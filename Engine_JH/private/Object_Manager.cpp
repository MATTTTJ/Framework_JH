#include "Object_Manager.h"

#include "GameObject.h"

IMPLEMENT_SINGLETON(CObject_Manager)

CObject_Manager::CObject_Manager()
{
}

HRESULT CObject_Manager::Reserve_Manager(_uint iNumLevels)
{
	// 레이어가 nullptr라면 실행
	if (m_pLayers != nullptr)
		return E_FAIL;

	// 레이어에 map 컨데이너를 레벨 배열에 맞게 동적할당함 
	m_pLayers = new LAYERS[iNumLevels];

	m_iNumLevels = iNumLevels;

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

	CLayer*	pLayer = Find_Layer(pLayerTag);

	// if (pLayer == nullptr)
	// {
	// 	pLayer = CLayer::Create();
	// 	if (pLayer == nullptr)
	// 		return E_FAIL;
	//
	// 	if (FAILED(pLayer->Add_GameObject(pGameObject)))
	// 		return E_FAIL;
	//
	// 	m_pLayers[iLevelIndex].emplace(pstLayerTag, pLayer);
	// }
	// else
	// 	pLayer->Add_GameObject(pPrototype);

	return S_OK;
}

CGameObject* CObject_Manager::Find_Prototype(const _tchar* pPrototypeTag)
{
	auto	iter = find_if(m_Prototypes.begin(), m_Prototypes.end(), CTag_Finder(pPrototypeTag));
	if (iter == m_Prototypes.end())
		return nullptr;

	return iter->second;
}

CLayer* CObject_Manager::Find_Layer(const _tchar* pLayer)
{
	auto	iter = find_if(m_pLayers->begin(), m_pLayers->end(), CTag_Finder(pLayer));
	if (iter == m_pLayers->end())
		return nullptr;
	
	return iter->second;
}

void CObject_Manager::Free()
{
}
