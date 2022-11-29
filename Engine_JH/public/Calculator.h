// #pragma once
// #include "Component.h"
//
//
//
// BEGIN(Engine)
//
// class CTransform;
// class CVIBuffer_Terrain;
// class CPipeLine;
//
// class ENGINE_DLL CCalculator final :	public CComponent
// {
// public:
// 	CCalculator(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
// 	CCalculator(const CCalculator& rhs);
// 	virtual ~CCalculator() = default;
//
// public:
// 	HRESULT			Ready_Calculator(void);
//
// 	_float			HeightOnTerrain(_float3 pPos, 
// 									const _float3* pTerrainVtxPos,	
// 									const _ulong& dwCntX, 
// 									const _ulong& dwCntZ, 
// 									const _ulong& dwVtxItv = 1);
//
// 	_float3			PickingOnTerrain(HWND hWnd, const CVIBuffer_Terrain* pTerrainBufferCom, const CTransform* pTerrainTransformCom);
//
// private:
// 	CPipeLine*		m_pPipeLine;
//
// public:
// 	virtual CComponent*			Clone(void* pArg = nullptr) override;
// 	static	CCalculator*		Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
// private:
// 	virtual void Free(void) override;
//
// };
//
// END