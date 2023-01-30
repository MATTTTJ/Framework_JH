#pragma once
#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL CContext_LockGuard
{
public:
	CContext_LockGuard(mutex& ContextMtx)
		: m_ContextMtx(ContextMtx)
	{
		m_ContextMtx.lock();
	}

	~CContext_LockGuard()
	{
		m_ContextMtx.unlock();
	}


private:
	mutex&	m_ContextMtx;
};





class CGraphic_Device :public CBase
{
	DECLARE_SINGLETON(CGraphic_Device)

public:
	CGraphic_Device();
	virtual ~CGraphic_Device() = default;

public:
	mutex&		GetContexMtx() { return m_ContextMtx; }

public:
	HRESULT		Ready_Graphic_Device(HWND hWnd, GRAPHIC_DESC::WINMODE Winmode,_uint iWinCX, _uint iWinCY,ID3D11Device** ppDeviceOut, ID3D11DeviceContext** ppDeviceContextOut);
	HRESULT		Clear_BackBuffer_View(_float4 vClearColor);
	HRESULT		Clear_DepthStencil_View();
	HRESULT		Present();
	HRESULT		Update_SwapChain(HWND hWnd, _uint iWinCX, _uint iWinCY, _bool bIsFullScreen, _bool bNeedUpdate);


public:
	D3D11_VIEWPORT		Get_ViewPort() {
		return m_pViewPort;
	}
private:
	ID3D11Device*			m_pDevice = nullptr;
	ID3D11DeviceContext*	m_pDeviceContext = nullptr;
	IDXGISwapChain*			m_pSwapChain = nullptr;

	ID3D11RenderTargetView*	m_pBackBufferRTV = nullptr;
	ID3D11DepthStencilView*	m_pDepthStencilView = nullptr;
	D3D11_VIEWPORT			m_pViewPort;
	mutex								m_ContextMtx;

private:
	HRESULT		Ready_SwapChain(HWND hWnd, GRAPHIC_DESC::WINMODE eWinMode, _uint iWinCX, _uint iWINCY);
	HRESULT		Ready_BackBufferRenderTargetView();
	HRESULT		Ready_DepthStencilRenderTargetView(_uint iWinCX, _uint iWinCY);

	
public:
	virtual void Free() override;
};
END
