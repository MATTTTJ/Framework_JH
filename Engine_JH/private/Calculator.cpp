// #include "..\public\Calculator.h"
//
// #include "PipeLine.h"
// #include "VIBuffer_Terrain.h"
// #include "Transform.h"
//
// CCalculator::CCalculator(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
// 	:CComponent(pDevice, pContext)
// {
// }
//
// CCalculator::CCalculator(const CCalculator& rhs)
// 	:CComponent(rhs)
// {
// }
//
// HRESULT CCalculator::Ready_Calculator()
// {
// 	return S_OK;
// }
//
// _float CCalculator::HeightOnTerrain(_float3 pPos, const _float3* pTerrainVtxPos, const _ulong& dwCntX,
// 	const _ulong& dwCntZ, const _ulong& dwVtxItv)
// {
// 		_ulong	dwIndex = _ulong(pPos->z / dwVtxItv) * dwCntX + _ulong(pPos->x / dwVtxItv);

	// _float	fWidth = (pPos->x - pTerrainVtxPos[dwIndex + dwCntX].x) / dwVtxItv;
	// _float	fHeight = (pTerrainVtxPos[dwIndex + dwCntX].z - pPos->z) / dwVtxItv;
	//
	// D3DXPLANE		Plane;
	//
	// // 오른쪽 위 삼각형
	// if (fWidth > fHeight)
	// {
	// 	D3DXPlaneFromPoints(&Plane,
	// 		&pTerrainVtxPos[dwIndex + dwCntX],
	// 		&pTerrainVtxPos[dwIndex + dwCntX + 1],
	// 		&pTerrainVtxPos[dwIndex + 1]);
	//
	// }
	// // 왼쪽 아래 삼각형
	// else
	// {
	// 	D3DXPlaneFromPoints(&Plane,
	// 		&pTerrainVtxPos[dwIndex + dwCntX],
	// 		&pTerrainVtxPos[dwIndex + 1],
	// 		&pTerrainVtxPos[dwIndex]);
	// }
	//
	// //ax + by + cz + d = 0;
	//
	// //by = -ax - cz - d
	// // y = (-ax - cz - d) / b
	//
	// pPos->y = (-Plane.a * pPos->x - Plane.c * pPos->z - Plane.d) / Plane.b;
	//
	// return pPos->y;
// }
//
// _float3 CCalculator::PickingOnTerrain(HWND hWnd, const CVIBuffer_Terrain* pTerrainBufferCom,
// 	const CTransform* pTerrainTransformCom)
// {
	// POINT		ptMouse{};
	//
	// GetCursorPos(&ptMouse);
	// ScreenToClient(hWnd, &ptMouse);
	//
	// _vec3		vPoint;
	//
	// D3DVIEWPORT9		ViewPort;
	// ZeroMemory(&ViewPort, sizeof(D3DVIEWPORT9));
	// m_pGraphicDev->GetViewport(&ViewPort);
	//
	// // 뷰포트 -> 투영
	// vPoint.x = ptMouse.x / (ViewPort.Width * 0.5f) - 1.f;
	// vPoint.y = ptMouse.y / -(ViewPort.Height * 0.5f) + 1.f;
	// vPoint.z = 0.f;
	//
	// // 투영 -> 뷰 스페이스
	// _matrix		matProj;
	//
	// m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);
	// D3DXMatrixInverse(&matProj, nullptr, &matProj);
	// D3DXVec3TransformCoord(&vPoint, &vPoint, &matProj);
	//
	// _vec3	vRayDir, vRayPos;		// 뷰 스페이스 영역에 있는 상태
	//
	// vRayPos = { 0.f, 0.f, 0.f };
	// vRayDir = vPoint - vRayPos;
	//
	// // 뷰 스페이스 -> 월드
	//
	// _matrix		matView;
	// m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	// D3DXMatrixInverse(&matView, nullptr, &matView);
	// D3DXVec3TransformCoord(&vRayPos, &vRayPos, &matView);
	// D3DXVec3TransformNormal(&vRayDir, &vRayDir, &matView);
	//
	// // 월드 -> 로컬
	// _matrix		matWorld;
	//
	// pTerrainTransformCom->Get_WorldMatrix(&matWorld);
	// D3DXMatrixInverse(&matWorld, nullptr, &matWorld);
	// D3DXVec3TransformCoord(&vRayPos, &vRayPos, &matWorld);
	// D3DXVec3TransformNormal(&vRayDir, &vRayDir, &matWorld);
	//
	// const _vec3*	pTerrainVtx = pTerrainBufferCom->Get_VtxPos();
	//
	// _ulong		dwVtxCntX = pTerrainBufferCom->Get_VtxCntX();
	// _ulong		dwVtxCntZ = pTerrainBufferCom->Get_VtxCntZ();
	//
	// _ulong	dwVtxIdx[3]{};
	// _float	fU, fV, fDist;
	//
	// for (_ulong i = 0; i < dwVtxCntZ - 1; ++i)
	// {
	// 	for (_ulong j = 0; j < dwVtxCntX - 1; ++j)
	// 	{
	// 		_ulong dwIndex = i * dwVtxCntX + j;
	//
	// 		// 오른쪽 위
	// 		dwVtxIdx[0] = dwIndex + dwVtxCntX;
	// 		dwVtxIdx[1] = dwIndex + dwVtxCntX + 1;
	// 		dwVtxIdx[2] = dwIndex + 1;
	//
	// 		if (D3DXIntersectTri(&pTerrainVtx[dwVtxIdx[1]],
	// 			&pTerrainVtx[dwVtxIdx[0]],
	// 			&pTerrainVtx[dwVtxIdx[2]],
	// 			&vRayPos, &vRayDir,
	// 			&fU, &fV, &fDist))
	// 		{
	// 			return _vec3(pTerrainVtx[dwVtxIdx[1]].x + (pTerrainVtx[dwVtxIdx[0]].x - pTerrainVtx[dwVtxIdx[1]].x) * fU,
	// 				0.f,
	// 				pTerrainVtx[dwVtxIdx[1]].z + (pTerrainVtx[dwVtxIdx[2]].z - pTerrainVtx[dwVtxIdx[1]].z) * fV);
	// 		}
	//
	// 		// 왼쪽 아래
	// 		dwVtxIdx[0] = dwIndex + dwVtxCntX;
	// 		dwVtxIdx[1] = dwIndex + 1;
	// 		dwVtxIdx[2] = dwIndex;
	//
	// 		if (D3DXIntersectTri(&pTerrainVtx[dwVtxIdx[2]],
	// 			&pTerrainVtx[dwVtxIdx[1]],
	// 			&pTerrainVtx[dwVtxIdx[0]],
	// 			&vRayPos, &vRayDir,
	// 			&fU, &fV, &fDist))
	// 		{
	// 			return _vec3(pTerrainVtx[dwVtxIdx[2]].x + (pTerrainVtx[dwVtxIdx[1]].x - pTerrainVtx[dwVtxIdx[2]].x) * fU,
	// 				0.f,
	// 				pTerrainVtx[dwVtxIdx[2]].z + (pTerrainVtx[dwVtxIdx[0]].z - pTerrainVtx[dwVtxIdx[2]].z) * fV);
	// 		}
	// 	}
	// }
	//
	// return _vec3(0.f, 0.f, 0.f);
// }
//
// CComponent* CCalculator::Clone(void* pArg)
// {
// }
//
// CCalculator* CCalculator::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
// {
// }
//
// void CCalculator::Free()
// {
// 	__super::Free();
// }
//
//
