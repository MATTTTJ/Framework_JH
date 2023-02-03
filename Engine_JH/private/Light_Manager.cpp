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

void CLight_Manager::Set_LightPos(_uint iIndex, _fvector vPos)
{
	auto iter = m_vecLight.begin();

	for (_uint i = 0; i < iIndex; ++i)
		++iter;

	(*iter)->Set_LightPos(vPos);
}

void CLight_Manager::Set_LightRange(_uint iIndex, _float fRange)
{
	auto	iter = m_vecLight.begin();

	for (_uint i = 0; i < iIndex; ++i)
		++iter;

	(*iter)->Set_LightRange(fRange);
}

void CLight_Manager::Set_LightDirection(_uint iIndex, _float4 vDirection)
{
	auto	iter = m_vecLight.begin();

	for (_uint i = 0; i < iIndex; ++i)
		++iter;

	(*iter)->Set_LightDirection(vDirection);
}

_float4x4 CLight_Manager::Get_Transform(_uint iIndex, _uint eState)
{
	if (m_vecLight.size() <= iIndex)
	{
		_float4x4 ReturnMatrix;
		XMStoreFloat4x4(&ReturnMatrix, XMMatrixIdentity());
		return ReturnMatrix;
	}

	auto	iter = m_vecLight.begin();

	for (_uint i = 0; i < iIndex; ++i)
		++iter;

	return (*iter)->Get_Transform((CLight::TRANSFORMSTATE)eState);
}

_float4x4 CLight_Manager::Get_Transform_TP(_uint iIndex, _uint eState)
{
	if (m_vecLight.size() <= iIndex)
	{
		_float4x4 ReturnMatrix;
		XMStoreFloat4x4(&ReturnMatrix, XMMatrixIdentity());
		return ReturnMatrix;
	}

	auto	iter = m_vecLight.begin();

	for (_uint i = 0; i < iIndex; ++i)
		++iter;

	return (*iter)->Get_Transform_TP((CLight::TRANSFORMSTATE)eState);
}

void CLight_Manager::CascadeUpdate(_float4x4 CamWorldMatrix, _float4* vFrustumCorners)
{
	for(auto& iter : m_vecLight)
	{
		if(LIGHTDESC::LIGHT_DIRECTIONAL == iter->Get_LightDesc()->eType)
		{
			iter->Update(CamWorldMatrix, vFrustumCorners);
		}
	}
}

HRESULT CLight_Manager::Delete_Light(_uint iLightIndex)
{
	if (m_vecLight.size() < iLightIndex)
		return E_FAIL;

	// Safe_Release(m_vecLight[iLightIndex]);
	m_vecLight[iLightIndex]->Set_LightEnable();
	// for(auto& iter : m_vecLight)
	// {
	// 	
	// }
	//
	// Safe_Release();

	return S_OK;
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
		if (LIGHTDESC::LIGHT_DIRECTIONAL == pLight->Get_LightDesc()->eType)
			continue;

		if (nullptr!= pLight && pLight->Get_LightDesc()->isEnable == true)
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
