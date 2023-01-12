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
	FAILED_CHECK_RETURN(pShader->Set_RawValue(L"g_vLightDir", &m_LightDesc.vDirection, sizeof(_float4)), E_FAIL);

	pShader->Begin(1);

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
