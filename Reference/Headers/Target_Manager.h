#pragma once
#include "Engine_Defines.h"
#include "Base.h"

BEGIN(Engine)
class CTarget_Manager final : public CBase
{
	DECLARE_SINGLETON(CTarget_Manager);

public:
	CTarget_Manager();
	virtual ~CTarget_Manager() = default;

public:
	HRESULT Add_RenderTarget(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const _tchar* pTargetTag, _uint iWidth, _uint iHeight, DXGI_FORMAT ePixelFormat, const _float4* pClearColor);
	HRESULT Add_MRT(const _tchar* pMRTTag, const _tchar* pTargetTag);

	HRESULT Begin_MRT(ID3D11DeviceContext* pContext, const _tchar* pMRTTag);
	HRESULT End_MRT(ID3D11DeviceContext* pContext, const _tchar* pMRTTag);

private:
	map<const _tchar*, class CRender_Target*>				m_mapRenderTarget;
	typedef map<const _tchar*, class CRender_Target*>		RENDERTARGETS;

private:
	/* 동시에 장치에 바인딩 되어야할 렌더타겟들. 최대 여덟개 */
	map<const _tchar*, list<class CRender_Target*>>			m_mapMRTs;
	typedef map<const _tchar*, list<class CRender_Target*>>	MRTS;

private:
	ID3D11RenderTargetView*				m_pBackBufferView = nullptr;
	ID3D11DepthStencilView*				m_pDepthStencilView = nullptr;

private:
	class CRender_Target* Find_RenderTarget(const _tchar* pTargetTag);
	list<class CRender_Target*>* Find_MRT(const _tchar* pMRTTag);

public:
	virtual void Free() override;
};

END