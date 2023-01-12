#include "stdafx.h"
#include "Light_Manager.h"
#include "Light.h"

IMPLEMENT_SINGLETON(CLight_Manager)

CLight_Manager::CLight_Manager()
{
}

const LIGHTDESC* CLight_Manager::Get_LightDesc(_uint iIndex)
{
	if (iIndex >= m_vecLight.size())
		return nullptr;

	return m_vecLight[iIndex]->Get_LightDesc();
}

HRESULT CLight_Manager::Add_Light(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const LIGHTDESC& LightDesc)
{
	CLight*		pLight = CLight::Create(pDevice, pContext, LightDesc);
	NULL_CHECK_RETURN(pLight, E_FAIL);

	m_vecLight.push_back(pLight);

	return S_OK;
}

void CLight_Manager::Render_Light(CVIBuffer_Rect* pVIBuffer, CShader* pShader)
{
	for(auto& pLight : m_vecLight)
	{
		if (nullptr!= pLight)
			pLight->Render(pVIBuffer, pShader);
	}
}

void CLight_Manager::Clear()
{
	for (auto& pLight : m_vecLight)
		Safe_Release(pLight);

	m_vecLight.clear();
}

void CLight_Manager::Free()
{
	for (auto& pLight : m_vecLight)
		Safe_Release(pLight);

	m_vecLight.clear();
}
