#include "stdafx.h"
#include "..\public\Sphere_Effect.h"
#include "GameInstance.h"


CSphere_Effect::CSphere_Effect(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject(pDevice, pContext)
{
}

CSphere_Effect::CSphere_Effect(const CSphere_Effect& rhs)
	: CGameObject(rhs)
{
}

HRESULT CSphere_Effect::Initialize_Prototype()
{
	FAILED_CHECK_RETURN(__super::Initialize_Prototype(), E_FAIL);

	return S_OK;
}

HRESULT CSphere_Effect::Initialize_Clone(const wstring& wstrPrototypeTag, void* pArg)
{

	CGameObject::GAMEOBJECTDESC		GameObjectDesc;
	ZeroMemory(&GameObjectDesc, sizeof(CGameObject::GAMEOBJECTDESC));

	if (nullptr != pArg)
	{
		memcpy(&GameObjectDesc, pArg, sizeof(CGameObject::GAMEOBJECTDESC));
	}
	else
	{
		GameObjectDesc.TransformDesc.fSpeedPerSec = 15.0f;
		GameObjectDesc.TransformDesc.fRotationPerSec = XMConvertToRadians(90.f);
	}

	FAILED_CHECK_RETURN(__super::Initialize_Clone(wstrPrototypeTag, &GameObjectDesc), E_FAIL);

	FAILED_CHECK_RETURN(SetUp_Component(), E_FAIL);

	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, (XMVectorSet(GameObjectDesc.TransformDesc.vInitPos.x, GameObjectDesc.TransformDesc.vInitPos.y, GameObjectDesc.TransformDesc.vInitPos.z, 1.f)));

	m_vInitScale = _float3(GameObjectDesc.m_vBulletLook);

	m_fLifeTime = 0.5f;

	return S_OK;
}

void CSphere_Effect::Tick(_double dTimeDelta)
{
	__super::Tick(dTimeDelta);
	// m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMLoadFloat4(&CGameInstance::GetInstance()->Get_CamPos()));
	m_fTime += (_float)dTimeDelta;

	if (m_fTime > 1.f)
		Set_Dead(true);

	m_fCurLifeTime += (_float)dTimeDelta;

	m_pTransformCom->Turn(m_pTransformCom->Get_State(CTransform::STATE_UP), dTimeDelta);

	if (m_fCurLifeTime >= m_fLifeTime)
		Set_Dead(true);

	_float3 scale = m_pTransformCom->Get_Scaled();

	scale += _float3((_float)dTimeDelta * 4.f, (_float)dTimeDelta* 4.f, (_float)dTimeDelta* 4.f);

	m_pTransformCom->Set_Scaled(scale);

}

void CSphere_Effect::Late_Tick(_double dTimeDelta)
{
	__super::Late_Tick(dTimeDelta);

	// m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMLoadFloat4(&CGameInstance::GetInstance()->Get_CamPos()));

	if (nullptr != m_pRendererCom)
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_EFFECT, this);
}

HRESULT CSphere_Effect::Render()
{
	FAILED_CHECK_RETURN(__super::Render(), E_FAIL);

	FAILED_CHECK_RETURN(SetUp_ShaderResource(), E_FAIL);

	_uint iNumMeshes = m_pModelCom->Get_NumMeshes();

	for (_uint i = 0; i < iNumMeshes; ++i)
	{
		m_pModelCom->Bind_Material(m_pShaderCom, i, aiTextureType_DIFFUSE, L"g_DiffuseTexture");

		m_pModelCom->Render(m_pShaderCom, i, L"g_BoneMatrices", 6);
	}

	return S_OK;
}

HRESULT CSphere_Effect::SetUp_Component()
{
	FAILED_CHECK_RETURN(__super::Add_Component(CGameInstance::Get_StaticLevelIndex(), L"Prototype_Component_Renderer", L"Com_Renderer", (CComponent**)&m_pRendererCom, this), E_FAIL);
	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Texture_Sphere_Tex", L"Com_Texture", (CComponent**)&m_pTextureCom, this), E_FAIL);
	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Shader_VtxNonAnim", L"Com_Shader", (CComponent**)&m_pShaderCom, this), E_FAIL);
	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Model_Sphere_Effect", L"Com_Model", (CComponent**)&m_pModelCom, this), E_FAIL);

	return S_OK;
}

HRESULT CSphere_Effect::SetUp_ShaderResource()
{
	NULL_CHECK_RETURN(m_pShaderCom, E_FAIL);

	CGameInstance*		pGameInstance = CGameInstance::GetInstance();
	// Safe_AddRef(pGameInstance);

	FAILED_CHECK_RETURN(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, L"g_WorldMatrix"), E_FAIL);

	FAILED_CHECK_RETURN(m_pShaderCom->Set_Matrix(L"g_ViewMatrix", &pGameInstance->Get_TransformFloat4x4(CPipeLine::D3DTS_VIEW)), E_FAIL);

	FAILED_CHECK_RETURN(m_pShaderCom->Set_Matrix(L"g_ProjMatrix", &pGameInstance->Get_TransformFloat4x4(CPipeLine::D3DTS_PROJ)), E_FAIL);
	FAILED_CHECK_RETURN(m_pShaderCom->Set_RawValue(L"g_fTime", &m_fTime, sizeof(_float)), E_FAIL);
	FAILED_CHECK_RETURN(m_pShaderCom->Set_RawValue(L"g_vMtrlDif", &XMVectorSet(0.f, 0.f,0.f,sin(m_fTime)), sizeof(_vector)), E_FAIL);

	// FAILED_CHECK_RETURN(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, L"g_DiffuseTexture", 0), E_FAIL);
	FAILED_CHECK_RETURN(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, L"g_NoiseTexture", 1), E_FAIL);


	// const LIGHTDESC*	pLightDesc = pGameInstance->Get_LightDesc(0);
	// NULL_CHECK_RETURN(pLightDesc, E_FAIL);
	//
	// m_pShaderCom->Set_RawValue(L"g_vLightDir", &pLightDesc->vDirection, sizeof(_float4));
	// m_pShaderCom->Set_RawValue(L"g_vLightDiffuse", &pLightDesc->vDiffuse, sizeof(_float4));
	// m_pShaderCom->Set_RawValue(L"g_vLightAmbient", &pLightDesc->vAmbient, sizeof(_float4));
	// m_pShaderCom->Set_RawValue(L"g_vLightSpecular", &pLightDesc->vSpecular, sizeof(_float4));
	//
	// Safe_Release(pGameInstance);

	return S_OK;
}

CSphere_Effect* CSphere_Effect::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CSphere_Effect *	pInstance = new CSphere_Effect(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Create : CSphere_Effect");
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

CGameObject* CSphere_Effect::Clone(const wstring& wstrPrototypeTag, void* pArg)
{
	CSphere_Effect *	pInstance = new CSphere_Effect(*this);

	if (FAILED(pInstance->Initialize_Clone(wstrPrototypeTag, pArg)))
	{
		MSG_BOX("Failed to Clone : CSphere_Effect");
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

void CSphere_Effect::Free()
{
	__super::Free();
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pModelCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pRendererCom);

}
