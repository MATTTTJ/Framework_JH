#include "stdafx.h"
#include "Level_Loading.h"
#include "Loader.h"
#include "GameInstance.h"
#include "Level_Logo.h"
#include "Level_GamePlay.h"

CLevel_Loading::CLevel_Loading(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CLevel(pDevice,pContext)
{
}

HRESULT CLevel_Loading::Initialize(LEVEL eNextLevelID)
{
	if(FAILED(CLevel::Initialize()))
	return E_FAIL;

	m_eNextLevelID = eNextLevelID;

	m_pLoader = CLoader::Create(m_pDevice, m_pContext, eNextLevelID);

	if ( nullptr == m_pLoader)
		return E_FAIL;

	return S_OK;
}

void CLevel_Loading::Tick(_double TimeDelta)
{
	__super::Tick(TimeDelta);
}

void CLevel_Loading::Late_Tick(_double TimeDelta)
{
	__super::Late_Tick(TimeDelta);

	CGameInstance*	pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	if (m_pLoader->IsFinished() == true)
	{
		if (GetKeyState(VK_RETURN) & 0x8000)
		{
			CLevel*		pLevel = nullptr;

			switch (m_eNextLevelID)
			{
			case LEVEL_LOGO:
				pLevel = CLevel_Logo::Create(m_pDevice, m_pContext);
				break;
			case LEVEL_GAMEPLAY:
				pLevel = CLevel_GamePlay::Create(m_pDevice, m_pContext);
				break;
			}

			if (nullptr == pLevel)
				return;

			if (FAILED(pGameInstance->Open_Level(m_eNextLevelID, pLevel)))
				return;
		}
	}

	Safe_Release(pGameInstance);
}

HRESULT CLevel_Loading::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	SetWindowText(g_hWnd, m_pLoader->Get_LoadingText());

	return S_OK;
}

CLevel_Loading* CLevel_Loading::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, LEVEL eNextLevelID)
{
	CLevel_Loading*		pInst = new CLevel_Loading(pDevice, pContext);

	if (FAILED(pInst->Initialize(eNextLevelID)))
	{
		MSG_BOX("Failed to Created : CLevel_Loading");
		Safe_Release(pInst);
	}
	return pInst;
}

void CLevel_Loading::Free()
{
	CLevel::Free();
	Safe_Release(m_pLoader);
}
