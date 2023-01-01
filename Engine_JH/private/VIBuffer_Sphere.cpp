#include "stdafx.h"
#include "..\public\VIBuffer_Sphere.h"

CVIBuffer_Sphere::CVIBuffer_Sphere(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CVIBuffer(pDevice,pContext)
{
}

CVIBuffer_Sphere::CVIBuffer_Sphere(const CVIBuffer_Sphere& rhs)
	:CVIBuffer(rhs)
{
}

HRESULT CVIBuffer_Sphere::Initialize_Prototype()
{
	FAILED_CHECK_RETURN(__super::Initialize_Prototype(), E_FAIL);

	m_iNumVertexBuffers = 1;
	m_iStride = sizeof(VTXTEX);
	m_eTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	m_eIndexFormat = DXGI_FORMAT_R32_UINT;
	m_iIndicesSizePerPrimitive = sizeof(FACEINDICES32);
	m_iNumIndicesPerPrimitive = 3;

	m_iNumVertices = 0;
	m_iNumPrimitive = 0;

	float fRadius = 10.f;
	unsigned __int32 iStackCount = 20;
	unsigned __int32 iSliceCount = 20;

	vector<VTXTEX> vecVtxSphere;
	VTXTEX v;

	v.vPosition = _float3(0.f, fRadius, 0.f);
	v.vTexUV = _float2(0.5f, 0.f);

	vecVtxSphere.push_back(v);
	m_iNumVertices++;

	float fStackAngle = XM_PI / iStackCount;
	float fSliceAngle = (XM_PI * 2.f) / iSliceCount;

	float fDeltaU = 1.f / static_cast<float>(iSliceCount);
	float fDeltaV = 1.f / static_cast<float>(iStackCount);

	for (unsigned __int32 y = 1; y <= iStackCount - 1; ++y)
	{
		float fPi = y * fStackAngle;

		for (unsigned __int32 x = 0; x <= iSliceCount; ++x)
		{
			float fTheta = x * fSliceAngle;

			v.vPosition.x = fRadius * sinf(fPi) * cosf(fTheta);
			v.vPosition.y = fRadius * cosf(fPi);
			v.vPosition.z = fRadius * sinf(fPi) * sinf(fTheta);

			v.vTexUV = _float2(fDeltaU * x, fDeltaV * y);

			vecVtxSphere.push_back(v);
			m_iNumVertices++;
		}
	}

	v.vPosition = _float3(0.f, -fRadius, 0.f);
	v.vTexUV = _float2(0.5f, 1.f);

	vecVtxSphere.push_back(v);
	m_iNumVertices++;

	vector<FACEINDICES32> vecIdx;

	for (unsigned __int32 i = 0; i <= iSliceCount; ++i)
	{
		vecIdx.push_back({ 0, i + 2, i + 1 });
		m_iNumPrimitive++;
	}

	unsigned __int32 iRingVertexCount = iSliceCount + 1;
	for (unsigned __int32 y = 0; y < iStackCount - 2; ++y)
	{
		for (unsigned __int32 x = 0; x < iSliceCount; ++x)
		{
			vecIdx.push_back({ 1 + (y)* iRingVertexCount + (x),
				1 + (y)* iRingVertexCount + (x + 1),
				1 + (y + 1) * iRingVertexCount + (x) });

			vecIdx.push_back({ 1 + (y + 1) * iRingVertexCount + (x),
				1 + (y)* iRingVertexCount + (x + 1),
				1 + (y + 1) * iRingVertexCount + (x + 1) });

			m_iNumPrimitive += 2;
		}
	}

	unsigned __int32 iBottomIndex = static_cast<unsigned __int32>(vecVtxSphere.size()) - 1;
	unsigned __int32 iLastRingStartIndex = iBottomIndex - iRingVertexCount;

	for (unsigned __int32 i = 0; i < iSliceCount; ++i)
	{
		vecIdx.push_back({ iBottomIndex,
			iLastRingStartIndex + i ,
			iLastRingStartIndex + i + 1 });

		m_iNumPrimitive++;
	}

	m_iNumIndices = m_iNumIndicesPerPrimitive * m_iNumPrimitive;

	ZeroMemory(&m_tBufferDesc, sizeof m_tBufferDesc);

	m_tBufferDesc.ByteWidth = m_iStride * m_iNumVertices;
	m_tBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	m_tBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_tBufferDesc.StructureByteStride = m_iStride;
	m_tBufferDesc.CPUAccessFlags = 0;
	m_tBufferDesc.MiscFlags = 0;

	VTXTEX*         pVertices = new VTXTEX[m_iNumVertices];
	ZeroMemory(pVertices, sizeof(VTXTEX));

	memcpy(pVertices, vecVtxSphere.data(), sizeof(VTXTEX) * vecVtxSphere.size());

	ZeroMemory(&m_tSubResourceData, sizeof m_tSubResourceData);
	m_tSubResourceData.pSysMem = pVertices;

	if (FAILED(__super::Create_VertexBuffer()))
		return E_FAIL;

	ZeroMemory(&m_tBufferDesc, sizeof m_tBufferDesc);

	m_tBufferDesc.ByteWidth = m_iIndicesSizePerPrimitive * m_iNumPrimitive;
	m_tBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	m_tBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	m_tBufferDesc.StructureByteStride = 0;
	m_tBufferDesc.CPUAccessFlags = 0;
	m_tBufferDesc.MiscFlags = 0;

	FACEINDICES32*      pIndices = new FACEINDICES32[m_iNumPrimitive];
	ZeroMemory(pIndices, sizeof(FACEINDICES32) * m_iNumPrimitive);

	memcpy(pIndices, vecIdx.data(), sizeof(FACEINDICES32) * vecIdx.size());

	ZeroMemory(&m_tSubResourceData, sizeof m_tSubResourceData);
	m_tSubResourceData.pSysMem = pIndices;

	if (FAILED(__super::Create_IndexBuffer()))
		return E_FAIL;

	Safe_Delete_Array(pVertices);
	Safe_Delete_Array(pIndices);

	return S_OK;
}

HRESULT CVIBuffer_Sphere::Initialize_Clone(CGameObject* pOwner, void* pArg)
{
	FAILED_CHECK_RETURN(__super::Initialize_Clone(pOwner, pArg), E_FAIL);

	return S_OK;
}

CVIBuffer_Sphere* CVIBuffer_Sphere::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CVIBuffer_Sphere*		pInstance = new CVIBuffer_Sphere(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Create : CVIBuffer_Sphere");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CComponent* CVIBuffer_Sphere::Clone(CGameObject* pOwner, void* pArg)
{
	CVIBuffer_Sphere*		pInstance = new CVIBuffer_Sphere(*this);

	if (FAILED(pInstance->Initialize_Clone(pOwner, pArg)))
	{
		MSG_BOX("Failed to Clone : CVIBuffer_Sphere");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CVIBuffer_Sphere::Free()
{
	__super::Free();
}
