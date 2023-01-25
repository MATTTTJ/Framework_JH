#include "stdafx.h"
#include "..\public\RocketArm.h"

#include "DangerRing.h"
#include "GameInstance.h"
#include "Monster.h"
#include "Player.h"



CRocketArm::CRocketArm(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CBullet(pDevice, pContext)
{
}

CRocketArm::CRocketArm(const CRocketArm& rhs)
	: CBullet(rhs)
{
}

HRESULT CRocketArm::Initialize_Prototype()
{
	m_bHasModel = true;

	FAILED_CHECK_RETURN(__super::Initialize_Prototype(), E_FAIL);

	return S_OK;
}

HRESULT CRocketArm::Initialize_Clone(const wstring& wstrPrototypeTag, void* pArg)
{
	if (nullptr != pArg)
	{
		m_tBulletOption = *(BULLETOPTION*)pArg;

		m_tBulletOption.BulletDesc.TransformDesc.fSpeedPerSec = 10.f;
		m_tBulletOption.m_eType = CBullet::BULLETOPTION::TYPE_FIRE;
		m_tBulletOption.BulletDesc.m_iDamage = 70;
		m_pOwner = m_tBulletOption.m_pOwner;

		if (m_tBulletOption.m_bTrueIsLeft)
		{
			m_iModelType = ARMTYPE_LEFT;
		}
		else
			m_iModelType = ARMTYPE_RIGHT;

		FAILED_CHECK_RETURN(__super::Initialize_Clone(wstrPrototypeTag, &m_tBulletOption), E_FAIL);
		FAILED_CHECK_RETURN(SetUp_Component(), E_FAIL);
		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_tBulletOption.BulletDesc.TransformDesc.vInitPos.x, m_tBulletOption.BulletDesc.TransformDesc.vInitPos.y, m_tBulletOption.BulletDesc.TransformDesc.vInitPos.z, 1.f));
		m_vTargetPos = m_tBulletOption.BulletDesc.m_vBulletLook;
		// m_vTargetPos.x = m_vTargetPos.x + CGameUtils::GetRandomFloat(-3.f, 3.f);
		// m_vTargetPos.z = m_vTargetPos.z + CGameUtils::GetRandomFloat(-3.f, 3.f);
		m_pTransformCom->LookAt(m_pTransformCom->Get_State(CTransform::STATE_TRANSLATION) + XMVector3Normalize(XMLoadFloat4(&m_vTargetPos) - m_pTransformCom->Get_State(CTransform::STATE_TRANSLATION)));
		m_pTransformCom->Set_Scaled(_float3(0.007f, 0.007f, 0.007f));

	}
	else
	{
		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(0.f, 0.f, 0.f, 1.f));
		m_pTransformCom->Set_Scaled(_float3(1000.f, 1000.f, 1.f));
	}

	Ready_Danger();

	return S_OK;
}

void CRocketArm::Tick(_double dTimeDelta)
{
	__super::Tick(dTimeDelta);

	_float4 CurPos = m_pTransformCom->Get_State(CTransform::STATE_TRANSLATION);
	
	if(CurPos.y < m_vTargetPos.y)
	{
		Set_Dead(true);
	}

	if (CGameInstance::GetInstance()->Key_Down(DIK_F9))
	{
		Set_Dead(true);
	}

	m_pTransformCom->Chase(m_vTargetPos, dTimeDelta);

	m_pBulletColliderCom->Update(m_pTransformCom->Get_WorldMatrix());
}

void CRocketArm::Late_Tick(_double dTimeDelta)
{
	// __super::Late_Tick(dTimeDelta);

	// Collision_To_Player(m_pBulletColliderCom);

	if (nullptr != m_pRendererCom)
	{
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHABLEND, this);
		m_pRendererCom->Add_DebugRenderGroup(m_pBulletColliderCom);
	}
}

HRESULT CRocketArm::Render()
{
	// return S_OK;

	// 모델이 왜 병신같이 나가는지 확인

	FAILED_CHECK_RETURN(__super::Render(), E_FAIL);

	_uint iNumMeshes = m_pArmModel[m_iModelType]->Get_NumMeshes();

	for (_uint i = 0; i < iNumMeshes; ++i)
	{
		m_pArmModel[m_iModelType]->Bind_Material(m_pShaderCom, i, aiTextureType_DIFFUSE, L"g_DiffuseTexture");
		FAILED_CHECK_RETURN(SetUp_ShaderResources(), E_FAIL);
		m_pArmModel[m_iModelType]->Render(m_pShaderCom, i);
	}
	return S_OK;
}

void CRocketArm::Ready_Danger()
{
	CDangerRing::RINGDESC		RingDesc;
	_float4 Pos = m_pTransformCom->Get_State(CTransform::STATE_TRANSLATION);
	RingDesc.GameObjectDesc.TransformDesc.vInitPos = _float3(m_vTargetPos.x, 10.514f, m_vTargetPos.z);
	RingDesc.GameObjectDesc.m_iHP = 1;
	RingDesc.m_fMinSize = 5.f;
	RingDesc.m_fMaxSize = 20.f;
	RingDesc.m_eType = CDangerRing::RINGDESC::RINGTYPE_ROCKETARM;
	(CGameInstance::GetInstance()->Clone_GameObjectReturnPtr(LEVEL_GAMEPLAY, L"Layer_DangerRing", L"Prototype_GameObject_Danger_Ring", &RingDesc));
	RingDesc.GameObjectDesc.TransformDesc.vInitPos = _float3(m_vTargetPos.x, 10.514f, m_vTargetPos.z);
	RingDesc.GameObjectDesc.m_iHP = 0;
	(CGameInstance::GetInstance()->Clone_GameObjectReturnPtr(LEVEL_GAMEPLAY, L"Layer_DangerRing", L"Prototype_GameObject_Danger_Ring", &RingDesc));
}

HRESULT CRocketArm::SetUp_Component()
{
	FAILED_CHECK_RETURN(__super::Add_Component(CGameInstance::Get_StaticLevelIndex(), L"Prototype_Component_Renderer", L"Com_Renderer", (CComponent**)&m_pRendererCom, this), E_FAIL);
	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Shader_VtxNonAnim", L"Com_Shader", (CComponent**)&m_pShaderCom, this), E_FAIL);
	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Model_LeftArm", L"Com_LeftArmModel", (CComponent**)&m_pArmModel[ARMTYPE_LEFT], this), E_FAIL);
	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Model_RightArm", L"Com_RightArmModel", (CComponent**)&m_pArmModel[ARMTYPE_RIGHT], this), E_FAIL);

	CCollider::COLLIDERDESC			ColliderDesc;
	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));
	ColliderDesc.vPosition = _float3(0.f, 0.f, 0.f);
	ColliderDesc.vSize = _float3(30.f, 30.f, 30.f);
	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Collider_SPHERE", L"Com_RocketArmSPHERE", (CComponent**)&m_pArmCollider, this, &ColliderDesc), E_FAIL);


	return S_OK;
}

HRESULT CRocketArm::SetUp_ShaderResources()
{
	NULL_CHECK_RETURN(m_pShaderCom, E_FAIL);

	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);
	FAILED_CHECK_RETURN(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, L"g_WorldMatrix"), E_FAIL);
	FAILED_CHECK_RETURN(m_pShaderCom->Set_Matrix(L"g_ViewMatrix", &pGameInstance->Get_TransformFloat4x4(CPipeLine::D3DTS_VIEW)), E_FAIL);
	FAILED_CHECK_RETURN(m_pShaderCom->Set_Matrix(L"g_ProjMatrix", &pGameInstance->Get_TransformFloat4x4(CPipeLine::D3DTS_PROJ)), E_FAIL);

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

CRocketArm* CRocketArm::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CRocketArm*		pInstance = new CRocketArm(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CRocketArm");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CRocketArm::Clone(const wstring& wstrPrototypeTag, void* pArg)
{
	CRocketArm*		pInstance = new CRocketArm(*this);

	if (FAILED(pInstance->Initialize_Clone(wstrPrototypeTag, pArg)))
	{
		MSG_BOX("Failed to Created : CRocketArm");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CRocketArm::Free()
{
	__super::Free();

	Safe_Release(m_pArmCollider);

	for (_uint i = 0; i < ARMTYPE_END; ++i)
	{
		Safe_Release(m_pArmModel[i]);
	}

	
}
