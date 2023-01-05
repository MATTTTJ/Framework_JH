#include "stdafx.h"
#include "..\public\Navigation.h"
#include "Cell.h"
#include "Shader.h"
#include "GameInstance.h"
#include "Json/json.hpp"
#include <fstream>

CNavigation::CNavigation(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CComponent(pDevice, pContext)
{
}

CNavigation::CNavigation(const CNavigation& rhs)
	: CComponent(rhs)
	, m_vecCell(rhs.m_vecCell)
	, m_tNaviDesc(rhs.m_tNaviDesc)

#ifdef _DEBUG
	, m_pShaderCom( rhs.m_pShaderCom)
#endif
{
	for (auto& pCell : m_vecCell)
		Safe_AddRef(pCell);

#ifdef _DEBUG
	Safe_AddRef(m_pShaderCom);
#endif

}

HRESULT CNavigation::Initialize_Prototype(const wstring& wstrFilePath)
{
	if (wstrFilePath != L"")
	{
		m_wstrFilePath = wstrFilePath;

		Json	jCells;

		string	strFilePath = "";
		strFilePath.assign(m_wstrFilePath.begin(), m_wstrFilePath.end());

		ifstream	file(strFilePath.c_str());
		file >> jCells;
		file.close();

		_uint	iCellCount;
		jCells["Cell Count"].get_to<_uint>(iCellCount);
		
		for (auto jCell : jCells["Cells"])
		{
			_float3	vPoint[CCell::POINT_END];

			jCell["Point A"][0].get_to<_float>(vPoint[CCell::POINT_A].x);
			jCell["Point A"][1].get_to<_float>(vPoint[CCell::POINT_A].y);
			jCell["Point A"][2].get_to<_float>(vPoint[CCell::POINT_A].z);
			jCell["Point B"][0].get_to<_float>(vPoint[CCell::POINT_B].x);
			jCell["Point B"][1].get_to<_float>(vPoint[CCell::POINT_B].y);
			jCell["Point B"][2].get_to<_float>(vPoint[CCell::POINT_B].z);
			jCell["Point C"][0].get_to<_float>(vPoint[CCell::POINT_C].x);
			jCell["Point C"][1].get_to<_float>(vPoint[CCell::POINT_C].y);
			jCell["Point C"][2].get_to<_float>(vPoint[CCell::POINT_C].z);

			_vector vA = vPoint[0];
			_vector vB = vPoint[1];
			_vector vC = vPoint[2];

			_vector vAB = vB - vA;
			_vector vBC = vC - vB;

			_vector vCross = XMVector3Cross(vAB, vBC);

			assert(XMVectorGetY(vCross) != 0.f);

			if (XMVectorGetY(vCross) < 0.f)
			{
				_float3 tmp = vPoint[1];
				vPoint[1] = vPoint[2];
				vPoint[2] = tmp;
			}

			CCell*		pCell = CCell::Create(m_pDevice, m_pContext, vPoint, (_int)m_vecCell.size());
			NULL_CHECK_RETURN(pCell, E_FAIL);

			m_vecCell.push_back(pCell);
		}
	}

	FAILED_CHECK_RETURN(Ready_Neighbor(), E_FAIL);

#ifdef _DEBUG
	m_pShaderCom = CShader::Create(m_pDevice, m_pContext, L"../Bin/ShaderFiles/Shader_Navigation.hlsl", CShader::DECLARATION_VTXPOS, VTXPOS_DECLARATION::Elements, VTXPOS_DECLARATION::iNumElements);
	NULL_CHECK_RETURN(m_pShaderCom, E_FAIL);
#endif // _DEBUG

	return S_OK;
}

HRESULT CNavigation::Initialize_Clone(CGameObject* pOwner, void* pArg)
{
	if (nullptr != pArg)
	{
		memcpy(&m_tNaviDesc, pArg, sizeof(NAVIDESC));
	}

	FAILED_CHECK_RETURN(__super::Initialize_Clone(pOwner, pArg), E_FAIL);

	return S_OK;
}

HRESULT CNavigation::Add_Cell(_float3* vPoints)
{
	CCell*		pCell = CCell::Create(m_pDevice, m_pContext, vPoints, (_int)m_vecCell.size());
	NULL_CHECK_RETURN(pCell, E_FAIL);

	m_vecCell.push_back(pCell);
	FAILED_CHECK_RETURN(Ready_Neighbor(), E_FAIL);

	return S_OK;
}

HRESULT CNavigation::Delete_Cell(_int iIndex)
{
	if (iIndex < 0 || iIndex >= (_int)m_vecCell.size())
		return E_FAIL;

	auto iter = m_vecCell.begin();

	for (_int i = 0; i < iIndex; ++i)
		++iter;

	Safe_Release(*iter);
	m_vecCell.erase(iter);

	return S_OK;
}

HRESULT CNavigation::Save_Cell(string strSaveFilePath)
{
	Json	jCells;
	Json	jCell;

	jCells["Cell Count"] = (_uint)m_vecCell.size();

	for (auto& pCell : m_vecCell)
	{
		jCell["Index"] = pCell->Get_Index();
		jCell["Point A"].push_back(pCell->Get_Point(CCell::POINT_A).x);
		jCell["Point A"].push_back(pCell->Get_Point(CCell::POINT_A).y);
		jCell["Point A"].push_back(pCell->Get_Point(CCell::POINT_A).z);
		jCell["Point B"].push_back(pCell->Get_Point(CCell::POINT_B).x);
		jCell["Point B"].push_back(pCell->Get_Point(CCell::POINT_B).y);
		jCell["Point B"].push_back(pCell->Get_Point(CCell::POINT_B).z);
		jCell["Point C"].push_back(pCell->Get_Point(CCell::POINT_C).x);
		jCell["Point C"].push_back(pCell->Get_Point(CCell::POINT_C).y);
		jCell["Point C"].push_back(pCell->Get_Point(CCell::POINT_C).z);
		jCell["Neighbor Index"].push_back(pCell->Get_NeighbotIndex()[CCell::NEIGHBOR_AB]);
		jCell["Neighbor Index"].push_back(pCell->Get_NeighbotIndex()[CCell::NEIGHBOR_BC]);
		jCell["Neighbor Index"].push_back(pCell->Get_NeighbotIndex()[CCell::NEIGHBOR_CA]);

		jCells["Cells"].push_back(jCell);
		jCell.clear();
	}

	ofstream	file(strSaveFilePath.c_str());
	file << jCells;
	file.close();

	jCells.clear();

	return S_OK;
}

HRESULT CNavigation::Find_NearBy_Point(_float3& vPoint)
{
	_float3	vPoints[CCell::POINT_END];

	for (auto& pCell : m_vecCell)
	{
		ZeroMemory(&vPoints, sizeof(_float3) * CCell::POINT_END);

		vPoints[CCell::POINT_A] = pCell->Get_Point(CCell::POINT_A);
		vPoints[CCell::POINT_B] = pCell->Get_Point(CCell::POINT_B);
		vPoints[CCell::POINT_C] = pCell->Get_Point(CCell::POINT_C);
		_vector	vDir;
		_float		fDist = 10000.f;

		vDir = vPoints[CCell::POINT_A] - vPoint;
		fDist = XMVectorGetX(XMVector3Length(vDir));
		if (fDist < 0.15f)
		{
			vPoint = vPoints[CCell::POINT_A];
			break;
		}

		vDir = vPoints[CCell::POINT_B] - vPoint;
		fDist = XMVectorGetX(XMVector3Length(vDir));
		if (fDist < 0.15f)
		{
			vPoint = vPoints[CCell::POINT_B];
			break;
		}

		vDir = vPoints[CCell::POINT_C] - vPoint;
		fDist = XMVectorGetX(XMVector3Length(vDir));
		if (fDist < 0.15f)
		{
			vPoint = vPoints[CCell::POINT_C];
			break;
		}
	}

	return S_OK;
}

_bool CNavigation::IsMove_OnNavigation(_fvector vTargetPos)
{
	if (-1 == m_tNaviDesc.iCurrentIndex)
		return false;

	_int	iNeighborIndex = -1;

	// 움직이고 난 결과위치가 셀 안에 있다면 
	if (true == m_vecCell[m_tNaviDesc.iCurrentIndex]->IsIn(vTargetPos, &iNeighborIndex))
	{
		m_fCellHeight = m_vecCell[m_tNaviDesc.iCurrentIndex]->Get_CellHeight(vTargetPos);

		return true;
	}
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

				if(true == m_vecCell[iNeighborIndex]->IsIn(vTargetPos, &iNeighborIndex))
				{
					// m_tNaviDesc.iCurrentIndex = 이웃의 인덱스
					m_tNaviDesc.iCurrentIndex = iNeighborIndex;

					m_fCellHeight = m_vecCell[iNeighborIndex]->Get_CellHeight(vTargetPos);

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
		fHeight = 0.1f;
		HRESULT	hr = m_pShaderCom->Set_RawValue(L"g_fHeight", &fHeight, sizeof(_float));
		m_pShaderCom->Set_RawValue(L"g_vColor", &_float4(0.f, 1.f, 0.f, 1.f), sizeof(_float4));
	}
	else
	{
		fHeight = 0.15f;
		HRESULT	hr = m_pShaderCom->Set_RawValue(L"g_fHeight", &fHeight, sizeof(_float));
		m_pShaderCom->Set_RawValue(L"g_vColor", &_float4(1.f, 0.f, 0.f, 1.f), sizeof(_float4));

		m_vecCell[m_tNaviDesc.iCurrentIndex]->Render(m_pShaderCom);

		return S_OK;
	}


	for (auto& pCell : m_vecCell)
	{
		if (pCell != nullptr)
			pCell->Render(m_pShaderCom);
	}

	return S_OK;
}
#endif
HRESULT CNavigation::Ready_Neighbor()
{
	for (auto& pSourCell : m_vecCell)
	{
		for (auto& pDestCell : m_vecCell)
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

CNavigation* CNavigation::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext,	const wstring& wstrFilePath)
{
	CNavigation*		pInstance = new CNavigation(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype(wstrFilePath)))
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

	for (auto& pCell : m_vecCell)
		Safe_Release(pCell);
	m_vecCell.clear();

#ifdef _DEBUG
	Safe_Release(m_pShaderCom);
#endif // _DEBUG
}
