#include "stdafx.h"
#include "Light.h"

CLight::CLight(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: m_pDevice(pDevice)
	, m_pContext(pContext)
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
}

HRESULT CLight::Initialize(const LIGHTDESC& LightDesc)
{
	m_LightDesc = LightDesc;

	return S_OK;
}

CLight* CLight::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const LIGHTDESC& LightDesc)
{
	CLight*		pInst = new CLight(pDevice, pContext);

	if(FAILED(pInst->Initialize(LightDesc)))
	{
		MSG_BOX("Failed to Create : CLight");
		Safe_Release(pInst);
	}

	return pInst;
}

void CLight::Free()
{
	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);
}
