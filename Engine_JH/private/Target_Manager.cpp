#include "stdafx.h"
#include "..\public\Target_Manager.h"
#include "Render_Target.h"
#include "Shader.h"
#include "VIBuffer_Rect.h"

IMPLEMENT_SINGLETON(CTarget_Manager);

CTarget_Manager::CTarget_Manager() {}

ID3D11ShaderResourceView* CTarget_Manager::Get_SRV(const _tchar* pTargetTag)
{
	CRender_Target*		pRenderTarget = Find_RenderTarget(pTargetTag);
	NULL_CHECK_RETURN(pTargetTag, nullptr);

	return pRenderTarget->Get_SRV();
}

HRESULT CTarget_Manager::Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	m_pContext = pContext;
	Safe_AddRef(m_pContext);

#ifdef _DEBUG

	D3D11_VIEWPORT			ViewportDesc;
	ZeroMemory(&ViewportDesc, sizeof (D3D11_VIEWPORT));

	_uint			iNumViewports = 1;

	pContext->RSGetViewports(&iNumViewports, &ViewportDesc);

	XMStoreFloat4x4(&m_ViewMatrix, XMMatrixIdentity());
	XMStoreFloat4x4(&m_ProjMatrix, XMMatrixOrthographicLH(ViewportDesc.Width, ViewportDesc.Height, 0.f, 1.f));

	// m_pShader = CShader::Create(pDevice, pContext, L"C:\\Users\\Jihoon\\Documents\\Visual Studio 2015\\Projects\\Framework_JH\\Engine_JH\\Bin\\ShaderFiles\\Shader_Deferred.hlsl", CShader::DECLARATION_VTXTEX, VTXTEX_DECLARATION::Elements, VTXTEX_DECLARATION::iNumElements);
	m_pShader = CShader::Create(pDevice, pContext, L"C:\\Users\\Hoon\\Desktop\\3D_JH\\Framework_JH\\Engine_JH\\Bin\\ShaderFiles\\Shader_Deferred.hlsl", CShader::DECLARATION_VTXTEX, VTXTEX_DECLARATION::Elements, VTXTEX_DECLARATION::iNumElements);
	NULL_CHECK_RETURN(m_pShader, E_FAIL);
	m_pVIBuffer = CVIBuffer_Rect::Create(pDevice, pContext);
	NULL_CHECK_RETURN(m_pVIBuffer, E_FAIL);
#endif

	return S_OK;
}

void CTarget_Manager::Tick(_double dTimeDelta)
{
	D3D11_VIEWPORT		ViewportDesc;
	ZeroMemory(&ViewportDesc, sizeof(D3D11_VIEWPORT));

	_uint		iNumViewport = 1;

	m_pContext->RSGetViewports(&iNumViewport, &ViewportDesc);

	for (auto& Pair : m_mapRenderTarget)
		Pair.second->Tick((_uint)ViewportDesc.Width, (_uint)ViewportDesc.Height);

	// XMStoreFloat4x4(&m_WorldMatrix, XMMatrixScaling(ViewportDesc.Width, ViewportDesc.Height, 1.f));
	// XMStoreFloat4x4(&m_ViewMatrix, XMMatrixIdentity());
	// XMStoreFloat4x4(&m_ProjMatrix, XMMatrixOrthographicLH(ViewportDesc.Width, ViewportDesc.Height, 0.f, 1.f));

#ifdef _DEBUG
	XMStoreFloat4x4(&m_ProjMatrix, XMMatrixOrthographicLH(ViewportDesc.Width, ViewportDesc.Height, 0.f, 1.f));
#endif // _DEBUG
}

HRESULT CTarget_Manager::Add_RenderTarget(ID3D11Device* pDevice, ID3D11DeviceContext* pContext,	const _tchar* pTargetTag, _uint iWidth, _uint iHeight, DXGI_FORMAT ePixelFormat, const _float4* pClearColor)
{
	// 기본적인 렌더 타겟을 추가함 다만 동일한 태그의 렌더타겟은 존재할 수 없음

	if (nullptr != Find_RenderTarget(pTargetTag))
		return E_FAIL;

	CRender_Target*	pRenderTarget = CRender_Target::Create(pDevice, pContext, iWidth, iHeight, ePixelFormat, pClearColor);
	NULL_CHECK_RETURN(pRenderTarget, E_FAIL);

	m_mapRenderTarget.emplace(pTargetTag, pRenderTarget);
	
	return S_OK;
}

HRESULT CTarget_Manager::Add_MRT(const _tchar* pMRTTag, const _tchar* pTargetTag)
{
	CRender_Target*	pRenderTarget = Find_RenderTarget(pTargetTag);

	NULL_CHECK_RETURN(pTargetTag, E_FAIL);

	list<CRender_Target*>*	pMRTList = Find_MRT(pMRTTag);

	if (nullptr == pMRTList)
	{
		// 해당 태그가 없는경우 새로 만들어서 넣음
		list<CRender_Target*> MRTList;
		MRTList.push_back(pRenderTarget);

		m_mapMRTs.emplace(pMRTTag, MRTList);
	}
	else
		pMRTList->push_back(pRenderTarget); // 해당 태그가 있는경우 그대그에 넣음

	Safe_AddRef(pRenderTarget);

	return S_OK;
}

HRESULT CTarget_Manager::Begin_MRT(ID3D11DeviceContext* pContext, const _tchar* pMRTTag)
{
	list<CRender_Target*>*	pMRTList = Find_MRT(pMRTTag);

	NULL_CHECK_RETURN(pMRTList, E_FAIL);

	pContext->OMGetRenderTargets(1, &m_pBackBufferView, &m_pDepthStencilView);

	ID3D11RenderTargetView*		pRTVs[8] = { nullptr };

	_uint iNumViews = 0;

	for (auto& pRTV : *pMRTList)
	{
		pRTV->Clear();
		pRTVs[iNumViews++] = pRTV->Get_RTV();
	}

	pContext->OMSetRenderTargets(iNumViews, pRTVs, m_pDepthStencilView);

	return S_OK;
}

HRESULT CTarget_Manager::End_MRT(ID3D11DeviceContext* pContext, const _tchar* pMRTTag)
{
	pContext->OMSetRenderTargets(1, &m_pBackBufferView, m_pDepthStencilView);

	Safe_Release(m_pBackBufferView);
	Safe_Release(m_pDepthStencilView);

	return S_OK;
}

#ifdef _DEBUG

HRESULT CTarget_Manager::Ready_Debug(const _tchar* pTargetTag, _float fX, _float fY, _float fSizeX, _float fSizeY)
{
	CRender_Target*		pTarget = Find_RenderTarget(pTargetTag);
	NULL_CHECK_RETURN(pTargetTag, E_FAIL);

	return pTarget->Ready_Debug(fX, fY, fSizeX, fSizeY);

}

HRESULT CTarget_Manager::Render_Debug(const _tchar* pMRTTag)
{
	list<CRender_Target*>*		pMRTList = Find_MRT(pMRTTag);
	NULL_CHECK_RETURN(pMRTList, E_FAIL );

	FAILED_CHECK_RETURN(m_pShader->Set_Matrix(L"g_ViewMatrix", &m_ViewMatrix), E_FAIL);
	FAILED_CHECK_RETURN(m_pShader->Set_Matrix(L"g_ProjMatrix", &m_ProjMatrix), E_FAIL);

	for (auto& pRenderTarget : *pMRTList)
	{
		pRenderTarget->Render(m_pShader, m_pVIBuffer);
	}

	return S_OK;
}
#endif // _DEBUG

CRender_Target* CTarget_Manager::Find_RenderTarget(const _tchar* pTargetTag)
{
	auto iter = find_if(m_mapRenderTarget.begin(), m_mapRenderTarget.end(), CTag_Finder(pTargetTag));

	if (iter == m_mapRenderTarget.end())
		return nullptr;

	return iter->second;
}

list<CRender_Target*>* CTarget_Manager::Find_MRT(const _tchar* pMRTTag)
{
	auto& iter = find_if(m_mapMRTs.begin(), m_mapMRTs.end(), CTag_Finder(pMRTTag));

	if (iter == m_mapMRTs.end())
		return nullptr;

	return &iter->second;
}

void CTarget_Manager::Free()
{
	for (auto& Pair : m_mapMRTs)
	{
		for (auto& pRenderTarget : Pair.second)
			Safe_Release(pRenderTarget);
		
		Pair.second.clear();
	}
	m_mapMRTs.clear();

	for (auto& Pair : m_mapRenderTarget)
		Safe_Release(Pair.second);

	m_mapRenderTarget.clear();

	Safe_Release(m_pContext);

#ifdef _DEBUG
	Safe_Release(m_pShader);
	Safe_Release(m_pVIBuffer);

#endif
}
