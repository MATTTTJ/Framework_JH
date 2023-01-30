#include "stdafx.h"
#include "..\public\Camera.h"
#include "GameInstance.h"
#include "Light_Manager.h"
#include "PipeLine.h"


CCamera::CCamera(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject(pDevice, pContext)
	, m_pPipeLine(CPipeLine::GetInstance())
{
	Safe_AddRef(m_pPipeLine);
}

CCamera::CCamera(const CCamera& rhs)
	: CGameObject(rhs)
	, m_pPipeLine(rhs.m_pPipeLine)
{
	Safe_AddRef(m_pPipeLine);
}

HRESULT CCamera::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CCamera::Initialize_Clone(const wstring& wstrPrototypeTag, void* pArg)
{
	ZeroMemory(&m_CameraDesc, sizeof(CAMERADESC));

	if (nullptr != pArg)
		memcpy(&m_CameraDesc, pArg, sizeof(m_CameraDesc));

	FAILED_CHECK_RETURN(__super::Initialize_Clone(wstrPrototypeTag, &m_CameraDesc), E_FAIL);

	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMLoadFloat4(&m_CameraDesc.vEye));
	m_pTransformCom->LookAt(XMLoadFloat4(&m_CameraDesc.vAt));

	return S_OK;
}

void CCamera::Tick(_double TimeDelta)
{
	// D3D11_VIEWPORT		ViewportDesc;
	// ZeroMemory(&ViewportDesc, sizeof(D3D11_VIEWPORT));
	//
	// _uint iNumViewPorts = 1;
	//
	// m_pContext->RSGetViewports(&iNumViewPorts, &ViewportDesc);

	// m_CameraDesc.vUp = m_pTransformCom->Get_State(CTransform::STATE_UP);
	// m_CameraDesc.vAt = m_pTransformCom->Get_State(CTransform::STATE_LOOK);
	// m_CameraDesc.vEye = m_pTransformCom->Get_State(CTransform::STATE_TRANSLATION);
	// m_CameraDesc.fAspect = ViewportDesc.Width / (_float)ViewportDesc.Height;

	// NULL_CHECK(m_pPipeLine);
	//
	// // 월드스페이스의 역행렬은 카메라의 뷰스페이스
	// m_pPipeLine->Set_Transform(CPipeLine::D3DTS_VIEW, m_pTransformCom->Get_WorldMatrix_Inverse());
	// m_pPipeLine->Set_Transform(CPipeLine::D3DTS_PROJ, XMMatrixPerspectiveFovLH(m_CameraDesc.fFovy, m_CameraDesc.fAspect, m_CameraDesc.fNear, m_CameraDesc.fFar));

	// SetCascade();
}

void CCamera::Late_Tick(_double TimeDelta)
{
}

HRESULT CCamera::Render()
{
	return S_OK;
}

void CCamera::SetCascade()
{
	// // 카메라의 역행렬과 시야각, 화면비, 가까운 평면z, 먼 평면 z
	//
	// // 시야각을 이용하여 수직 시야각을 구함
	// _float fTanHalfVFov = tanf(m_CameraDesc.fFovy * 0.5f);
	// // 수직 시야각을 이용하여 수평 시야각을 구함
	// _float fTanHalfHFov = fTanHalfVFov * m_CameraDesc.fAspect;
	//
	// // 캐스케이드는 여러개지만 일단 한개만
	// _float fCascadeEnd[2];
	//
	// fCascadeEnd[0] = m_CameraDesc.fNear;
	// fCascadeEnd[1] = 15.f;
	//
	// // 여러개의 절두체로 나눈다면 여기서 반복문을 돌려야겠지
	//
	// // +X, +Y 좌표에 수평, 수직 시야각을 이용하여 구함, 각 부분 절두체의 가까운, 먼 평면의 값을 곱하여 4개의 점을 구함
	// _float fXN = fCascadeEnd[0] * fTanHalfHFov;
	// _float fXF = fCascadeEnd[1] * fTanHalfHFov;
	// _float fYN = fCascadeEnd[0] * fTanHalfVFov;
	// _float fYF = fCascadeEnd[1] * fTanHalfVFov;
	//
	// // + 좌표값을 구하면 -좌표값을 구하여 각각의 절두체 평면을 구할 수 있음
	// // 각 절두체의 Z값을 저장하여 i가 낮은 순서대로 가까운 평면, 먼 평면을 구성한다.
	//
	// _float4 vFrustumCorners[8];
	//
	// // near Face
	// vFrustumCorners[0] = _float4(fXN, fYN, fCascadeEnd[0], 1.f);
	// vFrustumCorners[1] = _float4(-fXN, fYN, fCascadeEnd[0], 1.f);
	// vFrustumCorners[2] = _float4(fXN, -fYN, fCascadeEnd[0], 1.f);
	// vFrustumCorners[3] = _float4(-fXN, -fYN, fCascadeEnd[0], 1.f);
	//
	// vFrustumCorners[4] = _float4(fXF, fYF, fCascadeEnd[1], 1.f);
	// vFrustumCorners[5] = _float4(-fXN, fYF, fCascadeEnd[1], 1.f);
	// vFrustumCorners[6] = _float4(fXF, -fYF, fCascadeEnd[1], 1.f);
	// vFrustumCorners[7] = _float4(-fXF, -fYF, fCascadeEnd[1], 1.f);
	//
	// CLight_Manager::GetInstance()->CascadeUpdate(m_pTransformCom->Get_WorldFloat4x4(), vFrustumCorners);
}

void CCamera::Free()
{
	__super::Free();

	Safe_Release(m_pPipeLine);
}
