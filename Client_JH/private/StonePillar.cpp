#include "stdafx.h"
#include "..\public\StonePillar.h"
#include "Player.h"
#include "Bullet.h"
#include "DangerRing.h"
#include "GameInstance.h"
#include "MagicStone.h"


CStonePillar::CStonePillar(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CGameObject(pDevice, pContext)
{
}

CStonePillar::CStonePillar(const CStonePillar& rhs)
	: CGameObject(rhs)
{

}

HRESULT CStonePillar::Initialize_Prototype()
{
	m_bHasModel = true;

	FAILED_CHECK_RETURN(__super::Initialize_Prototype(), E_FAIL);

	return S_OK;
}

HRESULT CStonePillar::Initialize_Clone(const wstring& wstrPrototypeTag, void* pArg)
{
	if(nullptr != pArg)
	{
		FAILED_CHECK_RETURN(__super::Initialize_Clone(wstrPrototypeTag, pArg), E_FAIL);
		ZeroMemory(&m_tStonePillarDesc, sizeof(CGameObject::GAMEOBJECTDESC));

		m_tStonePillarDesc = *(GAMEOBJECTDESC*)pArg;

		_float4 InitPos = XMVectorSet(m_tStonePillarDesc.TransformDesc.vInitPos.x + CGameUtils::GetRandomFloat(-3.f, 3.f), -7.5f, m_tStonePillarDesc.TransformDesc.vInitPos.z + CGameUtils::GetRandomFloat(-3.f, 3.f), 1.f);
		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, InitPos);
		m_pTransformCom->Set_Scaled(_float3(1.f, 1.f, 1.f));
	}
	else
	{
		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(0.f, 0.f, 0.f, 1.f));
		m_pTransformCom->Set_Scaled(_float3(10.f, 10.f, 1.f));
	}


	// m_pTransformCom->Set_Scaled(_float3(0.009f, 0.009f, 0.009f));

	Ready_DangerEffect();

	FAILED_CHECK_RETURN(SetUp_Components(), E_FAIL);
	m_fMaxHeight = -0.135f;
	return S_OK;
}

void CStonePillar::Tick(_double TimeDelta)
{
	if(m_bIsDead)
	{
		return;
	}

	__super::Tick(TimeDelta);

	{
		m_fCurGrowTime += _float(TimeDelta);

		if(m_bDeadTimerStart == true)
		{
			m_fCurDeadTimer += _float(TimeDelta);
		}
	}

	if(m_fCurDeadTimer>= m_fDeadTimer)
	{
		if(m_bCollisionOnce == false)
		{
			Collision_To_Player_WhenExplode();
		}

		Set_Dead(true);
	}

	_float4 Pos = m_pTransformCom->Get_State(CTransform::STATE_TRANSLATION);
	m_fCurHeight = Pos.y;
	if (m_fCurGrowTime > m_fGrowTime)
	{
		if (m_fCurHeight < m_fMaxHeight)
		{
			m_fAddHeight += (_float)(TimeDelta * 25.f);
			m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(Pos.x, m_fCurHeight + m_fAddHeight, Pos.z, 1.f));
		}
		else
			m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(Pos.x, -0.135f, Pos.z, 1.f));
	}

	if(m_fCurGrowTime > 15.f)
	{
		m_fCurGrowTime = 10.f;
	}
	m_pStonePillarColliderCom->Update(m_pTransformCom->Get_WorldMatrix());
	m_pStonePillarExplodeCollCom->Update(m_pTransformCom->Get_WorldMatrix());
}

void CStonePillar::Late_Tick(_double TimeDelta)
{
	__super::Late_Tick(TimeDelta);

	if(m_pStonePillarColliderCom != nullptr)
	{
		Collision_To_Bullet();
		Collision_To_MagicStone();
		Collision_To_LaserBullet();
	}

	if (nullptr != m_pRendererCom)
	{
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_DYNAMIC_SHADOWDEPTH, this);
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHABLEND, this);
#ifdef _DEBUG
		m_pRendererCom->Add_DebugRenderGroup(m_pStonePillarColliderCom);
		m_pRendererCom->Add_DebugRenderGroup(m_pStonePillarExplodeCollCom);
#endif
	}
}

HRESULT CStonePillar::Render()
{
	FAILED_CHECK_RETURN(__super::Render(), E_FAIL);

	_uint iNumMeshes = m_pModelCom->Get_NumMeshes();

	for (_uint i = 0; i < iNumMeshes; ++i)
	{
		m_pModelCom->Bind_Material(m_pShaderCom, i, aiTextureType_DIFFUSE, L"g_DiffuseTexture");
		FAILED_CHECK_RETURN(SetUp_ShaderResources(), E_FAIL);
		m_pModelCom->Render(m_pShaderCom, i);
	}

	return S_OK;
}

void CStonePillar::Ready_DangerEffect()
{
	CDangerRing::RINGDESC		RingDesc;
	_float4 Pos = m_pTransformCom->Get_State(CTransform::STATE_TRANSLATION);
	RingDesc.GameObjectDesc.TransformDesc.vInitPos = _float3(Pos.x, 3.68f, Pos.z);
	RingDesc.GameObjectDesc.m_iHP = 1;
	RingDesc.m_fMinSize = 1.f;
	RingDesc.m_fMaxSize = 7.f;
	RingDesc.m_eType = CDangerRing::RINGDESC::RINGTYPE_PILLAR;
	(CGameInstance::GetInstance()->Clone_GameObjectReturnPtr(LEVEL_GAMEPLAY, L"Layer_DangerRing", L"Prototype_GameObject_Danger_Ring", &RingDesc));
	RingDesc.GameObjectDesc.TransformDesc.vInitPos = _float3(Pos.x, 3.68f, Pos.z);
	RingDesc.GameObjectDesc.m_iHP = 0;
	(CGameInstance::GetInstance()->Clone_GameObjectReturnPtr(LEVEL_GAMEPLAY, L"Layer_DangerRing", L"Prototype_GameObject_Danger_Ring", &RingDesc));
}

void CStonePillar::Collision_To_Golem()
{

	CGameObject::GAMEOBJECTDESC GameObjectDesc;
	_float4 Pos = m_pTransformCom->Get_State(CTransform::STATE_TRANSLATION);
	GameObjectDesc.TransformDesc.vInitPos = _float3(Pos.x, Pos.y, Pos.z);
	(CGameInstance::GetInstance()->Clone_GameObjectReturnPtr(LEVEL_GAMEPLAY, L"Layer_StoneLight", L"Prototype_GameObject_Normal_Boss_StonePillar_Light", &GameObjectDesc));

	m_bDeadTimerStart = true;

}

_bool CStonePillar::Collision_To_MagicStone()
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	// 몬스터 리스트를 가져와서 순회해야할것같음
	list<CGameObject*>* CloneBullet = CGameInstance::GetInstance()->Get_CloneObjectList(LEVEL_GAMEPLAY, L"Layer_MagicStone");
	if (CloneBullet == nullptr || CloneBullet->size() == 0)
		return false;
	else
	{
		m_MagicStoneList = *pGameInstance->Get_CloneObjectList(LEVEL_GAMEPLAY, L"Layer_MagicStone");
		_int ListSize = 0;
		ListSize = (_int)m_MagicStoneList.size();

		if (ListSize != 0)
		{
			auto iter = m_MagicStoneList.begin();
			for (auto& iter = m_MagicStoneList.begin(); iter != m_MagicStoneList.end();)
			{
				if (iter == m_MagicStoneList.end())
					return false;

				CCollider* pCollider = (CCollider*)(*iter)->Find_Component(L"Com_MagicStoneSPHERE");

				if (pCollider == nullptr)
					++iter;
				else if (m_pStonePillarColliderCom->Collision(pCollider))
				{
					CMagicStone* pBullet = (CMagicStone*)pCollider->Get_Owner();
					NULL_CHECK_RETURN(pBullet, false);

					pBullet->Set_Dead(true); // 총알이 어디 충돌했는지 판단하니까
					Collision_To_Golem();
					return true;
				}
				else
					++iter;
			}
			return false;
		}
		return false;

	}
}

_bool CStonePillar::Collision_To_Bullet()
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	// 몬스터 리스트를 가져와서 순회해야할것같음
	list<CGameObject*>* CloneBullet = CGameInstance::GetInstance()->Get_CloneObjectList(LEVEL_GAMEPLAY, L"Layer_Bullet");
	if (CloneBullet == nullptr || CloneBullet->size() == 0)
		return false;
	else
	{
		m_BulletList = *pGameInstance->Get_CloneObjectList(LEVEL_GAMEPLAY, L"Layer_Bullet");
		_int ListSize = 0;
		ListSize = (_int)m_BulletList.size();

		if (ListSize != 0)
		{
			auto iter = m_BulletList.begin();
			for (auto& iter = m_BulletList.begin(); iter != m_BulletList.end();)
			{
				if (iter == m_BulletList.end())
					return false;

				CCollider* pCollider = (CCollider*)(*iter)->Find_Component(L"Com_BulletSPHERE");

				if (pCollider == nullptr)
					++iter;
				else if (m_pStonePillarColliderCom->Collision(pCollider))
				{
					CBullet* pBullet = (CBullet*)pCollider->Get_Owner();
					NULL_CHECK_RETURN(pBullet, false);

					pBullet->Set_Dead(true); // 총알이 어디 충돌했는지 판단하니까
					return true;
					// ++iter;
					// continue;
				}
				else
					++iter;
			}
			return false;
		}
		return false;

	}
}

_bool CStonePillar::Collision_To_LaserBullet()
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	// 몬스터 리스트를 가져와서 순회해야할것같음
	list<CGameObject*>* CloneBullet = CGameInstance::GetInstance()->Get_CloneObjectList(LEVEL_GAMEPLAY, L"Layer_LaserBullet");
	if (CloneBullet == nullptr || CloneBullet->size() == 0)
		return false;
	else
	{
		m_BulletList = *pGameInstance->Get_CloneObjectList(LEVEL_GAMEPLAY, L"Layer_LaserBullet");
		_int ListSize = 0;
		ListSize = (_int)m_BulletList.size();

		if (ListSize != 0)
		{
			auto iter = m_BulletList.begin();
			for (auto& iter = m_BulletList.begin(); iter != m_BulletList.end();)
			{
				if (iter == m_BulletList.end())
					return false;

				CCollider* pCollider = (CCollider*)(*iter)->Find_Component(L"Com_BulletSPHERE");

				if (pCollider == nullptr)
					++iter;
				else if (m_pStonePillarColliderCom->Collision(pCollider))
				{
					CBullet* pBullet = (CBullet*)pCollider->Get_Owner();
					NULL_CHECK_RETURN(pBullet, false);

					pBullet->Set_Dead(true); // 총알이 어디 충돌했는지 판단하니까
					Collision_To_Golem();
					return true;
				}
				else
					++iter;
			}
			return false;
		}
		return false;

	}
}

_bool CStonePillar::Collision_To_Player_WhenExplode()
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	CCollider* pCollider = (CCollider*)pGameInstance->Get_ComponentPtr(LEVEL_GAMEPLAY, L"Layer_Player", L"Com_BODYSPHERE");
	NULL_CHECK_RETURN(pCollider, false);

	if (m_pStonePillarExplodeCollCom->Collision(pCollider))
	{
		CPlayer* pPlayer = dynamic_cast<CPlayer*>(pCollider->Get_Owner());
		NULL_CHECK_RETURN(pPlayer, false);
		pPlayer->Collision_Event(this);
		m_bCollisionOnce = true;
		return true;
	}
	return false;
}

HRESULT CStonePillar::Render_ShadowDepth()
{
	if (nullptr == m_pShaderCom ||
		nullptr == m_pModelCom)
		return E_FAIL;

	CGameInstance*		pGameInstance = CGameInstance::GetInstance();
	FAILED_CHECK_RETURN(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, L"g_WorldMatrix"), E_FAIL);
	m_pShaderCom->Set_Matrix(L"g_ViewMatrix", &pGameInstance->Get_LightTransform(1, 0)); // D3DTS_VIEW
	m_pShaderCom->Set_Matrix(L"g_ProjMatrix", &pGameInstance->Get_LightTransform(1, 1)); // D3DTS_PROJ

	_uint iNumMeshes = m_pModelCom->Get_NumMeshes();

	for (_uint i = 0; i < iNumMeshes; ++i)
	{
		m_pModelCom->Render(m_pShaderCom, i, L"g_BoneMatrices", 5);
	}

	return S_OK;
}

HRESULT CStonePillar::SetUp_Components()
{
	FAILED_CHECK_RETURN(__super::Add_Component(CGameInstance::Get_StaticLevelIndex(), L"Prototype_Component_Renderer", L"Com_Renderer", (CComponent**)&m_pRendererCom, this), E_FAIL);
	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Shader_VtxNonAnim", L"Com_Shader", (CComponent**)&m_pShaderCom, this), E_FAIL);
	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Model_StonePillar", L"Com_Model", (CComponent**)&m_pModelCom, this), E_FAIL);
	CCollider::COLLIDERDESC			ColliderDesc;

	/* For.Com_OBB */
	
	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));
	ColliderDesc.vSize = _float3(3.f, 7.f, 3.f);
	ColliderDesc.vRotation = _float3(0.f, 0.f, 0.f);
	ColliderDesc.vPosition = _float3(0.f, 3.5f, 0.f);
	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Collider_OBB", L"Com_StonePillarColl", (CComponent**)&m_pStonePillarColliderCom, this, &ColliderDesc), E_FAIL);


	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));
	ColliderDesc.vSize = _float3(10.f, 10.f, 10.f);
	ColliderDesc.vPosition = _float3(0.f, 3.f, 0.f);
	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Collider_SPHERE", L"Com_ExplodeSPHERE", (CComponent**)&m_pStonePillarExplodeCollCom, this, &ColliderDesc), E_FAIL);
	return S_OK;
}

HRESULT CStonePillar::SetUp_ShaderResources()
{
	NULL_CHECK_RETURN(m_pShaderCom, E_FAIL);

	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);
	FAILED_CHECK_RETURN(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, L"g_WorldMatrix"), E_FAIL);
	FAILED_CHECK_RETURN(m_pShaderCom->Set_Matrix(L"g_ViewMatrix", &pGameInstance->Get_TransformFloat4x4(CPipeLine::D3DTS_VIEW)), E_FAIL);
	FAILED_CHECK_RETURN(m_pShaderCom->Set_Matrix(L"g_ProjMatrix", &pGameInstance->Get_TransformFloat4x4(CPipeLine::D3DTS_PROJ)), E_FAIL);

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

CStonePillar* CStonePillar::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CStonePillar*		pInstance = new CStonePillar(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CStonePillar");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CStonePillar::Clone(const wstring& wstrPrototypeTag, void* pArg)
{
	CStonePillar*		pInstance = new CStonePillar(*this);

	if (FAILED(pInstance->Initialize_Clone(wstrPrototypeTag, pArg)))
	{
		MSG_BOX("Failed to Created : CStonePillar");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CStonePillar::Free()
{
	__super::Free();
	Safe_Release(m_pStonePillarExplodeCollCom);
	Safe_Release(m_pStonePillarColliderCom);
	Safe_Release(m_pModelCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pRendererCom);
}
