#pragma once
#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer abstract : public CComponent
{
public:
	CVIBuffer(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CVIBuffer(const CVIBuffer& rhs);
	virtual ~CVIBuffer() = default;

	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize_Clone(void* pArg);
	virtual HRESULT	Render();

protected:
	// 할당하고자하는 버퍼의 속성을 정의한다. 
	D3D11_BUFFER_DESC			m_BufferDesc; 
	// 할당시에 채워넣고자하는 데이터들 
	D3D11_SUBRESOURCE_DATA		m_SubResourceData;

	// 정점하나의 바이트 크기
	_uint						m_iStride = 0;
	// 정점의 갯수
	_uint						m_iNumVertices = 0;
	// 그리고자하는 도형의 갯수
	_uint						m_iNumPrimitives = 0;
	// 도형을 그릴 때 인덱스의 사이즈
	_uint						m_iIndicesSizePerPrimitive = 0;
	// 도형을 그릴 때 인덱스의 갯수
	_uint						m_iNumIndicesPerPrimitive = 0;
	// 인덱스의 갯수
	_uint						m_iNumIndices = 0;
	// 버텍스 버퍼의 갯수
	_uint						m_iNumVertexBuffer = 0;
	// 인덱스의 포맷
	DXGI_FORMAT					m_eIndexFormat;
	// 트라이앵글리스트와 같이 어떤식으로 데이터를 들고있을지 결정
	D3D11_PRIMITIVE_TOPOLOGY	m_eTopology;

protected: // 버텍스 버퍼 
	ID3D11Buffer*				m_pVB = nullptr;

protected:
	ID3D11Buffer*				m_pIB = nullptr;

protected:
	HRESULT						Create_VertexBuffer();
	HRESULT						Create_IndexBuffer();


public:
	virtual CComponent* Clone(void* pArg) = 0;
	virtual void Free() override;
};

END
