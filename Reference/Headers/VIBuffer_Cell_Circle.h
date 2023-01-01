#pragma once
#include "Engine_Defines.h"
#include "VIBuffer.h"

BEGIN(Engine)
class ENGINE_DLL CVIBuffer_Cell_Circle  final :	public CVIBuffer
{
public:
	CVIBuffer_Cell_Circle(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CVIBuffer_Cell_Circle(const CVIBuffer_Cell_Circle& rhs);
	virtual ~CVIBuffer_Cell_Circle() = default;

	virtual HRESULT Initialize_Prototype(const _float3& vPoint);
	virtual HRESULT Initialize_Clone(CGameObject* pOwner, void* pArg) override;

public:
	static CVIBuffer_Cell_Circle*	Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const _float3& vPoint);
	virtual CComponent*				Clone(CGameObject* pOwner, void* pArg = nullptr);
	virtual void					Free() override;
};
END