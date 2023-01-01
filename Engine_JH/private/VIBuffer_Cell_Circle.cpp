#include "stdafx.h"
#include "..\public\VIBuffer_Cell_Circle.h"

CVIBuffer_Cell_Circle::CVIBuffer_Cell_Circle(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CVIBuffer(pDevice, pContext)
{
}

CVIBuffer_Cell_Circle::CVIBuffer_Cell_Circle(const CVIBuffer_Cell_Circle& rhs)
	: CVIBuffer(rhs)
{
}

HRESULT CVIBuffer_Cell_Circle::Initialize_Prototype(const _float3& vPoint)
{
	FAILED_CHECK_RETURN(__super::Initialize_Prototype(), E_FAIL);

	/* Initialize Variable */
	m_eTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	m_iStride = sizeof(VTXPOS);
	m_iNumPrimitive = 10;
	m_iNumVertices = 11;
	m_iNumVertexBuffers = 1;
	m_eIndexFormat = DXGI_FORMAT_R16_UINT;
	m_iNumIndicesPerPrimitive = 3;
	m_iIndicesSizePerPrimitive = sizeof(FACEINDICES16);
	m_iNumIndices = m_iNumIndicesPerPrimitive * m_iNumPrimitive;

	/* Initialize Vertex Buffer */
	ZeroMemory(&m_tBufferDesc, sizeof(D3D11_BUFFER_DESC));
	m_tBufferDesc.ByteWidth = m_iStride * m_iNumVertices;
	m_tBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	m_tBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_tBufferDesc.StructureByteStride = m_iStride;
	m_tBufferDesc.CPUAccessFlags = 0;
	m_tBufferDesc.MiscFlags = 0;

	VTXPOS*	pVertices = new VTXPOS[m_iNumVertices];
	ZeroMemory(pVertices, sizeof(VTXPOS) * m_iNumVertices);

	pVertices[0].vPosition = vPoint;
	pVertices[1].vPosition = { pVertices[0].vPosition.x, pVertices[0].vPosition.y, pVertices[0].vPosition.z + 0.15f };

	_matrix	matTrans = XMMatrixTranslation(pVertices[0].vPosition.x, pVertices[0].vPosition.y, pVertices[0].vPosition.z);
	_matrix	matTransInv = XMMatrixInverse(nullptr, matTrans);

	for (_uint i = 2; i < m_iNumVertices; ++i)
	{
		_float		fAngle = XMConvertToRadians(36.f * (i - 1));
		if (fAngle > XM_PI)
			fAngle = -(XM_2PI - fAngle);
		_matrix	matRot = matTransInv * XMMatrixRotationY(fAngle);
		XMStoreFloat3(&pVertices[i].vPosition, XMVector3TransformCoord(XMLoadFloat3(&pVertices[1].vPosition), matRot * matTrans));
	}

	ZeroMemory(&m_tSubResourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	m_tSubResourceData.pSysMem = pVertices;

	FAILED_CHECK_RETURN(__super::Create_VertexBuffer(), E_FAIL);

	Safe_Delete_Array(pVertices);

	/* Initialize Index Buffer */
	ZeroMemory(&m_tBufferDesc, sizeof(D3D11_BUFFER_DESC));
	m_tBufferDesc.ByteWidth = m_iIndicesSizePerPrimitive * m_iNumPrimitive;
	m_tBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	m_tBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	m_tBufferDesc.StructureByteStride = 0;
	m_tBufferDesc.CPUAccessFlags = 0;
	m_tBufferDesc.MiscFlags = 0;

	FACEINDICES16*	pIndices = new FACEINDICES16[m_iNumPrimitive];
	ZeroMemory(pIndices, sizeof(FACEINDICES16) * m_iNumPrimitive);

	pIndices[0]._0 = 0;
	pIndices[0]._1 = 1;
	pIndices[0]._2 = 2;

	pIndices[1]._0 = 0;
	pIndices[1]._1 = 2;
	pIndices[1]._2 = 3;

	pIndices[2]._0 = 0;
	pIndices[2]._1 = 3;
	pIndices[2]._2 = 4;

	pIndices[3]._0 = 0;
	pIndices[3]._1 = 4;
	pIndices[3]._2 = 5;

	pIndices[4]._0 = 0;
	pIndices[4]._1 = 5;
	pIndices[4]._2 = 6;

	pIndices[5]._0 = 0;
	pIndices[5]._1 = 6;
	pIndices[5]._2 = 7;

	pIndices[6]._0 = 0;
	pIndices[6]._1 = 7;
	pIndices[6]._2 = 8;

	pIndices[7]._0 = 0;
	pIndices[7]._1 = 8;
	pIndices[7]._2 = 9;

	pIndices[8]._0 = 0;
	pIndices[8]._1 = 9;
	pIndices[8]._2 = 10;

	pIndices[9]._0 = 0;
	pIndices[9]._1 = 10;
	pIndices[9]._2 = 1;

	ZeroMemory(&m_tSubResourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	m_tSubResourceData.pSysMem = pIndices;

	FAILED_CHECK_RETURN(__super::Create_IndexBuffer(), E_FAIL);

	Safe_Delete_Array(pIndices);

	return S_OK;
}

HRESULT CVIBuffer_Cell_Circle::Initialize_Clone(CGameObject* pOwner, void* pArg)
{
	FAILED_CHECK_RETURN(__super::Initialize_Clone(pOwner, pArg), E_FAIL);

	return S_OK;
}

CVIBuffer_Cell_Circle* CVIBuffer_Cell_Circle::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext,
	const _float3& vPoint)
{
	CVIBuffer_Cell_Circle*		pInstance = new CVIBuffer_Cell_Circle(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype(vPoint)))
	{
		MSG_BOX("Failed to Create : CVIBuffer_Cell_Circle");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CComponent* CVIBuffer_Cell_Circle::Clone(CGameObject* pOwner, void* pArg)
{
	CVIBuffer_Cell_Circle*		pInstance = new CVIBuffer_Cell_Circle(*this);

	if (FAILED(pInstance->Initialize_Clone(pOwner, pArg)))
	{
		MSG_BOX("Failed to Clone : CVIBuffer_Cell_Circle");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CVIBuffer_Cell_Circle::Free()
{
	__super::Free();
}
