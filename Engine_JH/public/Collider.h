#pragma once

#include "Component.h"

/* �浹�� �ʿ��� ��ü�鿡�� �浹ü�� �߰����ֱ�����. */
/* ��ü�� �浹ü�� ������ �ְ� ������ְڴ�. */
/* ���������� �׸��� ������ ����� ������ְڴ�. */

BEGIN(Engine)

class ENGINE_DLL CCollider final : public CComponent
{
public:
	enum COLLIDERTYPE { COLLIDER_AABB, COLLIDER_OBB,  COLLIDER_SPHERE, COLLIDERTYPE_END };

public:
	typedef struct tagColliderDesc
	{
		_float3			vPosition;
		_float3			vSize; /* �ʺ�, ����, ���� */
		_float3			vRotation; /* x��ȸ��, y��ȸ��, z��ȸ�� */
	}COLLIDERDESC;

	typedef struct tagOBBDesc
	{
		_float3			vCenter;
		_float3			vCenterAxis[3];
		_float3			vAlignAxis[3];
	}OBBDESC;

public:
	CCollider(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CCollider(const CCollider& rhs);
	virtual ~CCollider() = default;

public:
	const	COLLIDERTYPE&	Get_ColliderType() const { return m_eType; }


public:
	virtual HRESULT			Initialize_Prototype(COLLIDERTYPE eType);
	virtual HRESULT			Initialize_Clone(void* pArg) override;

public:
	void					Update(_fmatrix TransformMatrix);

public:
	_bool					Collision(class CCollider* pTargetCollider);
	_bool					Collision_AABB(class CCollider* pTargetCollider);
	_bool					Collision_OBB(class CCollider* pTargetCollider);


#ifdef _DEBUG
public:
	HRESULT					Render();
#endif // _DEBUG

private:
	_matrix					Remove_Rotation(_fmatrix TransformMatrix);
	_float3					Compute_Min();
	_float3					Compute_Max();
	OBBDESC					Compute_OBBDesc();


private:
	_bool					m_bIsColl = false;
	COLLIDERTYPE			m_eType = COLLIDERTYPE_END;

	BoundingSphere*			m_pSphere_Original = nullptr;
	BoundingSphere*			m_pSphere = nullptr;

	BoundingBox*			m_pAABB_Original = nullptr;
	BoundingBox*			m_pAABB = nullptr;

	BoundingOrientedBox*	m_pOBB_Original = nullptr;
	BoundingOrientedBox*	m_pOBB = nullptr;

#ifdef _DEBUG
private:
	PrimitiveBatch<VertexPositionColor>*				m_pBatch = nullptr;
	BasicEffect*										m_pEffect = nullptr;
	ID3D11InputLayout*									m_pInputLayout = nullptr;	
	_float4												m_vColor;
#endif // _DEBUG

public:
	static CCollider*	Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, COLLIDERTYPE eType);
	virtual CComponent* Clone(void* pArg = nullptr) override;
	virtual void		Free() override;
};

END