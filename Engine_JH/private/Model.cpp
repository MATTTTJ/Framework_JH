#include "..\public\Model.h"
#include "Mesh.h"
#include "Shader.h"
#include "Texture.h"
#include "Bone.h"
#include "Animation.h"

CModel::CModel(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CComponent(pDevice, pContext)
{
}

CModel::CModel(const CModel & rhs)
	: CComponent(rhs)
	, m_pAIScene(rhs.m_pAIScene)
	, m_eType(rhs.m_eType)
	, m_iNumMeshes(rhs.m_iNumMeshes)
	, m_Meshes(rhs.m_Meshes)
	, m_Materials(rhs.m_Materials)
	, m_iNumMaterials(rhs.m_iNumMaterials)
	, m_Bones(rhs.m_Bones)
	, m_iNumBones(rhs.m_iNumBones)
	, m_Animations(rhs.m_Animations)
	, m_iNumAnimation(rhs.m_iNumAnimation)
	, m_iCurrentAnimIndex(rhs.m_iCurrentAnimIndex)
{
	for (auto& pMesh : m_Meshes)
		Safe_AddRef(pMesh);

	for(auto& Material : m_Materials)
	{
		for(_uint i = 0; i<AI_TEXTURE_TYPE_MAX; ++i)
		{
			Safe_AddRef(Material.pTexture[i]);
		}
	}

	for (auto& pBone : m_Bones)
		Safe_AddRef(pBone);

	for (auto& pAnimation : m_Animations)
		Safe_AddRef(pAnimation);

}

CBone* CModel::Get_BonePtr(const char* pBoneName)
{
	auto iter = find_if(m_Bones.begin(),m_Bones.end(),[&](CBone* pBone)->_bool
	{
		return !strcmp(pBoneName, pBone->Get_Name());
	});

	if (iter == m_Bones.end())
		return nullptr;
	return *iter;
}



HRESULT CModel::Initialize_Prototype(TYPE eType, const char * pModelFilePath)
{
	_uint			iFlag = 0;

	if (TYPE_NONANIM == eType)
		iFlag = aiProcess_PreTransformVertices | aiProcess_ConvertToLeftHanded | aiProcessPreset_TargetRealtime_Fast;
	else
		iFlag = aiProcess_ConvertToLeftHanded | aiProcessPreset_TargetRealtime_Fast;

	m_pAIScene = m_Importer.ReadFile(pModelFilePath, iFlag);

	NULL_CHECK_RETURN(m_pAIScene, E_FAIL)

	FAILED_CHECK_RETURN(Ready_Bones(m_pAIScene->mRootNode), E_FAIL)

	FAILED_CHECK_RETURN(Ready_MeshContainers(), E_FAIL)

	FAILED_CHECK_RETURN(Ready_Materials(pModelFilePath), E_FAIL)

	FAILED_CHECK_RETURN(Ready_Animation(), E_FAIL)

	return S_OK;
}

HRESULT CModel::Initialize_Clone(void * pArg)
{
	return S_OK;
}

void CModel::Play_Animation(_double TimeDelta)
{
	// ���� �ִϸ��̼ǿ� �´� ������ TransformMatrix�� �����Ѵ�.
	m_Animations[m_iCurrentAnimIndex]->Update_Bones(TimeDelta);

	for (auto& pBone : m_Bones)
	{
		if (nullptr != pBone)
		{
			pBone->compute_CombindTransformationMatrix();
		}
	}
}

HRESULT CModel::Bind_Material(CShader* pShader, _uint iMeshIndex, aiTextureType eType, const char* pConstantName)
{
	if (iMeshIndex >= m_iNumMeshes)
		return E_FAIL;

	_uint iMaterialIndex = m_Meshes[iMeshIndex]->Get_MaterialIndex();

	if (iMaterialIndex >= m_iNumMaterials)
		return E_FAIL;

	if (nullptr != m_Materials[iMaterialIndex].pTexture[eType])
	{
		m_Materials[iMaterialIndex].pTexture[eType]->Bind_ShaderResource(pShader, pConstantName);
	}
	else
		return E_FAIL;

	return S_OK;
}

HRESULT CModel::Render(CShader* pShader, _uint iMeshIndex)
{	 
	pShader->Begin(0);

	if (nullptr != m_Meshes[iMeshIndex])
		m_Meshes[iMeshIndex]->Render();

	return S_OK;


	// if(nullptr != m_Meshes[iMeshIndex])
	// {
	// 	if(nullptr!= pBoneConstantName)
	// 	{
	// 		_float4x4		BoneMatrices[256];
	//
	// 		m_Meshes[iMeshIndex]->SetUp_BoneMatrix(BoneMatrices);
	//
	// 		pShader->Set_MatrixArray(pBoneConstantName, BoneMatrices, 256);
	// 	}
	// }
	//
	// pShader->Begin(0);
	// m_Meshes[iMeshIndex]->Render();
}

HRESULT CModel::Ready_Bones(aiNode* pAINode)
{
	CBone*	pBone = CBone::Create(pAINode);
	NULL_CHECK_RETURN(pBone, E_FAIL)

	m_Bones.push_back(pBone);

	for(_uint i =0; i<pAINode->mNumChildren; ++i)
	{
		Ready_Bones(pAINode->mChildren[i]);
	}

	return S_OK;
}

HRESULT CModel::Ready_MeshContainers()
{
	NULL_CHECK_RETURN(m_pAIScene, E_FAIL)

	m_iNumMeshes = m_pAIScene->mNumMeshes;

	for (_uint i = 0; i < m_iNumMeshes; ++i)
	{
		aiMesh*		pAIMesh = m_pAIScene->mMeshes[i];

		CMesh*		pMesh = CMesh::Create(m_pDevice, m_pContext, m_eType, pAIMesh, this);
		NULL_CHECK_RETURN(pMesh, E_FAIL)

		m_Meshes.push_back(pMesh);
	}

	return S_OK;
}

HRESULT CModel::Ready_Materials(const char* pModelFilePath)
{
	char		szDirectory[MAX_PATH] = "";

	_splitpath_s(pModelFilePath, nullptr, 0, szDirectory, MAX_PATH, nullptr, 0, nullptr, 0);

	/* �ؽ��Ŀ� ��ϵǾ��ִ� �ȼ����� ���� ������ �ε��Ѵ�. */
	m_iNumMaterials = m_pAIScene->mNumMaterials;

	for (_uint i = 0; i < m_iNumMaterials; ++i)
	{
		char		szTextureFileName[MAX_PATH] = "";
		char		szExt[MAX_PATH] = "";
		char		szTexturePath[MAX_PATH] = "";

		MODELMATERIAL			ModelMaterial;
		ZeroMemory(&ModelMaterial, sizeof(ModelMaterial));

		aiMaterial*		pAIMaterial = m_pAIScene->mMaterials[i];

		for (_uint j = 0; j < AI_TEXTURE_TYPE_MAX; ++j)
		{
			/*j == 0 = > aiTextureType_NONE;
			j == 1 = > aiTextureType_DIFFUSE;
			j == 2 = > aiTextureType_SPECULAR;*/

			aiString			strTexturePath;

			if (FAILED(pAIMaterial->GetTexture(aiTextureType(j), 0, &strTexturePath)))
				continue;

			_splitpath_s(strTexturePath.data, nullptr, 0, nullptr, 0, szTextureFileName, MAX_PATH, szExt, MAX_PATH);

			strcpy_s(szTexturePath, szDirectory);
			strcat_s(szTexturePath, szTextureFileName);
			strcat_s(szTexturePath, szExt);

			_tchar			szFullPath[MAX_PATH] = TEXT("");

			MultiByteToWideChar(CP_ACP, 0, szTexturePath, (_int)strlen(szTexturePath), szFullPath, MAX_PATH);

			ModelMaterial.pTexture[j] = CTexture::Create(m_pDevice, m_pContext, szFullPath);
			NULL_CHECK_RETURN(ModelMaterial.pTexture[j], E_FAIL);
		}

		m_Materials.push_back(ModelMaterial);
	}
	return S_OK;
}

HRESULT CModel::Ready_Animation()
{
	// Assimp ���� ����ü���� �ִϸ��̼��� ������ �޾ƿ´�.
	m_iNumAnimation = m_pAIScene->mNumAnimations;
	// �ִϸ��̼��� ������ŭ Loop�� ���鼭 �ִϸ��̼��� �����ϴ� �۾�
	// Model���� ���� �����ϱ� ������ ���� ã�� ���ؼ� ���� �ּҵ� �־��ش�. 
	for(_uint i =0; i<m_iNumAnimation; ++i)
	{
		aiAnimation*		pAnimation = m_pAIScene->mAnimations[i];

		CAnimation*			pAnim = CAnimation::Create(pAnimation, this);
		NULL_CHECK_RETURN(pAnim, E_FAIL)

		m_Animations.push_back(pAnim);
	}

	return S_OK;
}

CModel * CModel::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, TYPE eType, const char * pModelFilePath)
{
	CModel*		pInstance = new CModel(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype(eType, pModelFilePath)))
	{
		MSG_BOX("Failed to Created : CModel");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CComponent * CModel::Clone(void * pArg)
{
	CModel*		pInstance = new CModel(*this);

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSG_BOX("Failed to Cloned : CModel");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CModel::Free()
{
	__super::Free();

	for (auto& Material : m_Materials)
	{
		for (_uint i = 0; i<AI_TEXTURE_TYPE_MAX; ++i)
		{
			Safe_Release(Material.pTexture[i]);
		}
	}

	for (auto& pMesh : m_Meshes)
		Safe_Release(pMesh);
	m_Meshes.clear();

	for (auto& pBone : m_Bones)
		Safe_Release(pBone);
	m_Bones.clear();

	for (auto& pAnimation : m_Animations)
		Safe_Release(pAnimation);
	m_Animations.clear();

	m_Importer.FreeScene();
}