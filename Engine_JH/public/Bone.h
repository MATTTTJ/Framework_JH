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

	// 모든 뼈들은 한 스페이스로 옮겨져야한다.
	// 모델러와 애니메이터가 작업한 스페이스가 달라서 같은 곳으로 보내주는 행렬
	_float4x4		m_OffsetMatrix;
	// 뼈의 상태 행렬
	_float4x4		m_TransformMatrix;
	// 상속받는 뼈의 행렬을 곱한 상태행렬 (연산 완료된 최종 행렬)
	_float4x4		m_CombindTransformMatrix;
	CBone*			m_pParent = nullptr;

public:
	static	CBone*	Create(aiNode* pAINode, CBone* pParent);
	virtual void	Free() override;

};

END