#include "stdafx.h"
#include "..\public\Renderer.h"
#include "GameObject.h"
#include "Target_Manager.h"
#include "Light_Manager.h"
#include "VIBuffer_Rect.h"
#include "Shader.h"


CRenderer::CRenderer(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CComponent(pDevice,pContext)
	, m_pTarget_Manager(CTarget_Manager::GetInstance())
	, m_pLight_Manager(CLight_Manager::GetInstance())
{
	Safe_AddRef(m_pTarget_Manager);
	Safe_AddRef(m_pLight_Manager);
}

HRESULT CRenderer::Add_RenderGroup(RENDERGROUP eRenderGroup, CGameObject* pGameObject)
{
	if (nullptr == pGameObject || eRenderGroup >= RENDER_END)
		return E_FAIL;

	m_RenderObjectList[eRenderGroup].push_back(pGameObject);

	Safe_AddRef(pGameObject);

	return S_OK;
}

HRESULT CRenderer::Add_DebugRenderGroup(CComponent* pComponent)
{
	NULL_CHECK_RETURN(pComponent, E_FAIL);

	m_DebugObject.push_back(pComponent);

	Safe_AddRef(pComponent);

	return S_OK;
}

HRESULT CRenderer::Draw_RenderGroup()
{
	FAILED_CHECK_RETURN(__super::Initialize_Prototype(), E_FAIL);

	D3D11_VIEWPORT		ViewportDesc;
	ZeroMemory(&ViewportDesc, sizeof(D3D11_VIEWPORT));

	_uint iNumViewPorts = 1;

	m_pContext->RSGetViewports(&iNumViewPorts, &ViewportDesc);

	XMStoreFloat4x4(&m_WorldMatrix, XMMatrixScaling(ViewportDesc.Width, ViewportDesc.Height, 1.f));
	XMStoreFloat4x4(&m_ViewMatrix, XMMatrixIdentity());
	XMStoreFloat4x4(&m_ProjMatrix, XMMatrixOrthographicLH(ViewportDesc.Width, ViewportDesc.Height, 0.f, 1.f));

	FAILED_CHECK_RETURN(Render_Priority(), E_FAIL);
	FAILED_CHECK_RETURN(Render_NonAlphaBlend(), E_FAIL);
	FAILED_CHECK_RETURN(Render_LightAcc(), E_FAIL);
	FAILED_CHECK_RETURN(Render_Blend(), E_FAIL);
	FAILED_CHECK_RETURN(Render_NonLight(), E_FAIL);
	FAILED_CHECK_RETURN(Render_AlphaBlend(), E_FAIL);
	FAILED_CHECK_RETURN(Render_UI(), E_FAIL);

#ifdef _DEBUG
	FAILED_CHECK_RETURN(Render_DebugObject(), E_FAIL);

	if (nullptr != m_pTarget_Manager)
	{
		FAILED_CHECK_RETURN(m_pTarget_Manager->Ready_Debug(TEXT("Target_Diffuse"), 100.0f, 100.f, 200.f, 200.f), E_FAIL);
		FAILED_CHECK_RETURN(m_pTarget_Manager->Ready_Debug(TEXT("Target_Normal"), 100.0f, 300.f, 200.f, 200.f), E_FAIL);
		FAILED_CHECK_RETURN(m_pTarget_Manager->Ready_Debug(TEXT("Target_Shade"), 300.0f, 100.f, 200.f, 200.f), E_FAIL);

		m_pTarget_Manager->Render_Debug(TEXT("MRT_Deferred"));
		m_pTarget_Manager->Render_Debug(TEXT("MRT_LightAcc"));
	}
#endif


	return S_OK;
}


HRESULT CRenderer::Initialize_Prototype()
{
	FAILED_CHECK_RETURN(__super::Initialize_Prototype(), E_FAIL);

	D3D11_VIEWPORT		ViewportDesc;
	ZeroMemory(&ViewportDesc, sizeof(D3D11_VIEWPORT));
	
	_uint iNumViewPorts = 1;
	
	m_pContext->RSGetViewports(&iNumViewPorts, &ViewportDesc);

	//렌더타겟 생성 //DXGI_FORMAT_B8G8R8A8_UNORM
	FAILED_CHECK_RETURN(m_pTarget_Manager->Add_RenderTarget(m_pDevice, m_pContext, TEXT("Target_Diffuse"), (_uint)ViewportDesc.Width, (_uint)ViewportDesc.Height, DXGI_FORMAT_R16G16B16A16_UNORM, &_float4(0.f, 0.f, 0.f, 0.f)), E_FAIL);
	FAILED_CHECK_RETURN(m_pTarget_Manager->Add_RenderTarget(m_pDevice, m_pContext, TEXT("Target_Normal"), (_uint)ViewportDesc.Width, (_uint)ViewportDesc.Height, DXGI_FORMAT_R16G16B16A16_UNORM, &_float4(1.f, 1.f, 1.f, 1.f)), E_FAIL);
	FAILED_CHECK_RETURN(m_pTarget_Manager->Add_RenderTarget(m_pDevice, m_pContext, TEXT("Target_Shade"), (_uint)ViewportDesc.Width, (_uint)ViewportDesc.Height, DXGI_FORMAT_R16G16B16A16_UNORM, &_float4(0.f, 0.f, 0.f, 1.f)), E_FAIL);
	
	FAILED_CHECK_RETURN(m_pTarget_Manager->Add_MRT(TEXT("MRT_Deferred"), TEXT("Target_Diffuse")), E_FAIL);  // 디퍼드 렌더링 (빛)을 수행하기 위한 
	FAILED_CHECK_RETURN(m_pTarget_Manager->Add_MRT(TEXT("MRT_Deferred"), TEXT("Target_Normal")), E_FAIL);   // 필요한 데이터들을 저장한 렌더타겟들
	FAILED_CHECK_RETURN(m_pTarget_Manager->Add_MRT(TEXT("MRT_LightAcc"), TEXT("Target_Shade")), E_FAIL); // 빛 연산의 결과를 저장할 렌더 타겟들

	m_pVIBuffer = CVIBuffer_Rect::Create(m_pDevice, m_pContext);
	NULL_CHECK_RETURN(m_pVIBuffer, E_FAIL);
	// m_pShader = CShader::Create(m_pDevice, m_pContext, L"../Bin/ShaderFiles/Shader_Deferred.hlsl", CShader::DECLARATION_VTXTEX, VTXTEX_DECLARATION::Elements, VTXTEX_DECLARATION::iNumElements);
	m_pShader = CShader::Create(m_pDevice, m_pContext, L"C:\\Users\\Hoon\\Desktop\\3D_JH\\Framework_JH\\Engine_JH\\Bin\\ShaderFiles\\Shader_Deferred.hlsl", CShader::DECLARATION_VTXTEX, VTXTEX_DECLARATION::Elements, VTXTEX_DECLARATION::iNumElements);
	NULL_CHECK_RETURN(m_pShader, E_FAIL);




#ifdef _DEBUG
	FAILED_CHECK_RETURN(m_pTarget_Manager->Ready_Debug(TEXT("Target_Diffuse"), 100.0f, 100.f, 200.f, 200.f), E_FAIL);
	FAILED_CHECK_RETURN(m_pTarget_Manager->Ready_Debug(TEXT("Target_Normal"), 100.0f, 300.f, 200.f, 200.f), E_FAIL);
	FAILED_CHECK_RETURN(m_pTarget_Manager->Ready_Debug(TEXT("Target_Shade"), 300.0f, 100.f, 200.f, 200.f), E_FAIL);
#endif


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
	NULL_CHECK_RETURN(m_pTarget_Manager, E_FAIL);
	
	// Diffuse + Normal
	
	FAILED_CHECK_RETURN(m_pTarget_Manager->Begin_MRT(m_pContext, TEXT("MRT_Deferred")), E_FAIL);

	for (auto& pGameObject : m_RenderObjectList[RENDER_NONALPHABLEND])
	{
		if (nullptr != pGameObject)
			pGameObject->Render();

		Safe_Release(pGameObject);
	}

	m_RenderObjectList[RENDER_NONALPHABLEND].clear();

	FAILED_CHECK_RETURN(m_pTarget_Manager->End_MRT(m_pContext, TEXT("MRT_Deferred")), E_FAIL);

	return S_OK;
}

HRESULT CRenderer::Render_LightAcc()
{
	NULL_CHECK_RETURN(m_pTarget_Manager, E_FAIL);

	FAILED_CHECK_RETURN(m_pTarget_Manager->Begin_MRT(m_pContext, L"MRT_LightAcc"), E_FAIL);

	FAILED_CHECK_RETURN(m_pShader->Set_Matrix(L"g_WorldMatrix", &m_WorldMatrix), E_FAIL);
	FAILED_CHECK_RETURN(m_pShader->Set_Matrix(L"g_ViewMatrix", &m_ViewMatrix), E_FAIL);
	FAILED_CHECK_RETURN(m_pShader->Set_Matrix(L"g_ProjMatrix", &m_ProjMatrix), E_FAIL);

	FAILED_CHECK_RETURN(m_pShader->Set_ShaderResourceView(L"g_NormalTexture_Deferred", m_pTarget_Manager->Get_SRV(TEXT("Target_Normal"))), E_FAIL);

	m_pLight_Manager->Render_Light(m_pVIBuffer, m_pShader);

	FAILED_CHECK_RETURN(m_pTarget_Manager->End_MRT(m_pContext, L"MRT_LightAcc"), E_FAIL);

	return S_OK;

}

HRESULT CRenderer::Render_Blend()
{

	FAILED_CHECK_RETURN(m_pShader->Set_Matrix(L"g_WorldMatrix", &m_WorldMatrix), E_FAIL);
	FAILED_CHECK_RETURN(m_pShader->Set_Matrix(L"g_ViewMatrix", &m_ViewMatrix), E_FAIL);
	FAILED_CHECK_RETURN(m_pShader->Set_Matrix(L"g_ProjMatrix", &m_ProjMatrix), E_FAIL);
	FAILED_CHECK_RETURN(m_pShader->Set_ShaderResourceView(L"g_DiffuseTexture_Deferred", m_pTarget_Manager->Get_SRV(TEXT("Target_Diffuse"))), E_FAIL);
	FAILED_CHECK_RETURN(m_pShader->Set_ShaderResourceView(L"g_ShadeTexture_Deferred", m_pTarget_Manager->Get_SRV(TEXT("Target_Shade"))), E_FAIL);

	m_pShader->Begin(3);
	m_pVIBuffer->Render();

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
#ifdef _DEBUG
HRESULT CRenderer::Render_DebugObject()
{
	for(auto & pComponent : m_DebugObject)
	{
		if (nullptr != pComponent)
			pComponent->Render();

		Safe_Release(pComponent);
	}
	m_DebugObject.clear();

	return S_OK;
}
#endif


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

	for (auto& pComponent : m_DebugObject)
		Safe_Release(pComponent);

	m_DebugObject.clear();

	for (_uint i = 0; i < RENDER_END; ++i)
	{
		for (auto& pGameObject : m_RenderObjectList[i])
			Safe_Release(pGameObject);

		m_RenderObjectList[i].clear();
	}

	Safe_Release(m_pVIBuffer);
	Safe_Release(m_pShader);

	Safe_Release(m_pLight_Manager);
	Safe_Release(m_pTarget_Manager);
}
