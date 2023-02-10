#pragma once
#include "Engine_Defines.h"
#include "Base.h"

BEGIN(Engine)
class CRender_Target final : public CBase
{
private:
	CRender_Target(ID3D11Device*	pDevice, ID3D11DeviceContext* pContext);
	virtual ~CRender_Target() = default;

public:
	ID3D11RenderTargetView*			Get_RTV() const { return m_pRTV; }
	ID3D11ShaderResourceView*		Get_SRV() const { return m_pSRV; }


public:
	HRESULT						Initialize(_uint iWidth, _uint iHeight, DXGI_FORMAT ePixelFormat, const _float4* pClearColor);
	HRESULT						Tick(_uint iWidth, _uint iHeight);
	HRESULT						Clear();

	HRESULT						Ready_DepthStencilRenderTargetView(_uint iWidth, _uint iHeight, DXGI_FORMAT eFormat);
	ID3D11DepthStencilView*		GetDepthStencilView() { return m_pDepthStencilView; }
	D3D11_VIEWPORT				GetViewPortDesc() { return m_ViewPort; }


// #ifdef _DEBUG

public:
	HRESULT						Ready_Debug(_float fX, _float fY, _float fSizeX, _float fSizeY);
// #endif

	HRESULT						Render(class CShader* pShader, class CVIBuffer_Rect* pVIBuffer);



private:
	ID3D11Device*				m_pDevice = nullptr;
	ID3D11DeviceContext*		m_pContext = nullptr;

	ID3D11DepthStencilView*		m_pDepthStencilView = nullptr;
	D3D11_VIEWPORT				m_ViewPort;
	_float4						m_vClearColor;

private:
	ID3D11Texture2D*			m_pTexture2D = nullptr;
	ID3D11RenderTargetView*		m_pRTV = nullptr;
	ID3D11ShaderResourceView*	m_pSRV = nullptr;

private:
	_float4x4					m_WorldMatrix;



public:
	static CRender_Target*		Create(ID3D11Device*	pDevice, ID3D11DeviceContext* pContext, _uint iWidth, _uint iHeight, DXGI_FORMAT ePixelFormat, const _float4* pClearColor);
	virtual void				Free() override;
};

END