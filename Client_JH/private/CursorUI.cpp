#include "stdafx.h"
#include "..\public\CursorUI.h"
#include "GameInstance.h"
#include "Player.h"

CCursorUI::CCursorUI(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUI(pDevice, pContext)
{
}

CCursorUI::CCursorUI(const CCursorUI& rhs)
	: CUI(rhs)
{
}

HRESULT CCursorUI::Initialize_Prototype()
{
	FAILED_CHECK_RETURN(__super::Initialize_Prototype(), E_FAIL);

	return S_OK;
}

HRESULT CCursorUI::Initialize_Clone(const wstring& wstrPrototypeTag, void* pArg)
{
	m_fSizeX = 60.5f;
	m_fSizeY = 60.5f;
	m_fX = m_fSizeX * 0.5f;
	m_fY = m_fSizeY * 0.5f;

	FAILED_CHECK_RETURN(__super::Initialize_Clone(wstrPrototypeTag, pArg), E_FAIL);

	FAILED_CHECK_RETURN(SetUp_Component(), E_FAIL);

	m_pTransformCom->Set_Scaled(_float3(m_fSizeX, m_fSizeY, 0.f));
	ShowCursor(false);
	return S_OK;
}

void CCursorUI::Tick(_double dTimeDelta)
{
	__super::Tick(dTimeDelta);

	POINT tPoint = Get_CursorPos();
	_float2 vScale = _float2(m_fX * 0.5f, m_fY	* 0.5f);
	_float fX = tPoint.x + vScale.x * 0.5f;
	_float fY = tPoint.y + vScale.y	* 0.5f;

	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(fX - (g_iWinSizeX * 0.5f), -fY + (g_iWinSizeY * 0.5f), 0.f, 1.f));

}

void CCursorUI::Late_Tick(_double dTimeDelta)
{
	__super::Late_Tick(dTimeDelta);

	if (nullptr != m_pRendererCom)
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);
}

HRESULT CCursorUI::Render()
{
	FAILED_CHECK_RETURN(__super::Render(), E_FAIL);

	FAILED_CHECK_RETURN(SetUp_ShaderResources(), E_FAIL);

	m_pShaderCom->Begin(1);

	m_pVIBufferCom->Render();

	return S_OK;
}

HRESULT CCursorUI::SetUp_Component()
{
	FAILED_CHECK_RETURN(__super::Add_Component(CGameInstance::Get_StaticLevelIndex(), L"Prototype_Component_Renderer", L"Com_Renderer", (CComponent**)&m_pRendererCom, this), E_FAIL);
	FAILED_CHECK_RETURN(__super::Add_Component(CGameInstance::Get_StaticLevelIndex(), L"Prototype_Component_Shader_VtxTex", L"Com_Shader", (CComponent**)&m_pShaderCom, this), E_FAIL);
	FAILED_CHECK_RETURN(__super::Add_Component(CGameInstance::Get_StaticLevelIndex(), L"Prototype_Component_VIBuffer_Rect", L"Com_VIBuffer", (CComponent**)&m_pVIBufferCom, this), E_FAIL);
	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Battle_Cursor", L"Com_Texture", (CComponent**)&m_pTextureCom, this), E_FAIL);

	return S_OK;
}

HRESULT CCursorUI::SetUp_ShaderResources()
{
	NULL_CHECK_RETURN(m_pShaderCom, E_FAIL);

	m_pTransformCom->Bind_ShaderResource(m_pShaderCom, L"g_WorldMatrix");
	m_pShaderCom->Set_Matrix(L"g_ViewMatrix", &m_ViewMatrix);
	m_pShaderCom->Set_Matrix(L"g_ProjMatrix", &m_ProjMatrix);
	m_pTextureCom->Bind_ShaderResource(m_pShaderCom, L"g_Texture");

	return S_OK;
}

CCursorUI* CCursorUI::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CCursorUI*	pInstance = new CCursorUI(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Create : CCursorUI");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CCursorUI::Clone(const wstring& wstrPrototypeTag, void* pArg)
{
	CCursorUI*	pInstance = new CCursorUI(*this);

	if (FAILED(pInstance->Initialize_Clone(wstrPrototypeTag, pArg)))
	{
		MSG_BOX("Failed to Create : CCursorUI");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCursorUI::Free()
{
	__super::Free();

}
