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
	ID3D11RenderTargetView*		Get_RTV() const { return m_pRTV; }

public:
	HRESULT						Initialize(_uint iWidth, _uint iHeight, DXGI_FORMAT ePixelFormat, const _float4* pClearColor);

private:
	ID3D11Device*				m_pDevice = nullptr;
	ID3D11DeviceContext*		m_pContext = nullptr;

private:
	ID3D11Texture2D*			m_pTexture2D = nullptr;
	ID3D11RenderTargetView*		m_pRTV = nullptr;
	ID3D11ShaderResourceView*	m_pSRV = nullptr;

private:
	_float4						m_vClearColor;

public:
	static CRender_Target*		Create(ID3D11Device*	pDevice, ID3D11DeviceContext* pContext, _uint iWidth, _uint iHeight, DXGI_FORMAT ePixelFormat, const _float4* pClearColor);
	virtual void				Free() override;
};

END