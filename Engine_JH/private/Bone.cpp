#include "Bone.h"

CBone::CBone()
{
}

HRESULT CBone::Save_Bone(HANDLE& hFile, DWORD& dwByte)
{
	_uint			iNameLength = (_uint)m_strName.length() + 1;
	const char*	pName = m_strName.c_str();
	WriteFile(hFile, &iNameLength, sizeof(_uint), &dwByte, nullptr);
	WriteFile(hFile, pName, sizeof(char) * iNameLength, &dwByte, nullptr);

	WriteFile(hFile, &m_OffsetMatrix, sizeof(_float4x4), &dwByte, nullptr);
	WriteFile(hFile, &m_TransformMatrix, sizeof(_float4x4), &dwByte, nullptr);
	WriteFile(hFile, &m_iNumChild, sizeof(_uint), &dwByte, nullptr);

	return S_OK;
}

HRESULT CBone::Save_BoneName(HANDLE& hFile, DWORD& dwByte)
{
	_uint			iNameLength = (_uint)m_strName.length() + 1;
	const char*	pName = m_strName.c_str();
	WriteFile(hFile, &iNameLength, sizeof(_uint), &dwByte, nullptr);
	WriteFile(hFile, pName, sizeof(char) * iNameLength, &dwByte, nullptr);

	return S_OK;
}

HRESULT CBone::Load_Bone(HANDLE& hFile, DWORD& dwByte)
{
	_uint			iNameLength = 0;
	ReadFile(hFile, &iNameLength, sizeof(_uint), &dwByte, nullptr);

	char*			pName = new char[iNameLength];
	ReadFile(hFile, pName, sizeof(char) * iNameLength, &dwByte, nullptr);

	m_strName = pName;

	Safe_Delete_Array(pName);

	ReadFile(hFile, &m_OffsetMatrix, sizeof(_float4x4), &dwByte, nullptr);
	ReadFile(hFile, &m_TransformMatrix, sizeof(_float4x4), &dwByte, nullptr);
	ReadFile(hFile, &m_iNumChild, sizeof(_uint), &dwByte, nullptr);

	return S_OK;
}

HRESULT CBone::Initialize(aiNode* pAINode, CBone* pParent, _uint iNumChild)
{

	if (nullptr != pParent)
	{
		m_pParent = pParent;
		Safe_AddRef(m_pParent);
	}

	if (pAINode == nullptr)
		return S_OK;

	m_strName = pAINode->mName.data;

	m_iNumChild = iNumChild;

	XMStoreFloat4x4(&m_OffsetMatrix, XMMatrixIdentity());
	// 이 트랜스폼 행렬은 전치된 상태로 저장되어있기때문에, 다시 전치해주어야한다. 
	memcpy(&m_TransformMatrix, &pAINode->mTransformation, sizeof(_float4x4));
	// 모델에 저장되어있던 뼈들의 행렬을 전치해줌
	XMStoreFloat4x4(&m_TransformMatrix, XMMatrixTranspose(XMLoadFloat4x4(&m_TransformMatrix)));
	
	XMStoreFloat4x4(&m_CombindTransformMatrix, XMMatrixIdentity());



	return S_OK;
}

void CBone::compute_CombindTransformationMatrix()
{
	if (nullptr == m_pParent)
		m_CombindTransformMatrix = m_TransformMatrix; // 부모가 없는 뼈의 경우 트랜스폼이 대체가능
	// 부모의 최종으로 연산된 행렬과 현재 상태 행렬을 곱해서 현재 최종으로 연산된 행렬을 구함
	else
		XMStoreFloat4x4(&m_CombindTransformMatrix, XMLoadFloat4x4(&m_TransformMatrix) * XMLoadFloat4x4(&m_pParent->m_CombindTransformMatrix));
}

CBone* CBone::Create(aiNode* pAINode, CBone* pParent, _uint iNumChild)
{
	CBone*	pInst = new CBone();

	if(FAILED(pInst->Initialize(pAINode, pParent, iNumChild)))
	{
		MSG_BOX("Failed to Create : CBone");

		Safe_Release(pInst);
	}

	return pInst;
}

void CBone::Free()
{
	if (nullptr != m_pParent)
	{
		Safe_Release(m_pParent);
	}
}
