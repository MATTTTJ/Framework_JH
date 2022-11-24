#include "..\public\Component_Manager.h"
#include "Component.h"

IMPLEMENT_SINGLETON(CComponent_Manager)

CComponent_Manager::CComponent_Manager()
{
}

HRESULT CComponent_Manager::Reserve_Manager(_uint iNumLevels)
{
	if (nullptr != m_pPrototypes)
		return E_FAIL;

	m_pPrototypes = new PROTOTYPES[iNumLevels];

	m_iNumLevels = iNumLevels;

	return S_OK;
}

HRESULT CComponent_Manager::Add_Prototype(_uint iLevelIndex, const _tchar* pPrototypeTag, CComponent* pPrototype)
{
	if (nullptr == m_pPrototypes ||
		iLevelIndex >= m_iNumLevels)
		return E_FAIL;

	if (nullptr != Find_Prototype(iLevelIndex, pPrototypeTag))
	{
		// assert(!"Already have Prototype in ComponentMgr");
		return E_FAIL;
	}

	m_pPrototypes[iLevelIndex].emplace(pPrototypeTag, pPrototype);

	return S_OK;
}


CComponent* CComponent_Manager::Clone_Component(_uint iLevelIndex, const _tchar* pPrototypeTag, void* pArg)
{
	CComponent*		pPrototype = Find_Prototype(iLevelIndex, pPrototypeTag);
	if (nullptr == pPrototype)
	{
		assert(!"Have no Prototype. You must be Add_Prototype");
		return nullptr;
	}

	CComponent*		pComponent = pPrototype->Clone(pArg);
	if (nullptr == pComponent)
	{
		assert(!"Failed to Clone in ComponentMgr");
		return nullptr;
	}
	return pComponent;
}

CComponent* CComponent_Manager::Find_Prototype(_uint iLevelIndex, const _tchar* pPrototypeTag)
{
	if (iLevelIndex >= m_iNumLevels)
	{
		assert(!"Worng Level Index");
		return nullptr;
	}
	auto iter = find_if(m_pPrototypes[iLevelIndex].begin(), m_pPrototypes[iLevelIndex].end(), CTag_Finder(pPrototypeTag));

	if (iter == m_pPrototypes[iLevelIndex].end())
		return nullptr;

	return iter->second;
}

void CComponent_Manager::Free()
{
	for(_uint i = 0; i < m_iNumLevels; ++i)
	{
		for (auto& Pair : m_pPrototypes[i])
			Safe_Release(Pair.second);
		m_pPrototypes[i].clear();
	}

	Safe_Delete_Array(m_pPrototypes);
}


