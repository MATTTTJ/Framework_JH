#include "stdafx.h"
#include "..\public\Target_Manager.h"
#include "Render_Target.h"

IMPLEMENT_SINGLETON(CTarget_Manager);

CTarget_Manager::CTarget_Manager() {}

HRESULT CTarget_Manager::Add_RenderTarget(ID3D11Device* pDevice, ID3D11DeviceContext* pContext,	const _tchar* pTargetTag, _uint iWidth, _uint iHeight, DXGI_FORMAT ePixelFormat, const _float4* pClearColor)
{
	// �⺻���� ���� Ÿ���� �߰��� �ٸ� ������ �±��� ����Ÿ���� ������ �� ����

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
		// �ش� �±װ� ���°�� ���� ���� ����
		list<CRender_Target*> MRTList;
		MRTList.push_back(pRenderTarget);

		m_mapMRTs.emplace(pMRTTag, MRTList);
	}
	else
		pMRTList->push_back(pRenderTarget); // �ش� �±װ� �ִ°�� �״�׿� ����

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
		pRTVs[iNumViews++] = pRTV->Get_RTV();

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
}
