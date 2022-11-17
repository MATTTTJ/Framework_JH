#include "GameObject.h"

#include "GameInstance.h"

CGameObject::CGameObject(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: m_pDevice(pDevice)
	, m_pContext(pContext)
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
}

CGameObject::CGameObject(const CGameObject& rhs)
	: m_pDevice(rhs.m_pDevice)
	, m_pContext(rhs.m_pContext)
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
}

HRESULT CGameObject::Initialize_Prototype()
{

	return S_OK;
}

HRESULT CGameObject::Initialize_Clone(void* pArg)
{
	GAMEOBJECTDESC GameObjectDesc;
	ZeroMemory(&GameObjectDesc, sizeof(GAMEOBJECTDESC));

	if (nullptr != pArg)
		GameObjectDesc = *(GAMEOBJECTDESC*)pArg;

	if (FAILED(Add_Component(CGameInstance::Get_StaticLevelIndex(), CGameInstance::m_pPrototypeTransformTag, m_pTransformComTag, (CComponent**)&m_pTransformCom, &GameObjectDesc.TransformDesc)))
		return E_FAIL;

	return S_OK;
}

void CGameObject::Tick(_double TimeDelta)
{
}

void CGameObject::Late_Tick(_double TimeDelta)
{
}

HRESULT CGameObject::Render()
{
	return S_OK;
}

HRESULT CGameObject::Add_Component(_uint iLevelIndex, const _tchar* pPrototypeTag, const _tchar* pComponentTag,
	CComponent** ppOut, void* pArg)
{
	if (nullptr != Find_Component(pComponentTag))
	{
		assert(!"Already have Component in GameObject");
		return E_FAIL;
	}

	CGameInstance*	pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	CComponent*		pComponentInstance = pGameInstance->Clone_Component(iLevelIndex, pPrototypeTag, pArg);

	if (nullptr == pComponentInstance)
	{
		assert(!"pComponentInstance is nullptr");
		return E_FAIL;
	}

	m_Components.emplace(pPrototypeTag, pComponentInstance);

	Safe_AddRef(pComponentInstance);

	*ppOut = pComponentInstance;

	Safe_Release(pGameInstance);

	return S_OK;

}

CComponent* CGameObject::Find_Component(const _tchar* pComopnentTag)
{
	auto iter = find_if(m_Components.begin(), m_Components.end(), CTag_Finder(pComopnentTag));

		if (iter == m_Components.end())
			return nullptr;

	return iter->second;
}

void CGameObject::Free()
{
	Safe_Release(m_pTransformCom);

	for (auto& Pair : m_Components)
		Safe_Release(Pair.second);

	m_Components.clear();

	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);
}

