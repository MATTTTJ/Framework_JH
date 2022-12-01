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

public:
	HRESULT			Initialize(aiNode* pAINode);
	void			compute_CombindTransformationMatrix();


private:
	char			m_szName[MAX_PATH];

	// 모든 뼈들은 한 스페이스로 옮겨져야한다.
	// 모델러와 애니메이터가 작업한 스페이스가 달라서 같은 곳으로 보내주는 행렬
	_float4x4		m_OffsetMatrix;
	// 뼈의 상태 행렬
	_float4x4		m_TransformMatrix;
	// 상속받는 뼈의 행렬을 곱한 상태행렬 (연산 완료된 최종 행렬)
	_float4x4		m_CombindTransformMatrix;
	CBone*			m_pParent;

public:
	static	CBone*	Create(aiNode* pAINode);
	virtual void	Free() override;

};

END