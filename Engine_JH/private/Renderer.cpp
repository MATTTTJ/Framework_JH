#include "stdafx.h"
#include "..\public\Renderer.h"
#include "GameObject.h"
#include "Target_Manager.h"

CRenderer::CRenderer(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CComponent(pDevice,pContext)
	, m_pTarget_Manager(CTarget_Manager::GetInstance())
{
	Safe_AddRef(m_pTarget_Manager);
}

HRESULT CRenderer::Add_RenderGroup(RENDERGROUP eRenderGroup, CGameObject* pGameObject)
{
	if (nullptr == pGameObject || eRenderGroup >= RENDER_END)
		return E_FAIL;

	m_RenderObjectList[eRenderGroup].push_back(pGameObject);

	Safe_AddRef(pGameObject);

	return S_OK;
}

HRESULT CRenderer::Draw_RenderGroup()
{
	FAILED_CHECK_RETURN(Render_Priority(), E_FAIL);
	FAILED_CHECK_RETURN(Render_NonAlphaBlend(), E_FAIL);
	FAILED_CHECK_RETURN(Render_NonLight(), E_FAIL);
	FAILED_CHECK_RETURN(Render_AlphaBlend(), E_FAIL);
	FAILED_CHECK_RETURN(Render_UI(), E_FAIL);

	return S_OK;
}


HRESULT CRenderer::Initialize_Prototype()
{
	FAILED_CHECK_RETURN(__super::Initialize_Prototype(), E_FAIL);

	// D3D11_VIEWPORT		ViewportDesc;
	// ZeroMemory(&ViewportDesc, sizeof(D3D11_VIEWPORT));
	//
	// _uint iNumViewPorts = 1;
	//
	// m_pContext->RSGetViewports(&iNumViewPorts, &ViewportDesc);

	// 렌더타겟 생성 //DXGI_FORMAT_B8G8R8A8_UNORM
	// FAILED_CHECK_RETURN(m_pTarget_Manager->Add_RenderTarget(m_pDevice, m_pContext, TEXT("Target_Diffuse"), ViewportDesc.Width, ViewportDesc.Height, DXGI_FORMAT_R16G16B16A16_UNORM, &_float4(1.f, 0.f, 0.f, 1.f)), E_FAIL);
	// FAILED_CHECK_RETURN(m_pTarget_Manager->Add_RenderTarget(m_pDevice, m_pContext, TEXT("Target_Normal"), ViewportDesc.Width, ViewportDesc.Height, DXGI_FORMAT_R16G16B16A16_UNORM, &_float4(1.f, 1.f, 1.f, 1.f)), E_FAIL);
	// FAILED_CHECK_RETURN(m_pTarget_Manager->Add_RenderTarget(m_pDevice, m_pContext, TEXT("Target_Shade"), ViewportDesc.Width, ViewportDesc.Height, DXGI_FORMAT_R16G16B16A16_UNORM, &_float4(1.f, 1.f, 1.f, 1.f)), E_FAIL);
	//
	// FAILED_CHECK_RETURN(m_pTarget_Manager->Add_MRT(TEXT("MRT_Defferd"), TEXT("Target_Diffuse")), E_FAIL);  // 디퍼드 렌더링 (빛)을 수행하기 위한 
	// FAILED_CHECK_RETURN(m_pTarget_Manager->Add_MRT(TEXT("MRT_Defferd"), TEXT("Target_Normal")), E_FAIL);   // 필요한 데이터들을 저장한 렌더타겟들
	// FAILED_CHECK_RETURN(m_pTarget_Manager->Add_MRT(TEXT("MRT_LightAcc"), TEXT("Target_Shade")), E_FAIL); // 빛 연산의 결과를 저장할 렌더 타겟들

	return S_OK;
}

HRESULT CRenderer::Initialize_Clone(CGameObject* pOwner, void* pArg)
{
	FAILED_CHECK_RETURN(__super::Initialize_Clone(pOwner,pArg), E_FAIL);

	return S_OK;
}




HRESULT CRenderer::Render_Priority()
{
	for(auto& pGameObject : m_RenderObjectList[RENDER_PRIORITY])
	{
		if (nullptr != pGameObject)
			pGameObject->Render();

		Safe_Release(pGameObject);
	}

	m_RenderObjectList[RENDER_PRIORITY].clear();

	return S_OK;
}

HRESULT CRenderer::Render_NonAlphaBlend()
{
	// NULL_CHECK_RETURN(m_pTarget_Manager, E_FAIL);
	//
	// // Diffuse + Normal
	//
	// FAILED_CHECK_RETURN(m_pTarget_Manager->Begin_MRT(m_pContext, TEXT("MRT_Defferd")), E_FAIL);

	for (auto& pGameObject : m_RenderObjectList[RENDER_NONALPHABLEND])
	{
		if (nullptr != pGameObject)
			pGameObject->Render();

		Safe_Release(pGameObject);
	}

	m_RenderObjectList[RENDER_NONALPHABLEND].clear();

	// FAILED_CHECK_RETURN(m_pTarget_Manager->End_MRT(m_pContext, TEXT("MRT_Defferd")), E_FAIL);

	return S_OK;
}

HRESULT CRenderer::Render_NonLight()
{
	for (auto& pGameObject : m_RenderObjectList[RENDER_NONLIGHT])
	{
		if (nullptr != pGameObject)
			pGameObject->Render();

		Safe_Release(pGameObject);
	}

	m_RenderObjectList[RENDER_NONLIGHT].clear();

	return S_OK;
}

HRESULT CRenderer::Render_AlphaBlend()
{
	for (auto& pGameObject : m_RenderObjectList[RENDER_ALPHABLEND])
	{
		if (nullptr != pGameObject)
			pGameObject->Render();

		Safe_Release(pGameObject);
	}

	m_RenderObjectList[RENDER_ALPHABLEND].clear();

	return S_OK;
}

HRESULT CRenderer::Render_UI()
{
	for (auto& pGameObject : m_RenderObjectList[RENDER_UI])
	{
		if (nullptr != pGameObject)
			pGameObject->Render();

		Safe_Release(pGameObject);
	}

	m_RenderObjectList[RENDER_UI].clear();

	return S_OK;
}

CRenderer* CRenderer::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CRenderer*		pInst = new CRenderer(pDevice, pContext);

	if(FAILED(pInst->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Create : CRenderer");
		Safe_Release(pInst);
	}

	return pInst;
}

CComponent* CRenderer::Clone(CGameObject* pOwner, void* pArg)
{
	AddRef();

	return this;
}

void CRenderer::Free()
{
	__super::Free();
	Safe_Release(m_pTarget_Manager);
}
