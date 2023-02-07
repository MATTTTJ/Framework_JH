#include "stdafx.h"
#include "..\public\Dust.h"
#include "GameInstance.h"
#include "Player.h"

CDust::CDust(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CGameObject(pDevice, pContext)
{
}

CDust::CDust(const CDust& rhs)
	: CGameObject(rhs)
{
}

HRESULT CDust::Initialize_Prototype()
{
	FAILED_CHECK_RETURN(__super::Initialize_Prototype(), E_FAIL);

	return S_OK;
}

HRESULT CDust::Initialize_Clone(const wstring& wstrPrototypeTag, void* pArg)
{
	if (nullptr != pArg)
	{
		m_tEffectDesc = *(EFFECTDESC*)pArg;

		// m_pOwner = m_tEffectDesc.m_pOwner;
		FAILED_CHECK_RETURN(__super::Initialize_Clone(wstrPrototypeTag, &m_tEffectDesc), E_FAIL);
		FAILED_CHECK_RETURN(SetUp_Component(), E_FAIL);

		m_vPSize = _float2{ 0.5f, 0.5f };
		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, (XMVectorSet(m_tEffectDesc.m_tGameObjectDesc.TransformDesc.vInitPos.x, m_tEffectDesc.m_tGameObjectDesc.TransformDesc.vInitPos.y, m_tEffectDesc.m_tGameObjectDesc.TransformDesc.vInitPos.z, 1.f)));
	}
	else
	{
		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(0.f, 0.f, 0.f, 1.f));
		m_pTransformCom->Set_Scaled(_float3(1000.f, 1000.f, 1.f));
	}

	_float4 vUp = m_pTransformCom->Get_State(CTransform::STATE_UP);
	// m_pTransformCom->Rotation(m_pTransformCom->Get_State(CTransform::STATE_RIGHT), XMConvertToRadians(45.f));
	m_vLook = XMVector3Normalize(XMVector3Cross(m_vDir, vUp));

	m_iUV_Max_Width_Num = 3;
	m_iUV_Max_Height_Num = 4;
	m_iFrameCnt = 3;
	m_vPSize = _float2{ 2.f, 2.f };
	m_iUV_Cur_Width_Num = 0;
	m_iUV_Cur_Height_Num = 0;
	m_fAlpha = 0.7f;
	return S_OK;
}

void CDust::Tick(_double dTimeDelta)
{
	if (m_bIsDead == true)
		return;

	__super::Tick(dTimeDelta);

	if (m_fAlpha < 0.f)
		Set_Dead(true);

	m_fAlpha -= (_float)dTimeDelta;

	// m_vPSize = _float2(m_vPSize.x + (_float)dTimeDelta, m_vPSize.y + (_float)dTimeDelta);

	m_iPlayOnFrameCnt++;

	if(m_iPlayOnFrameCnt == m_iFrameCnt)
	{
		if (m_iUV_Cur_Height_Num == m_iUV_Max_Height_Num - 1 && m_iUV_Cur_Width_Num == m_iUV_Max_Width_Num - 1)
		{
			Set_Dead(true);
			return;
		}
	
		m_iUV_Cur_Width_Num++;
	
		m_iPlayOnFrameCnt = 0;
	
		if(m_iUV_Cur_Width_Num == m_iUV_Max_Width_Num)
		{
			m_iUV_Cur_Width_Num = 0;
			m_iUV_Cur_Height_Num++;
		}
	
		
		// if(m_iUV_Cur_Height_Num == m_iUV_Max_Height_Num)
		// {
		// 	m_iUV_Cur_Height_Num = 0;
		// }
	
		
	}


}

void CDust::Late_Tick(_double dTimeDelta)
{
	__super::Late_Tick(dTimeDelta);
	__super::Compute_CamDistance();

	if (m_pRendererCom != nullptr)
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_EFFECT, this);
}

HRESULT CDust::Render()
{
	FAILED_CHECK_RETURN(__super::Render(), E_FAIL);

	FAILED_CHECK_RETURN(SetUp_ShaderResources(), E_FAIL);

	m_pShaderCom->Begin(11);

	m_pPointBuffer->Render();

	return S_OK;
}

HRESULT CDust::SetUp_Component()
{
	FAILED_CHECK_RETURN(__super::Add_Component(CGameInstance::Get_StaticLevelIndex(), L"Prototype_Component_Renderer", L"Com_Renderer", (CComponent**)&m_pRendererCom, this), E_FAIL);
	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Shader_VtxPointInstance", L"Com_Shader", (CComponent**)&m_pShaderCom, this), E_FAIL);
	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_VIBuffer_Point_Instancing", L"Com_VIBuffer", (CComponent**)&m_pPointBuffer, this), E_FAIL);
	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_DustTexture_Noise", L"Com_NoiseTexture", (CComponent**)&m_pNoiseTextureCom, this), E_FAIL);
	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_DustTexture_Diffuse", L"Com_DiffuseTexture", (CComponent**)&m_pTextureCom, this), E_FAIL);
	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_DustTexture_Alpha", L"Com_AlphaTexture", (CComponent**)&m_pAlphaTextureCom, this), E_FAIL);



	return S_OK;
}

HRESULT CDust::SetUp_ShaderResources()
{
	
	NULL_CHECK_RETURN(m_pShaderCom, E_FAIL);

	FAILED_CHECK_RETURN(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, L"g_WorldMatrix"), E_FAIL);
	FAILED_CHECK_RETURN(m_pShaderCom->Set_Matrix(L"g_ViewMatrix", &CGameInstance::GetInstance()->Get_TransformFloat4x4(CPipeLine::D3DTS_VIEW)), E_FAIL);
	FAILED_CHECK_RETURN(m_pShaderCom->Set_Matrix(L"g_ProjMatrix", &CGameInstance::GetInstance()->Get_TransformFloat4x4(CPipeLine::D3DTS_PROJ)), E_FAIL);
	// FAILED_CHECK_RETURN(m_pShaderCom->Set_RawValue(L"g_vLook", &m_vLook, sizeof(_float4)), E_FAIL);
	// FAILED_CHECK_RETURN(m_pShaderCom->Set_RawValue(L"g_vUp", &m_vUp, sizeof(_float4)), E_FAIL);
	FAILED_CHECK_RETURN(m_pShaderCom->Set_RawValue(L"g_iUV_Max_Width_Num", &m_iUV_Max_Width_Num, sizeof(_int)), E_FAIL);
	FAILED_CHECK_RETURN(m_pShaderCom->Set_RawValue(L"g_iUV_Max_Height_Num", &m_iUV_Max_Height_Num, sizeof(_int)), E_FAIL);
	FAILED_CHECK_RETURN(m_pShaderCom->Set_RawValue(L"g_iUV_Cur_Width_Num", &m_iUV_Cur_Width_Num, sizeof(_int)), E_FAIL);
	FAILED_CHECK_RETURN(m_pShaderCom->Set_RawValue(L"g_iUV_Cur_Height_Num", &m_iUV_Cur_Height_Num, sizeof(_int)), E_FAIL);
	FAILED_CHECK_RETURN(m_pShaderCom->Set_RawValue(L"g_vPSize", &m_vPSize, sizeof(_float2)), E_FAIL);
	FAILED_CHECK_RETURN(m_pShaderCom->Set_RawValue(L"g_fDustAlpha", &m_fAlpha, sizeof(_float)), E_FAIL);

	if (FAILED(m_pShaderCom->Set_RawValue(L"g_vCamPosition", &CGameInstance::GetInstance()->Get_CamPos(), sizeof(_float4))))
		return E_FAIL;
	FAILED_CHECK_RETURN(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, L"g_Texture"), E_FAIL);
	FAILED_CHECK_RETURN(m_pNoiseTextureCom->Bind_ShaderResource(m_pShaderCom, L"g_NoiseTexture"), E_FAIL);
	FAILED_CHECK_RETURN(m_pAlphaTextureCom->Bind_ShaderResource(m_pShaderCom, L"g_AlphaTexture"), E_FAIL);

	return S_OK;
}

CDust* CDust::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CDust*		pInstance = new CDust(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CDust");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CDust::Clone(const wstring& wstrPrototypeTag, void* pArg)
{
	CDust*		pInstance = new CDust(*this);

	if (FAILED(pInstance->Initialize_Clone(wstrPrototypeTag, pArg)))
	{
		MSG_BOX("Failed to Cloned : CDust");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CDust::Free()
{
	__super::Free();
	Safe_Release(m_pNoiseTextureCom);
	Safe_Release(m_pAlphaTextureCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pPointBuffer);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pRendererCom);
}
