#include "..\public\VIBuffer_Terrain.h"

CVIBuffer_Terrain::CVIBuffer_Terrain(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CVIBuffer(pDevice, pContext)
{

}

CVIBuffer_Terrain::CVIBuffer_Terrain(const CVIBuffer_Terrain & rhs)
	: CVIBuffer(rhs)
	, m_pVertices(rhs.m_pVertices)
	, m_iNumVerticesX(rhs.m_iNumVerticesX)
	, m_iNumVerticesZ(rhs.m_iNumVerticesZ)
{

}

HRESULT CVIBuffer_Terrain::Initialize_Prototype(const _tchar* pHeightMapFilePath)
{

	FAILED_CHECK_RETURN(__super::Initialize_Prototype(), E_FAIL);

	_ulong		dwByte = 0;
	HANDLE		hFile = CreateFile(pHeightMapFilePath, GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if (0 == hFile)
		return E_FAIL;

	BITMAPFILEHEADER			fh;
	BITMAPINFOHEADER			ih;
	_ulong*						pPixel = nullptr;

	ReadFile(hFile, &fh, sizeof fh, &dwByte, nullptr);
	ReadFile(hFile, &ih, sizeof ih, &dwByte, nullptr);

	m_iNumVerticesX = ih.biWidth;
	m_iNumVerticesZ = ih.biHeight;
	m_iNumVertices = m_iNumVerticesX * m_iNumVerticesZ;

	pPixel = new _ulong[m_iNumVertices];
	ZeroMemory(pPixel, sizeof(_ulong) * m_iNumVertices);

	ReadFile(hFile, pPixel, sizeof(_ulong) * m_iNumVertices, &dwByte, nullptr);

	CloseHandle(hFile);

	m_iNumVertexBuffers = 1;
	m_iStride = sizeof(VTXNORTEX);

	m_iNumPrimitives = (m_iNumVerticesX - 1) * (m_iNumVerticesZ - 1) * 2;
	m_eTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	m_eIndexFormat = DXGI_FORMAT_R32_UINT;
	m_iIndicesSizePerPrimitive = sizeof(FACEINDICES32);
	m_iNumIndicesPerPrimitive = 3;
	m_iNumIndices = m_iNumIndicesPerPrimitive * m_iNumPrimitives;

#pragma region VERTEX_BUFFER
	m_pVertices = new VTXNORTEX[m_iNumVertices];
	ZeroMemory(m_pVertices, sizeof(VTXNORTEX));

	for (_uint i = 0; i < m_iNumVerticesZ; ++i)
	{
		for (_uint j = 0; j < m_iNumVerticesX; ++j)
		{
			_uint			iIndex = i * m_iNumVerticesX + j;

			//	11111111 11111011 11111011 11111011
			//& 00000000 00000000 00000000 11111111

			m_pVertices[iIndex].vPosition = _float3(j, (pPixel[iIndex] & 0x000000ff) / 10.f, i);
			m_pVertices[iIndex].vNormal = _float3(0.f, 0.f, 0.f);
			m_pVertices[iIndex].vTexUV = _float2(j / (m_iNumVerticesX - 1.0f), i / (m_iNumVerticesZ - 1.0f));
		}
	}
	
	Safe_Delete_Array(pPixel);

#pragma endregion



#pragma region INDEX_BUFFER
	FACEINDICES32*		pIndices = new FACEINDICES32[m_iNumPrimitives];
	ZeroMemory(pIndices, sizeof(FACEINDICES32) * m_iNumPrimitives);

	_uint		iNumFaces = 0;

	for (_uint i = 0; i < m_iNumVerticesZ - 1; ++i)
	{
		for (_uint j = 0; j < m_iNumVerticesX - 1; ++j)
		{
			_uint			iIndex = i * m_iNumVerticesX + j;

			_uint			iIndices[4] = {
				iIndex + m_iNumVerticesX,
				iIndex + m_iNumVerticesX + 1,
				iIndex + 1,
				iIndex
			};

			_vector			vSour, vDest, vNormal;

			pIndices[iNumFaces]._0 = iIndices[0];
			pIndices[iNumFaces]._1 = iIndices[1];
			pIndices[iNumFaces]._2 = iIndices[2];

			vSour = XMLoadFloat3(&m_pVertices[pIndices[iNumFaces]._1].vPosition) - XMLoadFloat3(&m_pVertices[pIndices[iNumFaces]._0].vPosition);
			vDest = XMLoadFloat3(&m_pVertices[pIndices[iNumFaces]._2].vPosition) - XMLoadFloat3(&m_pVertices[pIndices[iNumFaces]._1].vPosition);
			vNormal = XMVector3Normalize(XMVector3Cross(vSour, vDest));

			XMStoreFloat3(&m_pVertices[pIndices[iNumFaces]._0].vNormal, XMVector3Normalize(XMLoadFloat3(&m_pVertices[pIndices[iNumFaces]._0].vNormal) + vNormal));
			XMStoreFloat3(&m_pVertices[pIndices[iNumFaces]._1].vNormal, XMVector3Normalize(XMLoadFloat3(&m_pVertices[pIndices[iNumFaces]._1].vNormal) + vNormal));
			XMStoreFloat3(&m_pVertices[pIndices[iNumFaces]._2].vNormal, XMVector3Normalize(XMLoadFloat3(&m_pVertices[pIndices[iNumFaces]._2].vNormal) + vNormal));

			++iNumFaces;

			pIndices[iNumFaces]._0 = iIndices[0];
			pIndices[iNumFaces]._1 = iIndices[2];
			pIndices[iNumFaces]._2 = iIndices[3];

			vSour = XMLoadFloat3(&m_pVertices[pIndices[iNumFaces]._1].vPosition) - XMLoadFloat3(&m_pVertices[pIndices[iNumFaces]._0].vPosition);
			vDest = XMLoadFloat3(&m_pVertices[pIndices[iNumFaces]._2].vPosition) - XMLoadFloat3(&m_pVertices[pIndices[iNumFaces]._1].vPosition);
			vNormal = XMVector3Normalize(XMVector3Cross(vSour, vDest));

			XMStoreFloat3(&m_pVertices[pIndices[iNumFaces]._0].vNormal, XMVector3Normalize(XMLoadFloat3(&m_pVertices[pIndices[iNumFaces]._0].vNormal) + vNormal));
			XMStoreFloat3(&m_pVertices[pIndices[iNumFaces]._1].vNormal, XMVector3Normalize(XMLoadFloat3(&m_pVertices[pIndices[iNumFaces]._1].vNormal) + vNormal));
			XMStoreFloat3(&m_pVertices[pIndices[iNumFaces]._2].vNormal, XMVector3Normalize(XMLoadFloat3(&m_pVertices[pIndices[iNumFaces]._2].vNormal) + vNormal));

			++iNumFaces;
		}
	}

#pragma endregion


	ZeroMemory(&m_BufferDesc, sizeof m_BufferDesc);

	m_BufferDesc.ByteWidth = m_iStride * m_iNumVertices;
	m_BufferDesc.Usage = D3D11_USAGE_DEFAULT;
	m_BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_BufferDesc.StructureByteStride = m_iStride;
	m_BufferDesc.CPUAccessFlags = 0;
	m_BufferDesc.MiscFlags = 0;

	ZeroMemory(&m_SubResourceData, sizeof m_SubResourceData);
	m_SubResourceData.pSysMem = m_pVertices;

	FAILED_CHECK_RETURN(__super::Create_VertexBuffer(), E_FAIL);

	ZeroMemory(&m_BufferDesc, sizeof m_BufferDesc);

	m_BufferDesc.ByteWidth = m_iIndicesSizePerPrimitive * m_iNumPrimitives;
	m_BufferDesc.Usage = D3D11_USAGE_DEFAULT;
	m_BufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	m_BufferDesc.StructureByteStride = 0;
	m_BufferDesc.CPUAccessFlags = 0;
	m_BufferDesc.MiscFlags = 0;

	ZeroMemory(&m_SubResourceData, sizeof m_SubResourceData);
	m_SubResourceData.pSysMem = pIndices;

	FAILED_CHECK_RETURN(__super::Create_IndexBuffer(), E_FAIL);

	Safe_Delete_Array(pIndices);

	return S_OK;
}

HRESULT CVIBuffer_Terrain::Initialize_Clone(void * pArg)
{
	FAILED_CHECK_RETURN(__super::Initialize_Clone(pArg), E_FAIL);

	return S_OK;
}

_float4 CVIBuffer_Terrain::PickingOnTerrain(HWND hwnd, CTransform* pTerrainTransformCom, _matrix WorldMatrix)
{
	POINT	ptMouse{};

	GetCursorPos(&ptMouse);
	ScreenToClient(hwnd, &ptMouse);

	_float4 vOrigin;
	_float4 vAt;

	D3D11_VIEWPORT	ViewPort;
	ZeroMemory(&ViewPort, sizeof(D3D11_VIEWPORT));
	ViewPort.Width = 1280;
	ViewPort.Height = 720;
	ViewPort.MinDepth = 0.0f;
	ViewPort.MaxDepth = 1.0f;
	ViewPort.TopLeftX = 0;
	ViewPort.TopLeftY = 0;

	_uint itmp = 1;

	m_pContext->RSGetViewports(&itmp, &ViewPort);

	vOrigin.x = (_float)ptMouse.x / ((_float)ViewPort.Width * 0.5f) - 1.f;
	vOrigin.y = (_float)ptMouse.y / -((_float)ViewPort.Height * 0.5f) + 1.f;
	vOrigin.z = 0.f;
	vOrigin.w = 1.f;

	vAt.x = vOrigin.x;
	vAt.y = vOrigin.y;
	vAt.z = 1.f;
	vAt.w = 1.f;

	_vector vectorAt = XMLoadFloat4(&vAt);
	_vector vectorOrigin = XMLoadFloat4(&vOrigin);

	CPipeLine*	pPipeline = GET_INSTANCE(CPipeLine)

	_matrix matProjInverse = pPipeline->Get_TransformMatrix_Inverse(CPipeLine::D3DTS_PROJ);
	XMVector4Transform(vectorOrigin, matProjInverse);
	XMVector4Transform(vectorAt, matProjInverse);

	// 뷰스페이스 -> 월드
	_matrix  matView = pPipeline->Get_TransformMatrix_Inverse(CPipeLine::D3DTS_VIEW);
	XMVector4Transform(vectorOrigin, matView);
	XMVector4Transform(vectorAt, matView);

	// 월드 -> 로컬
	_matrix matWorld = pTerrainTransformCom->Get_WorldMatrix();
	matWorld = XMMatrixInverse(nullptr, matWorld);
	XMVector4Transform(vectorOrigin, matWorld);
	XMVector4Transform(vectorAt, matWorld);

	_vector vRayDir;
	vRayDir = vectorAt - vectorOrigin;
	vRayDir = XMVector4Normalize(vRayDir);

	_ulong	dwVtxCntX = m_iNumVerticesX;
	_ulong	dwVtxCntZ = m_iNumVerticesZ;

	_ulong	dwVtxIdx[3] {};
	_float  fDist = 0.f;

	for (_ulong i = 0; i < dwVtxCntZ - 1; ++i)
	{
		for (_ulong j = 0; j < dwVtxCntX - 1; ++j)
		{
			_ulong dwIndex = i * dwVtxCntX + j;

			// 오른쪽 위
			dwVtxIdx[0] = dwIndex + dwVtxCntX;
			dwVtxIdx[1] = dwIndex + dwVtxCntX + 1;
			dwVtxIdx[2] = dwIndex + 1;

			_float4 v0 = _float4(m_pVertices[dwVtxIdx[0]].vPosition.x, m_pVertices[dwVtxIdx[0]].vPosition.y, m_pVertices[dwVtxIdx[0]].vPosition.z, 1.f);
			_vector _v0 = XMLoadFloat4(&v0);
			_float4 v1 = _float4(m_pVertices[dwVtxIdx[1]].vPosition.x, m_pVertices[dwVtxIdx[1]].vPosition.y, m_pVertices[dwVtxIdx[1]].vPosition.z, 1.f);
			_vector _v1 = XMLoadFloat4(&v1);
			_float4 v2 = _float4(m_pVertices[dwVtxIdx[2]].vPosition.x, m_pVertices[dwVtxIdx[2]].vPosition.y, m_pVertices[dwVtxIdx[2]].vPosition.z, 1.f);
			_vector _v2 = XMLoadFloat4(&v2);

			if (TriangleTests::Intersects(vectorOrigin, vRayDir, 
				_v0, _v1,_v2, fDist))
			{
				_float4 vDest;
				XMStoreFloat4(&vDest, vectorOrigin + vRayDir * fDist);
				return vDest;
			}

			//왼쪽 아래
			dwVtxIdx[0] = dwIndex + dwVtxCntX;
			dwVtxIdx[1] = dwIndex + 1;
			dwVtxIdx[2] = dwIndex;
			
			v0 = _float4(m_pVertices[dwVtxIdx[0]].vPosition.x, m_pVertices[dwVtxIdx[0]].vPosition.y, m_pVertices[dwVtxIdx[0]].vPosition.z, 1.f);
			_v0 = XMLoadFloat4(&v0);
			v1 = _float4(m_pVertices[dwVtxIdx[1]].vPosition.x, m_pVertices[dwVtxIdx[1]].vPosition.y, m_pVertices[dwVtxIdx[1]].vPosition.z, 1.f);
			_v1 = XMLoadFloat4(&v1);
			v2 = _float4(m_pVertices[dwVtxIdx[2]].vPosition.x, m_pVertices[dwVtxIdx[2]].vPosition.y, m_pVertices[dwVtxIdx[2]].vPosition.z, 1.f);
			_v2 = XMLoadFloat4(&v2);

			if (TriangleTests::Intersects(vectorOrigin, vRayDir,
											_v0, _v1,_v2, fDist))
			{
				_float4 vDest;
				XMStoreFloat4(&vDest, vectorOrigin + vRayDir * fDist);
				return vDest;
			}
			RELEASE_INSTANCE(CPipeLine)
		}
	}

	return _float4(0.f, 0.f, 0.f,0.f);


}


CVIBuffer_Terrain * CVIBuffer_Terrain::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, const _tchar* pHeightMapFilePath)
{
	CVIBuffer_Terrain*		pInstance = new CVIBuffer_Terrain(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype(pHeightMapFilePath)))
	{
		MSG_BOX("Failed to Created : CVIBuffer_Terrain");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CComponent * CVIBuffer_Terrain::Clone(void * pArg)
{
	CVIBuffer_Terrain*		pInstance = new CVIBuffer_Terrain(*this);

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSG_BOX("Failed to Cloned : CVIBuffer_Terrain");
		Safe_Release(pInstance);
	}
	return pInstance;
}


void CVIBuffer_Terrain::Free()
{

	__super::Free();

	if (!m_bIsCloned)
		Safe_Delete_Array(m_pVertices);

}
