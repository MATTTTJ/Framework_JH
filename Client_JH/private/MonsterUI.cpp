#include "stdafx.h"
#include "..\public\MonsterUI.h"
#include "GameInstance.h"
#include "Monster.h"


CMonsterUI::CMonsterUI(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CUI(pDevice, pContext)
{
}

CMonsterUI::CMonsterUI(const CMonsterUI& rhs)
	:CUI(rhs)
{
}

HRESULT CMonsterUI::Initialize_Prototype()
{
	FAILED_CHECK_RETURN(__super::Initialize_Prototype(), E_FAIL);

	return S_OK;
}

HRESULT CMonsterUI::Initialize_Clone(const wstring& wstrPrototypeTag, void* pArg)
{
	GAMEOBJECTDESC GameObjectDesc;
	ZeroMemory(&GameObjectDesc, sizeof(GAMEOBJECTDESC));

	if (nullptr != pArg)
		m_tMonsterUIDesc = *(MONSTERUIDESC*)pArg;

	NULL_CHECK_RETURN(m_tMonsterUIDesc.pModel, E_FAIL);
	NULL_CHECK_RETURN(m_tMonsterUIDesc.pMonster, E_FAIL);

	FAILED_CHECK_RETURN(__super::Initialize_Clone(wstrPrototypeTag, pArg), E_FAIL);
	FAILED_CHECK_RETURN(SetUp_Component(), E_FAIL);

	m_pMonster = m_tMonsterUIDesc.pMonster;
	m_pModelCom = m_tMonsterUIDesc.pModel;

	m_fSizeX = 200.f;
	m_fSizeY = 30.f;
	// m_fY= //  m_pMonster->Get_TransformState(CTransform::STATE_TRANSLATION);
	m_fX = m_fSizeX * 0.5f;
	m_fY = m_fSizeY * 0.5f;
	m_vPSize.x = 1.5f;
	m_vPSize.y = 0.3f;

	// m_pTransformCom->Set_Scaled(_float3(1.f, 1.f, 1.f));
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION,(m_pModelCom->Get_BoneMatrix("MonsterHp")  * CGameUtils::Get_PlayerPivotMatrix() * m_pMonster->Get_Transform()->Get_WorldMatrix()).r[3]);



	// m_pTransformCom->Set_Scaled(_float3(m_fSizeX, m_fSizeY, 1.f));
	// m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fX - m_fSizeX * 0.5f, -m_fY + m_fSizeY * 0.5f, 0.f, 1.f));

	return S_OK;
}

void CMonsterUI::Tick(_double dTimeDelta)
{
	__super::Tick(dTimeDelta);

	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, (m_pModelCom->Get_BoneMatrix("MonsterHp")  * CGameUtils::Get_PlayerPivotMatrix() * m_pMonster->Get_Transform()->Get_WorldMatrix()).r[3]);

	_float4 tmp = m_pTransformCom->Get_State(CTransform::STATE_TRANSLATION);

}

void CMonsterUI::Late_Tick(_double dTimeDelta)
{
	__super::Late_Tick(dTimeDelta);

	if (nullptr != m_pRendererCom &&
		true == CGameInstance::GetInstance()->isInFrustum_WorldSpace(m_pTransformCom->Get_State(CTransform::STATE_TRANSLATION), 2.f))
			m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);
}

HRESULT CMonsterUI::Render()
{
	FAILED_CHECK_RETURN(__super::Render(), E_FAIL);

	FAILED_CHECK_RETURN(SetUp_ShaderResources(), E_FAIL);

	m_pShaderCom->Begin(2);

	m_pPointBuffer->Render();

	return S_OK;
}

HRESULT CMonsterUI::SetUp_Component()
{
	FAILED_CHECK_RETURN(__super::Add_Component(CGameInstance::Get_StaticLevelIndex(), L"Prototype_Component_Renderer", L"Com_Renderer", (CComponent**)&m_pRendererCom, this), E_FAIL);
	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Shader_VtxPointInstance", L"Com_Shader", (CComponent**)&m_pShaderCom, this), E_FAIL);
	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_VIBuffer_Point_Instancing", L"Com_VIBuffer", (CComponent**)&m_pPointBuffer, this), E_FAIL);
	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_MonsterUI_Base", L"Com_Texture", (CComponent**)&m_pTextureCom, this), E_FAIL);

	return S_OK;
}

HRESULT CMonsterUI::SetUp_ShaderResources()
{
	NULL_CHECK_RETURN(m_pShaderCom, E_FAIL);
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);
	FAILED_CHECK_RETURN(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, L"g_WorldMatrix"), E_FAIL);
	FAILED_CHECK_RETURN(m_pShaderCom->Set_Matrix(L"g_ViewMatrix", &pGameInstance->Get_TransformFloat4x4(CPipeLine::D3DTS_VIEW)), E_FAIL);
	FAILED_CHECK_RETURN(m_pShaderCom->Set_Matrix(L"g_ProjMatrix", &pGameInstance->Get_TransformFloat4x4(CPipeLine::D3DTS_PROJ)), E_FAIL);
	FAILED_CHECK_RETURN(m_pShaderCom->Set_RawValue(L"g_vCamPosition", &pGameInstance->Get_CamPos(), sizeof(_float4)), E_FAIL);
	FAILED_CHECK_RETURN(m_pShaderCom->Set_RawValue(L"g_vPSize", &m_vPSize, sizeof(_float2)), E_FAIL);

	RELEASE_INSTANCE(CGameInstance);
	FAILED_CHECK_RETURN(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, L"g_Texture"), E_FAIL);

	return S_OK;
}

CMonsterUI* CMonsterUI::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CMonsterUI*	pInstance = new CMonsterUI(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Create : CMonsterUI");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CMonsterUI::Clone(const wstring& wstrPrototypeTag, void* pArg)
{
	CMonsterUI*	pInstance = new CMonsterUI(*this);

	if (FAILED(pInstance->Initialize_Clone(wstrPrototypeTag, pArg)))
	{
		MSG_BOX("Failed to Create : CMonsterUI");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMonsterUI::Free()
{
	__super::Free();
}
