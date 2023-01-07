#include "stdafx.h"
#include "..\public\Static_Camera.h"
#include "GameInstance.h"
#include "Player.h"
#include "Collider.h"

CStatic_Camera::CStatic_Camera(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CCamera(pDevice, pContext)
{
}

CStatic_Camera::CStatic_Camera(const CStatic_Camera& rhs)
	: CCamera(rhs)
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
		memcpy(&CameraDesc, pArg, sizeof(CCamera::CAMERADESC));
	else
	{
		CameraDesc.vEye = _float4(0.f, 20.f, -30.f, 1.f);
		CameraDesc.vAt = _float4(0.f, 0.f, 0.f, 1.f);
		CameraDesc.vUp = _float4(0.f, 1.f, 0.f, 0.f);
		CameraDesc.TransformDesc.fSpeedPerSec = 7.f;
		CameraDesc.TransformDesc.fRotationPerSec = XMConvertToRadians(90.f);
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

	if (m_bFix == true)
	{
		Mouse_Fix();
	}

	_uint iCurrentLevel = CGameInstance::GetInstance()->Get_CurLevelIndex();


	if (m_pOwner == nullptr)
		m_pOwner = CGameInstance::GetInstance()->Get_CloneObjectList(iCurrentLevel, L"Layer_Player")->front();
	else if (CGameInstance::GetInstance()->Get_CloneObjectList(iCurrentLevel, L"Layer_Player")->empty())
	{
		m_pOwner = nullptr;
		return;
	}

	if (m_vCameraLook.w == -1)
	{
		XMStoreFloat4(&m_vCameraLook, dynamic_cast<CTransform*>(CGameInstance::GetInstance()->Get_ComponentPtr(LEVEL_GAMEPLAY, L"Layer_Player", L"Com_Transform"))->Get_State(CTransform::STATE_LOOK));
	}

	

	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, dynamic_cast<CPlayer*>(m_pOwner)->Get_TransformState(CTransform::STATE_TRANSLATION));

	m_pTransformCom->LookAt(m_pTransformCom->Get_State(CTransform::STATE_TRANSLATION) + dynamic_cast<CPlayer*>(m_pOwner)->Get_TransformState(CTransform::STATE_LOOK));

	m_pColliderCom->Update(m_pTransformCom->Get_WorldMatrix());
}

void CStatic_Camera::Late_Tick(_double dTimeDelta)
{
	if (!m_bRender)
		return;

	__super::Late_Tick(dTimeDelta);
}

HRESULT CStatic_Camera::Render()
{
	if (!m_bRender)
		return E_FAIL;

	FAILED_CHECK_RETURN(__super::Render(), E_FAIL);

	m_pColliderCom->Render();

	return S_OK;
}

void CStatic_Camera::Camera_Update(_double dTimeDelta, _float4x4 PlayerWorld)
{
	if (!m_bRender)
		return;


	_uint iCurrentLevel = CGameInstance::GetInstance()->Get_CurLevelIndex();


	if (m_pOwner == nullptr)
		m_pOwner = CGameInstance::GetInstance()->Get_CloneObjectList(iCurrentLevel, L"Layer_Player")->front();
	else if (CGameInstance::GetInstance()->Get_CloneObjectList(iCurrentLevel, L"Layer_Player")->empty())
	{
		m_pOwner = nullptr;
		return;
	}

	_float4x4	OwnerWorldMatrix = PlayerWorld;
	_float4x4	TargetBoneMatrix = dynamic_cast<CPlayer*>(m_pOwner)->Get_HeadModelCom();
	_matrix		PivotMatrix = CGameUtils::Get_PlayerPivotMatrix();
	
	_float4x4	CombindMatrix;
	XMStoreFloat4x4(&CombindMatrix, TargetBoneMatrix *PivotMatrix* OwnerWorldMatrix);
	
	_vector vCamPos = XMVectorSet(CombindMatrix._41, CombindMatrix._42, CombindMatrix._43, CombindMatrix._44) + (_vector(PlayerWorld.Backward() * 0.1f) - (_vector(PlayerWorld.Right() * 0.05f) - (_vector(PlayerWorld.Up() * 0.3f))));

	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, vCamPos);
	m_pTransformCom->LookAt(vCamPos + _vector(PlayerWorld.Backward()));
}


void CStatic_Camera::Mouse_Fix()
{
	POINT	pt{ (LONG)g_iWinSizeX >> 1 , (LONG)g_iWinSizeY >> 1 };

	ClientToScreen(g_hWnd, &pt);
	SetCursorPos(pt.x, pt.y);
}

void CStatic_Camera::Set_CamPos(_float4 vPos, _float4 vLook)
{
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, vPos);
	m_pTransformCom->LookAt(m_pTransformCom->Get_State(CTransform::STATE_TRANSLATION) + XMLoadFloat4(&vLook));

}

HRESULT CStatic_Camera::SetUp_Component()
{

	CCollider::COLLIDERDESC			ColliderDesc;
	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));
	ColliderDesc.vSize = _float3(2.f, 2.f, 2.f);
	ColliderDesc.vPosition = _float3(0.f, 0.f, 0.f);
	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Collider_SPHERE", L"Com_SPHERE", (CComponent**)&m_pColliderCom, this, &ColliderDesc), E_FAIL);
	
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
