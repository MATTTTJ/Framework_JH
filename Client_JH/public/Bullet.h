#pragma once
#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CRenderer;
class CShader;
class CTexture;
class CVIBuffer_Rect;
class CCollider;
class CVIBuffer_Point_Instancing;
END

BEGIN(Client)
class CBullet abstract:	public CGameObject
{

protected:
	enum BULLETOWNERTYPE { OWNER_PLAYER, OWNER_MONSTER, OWNER_END };
	enum COLLIDERTYPE { COLLIDER_AABB, COLLIDER_OBB, COLLIDER_SPHERE, COLLIDER_MUZZLE, COLLIDERTYPE_END };

protected:
	CBullet(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CBullet(const CBullet& rhs);
	virtual ~CBullet() = default;

public:
	virtual HRESULT				Initialize_Prototype() override;
	virtual HRESULT				Initialize_Clone(const wstring& wstrPrototypeTag, void* pArg) override;
	virtual void				Tick(_double dTimeDelta) override;
	virtual void				Late_Tick(_double dTimeDelta) override;
	virtual HRESULT				Render() override;

private:
	HRESULT						SetUp_Components();
	HRESULT						SetUp_ShaderResources();

protected:
	CRenderer*					m_pRendererCom = nullptr;
	CShader*					m_pShaderCom = nullptr;
	CTexture*					m_pTextureCom = nullptr;
	CVIBuffer_Rect*				m_pVIBufferCom = nullptr;
	CGameObject*				m_pOwner = nullptr;
	CCollider*					m_pBulletColliderCom = nullptr;
	CVIBuffer_Point_Instancing*	m_pPointBuffer = nullptr;

protected:
	_float						m_fTrailCount;
	_bool						m_bTrailConvertOnce = false;

	_float4						m_vCamPos;
	_float4						m_vInitPos;
	_float4						m_vPlayerLook;

	_float4x4					m_PivotMatrix;
public:
	virtual CGameObject*		Clone(const wstring& wstrPrototypeTag, void* pArg = nullptr) PURE;
	virtual void				Free() override;
};	
END