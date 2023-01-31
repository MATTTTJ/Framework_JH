#include "stdafx.h"
#include "..\public\Effect_Point.h"
#include "GameInstance.h"


CEffect_Point::CEffect_Point(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CEffect(pDevice, pContext)
{
}

CEffect_Point::CEffect_Point(const CEffect_Point& rhs)
	: CEffect(rhs)
{
}

HRESULT CEffect_Point::Initialize_Prototype()
{
	return S_OK;

}

HRESULT CEffect_Point::Initialize_Clone(const wstring& wstrPrototypeTag, void* pArg)
{
	FAILED_CHECK_RETURN(__super::Initialize_Clone(wstrPrototypeTag, &pArg), E_FAIL);
	return S_OK;
}

void CEffect_Point::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
}

void CEffect_Point::Late_Tick(_float fTimeDelta)
{
	__super::Late_Tick(fTimeDelta);

	m_fCamDistance += 1.f;
}

HRESULT CEffect_Point::Render()
{
	__super::Render();

	if (nullptr == m_pShaderCom ||
		nullptr == m_pVIBufferCom)
		return E_FAIL;

	FAILED_CHECK_RETURN(SetUp_ShaderResource(), E_FAIL);

	m_pShaderCom->Begin(m_tEffect.iPassIndex);

	m_pVIBufferCom->Render();
	return S_OK;
}

HRESULT CEffect_Point::SetUp_Components()
{
	__super::SetUp_Components();

	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Shader_VtxPointInstance", L"Com_Shader", (CComponent**)&m_pShaderCom, this), E_FAIL;)

	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Texture_Effect", L"Com_Texture", (CComponent**)&m_pTextureCom, this), E_FAIL;)

	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_VIBuffer_Point", L"Com_VIBuffer", (CComponent**)&m_pVIBufferCom, this), E_FAIL);



	return S_OK;
}

HRESULT CEffect_Point::SetUp_ShaderResource()
{
	if (nullptr == m_pShaderCom)
		return E_FAIL;

	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	FAILED_CHECK_RETURN(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, L"g_WorldMatrix"), E_FAIL);

	FAILED_CHECK_RETURN(m_pShaderCom->Set_Matrix(L"g_ViewMatrix", &pGameInstance->Get_TransformFloat4x4(CPipeLine::D3DTS_VIEW)), E_FAIL);
	FAILED_CHECK_RETURN(m_pShaderCom->Set_Matrix(L"g_ProjMatrix", &pGameInstance->Get_TransformFloat4x4(CPipeLine::D3DTS_PROJ)), E_FAIL);
	FAILED_CHECK_RETURN(m_pShaderCom->Set_RawValue(L"g_fRatio", &m_fRatio, sizeof(_float)), E_FAIL);
	FAILED_CHECK_RETURN(m_pShaderCom->Set_RawValue(L"g_bBloom", &m_tEffect.bIsBloom, sizeof(_bool)), E_FAIL);
	
	FAILED_CHECK_RETURN(m_pShaderCom->Set_RawValue(L"g_bBlur", &m_tEffect.bIsBlur, sizeof(_bool)), E_FAIL);
	FAILED_CHECK_RETURN(m_pShaderCom->Set_RawValue(L"g_bGlow", &m_tEffect.bIsGlow, sizeof(_bool)), E_FAIL);
	FAILED_CHECK_RETURN(m_pShaderCom->Set_RawValue(L"g_vUVScale", &m_tEffect.vUVScale, sizeof(_float2)), E_FAIL);
	
	if (m_tEffect.bIsSprite)
	{
		FAILED_CHECK_RETURN(m_pShaderCom->Set_RawValue(L"g_vUVPos", &m_vSpriteUVPos, sizeof(_float2)), E_FAIL);
	}
	else
	{
		FAILED_CHECK_RETURN(m_pShaderCom->Set_RawValue(L"g_vUVPos", &m_tEffect.vUVPos, sizeof(_float2)), E_FAIL);
	}
	FAILED_CHECK_RETURN(m_pShaderCom->Set_RawValue(L"g_vColor", &m_tEffect.vColor, sizeof(_float3)), E_FAIL);
	FAILED_CHECK_RETURN(m_pShaderCom->Set_RawValue(L"g_fAlphaRatio", &m_fAlphaRatio, sizeof(_float)), E_FAIL);

	FAILED_CHECK_RETURN(m_pShaderCom->Set_RawValue(L"g_vCamPosition", &pGameInstance->Get_CamPos(), sizeof(_float4)), E_FAIL);
	FAILED_CHECK_RETURN(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, L"g_DiffuseTexture", m_tEffect.iTextureIndex), E_FAIL);

	RELEASE_INSTANCE(CGameInstance);
	return S_OK;
}

CEffect_Point* CEffect_Point::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CEffect_Point*		pInstance = new CEffect_Point(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CEffect_Point");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CEffect_Point::Clone(const wstring& wstrPrototypeTag, void* pArg)
{
	CEffect_Point*		pInstance = new CEffect_Point(*this);

	if (FAILED(pInstance->Initialize_Clone(wstrPrototypeTag, pArg)))
	{
		MSG_BOX("Failed to Cloned : CEffect_Point");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CEffect_Point::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pVIBufferCom);
}
