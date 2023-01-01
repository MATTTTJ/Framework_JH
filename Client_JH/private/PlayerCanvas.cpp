#include "stdafx.h"
#include "..\public\PlayerCanvas.h"
#include "GameInstance.h"

CPlayerCanvas::CPlayerCanvas(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CCanvas(pDevice, pContext)
{
}

CPlayerCanvas::CPlayerCanvas(const CPlayerCanvas& rhs)
	:CCanvas(rhs)
{
}

HRESULT CPlayerCanvas::Initialize_Prototype()
{
	FAILED_CHECK_RETURN(__super::Initialize_Prototype(), E_FAIL);

	return E_FAIL;
}

HRESULT CPlayerCanvas::Initialize_Clone(const wstring& wstrPrototypeTag, void* pArg)
{
	CGameObject::GAMEOBJECTDESC		GameObjectDesc;
	ZeroMemory(&GameObjectDesc, sizeof(GameObjectDesc));

	GameObjectDesc.TransformDesc.fRotationPerSec = XMConvertToRadians(90.0f);

	if (FAILED(CGameObject::Initialize_Clone(wstrPrototypeTag, &GameObjectDesc)))
		return E_FAIL;

	if (FAILED(SetUp_Components()))
		return E_FAIL;

	m_fSizeX = (_float)g_iWinSizeX / 4.f;
	m_fSizeY = (_float)g_iWinSizeY / 4.f;

	m_fX = m_fSizeX * 0.5f;
	m_fY = m_fSizeY * 0.5f;

	m_pTransformCom->Set_Scaled(_float3(m_fSizeX, m_fSizeY, 1.f));
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION,
		XMVectorSet(m_fX - (_float)g_iWinSizeX * 0.5f, - m_fY + (_float)g_iWinSizeY * 0.5f, 0.f, 1.f));

	XMStoreFloat4x4(&m_ViewMatrix, XMMatrixIdentity());
	XMStoreFloat4x4(&m_ProjMatrix, XMMatrixOrthographicLH((_float)g_iWinSizeX, (_float)g_iWinSizeY, 0.f, 1.f));

	return S_OK;
}

HRESULT CPlayerCanvas::LateInitialize()
{
	if (m_bLateInit)
		return S_OK;

	CGameInstance* pInstance = GET_INSTANCE(CGameInstance);

	/* 부모에다가 자식 추가하는 건 개인의 자유입니다.*/

	for (auto& pChild : m_vecChildrenUI)
	{
		pChild->Get_Transform()->SetParent(m_pTransformCom);
	}


	RELEASE_INSTANCE(CGameInstance);
	m_bLateInit = true;
	return S_OK;
}

void CPlayerCanvas::Tick(_double TimeDelta)
{
	LateInitialize();

	__super::Tick(TimeDelta);
}

void CPlayerCanvas::Late_Tick(_double TimeDelta)
{
	__super::Late_Tick(TimeDelta);

	if (nullptr != m_pRendererCom)
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_PRIORITY, this);
}

HRESULT CPlayerCanvas::Render()
{
	FAILED_CHECK_RETURN(__super::Render(), E_FAIL);
	FAILED_CHECK_RETURN(SetUp_ShaderResources(), E_FAIL);

	m_pShaderCom->Begin(0);

	m_pVIBufferCom->Render();

	return S_OK;
}



void CPlayerCanvas::Add_Child(CGameObject* pGameObject)
{
	m_vecChildrenUI.push_back((CUI*)pGameObject);
}

HRESULT CPlayerCanvas::SetUp_Components()
{
	FAILED_CHECK_RETURN(__super::Add_Component(CGameInstance::Get_StaticLevelIndex(), L"Prototype_Component_Renderer", L"Com_Renderer", (CComponent**)&m_pRendererCom, this), E_FAIL);
	FAILED_CHECK_RETURN(__super::Add_Component(CGameInstance::Get_StaticLevelIndex(), L"Prototype_Component_Shader_VtxTex", L"Com_Shader",(CComponent**)&m_pShaderCom, this), E_FAIL);
	FAILED_CHECK_RETURN(__super::Add_Component(CGameInstance::Get_StaticLevelIndex(), L"Prototype_Component_VIBuffer_Rect", L"Com_VIBuffer", (CComponent**)&m_pVIBufferCom, this), E_FAIL)
	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Texture_Snow", L"Com_Texture", (CComponent**)&m_pTextureCom, this), E_FAIL);

	return S_OK;
}

HRESULT CPlayerCanvas::SetUp_ShaderResources()
{
	NULL_CHECK_RETURN(m_pShaderCom, E_FAIL);
	FAILED_CHECK_RETURN(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, L"g_WorldMatrix"), E_FAIL);
	FAILED_CHECK_RETURN(m_pShaderCom->Set_Matrix(L"g_ViewMatrix", &m_ViewMatrix), E_FAIL);
	FAILED_CHECK_RETURN(m_pShaderCom->Set_Matrix(L"g_ProjMatrix", &m_ProjMatrix), E_FAIL);
	FAILED_CHECK_RETURN(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, L"g_Texture"), E_FAIL);

	return S_OK;
}

CPlayerCanvas* CPlayerCanvas::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CPlayerCanvas*		pInstance = new CPlayerCanvas(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CTestCanvas");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CPlayerCanvas::Clone(const wstring& wstrPrototypeTag, void* pArg)
{
	CPlayerCanvas*		pInstance = new CPlayerCanvas(*this);

	if (FAILED(pInstance->Initialize_Clone(wstrPrototypeTag, pArg)))
	{
		MSG_BOX("Failed to Cloned : CTestCanvas");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CPlayerCanvas::Free()
{
	CCanvas::Free();
}
