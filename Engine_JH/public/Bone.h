#pragma once
#include "Base.h"
BEGIN(Engine)
class CBone final :	public CBase
{
private:
	CBone();
	virtual ~CBone() = default;

public:
	HRESULT			Save_Bone(HANDLE& hFile, DWORD& dwByte);
	HRESULT			Save_BoneName(HANDLE& hFile, DWORD& dwByte);
	HRESULT			Load_Bone(HANDLE& hFile, DWORD& dwByte);
	const string&	Get_Name() const	{ return m_strName; }
	const _uint&	Get_NumChild() const { return m_iNumChild; }
	_matrix			Get_OffsetMatrix()	{ return XMLoadFloat4x4(&m_OffsetMatrix); }
	_matrix			Get_CombindMatrix() { return XMLoadFloat4x4(&m_CombindTransformMatrix);	}
	_matrix			Get_TransformMatrix() { return XMLoadFloat4x4(&m_TransformMatrix); }

	void			Set_TransformMatrix(_fmatrix TransformMatrix) { XMStoreFloat4x4(&m_TransformMatrix, TransformMatrix); }
	void			Set_OffsetMatrix(_float4x4 OffsetMatrix) { m_OffsetMatrix = OffsetMatrix; }
	void			Set_Transform_Fix(_float4 vPos)
	{
		m_TransformMatrix.Translation(_float3(vPos.x, vPos.y, vPos.z));
	}
public:
	HRESULT			Initialize(aiNode* pAINode, CBone* pParent, _uint iNumChild);
	void			compute_CombindTransformationMatrix();


private:
	string			m_strName = "";

	// ��� ������ �� �����̽��� �Ű������Ѵ�.
	// �𵨷��� �ִϸ����Ͱ� �۾��� �����̽��� �޶� ���� ������ �����ִ� ���
	_float4x4		m_OffsetMatrix;
	// ���� ���� ���
	_float4x4		m_TransformMatrix;
	// ��ӹ޴� ���� ����� ���� ������� (���� �Ϸ�� ���� ���)
	_float4x4		m_CombindTransformMatrix;
	CBone*			m_pParent = nullptr;
	_uint			m_iNumChild = 0;
public:
	static	CBone*	Create(aiNode* pAINode, CBone* pParent, _uint iNumChild);
	virtual void	Free() override;

};

END