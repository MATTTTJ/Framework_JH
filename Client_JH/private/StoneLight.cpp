#include "stdafx.h"
#include "..\public\StoneLight.h"
#include "GameInstance.h"



CStoneLight::CStoneLight(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CGameObject(pDevice, pContext)
{
}

CStoneLight::CStoneLight(const CStoneLight& rhs)
	: CGameObject(rhs)
{
}

HRESULT CStoneLight::Initialize_Prototype()
{
	m_bHasModel = true;

	FAILED_CHECK_RETURN(__super::Initialize_Prototype(), E_FAIL);

	return S_OK;
}

HRESULT CStoneLight::Initialize_Clone(const wstring& wstrPrototypeTag, void* pArg)
{
	CGameObject::GAMEOBJECTDESC		GameObjectDesc;

	if (nullptr != pArg)
	{
		FAILED_CHECK_RETURN(__super::Initialize_Clone(wstrPrototypeTag, pArg), E_FAIL);
		ZeroMemory(&GameObjectDesc, sizeof(CGameObject::GAMEOBJECTDESC));

		GameObjectDesc = *(GAMEOBJECTDESC*)pArg;

		_float4 InitPos = XMVectorSet(GameObjectDesc.TransformDesc.vInitPos.x, GameObjectDesc.TransformDesc.vInitPos.y, GameObjectDesc.TransformDesc.vInitPos.z, 1.f);
		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, InitPos);
		m_pTransformCom->Set_Scaled(_float3(0.1f, 0.1f, 0.1f));
	}
	else
	{
		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(0.f, 0.f, 0.f, 1.f));
		m_pTransformCom->Set_Scaled(_float3(10.f, 10.f, 1.f));
	}

	m_pTransformCom->Rotation(XMVectorSet(0.1f, 1.0f, 0.1f, 0.f), XMConvertToRadians(50.f + CGameUtils::GetRandomFloat(-50.f, 50.f)));

	m_fLifeTime = 1.5f;
	FAILED_CHECK_RETURN(SetUp_Components(), E_FAIL);
	return S_OK;
}

void CStoneLight::Tick(_double TimeDelta)
{
	if (m_bIsDead)
		return;

	__super::Tick(TimeDelta);

	{
		m_fCurLifeTime += (_float)TimeDelta;
	}
	m_fAlpha += (_float)TimeDelta;
	
	if(m_fAlpha >= 1.f)
	{
		m_fAlpha = 1.f;
		// m_pTransformCom->Set_Scaled(_float3(2.f, 1.5f, 2.f));

	}
	_float3 Scale =  m_pTransformCom->Get_Scaled();
	_float Time = (_float)(TimeDelta);
	m_pTransformCom->Set_Scaled(_float3(Scale.x + Time, 1.5f, Scale.z+ Time));
	if(m_fCurLifeTime > m_fLifeTime)
	{
		Set_Dead(true);
	}
}

void CStoneLight::Late_Tick(_double TimeDelta)
{
	__super::Late_Tick(TimeDelta);

	if (m_pRendererCom != nullptr)
	{
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_EFFECT, this);
	}




}

HRESULT CStoneLight::Render()
{
	FAILED_CHECK_RETURN(__super::Render(), E_FAIL);

	_uint iNumMeshes = m_pModelCom->Get_NumMeshes();

	for (_uint i = 0; i < iNumMeshes; ++i)
	{
		m_pModelCom->Bind_Material(m_pShaderCom, i, aiTextureType_DIFFUSE, L"g_DiffuseTexture");


		FAILED_CHECK_RETURN(SetUp_ShaderResources(), E_FAIL);

		m_pModelCom->Render(m_pShaderCom, i, L"g_BoneMatrices",3);
	}

	return S_OK;
}


HRESULT CStoneLight::SetUp_Components()
{
	FAILED_CHECK_RETURN(__super::Add_Component(CGameInstance::Get_StaticLevelIndex(), L"Prototype_Component_Renderer", L"Com_Renderer", (CComponent**)&m_pRendererCom, this), E_FAIL);
	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Shader_VtxNonAnim", L"Com_Shader", (CComponent**)&m_pShaderCom, this), E_FAIL);
	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Model_StonePillar_Light", L"Com_Model", (CComponent**)&m_pModelCom, this), E_FAIL);
	return S_OK;
}

HRESULT CStoneLight::SetUp_ShaderResources()
{
	NULL_CHECK_RETURN(m_pShaderCom, E_FAIL);

	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);
	FAILED_CHECK_RETURN(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, L"g_WorldMatrix"), E_FAIL);
	FAILED_CHECK_RETURN(m_pShaderCom->Set_Matrix(L"g_ViewMatrix", &pGameInstance->Get_TransformFloat4x4(CPipeLine::D3DTS_VIEW)), E_FAIL);
	FAILED_CHECK_RETURN(m_pShaderCom->Set_Matrix(L"g_ProjMatrix", &pGameInstance->Get_TransformFloat4x4(CPipeLine::D3DTS_PROJ)), E_FAIL);
	FAILED_CHECK_RETURN(m_pShaderCom->Set_RawValue(L"g_fAlpha", &m_fAlpha, sizeof(_float)),E_FAIL);

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

CStoneLight* CStoneLight::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CStoneLight*		pInstance = new CStoneLight(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CStoneLight");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CStoneLight::Clone(const wstring& wstrPrototypeTag, void* pArg)
{
	CStoneLight*		pInstance = new CStoneLight(*this);

	if (FAILED(pInstance->Initialize_Clone(wstrPrototypeTag, pArg)))
	{
		MSG_BOX("Failed to Created : CStoneLight");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CStoneLight::Free()
{
	__super::Free();
	Safe_Release(m_pModelCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pRendererCom);
}
