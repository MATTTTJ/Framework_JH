#include "stdafx.h"
#include "..\public\Button_Play.h"
#include "GameInstance.h"

CButton_Play::CButton_Play(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CGameObject(pDevice,pContext)
{
}

CButton_Play::CButton_Play(const CButton_Play& rhs)
	: CGameObject(rhs)
{
}

HRESULT CButton_Play::Initialize_Prototype()
{
	FAILED_CHECK_RETURN(__super::Initialize_Prototype(), E_FAIL);

	return S_OK;
}

HRESULT CButton_Play::Initialize_Clone(const wstring& wstrPrototypeTag, void* pArg)
{
	FAILED_CHECK_RETURN(__super::Initialize_Clone(wstrPrototypeTag, pArg), E_FAIL);
	FAILED_CHECK_RETURN(SetUp_Components(), E_FAIL);


	if(nullptr != pArg)
	{
		CGameObject::GAMEOBJECTDESC		tmp;
		tmp = *(GAMEOBJECTDESC*)pArg;
		m_iType = tmp.m_iCountType;
		
		m_vInitPos = _float4(tmp.TransformDesc.vInitPos.x, tmp.TransformDesc.vInitPos.y, 0.f, 1.f);
		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, m_vInitPos);
	}
	else
	{
		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(900.f, -315.f, 0.f, 1.f));
		m_pTransformCom->Set_Scaled(_float3(m_fSizeX, m_fSizeY, 1.f));
	}

	if (m_iType == 0)
	{
		m_fSizeX = 566.f;
		m_fSizeY = 116.f;
		m_pTransformCom->Rotation(XMVectorSet(0.f, 0.f, 1.f, 0.f), XMConvertToRadians(-10.f));
	}
	else if (m_iType == 1)
	{
		m_fSizeX = 360.f;
		m_fSizeY = 116.f;
		// m_pTransformCom->Rotation(XMVectorSet(0.f, 0.f, 1.f, 0.f), XMConvertToRadians(-10.f));

	}
	else if (m_iType == 2)
	{
		m_fSizeX = 222.f;
		m_fSizeY = 76.f;
		m_pTransformCom->Rotation(XMVectorSet(0.f, 0.f, 1.f, 0.f), XMConvertToRadians(10.f));

	}

	m_pTransformCom->Set_Scaled(_float3(m_fSizeX, m_fSizeY, 1.f));

	m_fX = m_fSizeX * 0.5f;
	m_fY = m_fSizeY * 0.5f;
	XMStoreFloat4x4(&m_ViewMatrix, XMMatrixIdentity());

	return S_OK;
}

void CButton_Play::Tick(_double dTimeDelta)
{
	__super::Tick(dTimeDelta);
	XMStoreFloat4x4(&m_ProjMatrix, XMMatrixOrthographicLH((_float)g_iWinSizeX, (_float)g_iWinSizeY, 0.f, 1.f));

	if (CGameUtils::Rect_Picking(g_hWnd, m_Rect))
	{
		m_bMouseHover = true;
	}
	else
		m_bMouseHover = false;

	m_bClicked = false;

	if (CGameInstance::GetInstance()->Key_Pressing(DIK_1) || m_bMouseHover == true)
	{
		// 클릭 할 때 디스에이블 추가하기

		if (m_bMouseHover && CGameInstance::GetInstance()->Mouse_Down(DIM_LB))
		{
			m_fHoverScale -= 0.15f;
			m_bClicked = true;
		}

		if (m_fHoverScale < 1.3f)
			m_fHoverScale += (_float)dTimeDelta * 4.f;
		else
			m_fHoverScale = 1.3f;
	}
	else
	{
		if (m_fHoverScale > 1.f)
			m_fHoverScale -= (_float)dTimeDelta * 4.f;
		else
			m_fHoverScale = 1.f;
	}

	m_pTransformCom->Set_Scaled(_float3(m_fSizeX * m_fHoverScale, m_fSizeY * m_fHoverScale, 1.f));

}

void CButton_Play::Late_Tick(_double dTimeDelta)
{
	__super::Late_Tick(dTimeDelta);

	// _float4 vPos = m_pTransformCom->Get_State(CTransform::STATE_TRANSLATION);

	m_Rect = { _long((g_iWinSizeX *0.5f) + m_vInitPos.x - m_fX), _long((g_iWinSizeY *0.5f) - m_vInitPos.y - m_fY),
		_long((g_iWinSizeX *0.5f) + m_vInitPos.x + m_fX), _long((g_iWinSizeY *0.5f) - m_vInitPos.y + m_fY) };

	if (nullptr != m_pRendererCom)
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);
}

HRESULT CButton_Play::Render()
{
	FAILED_CHECK_RETURN(__super::Render(), E_FAIL);

	FAILED_CHECK_RETURN(SetUp_ShaderResources(), E_FAIL);
	
	m_pShaderCom->Begin(1);
	
	m_pVIBufferCom->Render();

	return S_OK;
}

HRESULT CButton_Play::SetUp_Components()
{
	FAILED_CHECK_RETURN(__super::Add_Component(CGameInstance::Get_StaticLevelIndex(), L"Prototype_Component_Renderer", L"Com_Renderer",(CComponent**)&m_pRendererCom, this), E_FAIL);
	FAILED_CHECK_RETURN(__super::Add_Component(CGameInstance::Get_StaticLevelIndex(), L"Prototype_Component_Shader_VtxTex", L"Com_Shader",	(CComponent**)&m_pShaderCom, this), E_FAIL);
	FAILED_CHECK_RETURN(__super::Add_Component(CGameInstance::Get_StaticLevelIndex(), L"Prototype_Component_VIBuffer_Rect", L"Com_VIBuffer",	(CComponent**)&m_pVIBufferCom, this), E_FAIL);
	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_ButtonTex", L"Com_Texture", (CComponent**)&m_pTextureCom, this), E_FAIL);

	return S_OK;
}

HRESULT CButton_Play::SetUp_ShaderResources()
{
	NULL_CHECK_RETURN(m_pShaderCom, E_FAIL);

	// m_pShaderCom->Set_Matrix("g_WorldMatrix", &m_WorldMatrix);
	FAILED_CHECK_RETURN(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, L"g_WorldMatrix"), E_FAIL);
	FAILED_CHECK_RETURN(m_pShaderCom->Set_Matrix(L"g_ViewMatrix", &m_ViewMatrix), E_FAIL);
	FAILED_CHECK_RETURN(m_pShaderCom->Set_Matrix(L"g_ProjMatrix", &m_ProjMatrix), E_FAIL);
	FAILED_CHECK_RETURN(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, L"g_Texture", m_iType), E_FAIL);

	return S_OK;
}

CButton_Play* CButton_Play::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CButton_Play*		pInstance = new CButton_Play(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : Button");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CButton_Play::Clone(const wstring& wstrPrototypeTag, void* pArg)
{
	CButton_Play*		pInstance = new CButton_Play(*this);

	if (FAILED(pInstance->Initialize_Clone(wstrPrototypeTag, pArg)))
	{
		MSG_BOX("Failed to Cloned : Button");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CButton_Play::Free()
{
	__super::Free();

	if (m_bIsClone == true)
	{
		Safe_Release(m_pTextureCom);
	// if(false == m_bIsClone)
	// 	Safe_Delete_Array(m_pNumberingTexCom);

		Safe_Release(m_pVIBufferCom);
		Safe_Release(m_pShaderCom);
		Safe_Release(m_pRendererCom);
	}
}
