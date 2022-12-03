#include "Bone.h"

CBone::CBone()
{
}

HRESULT CBone::Initialize(aiNode* pAINode)
{
	NULL_CHECK_RETURN(pAINode, E_FAIL)

	strcpy_s(m_szName, pAINode->mName.data);
	
	XMStoreFloat4x4(&m_OffsetMatrix, XMMatrixIdentity());
	// �� Ʈ������ ����� ��ġ�� ���·� ����Ǿ��ֱ⶧����, �ٽ� ��ġ���־���Ѵ�. 
	memcpy(&m_TransformMatrix, &pAINode->mTransformation, sizeof(_float4x4));
	// �𵨿� ����Ǿ��ִ� ������ ����� ��ġ����
	XMStoreFloat4x4(&m_TransformMatrix, XMMatrixTranspose(XMLoadFloat4x4(&m_TransformMatrix)));
	
	XMStoreFloat4x4(&m_CombindTransformMatrix, XMMatrixIdentity());

	return S_OK;
}

void CBone::compute_CombindTransformationMatrix()
{
	if (nullptr == m_pParent)
		m_CombindTransformMatrix = m_TransformMatrix; // �θ� ���� ���� ��� Ʈ�������� ��ü����
	// �θ��� �������� ����� ��İ� ���� ���� ����� ���ؼ� ���� �������� ����� ����� ����
	else
		XMStoreFloat4x4(&m_CombindTransformMatrix, XMLoadFloat4x4(&m_TransformMatrix) * XMLoadFloat4x4(&m_pParent->m_CombindTransformMatrix));
}

CBone* CBone::Create(aiNode* pAINode)
{
	CBone*	pInst = new CBone();

	if(FAILED(pInst->Initialize(pAINode)))
	{
		MSG_BOX("Failed to Create : CBone");

		Safe_Release(pInst);
	}

	return pInst;
}

void CBone::Free()
{
}