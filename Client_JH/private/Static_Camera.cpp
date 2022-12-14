#include "stdafx.h"
#include "..\public\Static_Camera.h"
#include "GameInstance.h"
#include "Player.h"


CStatic_Camera::CStatic_Camera(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CCamera(pDevice, pContext)
{
}

CStatic_Camera::CStatic_Camera(const CStatic_Camera& rhs)
	:CCamera(rhs)
{
}

HRESULT CStatic_Camera::Initialize_Prototype()
{
	FAILED_CHECK_RETURN(__super::Initialize_Prototype(), E_FAIL);

	return S_OK;
}

HRESULT CStatic_Camera::Initialize_Clone(const wstring& wstrPrototypeTag, void* pArg)
{
	m_bRender = true;

	CCamera::CAMERADESC		CameraDesc;
	ZeroMemory(&CameraDesc, sizeof(CCamera::CAMERADESC));

	if (pArg != nullptr)
		memcpy(&CameraDesc,pArg, sizeof(CCamera::CAMERADESC));
	else
	{
		CameraDesc.vEye = _float4(0.f, 20.f, -30.f, 1.f);
		CameraDesc.vAt = _float4(0.f, 0.f, 0.f, 1.f);
		CameraDesc.vUp = _float4(0.f, 1.f, 0.f, 0.f);
		CameraDesc.TransformDesc.fSpeedPerSec = 15.0;
		CameraDesc.TransformDesc.fRotationPerSec= XMConvertToRadians(90.f);
	}

	FAILED_CHECK_RETURN(__super::Initialize_Clone(wstrPrototypeTag, &CameraDesc), E_FAIL);

	FAILED_CHECK_RETURN(SetUp_Component(), E_FAIL);

	return S_OK;
}

void CStatic_Camera::Tick(_double dTimeDelta)
{
	if (!m_bRender)
		return;

	__super::Tick(dTimeDelta);

	if (GetAsyncKeyState('Y'))
	{
		m_bFix = !m_bFix;
	}

	if (m_vCameraLook.w == -1)
	{
		XMStoreFloat4(&m_vCameraLook, dynamic_cast<CTransform*>(CGameInstance::GetInstance()->Get_ComponentPtr(LEVEL_GAMEPLAY, L"Layer_Player", L"Com_Transform"))->Get_State(CTransform::STATE_LOOK));
	}

	_int TurnX = 0, TurnY = 0;

	if(TurnX = CGameInstance::GetInstance()->Get_DIMouseMove(DIMS_X))
	{
		_matrix matRotation = XMMatrixRotationAxis(XMVectorSet(0.f, 1.f, 0.f, 0.f),
			XMConvertToRadians(90.f) *  0.1f * (_float)dTimeDelta * TurnX);
		XMStoreFloat4(&m_vCameraLook, XMVector4Transform(XMLoadFloat4(&m_vCameraLook), matRotation));
		XMStoreFloat4(&m_vCameraLook, XMVector3Normalize(XMLoadFloat4(&m_vCameraLook)));
	}
	if (TurnY = CGameInstance::GetInstance()->Get_DIMouseMove(DIMS_Y))
	{
		m_fCamHeight += (_float)TurnY / 100.f;

		if (m_fCamHeight >= 1.f)
			m_fCamHeight = 1.f;
		else if (m_fCamHeight <= -1.f)
			m_fCamHeight = -1.f;
	}
}

void CStatic_Camera::Late_Tick(_double dTimeDelta)
{
	if (!m_bRender)
		return;
	__super::Late_Tick(dTimeDelta);

	if(false != m_bFix)
	{
		Mouse_Fix();
	}

}

HRESULT CStatic_Camera::Render()
{
	if (!m_bRender)
		return E_FAIL;

	FAILED_CHECK_RETURN(__super::Render(), E_FAIL);

	return S_OK;
}

void CStatic_Camera::Camera_Update(_fvector PlayerPos, _fvector PlayerLook, _double dTimeDelta)
{
	if (!m_bRender)
		return;

	

	_vector vPlayerPos = PlayerPos;
	_vector vLook = XMLoadFloat4(&m_vCameraLook);

	
	_uint iCurrentLevel = CGameInstance::GetInstance()->Get_CurLevelIndex();

	if (m_pOwner == nullptr)
		m_pOwner = CGameInstance::GetInstance()->Get_CloneObjectList(iCurrentLevel, L"Layer_Player")->front();
	else
	{
		if (CGameInstance::GetInstance()->Get_CloneObjectList(iCurrentLevel, L"Layer_Player")->empty())
		{
			m_pOwner = nullptr;
			return;
		}

		vLook = XMVector3Normalize(vLook) * -5.f;

		// ���� �� ���� ��� ī�޶� ��ġ ���� ������
		_vector vCamPos = vPlayerPos + (vLook + XMVectorSet(0.f, 4.f + m_fCamHeight, 0.f, 0.f));

		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, vCamPos);

		m_pTransformCom->LookAt(vPlayerPos + XMVectorSet(0.f, 2.5f, 0.f, 0.f));

	}
}

void CStatic_Camera::Mouse_Fix()
{
	POINT	pt{ g_iWinSizeX >> 1 , g_iWinSizeY >> 1 };

	ClientToScreen(g_hWnd, &pt);
	SetCursorPos(pt.x, pt.y);
}

HRESULT CStatic_Camera::SetUp_Component()
{
	return S_OK;
}

CStatic_Camera* CStatic_Camera::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CStatic_Camera*	pInstance = new CStatic_Camera(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Create : CStaticCamera");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CStatic_Camera::Clone(const wstring& wstrPrototypeTag, void* pArg)
{
	CStatic_Camera*	pInstance = new CStatic_Camera(*this);

	if (FAILED(pInstance->Initialize_Clone(wstrPrototypeTag, pArg)))
	{
		MSG_BOX("Failed to Clone : CStaticCamera");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CStatic_Camera::Free()
{
	__super::Free();
}