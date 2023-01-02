#include "stdafx.h"
#include "..\public\NumberUI.h"
#include "GameInstance.h"

CNumberUI::CNumberUI(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CUI(pDevice,pContext)
{
}

CNumberUI::CNumberUI(const CNumberUI& rhs)
	: CUI(rhs)
{
}

HRESULT CNumberUI::Initialize_Prototype()
{
	FAILED_CHECK_RETURN(__super::Initialize_Prototype(), E_FAIL);

	return S_OK;
}

HRESULT CNumberUI::Initialize_Clone(const wstring& wstrPrototypeTag, void* pArg)
{
	m_fSizeX = 36.f;
	m_fSizeY = 36.f;
	m_fX = m_fSizeX * 0.5f;
	m_fY = m_fSizeY * 0.5f;

	FAILED_CHECK_RETURN(CGameObject::Initialize_Clone(wstrPrototypeTag, pArg), E_FAIL);
	FAILED_CHECK_RETURN(SetUp_Components(), E_FAIL);

	m_pTransformCom->Set_Scaled(_float3(m_fSizeX, m_fSizeY, 1.f));

	if(nullptr != pArg)
	{
		CGameObject::GAMEOBJECTDESC		tmp;
		tmp = *(GAMEOBJECTDESC*)pArg;
		m_iNumCnt = tmp.m_iNumCnt;
		m_iNumber = tmp.m_iNumber;

		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(tmp.TransformDesc.vInitPos.x, tmp.TransformDesc.vInitPos.y, 0.f, 1.f));
	}
	else
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(900.f, -315.f, 0.f, 1.f));


	return S_OK;
}

void CNumberUI::Tick(_double dTimeDelta)
{
	__super::Tick(dTimeDelta);
}

void CNumberUI::Late_Tick(_double dTimeDelta)
{
	__super::Late_Tick(dTimeDelta);

	if (nullptr != m_pRendererCom)
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);
}

HRESULT CNumberUI::Render()
{
	FAILED_CHECK_RETURN(__super::Render(), E_FAIL);

	FAILED_CHECK_RETURN(SetUp_ShaderResources(), E_FAIL);

	m_pShaderCom->Begin(4);

	m_pVIBufferCom->Render();

	return S_OK;
}

HRESULT CNumberUI::SetUp_Components()
{
	FAILED_CHECK_RETURN(__super::Add_Component(CGameInstance::Get_StaticLevelIndex(), L"Prototype_Component_Renderer", L"Com_Renderer",(CComponent**)&m_pRendererCom, this), E_FAIL);
	FAILED_CHECK_RETURN(__super::Add_Component(CGameInstance::Get_StaticLevelIndex(), L"Prototype_Component_Shader_VtxTex", L"Com_Shader",	(CComponent**)&m_pShaderCom, this), E_FAIL);
	FAILED_CHECK_RETURN(__super::Add_Component(CGameInstance::Get_StaticLevelIndex(), L"Prototype_Component_VIBuffer_Rect", L"Com_VIBuffer",	(CComponent**)&m_pVIBufferCom, this), E_FAIL);
	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Texture_Snow", L"Com_Texture", (CComponent**)&m_pTextureCom, this), E_FAIL);
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
	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Texture_Numbering_Slash", L"Com_Num-", (CComponent**)&m_pNumberingTexCom[SLASH], this), E_FAIL);

	return S_OK;
}

HRESULT CNumberUI::SetUp_ShaderResources()
{
	NULL_CHECK_RETURN(m_pShaderCom, E_FAIL);

	// m_pShaderCom->Set_Matrix("g_WorldMatrix", &m_WorldMatrix);
	FAILED_CHECK_RETURN(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, L"g_WorldMatrix"), E_FAIL);
	FAILED_CHECK_RETURN(m_pShaderCom->Set_Matrix(L"g_ViewMatrix", &m_ViewMatrix), E_FAIL);
	FAILED_CHECK_RETURN(m_pShaderCom->Set_Matrix(L"g_ProjMatrix", &m_ProjMatrix), E_FAIL);

	if(m_iNumber == 0)
	{
		FAILED_CHECK_RETURN(m_pNumberingTexCom[ZERO]->Bind_ShaderResource(m_pShaderCom, L"g_Texture"), E_FAIL);
	}
	else if(m_iNumber == 1)
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

	return S_OK;
}

CNumberUI* CNumberUI::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CNumberUI*		pInstance = new CNumberUI(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CEffect_Point_Instancing");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CNumberUI::Clone(const wstring& wstrPrototypeTag, void* pArg)
{
	CNumberUI*		pInstance = new CNumberUI(*this);

	if (FAILED(pInstance->Initialize_Clone(wstrPrototypeTag, pArg)))
	{
		MSG_BOX("Failed to Cloned : CEffect_Point_Instancing");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CNumberUI::Free()
{
	__super::Free();
}
