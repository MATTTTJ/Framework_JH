#include "stdafx.h"
#include "..\public\Damage_Font.h"
#include "GameInstance.h"

CDamage_Font::CDamage_Font(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CGameObject(pDevice, pContext)
{
}

CDamage_Font::CDamage_Font(const CDamage_Font& rhs)
	:CGameObject(rhs)
{
}


HRESULT CDamage_Font::Initialize_Prototype()
{
	FAILED_CHECK_RETURN(__super::Initialize_Prototype(), E_FAIL);

	return S_OK;
}

HRESULT CDamage_Font::Initialize_Clone(const wstring& wstrPrototypeTag, void* pArg)
{
	// m_fSizeX = 36.f;
	// m_fSizeY = 36.f;
	// m_fX = m_fSizeX * 0.5f;
	// m_fY = m_fSizeY * 0.5f;

	FAILED_CHECK_RETURN(SetUp_Components(), E_FAIL);


	if (nullptr != pArg)
	{
		CGameObject::GAMEOBJECTDESC		tmp;
		tmp = *(GAMEOBJECTDESC*)pArg;
		tmp.TransformDesc.fRotationPerSec = XMConvertToRadians(90.f);
		tmp.TransformDesc.fSpeedPerSec = 1.f;
		FAILED_CHECK_RETURN(__super::Initialize_Clone(wstrPrototypeTag, &tmp), E_FAIL);

		m_iNumCnt = tmp.m_iNumCnt;
		m_iNumber = tmp.m_iNumber;
		// m_vNumColor = tmp.m_vNumColor;
		m_iType = tmp.m_iCountType;
		m_fSizeX = tmp.m_vTexSize.x;
		m_fSizeY = tmp.m_vTexSize.y;
		m_vLook = tmp.m_vBulletLook;
		m_vPSize = tmp.m_vTexSize;
		m_pTransformCom->Set_Scaled(_float3(tmp.m_vTexSize.x, tmp.m_vTexSize.y, 1.f));
		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(tmp.TransformDesc.vInitPos.x, tmp.TransformDesc.vInitPos.y, 0.f, 1.f));
		m_pTransformCom->LookAt(m_pTransformCom->Get_State(CTransform::STATE_TRANSLATION) + XMLoadFloat4(&tmp.m_vBulletLook));

	}
	else
	{
		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(900.f, -315.f, 0.f, 1.f));
		m_pTransformCom->Set_Scaled(_float3(m_fSizeX, m_fSizeY, 1.f));
	}


	m_vCamPos = CGameInstance::GetInstance()->Get_CamPos();

	return S_OK;
}

void CDamage_Font::Tick(_double dTimeDelta)
{
	__super::Tick(dTimeDelta);

	{
		m_fCurLifeTime += (_float)dTimeDelta;
	}
	if (m_fCurLifeTime >= m_fLifeTime)
		Set_Dead(true);

	m_pTransformCom->Go_Straight(dTimeDelta, CTransform::TRANS_BULLET);

	_float4 Pos = m_pTransformCom->Get_State(CTransform::STATE_TRANSLATION);
	Pos.y -= (_float)dTimeDelta;
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, Pos);
}

void CDamage_Font::Late_Tick(_double dTimeDelta)
{
	__super::Late_Tick(dTimeDelta);

	if (nullptr != m_pRendererCom)
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);
}

HRESULT CDamage_Font::Render()
{
	FAILED_CHECK_RETURN(__super::Render(), E_FAIL);

	FAILED_CHECK_RETURN(SetUp_ShaderResources(), E_FAIL);

	m_pShaderCom->Begin(10);

	m_pPointBuffer->Render();

	return S_OK;
}

HRESULT CDamage_Font::SetUp_Components()
{
	FAILED_CHECK_RETURN(__super::Add_Component(CGameInstance::Get_StaticLevelIndex(), L"Prototype_Component_Renderer", L"Com_Renderer", (CComponent**)&m_pRendererCom, this), E_FAIL);
	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Shader_VtxPointInstance", L"Com_Shader", (CComponent**)&m_pShaderCom, this), E_FAIL);
	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_VIBuffer_Bullet_Instancing", L"Com_VIBuffer", (CComponent**)&m_pPointBuffer, this), E_FAIL);
	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Texture_Numbering_0", L"Com_Num0", (CComponent**)&m_pNumberingTexCom[ZERO], this), E_FAIL);
	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Texture_Numbering_1", L"Com_Num1", (CComponent**)&m_pNumberingTexCom[ONE], this), E_FAIL);
	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Texture_Numbering_2", L"Com_Num2", (CComponent**)&m_pNumberingTexCom[TWO], this), E_FAIL);
	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Texture_Numbering_3", L"Com_Num3", (CComponent**)&m_pNumberingTexCom[THREE], this), E_FAIL);
	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Texture_Numbering_4", L"Com_Num4", (CComponent**)&m_pNumberingTexCom[FOUR], this), E_FAIL);
	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Texture_Numbering_5", L"Com_Num5", (CComponent**)&m_pNumberingTexCom[FIVE], this), E_FAIL);
	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Texture_Numbering_6", L"Com_Num6", (CComponent**)&m_pNumberingTexCom[SIX], this), E_FAIL);
	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Texture_Numbering_7", L"Com_Num7", (CComponent**)&m_pNumberingTexCom[SEVEN], this), E_FAIL);
	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Texture_Numbering_8", L"Com_Num8", (CComponent**)&m_pNumberingTexCom[EIGHT], this), E_FAIL);
	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Texture_Numbering_9", L"Com_Num9", (CComponent**)&m_pNumberingTexCom[NINE], this), E_FAIL);

	return S_OK;
}

HRESULT CDamage_Font::SetUp_ShaderResources()
{
	NULL_CHECK_RETURN(m_pShaderCom, E_FAIL);

	// m_pShaderCom->Set_Matrix("g_WorldMatrix", &m_WorldMatrix);
	FAILED_CHECK_RETURN(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, L"g_WorldMatrix"), E_FAIL);

	FAILED_CHECK_RETURN(m_pShaderCom->Set_Matrix(L"g_ViewMatrix", &CGameInstance::GetInstance()->Get_TransformFloat4x4(CPipeLine::D3DTS_VIEW)), E_FAIL);
	FAILED_CHECK_RETURN(m_pShaderCom->Set_Matrix(L"g_ProjMatrix", &CGameInstance::GetInstance()->Get_TransformFloat4x4(CPipeLine::D3DTS_PROJ)), E_FAIL);
	if (m_iNumber == 0)
	{
		FAILED_CHECK_RETURN(m_pNumberingTexCom[ZERO]->Bind_ShaderResource(m_pShaderCom, L"g_Texture"), E_FAIL);
	}
	else if (m_iNumber == 1)
	{
		FAILED_CHECK_RETURN(m_pNumberingTexCom[ONE]->Bind_ShaderResource(m_pShaderCom, L"g_Texture"), E_FAIL);
	}
	else if (m_iNumber == 2)
	{
		FAILED_CHECK_RETURN(m_pNumberingTexCom[TWO]->Bind_ShaderResource(m_pShaderCom, L"g_Texture"), E_FAIL);
	}
	else if (m_iNumber == 3)
	{
		FAILED_CHECK_RETURN(m_pNumberingTexCom[THREE]->Bind_ShaderResource(m_pShaderCom, L"g_Texture"), E_FAIL);
	}
	else if (m_iNumber == 4)
	{
		FAILED_CHECK_RETURN(m_pNumberingTexCom[FOUR]->Bind_ShaderResource(m_pShaderCom, L"g_Texture"), E_FAIL);
	}
	else if (m_iNumber == 5)
	{
		FAILED_CHECK_RETURN(m_pNumberingTexCom[FIVE]->Bind_ShaderResource(m_pShaderCom, L"g_Texture"), E_FAIL);
	}
	else if (m_iNumber == 6)
	{
		FAILED_CHECK_RETURN(m_pNumberingTexCom[SIX]->Bind_ShaderResource(m_pShaderCom, L"g_Texture"), E_FAIL);
	}
	else if (m_iNumber == 7)
	{
		FAILED_CHECK_RETURN(m_pNumberingTexCom[SEVEN]->Bind_ShaderResource(m_pShaderCom, L"g_Texture"), E_FAIL);
	}
	else if (m_iNumber == 8)
	{
		FAILED_CHECK_RETURN(m_pNumberingTexCom[EIGHT]->Bind_ShaderResource(m_pShaderCom, L"g_Texture"), E_FAIL);
	}
	else if (m_iNumber == 9)
	{
		FAILED_CHECK_RETURN(m_pNumberingTexCom[NINE]->Bind_ShaderResource(m_pShaderCom, L"g_Texture"), E_FAIL);
	}
	FAILED_CHECK_RETURN(m_pShaderCom->Set_RawValue(L"g_vPSize", &m_vPSize, sizeof(_float2)), E_FAIL);
	FAILED_CHECK_RETURN(m_pShaderCom->Set_RawValue(L"g_vCamPosition", &m_vCamPos , sizeof(_float4)), E_FAIL);

	return S_OK;
}

CDamage_Font* CDamage_Font::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CDamage_Font*		pInstance = new CDamage_Font(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CDamage_Font");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CDamage_Font::Clone(const wstring& wstrPrototypeTag, void* pArg)
{
	CDamage_Font*		pInstance = new CDamage_Font(*this);

	if (FAILED(pInstance->Initialize_Clone(wstrPrototypeTag, pArg)))
	{
		MSG_BOX("Failed to Cloned : CDamage_Font");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CDamage_Font::Free()
{
	__super::Free();

	if (m_bIsClone)
	{
		Safe_Release(m_pShaderCom);

		for (_int i = 0; i < NUM_END; ++i)
			Safe_Release(m_pNumberingTexCom[i]);

		Safe_Release(m_pPointBuffer);
		Safe_Release(m_pRendererCom);
	}
}
