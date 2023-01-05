#pragma once

#include "VIBuffer_Instancing.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer_Point_Instancing final : public CVIBuffer_Instancing
{
public:
	CVIBuffer_Point_Instancing(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CVIBuffer_Point_Instancing(const CVIBuffer_Point_Instancing& rhs);
	virtual ~CVIBuffer_Point_Instancing() = default;

public:
	virtual HRESULT Initialize_Prototype(_uint iNumInstance, _float3 vPosition);
	virtual HRESULT Initialize_Clone(CGameObject* pOwner, void* pArg) override;
	virtual HRESULT Tick(_double TimeDelta) override;
	virtual HRESULT Render();

private:
	_double*				m_pSpeeds = nullptr;
	_float3					m_vInitPosition = { 0.f, 0.f, 0.f };

public:
	static CVIBuffer_Point_Instancing* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, _uint iNumInstance = 1, _float3 vPosition = { 0.f,0.f,0.f });
	virtual CComponent* Clone(CGameObject* pOwner, void* pArg = nullptr) override;
	virtual void Free() override;
};

END