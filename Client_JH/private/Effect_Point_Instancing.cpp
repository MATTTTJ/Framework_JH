#include "stdafx.h"
#include "..\public\Effect_Point_Instancing.h"
#include "GameInstance.h"


CEffect_Point_Instancing::CEffect_Point_Instancing(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject(pDevice, pContext)
{
}

CEffect_Point_Instancing::CEffect_Point_Instancing(const CEffect_Point_Instancing& rhs)
	: CGameObject(rhs)
{
}

HRESULT CEffect_Point_Instancing::Initialize_Prototype()
{
	FAILED_CHECK_RETURN(__super::Initialize_Prototype(), E_FAIL);

	return S_OK;
}

HRESULT CEffect_Point_Instancing::Initialize_Clone(const wstring& wstrPrototypeTag, void* pArg)
{
	if(nullptr != pArg)
	{
		ZeroMemory(&m_tSparkOption, sizeof(SPARKOPTION));
		m_tSparkOption = *(SPARKOPTION*)pArg;
		m_tSparkOption.m_tGameObjectDesc.TransformDesc.fRotationPerSec = XMConvertToRadians(90.f);
		m_tSparkOption.m_tGameObjectDesc.TransformDesc.fSpeedPerSec = CGameUtils::GetRandomFloat(1.f, 3.f);
		GAMEOBJECTDESC tmp;
		ZeroMemory(&tmp, sizeof(GAMEOBJECTDESC));
		tmp.TransformDesc = m_tSparkOption.m_tGameObjectDesc.TransformDesc;
		FAILED_CHECK_RETURN(CGameObject::Initialize_Clone(wstrPrototypeTag, &tmp), E_FAIL);
		_float4 vPos = _float4(m_tSparkOption.m_tGameObjectDesc.TransformDesc.vInitPos.x, m_tSparkOption.m_tGameObjectDesc.TransformDesc.vInitPos.y, m_tSparkOption.m_tGameObjectDesc.TransformDesc.vInitPos.z, 1.f);
		
		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, vPos);
		_float4 vLook = XMVector3Normalize(_float4(m_tSparkOption.m_tGameObjectDesc.m_vBulletLook.x, m_tSparkOption.m_tGameObjectDesc.m_vBulletLook.y, m_tSparkOption.m_tGameObjectDesc.m_vBulletLook.z,0.f));
		_float4 Look4 = XMVector3Normalize(XMVectorSet(vLook.x + CGameUtils::GetRandomFloat(-1.f, 1.f), vLook.y + CGameUtils::GetRandomFloat(-1.f, 1.f), vLook.z + CGameUtils::GetRandomFloat(-1.f, 1.f), 0.f));
		_float4 min = { -1.0f, -1.0f, -1.0f, 0.f };
		_float4 max = _float4(1.0f, 1.0f, 1.0f, 0.f);
		_float4 Dir;
		CGameUtils::GetRandomVector(&Dir, &min, &max);

		m_pTransformCom->LookAt(m_pTransformCom->Get_State(CTransform::STATE_TRANSLATION) +  XMLoadFloat4(&Dir));
		m_iColorType = m_tSparkOption.m_eColor;
	}
	FAILED_CHECK_RETURN(SetUp_Components(), E_FAIL);
	m_vPSize = _float2(m_tSparkOption.m_tGameObjectDesc.m_vTexSize.x, m_tSparkOption.m_tGameObjectDesc.m_vTexSize.y);
	m_fSizeable = m_vPSize.x;
	m_fJumpPower = CGameUtils::GetRandomFloat(0.f, 1.f);
	m_fDownSpeed = CGameUtils::GetRandomFloat(2.f, 5.f);
	return S_OK;
}

void CEffect_Point_Instancing::Tick(_double dTimeDelta)
{
	__super::Tick(dTimeDelta);

	if (m_fSizeable < 0.f)
		Set_Dead(true);

	m_fSizeable -= _float(dTimeDelta) * 0.25f;

	m_vPSize = _float2(m_fSizeable, m_fSizeable);

	_float4 BoomPos = m_pTransformCom->Get_State(CTransform::STATE_TRANSLATION);

	BoomPos.y += m_fJumpPower * m_fDownSpeed * (_float)dTimeDelta;

	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, BoomPos);
	m_pTransformCom->Go_Straight(dTimeDelta, CTransform::TRANS_BULLET);
	m_fJumpPower += CGameUtils::GetRandomFloat(-0.1f, -0.25f);

	// m_pVIBufferCom->Tick(dTimeDelta);
}

void CEffect_Point_Instancing::Late_Tick(_double dTimeDelta)
{
	__super::Late_Tick(dTimeDelta);
	// __super::Compute_CamDistance();

	if (nullptr != m_pRendererCom)
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONLIGHT, this);
}

HRESULT CEffect_Point_Instancing::Render()
{
	FAILED_CHECK_RETURN(__super::Render(), E_FAIL);

	FAILED_CHECK_RETURN(SetUp_ShaderResources(), E_FAIL);

	m_pShaderCom->Begin(13);

	m_pVIBufferCom->Render();

	return S_OK;
}

HRESULT CEffect_Point_Instancing::SetUp_Components()
{
	FAILED_CHECK_RETURN(__super::Add_Component(CGameInstance::Get_StaticLevelIndex(), TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"),(CComponent**)&m_pRendererCom, this), E_FAIL);
	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxPointInstance"), TEXT("Com_Shader"),	(CComponent**)&m_pShaderCom, this), E_FAIL);
	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_VIBuffer_Bullet_Instancing"), TEXT("Com_VIBuffer"),	(CComponent**)&m_pVIBufferCom, this), E_FAIL);
	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Spark"), TEXT("Com_Texture"), (CComponent**)&m_pTexture_A_Com, this), E_FAIL);

	return S_OK;
}

HRESULT CEffect_Point_Instancing::SetUp_ShaderResources()
{
	NULL_CHECK_RETURN(m_pShaderCom, E_FAIL);
	FAILED_CHECK_RETURN(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, L"g_WorldMatrix"), E_FAIL);
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	FAILED_CHECK_RETURN(m_pShaderCom->Set_Matrix(L"g_ViewMatrix", &pGameInstance->Get_TransformFloat4x4(CPipeLine::D3DTS_VIEW)), E_FAIL);
	FAILED_CHECK_RETURN(m_pShaderCom->Set_Matrix(L"g_ProjMatrix", &pGameInstance->Get_TransformFloat4x4(CPipeLine::D3DTS_PROJ)), E_FAIL);
	FAILED_CHECK_RETURN(m_pShaderCom->Set_RawValue(L"g_vCamPosition", &pGameInstance->Get_CamPos(), sizeof(_float4)), E_FAIL);
	FAILED_CHECK_RETURN(m_pShaderCom->Set_RawValue(L"g_vPSize", &m_vPSize, sizeof(_float2)), E_FAIL);
	RELEASE_INSTANCE(CGameInstance);
	FAILED_CHECK_RETURN(m_pTexture_A_Com->Bind_ShaderResource(m_pShaderCom, L"g_Texture", (_uint)m_iColorType), E_FAIL);

	return S_OK;
}

CEffect_Point_Instancing* CEffect_Point_Instancing::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CEffect_Point_Instancing*		pInstance = new CEffect_Point_Instancing(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CEffect_Point_Instancing");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CEffect_Point_Instancing::Clone(const wstring& wstrPrototypeTag, void* pArg)
{
	CEffect_Point_Instancing*		pInstance = new CEffect_Point_Instancing(*this);

	if (FAILED(pInstance->Initialize_Clone(wstrPrototypeTag, pArg)))
	{
		MSG_BOX("Failed to Cloned : CEffect_Point_Instancing");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CEffect_Point_Instancing::Free()
{
	__super::Free();

	Safe_Release(m_pTexture_A_Com);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pRendererCom);
}
