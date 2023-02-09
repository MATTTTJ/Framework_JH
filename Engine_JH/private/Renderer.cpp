#include "stdafx.h"
#include "..\public\Renderer.h"
#include "GameObject.h"
#include "Target_Manager.h"
#include "Light_Manager.h"
#include "PipeLine.h"
#include "Render_Target.h"
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
	// NULL_CHECK_RETURN(pComponent, E_FAIL);
	//
	// m_DebugObject.push_back(pComponent);
	//
	// Safe_AddRef(pComponent);

	return S_OK;
}


HRESULT CRenderer::Initialize_Prototype()
{
	FAILED_CHECK_RETURN(__super::Initialize_Prototype(), E_FAIL);

	D3D11_VIEWPORT		ViewportDesc;
	ZeroMemory(&ViewportDesc, sizeof(D3D11_VIEWPORT));

	_uint iNumViewPorts = 1;

	m_pContext->RSGetViewports(&iNumViewPorts, &ViewportDesc);

	//·»´õÅ¸°Ù »ý¼º //DXGI_FORMAT_B8G8R8A8_UNORM
	FAILED_CHECK_RETURN(m_pTarget_Manager->Add_RenderTarget(m_pDevice, m_pContext, TEXT("Target_Diffuse"), (_uint)ViewportDesc.Width, (_uint)ViewportDesc.Height, DXGI_FORMAT_R16G16B16A16_UNORM, &_float4(0.0f, 0.0f, 0.0f, 0.f)), E_FAIL);
	FAILED_CHECK_RETURN(m_pTarget_Manager->Add_RenderTarget(m_pDevice, m_pContext, TEXT("Target_Normal"), (_uint)ViewportDesc.Width, (_uint)ViewportDesc.Height, DXGI_FORMAT_R16G16B16A16_UNORM, &_float4(1.f, 1.f, 1.f, 1.f)), E_FAIL);
	FAILED_CHECK_RETURN(m_pTarget_Manager->Add_RenderTarget(m_pDevice, m_pContext, TEXT("Target_Depth"), (_uint)ViewportDesc.Width, (_uint)ViewportDesc.Height, DXGI_FORMAT_R32G32B32A32_FLOAT, &_float4(0.0f, 0.0f, 0.0f, 1.f)), E_FAIL);
	FAILED_CHECK_RETURN(m_pTarget_Manager->Add_RenderTarget(m_pDevice, m_pContext, TEXT("Target_Shade"), (_uint)ViewportDesc.Width, (_uint)ViewportDesc.Height, DXGI_FORMAT_R32G32B32A32_FLOAT, &_float4(0.f, 0.f, 0.f, 1.f)), E_FAIL);
	FAILED_CHECK_RETURN(m_pTarget_Manager->Add_RenderTarget(m_pDevice, m_pContext, TEXT("Target_Specular"), (_uint)ViewportDesc.Width, (_uint)ViewportDesc.Height, DXGI_FORMAT_R16G16B16A16_UNORM, &_float4(0.0f, 0.0f, 0.0f, 0.f)), E_FAIL);
	FAILED_CHECK_RETURN(m_pTarget_Manager->Add_RenderTarget(m_pDevice, m_pContext, TEXT("Target_Flag"), (_uint)ViewportDesc.Width, (_uint)ViewportDesc.Height, DXGI_FORMAT_R16G16B16A16_UNORM, &_float4(0.f, 0.f, 0.f, 0.f)), E_FAIL);
	FAILED_CHECK_RETURN(m_pTarget_Manager->Add_RenderTarget(m_pDevice, m_pContext, TEXT("Target_HorizontalBlur"), (_uint)ViewportDesc.Width, (_uint)ViewportDesc.Height, DXGI_FORMAT_R16G16B16A16_UNORM, &_float4(0.f, 0.f, 0.f, 0.f)), E_FAIL);
	FAILED_CHECK_RETURN(m_pTarget_Manager->Add_RenderTarget(m_pDevice, m_pContext, TEXT("Target_VerticalBlur"), (_uint)ViewportDesc.Width, (_uint)ViewportDesc.Height, DXGI_FORMAT_R16G16B16A16_UNORM, &_float4(0.f, 0.f, 0.f, 0.f)), E_FAIL);
	FAILED_CHECK_RETURN(m_pTarget_Manager->Add_RenderTarget(m_pDevice, m_pContext, TEXT("Target_Blur"), (_uint)ViewportDesc.Width, (_uint)ViewportDesc.Height, DXGI_FORMAT_R16G16B16A16_UNORM, &_float4(0.f, 0.f, 0.f, 0.f)), E_FAIL);
	FAILED_CHECK_RETURN(m_pTarget_Manager->Add_RenderTarget(m_pDevice, m_pContext, TEXT("Target_Bloom"), (_uint)ViewportDesc.Width, (_uint)ViewportDesc.Height, DXGI_FORMAT_R16G16B16A16_UNORM, &_float4(0.f, 0.f, 0.f, 0.f)), E_FAIL);
	FAILED_CHECK_RETURN(m_pTarget_Manager->Add_RenderTarget(m_pDevice, m_pContext, TEXT("Target_OriginEffect"), (_uint)ViewportDesc.Width, (_uint)ViewportDesc.Height, DXGI_FORMAT_R16G16B16A16_UNORM, &_float4(0.0f, 0.0f, 0.0f, 0.f)), E_FAIL);

	_uint		iShadowMapCX = 10240; // 5120; 
	_uint		iShadowMapCY =  5476; //2738;

	//For Target_Shadow
	// FAILED_CHECK_RETURN(m_pTarget_Manager->Add_RenderTarget(m_pDevice, m_pContext, TEXT("Target_Dynamic_ShadowDepth"), (_uint)ViewportDesc.Width, (_uint)ViewportDesc.Height, DXGI_FORMAT_R32G32B32A32_FLOAT, &_float4(1.f, 1.f, 1.f, 1.f)), E_FAIL);
	// m_pTarget_Manager->Find_RenderTarget(TEXT("Target_Dynamic_ShadowDepth"))->Ready_DepthStencilRenderTargetView((_uint)ViewportDesc.Width, (_uint)ViewportDesc.Height, DXGI_FORMAT_D24_UNORM_S8_UINT);

	FAILED_CHECK_RETURN(m_pTarget_Manager->Add_RenderTarget(m_pDevice, m_pContext, TEXT("Target_Dynamic_ShadowDepth"), iShadowMapCX, iShadowMapCY, DXGI_FORMAT_R32G32B32A32_FLOAT, &_float4(1.f, 1.f, 1.f, 1.f)), E_FAIL);
	m_pTarget_Manager->Find_RenderTarget(TEXT("Target_Dynamic_ShadowDepth"))->Ready_DepthStencilRenderTargetView(iShadowMapCX, iShadowMapCY, DXGI_FORMAT_D24_UNORM_S8_UINT);

	// For Target_OutLine
	FAILED_CHECK_RETURN(m_pTarget_Manager->Add_RenderTarget(m_pDevice, m_pContext, TEXT("Target_Outline"), (_uint)ViewportDesc.Width, (_uint)ViewportDesc.Height, DXGI_FORMAT_R16G16B16A16_UNORM, &_float4(1.f, 1.f, 1.f, 0.f)), E_FAIL);
	FAILED_CHECK_RETURN(m_pTarget_Manager->Add_RenderTarget(m_pDevice, m_pContext, TEXT("Target_OutlineFlag"), (_uint)ViewportDesc.Width, (_uint)ViewportDesc.Height, DXGI_FORMAT_R16G16B16A16_UNORM, &_float4(0.f, 0.f, 0.f, 0.f)), E_FAIL);

	FAILED_CHECK_RETURN(m_pTarget_Manager->Add_MRT(TEXT("MRT_Deferred"), TEXT("Target_Diffuse")), E_FAIL);  // µðÆÛµå ·»´õ¸µ (ºû)À» ¼öÇàÇÏ±â À§ÇÑ 
	FAILED_CHECK_RETURN(m_pTarget_Manager->Add_MRT(TEXT("MRT_Deferred"), TEXT("Target_Normal")), E_FAIL);   // ÇÊ¿äÇÑ µ¥ÀÌÅÍµéÀ» ÀúÀåÇÑ ·»´õÅ¸°Ùµé
	FAILED_CHECK_RETURN(m_pTarget_Manager->Add_MRT(TEXT("MRT_Deferred"), TEXT("Target_Depth")), E_FAIL); // ºû ¿¬»êÀÇ °á°ú¸¦ ÀúÀåÇÒ ·»´õ Å¸°Ùµé
	FAILED_CHECK_RETURN(m_pTarget_Manager->Add_MRT(TEXT("MRT_Deferred"), TEXT("Target_Flag")), E_FAIL);

	FAILED_CHECK_RETURN(m_pTarget_Manager->Add_MRT(TEXT("MRT_Effect"), TEXT("Target_OriginEffect")), E_FAIL);
	FAILED_CHECK_RETURN(m_pTarget_Manager->Add_MRT(TEXT("MRT_Effect"), TEXT("Target_Flag")), E_FAIL);

	FAILED_CHECK_RETURN(m_pTarget_Manager->Add_MRT(TEXT("MRT_LightAcc"), TEXT("Target_Shade")), E_FAIL); // ºû ¿¬»êÀÇ °á°ú¸¦ ÀúÀåÇÒ ·»´õ Å¸°Ùµé
	FAILED_CHECK_RETURN(m_pTarget_Manager->Add_MRT(TEXT("MRT_LightAcc"), TEXT("Target_Specular")), E_FAIL); // ºû ¿¬»êÀÇ °á°ú¸¦ ÀúÀåÇÒ ·»´õ Å¸°Ùµé

	FAILED_CHECK_RETURN(m_pTarget_Manager->Add_MRT(TEXT("MRT_LightDepth"), TEXT("Target_Dynamic_ShadowDepth")), E_FAIL);

	FAILED_CHECK_RETURN(m_pTarget_Manager->Add_MRT(TEXT("MRT_Outline"), TEXT("Target_Outline")), E_FAIL);

	FAILED_CHECK_RETURN(m_pTarget_Manager->Add_MRT(TEXT("MRT_Blur"), TEXT("Target_Blur")), E_FAIL);

	FAILED_CHECK_RETURN(m_pTarget_Manager->Add_MRT(TEXT("MRT_Bloom"), TEXT("Target_Bloom")), E_FAIL);

#ifdef _DEBUG
	FAILED_CHECK_RETURN(m_pTarget_Manager->Ready_Debug(TEXT("Target_Diffuse"), 50.0f, 50.f, 100.f, 100.f), E_FAIL);
	FAILED_CHECK_RETURN(m_pTarget_Manager->Ready_Debug(TEXT("Target_Normal"), 50.0f, 150.f, 100.f, 100.f), E_FAIL);
	FAILED_CHECK_RETURN(m_pTarget_Manager->Ready_Debug(TEXT("Target_Depth"), 50.0f, 250.f, 100.f, 100.f), E_FAIL);
	FAILED_CHECK_RETURN(m_pTarget_Manager->Ready_Debug(TEXT("Target_Flag"), 250.0f, 50.f, 100.f, 100.f), E_FAIL);
	FAILED_CHECK_RETURN(m_pTarget_Manager->Ready_Debug(TEXT("Target_Outline"), 250.0f, 150.f, 100.f, 100.f), E_FAIL);
	FAILED_CHECK_RETURN(m_pTarget_Manager->Ready_Debug(TEXT("Target_OriginEffect"), 50.0f, 350.f, 100.f, 100.f), E_FAIL);
	FAILED_CHECK_RETURN(m_pTarget_Manager->Ready_Debug(TEXT("Target_Shade"), 150.0f, 50.f, 100.f, 100.f), E_FAIL);
	FAILED_CHECK_RETURN(m_pTarget_Manager->Ready_Debug(TEXT("Target_Specular"), 150.0f, 150.f, 100.f, 100.f), E_FAIL);
	FAILED_CHECK_RETURN(m_pTarget_Manager->Ready_Debug(TEXT("Target_Dynamic_ShadowDepth"), 150.0f, 250.f, 100.f, 100.f), E_FAIL);
	FAILED_CHECK_RETURN(m_pTarget_Manager->Ready_Debug(TEXT("Target_Bloom"), 150.0f, 350.f, 100.f, 100.f), E_FAIL);
	FAILED_CHECK_RETURN(m_pTarget_Manager->Ready_Debug(TEXT("Target_Blur"), 150.0f, 450.f, 100.f, 100.f), E_FAIL);
#endif

	_matrix			WorldMatrix = XMMatrixIdentity();
	WorldMatrix.r[0] = XMVectorSet(ViewportDesc.Width, 0.f, 0.f, 0.f);
	WorldMatrix.r[1] = XMVectorSet(0.f, ViewportDesc.Height, 0.f, 0.f);

	XMStoreFloat4x4(&m_WorldMatrix, XMMatrixTranspose(WorldMatrix));

	XMStoreFloat4x4(&m_ViewMatrix, XMMatrixIdentity());

	XMStoreFloat4x4(&m_ProjMatrix, XMMatrixTranspose(XMMatrixOrthographicLH(ViewportDesc.Width, ViewportDesc.Height, 0.f, 1.f)));

	m_pVIBuffer = CVIBuffer_Rect::Create(m_pDevice, m_pContext);
	NULL_CHECK_RETURN(m_pVIBuffer, E_FAIL);
	m_pShader = CShader::Create(m_pDevice, m_pContext, L"../Bin/ShaderFiles/Shader_Deferred.hlsl", CShader::DECLARATION_VTXTEX, VTXTEX_DECLARATION::Elements, VTXTEX_DECLARATION::iNumElements);
	NULL_CHECK_RETURN(m_pShader, E_FAIL);

	return S_OK;
}

HRESULT CRenderer::Initialize_Clone(CGameObject* pOwner, void* pArg)
{
	// FAILED_CHECK_RETURN(__super::Initialize_Clone(pOwner,pArg), E_FAIL);

	return S_OK;
}

HRESULT CRenderer::Draw_RenderGroup()
{
	FAILED_CHECK_RETURN(__super::Initialize_Prototype(), E_FAIL);

	// D3D11_VIEWPORT		ViewportDesc;
	// ZeroMemory(&ViewportDesc, sizeof(D3D11_VIEWPORT));
	//
	// _uint iNumViewPorts = 1;
	//
	// m_pContext->RSGetViewports(&iNumViewPorts, &ViewportDesc);
	//
	// XMStoreFloat4x4(&m_WorldMatrix, XMMatrixScaling(ViewportDesc.Width, ViewportDesc.Height, 1.f));
	// XMStoreFloat4x4(&m_ViewMatrix, XMMatrixIdentity());
	// XMStoreFloat4x4(&m_ProjMatrix, XMMatrixOrthographicLH(ViewportDesc.Width, ViewportDesc.Height, 0.f, 1.f));

	FAILED_CHECK_RETURN(Render_Priority(), E_FAIL);
	FAILED_CHECK_RETURN(Render_ShadowDepth_Dynamic(), E_FAIL);

	FAILED_CHECK_RETURN(Render_NonAlphaBlend(), E_FAIL);

	FAILED_CHECK_RETURN(Render_LightAcc(), E_FAIL);

	// FAILED_CHECK_RETURN(Render_ShadowBlur(), E_FAIL);
	FAILED_CHECK_RETURN(Render_Blend(), E_FAIL);
	FAILED_CHECK_RETURN(Render_OutLine(), E_FAIL);

	FAILED_CHECK_RETURN(Render_NonLight(), E_FAIL);
	FAILED_CHECK_RETURN(Render_AlphaBlend(), E_FAIL);
	FAILED_CHECK_RETURN(Render_Glow(), E_FAIL);
	FAILED_CHECK_RETURN(Render_Effect(), E_FAIL);
	FAILED_CHECK_RETURN(Render_UI(), E_FAIL);
	FAILED_CHECK_RETURN(Render_FADE(), E_FAIL);

#ifdef _DEBUG
	// FAILED_CHECK_RETURN(Render_DebugObject(), E_FAIL);

	if (nullptr != m_pTarget_Manager)
	{
		FAILED_CHECK_RETURN(m_pTarget_Manager->Ready_Debug(TEXT("Target_Diffuse"), 100.0f, 100.f, 200.f, 200.f), E_FAIL);
		FAILED_CHECK_RETURN(m_pTarget_Manager->Ready_Debug(TEXT("Target_Normal"), 100.0f, 300.f, 200.f, 200.f), E_FAIL);
		FAILED_CHECK_RETURN(m_pTarget_Manager->Ready_Debug(TEXT("Target_Depth"), 100.0f, 500.f, 200.f, 200.f), E_FAIL);
		FAILED_CHECK_RETURN(m_pTarget_Manager->Ready_Debug(TEXT("Target_Flag"), 100.0f, 700.f, 200.f, 200.f), E_FAIL);
		FAILED_CHECK_RETURN(m_pTarget_Manager->Ready_Debug(TEXT("Target_Outline"), 300.0f, 100.f, 200.f, 200.f), E_FAIL);
		FAILED_CHECK_RETURN(m_pTarget_Manager->Ready_Debug(TEXT("Target_OriginEffect"), 300.0f, 300.f, 200.f, 200.f), E_FAIL);
		
		FAILED_CHECK_RETURN(m_pTarget_Manager->Ready_Debug(TEXT("Target_Shade"), 500.0f, 100.f, 200.f, 200.f), E_FAIL);
		FAILED_CHECK_RETURN(m_pTarget_Manager->Ready_Debug(TEXT("Target_Specular"), 500.0f, 300.f, 200.f, 200.f), E_FAIL);
		FAILED_CHECK_RETURN(m_pTarget_Manager->Ready_Debug(TEXT("Target_Dynamic_ShadowDepth"), 500.0f, 500.f, 200.f, 200.f), E_FAIL);
		FAILED_CHECK_RETURN(m_pTarget_Manager->Ready_Debug(TEXT("Target_Bloom"), 700.0f, 100.f, 200.f, 200.f), E_FAIL);
		FAILED_CHECK_RETURN(m_pTarget_Manager->Ready_Debug(TEXT("Target_Blur"), 700.0f, 300.f, 200.f, 200.f), E_FAIL);

		m_pTarget_Manager->Render_Debug(TEXT("MRT_Deferred"));
		m_pTarget_Manager->Render_Debug(TEXT("MRT_Effect"));
		m_pTarget_Manager->Render_Debug(TEXT("MRT_LightAcc"));
		m_pTarget_Manager->Render_Debug(TEXT("MRT_Bloom"));
		m_pTarget_Manager->Render_Debug(TEXT("MRT_Blur"));
		m_pTarget_Manager->Render_Debug(TEXT("MRT_LightDepth"));
		m_pTarget_Manager->Render_Debug(TEXT("MRT_Outline"));

	}
#endif


	return S_OK;
}

void CRenderer::Imgui_RenderProperty()
{
	if (ImGui::Button("Recompile Shader"))
		m_pShader->ReCompileShader();
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

HRESULT CRenderer::Render_ShadowDepth_Dynamic()
{
	FAILED_CHECK_RETURN(m_pTarget_Manager->Begin_ShadowDepthRenderTarget(m_pContext, TEXT("Target_Dynamic_ShadowDepth")), E_FAIL);

	for(auto& pGameObject : m_RenderObjectList[RENDER_DYNAMIC_SHADOWDEPTH])
	{
		if (nullptr != pGameObject)
			pGameObject->Render_ShadowDepth();

		Safe_Release(pGameObject);
	}
	m_RenderObjectList[RENDER_DYNAMIC_SHADOWDEPTH].clear();

	FAILED_CHECK_RETURN(m_pTarget_Manager->End_MRT(m_pContext, L""), E_FAIL);
	
	return S_OK;
}

HRESULT CRenderer::Render_ShadowDepth_Static()
{
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

	FAILED_CHECK_RETURN(m_pTarget_Manager->End_MRT(m_pContext, TEXT("")), E_FAIL);

	return S_OK;
}

HRESULT CRenderer::Render_Effect()
{
	FAILED_CHECK_RETURN(m_pTarget_Manager->Begin_MRT(m_pContext, TEXT("MRT_Effect")), E_FAIL);

	m_RenderObjectList[RENDER_EFFECT].sort([](CGameObject* pSour, CGameObject* pDest)
	{
		return pSour->Get_CamDistance() > pDest->Get_CamDistance();
	});

	for(auto& pGameObject : m_RenderObjectList[RENDER_EFFECT])
	{
		if (nullptr != pGameObject)
			pGameObject->Render();

		Safe_Release(pGameObject);
	}

	m_RenderObjectList[RENDER_EFFECT].clear();

	FAILED_CHECK_RETURN(m_pTarget_Manager->End_MRT(m_pContext, TEXT("MRT_Effect")), E_FAIL);
	//
	FAILED_CHECK_RETURN(Render_Blur(TEXT("Target_OriginEffect")), E_FAIL);
	FAILED_CHECK_RETURN(Render_Bloom(TEXT("Target_OriginEffect")), E_FAIL);
		
	return S_OK;
}

HRESULT CRenderer::Render_LightAcc()
{
	NULL_CHECK_RETURN(m_pTarget_Manager, E_FAIL);

	FAILED_CHECK_RETURN(m_pTarget_Manager->Begin_MRT(m_pContext, L"MRT_LightAcc"), E_FAIL);

	FAILED_CHECK_RETURN(m_pShader->Set_ShaderResourceView(L"g_NormalTexture_Deferred", m_pTarget_Manager->Get_SRV(TEXT("Target_Normal"))), E_FAIL);
	FAILED_CHECK_RETURN(m_pShader->Set_ShaderResourceView(L"g_DepthTexture_Deferred", m_pTarget_Manager->Get_SRV(TEXT("Target_Depth"))), E_FAIL);
	FAILED_CHECK_RETURN(m_pShader->Set_ShaderResourceView(L"g_ShadowDynamicDepthTexture", m_pTarget_Manager->Get_SRV(TEXT("Target_Dynamic_ShadowDepth"))), E_FAIL);

	FAILED_CHECK_RETURN(m_pShader->Set_Matrix(L"g_WorldMatrix", &m_WorldMatrix), E_FAIL);
	FAILED_CHECK_RETURN(m_pShader->Set_Matrix(L"g_ViewMatrix", &m_ViewMatrix), E_FAIL);
	FAILED_CHECK_RETURN(m_pShader->Set_Matrix(L"g_ProjMatrix", &m_ProjMatrix), E_FAIL);

	CPipeLine*		pPipeLine = CPipeLine::GetInstance();

	FAILED_CHECK_RETURN(m_pShader->Set_Matrix(L"g_ProjMatrixInv", &pPipeLine->Get_TransformFloat4x4_Inverse(CPipeLine::D3DTS_PROJ)), E_FAIL);
	FAILED_CHECK_RETURN(m_pShader->Set_Matrix(L"g_ViewMatrixInv", &pPipeLine->Get_TransformFloat4x4_Inverse(CPipeLine::D3DTS_VIEW)), E_FAIL);
	FAILED_CHECK_RETURN(m_pShader->Set_RawValue(L"g_vCamPosition", &pPipeLine->Get_CamPosition(), sizeof(_float4)), E_FAIL);

	_float fFar = 300.f;
	FAILED_CHECK_RETURN(m_pShader->Set_RawValue(L"g_fFar", &fFar, sizeof(_float)), E_FAIL);
	FAILED_CHECK_RETURN(m_pShader->Set_Matrix(L"g_matLightView", &CLight_Manager::GetInstance()->Get_Transform(1, 0)), E_FAIL);
	FAILED_CHECK_RETURN(m_pShader->Set_Matrix(L"g_matLightProj", &CLight_Manager::GetInstance()->Get_Transform(1, 1)), E_FAIL);

	m_pLight_Manager->Render_Light(m_pVIBuffer, m_pShader);

	FAILED_CHECK_RETURN(m_pTarget_Manager->End_MRT(m_pContext, L"MRT_LightAcc"), E_FAIL);

	// FAILED_CHECK_RETURN(Render_Bloom(TEXT("Target_Specular")), E_FAIL);

	return S_OK;
}

HRESULT CRenderer::Render_DOF()
{
	return S_OK;
}

HRESULT CRenderer::Render_Blend()
{
	FAILED_CHECK_RETURN(m_pShader->Set_Matrix(L"g_WorldMatrix", &m_WorldMatrix), E_FAIL);
	FAILED_CHECK_RETURN(m_pShader->Set_Matrix(L"g_ViewMatrix", &m_ViewMatrix), E_FAIL);
	FAILED_CHECK_RETURN(m_pShader->Set_Matrix(L"g_ProjMatrix", &m_ProjMatrix), E_FAIL);

	FAILED_CHECK_RETURN(m_pShader->Set_ShaderResourceView(L"g_DiffuseTexture_Deferred", m_pTarget_Manager->Get_SRV(TEXT("Target_Diffuse"))), E_FAIL);
	FAILED_CHECK_RETURN(m_pShader->Set_ShaderResourceView(L"g_ShadeTexture_Deferred", m_pTarget_Manager->Get_SRV(TEXT("Target_Shade"))), E_FAIL);
	FAILED_CHECK_RETURN(m_pShader->Set_ShaderResourceView(L"g_SpecularTexture_Deferred", m_pTarget_Manager->Get_SRV(TEXT("Target_Specular"))), E_FAIL);
	FAILED_CHECK_RETURN(m_pShader->Set_ShaderResourceView(L"g_FlagTexture", m_pTarget_Manager->Get_SRV(TEXT("Target_Flag"))), E_FAIL);

	FAILED_CHECK_RETURN(m_pShader->Set_ShaderResourceView(L"g_BlurTexture", m_pTarget_Manager->Get_SRV(TEXT("Target_Blur"))), E_FAIL);
	FAILED_CHECK_RETURN(m_pShader->Set_ShaderResourceView(L"g_BloomTexture", m_pTarget_Manager->Get_SRV(TEXT("Target_Bloom"))), E_FAIL);
	FAILED_CHECK_RETURN(m_pShader->Set_ShaderResourceView(L"g_EffectTexture", m_pTarget_Manager->Get_SRV(TEXT("Target_OriginEffect"))), E_FAIL);
	FAILED_CHECK_RETURN(m_pShader->Set_ShaderResourceView(L"g_OutlineTexture", m_pTarget_Manager->Get_SRV(TEXT("Target_Outline"))), E_FAIL);

	

	// m_pShader->Set_RawValue(L"g_WorldMatrix", &m_WorldMatrix, sizeof(_float4x4));
	// m_pShader->Set_RawValue(L"g_ViewMatrix", &m_ViewMatrix, sizeof(_float4x4));
	// m_pShader->Set_RawValue(L"g_ProjMatrix", &m_ProjMatrix, sizeof(_float4x4));

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
	m_RenderObjectList[RENDER_ALPHABLEND].sort([](CGameObject* pSour, CGameObject* pDest)->_bool
	{
		return pSour->Get_CamDistance() > pDest->Get_CamDistance();
	});

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

HRESULT CRenderer::Render_Blur(const _tchar* pTargetTag)
{
	FAILED_CHECK_RETURN(m_pTarget_Manager->Begin_RenderTarget(m_pContext, TEXT("Target_Blur")), E_FAIL);

	FAILED_CHECK_RETURN(m_pShader->Set_ShaderResourceView(L"g_DiffuseTexture_Deferred", m_pTarget_Manager->Get_SRV(pTargetTag)), E_FAIL);
	FAILED_CHECK_RETURN(m_pShader->Set_ShaderResourceView(L"g_FlagTexture", m_pTarget_Manager->Get_SRV(TEXT("Target_Flag"))), E_FAIL);

	D3D11_VIEWPORT			ViewPortDesc;
	ZeroMemory(&ViewPortDesc, sizeof(D3D11_VIEWPORT));

	_uint		iNumViewports = 1;

	m_pContext->RSGetViewports(&iNumViewports, &ViewPortDesc);

	m_pShader->Set_Matrix(L"g_WorldMatrix", &m_WorldMatrix);
	m_pShader->Set_Matrix(L"g_ViewMatrix", &m_ViewMatrix);
	m_pShader->Set_Matrix(L"g_ProjMatrix", &m_ProjMatrix);
							
	m_pShader->Set_RawValue(L"g_iWinCX", &ViewPortDesc.Width, sizeof(_float));
	m_pShader->Set_RawValue(L"g_iWinCY", &ViewPortDesc.Height, sizeof(_float));

	m_pShader->Begin(9);

	m_pVIBuffer->Render();

	FAILED_CHECK_RETURN(m_pTarget_Manager->End_MRT(m_pContext, L""), E_FAIL);

	FAILED_CHECK_RETURN(m_pTarget_Manager->Begin_RenderTarget(m_pContext, TEXT("Target_HorizontalBlur")), E_FAIL);
	FAILED_CHECK_RETURN(m_pShader->Set_ShaderResourceView(L"g_DiffuseTexture_Deferred", m_pTarget_Manager->Get_SRV(TEXT("Target_Blur"))), E_FAIL);

	m_pShader->Begin(5);

	m_pVIBuffer->Render();

	FAILED_CHECK_RETURN(m_pTarget_Manager->End_MRT(m_pContext, L""), E_FAIL;)

	FAILED_CHECK_RETURN(m_pTarget_Manager->Begin_RenderTarget(m_pContext, TEXT("Target_VerticalBlur")), E_FAIL);
	FAILED_CHECK_RETURN(m_pShader->Set_ShaderResourceView(L"g_HorizontalBlurTexture", m_pTarget_Manager->Get_SRV(TEXT("Target_HorizontalBlur"))), E_FAIL);

	m_pShader->Begin(6);

	m_pVIBuffer->Render();
	FAILED_CHECK_RETURN(m_pTarget_Manager->End_MRT(m_pContext, L""), E_FAIL);

	FAILED_CHECK_RETURN(m_pTarget_Manager->Begin_RenderTarget(m_pContext, TEXT("Target_Blur")), E_FAIL);
	FAILED_CHECK_RETURN(m_pShader->Set_ShaderResourceView(L"g_VerticalBlurTexture", m_pTarget_Manager->Get_SRV(TEXT("Target_VerticalBlur"))), E_FAIL);

	m_pShader->Begin(7);

	m_pVIBuffer->Render();

	FAILED_CHECK_RETURN(m_pTarget_Manager->End_MRT(m_pContext, L""), E_FAIL);

	return S_OK;
}

HRESULT CRenderer::Render_Bloom(const _tchar* pTargetTag)
{
	// FAILED_CHECK_RETURN(m_pTarget_Manager->Begin_RenderTarget(m_pContext, TEXT("Target_Bloom")), E_FAIL);

	FAILED_CHECK_RETURN(m_pShader->Set_ShaderResourceView(L"g_DiffuseTexture_Deferred", m_pTarget_Manager->Get_SRV(pTargetTag)), E_FAIL);
	FAILED_CHECK_RETURN(m_pShader->Set_ShaderResourceView(L"g_BlurTexture", m_pTarget_Manager->Get_SRV(TEXT("Target_Blur"))), E_FAIL);


	FAILED_CHECK_RETURN(m_pShader->Set_ShaderResourceView(L"g_FlagTexture", m_pTarget_Manager->Get_SRV(TEXT("Target_Flag"))), E_FAIL);

	D3D11_VIEWPORT			ViewPortDesc;
	ZeroMemory(&ViewPortDesc, sizeof(D3D11_VIEWPORT));

	_uint		iNumViewports = 1;

	m_pContext->RSGetViewports(&iNumViewports, &ViewPortDesc);

	m_pShader->Set_Matrix(L"g_WorldMatrix", &m_WorldMatrix);
	m_pShader->Set_Matrix(L"g_ViewMatrix", &m_ViewMatrix);
	m_pShader->Set_Matrix(L"g_ProjMatrix", &m_ProjMatrix);

	m_pShader->Set_RawValue(L"g_iWinCX", &ViewPortDesc.Width, sizeof(_float));
	m_pShader->Set_RawValue(L"g_iWinCY", &ViewPortDesc.Height, sizeof(_float));

	m_pShader->Begin(8);

	m_pVIBuffer->Render();

	// FAILED_CHECK_RETURN(m_pTarget_Manager->End_MRT(m_pContext, L""), E_FAIL);

	// FAILED_CHECK_RETURN(m_pTarget_Manager->Begin_RenderTarget(m_pContext, TEXT("Target_HorizontalBlur")), E_FAIL);
	// FAILED_CHECK_RETURN(m_pShader->Set_ShaderResourceView(L"g_DiffuseTexture_Deferred", m_pTarget_Manager->Get_SRV(TEXT("Target_Bloom"))), E_FAIL);

	// m_pShader->Begin(5);

	// m_pVIBuffer->Render();
	// FAILED_CHECK_RETURN(m_pTarget_Manager->End_MRT(m_pContext, L""), E_FAIL;)

		// FAILED_CHECK_RETURN(m_pTarget_Manager->Begin_RenderTarget(m_pContext, TEXT("Target_VerticalBlur")), E_FAIL);
	// FAILED_CHECK_RETURN(m_pShader->Set_ShaderResourceView(L"g_HorizontalBlurTexture", m_pTarget_Manager->Get_SRV(TEXT("Target_HorizontalBlur"))), E_FAIL);

	// m_pShader->Begin(6);

	// m_pVIBuffer->Render();
	// FAILED_CHECK_RETURN(m_pTarget_Manager->End_MRT(m_pContext, L""), E_FAIL);

	// FAILED_CHECK_RETURN(m_pTarget_Manager->Begin_RenderTarget(m_pContext, TEXT("Target_Bloom")), E_FAIL);
	// FAILED_CHECK_RETURN(m_pShader->Set_ShaderResourceView(L"g_VerticalBlurTexture", m_pTarget_Manager->Get_SRV(TEXT("Target_VerticalBlur"))), E_FAIL);

	// m_pShader->Begin(7);

	// m_pVIBuffer->Render();

	// FAILED_CHECK_RETURN(m_pTarget_Manager->End_MRT(m_pContext, L""), E_FAIL);

	return S_OK;
}

HRESULT CRenderer::Render_OutLine()
{
	FAILED_CHECK_RETURN(m_pTarget_Manager->Begin_RenderTarget(m_pContext, TEXT("Target_OutlineFlag")), E_FAIL);

	for (auto& pGameObject : m_RenderObjectList[RENDER_OUTLINE])
	{
		if (nullptr != pGameObject)
			pGameObject->Render_OutLineFlag();

		Safe_Release(pGameObject);
	}

	m_RenderObjectList[RENDER_OUTLINE].clear();

	FAILED_CHECK_RETURN(m_pTarget_Manager->End_MRT(m_pContext, L""), E_FAIL);

	// FAILED_CHECK_RETURN(m_pTarget_Manager->Begin_RenderTarget(m_pContext, TEXT("Target_Outline")), E_FAIL);

	D3D11_VIEWPORT			ViewPortDesc;
	ZeroMemory(&ViewPortDesc, sizeof(D3D11_VIEWPORT));

	_uint		iNumViewports = 1;

	m_pContext->RSGetViewports(&iNumViewports, &ViewPortDesc);

	m_pShader->Set_RawValue(L"g_iWinCX", &ViewPortDesc.Width, sizeof(_float));
	m_pShader->Set_RawValue(L"g_iWinCY", &ViewPortDesc.Height, sizeof(_float));

	FAILED_CHECK_RETURN(m_pShader->Set_ShaderResourceView(L"g_OutlineFlagTexture", m_pTarget_Manager->Get_SRV(TEXT("Target_OutlineFlag"))), E_FAIL);

	m_pShader->Set_Matrix(L"g_WorldMatrix", &m_WorldMatrix);
	m_pShader->Set_Matrix(L"g_ViewMatrix", &m_ViewMatrix);
	m_pShader->Set_Matrix(L"g_ProjMatrix", &m_ProjMatrix);

	m_pShader->Begin(10);

	m_pVIBuffer->Render();
	// FAILED_CHECK_RETURN(m_pTarget_Manager->End_MRT(m_pContext, L""), E_FAIL);

	return S_OK;
}

HRESULT CRenderer::Render_ShadowBlur()
{

	D3D11_VIEWPORT			ViewPortDesc;
	ZeroMemory(&ViewPortDesc, sizeof(D3D11_VIEWPORT));

	_uint		iNumViewports = 1;

	m_pContext->RSGetViewports(&iNumViewports, &ViewPortDesc);

	m_pShader->Set_Matrix(L"g_WorldMatrix", &m_WorldMatrix);
	m_pShader->Set_Matrix(L"g_ViewMatrix", &m_ViewMatrix);
	m_pShader->Set_Matrix(L"g_ProjMatrix", &m_ProjMatrix);

	m_pShader->Set_RawValue(L"g_iWinCX", &ViewPortDesc.Width, sizeof(_float));
	m_pShader->Set_RawValue(L"g_iWinCY", &ViewPortDesc.Height, sizeof(_float));

	FAILED_CHECK_RETURN(m_pTarget_Manager->Begin_RenderTarget(m_pContext, TEXT("Target_HorizontalBlur")), E_FAIL);
	FAILED_CHECK_RETURN(m_pShader->Set_ShaderResourceView(L"g_DiffuseTexture_Deferred", m_pTarget_Manager->Get_SRV(L"Target_Shade")), E_FAIL);

	m_pShader->Begin(5);

	m_pVIBuffer->Render();

	FAILED_CHECK_RETURN(m_pTarget_Manager->End_MRT(m_pContext, L""), E_FAIL);

	FAILED_CHECK_RETURN(m_pTarget_Manager->Begin_RenderTarget(m_pContext, L"Target_VerticalBlur"), E_FAIL);
	FAILED_CHECK_RETURN(m_pShader->Set_ShaderResourceView(L"g_HorizontalBlurTexture", m_pTarget_Manager->Get_SRV(L"Target_HorizontalBlur")), E_FAIL);

	m_pShader->Begin(6);

	m_pVIBuffer->Render();

	FAILED_CHECK_RETURN(m_pTarget_Manager->End_MRT(m_pContext, L""), E_FAIL);

	FAILED_CHECK_RETURN(m_pTarget_Manager->Begin_RenderTarget(m_pContext, L"Target_Shade"), E_FAIL);

	FAILED_CHECK_RETURN(m_pShader->Set_ShaderResourceView(L"g_VerticalBlurTexture", m_pTarget_Manager->Get_SRV(L"Target_VerticalBlur")), E_FAIL);

	m_pShader->Begin(7);
	m_pVIBuffer->Render();

	FAILED_CHECK_RETURN(m_pTarget_Manager->End_MRT(m_pContext, L""), E_FAIL);

	return S_OK;
}

HRESULT CRenderer::Render_FADE()
{
	for (auto& pGameObject : m_RenderObjectList[RENDER_FADE])
	{
		if (nullptr != pGameObject)
			pGameObject->Render();

		Safe_Release(pGameObject);
	}

	m_RenderObjectList[RENDER_FADE].clear();

	return S_OK;
}

HRESULT CRenderer::Render_Glow()
{
	for (auto& pGameObject : m_RenderObjectList[RENDER_GLOW])
	{
		if (nullptr != pGameObject)
			pGameObject->Render();

		Safe_Release(pGameObject);
	}

	m_RenderObjectList[RENDER_GLOW].clear();



	return S_OK;
}
#ifdef _DEBUG
HRESULT CRenderer::Render_DebugObject()
{
	for (auto & pComponent : m_DebugObject)
	{
		if (nullptr != pComponent)
			pComponent->Render();

		Safe_Release(pComponent);
	}

	m_DebugObject.clear();
	

	FAILED_CHECK_RETURN(m_pShader->Set_Matrix(L"g_ViewMatrix", &m_ViewMatrix), E_FAIL);
	FAILED_CHECK_RETURN(m_pShader->Set_Matrix(L"g_ProjMatrix", &m_ProjMatrix), E_FAIL);


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
	Safe_Release(m_pLight_Manager);
	Safe_Release(m_pTarget_Manager);
	Safe_Release(m_pVIBuffer);
	Safe_Release(m_pShader);
}
