#include "GameObject.h"

#include "GameInstance.h"
#include "GameUtils.h"

const _tchar* CGameObject::m_pTransformComTag = TEXT("Com_Transform");

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

	FAILED_CHECK_RETURN(Add_Component(CGameInstance::Get_StaticLevelIndex(),
						CGameInstance::m_wstrPrototypeTransformTag, m_pTransformComTag, 
		(CComponent**)&m_pTransformCom, &GameObjectDesc.TransformDesc), E_FAIL)

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

void CGameObject::Imgui_RenderComponentProperties()
{
	for (const auto& com : m_Components)
	{
		ImGui::Separator();
		char szName[128];
		CGameUtils::wc2c(com.first.c_str(), szName);

		if (ImGui::CollapsingHeader(szName))
			com.second->Imgui_RenderProperty();
	}
}


HRESULT CGameObject::Add_Component(_uint iLevelIndex, const wstring& pPrototypeTag, const wstring& pComponentTag,
	CComponent** ppOut, void* pArg)
{

	if (nullptr != Find_Component(pComponentTag))
		return E_FAIL;

	CGameInstance*		pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	CComponent*	pComponent = pGameInstance->Clone_Component(iLevelIndex, pPrototypeTag, pArg);
	NULL_CHECK_RETURN(pComponent, E_FAIL)

	m_Components.emplace(pComponentTag, pComponent);

	Safe_AddRef(pComponent);

	*ppOut = pComponent;

	Safe_Release(pGameInstance);

	return S_OK;
}

CComponent* CGameObject::Find_Component(const wstring& ComponentTag)
{
	auto iter = find_if(m_Components.begin(), m_Components.end(), CTag_Finder(ComponentTag));

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

