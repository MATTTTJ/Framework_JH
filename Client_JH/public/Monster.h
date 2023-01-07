#pragma once
#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CCollider;
class CState; 
END

BEGIN(Client)

class CMonster final : public CGameObject
{

public:
	enum COLLIDERTYPE { COLLTYPE_AABB, COLLTYPE_OBB, COLLTYPE_SPHERE, COLLTYPE_END };

private:
	CMonster(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CMonster(const CMonster& rhs);
	virtual ~CMonster() = default;

public:
	void					Set_Player(class CPlayer* pPlayer) { m_pPlayer = pPlayer; }

public:
	virtual HRESULT			Initialize_Prototype() override;
	virtual HRESULT			Initialize_Clone(const wstring& wstrPrototypeTag, void* pArg) override;
	virtual void			Tick(_double TimeDelta) override;
	virtual void			Late_Tick(_double TimeDelta) override;
	virtual HRESULT			Render() override;

public:
	virtual void			Collider_Tick(_double TimeDelta) {}
	virtual void			Collider_Late_Tick(_double TimeDelta) {} 
	virtual void			Set_On_NaviMesh() {}
public:
	virtual void			Collision_Event(class CPlayer* pPlayer) {}; // 구현해도되고 안해도되고 

private:
	class CPlayer*			m_pPlayer = nullptr;
	CState*					m_pStat = nullptr;

	_bool					m_bPlayerDetected = false;

	CCollider*				m_pHeadColliderCom = nullptr;
	CCollider*				m_pBodyColliderCom = nullptr;
	CCollider*				m_pAttackColliderCom = nullptr;

public:
	virtual CGameObject*	Clone(const wstring& wstrPrototypeTag, void* pArg = nullptr) PURE; // 필수 구현
	virtual void			Free() override;
};

END