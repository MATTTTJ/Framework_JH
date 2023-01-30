#include "stdafx.h"
#include "..\public\Effect.h"
#include "GameInstance.h"
#include "Imgui_LevelSwitcher.h"

CEffect::CEffect(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CGameObject(pDevice, pContext)
{
}

CEffect::CEffect(const CEffect& rhs)
	:CGameObject(rhs)
{
}

HRESULT CEffect::Initialize_Prototype()
{
	// FAILED_CHECK_RETURN(__super::Initialize_Prototype(), E_FAIL);

	return S_OK;
}

HRESULT CEffect::Initialize_Clone(const wstring& wstrPrototypeTag, void* pArg)
{
	memcpy(&m_tEffect, pArg, sizeof(EFFECTINFO));

	CTransform::TRANSFORMDESC		TransformDesc;
	TransformDesc.fSpeedPerSec = 5.f;
	TransformDesc.fRotationPerSec = XMConvertToRadians(90.0f);

	FAILED_CHECK_RETURN(__super::Initialize_Clone(wstrPrototypeTag, &TransformDesc), E_FAIL);

	FAILED_CHECK_RETURN(SetUp_Components(), E_FAIL);


	SetDisabled(true);

	XMStoreFloat4x4(&m_LocalMatrix, XMMatrixScaling(m_tEffect.vScale.x, m_tEffect.vScale.y, m_tEffect.vScale.z)
		* XMMatrixRotationRollPitchYaw(XMConvertToRadians(m_tEffect.vRotation.x), XMConvertToRadians(m_tEffect.vRotation.y), XMConvertToRadians(m_tEffect.vRotation.z))
		* XMMatrixTranslation(m_tEffect.vOffsetPos.x, m_tEffect.vOffsetPos.y, m_tEffect.vOffsetPos.z));

	return S_OK;
}

void CEffect::Tick(_double dTimeDelta)
{
	_float fTimeDelta = (_float)dTimeDelta;
	
	m_fTimeAcc += fTimeDelta;
	
	if (m_tEffect.fAppearTime <= m_fTimeAcc)
	{
		SetActive(true);
	}
	
	if (m_tEffect.fDelTime <= m_fTimeAcc)
	{
		SetDisabled(true);
		m_fTimeAcc = 0.f;
	}
	
	if (m_tEffect.fStartChangeTime <= m_fTimeAcc && m_tEffect.fEndChangeTime >= m_fTimeAcc)
	{
		m_fRatio += m_tEffect.fChangeRatio * fTimeDelta;
		if (m_fRatio >= 1.f)
		{
			m_fRatio = 1.f;
		}
	}
	
	//페이드 인
	if (m_tEffect.fFadeInStartTime <= m_fTimeAcc && m_tEffect.fFadeOutStartTime >= m_fTimeAcc)
	{
		m_fAlphaRatio += m_tEffect.fFadeInRatio * fTimeDelta;
		if (m_fAlphaRatio >= 1.f)
		{
			m_fAlphaRatio = 1.f;
		}
	}
	//페이드 아웃
	if (m_tEffect.fFadeOutStartTime <= m_fTimeAcc)
	{
		m_fAlphaRatio -= m_tEffect.fFadeOutRatio * fTimeDelta;
		if (m_fAlphaRatio <= 0.f)
		{
			m_fAlphaRatio = 0.f;
		}
	}
	
	if (m_tEffect.bIsSprite)
	{
		if (m_fTimeAcc >= m_tEffect.fUVchangeStartTime)
		{
			_uint iNum = (_uint)((m_fTimeAcc - m_tEffect.fUVchangeStartTime) / m_tEffect.fUVchangeTime);
	
			m_tEffect.vUVScale.x = 1.f / m_tEffect.vMaxUV.x;
			m_tEffect.vUVScale.y = 1.f / m_tEffect.vMaxUV.y;
	
			m_vSpriteUVPos.x = (iNum % (_uint)m_tEffect.vMaxUV.x) * m_tEffect.vUVScale.x;
			m_vSpriteUVPos.y = (iNum / (_uint)m_tEffect.vMaxUV.x) * m_tEffect.vUVScale.y;
		}
	}
	
	_matrix RotationMatrix = XMMatrixIdentity();
	if (m_tEffect.bIsRotation)
	{
		RotationMatrix = XMMatrixRotationX(XMConvertToRadians(m_tEffect.vRotationToTime.x * m_fTimeAcc))
			* XMMatrixRotationY(XMConvertToRadians(m_tEffect.vRotationToTime.y * m_fTimeAcc))
			* XMMatrixRotationZ(XMConvertToRadians(m_tEffect.vRotationToTime.z * m_fTimeAcc));
	}
	
	if (m_tEffect.bIsParentRotation)
	{
		m_pTransformCom->Set_State(CTransform::STATE_RIGHT, m_pOwnerTransform->Get_State(CTransform::STATE_RIGHT));
		m_pTransformCom->Set_State(CTransform::STATE_UP, m_pOwnerTransform->Get_State(CTransform::STATE_UP));
		m_pTransformCom->Set_State(CTransform::STATE_LOOK, m_pOwnerTransform->Get_State(CTransform::STATE_LOOK));
	}
	
	if (m_tEffect.eType == EFFECT_PARTICLE_POINT || m_tEffect.eType == EFFECT_PARTICLE_RECT || m_tEffect.eType == EFFECT_PARTICLE_MESH)
	{
	
	}
	else
	{
		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, m_pOwnerTransform->Get_State(CTransform::STATE_TRANSLATION));
	}
	
	if (m_tEffect.bIsBillBoard)
	{
		m_pTransformCom->Billboard();
	}
	
	XMStoreFloat4x4(&m_WorldMatrix, XMLoadFloat4x4(&m_LocalMatrix) * RotationMatrix * m_pTransformCom->Get_WorldMatrix());
	
	XMStoreFloat4x4(&m_WorldMatrixTP, XMMatrixTranspose(XMLoadFloat4x4(&m_WorldMatrix)));
}

void CEffect::Late_Tick(_double dTimeDelta)
{
	Compute_CamDistance();

	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_EFFECT, this);
}

HRESULT CEffect::Render()
{
	return S_OK;
}

CEffect* CEffect::Start(CTransform* vTransform)
{
	SetDisabled(false);
	SetActive(false);
	m_pOwnerTransform = vTransform;

	m_pTransformCom->Set_State(CTransform::STATE_RIGHT, m_pOwnerTransform->Get_State(CTransform::STATE_RIGHT));
	m_pTransformCom->Set_State(CTransform::STATE_UP, m_pOwnerTransform->Get_State(CTransform::STATE_UP));
	m_pTransformCom->Set_State(CTransform::STATE_LOOK, m_pOwnerTransform->Get_State(CTransform::STATE_LOOK));
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, m_pOwnerTransform->Get_State(CTransform::STATE_TRANSLATION));

	m_fRatio = 0.f;
	m_fAlphaRatio = 0.f;

	return this;
}

void CEffect::End()
{
	SetDisabled(true);
	m_fTimeAcc = 0.f;
	m_pOwnerTransform = nullptr;
	m_fRatio = 0.f;
	m_fAlphaRatio = 0.f;
}

HRESULT CEffect::SetUp_Components()
{
	FAILED_CHECK_RETURN(__super::Add_Component(CGameInstance::Get_StaticLevelIndex(), L"Prototype_Component_Renderer", L"Com_Renderer", (CComponent**)&m_pRendererCom, this), E_FAIL);

	return S_OK;
}

void CEffect::Free()
{
	__super::Free();

	Safe_Release(m_pRendererCom);
	m_pOwnerTransform = nullptr;
}
