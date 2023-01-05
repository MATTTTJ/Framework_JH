#include "stdafx.h"
#include "..\public\Layer.h"

#include "GameObject.h"


CLayer::CLayer()
{
}

CComponent* CLayer::Get_ComponentPtr(const wstring& pComponentTag, _uint iIndex)
{
	if (iIndex >= m_GameObjectList.size())
		return nullptr;

	auto iter = m_GameObjectList.begin();

	for (_uint i = 0; i < iIndex; ++i)
		++iter;

	if (iter == m_GameObjectList.end())
		return nullptr;

	return (*iter)->Find_Component(pComponentTag);
}

HRESULT CLayer::Initialize()
{
	return S_OK;
}

void CLayer::Tick(_double TimeDelta)
{


	for(auto iter = m_GameObjectList.begin(); iter != m_GameObjectList.end();)
	{
		if (true == (*iter)->Check_Dead())
		{
			(*iter)->Set_Dead(false);

			Safe_Release(*iter);
			iter = m_GameObjectList.erase(iter);
		}
		else
		{
			++iter;
		}
	}

	for (auto& pGameObject : m_GameObjectList)
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
