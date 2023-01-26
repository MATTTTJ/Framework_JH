#include "stdafx.h"
#include "../public/Arrow.h"
#include "GameInstance.h"
#include "Monster.h"
#include "Player.h"

CArrow::CArrow(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CBullet(pDevice, pContext)
{
}

CArrow::CArrow(const CArrow& rhs)
	:CBullet(rhs)
{
}

HRESULT CArrow::Initialize_Prototype()
{
	m_bHasModel = true;

	FAILED_CHECK_RETURN(__super::Initialize_Prototype(), E_FAIL);

	return S_OK;
}

HRESULT CArrow::Initialize_Clone(const wstring& wstrPrototypeTag, void* pArg)
{
	if (nullptr != pArg)
	{
		m_tBulletOption = *(BULLETOPTION*)pArg;

		m_tBulletOption.BulletDesc.TransformDesc.fSpeedPerSec = 10.f;
		m_tBulletOption.m_eType = CBullet::BULLETOPTION::TYPE_FIRE;
		m_tBulletOption.BulletDesc.m_iDamage = 30;
		m_pOwner = m_tBulletOption.m_pOwner;
		FAILED_CHECK_RETURN(__super::Initialize_Clone(wstrPrototypeTag, &m_tBulletOption), E_FAIL);
		FAILED_CHECK_RETURN(SetUp_Component(), E_FAIL);

		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_tBulletOption.BulletDesc.TransformDesc.vInitPos.x, m_tBulletOption.BulletDesc.TransformDesc.vInitPos.y, m_tBulletOption.BulletDesc.TransformDesc.vInitPos.z, 1.f));
		m_pTransformCom->LookAt(m_pTransformCom->Get_State(CTransform::STATE_TRANSLATION) + XMVector4Normalize((XMLoadFloat4(&m_tBulletOption.BulletDesc.m_vBulletLook))));
		m_pTransformCom->Set_Scaled(XMVectorSet(0.01f,0.01f,0.01f,1.f));
		
	}
	else
	{
		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(0.f, 0.f, 0.f, 1.f));
		m_pTransformCom->Set_Scaled(_float3(1000.f, 1000.f, 1.f));
	}

	return S_OK;
}

void CArrow::Tick(_double dTimeDelta)
{
	if(m_pOwner == nullptr)
	{
		Set_Dead(true);
	}
	__super::Tick(dTimeDelta);

	m_pTransformCom->Go_Straight(dTimeDelta, CTransform::TRANS_BULLET);

	m_pBulletColliderCom->Update(m_pTransformCom->Get_WorldMatrix());
}

void CArrow::Late_Tick(_double dTimeDelta)
{
	__super::Late_Tick(dTimeDelta);

	Collision_To_Player(m_pBulletColliderCom);

	if (nullptr != m_pRendererCom)
	{
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHABLEND, this);
	}
}

HRESULT CArrow::Render()
{
	FAILED_CHECK_RETURN(__super::Render(), E_FAIL);
	FAILED_CHECK_RETURN(SetUp_ShaderResources(), E_FAIL);

	_uint iNumMeshes = m_pModelCom->Get_NumMeshes();

	for (_uint i = 0; i < iNumMeshes; ++i)
	{
		m_pModelCom->Bind_Material(m_pShaderCom, i, aiTextureType_DIFFUSE, L"g_DiffuseTexture");
		m_pModelCom->Render(m_pShaderCom, i);
	}
	return S_OK;
}

HRESULT CArrow::SetUp_Component()
{
	FAILED_CHECK_RETURN(__super::Add_Component(CGameInstance::Get_StaticLevelIndex(), L"Prototype_Component_Renderer", L"Com_Renderer", (CComponent**)&m_pRendererCom, this), E_FAIL);
	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Shader_VtxNonAnim", L"Com_Shader", (CComponent**)&m_pShaderCom, this), E_FAIL);
	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Model_Arrow", L"Com_Model", (CComponent**)&m_pModelCom, this), E_FAIL);

	return S_OK;
}

HRESULT CArrow::SetUp_ShaderResources()
{
	NULL_CHECK_RETURN(m_pShaderCom, E_FAIL);

	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);
	FAILED_CHECK_RETURN(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, L"g_WorldMatrix"), E_FAIL);
	FAILED_CHECK_RETURN(m_pShaderCom->Set_Matrix(L"g_ViewMatrix", &pGameInstance->Get_TransformFloat4x4(CPipeLine::D3DTS_VIEW)), E_FAIL);
	FAILED_CHECK_RETURN(m_pShaderCom->Set_Matrix(L"g_ProjMatrix", &pGameInstance->Get_TransformFloat4x4(CPipeLine::D3DTS_PROJ)), E_FAIL);

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

CArrow* CArrow::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CArrow*		pInstance = new CArrow(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CArrow");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CArrow::Clone(const wstring& wstrPrototypeTag, void* pArg)
{
	CArrow*		pInstance = new CArrow(*this);

	if (FAILED(pInstance->Initialize_Clone(wstrPrototypeTag, pArg)))
	{
		MSG_BOX("Failed to Created : CArrow");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CArrow::Free()
{
	__super::Free();
}
