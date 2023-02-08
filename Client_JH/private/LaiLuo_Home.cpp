#include "stdafx.h"
#include "..\public\LaiLuo_Home.h"

#include "Dynamic_Camera.h"
#include "GameInstance.h"

CLaiLuo_Home::CLaiLuo_Home(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CGameObject(pDevice, pContext)
{

}

CLaiLuo_Home::CLaiLuo_Home(const CLaiLuo_Home & rhs)
	: CGameObject(rhs)
{

}

HRESULT CLaiLuo_Home::Initialize_Prototype()
{
	m_bHasModel = true;

	FAILED_CHECK_RETURN(__super::Initialize_Prototype(), E_FAIL);

	return S_OK;
}

HRESULT CLaiLuo_Home::Initialize_Clone(const wstring& wstrPrototypeTag, void * pArg)
{
	CGameObject::GAMEOBJECTDESC		GameObjectDesc;
	ZeroMemory(&GameObjectDesc, sizeof(GAMEOBJECTDESC));
	//
	// GameObjectDesc.TransformDesc.fSpeedPerSec = 7.0;
	// GameObjectDesc.TransformDesc.fRotationPerSec = XMConvertToRadians(90.f);

	FAILED_CHECK_RETURN(__super::Initialize_Clone(wstrPrototypeTag, &GameObjectDesc), E_FAIL);

	FAILED_CHECK_RETURN(SetUp_Components(), E_FAIL);
	// m_pModelCom->Set_CurAnimIndex(rand() % 20);
	// m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(rand() % 10, 0.f, rand() % 10, 1.f));

	m_pModelCom->Set_CurAnimIndex(1);

	_bool&		DynamicCamera = dynamic_cast<CCamera*>(CGameInstance::GetInstance()->Get_CloneObjectList(LEVEL_GAMEPLAY, L"Layer_ZCamera")->front())->Get_RenderState();
	_bool&		StaticCamera = dynamic_cast<CCamera*>(CGameInstance::GetInstance()->Get_CloneObjectList(LEVEL_GAMEPLAY, L"Layer_ZCamera")->back())->Get_RenderState();
	DynamicCamera = !DynamicCamera;
	StaticCamera = !StaticCamera;
	_float scale[3]{ 1.f, 1.f, 1.f }, Rot[3]{ 21.148f, -41.228f, 0.f }, Pos[3]{ -20.526f, 5.805f, -30.465f };
	_matrix camWorld;
	ImGuizmo::RecomposeMatrixFromComponents(Pos, Rot, scale, (_float*)&camWorld);
	CGameInstance::GetInstance()->Change_Camera();
	dynamic_cast<CDynamic_Camera*>(CGameInstance::GetInstance()->Get_CloneObjectList(LEVEL_GAMEPLAY, L"Layer_ZCamera")->front())->Set_Boss_IntroCam(camWorld);
	dynamic_cast<CDynamic_Camera*>(CGameInstance::GetInstance()->Get_CloneObjectList(LEVEL_GAMEPLAY, L"Layer_ZCamera")->front())->Set_LobbyCam(true);

	m_pModelCom->Set_CurAnimIndex(1);
	// _vector vPos = m_pTransformCom->Get_State(CTransform::STATE_TRANSLATION);
	// CGameInstance::GetInstance()->Set_LightPos(0, vPos);
	_float4 vDirection;
	_matrix	RotationMatrix = XMMatrixRotationAxis(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(32.2158));
	XMStoreFloat4(&vDirection, XMVector3TransformNormal(XMVectorSet(-cosf(XMConvertToRadians(60.f)), -sinf(XMConvertToRadians(60.f)), 0.f, 0.f), RotationMatrix));

	CGameInstance::GetInstance()->Set_LightDirection(1, vDirection);
	return S_OK;
}

void CLaiLuo_Home::Tick(_double TimeDelta)
{
	__super::Tick(TimeDelta);
	
	// CGameInstance::GetInstance()->Set_Far(6.f);
	_vector vPos = m_pTransformCom->Get_State(CTransform::STATE_TRANSLATION);
	CGameInstance::GetInstance()->Set_LightPos(0, vPos);

	// if (CGameInstance::GetInstance()->Key_Pressing(DIK_F3))
	// {
	// 	m_fDegree += (_float)TimeDelta * 10.f;
	// 	_float4 vDirection;
	// 	_matrix	RotationMatrix = XMMatrixRotationAxis(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(32.2158));
	// 	XMStoreFloat4(&vDirection, XMVector3TransformNormal(XMVectorSet(-cosf(XMConvertToRadians(60.f)), -sinf(XMConvertToRadians(60.f)), 0.f, 0.f), RotationMatrix));
	// 	
	// 	CGameInstance::GetInstance()->Set_LightDirection(1, vDirection); 
	// }

	m_pModelCom->Play_Animation(TimeDelta);

}

void CLaiLuo_Home::Late_Tick(_double TimeDelta)
{
	__super::Late_Tick(TimeDelta);

	if (nullptr != m_pRendererCom)
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHABLEND, this);
}

HRESULT CLaiLuo_Home::Render()
{
	FAILED_CHECK_RETURN(__super::Render(), E_FAIL);
	FAILED_CHECK_RETURN(SetUp_ShaderResources(), E_FAIL);

	_uint iNumMeshes = m_pModelCom->Get_NumMeshes();

	for (_uint i = 0; i < iNumMeshes; ++i)
	{
		/* �� ���� �׸������� ���̴��� ���׸��� �ؽ��ĸ� �����ϳ�. */
		m_pModelCom->Bind_Material(m_pShaderCom, i, aiTextureType_DIFFUSE, L"g_DiffuseTexture");

		m_pModelCom->Render(m_pShaderCom, i, L"g_BoneMatrices");
	}

	return S_OK;
}

HRESULT CLaiLuo_Home::SetUp_Components()
{
	FAILED_CHECK_RETURN(__super::Add_Component(CGameInstance::Get_StaticLevelIndex(), L"Prototype_Component_Renderer", L"Com_Renderer",	(CComponent**)&m_pRendererCom, this), E_FAIL);
	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Shader_VtxAnimModel", L"Com_Shader", (CComponent**)&m_pShaderCom, this), E_FAIL);
	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Model_LaiHome", L"Com_Model", (CComponent**)&m_pModelCom, this), E_FAIL);
	
	return S_OK;
}

HRESULT CLaiLuo_Home::SetUp_ShaderResources()
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

CLaiLuo_Home * CLaiLuo_Home::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CLaiLuo_Home*		pInstance = new CLaiLuo_Home(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CLaiLuo_Home");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CLaiLuo_Home::Clone(const wstring& wstrPrototypeTag, void * pArg)
{
	CLaiLuo_Home*		pInstance = new CLaiLuo_Home(*this);

	if (FAILED(pInstance->Initialize_Clone(wstrPrototypeTag, pArg)))
	{
		MSG_BOX("Failed to Cloned : CLaiLuo_Home");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CLaiLuo_Home::Free()
{

	__super::Free();

	Safe_Release(m_pModelCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pRendererCom);

}
