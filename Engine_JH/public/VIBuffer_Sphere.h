#pragma once
#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer_Sphere final : public CVIBuffer
{
private:
	CVIBuffer_Sphere(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CVIBuffer_Sphere(const CVIBuffer_Sphere& rhs);
	virtual ~CVIBuffer_Sphere() = default;

public:
	HRESULT						Initialize_Prototype();
	virtual HRESULT				Initialize_Clone(class CGameObject* pOwner, void* pArg) override;

private:
	_uint						m_iSegments = 0;
	_uint						m_iSlices = 0;

public:
	static	CVIBuffer_Sphere*	Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CComponent*			Clone(class CGameObject* pOwner, void* pArg = nullptr) override;
	virtual void				Free() override;
};

END