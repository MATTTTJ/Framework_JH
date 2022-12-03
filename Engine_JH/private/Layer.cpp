#include "..\public\Layer.h"

#include "GameObject.h"


CLayer::CLayer()
{
}

HRESULT CLayer::Initialize()
{
	return S_OK;
}

void CLayer::Tick(_double TimeDelta)
{
	for(auto& pGameObject : m_GameObjectList)
	{
		if (nullptr != pGameObject)
			pGameObject->Tick(TimeDelta);
	}
}

void CLayer::Late_Tick(_double TimeDelta)
{
	for (auto& pGameObject : m_GameObjectList)
	{
		if (nullptr != pGameObject)
			pGameObject->Late_Tick(TimeDelta);
	}
}



HRESULT CLayer::Add_GameObject(CGameObject* pGameObject)
{
	NULL_CHECK_RETURN(pGameObject, E_FAIL)
	// 오브젝트 매니저에서 어떤 레이어에 넣을건지 동적할당후에 맞는 레벨 레이어에 게임오브젝트 보관
	m_GameObjectList.push_back(pGameObject);

	return S_OK;
}


CLayer* CLayer::Create()
{
	CLayer*		pInst = new CLayer;

	if(FAILED(pInst->Initialize()))
	{
		MSG_BOX("Failed to Create : CLayer");
		Safe_Release(pInst);
	}
	return pInst;
}

void CLayer::Free()
{
	for (auto& pGameObject : m_GameObjectList)
		Safe_Release(pGameObject);

	m_GameObjectList.clear();
}
