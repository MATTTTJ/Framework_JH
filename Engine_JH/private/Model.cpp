#include "..\public\Model.h"
#include "Mesh.h"
#include "Shader.h"
#include "Texture.h"
#include "Bone.h"
#include "Animation.h"
#include "GameUtils.h"

CModel::CModel(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CComponent(pDevice, pContext)
{
}

CModel::CModel(const CModel & rhs)
	: CComponent(rhs)
	, m_pAIScene(rhs.m_pAIScene)
	, m_eType(rhs.m_eType)
	, m_PivotMatrix(rhs.m_PivotMatrix)
	, m_iNumMeshes(rhs.m_iNumMeshes)
	, m_vecMaterials(rhs.m_vecMaterials)
	, m_iNumMaterials(rhs.m_iNumMaterials)
	, m_iNumBones(rhs.m_iNumBones)
	, m_iNumAnimation(rhs.m_iNumAnimation)
	, m_iCurrentAnimIndex(rhs.m_iCurrentAnimIndex)
{
	for (auto& pMesh : rhs.m_vecMeshes)
		m_vecMeshes.push_back(dynamic_cast<CMesh*>(pMesh->Clone()));

	for(auto& Material : m_vecMaterials)
	{
		for(_uint i = 0; i<AI_TEXTURE_TYPE_MAX; ++i)
			Safe_AddRef(Material.pTexture[i]);
	}
}

_matrix CModel::Get_BoneMatrix(const string& strBoneName)
{
	CBone*	pBone = Get_BonePtr(strBoneName);
	NULL_CHECK_RETURN(pBone, XMMatrixIdentity());

	return pBone->Get_CombindMatrix();
}

_matrix CModel::Get_OffsetMatrix(const string& strBoneName)
{
	CBone* pBone = Get_BonePtr(strBoneName);
	NULL_CHECK_RETURN(pBone, XMMatrixIdentity());

	return pBone->Get_OffsetMatrix();
}

CBone* CModel::Get_BonePtr(const string& strBoneName)
{
	auto iter = find_if(m_vecBones.begin(),m_vecBones.end(),[&](CBone* pBone)->_bool {
		return strBoneName == pBone->Get_Name();
	});

	if (iter == m_vecBones.end())
		return nullptr;

	return *iter;
}

HRESULT CModel::Initialize_Prototype(MODELTYPE eType, const char * pModelFilePath, _fmatrix PivotMatrix)
{
	if (eType == MODELTYPE_END)
		return E_FAIL;

	m_eType = eType;
	_tchar	wszModelFilePath[MAX_PATH] = L"";
	CGameUtils::ctwc(pModelFilePath, wszModelFilePath);
	m_wstrFilePath = wstring(wszModelFilePath);
	XMStoreFloat4x4(&m_PivotMatrix, PivotMatrix);

	_uint			iFlag = 0;

	if (MODEL_NONANIM == eType)
		iFlag = aiProcess_PreTransformVertices | aiProcess_ConvertToLeftHanded | aiProcessPreset_TargetRealtime_Fast;
	else
		iFlag = aiProcess_ConvertToLeftHanded | aiProcessPreset_TargetRealtime_Fast;

	m_pAIScene = m_Importer.ReadFile(pModelFilePath, iFlag);
	NULL_CHECK_RETURN(m_pAIScene, E_FAIL);

	FAILED_CHECK_RETURN(Ready_MeshContainers(), E_FAIL)
	FAILED_CHECK_RETURN(Ready_Materials(pModelFilePath), E_FAIL)

	return S_OK;
}

HRESULT CModel::Initialize_Clone(void * pArg)
{
	FAILED_CHECK_RETURN(Ready_Bones(m_pAIScene->mRootNode, nullptr), E_FAIL);

	for(auto& pMesh : m_vecMeshes)
		pMesh->SetUp_MeshBones(this);

	FAILED_CHECK_RETURN(Ready_Animation(), E_FAIL);

	return S_OK;
}

void CModel::Imgui_RenderProperty()
{
	if (ImGui::CollapsingHeader("Materials"))
	{
		for (size_t i = 0; i < m_iNumMaterials; ++i)
		{
			ImGui::Text("%d", i);
			ImGui::Separator();
			for (_uint j = 0; j < AI_TEXTURE_TYPE_MAX; j++)
			{
				if (m_vecMaterials[i].pTexture[j] != nullptr)
				{
					ImGui::Image(m_vecMaterials[i].pTexture[j]->Get_Texture(), ImVec2(50.f, 50.f));
					ImGui::SameLine();
				}
			}
			ImGui::NewLine();
		}
	}

	if (ImGui::CollapsingHeader("Meshes"))
	{
		for (size_t i = 0; i < m_iNumMeshes; ++i)
		{
			_uint	iMaterialIndex = m_vecMeshes[i]->Get_MaterialIndex();
			ImGui::BulletText("%s", m_vecMeshes[i]->Get_MeshName().c_str());
			ImGui::Separator();

			for (_uint j = 0; j < AI_TEXTURE_TYPE_MAX; j++)
			{
				if (m_vecMaterials[iMaterialIndex].pTexture[j] != nullptr)
				{
					ImGui::SameLine();
					ImGui::Image(m_vecMaterials[iMaterialIndex].pTexture[j]->Get_Texture(), ImVec2(50.f, 50.f));
				}
			}
		}
	}
}

void CModel::Play_Animation(_double TimeDelta)
{
	if (MODEL_NONANIM == m_eType)
		return;

	// 현재 애니메이션에 맞는 뼈들의 TransformMatrix를 갱신한다.
	m_vecAnimations[m_iCurrentAnimIndex]->Update_Bones(TimeDelta);

	for (auto& pBone : m_vecBones)
	{
		if (nullptr != pBone)
			pBone->compute_CombindTransformationMatrix();
	}
}

HRESULT CModel::Bind_Material(CShader* pShader, _uint iMeshIndex, aiTextureType eType, const wstring& pConstantName)
{
	if (iMeshIndex >= m_iNumMeshes)
		return E_FAIL;

	_uint iMaterialIndex = m_vecMeshes[iMeshIndex]->Get_MaterialIndex();

	if (iMaterialIndex >= m_iNumMaterials)
		return E_FAIL;

	if (nullptr != m_vecMaterials[iMaterialIndex].pTexture[eType])
	{
		m_vecMaterials[iMaterialIndex].pTexture[eType]->Bind_ShaderResource(pShader, pConstantName);
	}
	else
		return E_FAIL;

	return S_OK;
}

HRESULT CModel::Render(CShader* pShader, _uint iMeshIndex, const wstring & wstrBoneConstantName)
{	 
	NULL_CHECK_RETURN(pShader, E_FAIL);

	if(nullptr != m_vecMeshes[iMeshIndex])
	{
		if(wstrBoneConstantName != L"")
		{
			_float4x4		BoneMatrices[256];
	
			m_vecMeshes[iMeshIndex]->SetUp_BoneMatrix(BoneMatrices, XMLoadFloat4x4(&m_PivotMatrix));
	
			pShader->Set_MatrixArray(wstrBoneConstantName, BoneMatrices, 256);
		}
	}
	
	pShader->Begin(0);
	m_vecMeshes[iMeshIndex]->Render();

	return S_OK;
}

HRESULT CModel::Ready_Bones(aiNode* pAINode, CBone* pParent)
{
	CBone*	pBone = CBone::Create(pAINode, pParent);
	NULL_CHECK_RETURN(pBone, E_FAIL)

	m_vecBones.push_back(pBone);

	for(_uint i =0; i < pAINode->mNumChildren; ++i)
		Ready_Bones(pAINode->mChildren[i], pBone);

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

		m_vecMeshes.push_back(pMesh);
	}

	return S_OK;
}

HRESULT CModel::Ready_Materials(const char* pModelFilePath)
{
	char		szDirectory[MAX_PATH] = "";

	_splitpath_s(pModelFilePath, nullptr, 0, szDirectory, MAX_PATH, nullptr, 0, nullptr, 0);

	/* 텍스쳐에 기록되어있는 픽셀단위 재질 정보를 로드한다. */
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

		m_vecMaterials.push_back(ModelMaterial);
	}
	return S_OK;
}

HRESULT CModel::Ready_Animation()
{
	// Assimp 상위 구조체에서 애니메이션의 갯수를 받아온다.
	m_iNumAnimation = m_pAIScene->mNumAnimations;
	// 애니메이션의 갯수만큼 Loop를 돌면서 애니메이션을 생성하는 작업
	// Model에서 뼈를 보관하기 떄문에 뼈를 찾기 위해서 모델의 주소도 넣어준다. 
	for(_uint i =0; i<m_iNumAnimation; ++i)
	{
		aiAnimation*		pAnimation = m_pAIScene->mAnimations[i];

		CAnimation*			pAnim = CAnimation::Create(pAnimation, this);
		NULL_CHECK_RETURN(pAnim, E_FAIL)

		m_vecAnimations.push_back(pAnim);
	}

	return S_OK;
}

CModel * CModel::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, MODELTYPE eType, const char * pModelFilePath, _fmatrix PivotMatrix)
{
	CModel*		pInstance = new CModel(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype(eType, pModelFilePath, PivotMatrix)))
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

	for (auto& pMesh : m_vecMeshes)
		Safe_Release(pMesh);
	m_vecMeshes.clear();

	for (auto& Material : m_vecMaterials)
	{
		for (_uint i = 0; i<AI_TEXTURE_TYPE_MAX; ++i)
			Safe_Release(Material.pTexture[i]);
	}
	m_vecMaterials.clear();

	for (auto& pBone : m_vecBones)
		Safe_Release(pBone);
	m_vecBones.clear();

	for (auto& pAnimation : m_vecAnimations)
		Safe_Release(pAnimation);
	m_vecAnimations.clear();

	m_Importer.FreeScene();
}
