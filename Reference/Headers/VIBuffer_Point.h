#pragma once
#include "VIBuffer.h"

BEGIN(Engine)
class ENGINE_DLL CVIBuffer_Point final : public CVIBuffer
{
protected:
	CVIBuffer_Point(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CVIBuffer_Point(const CVIBuffer_Point& rhs);
	virtual ~CVIBuffer_Point() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize_Clone(CGameObject* pOwner, void* pArg) override;
	virtual HRESULT Render();

public:
	virtual _uint3 Get_Indices(_uint iIndex) const { return _uint3(0, 0, 0); }

private:
	ID3D11Buffer*				m_pVBInstance = nullptr;
	_uint						m_iInstanceStride = 0;
	_uint						m_iNumInstance = 0;

public:
	static CVIBuffer_Point* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CComponent* Clone(CGameObject* pOwner, void* pArg) override; 
	virtual void Free() override;
};

END