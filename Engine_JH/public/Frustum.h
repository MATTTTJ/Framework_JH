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
private:
	_float3		m_vPoints[8];
	_float4		m_vPlane[6];


public:
	virtual void Free() override;
};

END