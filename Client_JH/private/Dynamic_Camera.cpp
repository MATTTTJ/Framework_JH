#include "stdafx.h"
#include "Dynamic_Camera.h"
#include "GameInstance.h"


CDynamic_Camera::CDynamic_Camera(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CCamera(pDevice,pContext)
{
}

CDynamic_Camera::CDynamic_Camera(const CDynamic_Camera& rhs)
	:CCamera(rhs)
{
}

HRESULT CDynamic_Camera::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CDynamic_Camera::Initialize_Clone(void* pArg)
{
	CCamera::CAMERADESC			CameraDesc;
	ZeroMemory(&CameraDesc, sizeof CameraDesc);

	if (nullptr != pArg)
		memcpy(&CameraDesc, pArg, sizeof(CAMERADESC));
	else
	{
		CameraDesc.vEye = _float4(0.f, 0.f, -10.f, 1.f);
		CameraDesc.vAt = _float4(0.f, 0.f, 0.f, 1.f);
		CameraDesc.vUp = _float4(0.f, 1.f, 0.f, 0.f);
		CameraDesc.TransformDesc.fSpeedPerSec = 5.f;
		CameraDesc.TransformDesc.fRotationPerSec = XMConvertToRadians(90.0f);
	}

	if (FAILED(__super::Initialize_Clone(&CameraDesc)))
		return E_FAIL;

	if (FAILED(SetUp_Components()))
		return E_FAIL;

	return S_OK;
}

void CDynamic_Camera::Tick(_double TimeDelta)
{
	if(GetKeyState(VK_UP) & 0x8000)
	{
		m_pTransformCom->Go_Straight(TimeDelta);
	}

	__super::Tick(TimeDelta);
}

void CDynamic_Camera::Late_Tick(_double TimeDelta)
{
	__super::Late_Tick(TimeDelta);
}

HRESULT CDynamic_Camera::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CDynamic_Camera::SetUp_Components()
{
	return S_OK;
}

CDynamic_Camera* CDynamic_Camera::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CDynamic_Camera*		pInstance = new CDynamic_Camera(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CDynamic_Camera");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CDynamic_Camera::Clone(void * pArg)
{
	CDynamic_Camera*		pInstance = new CDynamic_Camera(*this);

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSG_BOX("Failed to Cloned : CDynamic_Camera");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CDynamic_Camera::Free()
{
	CGameObject::Free();
}
