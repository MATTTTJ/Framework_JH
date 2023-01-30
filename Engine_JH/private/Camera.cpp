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
	// // ���彺���̽��� ������� ī�޶��� �佺���̽�
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
	// // ī�޶��� ����İ� �þ߰�, ȭ���, ����� ���z, �� ��� z
	//
	// // �þ߰��� �̿��Ͽ� ���� �þ߰��� ����
	// _float fTanHalfVFov = tanf(m_CameraDesc.fFovy * 0.5f);
	// // ���� �þ߰��� �̿��Ͽ� ���� �þ߰��� ����
	// _float fTanHalfHFov = fTanHalfVFov * m_CameraDesc.fAspect;
	//
	// // ĳ�����̵�� ���������� �ϴ� �Ѱ���
	// _float fCascadeEnd[2];
	//
	// fCascadeEnd[0] = m_CameraDesc.fNear;
	// fCascadeEnd[1] = 15.f;
	//
	// // �������� ����ü�� �����ٸ� ���⼭ �ݺ����� �����߰���
	//
	// // +X, +Y ��ǥ�� ����, ���� �þ߰��� �̿��Ͽ� ����, �� �κ� ����ü�� �����, �� ����� ���� ���Ͽ� 4���� ���� ����
	// _float fXN = fCascadeEnd[0] * fTanHalfHFov;
	// _float fXF = fCascadeEnd[1] * fTanHalfHFov;
	// _float fYN = fCascadeEnd[0] * fTanHalfVFov;
	// _float fYF = fCascadeEnd[1] * fTanHalfVFov;
	//
	// // + ��ǥ���� ���ϸ� -��ǥ���� ���Ͽ� ������ ����ü ����� ���� �� ����
	// // �� ����ü�� Z���� �����Ͽ� i�� ���� ������� ����� ���, �� ����� �����Ѵ�.
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
