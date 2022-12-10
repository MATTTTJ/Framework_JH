#pragma once

#include "Component.h"

/* 충돌이 필요한 객체들에게 충돌체를 추가해주기위해. */
/* 객체가 충돌체를 가지고 있게 만들어주겠다. */
/* 디버깅용으로 그리는 형태의 기능을 만들어주겠다. */

BEGIN(Engine)

class ENGINE_DLL CCollider final : public CComponent
{
public:
	enum TYPE { TYPE_AABB, TYPE_OBB, TYPE_SPHERE, TYPE_END };

public:
	typedef struct tagColliderDesc
	{
		_float3			vCenter;
		_float3			vSize; /* 너비, 높이, 깊이 */
		_float3			vRotation; /* x축회전, y축회전, z축회전 */
	}COLLIDERDESC;
public:
	CCollider(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CCollider(const CCollider& rhs);
	virtual ~CCollider() = default;

public:
	virtual HRESULT Initialize_Prototype(TYPE eType);
	virtual HRESULT Initialize(void* pArg);

public:
	void Update(_fmatrix TransformMatrix);

#ifdef _DEBUG
public:
	HRESULT Render();
#endif // _DEBUG

private:
	TYPE						m_eType = TYPE_END;
	BoundingBox*				m_pAABB_Original = nullptr;
	BoundingBox*				m_pAABB = nullptr;
	BoundingOrientedBox*		m_pOBB = nullptr;
	BoundingSphere*				m_pSphere = nullptr;
	_bool						m_isColl = false;

#ifdef _DEBUG
private:
	PrimitiveBatch<VertexPositionColor>*				m_pBatch = nullptr;
	BasicEffect*										m_pEffect = nullptr;
	ID3D11InputLayout*									m_pInputLayout = nullptr;	
	_float4												m_vColor;
#endif // _DEBUG

	

public:
	static CCollider* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, TYPE eType);
	virtual CComponent* Clone(void* pArg = nullptr);
	virtual void Free() override;
};

END