#include "stdafx.h"
#include "..\public\SkySphere.h"
#include "GameInstance.h"


CSkySphere::CSkySphere(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject(pDevice, pContext)
{
}

CSkySphere::CSkySphere(const CSkySphere& rhs)
	: CGameObject(rhs)
{
}

HRESULT CSkySphere::Initialize_Prototype()
{
	FAILED_CHECK_RETURN(__super::Initialize_Prototype(), E_FAIL);

	return S_OK;
}

HRESULT CSkySphere::Initialize_Clone(const wstring& wstrPrototypeTag, void* pArg)
{

	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	if (nullptr != pArg)
		memcpy(&TransformDesc, pArg, sizeof(CTransform::TRANSFORMDESC));
	else
	{
		TransformDesc.fSpeedPerSec = 15.0f;
		TransformDesc.fRotationPerSec = XMConvertToRadians(90.f);
	}

	FAILED_CHECK_RETURN(__super::Initialize_Clone(wstrPrototypeTag, pArg), E_FAIL);

	FAILED_CHECK_RETURN(SetUp_Component(), E_FAIL);

	return S_OK;
}

void CSkySphere::Tick(_double dTimeDelta)
{
	__super::Tick(dTimeDelta);
	// m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMLoadFloat4(&CGameInstance::GetInstance()->Get_CamPos()));

}

void CSkySphere::Late_Tick(_double dTimeDelta)
{
	__super::Late_Tick(dTimeDelta);

	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMLoadFloat4(&CGameInstance::GetInstance()->Get_CamPos()));

	if (nullptr != m_pRendererCom)
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_PRIORITY, this);
}

HRESULT CSkySphere::Render()
{
	FAILED_CHECK_RETURN(__super::Render(), E_FAIL);

	FAILED_CHECK_RETURN(SetUp_ShaderResource(), E_FAIL);

	m_pShaderCom->Begin(10);

	m_pVIBufferCom->Render();

	return S_OK;
}

HRESULT CSkySphere::SetUp_Component()
{
	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_SphereTex", L"Com_Texture", (CComponent**)&m_pTextureCom, this), E_FAIL);

	FAILED_CHECK_RETURN(__super::Add_Component(CGameInstance::Get_StaticLevelIndex(), L"Prototype_Component_Shader_VtxTex", L"Com_Shader", (CComponent**)&m_pShaderCom, this), E_FAIL);

	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_VIBuffer_Sphere", L"Com_VIBuffer", (CComponent**)&m_pVIBufferCom, this), E_FAIL);

	FAILED_CHECK_RETURN(__super::Add_Component(CGameInstance::Get_StaticLevelIndex(), L"Prototype_Component_Renderer", L"Com_Renderer", (CComponent**)&m_pRendererCom, this), E_FAIL);

	return S_OK;
}

HRESULT CSkySphere::SetUp_ShaderResource()
{
	NULL_CHECK_RETURN(m_pShaderCom, E_FAIL);

	CGameInstance*		pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	FAILED_CHECK_RETURN(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, L"g_WorldMatrix"), E_FAIL);

	FAILED_CHECK_RETURN(m_pShaderCom->Set_Matrix(L"g_ViewMatrix", &pGameInstance->Get_TransformFloat4x4(CPipeLine::D3DTS_VIEW)), E_FAIL);

	FAILED_CHECK_RETURN(m_pShaderCom->Set_Matrix(L"g_ProjMatrix", &pGameInstance->Get_TransformFloat4x4(CPipeLine::D3DTS_PROJ)), E_FAIL);

	FAILED_CHECK_RETURN(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, L"g_Texture"), E_FAIL);

	const LIGHTDESC*	pLightDesc = pGameInstance->Get_LightDesc(0);
	NULL_CHECK_RETURN(pLightDesc, E_FAIL);

	m_pShaderCom->Set_RawValue(L"g_vLightDir", &pLightDesc->vDirection, sizeof(_float4));
	m_pShaderCom->Set_RawValue(L"g_vLightDiffuse", &pLightDesc->vDiffuse, sizeof(_float4));
	m_pShaderCom->Set_RawValue(L"g_vLightAmbient", &pLightDesc->vAmbient, sizeof(_float4));
	m_pShaderCom->Set_RawValue(L"g_vLightSpecular", &pLightDesc->vSpecular, sizeof(_float4));

	Safe_Release(pGameInstance);

	return S_OK;
}

CSkySphere* CSkySphere::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CSkySphere *	pInstance = new CSkySphere(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Create : CSkySphere");
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

CGameObject* CSkySphere::Clone(const wstring& wstrPrototypeTag, void* pArg)
{
	CSkySphere *	pInstance = new CSkySphere(*this);

	if (FAILED(pInstance->Initialize_Clone(wstrPrototypeTag, pArg)))
	{
		MSG_BOX("Failed to Clone : CSkySphere");
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

void CSkySphere::Free()
{
	__super::Free();

	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pTextureCom);
}
