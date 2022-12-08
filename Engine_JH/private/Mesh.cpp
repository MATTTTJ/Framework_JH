#include "..\public\Mesh.h"

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
{
}

HRESULT CMesh::Initialize_Prototype(CModel::MODELTYPE eType, aiMesh * pAIMesh, CModel* pModel)
{
	m_pAIMesh = pAIMesh;
	m_eType = eType;

	FAILED_CHECK_RETURN(__super::Initialize_Prototype(), E_FAIL);

	m_iMaterialIndex = pAIMesh->mMaterialIndex;
	m_strName = pAIMesh->mName.C_Str();
	m_iNumVertexBuffers = 1;
	m_iNumVertices = pAIMesh->mNumVertices;
	m_iNumPrimitives = pAIMesh->mNumFaces;
	m_eTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	m_eIndexFormat = DXGI_FORMAT_R32_UINT;
	m_iIndicesSizePerPrimitive = sizeof(FACEINDICES32);
	m_iNumIndicesPerPrimitive = 3;
	m_iNumIndices = m_iNumIndicesPerPrimitive * m_iNumPrimitives;

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

	ZeroMemory(&m_BufferDesc, sizeof m_BufferDesc);

	m_BufferDesc.ByteWidth = m_iIndicesSizePerPrimitive * m_iNumPrimitives;
	m_BufferDesc.Usage = D3D11_USAGE_DEFAULT;
	m_BufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	m_BufferDesc.StructureByteStride = 0;
	m_BufferDesc.CPUAccessFlags = 0;
	m_BufferDesc.MiscFlags = 0;

	FACEINDICES32*		pIndices = new FACEINDICES32[m_iNumPrimitives];
	ZeroMemory(pIndices, sizeof(FACEINDICES32) * m_iNumPrimitives);

	for (_uint i = 0; i < m_iNumPrimitives; ++i)
	{
		pIndices[i]._0 = pAIMesh->mFaces[i].mIndices[0];
		pIndices[i]._1 = pAIMesh->mFaces[i].mIndices[1];
		pIndices[i]._2 = pAIMesh->mFaces[i].mIndices[2];
	}

	ZeroMemory(&m_SubResourceData, sizeof m_SubResourceData);
	m_SubResourceData.pSysMem = pIndices;

	if (FAILED(__super::Create_IndexBuffer()))
		return E_FAIL;

	Safe_Delete_Array(pIndices);


#pragma endregion

	return S_OK;
}

HRESULT CMesh::Initialize_Clone(void * pArg)
{
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
		NULL_CHECK_RETURN(pBone, );

		m_iNumMeshBones = 1;
		m_vecMeshBones.push_back(pBone);
		Safe_AddRef(pBone);
	}
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
	ZeroMemory(&m_BufferDesc, sizeof m_BufferDesc);

	m_BufferDesc.ByteWidth = m_iStride * m_iNumVertices;
	m_BufferDesc.Usage = D3D11_USAGE_DEFAULT;
	m_BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_BufferDesc.StructureByteStride = m_iStride;
	m_BufferDesc.CPUAccessFlags = 0;
	m_BufferDesc.MiscFlags = 0;

	VTXMODEL*			pVertices = new VTXMODEL[m_iNumVertices];
	ZeroMemory(pVertices, sizeof(VTXMODEL));

	_matrix PivotMatrix = pModel->Get_PivotMatrix();

	for (_uint i = 0; i < m_iNumVertices; ++i)
	{
		memcpy(&pVertices[i].vPosition, &pAIMesh->mVertices[i], sizeof(_float3));
		XMStoreFloat3(&pVertices[i].vPosition, XMVector3TransformCoord(XMLoadFloat3(&pVertices[i].vPosition), PivotMatrix));

		memcpy(&pVertices[i].vNormal, &pAIMesh->mNormals[i], sizeof(_float3));
		XMStoreFloat3(&pVertices[i].vNormal, XMVector3TransformNormal(XMLoadFloat3(&pVertices[i].vNormal), PivotMatrix));

		memcpy(&pVertices[i].vTexUV, &pAIMesh->mTextureCoords[0][i], sizeof(_float2));
		memcpy(&pVertices[i].vTangent, &pAIMesh->mTangents[i], sizeof(_float3));
	}

	ZeroMemory(&m_SubResourceData, sizeof m_SubResourceData);
	m_SubResourceData.pSysMem = pVertices;

	FAILED_CHECK_RETURN(__super::Create_VertexBuffer(), E_FAIL)

	Safe_Delete_Array(pVertices);

	return S_OK;
}

HRESULT CMesh::Ready_VertexBuffer_AnimModel(aiMesh* pAIMesh, CModel* pModel)
{
	m_iStride = sizeof(VTXANIMMODEL);
	ZeroMemory(&m_BufferDesc, sizeof m_BufferDesc);

	m_BufferDesc.ByteWidth = m_iStride * m_iNumVertices;
	m_BufferDesc.Usage = D3D11_USAGE_DEFAULT;
	m_BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_BufferDesc.StructureByteStride = m_iStride;
	m_BufferDesc.CPUAccessFlags = 0;
	m_BufferDesc.MiscFlags = 0;

	VTXANIMMODEL*			pVertices = new VTXANIMMODEL[m_iNumVertices];
	ZeroMemory(pVertices, sizeof(VTXANIMMODEL) * m_iNumVertices);

	for (_uint i = 0; i < m_iNumVertices; ++i)
	{
		memcpy(&pVertices[i].vPosition, &pAIMesh->mVertices[i], sizeof(_float3));
		memcpy(&pVertices[i].vNormal, &pAIMesh->mNormals[i], sizeof(_float3));
		memcpy(&pVertices[i].vTexUV, &pAIMesh->mTextureCoords[0][i], sizeof(_float2));
		memcpy(&pVertices[i].vTangent, &pAIMesh->mTangents[i], sizeof(_float3));
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

			if (fabs(pVertices[iVertexIndex].vBlendWeight.x - 0.f) < EPSILON)
			{
				pVertices[iVertexIndex].vBlendIndex.x = i;
				pVertices[iVertexIndex].vBlendWeight.x = pAIBone->mWeights[j].mWeight;
			}

			else if (fabs(pVertices[iVertexIndex].vBlendWeight.y - 0.f) < EPSILON)
			{
				pVertices[iVertexIndex].vBlendIndex.y = i;
				pVertices[iVertexIndex].vBlendWeight.y = pAIBone->mWeights[j].mWeight;
			}

			else if (fabs(pVertices[iVertexIndex].vBlendWeight.z - 0.f) < EPSILON)
			{
				pVertices[iVertexIndex].vBlendIndex.z = i;
				pVertices[iVertexIndex].vBlendWeight.z = pAIBone->mWeights[j].mWeight;
			}

			else if (fabs(pVertices[iVertexIndex].vBlendWeight.w - 0.f) < EPSILON)
			{
				pVertices[iVertexIndex].vBlendIndex.w = i;
				pVertices[iVertexIndex].vBlendWeight.w = pAIBone->mWeights[j].mWeight;
			}
		}
	}
	ZeroMemory(&m_SubResourceData, sizeof m_SubResourceData);
	m_SubResourceData.pSysMem = pVertices;

	FAILED_CHECK_RETURN(__super::Create_VertexBuffer(), E_FAIL);

	Safe_Delete_Array(pVertices);

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

CComponent * CMesh::Clone(void * pArg)
{
	CMesh*		pInstance = new CMesh(*this);

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSG_BOX("Failed to Cloned : CMesh");
		Safe_Release(pInstance);
	}

	return pInstance;
}


void CMesh::Free()
{
	__super::Free();

	for(auto& pBone : m_vecMeshBones)
		Safe_Release(pBone);

	m_vecMeshBones.clear();
}
