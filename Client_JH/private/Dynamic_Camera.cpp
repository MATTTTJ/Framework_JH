#include "stdafx.h"
#include "..\public\Dynamic_Camera.h"
#include "GameInstance.h"

CDynamic_Camera::CDynamic_Camera(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CCamera(pDevice, pContext)
{
}

CDynamic_Camera::CDynamic_Camera(const CDynamic_Camera & rhs)
	: CCamera(rhs)
{
}

HRESULT CDynamic_Camera::Initialize_Prototype()
{
	FAILED_CHECK_RETURN(__super::Initialize_Prototype(), E_FAIL);

	return S_OK;
}

HRESULT CDynamic_Camera::Initialize_Clone(const wstring& wstrPrototypeTag, void * pArg)
{
	CCamera::CAMERADESC			CameraDesc;
	ZeroMemory(&CameraDesc, sizeof CameraDesc);

	if (nullptr != pArg)
		memcpy(&CameraDesc, pArg, sizeof(CAMERADESC));
	else
	{
		CameraDesc.vEye = _float4(0.f, 10.f, -10.f, 1.f);
		CameraDesc.vAt = _float4(0.f, 0.f, 0.f, 1.f);
		CameraDesc.vUp = _float4(0.f, 1.f, 0.f, 0.f);
		CameraDesc.TransformDesc.fSpeedPerSec = 5.f;
		CameraDesc.TransformDesc.fRotationPerSec = XMConvertToRadians(90.0f);
	}
	FAILED_CHECK_RETURN(__super::Initialize_Clone(wstrPrototypeTag, &CameraDesc), E_FAIL);

	if (FAILED(SetUp_Components()))
		return E_FAIL;

	return S_OK;
}

void CDynamic_Camera::Tick(_double TimeDelta)
{
	if (!m_bRender)
		return;

	if (GetKeyState('W') & 0x8000)
	{
		m_pTransformCom->Go_Straight(TimeDelta);
	}
	if (GetKeyState('S') & 0x8000)
	{
		m_pTransformCom->Go_Backward(TimeDelta);
	}
	if (GetKeyState('A') & 0x8000)
	{
		m_pTransformCom->Go_Left(TimeDelta);
	}
	if (GetKeyState('D') & 0x8000)
	{
		m_pTransformCom->Go_Right(TimeDelta);
	}
	m_pTransformCom->Speed_Up(GetKeyState(VK_LSHIFT) & 0x8000);

	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	if (pGameInstance->Get_DIMouseState(DIM_RB))
	{
		_long			MouseMove = 0;
	
		if (MouseMove = pGameInstance->Get_DIMouseMove(DIMS_X))
		{
			m_pTransformCom->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), TimeDelta * MouseMove * 0.1f);
		}
	
		if (MouseMove = pGameInstance->Get_DIMouseMove(DIMS_Y))
		{
			m_pTransformCom->Turn(m_pTransformCom->Get_State(CTransform::STATE_RIGHT), TimeDelta * MouseMove * 0.1f);
		}
	}

	RELEASE_INSTANCE(CGameInstance);

	__super::Tick(TimeDelta);
}

void CDynamic_Camera::Late_Tick(_double TimeDelta)
{
	if (false == m_bRender)
		return;

	__super::Late_Tick(TimeDelta);
}

HRESULT CDynamic_Camera::Render()
{
	if (false == m_bRender)
		return E_FAIL;
	FAILED_CHECK_RETURN(__super::Render(), E_FAIL);
	
	return S_OK;
}

HRESULT CDynamic_Camera::SetUp_Components()
{
	return S_OK;
}

CDynamic_Camera * CDynamic_Camera::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CDynamic_Camera*		pInstance = new CDynamic_Camera(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CCamera_Dynamic");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CDynamic_Camera::Clone(const wstring& wstrPrototypeTag, void * pArg)
{
	CDynamic_Camera*		pInstance = new CDynamic_Camera(*this);

	if (FAILED(pInstance->Initialize_Clone(wstrPrototypeTag, pArg)))
	{
		MSG_BOX("Failed to Cloned : CCamera_Dynamic");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CDynamic_Camera::Free()
{
	__super::Free();
}
