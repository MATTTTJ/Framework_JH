#include "stdafx.h"
#include "..\public\Sky.h"
#include "GameInstance.h"

CSky::CSky(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CGameObject(pDevice,pContext)
{
}

CSky::CSky(const CSky& rhs)
	: CGameObject(rhs)
{
}

HRESULT CSky::Initialize_Prototype()
{
	FAILED_CHECK_RETURN(__super::Initialize_Prototype(), E_FAIL);

	return S_OK;
}

HRESULT CSky::Initialize_Clone(const wstring& wstrPrototypeTag, void* pArg)
{


	if(nullptr != pArg)
	{
		GAMEOBJECTDESC GameObjectDesc;
		ZeroMemory(&GameObjectDesc, sizeof(GAMEOBJECTDESC));
		GameObjectDesc = *(GAMEOBJECTDESC*)pArg;
		GameObjectDesc.TransformDesc.fSpeedPerSec = 15.f;
		GameObjectDesc.TransformDesc.fRotationPerSec = XMConvertToRadians(60.f);
		FAILED_CHECK_RETURN(__super::Initialize_Clone(wstrPrototypeTag, &GameObjectDesc), E_FAIL);

		m_iLightNumber = GameObjectDesc.m_iNumber;

		m_pTransformCom->Set_Scaled(_float3(0.3f, 0.3f, 0.3f));
		_float4 InitPos = m_vInitPos= XMVectorSet(GameObjectDesc.TransformDesc.vInitPos.x + CGameUtils::GetRandomFloat(-0.1f, 0.1f), GameObjectDesc.TransformDesc.vInitPos.y + CGameUtils::GetRandomFloat(-0.1f, 0.1f), GameObjectDesc.TransformDesc.vInitPos.z + CGameUtils::GetRandomFloat(-0.1f, 0.1f), 1.f);
		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, InitPos);
		 m_fUpSpeed = CGameUtils::GetRandomFloat(0.1f, 1.f);


	}

	


	FAILED_CHECK_RETURN(SetUp_Components(), E_FAIL);

	return S_OK;
}

void CSky::Tick(_double dTimeDelta)
{
	if (CGameInstance::GetInstance()->Get_LightDesc(m_iLightNumber)->isEnable == false)
	{
		Set_Dead(true);
	}

	__super::Tick(dTimeDelta);

	// m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMLoadFloat4(&CGameInstance::GetInstance()->Get_CamPos()));
	m_pTransformCom->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), (_float)dTimeDelta);
	m_pTransformCom->Turn(XMVectorSet(0.f,0.f,1.f,0.f), (_float)dTimeDelta);

	if (m_fMaxRange <= 1.f && m_bRangeChange == false && m_bIsDead == false)
	{
		m_fMaxRange += (_float)dTimeDelta * 2.f;

		if (m_fMaxRange > 1.f)
			m_bRangeChange = true;

		if (m_bRangeChange == false)
			CGameInstance::GetInstance()->Set_LightRange(m_iLightNumber, 8.f + m_fMaxRange);
	}
	else if (m_bRangeChange == true)
	{
		m_fMaxRange -= (_float)dTimeDelta * 4.f;

		if (m_fMaxRange < 0.f)
			m_bRangeChange = false;

		CGameInstance::GetInstance()->Set_LightRange(m_iLightNumber, 8.f + m_fMaxRange);
	}

	if (m_vColorDown < 0.f)
	{
		m_vColorDown = 1.f;
		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, m_vInitPos);
		m_fSize = 0.3f;
		m_pTransformCom->Set_Scaled(_float3(m_fSize, m_fSize, m_fSize));
	}

	m_fSize -= (_float)dTimeDelta * 0.1f;

	if(m_fSize < 0.f)
	{
		m_fSize = 0.f;
	}

	m_pTransformCom->Set_Scaled(_float3(m_fSize, m_fSize, m_fSize));





	

	m_vColorDown -= (_float)dTimeDelta * m_fUpSpeed;

	_float4 Pos = m_pTransformCom->Get_State(CTransform::STATE_TRANSLATION);
	Pos.y += (_float)dTimeDelta * m_fUpSpeed;
	
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, Pos);

}

void CSky::Late_Tick(_double dTimeDelta)
{
	__super::Late_Tick(dTimeDelta);

	if (nullptr != m_pRendererCom)
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_EFFECT, this);
}

HRESULT CSky::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	if (FAILED(SetUp_ShaderResources()))
		return E_FAIL;

	m_pShaderCom->Begin(0);

	m_pVIBufferCom->Render();

	return S_OK;
}

HRESULT CSky::SetUp_Components()
{
	/* For.Com_Renderer */
	FAILED_CHECK_RETURN(__super::Add_Component(CGameInstance::Get_StaticLevelIndex(), L"Prototype_Component_Renderer", L"Com_Renderer", (CComponent**)&m_pRendererCom, this), E_FAIL);
	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Shader_VtxCubeTex", L"Com_Shader", (CComponent**)&m_pShaderCom, this), E_FAIL)
	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_BlueFire_Lamp", L"Com_Texture",	(CComponent**)&m_pTextureCom, this), E_FAIL);
	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_VIBuffer_Cube", L"Com_VIBuffer",	(CComponent**)&m_pVIBufferCom, this), E_FAIL);
	
	return S_OK;
}

HRESULT CSky::SetUp_ShaderResources()
{
	NULL_CHECK_RETURN(m_pShaderCom, E_FAIL);
	FAILED_CHECK_RETURN(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, L"g_WorldMatrix"), E_FAIL);
	FAILED_CHECK_RETURN(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, L"g_Texture"), E_FAIL);
	
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);
	FAILED_CHECK_RETURN(m_pShaderCom->Set_Matrix(L"g_ViewMatrix", &pGameInstance->Get_TransformFloat4x4(CPipeLine::D3DTS_VIEW)), E_FAIL);
	FAILED_CHECK_RETURN(m_pShaderCom->Set_Matrix(L"g_ProjMatrix", &pGameInstance->Get_TransformFloat4x4(CPipeLine::D3DTS_PROJ)), E_FAIL);
	FAILED_CHECK_RETURN(m_pShaderCom->Set_RawValue(L"g_fColorDown", &m_vColorDown, sizeof(_float)), E_FAIL);

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}
CSky* CSky::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CSky*		pInstance = new CSky(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CSky");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CSky::Clone(const wstring& wstrPrototypeTag, void* pArg)
{
	CSky*		pInstance = new CSky(*this);

	if (FAILED(pInstance->Initialize_Clone(wstrPrototypeTag, pArg)))
	{
		MSG_BOX("Failed to Cloned : CSky");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CSky::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pRendererCom);
}
