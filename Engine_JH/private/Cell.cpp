#include "..\public\Cell.h"

CCell::CCell(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: m_iIndex(0)
	, m_vPoints{_float3(0.f,0.f,0.f),_float3(0.f,0.f,0.f) ,_float3(0.f,0.f,0.f) }
	, m_iNeighborIndices{ -1, -1, -1 }
{
}

HRESULT CCell::Initialize(const _float3* pPoints, _int iIndex)
{
	return S_OK;
}

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
}
