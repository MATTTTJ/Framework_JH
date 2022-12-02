#pragma once
#include "Base.h"
BEGIN(Engine)
class CBone final :	public CBase
{
private:
	CBone();
	virtual ~CBone() = default;

public:
	const char*		Get_Name() const
	{
		return m_szName;
	}

	void			Set_OffetMatrix(_float4x4 OffsetMatrix)
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
	HRESULT			Initialize(aiNode* pAINode);
	void			compute_CombindTransformationMatrix();


private:
	char			m_szName[MAX_PATH];

	// ��� ������ �� �����̽��� �Ű������Ѵ�.
	// �𵨷��� �ִϸ����Ͱ� �۾��� �����̽��� �޶� ���� ������ �����ִ� ���
	_float4x4		m_OffsetMatrix;
	// ���� ���� ���
	_float4x4		m_TransformMatrix;
	// ��ӹ޴� ���� ����� ���� ������� (���� �Ϸ�� ���� ���)
	_float4x4		m_CombindTransformMatrix;
	CBone*			m_pParent = nullptr;

public:
	static	CBone*	Create(aiNode* pAINode);
	virtual void	Free() override;

};

END