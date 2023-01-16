#include "stdafx.h"
#include "..\public\Bullet.h"
#include "GameInstance.h"
#include "Monster.h"
#include "Player.h"
#include "Collider.h"

CBullet::CBullet(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CGameObject(pDevice,pContext)
{
}

CBullet::CBullet(const CBullet& rhs)
	:CGameObject(rhs)
{
}

HRESULT CBullet::Initialize_Prototype()
{
	FAILED_CHECK_RETURN(__super::Initialize_Prototype(), E_FAIL);

	return S_OK;
}

HRESULT CBullet::Initialize_Clone(const wstring& wstrPrototypeTag, void* pArg)
{
	FAILED_CHECK_RETURN(__super::Initialize_Clone(wstrPrototypeTag, pArg), E_FAIL);

	// if (nullptr == m_pOwner)
	// {
	// 	m_pOwner = CGameInstance::GetInstance()->Get_CloneObjectList(LEVEL_GAMEPLAY, L"Layer_Player")->front();
	// }
	// else
	// {
	// 	if (CGameInstance::GetInstance()->Get_CloneObjectList(LEVEL_GAMEPLAY, L"Layer_Player")->empty())
	// 	{
	// 		m_pOwner = nullptr;
	// 		return E_FAIL;
	// 	}
	// }
	FAILED_CHECK_RETURN(SetUp_Components(), E_FAIL);



	return S_OK;
}

void CBullet::Tick(_double dTimeDelta)
{
	__super::Tick(dTimeDelta);

	// if (nullptr == m_pOwner)
	// {
	// 	m_pOwner = CGameInstance::GetInstance()->Get_CloneObjectList(LEVEL_GAMEPLAY, L"Layer_Player")->front();
	// }
	// else
	// {
	// 	if(CGameInstance::GetInstance()->Get_CloneObjectList(LEVEL_GAMEPLAY, L"Layer_Player")->empty())
	// 	{
	// 		m_pOwner = nullptr;
	// 		return;
	// 	}
	// }

	_float4 tmp = m_vInitPos -= m_pTransformCom->Get_State(CTransform::STATE_TRANSLATION);

	if(	tmp.x + tmp.y + tmp.z  / 3.f > 500.f)
	{
		Set_Dead(true);
	}
	

}

void CBullet::Late_Tick(_double dTimeDelta)
{
	__super::Late_Tick(dTimeDelta);



	if (m_bCollOnce == false && m_tBulletOption.m_eOwner == OWNER_PLAYER)
	{
		Collision_Body();
		Collision_Head();
		Collision_HideCollider();
	}


	if (nullptr != m_pRendererCom &&
		true == CGameInstance::GetInstance()->isInFrustum_WorldSpace(m_pTransformCom->Get_State(CTransform::STATE_TRANSLATION), 2.f))
	{
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONLIGHT, this);
		m_pRendererCom->Add_DebugRenderGroup(m_pBulletColliderCom);
	}


}

HRESULT CBullet::Render()
{
	FAILED_CHECK_RETURN(__super::Render(), E_FAIL);


	return S_OK;
}

_bool CBullet::Collision_Body()
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	// 몬스터 리스트를 가져와서 순회해야할것같음
	m_MonsterList = *pGameInstance->Get_CloneObjectList(LEVEL_GAMEPLAY, L"Layer_Monster");
	_int ListSize = 0;
	ListSize = (_int)m_MonsterList.size();

	if (ListSize != 0)
	{
		auto iter = m_MonsterList.begin();
		for (auto& iter = m_MonsterList.begin(); iter != m_MonsterList.end();)
		{
			CCollider* pCollider = (CCollider*)(*iter)->Find_Component(L"Com_HitBodySphere");

			if (pCollider == nullptr)
				++iter;
			else if (m_pBulletColliderCom->Collision(pCollider))
			{
				CMonster* pMonster = (CMonster*)pCollider->Get_Owner();
				NULL_CHECK_RETURN(pMonster, false);

				pMonster->Set_HitColor();
				pMonster->Collision_Body(this); // 총알이 어디 충돌했는지 판단하니까
				m_bCollOnce = true;
				return true;
			}
			else
				++iter;
		}
		return false;
	}
	return false;
}

_bool CBullet::Collision_Head()
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	// 몬스터 리스트를 가져와서 순회해야할것같음
	m_MonsterList = *pGameInstance->Get_CloneObjectList(LEVEL_GAMEPLAY, L"Layer_Monster");
	_int ListSize = 0;
	ListSize = (_int)m_MonsterList.size();

	if (ListSize != 0)
	{
		auto iter = m_MonsterList.begin();
		for (auto& iter = m_MonsterList.begin(); iter != m_MonsterList.end();)
		{
			CCollider* pCollider = (CCollider*)(*iter)->Find_Component(L"Com_HitHeadSphere");

			if (pCollider == nullptr)
				++iter;
			else if (m_pBulletColliderCom->Collision(pCollider))
			{
				CMonster* pMonster = (CMonster*)pCollider->Get_Owner();
				NULL_CHECK_RETURN(pMonster, false);

				pMonster->Set_HitColor();
				pMonster->Collision_Body(this); // 총알이 어디 충돌했는지 판단하니까
				m_bCollOnce = true;
				return true;
			}
			else
				++iter;
		}
		return false;
	}
	return false;
}

_bool CBullet::Collision_HideCollider()
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	// 몬스터 리스트를 가져와서 순회해야할것같음
	m_MonsterList = *pGameInstance->Get_CloneObjectList(LEVEL_GAMEPLAY, L"Layer_Monster");
	_int ListSize = 0;
	ListSize = (_int)m_MonsterList.size();

	if (ListSize != 0)
	{
		auto iter = m_MonsterList.begin();
		for (auto& iter = m_MonsterList.begin(); iter != m_MonsterList.end();)
		{
			CCollider* pCollider = (CCollider*)(*iter)->Find_Component(L"Com_AttackRangeSphere");

			if (pCollider == nullptr)
				++iter;
			else if (m_pBulletColliderCom->Collision(pCollider))
			{
				CMonster* pMonster = (CMonster*)pCollider->Get_Owner();
				NULL_CHECK_RETURN(pMonster, false);

				pMonster->Collision_Hide(this); // 총알이 어디 충돌했는지 판단하니까
				return true;
			}
			else
				++iter;
		}
		return false;
	}
	return false;

	//
	// CGameInstance* pGameInstance = CGameInstance::GetInstance();
	//
	// CCollider* pCollider = (CCollider*)pGameInstance->Get_ComponentPtr(LEVEL_GAMEPLAY, L"Layer_Monster", L"Com_AttackRangeSphere");
	// if (pCollider == nullptr)
	// 	return false;
	//
	// if (m_pBulletColliderCom->Collision(pCollider))
	// {
	// 	CMonster* pMonster = (CMonster*)pCollider->Get_Owner();
	// 	NULL_CHECK_RETURN(pMonster, false);
	//
	// 	pMonster->Collision_Hide(this); // 총알이 어디 충돌했는지 판단하니까
	// 	// m_bCollOnce = true;
	// 	return true;					// 총알과 몬스터 둘다에 바디와 헤드 만들어서 충돌 이벤트 던지기
	// }
	// else
	// 	return false;
}

_bool CBullet::Collision_Player()
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	CCollider* pCollider = (CCollider*)pGameInstance->Get_ComponentPtr(LEVEL_GAMEPLAY, L"Layer_Player", L"Com_BODYSPHERE");
	NULL_CHECK_RETURN(pCollider, false);

	if (m_pBulletColliderCom->Collision(pCollider))
	{
		CPlayer* pPlayer = dynamic_cast<CPlayer*>(pCollider->Get_Owner());
		NULL_CHECK_RETURN(pPlayer, false);
		pPlayer->Collision_Event(dynamic_cast<CMonster*>(m_pOwner));
		return true;
	}
	return false;
}

HRESULT CBullet::SetUp_Components()
{

	CCollider::COLLIDERDESC			ColliderDesc;

	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));
	ColliderDesc.vPosition = _float3(0.f, 0.f, 0.f);
	ColliderDesc.vSize = _float3(0.3f, 0.3f, 0.5f);
	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Collider_SPHERE", L"Com_BulletSPHERE", (CComponent**)&m_pBulletColliderCom, this, &ColliderDesc), E_FAIL);

	return S_OK;
}

HRESULT CBullet::SetUp_ShaderResources()
{
	NULL_CHECK_RETURN(m_pShaderCom, E_FAIL);

	FAILED_CHECK_RETURN(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, L"g_WorldMatrix"), E_FAIL);
	FAILED_CHECK_RETURN(m_pShaderCom->Set_Matrix(L"g_ViewMatrix", &CGameInstance::GetInstance()->Get_TransformFloat4x4(CPipeLine::D3DTS_VIEW)), E_FAIL);
	FAILED_CHECK_RETURN(m_pShaderCom->Set_Matrix(L"g_ProjMatrix", &CGameInstance::GetInstance()->Get_TransformFloat4x4(CPipeLine::D3DTS_PROJ)), E_FAIL);

	return S_OK;
}




void CBullet::Free()
{
	__super::Free();

	Safe_Release(m_pModelCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pBulletColliderCom);
	Safe_Release(m_pPointBuffer);

}
