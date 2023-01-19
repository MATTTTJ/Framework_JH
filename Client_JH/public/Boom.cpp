#include "stdafx.h"
#include "..\private\Boom.h"
#include "GameInstance.h"
#include "Monster.h"
#include "Player.h"


CBoom::CBoom(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CBullet(pDevice, pContext)
{
}

CBoom::CBoom(const CBoom& rhs)
	:CBullet(rhs)
{
}



HRESULT CBoom::Initialize_Prototype()
{
	FAILED_CHECK_RETURN(__super::Initialize_Prototype(), E_FAIL);

	return S_OK;
}

HRESULT CBoom::Initialize_Clone(const wstring& wstrPrototypeTag, void* pArg)
{
	if (nullptr != pArg)
	{
		m_tBulletOption = *(BULLETOPTION*)pArg;
		
		m_tBulletOption.BulletDesc.TransformDesc.fSpeedPerSec = 10.f;
		m_tBulletOption.m_eType = CBullet::BULLETOPTION::TYPE_FIRE;
		m_tBulletOption.BulletDesc.m_iDamage = 30;
		m_pOwner = m_tBulletOption.m_pOwner;
		m_tBulletOption.BulletDesc.TransformDesc.fRotationPerSec = XMConvertToRadians(180.f);
		FAILED_CHECK_RETURN(__super::Initialize_Clone(wstrPrototypeTag, &m_tBulletOption), E_FAIL);
		FAILED_CHECK_RETURN(SetUp_Component(), E_FAIL);
		// _matrix pivot = XMMatrixIdentity();
		// pivot = XMMatrixRotationAxis(m_pTransformCom->Get_State(CTransform::STATE_UP), XMConvertToDegrees(180.f));
		
		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_tBulletOption.BulletDesc.TransformDesc.vInitPos.x, m_tBulletOption.BulletDesc.TransformDesc.vInitPos.y, m_tBulletOption.BulletDesc.TransformDesc.vInitPos.z, 1.f));
		m_pTransformCom->LookAt(m_pTransformCom->Get_State(CTransform::STATE_TRANSLATION) + XMVector4Normalize((XMLoadFloat4(&m_tBulletOption.BulletDesc.m_vBulletLook))));
		m_pTransformCom->Set_Scaled(XMVectorSet(0.1f, 0.1f, 0.1f, 1.f));
		_float4 OwnerPos = dynamic_cast<CMonster*>(m_pOwner)->Get_TransformState(CTransform::STATE_TRANSLATION);
		m_fDeadHeight = m_pTransformCom->Get_State(CTransform::STATE_TRANSLATION);
		// 위에서 떨어지는 로직으로 만들기 
		m_fJumpPower = 1.15f;
	}
	else
	{
		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(0.f, 0.f, 0.f, 1.f));
		m_pTransformCom->Set_Scaled(_float3(1000.f, 1000.f, 1.f));
	}

	return S_OK;
}

void CBoom::Tick(_double dTimeDelta)
{
	__super::Tick(dTimeDelta);
	{
		if(m_bFallTime == true)
		{
			m_fCurBoomTime += (_float)dTimeDelta;
		}
	}
	_float4 BoomPos = m_pTransformCom->Get_State(CTransform::STATE_TRANSLATION);

	BoomPos.y += m_fJumpPower * (_float)m_tBulletOption.BulletDesc.TransformDesc.fSpeedPerSec * (_float)dTimeDelta;

	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, BoomPos);
	m_pTransformCom->Go_Straight(dTimeDelta, CTransform::TRANS_BULLET);
	m_fJumpPower -= 0.045f;

	if (m_bFallTime == false)
	{
		m_pTransformCom->Turn(m_pTransformCom->Get_State(CTransform::STATE_UP), dTimeDelta);
		m_pTransformCom->Turn(m_pTransformCom->Get_State(CTransform::STATE_LOOK), dTimeDelta);
	}

	if(BoomPos.y <= m_fDeadHeight.y)
	{
		m_bFallTime = true;
		_float4 tmp = dynamic_cast<CMonster*>(m_pOwner)->Get_TransformState(CTransform::STATE_TRANSLATION);
		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(BoomPos.x, tmp.y + 0.35f, BoomPos.z, 1.f));
	}
	m_pBoomColliderCom->Update(m_pTransformCom->Get_WorldMatrix());
	m_pBulletColliderCom->Update(m_pTransformCom->Get_WorldMatrix());


}

void CBoom::Late_Tick(_double dTimeDelta)
{
	__super::Late_Tick(dTimeDelta);

	if (m_fCurBoomTime >= m_fBoomTime)
	{
		// TODO :: 폭발 이펙트 추가
		Collision_To_Player(m_pBoomColliderCom);
		Set_Dead(true);
	}

	if (nullptr != m_pRendererCom &&
		true == CGameInstance::GetInstance()->isInFrustum_WorldSpace(m_pTransformCom->Get_State(CTransform::STATE_TRANSLATION), 2.f))
	{
		m_pRendererCom->Add_DebugRenderGroup(m_pBoomColliderCom);
	}

}

HRESULT CBoom::Render()
{
	FAILED_CHECK_RETURN(__super::Render(), E_FAIL);
	FAILED_CHECK_RETURN(SetUp_ShaderResources(), E_FAIL);

	_uint iNumMeshes = m_pModelCom->Get_NumMeshes();

	for (_uint i = 0; i < iNumMeshes; ++i)
	{
		m_pModelCom->Bind_Material(m_pShaderCom, i, aiTextureType_DIFFUSE, L"g_DiffuseTexture");
		m_pModelCom->Bind_Material(m_pShaderCom, i, aiTextureType_NORMALS, L"g_NormalTexture");

		m_pModelCom->Render(m_pShaderCom, i);
	}
	return S_OK;
}

HRESULT CBoom::SetUp_Component()
{
	FAILED_CHECK_RETURN(__super::Add_Component(CGameInstance::Get_StaticLevelIndex(), L"Prototype_Component_Renderer", L"Com_Renderer", (CComponent**)&m_pRendererCom, this), E_FAIL);
	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Shader_VtxNonAnim", L"Com_Shader", (CComponent**)&m_pShaderCom, this), E_FAIL);
	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Model_Boom", L"Com_Model", (CComponent**)&m_pModelCom, this), E_FAIL);

	CCollider::COLLIDERDESC			ColliderDesc;
	// 원 반지름 2.75  장판 이펙트 만들기 (터질듯한) 
	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));
	ColliderDesc.vPosition = _float3(0.f, 0.f, 0.f);
	ColliderDesc.vSize = _float3(55.f, 0.f, 0.f);
	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Collider_SPHERE", L"Com_BoomSPHERE", (CComponent**)&m_pBoomColliderCom, this, &ColliderDesc), E_FAIL);
	return S_OK;
}

HRESULT CBoom::SetUp_ShaderResources()
{
	NULL_CHECK_RETURN(m_pShaderCom, E_FAIL);

	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);
	FAILED_CHECK_RETURN(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, L"g_WorldMatrix"), E_FAIL);
	FAILED_CHECK_RETURN(m_pShaderCom->Set_Matrix(L"g_ViewMatrix", &pGameInstance->Get_TransformFloat4x4(CPipeLine::D3DTS_VIEW)), E_FAIL);
	FAILED_CHECK_RETURN(m_pShaderCom->Set_Matrix(L"g_ProjMatrix", &pGameInstance->Get_TransformFloat4x4(CPipeLine::D3DTS_PROJ)), E_FAIL);

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

CBoom* CBoom::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CBoom*		pInstance = new CBoom(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CBoom");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CBoom::Clone(const wstring& wstrPrototypeTag, void* pArg)
{
	CBoom*		pInstance = new CBoom(*this);

	if (FAILED(pInstance->Initialize_Clone(wstrPrototypeTag, pArg)))
	{
		MSG_BOX("Failed to Created : CBoom");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CBoom::Free()
{
	__super::Free();
}
