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
class CModel;
END

BEGIN(Client)
class CBullet abstract:	public CGameObject
{


protected:
	enum COLLIDERTYPE { COLLIDER_AABB, COLLIDER_OBB, COLLIDER_SPHERE, COLLIDER_MUZZLE, COLLIDERTYPE_END };

public:
	enum BULLETOWNERTYPE { OWNER_PLAYER, OWNER_MONSTER, OWNER_BOSS, OWNER_END };

	typedef	struct tagBulletOption
	{
		enum ELEMENTS { TYPE_NONE, TYPE_FIRE, TYPE_POISON, TYPE_THUNDER, TYPE_END };

		CGameObject::GAMEOBJECTDESC BulletDesc;
		ELEMENTS m_eType = TYPE_END;

		_bool			m_bTrueIsLeft = true;
		CGameObject*	m_pOwner = nullptr;
		BULLETOWNERTYPE m_eOwner = OWNER_END;
	}BULLETOPTION;



protected:
	CBullet(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CBullet(const CBullet& rhs);
	virtual ~CBullet() = default;

public:
	virtual CCollider*			Get_ColliderPtr() { return m_pBulletColliderCom; }
	virtual BULLETOPTION&		Get_BulletOption() { return m_tBulletOption; }

public:
	virtual HRESULT				Initialize_Prototype() override;
	virtual HRESULT				Initialize_Clone(const wstring& wstrPrototypeTag, void* pArg) override;
	virtual void				Tick(_double dTimeDelta) override;
	virtual void				Late_Tick(_double dTimeDelta) override;
	virtual HRESULT				Render() override;
	virtual _bool				Collision_Body();
	virtual _bool				Collision_Head();
	virtual _bool				Collision_HideCollider();
	virtual _bool				Collision_Shield();
	virtual _bool				Collision_LArm();
	virtual _bool				Collision_RArm();
	virtual _bool				Collision_To_BossMonster();
	virtual _bool				Collision_To_Player(CCollider* pBulletCollider);
private:
	HRESULT						SetUp_Components();
	HRESULT						SetUp_ShaderResources();
	void						Create_DamageFont();
public:

protected:
	CRenderer*					m_pRendererCom = nullptr;
	CShader*					m_pShaderCom = nullptr;
	CTexture*					m_pTextureCom = nullptr;
	CVIBuffer_Rect*				m_pVIBufferCom = nullptr;
	CGameObject*				m_pOwner = nullptr;
	CCollider*					m_pBulletColliderCom = nullptr;
	CCollider*					m_pBoomColliderCom = nullptr;
	CCollider*					m_pBladeColliderCom = nullptr;


	CVIBuffer_Point_Instancing*	m_pPointBuffer = nullptr;
	CModel*						m_pModelCom = nullptr;
protected:
	BULLETOPTION				m_tBulletOption;
	

	_float						m_fTrailCount;
	_bool						m_bTrailConvertOnce = false;

	_float4						m_vCamPos;
	_float4						m_vInitPos;
	_float4						m_vPlayerLook;
	_float4						m_vFirstSpawn;
	_float4x4					m_PivotMatrix;

	_bool						m_bCollOnce = false;
	_float2						m_vPSize{ 0.f, 0.f };

	//For Collision Monster
private:
	list<CGameObject*>			m_MonsterList;

public:
	virtual CGameObject*		Clone(const wstring& wstrPrototypeTag, void* pArg = nullptr) PURE;
	virtual void				Free() override;
};	
END