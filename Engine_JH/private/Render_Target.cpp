#include "stdafx.h"
#include "..\public\Render_Target.h"

CRender_Target::CRender_Target(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: m_pDevice(pDevice)
	, m_pContext(pContext)
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
}

HRESULT CRender_Target::Initialize(_uint iWidth, _uint iHeight, DXGI_FORMAT ePixelFormat, const _float4* pClearColor)
{
	D3D11_TEXTURE2D_DESC		TextureDesc;
	ZeroMemory(&TextureDesc, sizeof(D3D11_TEXTURE2D_DESC));

	TextureDesc.Width = iWidth;
	TextureDesc.Height = iHeight;
	TextureDesc.Format = ePixelFormat;
	TextureDesc.MipLevels = 1;
	TextureDesc.ArraySize = 1;

	TextureDesc.SampleDesc.Quality = 0;
	TextureDesc.SampleDesc.Count = 1;

	TextureDesc.Usage = D3D11_USAGE_DEFAULT;
	TextureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	TextureDesc.CPUAccessFlags = 0;
	TextureDesc.MiscFlags = 0;

	FAILED_CHECK_RETURN(m_pDevice->CreateTexture2D(&TextureDesc, nullptr, &m_pTexture2D), E_FAIL); 
	FAILED_CHECK_RETURN(m_pDevice->CreateRenderTargetView(m_pTexture2D, nullptr, &m_pRTV), E_FAIL);  // 장치에 바인딩해, 셰이더를 통해 바인딩된 이 텍스쳐에 픽셀을 기록
	FAILED_CHECK_RETURN(m_pDevice->CreateShaderResourceView(m_pTexture2D, nullptr, &m_pSRV), E_FAIL); // 이 텍스쳐를 셰이더 전역으로 전달,
																									//픽셀셰이더 안에서 이 텍스쳐에 저장된 픽셀의 값 (Diffuse, Normal, Shade)을 읽어온다. 
	m_vClearColor = *pClearColor;

	return S_OK;
}

CRender_Target* CRender_Target::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, _uint iWidth, _uint iHeight, DXGI_FORMAT ePixelFormat, const _float4* pClearColor)
{
	CRender_Target*		pInstance = new CRender_Target(pDevice, pContext);

	if (FAILED(pInstance->Initialize(iWidth, iHeight, ePixelFormat, pClearColor)))
	{
		MSG_BOX("Failed to Created : CRender_Target");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CRender_Target::Free()
{
	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);

	Safe_Release(m_pSRV);
	Safe_Release(m_pRTV);
	Safe_Release(m_pTexture2D);
}

