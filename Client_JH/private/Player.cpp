#include "stdafx.h"
#include "..\public\Player.h"
#include "GameInstance.h"

CPlayer::CPlayer(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CGameObject(pDevice, pContext)
{

}

CPlayer::CPlayer(const CPlayer & rhs)
	: CGameObject(rhs)
{

}

_matrix CPlayer::Get_BoneMatrix(const string& strBoneName)
{
	if (nullptr == m_pModelCom)
		return XMMatrixIdentity();

	return m_pModelCom->Get_BoneMatrix(strBoneName);
}

_matrix CPlayer::Get_OffsetMatrix(const string& strBoneName)
{
	if (nullptr == m_pModelCom)
		return XMMatrixIdentity();

	return m_pModelCom->Get_OffsetMatrix(strBoneName);
}

_matrix CPlayer::Get_PivotMatrix()
{
	if (nullptr == m_pModelCom)
		return XMMatrixIdentity();

	return m_pModelCom->Get_PivotMatrix();
}

_vector CPlayer::Get_TransformState(CTransform::STATE eState)
{
	if (m_pModelCom == nullptr)
		return XMVectorSet(0.f, 1.f, 0.f, 0.f);

	return m_pTransformCom->Get_State(eState);
}

HRESULT CPlayer::Initialize_Prototype()
{
	m_bHasModel = true;

	FAILED_CHECK_RETURN(__super::Initialize_Prototype(), E_FAIL);

	return S_OK;
}

HRESULT CPlayer::Initialize_Clone(const wstring& wstrPrototypeTag, void * pArg)
{
	CGameObject::GAMEOBJECTDESC		GameObjectDesc;
	ZeroMemory(&GameObjectDesc, sizeof(GAMEOBJECTDESC));

	GameObjectDesc.TransformDesc.fSpeedPerSec = 7.0;
	GameObjectDesc.TransformDesc.fRotationPerSec = XMConvertToRadians(90.f);

	FAILED_CHECK_RETURN(__super::Initialize_Clone(wstrPrototypeTag, &GameObjectDesc), E_FAIL);

	FAILED_CHECK_RETURN(SetUp_Components(), E_FAIL);
	// m_pModelCom->Set_CurAnimIndex(rand() % 20);
	// m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(rand() % 10, 0.f, rand() % 10, 1.f));

	m_pModelCom->Set_CurAnimIndex(3);

	return S_OK;
}

void CPlayer::Tick(_double TimeDelta)
{
	__super::Tick(TimeDelta);

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	if (pGameInstance->Get_DIKeyState(DIK_DOWN))
	{
		m_pTransformCom->Go_Backward(TimeDelta);
	}

	if (pGameInstance->Get_DIKeyState(DIK_LEFT))
	{
		m_pTransformCom->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), TimeDelta * -1.f);
	}

	if (pGameInstance->Get_DIKeyState(DIK_RIGHT))
	{
		m_pTransformCom->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), TimeDelta);
	}

	if (pGameInstance->Get_DIKeyState(DIK_UP))
	{
		m_pTransformCom->Go_Straight(TimeDelta);
		m_pModelCom->Set_CurAnimIndex(4);
	}
	else
		m_pModelCom->Set_CurAnimIndex(3);

	m_pModelCom->Play_Animation(TimeDelta);

	RELEASE_INSTANCE(CGameInstance);
}

void CPlayer::Late_Tick(_double TimeDelta)
{
	__super::Late_Tick(TimeDelta);

	if (nullptr != m_pRendererCom)
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHABLEND, this);
}

HRESULT CPlayer::Render()
{
	FAILED_CHECK_RETURN(__super::Render(), E_FAIL);
	FAILED_CHECK_RETURN(SetUp_ShaderResources(), E_FAIL);

	_uint iNumMeshes = m_pModelCom->Get_NumMeshes();

	for (_uint i = 0; i < iNumMeshes; ++i)
	{
		/* 이 모델을 그리기위한 셰이더에 머테리얼 텍스쳐를 전달하낟. */
		m_pModelCom->Bind_Material(m_pShaderCom, i, aiTextureType_DIFFUSE, L"g_DiffuseTexture");

		m_pModelCom->Render(m_pShaderCom, i, L"g_BoneMatrices");
	}

	return S_OK;
}

HRESULT CPlayer::SetUp_Components()
{
	/* For.Com_Renderer */
	FAILED_CHECK_RETURN(__super::Add_Component(CGameInstance::Get_StaticLevelIndex(), L"Prototype_Component_Renderer", L"Com_Renderer",
		(CComponent**)&m_pRendererCom), E_FAIL);

	/* For.Com_Shader */
	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Shader_VtxAnimModel", L"Com_Shader",
		(CComponent**)&m_pShaderCom), E_FAIL);

	/* For.Com_Model */
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Model_Fiona", L"Com_Model",
	(CComponent**)&m_pModelCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CPlayer::SetUp_ShaderResources()
{
	NULL_CHECK_RETURN(m_pShaderCom, E_FAIL);

	FAILED_CHECK_RETURN(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, L"g_WorldMatrix"), E_FAIL);

	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	FAILED_CHECK_RETURN(m_pShaderCom->Set_Matrix(L"g_ViewMatrix", &pGameInstance->Get_TransformFloat4x4(CPipeLine::D3DTS_VIEW)), E_FAIL);

	FAILED_CHECK_RETURN(m_pShaderCom->Set_Matrix(L"g_ProjMatrix", &pGameInstance->Get_TransformFloat4x4(CPipeLine::D3DTS_PROJ)), E_FAIL);

	/* For.Lights */
	const LIGHTDESC* pLightDesc = pGameInstance->Get_LightDesc(0);
	if (nullptr == pLightDesc)
		return E_FAIL;

	//if (FAILED(m_pShaderCom->Set_RawValue("g_vCamPosition", &pGameInstance->Get_CamPosition(), sizeof(_float4))))
	//	return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);





	return S_OK;
}

CPlayer * CPlayer::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CPlayer*		pInstance = new CPlayer(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CPlayer");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CPlayer::Clone(const wstring& wstrPrototypeTag, void * pArg)
{
	CPlayer*		pInstance = new CPlayer(*this);

	if (FAILED(pInstance->Initialize_Clone(wstrPrototypeTag, pArg)))
	{
		MSG_BOX("Failed to Cloned : CPlayer");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CPlayer::Free()
{
	__super::Free();

	Safe_Release(m_pModelCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pRendererCom);

}
