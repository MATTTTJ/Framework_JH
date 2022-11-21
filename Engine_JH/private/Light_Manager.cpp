#include "Light_Manager.h"

#include "Light.h"

IMPLEMENT_SINGLETON(CLight_Manager)

CLight_Manager::CLight_Manager()
{
}

const LIGHTDESC* CLight_Manager::Get_LightDesc(_uint iIndex)
{
	if (iIndex >= m_vLight.size())
		return nullptr;

	return m_vLight[iIndex]->Get_LightDesc();
}

HRESULT CLight_Manager::Add_Light(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const LIGHTDESC& LightDesc)
{
	CLight*		pLight = CLight::Create(pDevice, pContext, LightDesc);

	if (nullptr == pLight)
		return E_FAIL;

	m_vLight.push_back(pLight);

	return S_OK;
}

void CLight_Manager::Clear()
{
	for (auto& pLight : m_vLight)
		Safe_Release(pLight);

	m_vLight.clear();
}

void CLight_Manager::Free()
{
	for (auto& pLight : m_vLight)
		Safe_Release(pLight);

	m_vLight.clear();
}
