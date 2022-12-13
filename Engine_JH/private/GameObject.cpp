#include "GameObject.h"
#include "GameInstance.h"
#include "GameUtils.h"

const wstring CGameObject::m_wstrTransformComTag = TEXT("Com_Transform");

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
	, m_bHasModel(rhs.m_bHasModel)
	, m_bIsClone(true)
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
}

HRESULT CGameObject::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CGameObject::Initialize_Clone(const wstring& wstrPrototypeTag, void* pArg)
{
	m_wstrPrototypeGameObjectTag = wstrPrototypeTag;

	GAMEOBJECTDESC GameObjectDesc;
	ZeroMemory(&GameObjectDesc, sizeof(GAMEOBJECTDESC));

	if (nullptr != pArg)
		GameObjectDesc = *(GAMEOBJECTDESC*)pArg;

	FAILED_CHECK_RETURN(Add_Component(CGameInstance::Get_StaticLevelIndex(),CGameInstance::m_wstrPrototypeTransformTag, m_wstrTransformComTag, (CComponent**)&m_pTransformCom, this, &GameObjectDesc.TransformDesc), E_FAIL)

	return S_OK;
}

void CGameObject::Tick(_double dTimeDelta)
{
}

void CGameObject::Late_Tick(_double dTimeDelta)
{
}

HRESULT CGameObject::Render()
{
	return S_OK;
}

void CGameObject::Imgui_RenderComponentProperties()
{
	for (const auto& Pair : m_Components)
	{
		char szName[MAX_PATH];
		CGameUtils::wc2c(Pair.first.c_str(), szName);

		if (ImGui::CollapsingHeader(szName))
			Pair.second->Imgui_RenderProperty();
	}
}


HRESULT CGameObject::Add_Component(_uint iLevelIndex, const wstring& wstrPrototypeTag, const wstring& wstrComponentTag,	CComponent** ppOut, CGameObject* pOwner, void* pArg)
{

	if (nullptr != Find_Component(wstrComponentTag))
		return E_FAIL;

	CGameInstance*		pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	CComponent*	pComponent = pGameInstance->Clone_Component(iLevelIndex, wstrPrototypeTag, pOwner, pArg);
	NULL_CHECK_RETURN(pComponent, E_FAIL)

	m_Components.emplace(wstrComponentTag, pComponent);
	Safe_AddRef(pComponent);

	*ppOut = pComponent;

	Safe_Release(pGameInstance);

	return S_OK;
}

CComponent* CGameObject::Find_Component(const wstring& wstrComponentTag)
{
	auto iter = find_if(m_Components.begin(), m_Components.end(), CTag_Finder(wstrComponentTag));

		if (iter == m_Components.end())
			return nullptr;

	return iter->second;
}

void CGameObject::Free()
{
	if(m_pTransformCom != nullptr)
		Safe_Release(m_pTransformCom);

	for (auto& Pair : m_Components)
		Safe_Release(Pair.second);
	m_Components.clear();

	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);
}

