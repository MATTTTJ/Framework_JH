#include "stdafx.h"
#include "..\public\Smoke.h"
#include "GameInstance.h"


CSmoke::CSmoke(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CGameObject(pDevice, pContext)
{
}

CSmoke::CSmoke(const CSmoke& rhs)
	: CGameObject(rhs)
{
}

HRESULT CSmoke::Initialize_Prototype()
{
	FAILED_CHECK_RETURN(__super::Initialize_Prototype(), E_FAIL);

	return S_OK;
}

HRESULT CSmoke::Initialize_Clone(const wstring& wstrPrototypeTag, void* pArg)
{
	if (nullptr != pArg)
	{
		m_tEffectDesc = *(GAMEOBJECTDESC*)pArg;

		// m_pOwner = m_tEffectDesc.m_pOwner;
		m_tEffectDesc.TransformDesc.fSpeedPerSec = 0.01f;
		FAILED_CHECK_RETURN(__super::Initialize_Clone(wstrPrototypeTag, &m_tEffectDesc), E_FAIL);
		FAILED_CHECK_RETURN(SetUp_Component(), E_FAIL);
		_float4 RandPos = _float4(m_tEffectDesc.TransformDesc.vInitPos.x + CGameUtils::GetRandomFloat(-3.f, 3.f), m_tEffectDesc.TransformDesc.vInitPos.y, m_tEffectDesc.TransformDesc.vInitPos.z + CGameUtils::GetRandomFloat(-1.f, 1.f), 1.f);
		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, RandPos);//(XMVectorSet(m_tEffectDesc.TransformDesc.vInitPos.x, m_tEffectDesc.TransformDesc.vInitPos.y, m_tEffectDesc.TransformDesc.vInitPos.z, 1.f)));
		m_iTextureIndex = m_tEffectDesc.m_iHP;
	}
	else
	{
		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(0.f, 0.f, 0.f, 1.f));
		m_pTransformCom->Set_Scaled(_float3(1000.f, 1000.f, 1.f));
	}

	if (m_iTextureIndex == 0)
	{
		m_vLook = CGameInstance::GetInstance()->Get_CamLook();
		_vector CamLook = XMVectorSet(m_vLook.x, m_vLook.y, m_vLook.z, 0.f);
		m_pTransformCom->LookAt(m_pTransformCom->Get_State(CTransform::STATE_TRANSLATION) + CamLook);
		m_pTransformCom->Rotation(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(m_tEffectDesc.TransformDesc.fRotationPerSec));
		m_pTransformCom->Set_Scaled(_float3(12.9f, 10.f, 0.f));
		m_iFrameCnt = 3;
		m_fAlpha = 0.7f;
		m_fMaxMoveDistance = 1.f;
		_float4 min = { -1.0f, 0.0f, 0.0f, 0.f };
		_float4 max = _float4(1.0f, 1.0f, 1.0f, 0.f);
		CGameUtils::GetRandomVector(&m_vDir, &min, &max);
		m_bFlowSmoke = false;
		 
	}
	else if (m_iTextureIndex == 1)
	{
		m_vLook = CGameInstance::GetInstance()->Get_CamLook();
		_vector CamLook = XMVectorSet(m_vLook.x, m_vLook.y, m_vLook.z, 0.f);
		m_pTransformCom->LookAt(m_pTransformCom->Get_State(CTransform::STATE_TRANSLATION) + CamLook);
		m_pTransformCom->Rotation(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(m_tEffectDesc.TransformDesc.fRotationPerSec));
		m_pTransformCom->Set_Scaled(_float3(105.8f, 12.1f, 0.f));
		m_iFrameCnt = 3;
		m_fAlpha = 0.7f;
		m_fMaxMoveDistance = 1.f;
		m_bFlowSmoke = true;

		_float4 min = { -1.0f, 0.0f, 0.0f, 0.f };
		_float4 max = _float4(1.0f, 1.0f, 1.0f, 0.f);
		CGameUtils::GetRandomVector(&m_vDir, &min, &max);
	}
	return S_OK;
}

void CSmoke::Tick(_double dTimeDelta)
{
	if (m_bIsDead == true)
		return;

	__super::Tick(dTimeDelta);

	m_fFlowTime += 0.05f;

	if (m_fFlowTime > 1.f)
		m_fFlowTime = 0.f;

	if (m_bCloseCam == false)
	{
		if (m_fMaxMoveDistance < 0.1f)
		{
			m_bChangeTime = true;
			// _float4 RandPos = _float4(m_tEffectDesc.TransformDesc.vInitPos.x + CGameUtils::GetRandomFloat(-1.f, 1.f), m_tEffectDesc.TransformDesc.vInitPos.y + CGameUtils::GetRandomFloat(-1.f, 1.f), m_tEffectDesc.TransformDesc.vInitPos.z + CGameUtils::GetRandomFloat(-1.f, 1.f), 1.f);
			// m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, RandPos);//(XMVectorSet(m_tEffectDesc.TransformDesc.vInitPos.x, m_tEffectDesc.TransformDesc.vInitPos.y, m_tEffectDesc.TransformDesc.vInitPos.z, 1.f)));

		}
		else if (m_fMaxMoveDistance > 0.6f)
		{
			m_bChangeTime = false;
		}

		if (m_bChangeTime == false)
		{
			m_fMaxMoveDistance -= (_float)dTimeDelta * CGameUtils::GetRandomFloat(0.1f, 0.3f);
		}
		else if (m_bChangeTime == true)
		{
			m_fMaxMoveDistance += (_float)dTimeDelta* CGameUtils::GetRandomFloat(0.1f, 0.3f);
		}
		// m_pTransformCom->Billboard();

		_float3 vLook = m_pTransformCom->Get_State(CTransform::STATE_LOOK);


		_vector vPos = m_pTransformCom->Get_State(CTransform::STATE_TRANSLATION);
		_vector vMovePos = vPos + XMVector3Normalize(XMVectorSet(m_vDir.x, m_vDir.y, m_vDir.z, 0.f)) * m_tEffectDesc.TransformDesc.fSpeedPerSec * (_float)dTimeDelta;

		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, vMovePos);
	}
	else
	{
		if (m_fMaxMoveDistance < 0.f)
			Set_Dead(true);

		m_fMaxMoveDistance -= (_float)dTimeDelta * 0.1f;
		
	}
	// m_fAlpha -= (_float)dTimeDelta * 3.f;

	// if (m_fAlpha < 0.f)
		// Set_Dead(true);

	// m_iPlayOnFrameCnt++;
	//
	// if (m_iPlayOnFrameCnt == 3)
	// 	Set_Dead(true);
}

void CSmoke::Late_Tick(_double dTimeDelta)
{
	__super::Late_Tick(dTimeDelta);

	__super::Compute_CamDistance();

	if (m_fCamDistance < 15.f)
		m_bCloseCam = true;

	if (m_pRendererCom != nullptr)
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_EFFECT, this);
}

HRESULT CSmoke::Render()
{
	FAILED_CHECK_RETURN(__super::Render(), E_FAIL);

	FAILED_CHECK_RETURN(SetUp_ShaderResources(), E_FAIL);

	m_pShaderCom->Begin(6);

	m_pVIBufferCom->Render();

	return S_OK;
}

HRESULT CSmoke::SetUp_Component()
{
	FAILED_CHECK_RETURN(__super::Add_Component(CGameInstance::Get_StaticLevelIndex(), L"Prototype_Component_Renderer", L"Com_Renderer", (CComponent**)&m_pRendererCom, this), E_FAIL);
	FAILED_CHECK_RETURN(__super::Add_Component(CGameInstance::Get_StaticLevelIndex(), L"Prototype_Component_Shader_VtxTex", L"Com_Shader", (CComponent**)&m_pShaderCom, this), E_FAIL);
	FAILED_CHECK_RETURN(__super::Add_Component(CGameInstance::Get_StaticLevelIndex(), L"Prototype_Component_VIBuffer_Rect", L"Com_VIBuffer", (CComponent**)&m_pVIBufferCom, this), E_FAIL);
	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Smoke", L"Com_Texture", (CComponent**)&m_pTextureCom, this), E_FAIL);

	return S_OK;
}

HRESULT CSmoke::SetUp_ShaderResources()
{
	NULL_CHECK_RETURN(m_pShaderCom, E_FAIL);

	FAILED_CHECK_RETURN(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, L"g_WorldMatrix"), E_FAIL);
	FAILED_CHECK_RETURN(m_pShaderCom->Set_Matrix(L"g_ViewMatrix", &CGameInstance::GetInstance()->Get_TransformFloat4x4(CPipeLine::D3DTS_VIEW)), E_FAIL);
	FAILED_CHECK_RETURN(m_pShaderCom->Set_Matrix(L"g_ProjMatrix", &CGameInstance::GetInstance()->Get_TransformFloat4x4(CPipeLine::D3DTS_PROJ)), E_FAIL);
	FAILED_CHECK_RETURN(m_pShaderCom->Set_ShaderResourceView(L"g_DepthTexture", CGameInstance::GetInstance()->Get_DepthTargetSRV()), E_FAIL);
	FAILED_CHECK_RETURN(m_pShaderCom->Set_RawValue(L"g_fAlpha", &m_fMaxMoveDistance, sizeof(_float)), E_FAIL);
	FAILED_CHECK_RETURN(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, L"g_Texture", m_iTextureIndex), E_FAIL);
	if(m_iTextureIndex == 1)
	{
		FAILED_CHECK_RETURN(m_pShaderCom->Set_RawValue(L"g_bFlow", &m_bFlowSmoke, sizeof(_bool)), E_FAIL);
		FAILED_CHECK_RETURN(m_pShaderCom->Set_RawValue(L"g_fTime", &m_fFlowTime, sizeof(_float)), E_FAIL);
	}
	else
	{
		FAILED_CHECK_RETURN(m_pShaderCom->Set_RawValue(L"g_bFlow", &m_bFlowSmoke, sizeof(_bool)), E_FAIL);
	}
	return S_OK;
}

CSmoke* CSmoke::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CSmoke*		pInstance = new CSmoke(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CSmoke");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CSmoke::Clone(const wstring& wstrPrototypeTag, void* pArg)
{
	CSmoke*		pInstance = new CSmoke(*this);

	if (FAILED(pInstance->Initialize_Clone(wstrPrototypeTag, pArg)))
	{
		MSG_BOX("Failed to Cloned : CSmoke");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CSmoke::Free()
{
	__super::Free();
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pRendererCom);
}
