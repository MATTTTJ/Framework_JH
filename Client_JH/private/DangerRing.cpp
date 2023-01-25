#include "stdafx.h"
#include "..\public\DangerRing.h"
#include "GameInstance.h"

CDangerRing::CDangerRing(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject(pDevice, pContext)
{
}

CDangerRing::CDangerRing(const CDangerRing& rhs)
	: CGameObject(rhs)
{
}

HRESULT CDangerRing::Initialize_Prototype()
{
	FAILED_CHECK_RETURN(__super::Initialize_Prototype(), E_FAIL);

	return S_OK;
}

HRESULT CDangerRing::Initialize_Clone(const wstring& wstrPrototypeTag, void* pArg)
{
	ZeroMemory(&m_tRingDesc, sizeof(RINGDESC));

	if(nullptr != pArg)
	{
		m_tRingDesc = *(RINGDESC*)pArg;
		FAILED_CHECK_RETURN(__super::Initialize_Clone(wstrPrototypeTag, &m_tRingDesc), E_FAIL);

		_float4 InitPos = _float4(m_tRingDesc.GameObjectDesc.TransformDesc.vInitPos.x, m_tRingDesc.GameObjectDesc.TransformDesc.vInitPos.y, m_tRingDesc.GameObjectDesc.TransformDesc.vInitPos.z, 1.f);

		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, InitPos);
		
		
		if (m_tRingDesc.m_eType == tagRingDesc::RINGTYPE_PILLAR)
		{
			if (m_tRingDesc.GameObjectDesc.m_iHP == 1)
			{
				m_bBaseRing = false;
				m_fCurSize = m_tRingDesc.m_fMinSize;
				m_fMaxSize = m_tRingDesc.m_fMaxSize;
				m_pTransformCom->Set_Scaled(_float3(m_fMaxSize, m_fCurSize, m_fCurSize));
			}
			else
			{
				m_bBaseRing = true;
				m_fMaxSize = m_tRingDesc.m_fMaxSize;
				m_fMaxSize = m_tRingDesc.m_fMaxSize;
				m_pTransformCom->Set_Scaled(_float3(m_fMaxSize, m_fMaxSize, m_fMaxSize));
			}
		}
		else if(m_tRingDesc.m_eType == tagRingDesc::RINGTYPE_ROCKETARM)
		{
			if (m_tRingDesc.GameObjectDesc.m_iHP == 1)
			{
				m_bBaseRing = false;
				m_fCurSize = m_tRingDesc.m_fMinSize;
				m_fMaxSize = m_tRingDesc.m_fMaxSize;
				m_pTransformCom->Set_Scaled(_float3(m_fMaxSize, m_fCurSize, m_fCurSize));
			}
			else
			{
				m_bBaseRing = true;
				m_fCurSize = m_tRingDesc.m_fMinSize;
				m_fMaxSize = m_tRingDesc.m_fMaxSize;
				m_pTransformCom->Set_Scaled(_float3(m_fMaxSize, m_fMaxSize, m_fMaxSize));
			}
		}
	}
	else
	{
		FAILED_CHECK_RETURN(__super::Initialize_Clone(wstrPrototypeTag, &m_tRingDesc), E_FAIL);

		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, _float4(0.f, 0.f, 0.f, 1.f));
		m_pTransformCom->Set_Scaled(_float3(10.f, 10.f, 10.f));

	}
	m_fLifeTime = 2.f;
	m_pTransformCom->Rotation(XMVectorSet(1.f, 0.f, 0.f, 0.f), XMConvertToRadians(90.f));
	FAILED_CHECK_RETURN(SetUp_Components(), E_FAIL);

	return S_OK;
}

void CDangerRing::Tick(_double TimeDelta)
{
	if (m_bIsDead)
		return;

	__super::Tick(TimeDelta);

	{
		m_fCurLifeTime += TimeDelta;
	}

	if(m_fCurLifeTime >= m_fLifeTime)
	{
		Set_Dead(true);
	}

	if (m_bBaseRing == false)
	{
		if (m_fCurSize < m_fMaxSize)
		{
			if (m_tRingDesc.m_eType == tagRingDesc::RINGTYPE_PILLAR)
			{
				m_fCurSize += (_float)(TimeDelta * 8.f);
			}
			else if (m_tRingDesc.m_eType == tagRingDesc::RINGTYPE_ROCKETARM)
			{
				m_fCurSize += (_float)(TimeDelta * 20.f);
			}

			_float3 Size = _float3(m_fCurSize, m_fCurSize, m_fMaxSize);
			m_pTransformCom->Set_Scaled(Size);
		}
		else
		{
			m_fCurSize = m_tRingDesc.m_fMinSize;
		}
	}
	else
		m_pTransformCom->Set_Scaled(_float3(m_fMaxSize, m_fMaxSize, m_fMaxSize));

	
}

void CDangerRing::Late_Tick(_double TimeDelta)
{
	__super::Late_Tick(TimeDelta);

	if (nullptr != m_pRendererCom)
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONLIGHT, this);
}

HRESULT CDangerRing::Render()
{
	FAILED_CHECK_RETURN(__super::Render(), E_FAIL);
	FAILED_CHECK_RETURN(SetUp_ShaderResources(), E_FAIL);

	m_pShaderCom->Begin(9);

	m_pVIBufferCom->Render();

	return S_OK;
}

HRESULT CDangerRing::SetUp_Components()
{
	FAILED_CHECK_RETURN(__super::Add_Component(CGameInstance::Get_StaticLevelIndex(), L"Prototype_Component_Renderer", L"Com_Renderer", (CComponent**)&m_pRendererCom, this), E_FAIL);
	FAILED_CHECK_RETURN(__super::Add_Component(CGameInstance::Get_StaticLevelIndex(), L"Prototype_Component_Shader_VtxTex", L"Com_Shader", (CComponent**)&m_pShaderCom, this), E_FAIL);
	FAILED_CHECK_RETURN(__super::Add_Component(CGameInstance::Get_StaticLevelIndex(), L"Prototype_Component_VIBuffer_Rect", L"Com_VIBuffer", (CComponent**)&m_pVIBufferCom, this), E_FAIL);
	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Danger_Ring", L"Com_Texture", (CComponent**)&m_pTextureCom, this), E_FAIL);

	return S_OK;
}

HRESULT CDangerRing::SetUp_ShaderResources()
{
	NULL_CHECK_RETURN(m_pShaderCom, E_FAIL);

	FAILED_CHECK_RETURN(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, L"g_WorldMatrix"), E_FAIL);

	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);
	FAILED_CHECK_RETURN(m_pShaderCom->Set_Matrix(L"g_ViewMatrix", &pGameInstance->Get_TransformFloat4x4(CPipeLine::D3DTS_VIEW)), E_FAIL);
	FAILED_CHECK_RETURN(m_pShaderCom->Set_Matrix(L"g_ProjMatrix", &pGameInstance->Get_TransformFloat4x4(CPipeLine::D3DTS_PROJ)), E_FAIL);

	RELEASE_INSTANCE(CGameInstance);
	FAILED_CHECK_RETURN(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, L"g_Texture"), E_FAIL);

	return S_OK;
}

CDangerRing* CDangerRing::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CDangerRing*		pInstance = new CDangerRing(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CDangerRing");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CDangerRing::Clone(const wstring& wstrPrototypeTag, void* pArg)
{
	CDangerRing*		pInstance = new CDangerRing(*this);

	if (FAILED(pInstance->Initialize_Clone(wstrPrototypeTag, pArg)))
	{
		MSG_BOX("Failed to Cloned : CDangerRing");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CDangerRing::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pRendererCom);

}
