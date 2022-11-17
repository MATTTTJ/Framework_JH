#pragma once
#include "Base.h"

// ��, ���� ����� �����ϴ� �̱����̴�. 

BEGIN(Engine)
class CPipeLine final : public CBase
{
	DECLARE_SINGLETON(CPipeLine)
public:
	enum TRANSFORMSTATE { D3DTS_VIEW, D3DTS_PROJ, D3DTS_END};
public:
	CPipeLine();
	virtual ~CPipeLine() = default;

public:
	_matrix				Get_TransformMatrix(TRANSFORMSTATE eState) const;
	_float4x4			Get_TransformFloat4x4(TRANSFORMSTATE eState) const;
	_matrix				Get_TransformMatrix_Inverse(TRANSFORMSTATE eState) const;

public:
	// _fmatrix Ÿ���� ����� �� �Ǵ� ������Ŀ� �����Ѵ�. 
	void				Set_Transform(TRANSFORMSTATE eState, _fmatrix TransformMatrix);

public:
	void				Tick();

private:
	_float4x4			m_TransformMatrices[D3DTS_END];
	_float4x4			m_TransformMatrices_Inverse[D3DTS_END];
	_float4				m_vCamPosition;

public:
	virtual void Free() override;
};

END