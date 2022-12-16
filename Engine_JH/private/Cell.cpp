#include "stdafx.h"
#include "..\public\Cell.h"
#include "Shader.h"
#include "PipeLine.h"
#include "VIBuffer_Cell.h"

CCell::CCell(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: m_pDevice(pDevice)
	, m_pContext(pContext)
	, m_iIndex(0)
	, m_vPoints{_float3(0.f,0.f,0.f),_float3(0.f,0.f,0.f) ,_float3(0.f,0.f,0.f) }
	, m_iNeighborIndices{ -1, -1, -1 }
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
}

HRESULT CCell::Initialize(const _float3* pPoints, _int iIndex)
{
	m_iIndex = iIndex;
	memcpy(m_vPoints, pPoints, sizeof(_float)*POINT_END);


#ifdef _DEBUG
	m_pVIBuffer = CVIBuffer_Cell::Create(m_pDevice, m_pContext, m_vPoints);
	NULL_CHECK_RETURN(m_pVIBuffer, E_FAIL);
#endif

	return S_OK;
}
// 세개의 점들 중 전달받은 두개의 점과 같은 점이 있는지를 체크한다.
_bool CCell::Compare_Points(const _float3& SourPoint, const _float3& DestPoint)
{
	if(true == XMVector3Equal(XMLoadFloat3(&m_vPoints[POINT_A]), XMLoadFloat3(&SourPoint)))
	{
		if (true == XMVector3Equal(XMLoadFloat3(&m_vPoints[POINT_B]), XMLoadFloat3(&DestPoint)))
			return true;
		if (true == XMVector3Equal(XMLoadFloat3(&m_vPoints[POINT_C]), XMLoadFloat3(&DestPoint)))
			return true;
	}

	if (true == XMVector3Equal(XMLoadFloat3(&m_vPoints[POINT_B]), XMLoadFloat3(&SourPoint)))
	{
		if (true == XMVector3Equal(XMLoadFloat3(&m_vPoints[POINT_C]), XMLoadFloat3(&DestPoint)))
			return true;
		if (true == XMVector3Equal(XMLoadFloat3(&m_vPoints[POINT_A]), XMLoadFloat3(&DestPoint)))
			return true;
	}

	if (true == XMVector3Equal(XMLoadFloat3(&m_vPoints[POINT_C]), XMLoadFloat3(&SourPoint)))
	{
		if (true == XMVector3Equal(XMLoadFloat3(&m_vPoints[POINT_A]), XMLoadFloat3(&DestPoint)))
			return true;
		if (true == XMVector3Equal(XMLoadFloat3(&m_vPoints[POINT_B]), XMLoadFloat3(&DestPoint)))
			return true;
	}

	return false;
}

_bool CCell::IsIn(_fvector vTargetPos, _int* pNeighborIndex)
{
	for(_uint i = 0; i<NEIGHBOR_END; ++i)
	{
		_vector		vLine = XMLoadFloat3(&m_vPoints[(i + 1) % NEIGHBOR_END]) - XMLoadFloat3(&m_vPoints[i]);
		_vector		vNormal = XMVector3Normalize(XMVectorSet(XMVectorGetZ(vLine) * -1.f, 0.f, XMVectorGetZ(vLine), 0.f));
		_vector		vDir = XMVector3Normalize(vTargetPos - XMLoadFloat3(&m_vPoints[i]));

		if(0<XMVectorGetX(XMVector3Dot(vNormal, vDir)))
		{
			*pNeighborIndex = m_iNeighborIndices[i];
			return false;
		}
	}
	return true;
}

#ifdef _DEBUG
HRESULT CCell::Render(CShader* pShader)
{
	if (nullptr == m_pVIBuffer)
		return E_FAIL;

	_float4x4		WorldMatrix;
	XMStoreFloat4x4(&WorldMatrix, XMMatrixIdentity());

	pShader->Set_Matrix(L"g_WorldMatrix", &WorldMatrix);

	CPipeLine*		pPipeLine = GET_INSTANCE(CPipeLine);

	pShader->Set_Matrix(L"g_ViewMatrix", &pPipeLine->Get_TransformFloat4x4(CPipeLine::D3DTS_VIEW));
	pShader->Set_Matrix(L"g_ProjMatrix", &pPipeLine->Get_TransformFloat4x4(CPipeLine::D3DTS_PROJ));


	RELEASE_INSTANCE(CPipeLine);

	pShader->Begin(0);

	m_pVIBuffer->Render();

	return S_OK;
}
#endif

CCell* CCell::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const _float3* pPoints, _int iIndex)
{
	CCell*		pInstance = new CCell(pDevice, pContext);

	if (FAILED(pInstance->Initialize(pPoints, iIndex)))
	{
		MSG_BOX("Failed to Created : CCell");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CCell::Free()
{
	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);

#ifdef _DEBUG
	Safe_Release(m_pVIBuffer);
#endif

}
