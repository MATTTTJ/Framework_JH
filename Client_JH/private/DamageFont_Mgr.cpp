#include "stdafx.h"
#include "..\public\DamageFont_Mgr.h"
#include "GameInstance.h"

IMPLEMENT_SINGLETON(CDamageFont_Mgr);

CDamageFont_Mgr::CDamageFont_Mgr()
{
}

void CDamageFont_Mgr::Tick()
{
}

HRESULT CDamageFont_Mgr::Add_DamageFont(_uint iLevelIndex, const wstring pLayerTag,
	void* tFontDesc)
{

	if(nullptr != tFontDesc)
	{
		CGameObject* pObject = nullptr;
		pObject = CGameInstance::GetInstance()->Clone_GameObjectReturnPtr(iLevelIndex, pLayerTag, L"Prototype_GameObject_Player_DamageFont", tFontDesc);
		NULL_CHECK_RETURN(pObject, E_FAIL);
		// m_vecFont.push_back(pObject);
	}


	return S_OK;
}

void CDamageFont_Mgr::Free()
{
	// for(auto& iter : m_vecFont)
	// {
	// 	Safe_Release(iter);
	// }
	// m_vecFont.clear();
}
