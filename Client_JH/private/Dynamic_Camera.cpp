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

void CDynamic_Camera::Set_Boss_IntroCam(_matrix camWorld)
{
	m_pTransformCom->Set_WorldMatrix(camWorld);
}

HRESULT CDynamic_Camera::Initialize_Prototype()
{
	FAILED_CHECK_RETURN(__super::Initialize_Prototype(), E_FAIL);

	return S_OK;
}

HRESULT CDynamic_Camera::Initialize_Clone(const wstring& wstrPrototypeTag, void * pArg)
{
	m_bRender = false;

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

	for(_uint i=0; i< 5; ++i)
	{
		CAMFRAME KeyFrame;
		ZeroMemory(&KeyFrame, sizeof(CAMFRAME));

		KeyFrame.dTime = 0.6f * (i + 1);

		m_vecCamKeyFrame.push_back(KeyFrame);
	}

	m_vFinalScale = _float3(1.f, 1.f, 1.f);
	m_vFinalPos = _float3(-20.526f, 5.805f, -30.465f);
	m_vFinalRotation = _float4(21.148f, -41.228f, 0.f, 0.f);
	
	m_vInitScale = _float3(1.f, 1.f, 1.f);
	m_vInitPos = _float3(-18.092f, 5.205f, -33.586f);
	m_vInitRotation = _float4(34.817f, -38.979f, 0.f,0.f);



	return S_OK;
}

void CDynamic_Camera::Tick(_double TimeDelta)
{
	if (!m_bRender)
		return;
	_vector vScale;
	_vector vRotation;
	_vector vPosition;
	_matrix TransformMatrix;



	if(m_bLobby == true)
	{
		if (m_bArrivedCam == false)
		{
			m_fCurPlayTime += (_float)TimeDelta;
		}

		if (m_fCurPlayTime >= 3.f)
		{
			vScale = m_vFinalScale;
			vRotation = m_vFinalRotation;
			vPosition = m_vFinalPos;
			vPosition = XMVectorSetW(vPosition, 1.f);
			m_bArrivedCam = true;
		}
		else
		{
			// while (m_fCurPlayTime >= m_vecKeyframes[m_iCurrentKeyframeIndex + 1].dTime)
			// {
			// 	// 애니메이션 재생시간이 키프레임 사이에 존재할 때
			// 	// 플레이타임 시간이 다음 키프레임 시간보다 클 때 현재 키프레임 인덱스를 증가
			// 	++m_iCurrentKeyframeIndex;
			// }
			// 현재 시간 - 현재 키프레임 인덱스 시간 / 현재 키프레임 인덱스 시간 - 현재 시간 
			_double	dRatio = (m_fCurPlayTime - 1.5f) / (3.f - 1.5f);
			// 위 비율은 키 프레임 사이의 데이터 변경 값을 보간해주기 위한 비율이다.

			vScale = _float3(1.f,1.f,1.f);
			vRotation = XMQuaternionSlerp(XMLoadFloat4(&m_vInitRotation), XMLoadFloat4(&m_vFinalRotation), (_float)dRatio);
			vPosition = XMVectorLerp(XMLoadFloat3(&m_vInitPos), XMLoadFloat3(&m_vFinalPos), (_float)dRatio);
			vPosition = XMVectorSetW(vPosition, 1.f);
		}
		// 백터들을 받아서 행렬로 만들어주는 함수. 2번째 인자는 기준점인데 원점이 들어간다.
		TransformMatrix = XMMatrixAffineTransformation(vScale, XMVectorSet(0.f, 0.f, 0.f, 1.f), vRotation, vPosition);
		_matrix PivotMatrix = XMMatrixIdentity();
		_float3 P, S, R;
		_float	vPos[3], vscale[3], vAngle[3];
		P = vPosition; S = vScale; R = vRotation;

		vPos[0] = P.x;	vPos[1] = P.y; vPos[2] = P.z;
		vscale[0] = S.x;	vscale[1] = S.y;	vscale[2] = S.z;
		vAngle[0] = R.x;	vAngle[1] = R.y;	vAngle[2] = R.z;
		ImGuizmo::RecomposeMatrixFromComponents(vPos, vAngle, vscale, (_float*)&PivotMatrix);

		m_pTransformCom->Set_WorldMatrix(PivotMatrix);
	}
	else
		m_pTransformCom->Go_Straight(TimeDelta * 0.15f, CTransform::TRANS_BULLET);


	if (GetKeyState(VK_UP) & 0x8000)
	{
		m_pTransformCom->Go_Straight(TimeDelta, CTransform::TRANS_BULLET);
	}
	if (GetKeyState(VK_DOWN) & 0x8000)
	{
		m_pTransformCom->Go_Backward(TimeDelta);
	}
	if (GetKeyState(VK_LEFT) & 0x8000)
	{
		m_pTransformCom->Go_Left(TimeDelta);
	}
	if (GetKeyState(VK_RIGHT) & 0x8000)
	{
		m_pTransformCom->Go_Right(TimeDelta);
	}
	m_pTransformCom->Speed_Up(CGameInstance::GetInstance()->Key_Pressing(DIK_LSHIFT));

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
