#pragma once
#include "Base.h"
BEGIN(Engine)
class CBone final :	public CBase
{
private:
	CBone();
	virtual ~CBone() = default;

public:
	const string&		Get_Name() const
	{
		return m_szName;
	}

	void			Set_OffsetMatrix(_float4x4 OffsetMatrix)
	{
		m_OffsetMatrix = OffsetMatrix;
	}

	_matrix Get_OffsetMatrix()
	{
		return XMLoadFloat4x4(&m_OffsetMatrix);
	}

	_matrix Get_CombindMatrix()
	{
		return XMLoadFloat4x4(&m_CombindTransformMatrix);
	}

	void	Set_TransformMatrix(_fmatrix TransformMatrix)
	{
		XMStoreFloat4x4(&m_TransformMatrix, TransformMatrix);
	}

public:
	HRESULT			Initialize(aiNode* pAINode, CBone* pParent);
	void			compute_CombindTransformationMatrix();


private:
	string			m_szName = "";

	// ��� ������ �� �����̽��� �Ű������Ѵ�.
	// �𵨷��� �ִϸ����Ͱ� �۾��� �����̽��� �޶� ���� ������ �����ִ� ���
	_float4x4		m_OffsetMatrix;
	// ���� ���� ���
	_float4x4		m_TransformMatrix;
	// ��ӹ޴� ���� ����� ���� ������� (���� �Ϸ�� ���� ���)
	_float4x4		m_CombindTransformMatrix;
	CBone*			m_pParent = nullptr;

public:
	static	CBone*	Create(aiNode* pAINode, CBone* pParent);
	virtual void	Free() override;

};

END