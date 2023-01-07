#pragma once
#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL CCell final :	public CBase
{
public:
	enum POINT	{ POINT_A, POINT_B, POINT_C, POINT_END };
	enum NEIGHBOR { NEIGHBOR_AB, NEIGHBOR_BC, NEIGHBOR_CA, NEIGHBOR_END	};
private:
	CCell(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CCell() = default;

public:
	const _int&						Get_Index() const { return m_iIndex; }
	_float3							Get_Point(POINT ePoint) { return m_vPoints[ePoint]; }
	_int*							Get_NeighbotIndex() { return m_iNeighborIndices; }
	void							Set_Neighbor(NEIGHBOR eNeighbor, CCell* pNeighbor) { m_iNeighborIndices[eNeighbor] = pNeighbor->m_iIndex;  }


public:
	HRESULT							Initialize(const _float3* pPoints, _int iIndex);
	void							ImGui_RenderProperty();

	_bool							Compare_Points(const _float3& SourPoint, const _float3& DestPoint);
	_bool							Compare_Height(_fvector vTargetPos);
	_bool							Compare_VerHeight(_fvector vTargetPos);
	_bool							Compare_VerHeight(_fvector vTargetPos, CCell* pOtherCell);
	_bool							IsIn(_fvector vTargetPos, _int& pNeighborIndex, _float4 & vBlockedLine, _float4 & vBlockedLineNormal);
	_float4							Get_CellHeight(_float4 fTargetPos);
	void							Get_BlockedLine(NEIGHBOR eNeighbor, _float4& vBlockedLine, _float4& vBlockedLineNormal);
#ifdef _DEBUG
	HRESULT							Render(class CShader* pShader);
#endif

private:
	ID3D11Device*					m_pDevice = nullptr;
	ID3D11DeviceContext*			m_pContext = nullptr;

private:
	_int							m_iIndex = 0;
	_float3							m_vPoints[POINT_END] = { _float3(0.f, 0.f, 0.f), _float3(0.f, 0.f, 0.f), _float3(0.f, 0.f, 0.f) };
	_int							m_iNeighborIndices[NEIGHBOR_END] = { -1, -1, -1 };

#ifdef _DEBUG
private:
	class CVIBuffer_Cell*			m_pVIBuffer = nullptr;
	class CVIBuffer_Cell_Circle*	m_pVIBufferCircleCom[POINT_END] = { nullptr, nullptr, nullptr };
#endif

public:
	static CCell*	Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const _float3* pPoints, _int iIndex);
	virtual void	Free();
};



END
