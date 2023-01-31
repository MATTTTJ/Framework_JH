#include "stdafx.h"
#include "..\public\VIBuffer_Point.h"

CVIBuffer_Point::CVIBuffer_Point(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CVIBuffer(pDevice, pContext)
{
}

CVIBuffer_Point::CVIBuffer_Point(const CVIBuffer_Point& rhs)
	: CVIBuffer(rhs)
	, m_pVBInstance(rhs.m_pVBInstance)
	, m_iNumInstance(rhs.m_iNumInstance)
	, m_iInstanceStride(rhs.m_iInstanceStride)
{
	Safe_AddRef(m_pVBInstance);
}

HRESULT CVIBuffer_Point::Initialize_Prototype()
{
#pragma region VERTEXBUFFER
	m_iStride = sizeof(VTXPOINT);
	m_iNumVertices = 1;
	m_iNumVertexBuffers = 2;

	ZeroMemory(&m_tBufferDesc, sizeof(D3D11_BUFFER_DESC));
	m_tBufferDesc.ByteWidth = m_iStride * m_iNumVertices;
	m_tBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	m_tBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_tBufferDesc.StructureByteStride = m_iStride;
	m_tBufferDesc.CPUAccessFlags = 0;
	m_tBufferDesc.MiscFlags = 0;

	VTXPOINT*			pVertices = new VTXPOINT;

	pVertices->vPosition = _float3(0.f, 0.f, 0.f);
	pVertices->vPSize = _float2(1.0f, 1.f);

	ZeroMemory(&m_tSubResourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	m_tSubResourceData.pSysMem = pVertices;

	if (FAILED(__super::Create_VertexBuffer()))
		return E_FAIL;

	Safe_Delete_Array(pVertices);


#pragma endregion


#pragma region INSTANCEBUFFER
	m_iInstanceStride = sizeof(VTXINSTANCE);
	m_iNumInstance = 1;

	ZeroMemory(&m_tBufferDesc, sizeof(D3D11_BUFFER_DESC));
	m_tBufferDesc.ByteWidth = m_iInstanceStride * m_iNumInstance;
	m_tBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	m_tBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_tBufferDesc.StructureByteStride = m_iInstanceStride;
	m_tBufferDesc.CPUAccessFlags = 0;
	m_tBufferDesc.MiscFlags = 0;

	VTXINSTANCE*		pInstance = new VTXINSTANCE[m_iNumInstance];

	for (_uint i = 0; i < m_iNumInstance; ++i)
	{
		pInstance[i].vRight = _float4(1.f, 0.f, 0.f, 0.f);
		pInstance[i].vUp = _float4(0.f, 1.f, 0.f, 0.f);
		pInstance[i].vLook = _float4(0.f, 0.f, 1.f, 0.f);
		pInstance[i].vTranslation = _float4(0.f, 0.f, 0.f, 1.f);
	}

	ZeroMemory(&m_tSubResourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	m_tSubResourceData.pSysMem = pInstance;

	if (FAILED(m_pDevice->CreateBuffer(&m_tBufferDesc, &m_tSubResourceData, &m_pVBInstance)))
		return E_FAIL;

	Safe_Delete_Array(pInstance);

#pragma endregion

#pragma region INDEXBUFFER	

#pragma endregion
	m_iStride = sizeof(_ushort);
	m_iNumPrimitive = m_iNumInstance;
	m_iNumIndices = m_iNumInstance;
	m_eIndexFormat = DXGI_FORMAT_R16_UINT;
	m_eTopology = D3D11_PRIMITIVE_TOPOLOGY_POINTLIST;
	
	ZeroMemory(&m_tBufferDesc, sizeof(D3D11_BUFFER_DESC));
	m_tBufferDesc.ByteWidth = m_iStride * m_iNumPrimitive;
	m_tBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	m_tBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	m_tBufferDesc.StructureByteStride = 0;
	m_tBufferDesc.CPUAccessFlags = 0;
	m_tBufferDesc.MiscFlags = 0;

	_ushort*		pIndices = new _ushort[m_iNumPrimitive];
	ZeroMemory(pIndices, sizeof(_ushort) * m_iNumPrimitive);

	ZeroMemory(&m_tSubResourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	m_tSubResourceData.pSysMem = pIndices;

	if (FAILED(Create_IndexBuffer()))
		return E_FAIL;

	Safe_Delete_Array(pIndices);

	return S_OK;
}

HRESULT CVIBuffer_Point::Initialize_Clone(CGameObject* pOwner, void* pArg)
{
	return S_OK;
}

HRESULT CVIBuffer_Point::Render()
{
	if (nullptr == m_pContext)
		return E_FAIL;

	ID3D11Buffer*			pVertexBuffers[] = {
		m_pVB,
		m_pVBInstance
	};

	_uint		iStrides[] = {
		m_iStride,
		m_iInstanceStride
	};

	_uint		iOffsets[] = {
		0,
		0
	};

	m_pContext->IASetVertexBuffers(0, m_iNumVertexBuffers, pVertexBuffers, iStrides, iOffsets);
	m_pContext->IASetIndexBuffer(m_pIB, m_eIndexFormat, 0);
	m_pContext->IASetPrimitiveTopology(m_eTopology);

	/* 6 : 하나의 도형을 그리기위해 사용하는 인덱스의 갯수. 네모라서 여섯개.  */
	m_pContext->DrawIndexedInstanced(1, m_iNumInstance, 0, 0, 0);

	return S_OK;
}

CVIBuffer_Point* CVIBuffer_Point::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CVIBuffer_Point*		pInstance = new CVIBuffer_Point(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CVIBuffer_Point");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CVIBuffer_Point::Clone(CGameObject* pOwner, void* pArg)
{
	CVIBuffer_Point*		pInstance = new CVIBuffer_Point(*this);

	if (FAILED(pInstance->Initialize_Clone(pOwner, pArg)))
	{
		MSG_BOX("Failed to Cloned : CVIBuffer_Point");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CVIBuffer_Point::Free()
{
	__super::Free();

	Safe_Release(m_pVBInstance);
}
