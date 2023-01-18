#include "stdafx.h"
#include "..\public\Cell.h"
#include "GameInstance.h"
#include "GameUtils.h"

CCell::CCell(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: m_pDevice(pDevice)
	, m_pContext(pContext)

{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
}

HRESULT CCell::Initialize(const _float3* pPoints, _int iIndex)
{
	m_iIndex = iIndex;
	memcpy(m_vPoints, pPoints, sizeof(_float3) * POINT_END);


#ifdef _DEBUG
	m_pVIBuffer = CVIBuffer_Cell::Create(m_pDevice, m_pContext, m_vPoints);

	NULL_CHECK_RETURN(m_pVIBuffer, E_FAIL);

	m_pVIBufferCircleCom[POINT_A] = CVIBuffer_Cell_Circle::Create(m_pDevice, m_pContext, m_vPoints[POINT_A]);
	m_pVIBufferCircleCom[POINT_B] = CVIBuffer_Cell_Circle::Create(m_pDevice, m_pContext, m_vPoints[POINT_B]);
	m_pVIBufferCircleCom[POINT_C] = CVIBuffer_Cell_Circle::Create(m_pDevice, m_pContext, m_vPoints[POINT_C]);
#endif

	return S_OK;
}

void CCell::ImGui_RenderProperty()
{
	ImGui::InputFloat3("POINT A", &m_vPoints[POINT_A].x);
	ImGui::InputFloat3("POINT B", &m_vPoints[POINT_B].x);
	ImGui::InputFloat3("POINT C", &m_vPoints[POINT_C].x);
	ImGui::NewLine();
	ImGui::InputInt3("Neighbor", m_iNeighborIndices, ImGuiInputTextFlags_ReadOnly);
}

// 세개의 점들 중 전달받은 두개의 점과 같은 점이 있는지를 체크한다.
_bool CCell::Compare_Points(const _float3& SourPoint, const _float3& DestPoint)
{
	if (true == XMVector3Equal(XMLoadFloat3(&m_vPoints[POINT_A]), XMLoadFloat3(&SourPoint)))
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

_bool CCell::Compare_Height(_fvector vTargetPos)
{
	_float		fAvgHeight = 0.f;

	for (_uint i = 0; i < POINT_END; ++i)
		fAvgHeight += m_vPoints[i].y;

	fAvgHeight /= 3.f;

	if (fAvgHeight > XMVectorGetY(vTargetPos))
		return true;
	else
		return false;
}

_bool CCell::Compare_VerHeight(_fvector vTargetPos)
{
	_float		fAvgHeight = 0.f;

	for (_uint i = 0; i < POINT_END; ++i)
		fAvgHeight += m_vPoints[i].y;

	fAvgHeight /= 3.f;

	fAvgHeight = fAvgHeight + 2.644f;
	if (fAvgHeight - XMVectorGetY(vTargetPos) > 0.3f)
		return true;
	else
		return false;
}

_bool CCell::Compare_VerHeight(_fvector vTargetPos, CCell* pOtherCell)
{
	_float		fAvgHeight = 0.f;
	_float		fAvgHeight_A = 0.f;

	for (_uint i = 0; i < POINT_END; ++i)
	{
		fAvgHeight += m_vPoints[i].y;
		fAvgHeight_A += pOtherCell->m_vPoints[i].y;
	}

	fAvgHeight /= 3.f;
	fAvgHeight_A /= 3.f;

	if ((fAvgHeight - XMVectorGetY(vTargetPos)) > 0.15f)
		return false;
	else
		return true;
}

void CCell::Get_BlockedLine(NEIGHBOR eNeighbor, _float4& vBlockedLine, _float4& vBlockedLineNormal)
{
	_vector	vLine = XMLoadFloat3(&m_vPoints[(eNeighbor + 1 % NEIGHBOR_END)]) - XMLoadFloat3(&m_vPoints[eNeighbor]);
	_vector vNormal = XMVector3Normalize(XMVectorSet(XMVectorGetZ(vLine) * -1.f, 0.f, XMVectorGetX(vLine), 0.f));

	XMStoreFloat4(&vBlockedLine, vLine);
	XMStoreFloat4(&vBlockedLineNormal, vNormal);
}

_bool CCell::IsIn(_fvector vTargetPos, _int& pNeighborIndex, _float4 & vBlockedLine, _float4 & vBlockedLineNormal)
{
	for (_uint i = 0; i < NEIGHBOR_END; ++i)
	{
		_vector		vLine = XMLoadFloat3(&m_vPoints[(i + 1) % NEIGHBOR_END]) - XMLoadFloat3(&m_vPoints[i]);
		_vector		vNormal = XMVector3Normalize(XMVectorSet(XMVectorGetZ(vLine) * -1.f, 0.f, XMVectorGetX(vLine), 0.f));
		_vector		vDir = XMVector3Normalize(vTargetPos - XMLoadFloat3(&m_vPoints[i]));

		if (0.f < XMVectorGetX(XMVector3Dot(vNormal, vDir)))
		{
			pNeighborIndex = m_iNeighborIndices[i];

			if(-1 == pNeighborIndex)
			{
				XMStoreFloat4(&vBlockedLine, vLine);
				XMStoreFloat4(&vBlockedLineNormal, vNormal);
			}
			return false;
		}
	}
	return true;
}



_float4 CCell::Get_CellHeight(_float4 fTargetPos)
{

	// 플레이어 위치에서 위로 수직인 위치를 빼서 광선을 만들고, 그 광선으로 셀에 쏜다.
	
	//
	// _float4 fTargetPosUpper = _float4(fTargetPos.x, fTargetPos.y + 10.f, fTargetPos.z, 1.f);
	// _float4 fDir = fTargetPos - fTargetPosUpper;
	// fDir.Normalize();

	// _float4	vRayPos, vRayDir;
	// ZeroMemory(&vRayPos, sizeof(_float4));
	// ZeroMemory(&vRayDir, sizeof(_float4));
	//
	// vRayPos = fTargetPos;
	// vRayPos.y += 50.f;
	//
	// vRayDir = fTargetPos - vRayPos;
	// vRayDir.Normalize();

	_vector vRayPos, vRayDir;


	vRayPos = XMLoadFloat4(&fTargetPos);
	vRayPos += XMVectorSet(0.f, 50.f, 0.f, 0.f);

	vRayDir = XMLoadFloat4(&fTargetPos) - vRayPos;
	vRayDir = XMVector3Normalize(vRayDir);
	// vRayDir -= XMVectorSet(0.f, 0.f, 0.f, XMVectorGetW(vRayDir));
	_vector tmp = vRayDir;
	_vector dest = vRayPos;

	_float fDist = 0.f;

	_vector	vPointA = XMVectorSetW(m_vPoints[POINT_A], 1.f);
	_vector	vPointB = XMVectorSetW(m_vPoints[POINT_B], 1.f);
	_vector	vPointC = XMVectorSetW(m_vPoints[POINT_C], 1.f);

	if (TriangleTests::Intersects(dest, vRayDir,	vPointA, vPointB, vPointC, fDist))
	{
		_float4 vDest;
		XMStoreFloat4(&vDest, vRayPos + vRayDir * fDist);
		return vDest;
	}

	return _float4(0.f,0.f,0.f,1);
}

CCell* CCell::Picking_Cell()
{
	return nullptr;
}

#ifdef _DEBUG
HRESULT CCell::Render(CShader* pShader)
{
	if (nullptr == m_pVIBuffer)
		return E_FAIL;

	_float4x4		WorldMatrix;
	XMStoreFloat4x4(&WorldMatrix, XMMatrixIdentity());

	pShader->Set_Matrix(L"g_WorldMatrix", &WorldMatrix);
	pShader->Set_Matrix(L"g_ViewMatrix", &CGameInstance::GetInstance()->Get_TransformFloat4x4(CPipeLine::D3DTS_VIEW));
	pShader->Set_Matrix(L"g_ProjMatrix", &CGameInstance::GetInstance()->Get_TransformFloat4x4(CPipeLine::D3DTS_PROJ));

	pShader->Begin(0);

	m_pVIBuffer->Render();

	for (_uint i = 0; i < (_uint)POINT_END; ++i)
		m_pVIBufferCircleCom[i]->Render();

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

	for (_uint i = 0; i < (_uint)POINT_END; ++i)
		Safe_Release(m_pVIBufferCircleCom[i]);
#endif

}
