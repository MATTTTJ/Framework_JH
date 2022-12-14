#pragma once
#include "Base.h"

BEGIN(Engine)

class CCell final :	public CBase
{
public:
	enum POINT	{ POINT_A, POINT_B, POINT_C, POINT_END };
	enum NEIGHBOR { NEIGHBOR_AB, NEIGHBOR_BC, NEIGHBOR_CA, NEIGHBOR_END	};
private:
	CCell(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CCell() = default;

public:
	HRESULT		Initialize(const _float3* pPoints, _int iIndex);

private:
	_int		m_iIndex = 0;
	_float3		m_vPoints[POINT_END];
	_int		m_iNeighborIndices[NEIGHBOR_END];

public:
	static CCell*	Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const _float3* pPoints, _int iIndex);
	virtual void	Free();
};

END