#include "stdafx.h"
#include "..\public\VIBuffer_Rect_Instancing.h"

CVIBuffer_Rect_Instancing::CVIBuffer_Rect_Instancing(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CVIBuffer_Instancing(pDevice, pContext)
{
}

CVIBuffer_Rect_Instancing::CVIBuffer_Rect_Instancing(const CVIBuffer_Rect_Instancing & rhs)
	: CVIBuffer_Instancing(rhs)
	, m_pSpeeds(rhs.m_pSpeeds)
{

}

HRESULT CVIBuffer_Rect_Instancing::Initialize_Prototype(_uint iNumInstance)
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	m_pSpeeds = new _double[iNumInstance];

	for (_uint i = 0; i < iNumInstance; ++i)
		m_pSpeeds[i] = rand() % 5 + 1.0;

	m_iNumInstance = iNumInstance;
	m_iIndexCountPerInstance = 6;
	m_iNumVertexBuffers = 2;
	m_iStride = sizeof(VTXTEX);
	m_iNumVertices = 4;
	m_iNumPrimitive = 2 * iNumInstance;
	m_eTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	m_eIndexFormat = DXGI_FORMAT_R16_UINT;
	m_iIndicesSizePerPrimitive = sizeof(FACEINDICES16);
	m_iNumIndicesPerPrimitive = 3;
	m_iNumIndices = m_iNumIndicesPerPrimitive * m_iNumPrimitive;

#pragma region VERTEX_BUFFER

	ZeroMemory(&m_tBufferDesc, sizeof m_tBufferDesc);

	m_tBufferDesc.ByteWidth = m_iStride * m_iNumVertices;
	m_tBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	m_tBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_tBufferDesc.StructureByteStride = m_iStride;
	m_tBufferDesc.CPUAccessFlags = 0;
	m_tBufferDesc.MiscFlags = 0;

	VTXTEX*			pVertices = new VTXTEX[m_iNumVertices];
	ZeroMemory(pVertices, sizeof(VTXTEX));

	pVertices[0].vPosition = _float3(-0.5f, 0.5f, 0.5f);
	pVertices[0].vTexUV = _float2(0.0f, 0.f);

	pVertices[1].vPosition = _float3(0.5f, 0.5f, 0.5f);
	pVertices[1].vTexUV = _float2(1.0f, 0.f);

	pVertices[2].vPosition = _float3(0.5f, -0.5f, 0.5f);
	pVertices[2].vTexUV = _float2(1.0f, 1.f);

	pVertices[3].vPosition = _float3(-0.5f, -0.5f, 0.5f);
	pVertices[3].vTexUV = _float2(0.0f, 1.f);

	ZeroMemory(&m_tSubResourceData, sizeof m_tSubResourceData);
	m_tSubResourceData.pSysMem = pVertices;

	if (FAILED(__super::Create_VertexBuffer()))
		return E_FAIL;

	Safe_Delete_Array(pVertices);

#pragma endregion

#pragma region INDEX_BUFFER

	ZeroMemory(&m_tBufferDesc, sizeof m_tBufferDesc);

	m_tBufferDesc.ByteWidth = m_iIndicesSizePerPrimitive * m_iNumPrimitive;
	m_tBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	m_tBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	m_tBufferDesc.StructureByteStride = 0;
	m_tBufferDesc.CPUAccessFlags = 0;
	m_tBufferDesc.MiscFlags = 0;

	FACEINDICES16*		pIndices = new FACEINDICES16[m_iNumPrimitive];
	ZeroMemory(pIndices, sizeof(FACEINDICES16) * m_iNumPrimitive);

	_uint		iNumFaces = 0;

	for (_uint i = 0; i < iNumInstance; ++i)
	{
		pIndices[iNumFaces]._0 = 0;
		pIndices[iNumFaces]._1 = 1;
		pIndices[iNumFaces]._2 = 2;
		++iNumFaces;

		pIndices[iNumFaces]._0 = 0;
		pIndices[iNumFaces]._1 = 2;
		pIndices[iNumFaces]._2 = 3;
		++iNumFaces;
	}

	

	ZeroMemory(&m_tSubResourceData, sizeof m_tSubResourceData);
	m_tSubResourceData.pSysMem = pIndices;

	if (FAILED(__super::Create_IndexBuffer()))
		return E_FAIL;

	Safe_Delete_Array(pIndices);
#pragma endregion


#pragma region INSTANCE_BUFFER

	m_iInstanceStride = sizeof(VTXMATRIX);

	ZeroMemory(&m_tBufferDesc, sizeof m_tBufferDesc);

	m_tBufferDesc.ByteWidth = m_iInstanceStride * iNumInstance;
	m_tBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	m_tBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_tBufferDesc.StructureByteStride = m_iInstanceStride;
	m_tBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	m_tBufferDesc.MiscFlags = 0;

	VTXMATRIX*			pInstanceVertices = new VTXMATRIX[iNumInstance];
	ZeroMemory(pInstanceVertices, sizeof(VTXMATRIX));

	for (_uint i = 0; i < iNumInstance; ++i)
	{
		pInstanceVertices[i].vRight = _float4(1.0f, 0.f, 0.f, 0.f);
		pInstanceVertices[i].vUp = _float4(0.0f, 1.f, 0.f, 0.f);
		pInstanceVertices[i].vLook = _float4(0.0f, 0.f, 1.f, 0.f);
		pInstanceVertices[i].vPosition = _float4(_float(rand() % 5), 0.f, _float(rand() % 5), 1.f);
	}
	
	ZeroMemory(&m_tSubResourceData, sizeof m_tSubResourceData);
	m_tSubResourceData.pSysMem = pInstanceVertices;

	m_pDevice->CreateBuffer(&m_tBufferDesc, &m_tSubResourceData, &m_pInstanceBuffer);

	Safe_Delete_Array(pInstanceVertices);
#pragma endregion

	return S_OK;
}

HRESULT CVIBuffer_Rect_Instancing::Initialize_Clone(CGameObject* pOwner, void * pArg)
{
	return S_OK;
}

HRESULT CVIBuffer_Rect_Instancing::Tick(_double TimeDelta)
{
	D3D11_MAPPED_SUBRESOURCE			SubResource;
	ZeroMemory(&SubResource, sizeof(D3D11_MAPPED_SUBRESOURCE));

	m_pContext->Map(m_pInstanceBuffer, 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &SubResource);

	for (_uint i = 0; i < m_iNumInstance; ++i)	
	{
		((VTXMATRIX*)SubResource.pData)[i].vPosition.y += (_float)m_pSpeeds[i] * (_float)TimeDelta;

		if (((VTXMATRIX*)SubResource.pData)[i].vPosition.y > 3.f)
			((VTXMATRIX*)SubResource.pData)[i].vPosition.y = 0.f;
	}	

	m_pContext->Unmap(m_pInstanceBuffer, 0);

	return S_OK;
}

HRESULT CVIBuffer_Rect_Instancing::Render()
{
	if (nullptr == m_pContext)
		return E_FAIL;

	/* 정점버퍼들을 장치에 바인딩한다.(복수를 바인딩한다.)  */

	ID3D11Buffer*			pVertexBuffers[] = {
		m_pVB,
		m_pInstanceBuffer
	};

	_uint					iStrides[] = {
		m_iStride,
		m_iInstanceStride,
	};

	_uint					iOffsets[] = {
		0,
		0,
	};

	m_pContext->IASetVertexBuffers(0, m_iNumVertexBuffers, pVertexBuffers, iStrides, iOffsets);

	/* 인덱스버퍼를 장치에 바인딩한다.(단일로 바인딩한다.)  */
	m_pContext->IASetIndexBuffer(m_pIB, m_eIndexFormat, 0);

	m_pContext->IASetPrimitiveTopology(m_eTopology);

	/*m_pContext->DrawIndexed(m_iNumIndices, 0, 0);*/
	m_pContext->DrawIndexedInstanced(m_iIndexCountPerInstance, m_iNumInstance, 0, 0, 0);

	return S_OK;
}

CVIBuffer_Rect_Instancing * CVIBuffer_Rect_Instancing::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, _uint iNumInstance)
{
	CVIBuffer_Rect_Instancing*		pInstance = new CVIBuffer_Rect_Instancing(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype(iNumInstance)))
	{
		MSG_BOX("Failed to Created : CVIBuffer_Rect_Instancing");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CComponent * CVIBuffer_Rect_Instancing::Clone(CGameObject* pOwner, void * pArg)
{
	CVIBuffer_Rect_Instancing*		pInstance = new CVIBuffer_Rect_Instancing(*this);

	if (FAILED(pInstance->Initialize_Clone(pOwner, pArg)))
	{
		MSG_BOX("Failed to Cloned : CVIBuffer_Rect_Instancing");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CVIBuffer_Rect_Instancing::Free()
{
	__super::Free();

	if (false == m_bIsCloned)
	{
		Safe_Delete_Array(m_pSpeeds);
	}
}
