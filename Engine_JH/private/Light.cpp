#include "stdafx.h"
#include "Light.h"
#include "Shader.h"
#include "VIBuffer_Rect.h"

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

HRESULT CLight::Render(CVIBuffer_Rect* pVIBuffer, CShader* pShader)
{
	_uint		iPassIndex = 1;

	if (LIGHTDESC::LIGHT_DIRECTIONAL == m_LightDesc.eType)
	{
		FAILED_CHECK_RETURN(pShader->Set_RawValue(L"g_vLightDir", &m_LightDesc.vDirection, sizeof(_float4)), E_FAIL);
		iPassIndex = 1;
	}
	else if(LIGHTDESC::LIGHT_POINT == m_LightDesc.eType)
	{
		FAILED_CHECK_RETURN(pShader->Set_RawValue(L"g_vLightPos", &m_LightDesc.vPosition, sizeof(_float4)), E_FAIL);
		FAILED_CHECK_RETURN(pShader->Set_RawValue(L"g_fLightRange", &m_LightDesc.fRange, sizeof(_float)), E_FAIL);
		iPassIndex = 2;
	}
	FAILED_CHECK_RETURN(pShader->Set_RawValue(L"g_vLightDiffuse", &m_LightDesc.vDiffuse, sizeof(_float4)), E_FAIL);
	FAILED_CHECK_RETURN(pShader->Set_RawValue(L"g_vLightAmbient", &m_LightDesc.vAmbient, sizeof(_float4)), E_FAIL);
	FAILED_CHECK_RETURN(pShader->Set_RawValue(L"g_vLightSpecular", &m_LightDesc.vSpecular, sizeof(_float4)), E_FAIL);

	pShader->Begin(iPassIndex);

	pVIBuffer->Render();

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
