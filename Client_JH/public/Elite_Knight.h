#pragma once
#include "Client_Defines.h"
#include "Monster.h"

BEGIN(Engine)
class CRenderer;
class CShader;
class CState;
class CNavigation;
class CModel;
class CCollider;
END

BEGIN(Client)

class CElite_Knight final  :	public CMonster
{
	friend class CElite_Knight_State;

public:
	CElite_Knight(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CElite_Knight(const CElite_Knight& rhs);
	virtual ~CElite_Knight() = default;

public:
	void						Get_Damaged() { m_tMonsterOption.MonsterDesc.m_iHP -= 30; }
	virtual HRESULT				Initialize_Prototype() override;
	virtual HRESULT				Initialize_Clone(const wstring& wstrPrototypeTag, void* pArg) override;
	virtual void				Tick(_double TimeDelta) override;
	virtual void				Late_Tick(_double TimeDelta) override;
	virtual HRESULT				Render() override;
	HRESULT						Ready_UI();

public:
	virtual void				Collider_Tick(_double TimeDelta) override;

public:
	_bool						Collision_Detected(CCollider* pOtherCollider);
	_bool						Collider_AttRange(CCollider* pOtherCollider);
	
public:
	virtual void				Set_On_NaviMesh() override;
	virtual void				Collision_Body(CBullet* pBullet) override;
	virtual void				Collision_Head(CBullet* pBullet) override;
	virtual void				Collision_Hide(CBullet* pBullet) override;
	virtual void				Collision_PlayerEyes() override;
	virtual void				Collision_Shield(class CBullet* pBullet) override;
	virtual void				Collision_Armor(CBullet* pBullet) override;
private:
	CRenderer*					m_pRendererCom = nullptr;
	CShader*					m_pShaderCom = nullptr;
	class CElite_Knight_State*	m_pElite_Knight_State = nullptr;

	CCollider*					m_pShieldColliderCom = nullptr;
	CCollider*					m_pLeftArmColliderCom = nullptr;
	CCollider*					m_pRightArmColliderCom = nullptr;


private:
	HRESULT						SetUp_Components();
	HRESULT						SetUp_ShaderResources();

public:
	static CElite_Knight*		Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject*		Clone(const wstring& wstrPrototypeTag, void* pArg = nullptr) override;
	virtual void				Free() override;
};



END
