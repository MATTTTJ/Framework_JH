#include "stdafx.h"
#include "..\public\Navigation.h"
#include "Cell.h"
#include "Shader.h"

CNavigation::CNavigation(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CComponent(pDevice, pContext)
{
}

CNavigation::CNavigation(const CNavigation& rhs)
	: CComponent(rhs)
	, m_vecCells(rhs.m_vecCells)
#ifdef _DEBUG
	, m_pShader(rhs.m_pShader)
#endif
{
	for (auto& pCell : m_vecCells)
		Safe_AddRef(pCell);

#ifdef _DEBUG
	Safe_AddRef(m_pShader);
#endif

}

HRESULT CNavigation::Initialize_Prototype(const wstring pNavigationDataFilePath)
{
	_ulong		dwByte = 0;
	HANDLE		hFile = CreateFile(pNavigationDataFilePath.c_str(), GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if (0 == hFile)
		return E_FAIL;

	_float3		vPoints[CCell::POINT_END];

	while (true)
	{
		ReadFile(hFile, vPoints, sizeof(_float3) * CCell::POINT_END, &dwByte, nullptr);
		if (0 == dwByte)
			break;

		CCell*		pCell = CCell::Create(m_pDevice, m_pContext, vPoints, (_int)m_vecCells.size());
		if (nullptr == pCell)
			return E_FAIL;

		m_vecCells.push_back(pCell);
	}

	CloseHandle(hFile);

	FAILED_CHECK_RETURN(Ready_Neighbor(), E_FAIL);

#ifdef _DEBUG
	m_pShader = CShader::Create(m_pDevice, m_pContext, L"../Bin/ShaderFiles/Shader_Navigation.hlsl",CShader::DECLARATION_VTXPOS, VTXPOS_DECLARATION::Elements, VTXPOS_DECLARATION::iNumElements);
	NULL_CHECK_RETURN(m_pShader, E_FAIL);
#endif

	return S_OK;
}

HRESULT CNavigation::Initialize_Clone(CGameObject* pOwner, void* pArg)
{
	if (nullptr != pArg)
	{
		memcpy(&m_tNaviDesc, pArg, sizeof(NAVIDESC));
	}

	return S_OK;
}

_bool CNavigation::IsMove_OnNavigation(_fvector vTargetPos)
{
	if (-1 == m_tNaviDesc.iCurrentIndex)
		return false;

	_int	iNeighborIndex = -1;

	// 움직이고 난 결과위치가 셀 안에 있다면 
	if (true == m_vecCells[m_tNaviDesc.iCurrentIndex]->IsIn(vTargetPos, &iNeighborIndex))
		return true;
	// 움직이고 난 결과 위치가 이 셀을 벗어난다면
	else
	{
		// 나간 방향으로 이웃이 있다면
		if(-1 != iNeighborIndex)
		{
			while(true)
			{
				if( -1 == iNeighborIndex)
				{
					return false;
				}

				if(true == m_vecCells[iNeighborIndex]->IsIn(vTargetPos, &iNeighborIndex))
				{
					// m_tNaviDesc.iCurrentIndex = 이웃의 인덱스
					m_tNaviDesc.iCurrentIndex = iNeighborIndex;
					return true;
				}
			}
		}
		else // 나간 방향으로 이웃이 없었다면 
		{
			return false;
		}
	}
}
#ifdef _DEBUG
HRESULT CNavigation::Render()
{
	_float fHeight = 0.f;

	if(-1 == m_tNaviDesc.iCurrentIndex)
	{
		fHeight = 0.0f;
		HRESULT	hr = m_pShader->Set_RawValue(L"g_Height", &fHeight, sizeof(_float));
		m_pShader->Set_RawValue(L"g_vColor", &_float4(0.f, 1.f, 0.f, 1.f), sizeof(_float4));
	}
	else
	{
		fHeight = 0.1f;
		HRESULT	hr = m_pShader->Set_RawValue(L"g_Height", &fHeight, sizeof(_float));
		m_pShader->Set_RawValue(L"g_vColor", &_float4(1.f, 0.f, 0.f, 1.f), sizeof(_float4));

		m_vecCells[m_tNaviDesc.iCurrentIndex]->Render(m_pShader);

		return S_OK;
	}

	for(auto& pCell : m_vecCells)
	{
		if (nullptr != pCell)
			pCell->Render(m_pShader);
	}

	return S_OK;
}
#endif
HRESULT CNavigation::Ready_Neighbor()
{
	for (auto& pSourCell : m_vecCells)
	{
		for (auto& pDestCell : m_vecCells)
		{
			if (pSourCell == pDestCell)
				continue;

			if (true == pDestCell->Compare_Points(pSourCell->Get_Point(CCell::POINT_A), pSourCell->Get_Point(CCell::POINT_B)))
			{
				pSourCell->Set_Neighbor(CCell::NEIGHBOR_AB, pDestCell);
			}

			else if (true == pDestCell->Compare_Points(pSourCell->Get_Point(CCell::POINT_B), pSourCell->Get_Point(CCell::POINT_C)))
			{
				pSourCell->Set_Neighbor(CCell::NEIGHBOR_BC, pDestCell);
			}

			else if (true == pDestCell->Compare_Points(pSourCell->Get_Point(CCell::POINT_C), pSourCell->Get_Point(CCell::POINT_A)))
			{
				pSourCell->Set_Neighbor(CCell::NEIGHBOR_CA, pDestCell);
			}
		}
	}

	return S_OK;
}

CNavigation* CNavigation::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext,	const wstring wstrNavigationFilePath)
{
	CNavigation*		pInstance = new CNavigation(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype(wstrNavigationFilePath)))
	{
		MSG_BOX("Failed to Created : CNavigation");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CComponent* CNavigation::Clone(CGameObject* pOwner, void* pArg)
{
	CNavigation*		pInstance = new CNavigation(*this);

	if (FAILED(pInstance->Initialize_Clone(pOwner, pArg)))
	{
		MSG_BOX("Failed to Created : CNavigation");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CNavigation::Free()
{
	__super::Free();

	for (auto& pCell : m_vecCells)
		Safe_Release(pCell);
	m_vecCells.clear();

#ifdef _DEBUG
	Safe_Release(m_pShader);
#endif // _DEBUG
}
