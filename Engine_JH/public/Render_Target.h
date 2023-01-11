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
	HRESULT						Clear();



#ifdef _DEBUG

public:
	HRESULT						Ready_Debug(_float fX, _float fY, _float fSizeX, _float fSizeY);
	void						Render(class CShader* pShader, class CVIBuffer_Rect* pVIBuffer);

#endif


private:
	ID3D11Device*				m_pDevice = nullptr;
	ID3D11DeviceContext*		m_pContext = nullptr;

private:
	ID3D11Texture2D*			m_pTexture2D = nullptr;
	ID3D11RenderTargetView*		m_pRTV = nullptr;
	ID3D11ShaderResourceView*	m_pSRV = nullptr;

private:
	_float4						m_vClearColor;

#ifdef _DEBUG
private:
	_float4x4					m_WorldMatrix;

#endif


public:
	static CRender_Target*		Create(ID3D11Device*	pDevice, ID3D11DeviceContext* pContext, _uint iWidth, _uint iHeight, DXGI_FORMAT ePixelFormat, const _float4* pClearColor);
	virtual void				Free() override;
};

END