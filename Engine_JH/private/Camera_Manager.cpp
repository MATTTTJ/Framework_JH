#include "stdafx.h"
#include "..\public\Camera_Manager.h"

#include "GameInstance.h"
#include "Object_Manager.h"
#include "PipeLine.h"
#include "Light_Manager.h"

IMPLEMENT_SINGLETON(CCamera_Manager);

CCamera_Manager::CCamera_Manager()
{
	m_iIndex = 1;
}

void CCamera_Manager::Tick()
{
	if (0 == m_vecCamera.size())
		return;

	CGameInstance* pGameInstance = CGameInstance::GetInstance();

	if (pGameInstance->Get_CurLevelIndex() == 2)
	{
		CCamera* pCamera = m_vecCamera[m_iIndex];

		NULL_CHECK(pCamera);

		CPipeLine* pPipeLine = CPipeLine::GetInstance();

		CTransform* pTransformCom = dynamic_cast<CTransform*>(pCamera->Get_Component(L"Com_Transform"));
		CCamera::CAMERADESC tCameraDesc = pCamera->Get_CamDesc();

		_matrix ViewMatrix = XMMatrixInverse(nullptr, pTransformCom->Get_WorldMatrix());
		pPipeLine->Set_Transform(CPipeLine::D3DTS_VIEW, ViewMatrix);

		_matrix ProjMatrix = XMMatrixPerspectiveFovLH(tCameraDesc.fFovy, tCameraDesc.fAspect, tCameraDesc.fNear, tCameraDesc.fFar);
		pPipeLine->Set_Transform(CPipeLine::D3DTS_PROJ, ProjMatrix);

		Set_Cascade();
	}

	if(pGameInstance->Get_CurLevelIndex() == 1)
	{
		m_vecCamera.clear();
	}
}

HRESULT CCamera_Manager::Add_Camera(_uint iLevelIndex, const wstring pLayerTag, const wstring pPrototypeTag,
	CCamera::CAMERADESC& tCameraDesc)
{
	CGameObject* pCamera = nullptr;
	pCamera = CObject_Manager::GetInstance()->Clone_GameObjectReturnPtr(iLevelIndex, pLayerTag, pPrototypeTag, &tCameraDesc);
	NULL_CHECK_RETURN(pCamera, E_FAIL);

	m_vecCamera.push_back(dynamic_cast<CCamera*>(pCamera));

	return S_OK;
}

void CCamera_Manager::Change_Camera()
{
	++m_iIndex;
	if (m_iIndex >= m_vecCamera.size())
		m_iIndex = 0;
}

_matrix CCamera_Manager::Get_Transform(_uint iIndex, _uint iState)
{
	if (0 == m_vecCamera.size())
		return XMMatrixIdentity();

	CCamera* pCamera = m_vecCamera[iIndex];

	if (nullptr == pCamera)
		return XMMatrixIdentity();

	switch (iState)
	{
		case CPipeLine::D3DTS_VIEW:
		{
			CTransform* pTransformCom = dynamic_cast<CTransform*>(pCamera->Get_Component(L"Com_Transform"));
			return XMMatrixInverse(nullptr, pTransformCom->Get_WorldMatrix());
		}
		break;

		case CPipeLine::D3DTS_PROJ:
		{
			CCamera::CAMERADESC tCameraDesc = pCamera->Get_CamDesc();
			return XMMatrixPerspectiveFovLH(tCameraDesc.fFovy, tCameraDesc.fAspect, tCameraDesc.fNear, tCameraDesc.fFar);;
		}
		break;
		}

	return XMMatrixIdentity();
}

_float4 CCamera_Manager::Get_CamPosition(_uint iIndex)
{
	CCamera* pCamera = m_vecCamera[iIndex];

	if (nullptr == pCamera)
		return _float4(0.f, 0.f, 0.f, 1.f);

	CTransform* pTransformCom = dynamic_cast<CTransform*>(pCamera->Get_Component(TEXT("Com_Transform")));

	_float4 vPos;

	XMStoreFloat4(&vPos, pTransformCom->Get_State(CTransform::STATE_TRANSLATION));

	return vPos;
}

CCamera::CAMERADESC CCamera_Manager::Get_CameraDesc()
{
	if (0 == m_vecCamera.size())
	{
		CCamera::CAMERADESC tDesc;
		ZeroMemory(&tDesc, sizeof(CCamera::CAMERADESC));
		return tDesc;
	}

	CCamera* pCamera = m_vecCamera[m_iIndex];

	if (nullptr == pCamera)
	{
		CCamera::CAMERADESC tDesc;
		ZeroMemory(&tDesc, sizeof(CCamera::CAMERADESC));
		return tDesc;
	}

	return pCamera->Get_CamDesc();
}

void CCamera_Manager::Clear()
{
	m_vecCamera.clear();
}

void CCamera_Manager::Set_Cascade()
{// 스태틱카메라
	CCamera* pCamera = m_vecCamera[m_iIndex];
	if (nullptr == pCamera)
		return;

	CTransform* pTransformCom = dynamic_cast<CTransform*>(pCamera->Get_Component(L"Com_Transform"));
	// 카메라의 역행렬과 시야각,화면비, 가까운 평면 Z,먼 평면 Z
	CCamera::CAMERADESC tCamDesc = pCamera->Get_CamDesc();

	//시야각을 이용하여 수직 시야각을 구함
	float fTanHalfVFov = tanf(tCamDesc.fFovy * 0.5f);
	// 수직 시야각을 이용하여 수평 시야각을 구함
	float fTanHalfHFov = fTanHalfVFov * tCamDesc.fAspect;

	//캐스케이드는 여러개 일단 한개만
	_float fCascadeEnd[2];

	fCascadeEnd[0] = tCamDesc.fNear;
	fCascadeEnd[1] = m_fFar;
	//fCascadeEnd[1] = tCamDesc.fFar;

	// 여러개의 절두체로 나눈다면 여기서 반복문을 실행!

	//+X,+Y 좌표에 수평,수직 시야각을 이용하여 구함. 각 부분 절두체의 가까운, 먼 평면의 값을 곱하여 4개의 점을 구함
	float fXN = fCascadeEnd[0] * fTanHalfHFov;
	float fXF = fCascadeEnd[1] * fTanHalfHFov;
	float fYN = fCascadeEnd[0] * fTanHalfVFov;
	float fYF = fCascadeEnd[1] * fTanHalfVFov;

	//+좌표값을 구하면 -좌표값을 구하여 각각의 절두체 평면을 구할수있음.
	//각 절두체의 Z값을 저장하여 i 가 낮은 순서로 가까운 평면, 먼평면을 구성함
	_float4 vFrustumCorners[8];
	//near Face
	vFrustumCorners[0] = _float4(fXN, fYN, fCascadeEnd[0], 1.f);
	vFrustumCorners[1] = _float4(-fXN, fYN, fCascadeEnd[0], 1.f);
	vFrustumCorners[2] = _float4(fXN, -fYN, fCascadeEnd[0], 1.f);
	vFrustumCorners[3] = _float4(-fXN, -fYN, fCascadeEnd[0], 1.f);
	//far Face
	vFrustumCorners[4] = _float4(fXF, fYF, fCascadeEnd[1], 1.f);
	vFrustumCorners[5] = _float4(-fXF, fYF, fCascadeEnd[1], 1.f);
	vFrustumCorners[6] = _float4(fXF, -fYF, fCascadeEnd[1], 1.f);
	vFrustumCorners[7] = _float4(-fXF, -fYF, fCascadeEnd[1], 1.f);

	CLight_Manager::GetInstance()->CascadeUpdate(pTransformCom->Get_WorldMatrix(), vFrustumCorners);
}

void CCamera_Manager::Free()
{
	m_vecCamera.clear();
}
