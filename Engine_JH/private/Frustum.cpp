#include "stdafx.h"
#include "..\public\Frustum.h"
#include "PipeLine.h"

IMPLEMENT_SINGLETON(CFrustum);

CFrustum::CFrustum()
{
}


HRESULT CFrustum::Initialize()
{
	// 투영 스페이스 공간에서의 여덟개 점을 정의
	m_vPoints[0] = _float3(-1.f, 1.f, 0.f);
	m_vPoints[1] = _float3(1.f, 1.f, 0.f);
	m_vPoints[2] = _float3(1.f, -1.f, 0.f);
	m_vPoints[3] = _float3(-1.f, -1.f, 0.f);

	m_vPoints[4] = _float3(-1.f, 1.f, 1.f);
	m_vPoints[5] = _float3(1.f, 1.f, 1.f);
	m_vPoints[6] = _float3(1.f, -1.f, 1.f);
	m_vPoints[7] = _float3(-1.f, -1.f, 1.f);

	return S_OK;
}

void CFrustum::Transform_ToWorldSpace()
{
	// 일반적인 절두체와 객체와의 위치비교는 월드 스페이스에서 자주 일어남
	// 단 정점의 경우 위치와 절두체비교는 정점이 너무 많아서 로컬이 훨씬 유리하다. 
	CPipeLine*			pPipeLine = GET_INSTANCE(CPipeLine);

	_matrix			ProjMatrixInv = pPipeLine->Get_TransformMatrix_Inverse(CPipeLine::D3DTS_PROJ);
	_matrix			ViewMatrixInv = pPipeLine->Get_TransformMatrix_Inverse(CPipeLine::D3DTS_VIEW);

	for(_uint i =0; i < 8; ++i)
	{
		XMStoreFloat3(&m_vPointInWorld[i], XMVector3TransformCoord(XMLoadFloat3(&m_vPoints[i]), ProjMatrixInv));
		XMStoreFloat3(&m_vPointInWorld[i], XMVector3TransformCoord(XMLoadFloat3(&m_vPointInWorld[i]), ViewMatrixInv));
	}

	Make_Planes(m_PlaneInWorld, m_vPointInWorld);

	RELEASE_INSTANCE(CPipeLine);
}

void CFrustum::Transform_ToLocalSpace(_fmatrix WorldMatrix)
{
	_matrix WorldMatrixInv = XMMatrixInverse(nullptr, WorldMatrix);

	_float3	vPoint[8];

	for (_uint i = 0; i < 8; ++i)
		XMStoreFloat3(&vPoint[i], XMVector3TransformCoord(XMLoadFloat3(&m_vPointInWorld[i]), WorldMatrixInv));

	Make_Planes(m_PlaneInLocal, vPoint);
}

_bool CFrustum::isInFrustum_WorldSpace(_fvector vWorldPos, _float fRange)
{
	for(_uint i = 0; i < 6; ++i)
	{
		if (fRange < XMVectorGetX(XMPlaneDotCoord(XMLoadFloat4(&m_PlaneInWorld[i]), vWorldPos)))
			return false;
	}

	return true;
}

_bool CFrustum::isInFrustum_LocalSpace(_fvector vLocalPos, _float fRange)
{
	for(_uint i = 0; i < 6; ++i)
	{
		if (fRange < XMVectorGetX(XMPlaneDot(XMLoadFloat4(&m_PlaneInLocal[i]), vLocalPos)))
			return false;
	}

	return true;
}

void CFrustum::Make_Planes(_float4* pPlane, const _float3* pPoints)
{
	XMStoreFloat4(&pPlane[0],
		XMPlaneFromPoints(XMLoadFloat3(&pPoints[1]), XMLoadFloat3(&pPoints[5]), XMLoadFloat3(&pPoints[6])));
	XMStoreFloat4(&pPlane[1],
		XMPlaneFromPoints(XMLoadFloat3(&pPoints[4]), XMLoadFloat3(&pPoints[0]), XMLoadFloat3(&pPoints[3])));
	XMStoreFloat4(&pPlane[2],
		XMPlaneFromPoints(XMLoadFloat3(&pPoints[4]), XMLoadFloat3(&pPoints[5]), XMLoadFloat3(&pPoints[1])));
	XMStoreFloat4(&pPlane[3],
		XMPlaneFromPoints(XMLoadFloat3(&pPoints[3]), XMLoadFloat3(&pPoints[2]), XMLoadFloat3(&pPoints[6])));
	XMStoreFloat4(&pPlane[4],
		XMPlaneFromPoints(XMLoadFloat3(&pPoints[5]), XMLoadFloat3(&pPoints[4]), XMLoadFloat3(&pPoints[7])));
	XMStoreFloat4(&pPlane[5],
		XMPlaneFromPoints(XMLoadFloat3(&pPoints[0]), XMLoadFloat3(&pPoints[1]), XMLoadFloat3(&pPoints[2])));
}

void CFrustum::Free()
{
}



