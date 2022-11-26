#pragma once
#include "VIBuffer.h"
BEGIN(Engine)
class CMesh final :	public CVIBuffer
{
public:
	CMesh(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CMesh(const CMesh& rhs);
	virtual ~CMesh() = default;

public:
	virtual HRESULT		Initialize_Prototype(aiMesh* pAIMesh);
	virtual HRESULT		Initialize_Clone(void* pArg) override;

private:
	_uint				m_iMaterialIndex = 0;

public:
	static CMesh*		Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, aiMesh* pAIMesh);
	virtual CComponent* Clone(void* pArg = nullptr) override;
	virtual void		Free();

};

END