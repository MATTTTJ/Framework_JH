#pragma once
#include "Engine_Defines.h"
#include "Base.h"

BEGIN(Engine)

class CFrustum final :	public CBase
{
	DECLARE_SINGLETON(CFrustum);

private:
	CFrustum();
	virtual ~CFrustum() = default;

public:
	HRESULT		Initialize();
	void		Transform_ToWorldSpace();
	void		Transform_ToLocalSpace(_fmatrix WorldMatrix);


public:
	_bool		isInFrustum_WorldSpace(_fvector vWorldPos, _float fRange = 0.f);
	_bool		isInFrustum_LocalSpace(_fvector vLocalPos, _float fRange = 0.f);

private:
	_float3		m_vPoints[8];
	_float3		m_vPointInWorld[8];

	_float4		m_PlaneInWorld[6];
	_float4		m_PlaneInLocal[6];


private:
	void		Make_Planes(_float4* pPlane, const _float3* pPoints);

public:
	virtual void Free() override;
};

END