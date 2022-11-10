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
	// �Ҵ��ϰ����ϴ� ������ �Ӽ��� �����Ѵ�. 
	D3D11_BUFFER_DESC			m_BufferDesc; 
	// �Ҵ�ÿ� ä���ְ����ϴ� �����͵� 
	D3D11_SUBRESOURCE_DATA		m_SubResourceData;

	// �����ϳ��� ����Ʈ ũ��
	_uint						m_iStride = 0;
	// ������ ����
	_uint						m_iNumVertices = 0;
	// �׸������ϴ� ������ ����
	_uint						m_iNumPrimitives = 0;
	// ������ �׸� �� �ε����� ������
	_uint						m_iIndicesSizePerPrimitive = 0;
	// ������ �׸� �� �ε����� ����
	_uint						m_iNumIndicesPerPrimitive = 0;
	// �ε����� ����
	_uint						m_iNumIndices = 0;
	// ���ؽ� ������ ����
	_uint						m_iNumVertexBuffer = 0;
	// �ε����� ����
	DXGI_FORMAT					m_eIndexFormat;
	// Ʈ���̾ޱ۸���Ʈ�� ���� ������� �����͸� ��������� ����
	D3D11_PRIMITIVE_TOPOLOGY	m_eTopology;

protected: // ���ؽ� ���� 
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
