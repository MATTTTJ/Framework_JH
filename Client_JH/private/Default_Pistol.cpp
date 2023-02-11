#include "stdafx.h"
#include "..\public\Default_Pistol.h"

#include "Default_Bullet_Dead.h"
#include "GameInstance.h"
#include "Monster.h"
#include "Player.h"
#include "Static_Camera.h"

CDefault_Pistol::CDefault_Pistol(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CBullet(pDevice, pContext)
{
}

CDefault_Pistol::CDefault_Pistol(const CDefault_Pistol& rhs)
	: CBullet(rhs)
{
}

HRESULT CDefault_Pistol::Initialize_Prototype()
{
	FAILED_CHECK_RETURN(__super::Initialize_Prototype(), E_FAIL);

	return S_OK;
}

HRESULT CDefault_Pistol::Initialize_Clone(const wstring& wstrPrototypeTag, void* pArg)
{

	if (nullptr != pArg)
	{
		m_tBulletOption = *(BULLETOPTION*)pArg;

		m_tBulletOption.BulletDesc.TransformDesc.fSpeedPerSec = 70.f;
		m_tBulletOption.m_eType = CBullet::BULLETOPTION::TYPE_FIRE;
		m_pOwner = m_tBulletOption.m_pOwner;
		FAILED_CHECK_RETURN(__super::Initialize_Clone(wstrPrototypeTag, &m_tBulletOption), E_FAIL);
		FAILED_CHECK_RETURN(SetUp_Component(), E_FAIL);
		m_iTextureIndex = m_tBulletOption.BulletDesc.m_iCountType;
		m_vPSize = _float2{ 2.5f, 0.5f };
		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_tBulletOption.BulletDesc.TransformDesc.vInitPos.x, m_tBulletOption.BulletDesc.TransformDesc.vInitPos.y, m_tBulletOption.BulletDesc.TransformDesc.vInitPos.z, 1.f));

		_matrix matpivot;
		matpivot = XMMatrixIdentity();
		matpivot = XMMatrixRotationY(XMConvertToRadians(180.f));

		if (m_iTextureIndex == 0)
		{
			m_vPSize = _float2{ 2.5f, 0.5f };
			m_tBulletOption.BulletDesc.m_iDamage = 30;

			_float4 vDir = XMVector3Normalize((dynamic_cast<CPlayer*>(m_pOwner)->Get_CurWeaponModelCom()->Get_BoneMatrix("Att") * matpivot * XMLoadFloat4x4(&dynamic_cast<CPlayer*>(m_pOwner)->Get_WorldFloat4x4())).r[3] -
				(dynamic_cast<CPlayer*>(m_pOwner)->Get_CurWeaponModelCom()->Get_BoneMatrix("1202_Bone004") * matpivot * XMLoadFloat4x4(&dynamic_cast<CPlayer*>(m_pOwner)->Get_WorldFloat4x4())).r[3]);
			m_vDir = vDir;
			_matrix matpivot;
			matpivot = XMMatrixRotationY(XMConvertToRadians(180.f));

			_matrix matTransform = dynamic_cast<CPlayer*>(m_pOwner)->Get_CurWeaponModelCom()->Get_BoneMatrix("Att") * matpivot * XMLoadFloat4x4(&dynamic_cast<CPlayer*>(m_pOwner)->Get_WorldFloat4x4());
			_vector S, R, P;
			XMMatrixDecompose(&S, &R, &P, matTransform);
			_float4 vPosition;
			XMStoreFloat4(&vPosition, (dynamic_cast<CPlayer*>(m_pOwner)->Get_CurWeaponModelCom()->Get_BoneMatrix("Att") * matpivot * XMLoadFloat4x4(&dynamic_cast<CPlayer*>(m_pOwner)->Get_WorldFloat4x4())).r[3]);
			XMStoreFloat4(&m_vCamPos, P);

			matTransform = dynamic_cast<CPlayer*>(m_pOwner)->Get_CurWeaponModelCom()->Get_BoneMatrix("1202_Bone009") * matpivot * XMLoadFloat4x4(&dynamic_cast<CPlayer*>(m_pOwner)->Get_WorldFloat4x4());

			XMMatrixDecompose(&S, &R, &P, matTransform);

			XMStoreFloat4(&m_vInitPos, P);

			_float4 vUp = m_pTransformCom->Get_State(CTransform::STATE_UP);

			m_vLook = XMVector3Normalize(XMVector3Cross(m_vDir, vUp));


		}
		else if(m_iTextureIndex ==1)
		{
			m_vPSize = _float2{ 10.f, 0.5f };
			m_tBulletOption.BulletDesc.m_iDamage = 70;

			_float4 vDir = XMVector3Normalize((dynamic_cast<CPlayer*>(m_pOwner)->Get_CurWeaponModelCom()->Get_BoneMatrix("Att001") * matpivot * XMLoadFloat4x4(&dynamic_cast<CPlayer*>(m_pOwner)->Get_WorldFloat4x4())).r[3] -
				(dynamic_cast<CPlayer*>(m_pOwner)->Get_CurWeaponModelCom()->Get_BoneMatrix("1205_Bone001") * matpivot * XMLoadFloat4x4(&dynamic_cast<CPlayer*>(m_pOwner)->Get_WorldFloat4x4())).r[3]);
			m_vDir = vDir;
			_matrix matpivot;
			matpivot = XMMatrixRotationY(XMConvertToRadians(180.f));

			_matrix matTransform = dynamic_cast<CPlayer*>(m_pOwner)->Get_CurWeaponModelCom()->Get_BoneMatrix("Att001") * matpivot * XMLoadFloat4x4(&dynamic_cast<CPlayer*>(m_pOwner)->Get_WorldFloat4x4());
			_vector S, R, P;
			XMMatrixDecompose(&S, &R, &P, matTransform);
			_float4 vPosition;
			XMStoreFloat4(&vPosition, (dynamic_cast<CPlayer*>(m_pOwner)->Get_CurWeaponModelCom()->Get_BoneMatrix("Att001") * matpivot * XMLoadFloat4x4(&dynamic_cast<CPlayer*>(m_pOwner)->Get_WorldFloat4x4())).r[3]);
			XMStoreFloat4(&m_vCamPos, P);

			matTransform = dynamic_cast<CPlayer*>(m_pOwner)->Get_CurWeaponModelCom()->Get_BoneMatrix("1205_Bone001") * matpivot * XMLoadFloat4x4(&dynamic_cast<CPlayer*>(m_pOwner)->Get_WorldFloat4x4());

			XMMatrixDecompose(&S, &R, &P, matTransform);

			XMStoreFloat4(&m_vInitPos, P);

			_float4 vUp = m_pTransformCom->Get_State(CTransform::STATE_UP);

			m_vLook = XMVector3Normalize(XMVector3Cross(m_vDir, vUp));

		}


		// _float4   fCamLook = *dynamic_cast<CStatic_Camera*>(CGameInstance::GetInstance()->Get_CloneObjectList(LEVEL_GAMEPLAY, L"Layer_ZCamera")->back())->Get_CamLook();
		// _float4 fPlayerLook = dynamic_cast<CPlayer*>(m_pOwner)->Get_TransformState(CTransform::STATE_LOOK);

		m_pTransformCom->LookAt(m_pTransformCom->Get_State(CTransform::STATE_TRANSLATION) + XMLoadFloat4(&m_tBulletOption.BulletDesc.m_vBulletLook));
	}
	else
	{
		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(0.f, 0.f, 0.f, 1.f));
		m_pTransformCom->Set_Scaled(_float3(1000.f, 1000.f, 1.f));
	}

	

	m_fTrailCount = 1.f;
	m_fStrong = 0.0f;
	return S_OK;
}

void CDefault_Pistol::Tick(_double dTimeDelta)
{
	__super::Tick(dTimeDelta);

	// if (m_fTrailCount >= 1.f)
	// m_fStrong += dTimeDelta * 3.f ;
	// m_fTrailCount += dTimeDelta * 30;
	// else
		// m_fTrailCount = 0.f;
	// 0.016

	m_vUp = dynamic_cast<CPlayer*>(m_pOwner)->Get_TransformState(CTransform::STATE_UP);


	m_pTransformCom->Go_Straight(dTimeDelta, CTransform::TRANS_BULLET);

}

void CDefault_Pistol::Late_Tick(_double dTimeDelta)
{
	__super::Late_Tick(dTimeDelta);
	__super::Compute_CamDistance();

	m_vTest = dynamic_cast<CPlayer*>(m_pOwner)->Get_TransformState(CTransform::STATE_UP);

	// if (Collision_Body())
	// 	int i = 0;

	// 기존 코드 정리하고 총알 자체에서 몬스터들 콜라이더와 비교하기
	// 부딪힌 객체 받아서 그 객체에 어떻게 전달할건지 생각해보기 


	// if (nullptr != m_pBulletColliderCom)
	// 	m_pBulletColliderCom->Update(m_pTransformCom->Get_WorldMatrix());
	if (m_pRendererCom != nullptr)
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_EFFECT, this);


	m_pBulletColliderCom->Update(m_pTransformCom->Get_WorldMatrix());
}

HRESULT CDefault_Pistol::Render()
{
	FAILED_CHECK_RETURN(__super::Render(), E_FAIL);

	FAILED_CHECK_RETURN(SetUp_ShaderResources(), E_FAIL);

	m_pShaderCom->Begin(1);

	m_pPointBuffer->Render();

	return S_OK;
}

HRESULT CDefault_Pistol::SetUp_Component()
{
	FAILED_CHECK_RETURN(__super::Add_Component(CGameInstance::Get_StaticLevelIndex(), L"Prototype_Component_Renderer", L"Com_Renderer", (CComponent**)&m_pRendererCom, this), E_FAIL);
	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Shader_VtxPointInstance", L"Com_Shader", (CComponent**)&m_pShaderCom, this), E_FAIL);
	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_VIBuffer_Bullet_Instancing", L"Com_VIBuffer", (CComponent**)&m_pPointBuffer, this), E_FAIL);
	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Texture_Bullet", L"Com_Texture", (CComponent**)&m_pTextureCom, this), E_FAIL);
	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Texture_BulletAdd", L"Com_AddTexture", (CComponent**)&m_pTestTexture, this), E_FAIL);

	return S_OK;
}

HRESULT CDefault_Pistol::SetUp_ShaderResources()
{
	NULL_CHECK_RETURN(m_pShaderCom, E_FAIL);

	FAILED_CHECK_RETURN(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, L"g_WorldMatrix"), E_FAIL);
	FAILED_CHECK_RETURN(m_pShaderCom->Set_Matrix(L"g_ViewMatrix", &CGameInstance::GetInstance()->Get_TransformFloat4x4(CPipeLine::D3DTS_VIEW)), E_FAIL);
	FAILED_CHECK_RETURN(m_pShaderCom->Set_Matrix(L"g_ProjMatrix", &CGameInstance::GetInstance()->Get_TransformFloat4x4(CPipeLine::D3DTS_PROJ)), E_FAIL);
	FAILED_CHECK_RETURN(m_pShaderCom->Set_RawValue(L"g_fTrailCount", &m_fTrailCount, sizeof(_float)), E_FAIL);
	FAILED_CHECK_RETURN(m_pShaderCom->Set_RawValue(L"g_vCamPosition", &m_vCamPos, sizeof(_float4)), E_FAIL);
	FAILED_CHECK_RETURN(m_pShaderCom->Set_RawValue(L"g_vLook", &m_vLook, sizeof(_float4)), E_FAIL);
	FAILED_CHECK_RETURN(m_pShaderCom->Set_RawValue(L"g_vRightSrc", &m_vTest, sizeof(_float4)), E_FAIL);
	FAILED_CHECK_RETURN(m_pShaderCom->Set_RawValue(L"g_vUp", &m_vUp, sizeof(_float4)), E_FAIL);
	FAILED_CHECK_RETURN(m_pShaderCom->Set_RawValue(L"g_vPSize", &m_vPSize, sizeof(_float2)), E_FAIL);
	FAILED_CHECK_RETURN(m_pTestTexture->Bind_ShaderResource(m_pShaderCom, L"g_AddTexture"), E_FAIL);
	FAILED_CHECK_RETURN(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, L"g_Texture", m_iTextureIndex), E_FAIL);

	return S_OK;
}

// _bool CDefault_Pistol::Collision_Body()
// {
// 	// CGameInstance* pGameInstance = CGameInstance::GetInstance();
// 	//
// 	// CCollider* pCollider = (CCollider*)pGameInstance->Get_ComponentPtr(LEVEL_GAMEPLAY, L"Layer_Monster", L"Com_HitBodySphere");
// 	// if(m_pBulletColliderCom->Collision(pCollider))
// 	// {
// 	// 	CMonster* pMonster = (CMonster*)pCollider->Get_Owner();
// 	// 	if (pMonster == nullptr)
// 	// 		return false;
// 	//
// 	// 	pMonster->Collision_Event(this); // 총알이 어디 충돌했는지 판단하니까 
// 	// 	return true;					// 총알과 몬스터 둘다에 바디와 헤드 만들어서 충돌 이벤트 던지기
// 	// }
// 	// else 
// 	// 	return false;
// }


CDefault_Pistol* CDefault_Pistol::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CDefault_Pistol*		pInstance = new CDefault_Pistol(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CDefault_Pistol");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CDefault_Pistol::Clone(const wstring& wstrPrototypeTag, void* pArg)
{
	CDefault_Pistol*		pInstance = new CDefault_Pistol(*this);

	if (FAILED(pInstance->Initialize_Clone(wstrPrototypeTag, pArg)))
	{
		MSG_BOX("Failed to Cloned : CDefault_Pistol");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CDefault_Pistol::Free()
{
	

	__super::Free();

	Safe_Release(m_pTestTexture);
}
