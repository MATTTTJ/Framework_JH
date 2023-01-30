#include "stdafx.h"
#include "..\public\MagicStone.h"

#include "Bullet.h"
#include "GameInstance.h"
#include "Player.h"

CMagicStone::CMagicStone(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CGameObject(pDevice, pContext)
{
}

CMagicStone::CMagicStone(const CMagicStone& rhs)
	: CGameObject(rhs)
{
}

HRESULT CMagicStone::Initialize_Prototype()
{
	m_bHasModel = true;

	FAILED_CHECK_RETURN(__super::Initialize_Prototype(), E_FAIL);

	return S_OK;
}

HRESULT CMagicStone::Initialize_Clone(const wstring& wstrPrototypeTag, void* pArg)
{
	if (nullptr != pArg)
	{

		ZeroMemory(&m_tMagicStoneDesc, sizeof(MAGICSTONEDESC));

		m_tMagicStoneDesc = *(MAGICSTONEDESC*)pArg;
		// 위치 잡는거 수정
		_float4 InitPos = XMVectorSet(m_tMagicStoneDesc.GameObjectDesc.TransformDesc.vInitPos.x + CGameUtils::GetRandomFloat(-8.f, 8.f), m_tMagicStoneDesc.GameObjectDesc.TransformDesc.vInitPos.y + CGameUtils::GetRandomFloat(-2.f, 2.f), m_tMagicStoneDesc.GameObjectDesc.TransformDesc.vInitPos.z + CGameUtils::GetRandomFloat(-5.f, 5.f), 1.f);

		m_tMagicStoneDesc.GameObjectDesc.TransformDesc.fSpeedPerSec = 15.f;
		m_tMagicStoneDesc.GameObjectDesc.TransformDesc.fRotationPerSec = XMConvertToDegrees(5.f);
		FAILED_CHECK_RETURN(__super::Initialize_Clone(wstrPrototypeTag, &m_tMagicStoneDesc), E_FAIL);

		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, InitPos);
		m_pTransformCom->Set_Scaled(_float3(0.1f, 0.1f, 0.1f));
		
		m_iRand = rand() % 4;

		m_pPlayer = dynamic_cast<CPlayer*>(m_tMagicStoneDesc.m_pTarget);
	}
	else
	{
		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(0.f, 0.f, 0.f, 1.f));
		m_pTransformCom->Set_Scaled(_float3(10.f, 10.f, 1.f));
	}
	// m_vDir = XMVector3Normalize(m_pPlayer->Get_TransformState(CTransform::STATE_TRANSLATION) - m_pTransformCom->Get_State(CTransform::STATE_TRANSLATION));
	m_vDir = m_pPlayer->Get_TransformState(CTransform::STATE_TRANSLATION);

	m_fFireCoolTime = 20.f + CGameUtils::GetRandomFloat(-5.f, 10.f);

	Ready_DangerEffect(); // 돌이모이면서 메시가 생기는거처럼

	FAILED_CHECK_RETURN(SetUp_Components(), E_FAIL);
	return S_OK;
}

void CMagicStone::Tick(_double TimeDelta)
{
	if (m_bIsDead)
	{
		return;
	}

	__super::Tick(TimeDelta);

	{
		if (m_bCanGrow == true)
		{// 좀더 빠르게 
			m_fMinSize += (_float)(TimeDelta * 0.5f);
		}

		if(m_bFire == false)
		{
			m_fCurFireCoolTime += (_float)TimeDelta;
		}
	}

	if(m_fMinSize < m_fMaxSize && m_bCanGrow == true)
	{
		m_pTransformCom->Set_Scaled(_float3(m_fMinSize, m_fMinSize, m_fMinSize));

		if(m_fMinSize >= 1.f)
			m_bCanGrow = false;
	}
	else
	{
		m_pTransformCom->Set_Scaled(_float3(1.f, 1.f, 1.f));
	}

	if(m_fCurFireCoolTime>= m_fFireCoolTime && m_bFire == false)
	{
		m_vDir = m_pPlayer->Get_TransformState(CTransform::STATE_TRANSLATION);
		m_bFire = true;
	}

	m_pTransformCom->Turn(m_pTransformCom->Get_State(CTransform::STATE_LOOK), XMConvertToRadians(0.001f + CGameUtils::GetRandomFloat(0.001f, 0.005f)));
	m_pTransformCom->Turn(m_pTransformCom->Get_State(CTransform::STATE_RIGHT), XMConvertToRadians(0.007f + CGameUtils::GetRandomFloat(-0.001f, 0.005f)));

	Fire_To_Player(TimeDelta);

	m_pMagicStoneColliderCom->Update(m_pTransformCom->Get_WorldMatrix());

	
}

void CMagicStone::Late_Tick(_double TimeDelta)
{
	__super::Late_Tick(TimeDelta);

	if (m_pMagicStoneColliderCom != nullptr)
	{
		Collision_To_Bullet();
	}

	if (nullptr != m_pRendererCom)
	{
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_DYNAMIC_SHADOWDEPTH, this);
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHABLEND, this);
		m_pRendererCom->Add_DebugRenderGroup(m_pMagicStoneColliderCom);
	}
}

HRESULT CMagicStone::Render()
{
	FAILED_CHECK_RETURN(__super::Render(), E_FAIL);

	_uint iNumMeshes = m_pModelCom[m_iRand]->Get_NumMeshes();

	for (_uint i = 0; i < iNumMeshes; ++i)
	{
		m_pModelCom[m_iRand]->Bind_Material(m_pShaderCom, i, aiTextureType_DIFFUSE, L"g_DiffuseTexture");


		FAILED_CHECK_RETURN(SetUp_ShaderResources(), E_FAIL);

		m_pModelCom[m_iRand]->Render(m_pShaderCom, i, L"g_BoneMatrices");
	}

	return S_OK;
}

void CMagicStone::Ready_DangerEffect()
{
	// CGameObject::GAMEOBJECTDESC GameObjectDesc;
	// _float4 Pos = m_pTransformCom->Get_State(CTransform::STATE_TRANSLATION);
	// GameObjectDesc.TransformDesc.vInitPos = _float3(Pos.x, 3.68f, Pos.z);
	// GameObjectDesc.m_iHP = 1;
	// (CGameInstance::GetInstance()->Clone_GameObjectReturnPtr(LEVEL_GAMEPLAY, L"Layer_DangerRing", L"Prototype_GameObject_Danger_Ring", &GameObjectDesc));
	// GameObjectDesc.TransformDesc.vInitPos = _float3(Pos.x, 3.68f, Pos.z);
	// GameObjectDesc.m_iHP = 0;
	// (CGameInstance::GetInstance()->Clone_GameObjectReturnPtr(LEVEL_GAMEPLAY, L"Layer_DangerRing", L"Prototype_GameObject_Danger_Ring", &GameObjectDesc));
}

_bool CMagicStone::Collision_To_Bullet()
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
				else if (m_pMagicStoneColliderCom->Collision(pCollider))
				{
					CBullet* pBullet = (CBullet*)pCollider->Get_Owner();
					NULL_CHECK_RETURN(pBullet, false);

					pBullet->Set_Dead(true); // 총알이 어디 충돌했는지 판단하니까
					Set_Dead(true);
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

void CMagicStone::Fire_To_Player(_double TimeDelta)
{
	if (m_bFire == false)
		return;

	_float4 CurPos = m_pTransformCom->Get_State(CTransform::STATE_TRANSLATION);
	_float4 TargetPos = m_pPlayer->Get_TransformState(CTransform::STATE_TRANSLATION);

	if (CurPos.y < TargetPos.y)
	{
		Set_Dead(true);
	}

	CurPos.y -= (_float)TimeDelta * CGameUtils::GetRandomFloat(0.f, 0.1f);
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, CurPos);

	m_pTransformCom->Chase(m_vDir, _float(TimeDelta * 3.f));

	// 플레이어 유도
}

HRESULT CMagicStone::Render_ShadowDepth()
{
	if (nullptr == m_pShaderCom ||
		nullptr == m_pModelCom)
		return E_FAIL;

	CGameInstance*		pGameInstance = CGameInstance::GetInstance();
	FAILED_CHECK_RETURN(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, L"g_WorldMatrix"), E_FAIL);
	m_pShaderCom->Set_Matrix(L"g_ViewMatrix", &pGameInstance->Get_LightTransform(1, 0)); // D3DTS_VIEW
	m_pShaderCom->Set_Matrix(L"g_ProjMatrix", &pGameInstance->Get_LightTransform(1, 1)); // D3DTS_PROJ

	_uint iNumMeshes = m_pModelCom[m_iRand]->Get_NumMeshes();

	for (_uint i = 0; i < iNumMeshes; ++i)
	{
		m_pModelCom[m_iRand]->Render(m_pShaderCom, i, L"g_BoneMatrices", 5);
	}

	return S_OK;
}

HRESULT CMagicStone::SetUp_Components()
{
	FAILED_CHECK_RETURN(__super::Add_Component(CGameInstance::Get_StaticLevelIndex(), L"Prototype_Component_Renderer", L"Com_Renderer", (CComponent**)&m_pRendererCom, this), E_FAIL);
	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Shader_VtxNonAnim", L"Com_Shader", (CComponent**)&m_pShaderCom, this), E_FAIL);
	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Model_MagicStone_A", L"Com_A_Model", (CComponent**)&m_pModelCom[STONE_A], this), E_FAIL);
	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Model_MagicStone_B", L"Com_B_Model", (CComponent**)&m_pModelCom[STONE_B], this), E_FAIL);
	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Model_MagicStone_C", L"Com_C_Model", (CComponent**)&m_pModelCom[STONE_C], this), E_FAIL);
	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Model_MagicStone_D", L"Com_D_Model", (CComponent**)&m_pModelCom[STONE_D], this), E_FAIL);


	CCollider::COLLIDERDESC			ColliderDesc;
	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));
	ColliderDesc.vPosition = _float3(0.f, 0.f, 0.f);
	ColliderDesc.vSize = _float3(3.f, 3.f, 3.f);
	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Collider_SPHERE", L"Com_MagicStoneSPHERE", (CComponent**)&m_pMagicStoneColliderCom, this, &ColliderDesc), E_FAIL);


	return S_OK;
}

HRESULT CMagicStone::SetUp_ShaderResources()
{
	NULL_CHECK_RETURN(m_pShaderCom, E_FAIL);

	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);
	FAILED_CHECK_RETURN(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, L"g_WorldMatrix"), E_FAIL);
	FAILED_CHECK_RETURN(m_pShaderCom->Set_Matrix(L"g_ViewMatrix", &pGameInstance->Get_TransformFloat4x4(CPipeLine::D3DTS_VIEW)), E_FAIL);
	FAILED_CHECK_RETURN(m_pShaderCom->Set_Matrix(L"g_ProjMatrix", &pGameInstance->Get_TransformFloat4x4(CPipeLine::D3DTS_PROJ)), E_FAIL);

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

CMagicStone* CMagicStone::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CMagicStone*		pInstance = new CMagicStone(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CMagicStone");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CMagicStone::Clone(const wstring& wstrPrototypeTag, void* pArg)
{
	CMagicStone*		pInstance = new CMagicStone(*this);

	if (FAILED(pInstance->Initialize_Clone(wstrPrototypeTag, pArg)))
	{
		MSG_BOX("Failed to Created : CMagicStone");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CMagicStone::Free()
{
	__super::Free();
	Safe_Release(m_pMagicStoneColliderCom);

	for (_int i = 0; i < STONETYPE_END; ++i)
	{
		Safe_Release(m_pModelCom[i]);
	}

	Safe_Release(m_pShaderCom);
	Safe_Release(m_pRendererCom);
}
