#include "stdafx.h"
#include "..\public\Render_Target.h"
#include "Shader.h"
#include "VIBuffer_Rect.h"

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
	// m_ePixelFormat = ePixelFormat;
	m_vClearColor = *pClearColor;

	ZeroMemory(&m_ViewPort, sizeof(D3D11_VIEWPORT));
	m_ViewPort.TopLeftX = 0.f;
	m_ViewPort.TopLeftY = 0.f;
	m_ViewPort.Width = (_float)iWidth;
	m_ViewPort.Height = (_float)iHeight;
	m_ViewPort.MinDepth = 0.f;
	m_ViewPort.MaxDepth = 1.f;

	return S_OK;
}

HRESULT CRender_Target::Tick(_uint iWidth, _uint iHeight)
{
	// if (iWidth >= 1300)
		// return S_OK;
	
	// if (m_pTexture2D != nullptr)
	// 	Safe_Release(m_pTexture2D);
	//
	// if (m_pRTV != nullptr)
	// 	Safe_Release(m_pRTV);
	//
	// if (m_pSRV != nullptr)
	// 	Safe_Release(m_pSRV);
	//
	// D3D11_TEXTURE2D_DESC		TextureDesc;
	// ZeroMemory(&TextureDesc, sizeof(D3D11_TEXTURE2D_DESC));
	//
	// TextureDesc.Width = iWidth;
	// TextureDesc.Height = iHeight;
	// TextureDesc.MipLevels = 1;
	// TextureDesc.ArraySize = 1;
	// TextureDesc.Format = m_ePixelFormat;
	//
	// TextureDesc.SampleDesc.Quality = 0;
	// TextureDesc.SampleDesc.Count = 1;
	//
	// TextureDesc.Usage = D3D11_USAGE_DEFAULT;
	// TextureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	// TextureDesc.CPUAccessFlags = 0;
	// TextureDesc.MiscFlags = 0;
	//
	// FAILED_CHECK_RETURN(m_pDevice->CreateTexture2D(&TextureDesc, nullptr, &m_pTexture2D), E_FAIL);
	//
	// FAILED_CHECK_RETURN(m_pDevice->CreateRenderTargetView(m_pTexture2D, nullptr, &m_pRTV), E_FAIL);
	//
	// FAILED_CHECK_RETURN(m_pDevice->CreateShaderResourceView(m_pTexture2D, nullptr, &m_pSRV), E_FAIL);

	return S_OK;
}

HRESULT CRender_Target::Clear()
{
	m_pContext->ClearRenderTargetView(m_pRTV, (_float*)&m_vClearColor);

	return S_OK;
}

HRESULT CRender_Target::Ready_DepthStencilRenderTargetView(_uint iWidth, _uint iHeight, DXGI_FORMAT eFormat)
{
	NULL_CHECK_RETURN(m_pDevice, E_FAIL);

	ID3D11Texture2D*		pDepthStencilTexture = nullptr;

	D3D11_TEXTURE2D_DESC	TextureDesc;
	ZeroMemory(&TextureDesc, sizeof(D3D11_TEXTURE2D_DESC));

	TextureDesc.Width = iWidth;
	TextureDesc.Height = iHeight;
	TextureDesc.MipLevels = 1;
	TextureDesc.ArraySize = 1;
	TextureDesc.Format = eFormat;

	TextureDesc.SampleDesc.Quality = 0;
	TextureDesc.SampleDesc.Count = 1;

	TextureDesc.Usage = D3D11_USAGE_DEFAULT;
	TextureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	TextureDesc.CPUAccessFlags = 0;
	TextureDesc.MiscFlags = 0;

	FAILED_CHECK_RETURN(m_pDevice->CreateTexture2D(&TextureDesc, nullptr, &pDepthStencilTexture), E_FAIL);
	FAILED_CHECK_RETURN(m_pDevice->CreateDepthStencilView(pDepthStencilTexture, nullptr, &m_pDepthStencilView), E_FAIL);

	Safe_Release(pDepthStencilTexture);

	return S_OK;


}
#ifdef _DEBUG
HRESULT CRender_Target::Ready_Debug(_float fX, _float fY, _float fSizeX, _float fSizeY)
{
	D3D11_VIEWPORT			ViewportDesc;
	ZeroMemory(&ViewportDesc, sizeof(D3D11_VIEWPORT));

	_uint			iNumViewports = 1;

	m_pContext->RSGetViewports(&iNumViewports, &ViewportDesc);

	XMStoreFloat4x4(&m_WorldMatrix, XMMatrixIdentity());

	m_WorldMatrix._11 = fSizeX;
	m_WorldMatrix._22 = fSizeY;
	m_WorldMatrix._41 = fX - ViewportDesc.Width * 0.5f;
	m_WorldMatrix._42 = -fY + ViewportDesc.Height * 0.5f;


	return S_OK;
}

HRESULT CRender_Target::Render(CShader* pShader, CVIBuffer_Rect* pVIBuffer)
{
	NULL_CHECK_RETURN(pShader, E_FAIL);
	NULL_CHECK_RETURN(pVIBuffer, E_FAIL);

	pShader->Set_Matrix(L"g_WorldMatrix", &m_WorldMatrix);
	pShader->Set_ShaderResourceView(L"g_Texture", m_pSRV);

	pShader->Begin(0);
	pVIBuffer->Render();

	return S_OK;
}
#endif

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
	Safe_Release(m_pDepthStencilView);

	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);

	Safe_Release(m_pSRV);
	Safe_Release(m_pRTV);
	Safe_Release(m_pTexture2D);
}

