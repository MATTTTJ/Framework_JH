#pragma once
#include "PipeLine.h"
#include "Transform.h"
#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer_Terrain final : public CVIBuffer
{
protected:
	CVIBuffer_Terrain(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CVIBuffer_Terrain(const CVIBuffer_Terrain& rhs);
	virtual ~CVIBuffer_Terrain() = default;

public:
	virtual HRESULT Initialize_Prototype(const _tchar* pHeightMapFilePath);
	virtual HRESULT Initialize_Clone(class CGameObject* pOwner, void* pArg) override;


public:
	_float4			PickingOnTerrain(HWND hwnd, _matrix WorldMatrixInverse);

private:
	_uint			m_iNumVerticesX = 0;
	_uint			m_iNumVerticesZ = 0;

	CPipeLine*		m_pPipeLine;
	CTransform*		m_pTransformCom;

	// 버텍스 위치 멤버변수로 저장, 원본이 지워질 떄 딜리트 어레이
	VTXNORTEX*		m_pVertices;

	
public:
	static CVIBuffer_Terrain*	Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const _tchar* pHeightMapFilePath);
	virtual CComponent*			Clone(class CGameObject* pOwner, void* pArg = nullptr) override;
	virtual void				Free() override;
};

END