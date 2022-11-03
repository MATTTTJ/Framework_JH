#include "Level_Manager.h"
#include "Level.h"

IMPLEMENT_SINGLETON(CLevel_Manager)

CLevel_Manager::CLevel_Manager()
{
}

HRESULT CLevel_Manager::Open_Level(CLevel* pNewLevel)
{
	if (pNewLevel == nullptr)
		return E_FAIL;

	Safe_Release(m_pCurrentLevel);

	m_pCurrentLevel = pNewLevel;

	return S_OK;
}

void CLevel_Manager::Tick(_double TimeDelta)
{
	if (m_pCurrentLevel == nullptr)
		return;

	m_pCurrentLevel->Tick(TimeDelta);
}

void CLevel_Manager::Late_Tick(_double TimeDelta)
{
	if (m_pCurrentLevel == nullptr)
		return;

	m_pCurrentLevel->Late_Tick(TimeDelta);
}

HRESULT CLevel_Manager::Render()
{
	if (m_pCurrentLevel == nullptr)
		return E_FAIL;

	m_pCurrentLevel->Render();

	return S_OK;
}

void CLevel_Manager::Free()
{
	Safe_Release(m_pCurrentLevel);
}
