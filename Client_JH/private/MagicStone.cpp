#include "stdafx.h"
#include "..\public\MagicStone.h"
#include "GameInstance.h"

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
		FAILED_CHECK_RETURN(__super::Initialize_Clone(wstrPrototypeTag, pArg), E_FAIL);
		ZeroMemory(&m_tStonePillarDesc, sizeof(CGameObject::GAMEOBJECTDESC));

		m_tStonePillarDesc = *(GAMEOBJECTDESC*)pArg;
		// 위치 잡는거 수정
		_float4 InitPos = XMVectorSet(m_tStonePillarDesc.TransformDesc.vInitPos.x + CGameUtils::GetRandomFloat(-3.f, 3.f), -7.5f, m_tStonePillarDesc.TransformDesc.vInitPos.z + CGameUtils::GetRandomFloat(-3.f, 3.f), 1.f);
		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, InitPos);
		m_pTransformCom->Set_Scaled(_float3(1.f, 1.f, 1.f));
	}
	else
	{
		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(0.f, 0.f, 0.f, 1.f));
		m_pTransformCom->Set_Scaled(_float3(10.f, 10.f, 1.f));
	}

	Ready_DangerEffect(); // 돌이모이면서 메시가 생기는거처럼

	FAILED_CHECK_RETURN(SetUp_Components(), E_FAIL);
	m_fMaxHeight = -0.135f;
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
		m_fCurGrowTime += _float(TimeDelta);
	}

	// 돌이모이는 효과 끝나면 사이즈 점점 커지게 수정

	_float4 Pos = m_pTransformCom->Get_State(CTransform::STATE_TRANSLATION);
	m_fCurHeight = Pos.y;
	if (m_fCurGrowTime > m_fGrowTime)
	{
		if (m_fCurHeight < m_fMaxHeight)
		{
			m_fAddHeight += (_float)(TimeDelta * 12.f);
			m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(Pos.x, m_fCurHeight + ((_float)TimeDelta * 2.f), Pos.z, 1.f));
		}
		else
			m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(Pos.x, -0.135f, Pos.z, 1.f));
	}

	if (m_fCurGrowTime > 15.f)
	{
		m_fCurGrowTime = 10.f;
	}
	m_pStonePillarColliderCom->Update(m_pTransformCom->Get_WorldMatrix());


	if (CGameInstance::GetInstance()->Key_Down(DIK_F9))
	{
		Set_Dead(true);
	}
}

void CMagicStone::Late_Tick(_double TimeDelta)
{
	__super::Late_Tick(TimeDelta);

	if (m_pStonePillarColliderCom != nullptr)
	{
		Collision_To_Bullet();
		Collision_To_Golem();
		Collision_To_MagicStone();
	}

	if (nullptr != m_pRendererCom)
	{
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHABLEND, this);
		m_pRendererCom->Add_DebugRenderGroup(m_pStonePillarColliderCom);
	}
}

HRESULT CMagicStone::Render()
{
	FAILED_CHECK_RETURN(__super::Render(), E_FAIL);

	_uint iNumMeshes = m_pModelCom->Get_NumMeshes();

	for (_uint i = 0; i < iNumMeshes; ++i)
	{
		m_pModelCom->Bind_Material(m_pShaderCom, i, aiTextureType_DIFFUSE, L"g_DiffuseTexture");


		FAILED_CHECK_RETURN(SetUp_ShaderResources(), E_FAIL);

		m_pModelCom->Render(m_pShaderCom, i, L"g_BoneMatrices");
	}

	return S_OK;
}

void CMagicStone::Ready_DangerEffect()
{
	CGameObject::GAMEOBJECTDESC GameObjectDesc;
	_float4 Pos = m_pTransformCom->Get_State(CTransform::STATE_TRANSLATION);
	GameObjectDesc.TransformDesc.vInitPos = _float3(Pos.x, 3.68f, Pos.z);
	GameObjectDesc.m_iHP = 1;
	(CGameInstance::GetInstance()->Clone_GameObjectReturnPtr(LEVEL_GAMEPLAY, L"Layer_DangerRing", L"Prototype_GameObject_Danger_Ring", &GameObjectDesc));
	GameObjectDesc.TransformDesc.vInitPos = _float3(Pos.x, 3.68f, Pos.z);
	GameObjectDesc.m_iHP = 0;
	(CGameInstance::GetInstance()->Clone_GameObjectReturnPtr(LEVEL_GAMEPLAY, L"Layer_DangerRing", L"Prototype_GameObject_Danger_Ring", &GameObjectDesc));
}

_bool CMagicStone::Collision_To_Golem()
{
	return false;
}

_bool CMagicStone::Collision_To_MagicStone()
{
	return false;
}

_bool CMagicStone::Collision_To_Bullet()
{
	return false;
}

HRESULT CMagicStone::SetUp_Components()
{
	FAILED_CHECK_RETURN(__super::Add_Component(CGameInstance::Get_StaticLevelIndex(), L"Prototype_Component_Renderer", L"Com_Renderer", (CComponent**)&m_pRendererCom, this), E_FAIL);
	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Shader_VtxNonAnim", L"Com_Shader", (CComponent**)&m_pShaderCom, this), E_FAIL);
	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Model_StonePillar", L"Com_Model", (CComponent**)&m_pModelCom, this), E_FAIL);
	CCollider::COLLIDERDESC			ColliderDesc;

	/* For.Com_OBB */ // 스피어로 바꾸기 

	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));
	ColliderDesc.vSize = _float3(3.f, 7.f, 3.f);
	ColliderDesc.vRotation = _float3(0.f, 0.f, 0.f);
	ColliderDesc.vPosition = _float3(0.f, 3.5f, 0.f);
	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Collider_OBB", L"Com_StonePillarColl", (CComponent**)&m_pStonePillarColliderCom, this, &ColliderDesc), E_FAIL);

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
	Safe_Release(m_pStonePillarColliderCom);
	Safe_Release(m_pModelCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pRendererCom);
}
