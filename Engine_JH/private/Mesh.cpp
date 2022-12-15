#include "stdafx.h"
#include "..\public\Mesh.h"
#include "Model.h"
#include "Bone.h"

CMesh::CMesh(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CVIBuffer(pDevice, pContext)
{
}

CMesh::CMesh(const CMesh & rhs)
	: CVIBuffer(rhs)
	, m_pAIMesh(rhs.m_pAIMesh)
	, m_eType(rhs.m_eType)
	, m_strName(rhs.m_strName)
	, m_iMaterialIndex(rhs.m_iMaterialIndex)
	, m_iNumMeshBones(rhs.m_iNumMeshBones)
	, m_pNonAnimVertices(rhs.m_pNonAnimVertices)
	, m_pAnimVertices(rhs.m_pAnimVertices)
	, m_pIndices(rhs.m_pIndices)
{
}

HRESULT CMesh::Save_Mesh(HANDLE& hFile, DWORD& dwByte)
{
	if (m_eType == CModel::MODELTYPE_END)
		return E_FAIL;

	_uint			iNameLength = (_uint)m_strName.length() + 1;
	const char*	pName = m_strName.c_str();
	WriteFile(hFile, &iNameLength, sizeof(_uint), &dwByte, nullptr);
	WriteFile(hFile, pName, sizeof(char) * iNameLength, &dwByte, nullptr);

	WriteFile(hFile, &m_eType, sizeof(m_eType), &dwByte, nullptr);
	WriteFile(hFile, &m_iMaterialIndex, sizeof(_uint), &dwByte, nullptr);
	WriteFile(hFile, &m_iNumVertices, sizeof(_uint), &dwByte, nullptr);
	WriteFile(hFile, &m_iNumPrimitive, sizeof(_uint), &dwByte, nullptr);
	WriteFile(hFile, &m_iNumMeshBones, sizeof(_uint), &dwByte, nullptr);
	WriteFile(hFile, &m_iStride, sizeof(_uint), &dwByte, nullptr);

	if (m_eType == CModel::MODEL_NONANIM)
	{
		for (_uint i = 0; i < m_iNumVertices; ++i)
			WriteFile(hFile, &m_pNonAnimVertices[i], sizeof(VTXMODEL), &dwByte, nullptr);
	}
	else if (m_eType == CModel::MODEL_ANIM)
	{
		for (_uint i = 0; i < m_iNumVertices; ++i)
			WriteFile(hFile, &m_pAnimVertices[i], sizeof(VTXANIMMODEL), &dwByte, nullptr);
	}

	for (_uint i = 0; i < m_iNumPrimitive; ++i)
		WriteFile(hFile, &m_pIndices[i], sizeof(FACEINDICES32), &dwByte, nullptr);

	return S_OK;
}

HRESULT CMesh::Save_MeshBones(HANDLE& hFile, DWORD& dwByte)
{
	for (auto& pBone : m_vecMeshBones)
		pBone->Save_BoneName(hFile, dwByte);

	return S_OK;
}

HRESULT CMesh::Load_Mesh(HANDLE& hFile, DWORD& dwByte)
{
	if (m_eType == CModel::MODELTYPE_END)
		return E_FAIL;

	_uint			iNameLength = 0;
	ReadFile(hFile, &iNameLength, sizeof(_uint), &dwByte, nullptr);
	char*			pName = new char[iNameLength];
	ReadFile(hFile, pName, sizeof(char) * iNameLength, &dwByte, nullptr);

	m_strName = pName;
	Safe_Delete_Array(pName);

	ReadFile(hFile, &m_eType, sizeof(m_eType), &dwByte, nullptr);
	ReadFile(hFile, &m_iMaterialIndex, sizeof(_uint), &dwByte, nullptr);
	ReadFile(hFile, &m_iNumVertices, sizeof(_uint), &dwByte, nullptr);
	ReadFile(hFile, &m_iNumPrimitive, sizeof(_uint), &dwByte, nullptr);
	ReadFile(hFile, &m_iNumMeshBones, sizeof(_uint), &dwByte, nullptr);
	ReadFile(hFile, &m_iStride, sizeof(_uint), &dwByte, nullptr);

	m_iNumVertexBuffers = 1;
	m_eTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	m_eIndexFormat = DXGI_FORMAT_R32_UINT;
	m_iIndicesSizePerPrimitive = sizeof(FACEINDICES32);
	m_iNumIndicesPerPrimitive = 3;
	m_iNumIndices = m_iNumIndicesPerPrimitive * m_iNumPrimitive;

	if (m_eType == CModel::MODEL_NONANIM)
	{
		m_pNonAnimVertices = new VTXMODEL[m_iNumVertices];

		for (_uint i = 0; i < m_iNumVertices; ++i)
			ReadFile(hFile, &m_pNonAnimVertices[i], sizeof(VTXMODEL), &dwByte, nullptr);

		ZeroMemory(&m_tSubResourceData, sizeof(D3D11_SUBRESOURCE_DATA));
		m_tSubResourceData.pSysMem = m_pNonAnimVertices;
	}
	else if (m_eType == CModel::MODEL_ANIM)
	{
		m_pAnimVertices = new VTXANIMMODEL[m_iNumVertices];

		for (_uint i = 0; i < m_iNumVertices; ++i)
			ReadFile(hFile, &m_pAnimVertices[i], sizeof(VTXANIMMODEL), &dwByte, nullptr);

		ZeroMemory(&m_tSubResourceData, sizeof(D3D11_SUBRESOURCE_DATA));
		m_tSubResourceData.pSysMem = m_pAnimVertices;
	}

	ZeroMemory(&m_tBufferDesc, sizeof(D3D11_BUFFER_DESC));
	m_tBufferDesc.ByteWidth = m_iStride * m_iNumVertices;
	m_tBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	m_tBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_tBufferDesc.StructureByteStride = m_iStride;
	m_tBufferDesc.CPUAccessFlags = 0;
	m_tBufferDesc.MiscFlags = 0;

	if (FAILED(__super::Create_VertexBuffer()))
		return E_FAIL;

	ZeroMemory(&m_tBufferDesc, sizeof(D3D11_BUFFER_DESC));
	m_tBufferDesc.ByteWidth = m_iIndicesSizePerPrimitive * m_iNumPrimitive;
	m_tBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	m_tBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	m_tBufferDesc.StructureByteStride = 0;
	m_tBufferDesc.CPUAccessFlags = 0;
	m_tBufferDesc.MiscFlags = 0;

	m_pIndices = new FACEINDICES32[m_iNumPrimitive];

	for (_uint i = 0; i < m_iNumPrimitive; ++i)
		ReadFile(hFile, &m_pIndices[i], sizeof(FACEINDICES32), &dwByte, nullptr);

	ZeroMemory(&m_tSubResourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	m_tSubResourceData.pSysMem = m_pIndices;

	if (FAILED(__super::Create_IndexBuffer()))
		return E_FAIL;

	return S_OK;
}

HRESULT CMesh::Initialize_Prototype(CModel::MODELTYPE eType, aiMesh * pAIMesh, CModel* pModel)
{
	if (CModel::MODELTYPE_END == eType)
		return E_FAIL;

	m_eType = eType;

	if (pAIMesh == nullptr)
		return S_OK;

	m_pAIMesh = pAIMesh;

	FAILED_CHECK_RETURN(__super::Initialize_Prototype(), E_FAIL);

	m_iMaterialIndex = pAIMesh->mMaterialIndex;
	m_strName = pAIMesh->mName.C_Str();
	m_iNumVertexBuffers = 1;
	m_iNumVertices = pAIMesh->mNumVertices;
	m_iNumPrimitive = pAIMesh->mNumFaces;
	m_eTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	m_eIndexFormat = DXGI_FORMAT_R32_UINT;
	m_iIndicesSizePerPrimitive = sizeof(FACEINDICES32);
	m_iNumIndicesPerPrimitive = 3;
	m_iNumIndices = m_iNumIndicesPerPrimitive * m_iNumPrimitive;

#pragma region VERTEX_BUFFER
		if (CModel::MODEL_NONANIM == m_eType)
	{
		FAILED_CHECK_RETURN(Ready_VertexBuffer_NonAnimModel(pAIMesh,pModel), E_FAIL);
	}
	else
	{
		FAILED_CHECK_RETURN(Ready_VertexBuffer_AnimModel(pAIMesh, pModel), E_FAIL);
	}
#pragma endregion

#pragma region INDEX_BUFFER

	ZeroMemory(&m_tBufferDesc, sizeof m_tBufferDesc);

	m_tBufferDesc.ByteWidth = m_iIndicesSizePerPrimitive * m_iNumPrimitive;
	m_tBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	m_tBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	m_tBufferDesc.StructureByteStride = 0;
	m_tBufferDesc.CPUAccessFlags = 0;
	m_tBufferDesc.MiscFlags = 0;

	// FACEINDICES32*		pIndices = new FACEINDICES32[m_iNumPrimitive];
	m_pIndices = new FACEINDICES32[m_iNumPrimitive];
	ZeroMemory(m_pIndices, sizeof(FACEINDICES32) * m_iNumPrimitive);

	for (_uint i = 0; i < m_iNumPrimitive; ++i)
	{
		m_pIndices[i]._0 = pAIMesh->mFaces[i].mIndices[0];
		m_pIndices[i]._1 = pAIMesh->mFaces[i].mIndices[1];
		m_pIndices[i]._2 = pAIMesh->mFaces[i].mIndices[2];
	}

	ZeroMemory(&m_tSubResourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	m_tSubResourceData.pSysMem = m_pIndices;

	if (FAILED(__super::Create_IndexBuffer()))
		return E_FAIL;

	// Safe_Delete_Array(pIndices);


#pragma endregion

	return S_OK;
}

HRESULT CMesh::Initialize_Clone(CGameObject* pOwner, void * pArg)
{
	FAILED_CHECK_RETURN(__super::Initialize_Clone(pOwner, pArg), E_FAIL);

	return S_OK;
}

void CMesh::SetUp_MeshBones(CModel* pModel)
{
	for(_uint i = 0 ; i < m_iNumMeshBones; ++i)
	{
		aiBone* pAIBone = m_pAIMesh->mBones[i];

		CBone*	pBone = pModel->Get_BonePtr(pAIBone->mName.data);
		NULL_CHECK_RETURN(pBone, );

		_float4x4		OffsetMatrix;
		memcpy(&OffsetMatrix, &pAIBone->mOffsetMatrix, sizeof(_float4x4));
		XMStoreFloat4x4(&OffsetMatrix, XMMatrixTranspose(XMLoadFloat4x4(&OffsetMatrix)));

		pBone->Set_OffsetMatrix(OffsetMatrix);

		m_vecMeshBones.push_back(pBone);
		Safe_AddRef(pBone);
	}

	// 애니메이션 메시를 로드하던 중 뼈의 갯수가 없으면 같은 이름의 뼈대위치에 존재하도록
	if(0 == m_iNumMeshBones)
	{
		CBone* pBone = pModel->Get_BonePtr(m_strName);
		if (pBone == nullptr) return;

		m_iNumMeshBones = 1;
		m_vecMeshBones.push_back(pBone);
		Safe_AddRef(pBone);
	}
}

HRESULT CMesh::SetUp_MeshBones(HANDLE& hFile, DWORD& dwByte, CModel* pModel)
{
	for(_uint i =0; i<m_iNumMeshBones; ++i)
	{
		_uint	iBoneNameLength = 0;
		ReadFile(hFile, &iBoneNameLength, sizeof(_uint), &dwByte, nullptr);
		char*		pBoneName = new char[iBoneNameLength];
		ReadFile(hFile, pBoneName, sizeof(char) * iBoneNameLength, &dwByte, nullptr);

		CBone*		pBone = pModel->Get_BonePtr(string(pBoneName));

		Safe_Delete_Array(pBoneName);

		if (pBone == nullptr)
			continue;

		m_vecMeshBones.push_back(pBone);
		Safe_AddRef(pBone);
	}

	return S_OK;
}

void CMesh::SetUp_BoneMatrix(_float4x4* pBoneMatrices, _fmatrix PivotMatrix)
{
	// 뼈를 하나씩 돌면서 순회하기 위한 갯수 

	if (0 == m_iNumMeshBones)
	{
		XMStoreFloat4x4(&pBoneMatrices[0], XMMatrixIdentity());
		return;
	}
	

	_uint iNumBones = 0;

	for(auto& pBone : m_vecMeshBones)
	{
		// BoneMatrix = 오프셋 매트릭스 * 콤바인 매트릭스
		XMStoreFloat4x4(&pBoneMatrices[iNumBones++], pBone->Get_OffsetMatrix() * pBone->Get_CombindMatrix() * PivotMatrix);
	}
}

HRESULT CMesh::Ready_VertexBuffer_NonAnimModel(aiMesh* pAIMesh, CModel* pModel)
{

	m_iStride = sizeof(VTXMODEL);
	ZeroMemory(&m_tBufferDesc, sizeof m_tBufferDesc);

	m_tBufferDesc.ByteWidth = m_iStride * m_iNumVertices;
	m_tBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	m_tBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_tBufferDesc.StructureByteStride = m_iStride;
	m_tBufferDesc.CPUAccessFlags = 0;
	m_tBufferDesc.MiscFlags = 0;

	// VTXMODEL*			pVertices = new VTXMODEL[m_iNumVertices];
	m_pNonAnimVertices = new VTXMODEL[m_iNumVertices];
	ZeroMemory(m_pNonAnimVertices, sizeof(VTXMODEL) * m_iNumVertices);


	_matrix PivotMatrix = pModel->Get_PivotMatrix();

	for (_uint i = 0; i < m_iNumVertices; ++i)
	{
		memcpy(&m_pNonAnimVertices[i].vPosition, &pAIMesh->mVertices[i], sizeof(_float3));
		XMStoreFloat3(&m_pNonAnimVertices[i].vPosition, XMVector3TransformCoord(XMLoadFloat3(&m_pNonAnimVertices[i].vPosition), PivotMatrix));

		memcpy(&m_pNonAnimVertices[i].vNormal, &pAIMesh->mNormals[i], sizeof(_float3));
		XMStoreFloat3(&m_pNonAnimVertices[i].vNormal, XMVector3TransformNormal(XMLoadFloat3(&m_pNonAnimVertices[i].vNormal), PivotMatrix));

		memcpy(&m_pNonAnimVertices[i].vTexUV, &pAIMesh->mTextureCoords[0][i], sizeof(_float2));
		memcpy(&m_pNonAnimVertices[i].vTangent, &pAIMesh->mTangents[i], sizeof(_float3));
	}

	ZeroMemory(&m_tSubResourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	m_tSubResourceData.pSysMem = m_pNonAnimVertices;

	FAILED_CHECK_RETURN(__super::Create_VertexBuffer(), E_FAIL)

	// Safe_Delete_Array(pVertices);

	return S_OK;
}

HRESULT CMesh::Ready_VertexBuffer_AnimModel(aiMesh* pAIMesh, CModel* pModel)
{
	m_iStride = sizeof(VTXANIMMODEL);
	ZeroMemory(&m_tBufferDesc, sizeof m_tBufferDesc);

	m_tBufferDesc.ByteWidth = m_iStride * m_iNumVertices;
	m_tBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	m_tBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_tBufferDesc.StructureByteStride = m_iStride;
	m_tBufferDesc.CPUAccessFlags = 0;
	m_tBufferDesc.MiscFlags = 0;

	// VTXANIMMODEL*			pVertices = new VTXANIMMODEL[m_iNumVertices];
	m_pAnimVertices = new VTXANIMMODEL[m_iNumVertices];

	ZeroMemory(m_pAnimVertices, sizeof(VTXANIMMODEL) * m_iNumVertices);


	for (_uint i = 0; i < m_iNumVertices; ++i)
	{
		memcpy(&m_pAnimVertices[i].vPosition, &pAIMesh->mVertices[i], sizeof(_float3));
		memcpy(&m_pAnimVertices[i].vNormal, &pAIMesh->mNormals[i], sizeof(_float3));
		memcpy(&m_pAnimVertices[i].vTexUV, &pAIMesh->mTextureCoords[0][i], sizeof(_float2));
		memcpy(&m_pAnimVertices[i].vTangent, &pAIMesh->mTangents[i], sizeof(_float3));
	}

	/* 메시에 영향ㅇ르 준다.ㅏ */
	m_iNumMeshBones = pAIMesh->mNumBones;

	for (_uint i = 0; i < m_iNumMeshBones; ++i)
	{
		aiBone*		pAIBone = pAIMesh->mBones[i];	

		/* 이 뼈는 몇개의 정점에 영향을 주는가?! */
		_uint iNumWeights = pAIBone->mNumWeights;

		for (_uint j = 0; j < iNumWeights; ++j)
		{
			_uint iVertexIndex = pAIBone->mWeights[j].mVertexId;

			if (fabs(m_pAnimVertices[iVertexIndex].vBlendWeight.x - 0.f) < EPSILON)
			{
				m_pAnimVertices[iVertexIndex].vBlendIndex.x = i;
				m_pAnimVertices[iVertexIndex].vBlendWeight.x = pAIBone->mWeights[j].mWeight;
			}

			else if (fabs(m_pAnimVertices[iVertexIndex].vBlendWeight.y - 0.f) < EPSILON)
			{
				m_pAnimVertices[iVertexIndex].vBlendIndex.y = i;
				m_pAnimVertices[iVertexIndex].vBlendWeight.y = pAIBone->mWeights[j].mWeight;
			}

			else if (fabs(m_pAnimVertices[iVertexIndex].vBlendWeight.z - 0.f) < EPSILON)
			{
				m_pAnimVertices[iVertexIndex].vBlendIndex.z = i;
				m_pAnimVertices[iVertexIndex].vBlendWeight.z = pAIBone->mWeights[j].mWeight;
			}

			else if (fabs(m_pAnimVertices[iVertexIndex].vBlendWeight.w - 0.f) < EPSILON)
			{
				m_pAnimVertices[iVertexIndex].vBlendIndex.w = i;
				m_pAnimVertices[iVertexIndex].vBlendWeight.w = pAIBone->mWeights[j].mWeight;
			}
		}
	}
	ZeroMemory(&m_tSubResourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	m_tSubResourceData.pSysMem = m_pAnimVertices;

	FAILED_CHECK_RETURN(__super::Create_VertexBuffer(), E_FAIL);

	return S_OK;
}

CMesh * CMesh::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext,CModel::MODELTYPE eType, aiMesh * pAIMesh, CModel* pModel)
{
	CMesh*		pInstance = new CMesh(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype(eType, pAIMesh, pModel)))
	{
		MSG_BOX("Failed to Created : CMesh");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CComponent * CMesh::Clone(CGameObject* pOwner, void * pArg)
{
	CMesh*		pInstance = new CMesh(*this);

	if (FAILED(pInstance->Initialize_Clone(pOwner, pArg)))
	{
		MSG_BOX("Failed to Cloned : CMesh");
		Safe_Release(pInstance);
	}

	return pInstance;
}


void CMesh::Free()
{
	__super::Free();

	if(!m_bIsCloned)
	{
		Safe_Delete_Array(m_pNonAnimVertices);
		Safe_Delete_Array(m_pAnimVertices);
		Safe_Delete_Array(m_pIndices);
	}

	for(auto& pBone : m_vecMeshBones)
		Safe_Release(pBone);

	m_vecMeshBones.clear();
}
