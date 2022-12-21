#include "stdafx.h"
#include "..\public\Model.h"
#include "Mesh.h"
#include "Shader.h"
#include "Texture.h"
#include "Bone.h"
#include "Animation.h"
#include "GameInstance.h"
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
	// , m_bIsAnimFinished(true)
	, m_iCurAnimIndex(rhs.m_iCurAnimIndex)
	, m_dwBeginBoneData(rhs.m_dwBeginBoneData)
{
	for (auto& pMesh : rhs.m_vecMeshes)
		m_vecMeshes.push_back(dynamic_cast<CMesh*>(pMesh->Clone(nullptr)));

	for(auto& Material : m_vecMaterials)
	{
		for(_uint i = 0; i<AI_TEXTURE_TYPE_MAX; ++i)
			Safe_AddRef(Material.pTexture[i]);
	}
	m_iLastAnimIndex = 0;


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

void CModel::Set_CurAnimIndex(_uint AnimIndex)
{
	if (AnimIndex < 0 || AnimIndex > m_iNumAnimation || AnimIndex == m_iCurAnimIndex)
		return;

	m_iLastAnimIndex = m_iCurAnimIndex;
	m_iCurAnimIndex = AnimIndex;
	m_vecAnimations[m_iCurAnimIndex]->Reset_Animation();

	if (m_iLastAnimIndex != m_iCurAnimIndex)
		m_fCurAnimChangeTime = 0.f;

	m_bIsAnimChange = true;
}

void CModel::Set_BlendAnimIndex(_uint BlendAnimIndex)
{
	if (BlendAnimIndex < 0 || BlendAnimIndex == m_iCurBlendIndex)
		return;

	m_iLastBlendIndex = m_iCurBlendIndex;
	m_iCurBlendIndex = BlendAnimIndex;
	if (m_iLastBlendIndex != m_iCurBlendIndex)
		m_fCurAnimBlendingTime = 0.f;

}

CAnimation* CModel::Find_Anim(const string& strAnim)
{
	auto itr = find_if(m_vecAnimations.begin(), m_vecAnimations.end(),
		[strAnim](CAnimation* pAnim) {
		return  0 == strcmp(strAnim.c_str() ,pAnim->Get_AnimationName().c_str());
	});

	return *itr;
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

	_tchar			wszExt[32] = L"";
	_wsplitpath_s(m_wstrFilePath.c_str(), nullptr, 0, nullptr, 0, nullptr, 0, wszExt, 32);

	if (!lstrcmp(wszExt, L".fbx"))
	{

		_uint			iFlag = 0;

		if (MODEL_NONANIM == eType)
			iFlag = aiProcess_PreTransformVertices | aiProcess_ConvertToLeftHanded | aiProcessPreset_TargetRealtime_Fast;
		else
			iFlag = aiProcess_ConvertToLeftHanded | aiProcessPreset_TargetRealtime_Fast;

		m_pAIScene = m_Importer.ReadFile(pModelFilePath, iFlag);
		NULL_CHECK_RETURN(m_pAIScene, E_FAIL);

		FAILED_CHECK_RETURN(Ready_MeshContainers(), E_FAIL)
			FAILED_CHECK_RETURN(Ready_Materials(pModelFilePath), E_FAIL)
	}
	else if (!lstrcmp(wszExt, L".model"))
		FAILED_CHECK_RETURN(Load_MeshMaterial(m_wstrFilePath), E_FAIL);

	return S_OK;
}

HRESULT CModel::Initialize_Clone(CGameObject* pOwner, void * pArg)
{
	FAILED_CHECK_RETURN(__super::Initialize_Clone(pOwner, pArg), E_FAIL);


	_tchar	wszExt[32] = L"";
	_wsplitpath_s(m_wstrFilePath.c_str(), nullptr, 0, nullptr, 0, nullptr, 0, wszExt, 32);

	if (!lstrcmp(wszExt, L".fbx"))
	{
		FAILED_CHECK_RETURN(Ready_Bones(m_pAIScene->mRootNode, nullptr), E_FAIL);

		for (auto& pMesh : m_vecMeshes)
			pMesh->SetUp_MeshBones(this);

		FAILED_CHECK_RETURN(Ready_Animation(), E_FAIL);
	}
	else if (!lstrcmp(wszExt, L".model"))
	{
		DWORD	dwByte = 0;
		HANDLE	hFile = CreateFileW(m_wstrFilePath.c_str(), GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
		SetFilePointer(hFile, m_dwBeginBoneData, nullptr, FILE_BEGIN);

		DWORD	Temp = 0;
		ReadFile(hFile, &Temp, sizeof(DWORD), &dwByte, nullptr);

		FAILED_CHECK_RETURN(Load_BoneAnimation(hFile, dwByte), E_FAIL);

		CloseHandle(hFile);
	}

	if (m_eType == MODEL_ANIM && m_pOwner != nullptr)
		CGameInstance::GetInstance()->Add_AnimObject(m_pOwner);
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

void CModel::Imgui_RenderAnimation()
{
	static _int	iSelectAnimation = -1;
	CAnimation*	pAnimation = nullptr;
	char**			ppAnimationTag = new char*[m_iNumAnimation];

	for (_uint i = 0; i < m_iNumAnimation; ++i)
	{
		_uint	iTagLength = (_uint)m_vecAnimations[i]->Get_AnimationName().length() + 1;
		ppAnimationTag[i] = new char[iTagLength];
		sprintf_s(ppAnimationTag[i], sizeof(char) * iTagLength, m_vecAnimations[i]->Get_AnimationName().c_str());
	}

	ImGui::BulletText("Animation List");
	ImGui::ListBox("Animations", &iSelectAnimation, ppAnimationTag, (_int)m_iNumAnimation);

	if (iSelectAnimation != -1)
	{
		static _bool	bReName = false;
		static char	szNewName[MAX_PATH] = "";
		pAnimation = m_vecAnimations[iSelectAnimation];

		if (ImGui::Button("Play"))
		{
			Set_CurAnimIndex(iSelectAnimation);
		}
		ImGui::SameLine();
		if (ImGui::Button("ReName"))
		{
			bReName = true;
		}
		ImGui::SameLine();
		if (ImGui::Button("Cancel"))
		{
			iSelectAnimation = -1;
			bReName = false;
		}

		if (bReName)
		{
			IMGUI_LEFT_LABEL(ImGui::InputText, "Input New Name", szNewName, MAX_PATH);
			if (ImGui::Button("Confirm"))
			{
				pAnimation->Get_AnimationName() = szNewName;
				sprintf_s(szNewName, "");
				bReName = false;
			}
			ImGui::SameLine();
			if (ImGui::Button("UnDo"))
			{
				sprintf_s(szNewName, "");
				bReName = false;
			}
		}
	}

	for (_uint i = 0; i < m_iNumAnimation; ++i)
		Safe_Delete_Array(ppAnimationTag[i]);
	Safe_Delete_Array(ppAnimationTag);

}

void CModel::Play_Animation(_double TimeDelta, _double LerpSpeed, _double AnimSpeed, _bool bFinish)
{
	if (MODEL_NONANIM == m_eType)
		return;

	m_fAnimChangeTime = 0.1;

	if(m_fCurAnimChangeTime < m_fAnimChangeTime && m_bIsAnimChange)
	{		
		m_vecAnimations[m_iLastAnimIndex]->Update_Bones(TimeDelta);
		m_vecAnimations[m_iCurAnimIndex]->Update_Lerp(0.1, m_fCurAnimChangeTime / m_fAnimChangeTime);
		m_fCurAnimChangeTime += (_float)TimeDelta;

	}
	// if(m_fCurAnimBlendingTime < m_fAnimBlendingTime && m_iCurBlendIndex == 0 )
	// {
	// 	m_vecAnimations[m_iCurAnimIndex]->Update_Bones(TimeDelta);
	// 	m_vecAnimations[m_iCurBlendIndex]->Update_Additive(0.1, m_fCurAnimBlendingTime / m_fAnimBlendingTime);
	// 	m_fCurAnimBlendingTime += (_float)TimeDelta;
	// }
	else
	{
		m_bIsAnimFinished = m_vecAnimations[m_iCurAnimIndex]->Update_Bones(TimeDelta);
		m_iLastAnimIndex = m_iCurAnimIndex;
	}

	for (auto& pBone : m_vecBones)
	{
		if (nullptr != pBone)
			pBone->compute_CombindTransformationMatrix();
	}

	// if (m_bIsAnimChange)
	// {
	// 	m_bIsAnimChange = m_vecAnimations[m_iLastAnimIndex]->Update_Lerp(TimeDelta, m_vecAnimations[m_iCurAnimIndex], LerpSpeed, bFinish);
	//
	// 	if (false == m_bIsAnimChange)
	// 		m_iLastAnimIndex = m_iCurAnimIndex;
	// }
	// else
	// 	m_bIsAnimFinished = m_vecAnimations[m_iLastAnimIndex]->Update_Bones(TimeDelta);
	//
	//
	// for (auto& pBone : m_vecBones)
	// {
	// 	if (nullptr != pBone)
	// 		pBone->compute_CombindTransformationMatrix();
	// }
}

HRESULT CModel::Bind_Material(CShader* pShader, _uint iMeshIndex, aiTextureType eType, const wstring& pConstantName)
{
	NULL_CHECK_RETURN(pShader, E_FAIL);

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
		return S_FALSE;

	return S_OK;
}

HRESULT CModel::Render(CShader* pShader, _uint iMeshIndex, const wstring & wstrBoneConstantName, _uint iPassIndex)
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


	pShader->Begin(iPassIndex);

	m_vecMeshes[iMeshIndex]->Render();

	return S_OK;
}

HRESULT CModel::Ready_Bones(aiNode* pAINode, CBone* pParent)
{
	CBone*	pBone = CBone::Create(pAINode, pParent, pAINode->mNumChildren);
	NULL_CHECK_RETURN(pBone, E_FAIL)

	m_vecBones.push_back(pBone);

	for(_uint i =0; i < pAINode->mNumChildren; ++i)
		Ready_Bones(pAINode->mChildren[i], pBone);

	return S_OK;
}

HRESULT CModel::Ready_Bones(HANDLE& hFile, DWORD& dwByte, CBone* pParent)
{
	NULL_CHECK_RETURN(pParent, E_FAIL);

	_uint iNumChild = pParent->Get_NumChild();
	while(iNumChild--)
	{
		CBone*		pBone = CBone::Create(nullptr, pParent, 0);
		NULL_CHECK_RETURN(pBone, E_FAIL);

		pBone->Load_Bone(hFile, dwByte);
		m_vecBones.push_back(pBone);

		FAILED_CHECK_RETURN(Ready_Bones(hFile, dwByte, pBone), E_FAIL);
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
		ZeroMemory(&ModelMaterial, sizeof(MODELMATERIAL));

		aiMaterial*		pAIMaterial = m_pAIScene->mMaterials[i];

		for (_uint j = 0; j < AI_TEXTURE_TYPE_MAX; ++j)
		{
			aiString		strTexturePath;

			if (FAILED(pAIMaterial->GetTexture(aiTextureType(j), 0, &strTexturePath)))
				continue;

			_splitpath_s(strTexturePath.data, nullptr, 0, nullptr, 0, szTextureFileName, MAX_PATH, szExt, MAX_PATH);

			strcpy_s(szTexturePath, szDirectory);
			strcat_s(szTexturePath, szTextureFileName);
			strcat_s(szTexturePath, szExt);

			_tchar		szFullPath[MAX_PATH] = L"";

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

HRESULT CModel::Load_MeshMaterial(const wstring& wstrModelFilePath)
{
	DWORD	dwByte = 0;
	HANDLE	hFile = CreateFileW(wstrModelFilePath.c_str(), GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

	if (hFile == INVALID_HANDLE_VALUE)
		return E_FAIL;

	/* Meshes */
	ReadFile(hFile, &m_iNumMeshes, sizeof(_uint), &dwByte, nullptr);
	m_vecMeshes.reserve(m_iNumMeshes);

	for (_uint i = 0; i < m_iNumMeshes; ++i)
	{
		CMesh*		pMesh = CMesh::Create(m_pDevice, m_pContext, m_eType, nullptr, this);
		NULL_CHECK_RETURN(pMesh, E_FAIL);

		pMesh->Load_Mesh(hFile, dwByte);

		m_vecMeshes.push_back(pMesh);
	}

	/* Materials */
	ReadFile(hFile, &m_iNumMaterials, sizeof(_uint), &dwByte, nullptr);
	m_vecMaterials.reserve(m_iNumMaterials);

	for (_uint i = 0; i < m_iNumMaterials; ++i)
	{
		MODELMATERIAL		tMaterial;
		ZeroMemory(&tMaterial, sizeof(MODELMATERIAL));

		for (_uint j = 0; j < AI_TEXTURE_TYPE_MAX; ++j)
		{
			_uint		iTemp = AI_TEXTURE_TYPE_MAX;
			ReadFile(hFile, &iTemp, sizeof(_uint), &dwByte, nullptr);

			if (iTemp == AI_TEXTURE_TYPE_MAX)
				continue;

			_uint		iFilePathLength = 0;
			ReadFile(hFile, &iFilePathLength, sizeof(_uint), &dwByte, nullptr);

			_tchar	*	pFilePath = new _tchar[iFilePathLength];
			ReadFile(hFile, pFilePath, sizeof(_tchar) * iFilePathLength, &dwByte, nullptr);

			tMaterial.pTexture[j] = CTexture::Create(m_pDevice, m_pContext, wstring(pFilePath));
			NULL_CHECK_RETURN(tMaterial.pTexture[j], E_FAIL);

			Safe_Delete_Array(pFilePath);
		}

		m_vecMaterials.push_back(tMaterial);
	}

	ReadFile(hFile, &m_dwBeginBoneData, sizeof(DWORD), &dwByte, nullptr);

	CloseHandle(hFile);

	return S_OK;
}


HRESULT CModel::Save_Model(const char* pSaveFileDirectory)
{
	_tchar		wszSaveFileDirectory[MAX_PATH] = L"";
	CGameUtils::ctwc(pSaveFileDirectory, wszSaveFileDirectory);

	DWORD	dwByte = 0;
	HANDLE	hFile = CreateFileW(wszSaveFileDirectory, GENERIC_WRITE, 0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);

	if (hFile == INVALID_HANDLE_VALUE)
		return E_FAIL;

	/* Meshes */
	WriteFile(hFile, &m_iNumMeshes, sizeof(_uint), &dwByte, nullptr);

	for (auto& pMesh : m_vecMeshes)
	{
		if (FAILED(pMesh->Save_Mesh(hFile, dwByte)))
		{
			MSG_BOX("Failed to Save : Mesh");
			CloseHandle(hFile);
			return E_FAIL;
		}
	}

	/* Materials*/
	if (m_iNumMaterials == 0)
		return E_FAIL;

	WriteFile(hFile, &m_iNumMaterials, sizeof(_uint), &dwByte, nullptr);

	for (auto& tMaterial : m_vecMaterials)
	{
		for (_uint i = 0; i < AI_TEXTURE_TYPE_MAX; ++i)
		{
			if (tMaterial.pTexture[i] == nullptr)
			{
				_uint		iTemp = AI_TEXTURE_TYPE_MAX;
				WriteFile(hFile, &iTemp, sizeof(_uint), &dwByte, nullptr);
			}
			else
			{
				WriteFile(hFile, &i, sizeof(_uint), &dwByte, nullptr);

				const _tchar*	wszFilePath = tMaterial.pTexture[i]->Get_FilePath().c_str();
				_uint				iFilePathLength = (_uint)tMaterial.pTexture[i]->Get_FilePath().length() + 1;

				WriteFile(hFile, &iFilePathLength, sizeof(_uint), &dwByte, nullptr);
				WriteFile(hFile, wszFilePath, sizeof(_tchar) * iFilePathLength, &dwByte, nullptr);
			}
		}
	}

	/* 파일 위치 포인터 받아서 저장하기 */
	m_dwBeginBoneData = SetFilePointer(hFile, 0, nullptr, FILE_CURRENT);

	/* Bones */
	WriteFile(hFile, &m_dwBeginBoneData, sizeof(DWORD), &dwByte, nullptr);

	WriteFile(hFile, &m_iNumBones, sizeof(_uint), &dwByte, nullptr);
	for (auto& pBone : m_vecBones)
	{
		if (FAILED(pBone->Save_Bone(hFile, dwByte)))
		{
			MSG_BOX("Failed to Save : Bone");
			CloseHandle(hFile);
			return E_FAIL;
		}
	}

	/* Mesh Bones */
	for (auto& pMesh : m_vecMeshes)
		pMesh->Save_MeshBones(hFile, dwByte);

	/* Animations */
	WriteFile(hFile, &m_iNumAnimation, sizeof(_uint), &dwByte, nullptr);

	for (auto& pAnimation : m_vecAnimations)
	{
		if (FAILED(pAnimation->Save_Animation(hFile, dwByte)))
		{
			MSG_BOX("Failed to Save : Animation");
			CloseHandle(hFile);
			return E_FAIL;
		}
	}

	CloseHandle(hFile);

	return S_OK;

}

HRESULT CModel::Load_BoneAnimation(HANDLE& hFile, DWORD& dwByte)
{
	/* Bones */
	ReadFile(hFile, &m_iNumBones, sizeof(_uint), &dwByte, nullptr);
	m_vecBones.reserve(m_iNumBones);

	CBone*		pRootBone = CBone::Create(nullptr, nullptr, 0);
	pRootBone->Load_Bone(hFile, dwByte);
	m_vecBones.push_back(pRootBone);

	FAILED_CHECK_RETURN(Ready_Bones(hFile, dwByte, pRootBone), E_FAIL);

	/* SetUp Mesh Bones */
	for (auto& pMesh : m_vecMeshes)
		pMesh->SetUp_MeshBones(hFile, dwByte, this);

	/* Animations */
	ReadFile(hFile, &m_iNumAnimation, sizeof(_uint), &dwByte, nullptr);
	m_vecAnimations.reserve(m_iNumAnimation);

	for (_uint i = 0; i < m_iNumAnimation; ++i)
	{
		CAnimation*	pAnimation = CAnimation::Create(nullptr, this);
		NULL_CHECK_RETURN(pAnimation, E_FAIL);
		FAILED_CHECK_RETURN(pAnimation->Load_Animation(hFile, dwByte), E_FAIL);

		m_vecAnimations.push_back(pAnimation);
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

CComponent * CModel::Clone(CGameObject* pOwner, void * pArg)
{
	CModel*		pInstance = new CModel(*this);

	if (FAILED(pInstance->Initialize_Clone(pOwner, pArg)))
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
