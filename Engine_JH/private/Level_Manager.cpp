#include "Level_Manager.h"

#include "GameInstance.h"
#include "Level.h"

IMPLEMENT_SINGLETON(CLevel_Manager)

CLevel_Manager::CLevel_Manager()
{
}

HRESULT CLevel_Manager::Open_Level(_uint iLevelIndex, CLevel* pNewLevel)
{
	NULL_CHECK_RETURN(pNewLevel, E_FAIL);

	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	if (nullptr != m_pCurrentLevel)
		pGameInstance->Clear_Level(m_iLevelIndex);

	Safe_Release(m_pCurrentLevel);

	m_pCurrentLevel = pNewLevel;
	m_iLevelIndex = iLevelIndex;

	Safe_Release(pGameInstance);

	return S_OK;
}

void CLevel_Manager::Tick(_double TimeDelta)
{
	NULL_CHECK_RETURN(m_pCurrentLevel, );

	m_pCurrentLevel->Tick(TimeDelta);
}

void CLevel_Manager::Late_Tick(_double TimeDelta)
{
	NULL_CHECK(m_pCurrentLevel);

	m_pCurrentLevel->Late_Tick(TimeDelta);
}

HRESULT CLevel_Manager::Render()
{
	NULL_CHECK_RETURN(m_pCurrentLevel, E_FAIL);

	return m_pCurrentLevel->Render();
}

void CLevel_Manager::Free()
{
	Safe_Release(m_pCurrentLevel);
}
