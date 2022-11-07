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
	for(auto& pGameObject : m_GameObjects)
	{
		if (nullptr != pGameObject)
			pGameObject->Tick(TimeDelta);
	}
}

void CLayer::Late_Tick(_double TimeDelta)
{
	for (auto& pGameObject : m_GameObjects)
	{
		if (nullptr != pGameObject)
			pGameObject->Late_Tick(TimeDelta);
	}
}



HRESULT CLayer::Add_GameObject(CGameObject* pGameObject)
{
	if (nullptr == pGameObject)
		return E_FAIL;

	m_GameObjects.push_back(pGameObject);

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
	for (auto& pGameObject : m_GameObjects)
		Safe_Release(pGameObject);

	m_GameObjects.clear();
}
