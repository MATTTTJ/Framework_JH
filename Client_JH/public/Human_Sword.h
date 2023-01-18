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

class CHuman_Sword final  :	public CMonster
{
	friend class CHuman_Sword_State;


public:
	CHuman_Sword(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CHuman_Sword(const CHuman_Sword& rhs);
	virtual ~CHuman_Sword() = default;

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
	 _bool						Collision_Detected(CCollider* pOtherCollider) ;
	 _bool						Collider_AttRange(CCollider* pOtherCollider) ;

public:
	virtual void				Set_On_NaviMesh() override;
	virtual void				Collision_Body(CBullet* pBullet) override;
	virtual void				Collision_Head(CBullet* pBullet) override;
	virtual void				Collision_Hide(CBullet* pBullet) override;
	virtual void				Collision_PlayerEyes() override;

private:
	CRenderer*					m_pRendererCom = nullptr;
	CShader*					m_pShaderCom = nullptr;
	CNavigation*				m_pNavigationCom = nullptr;
	class CHuman_Sword_State*	m_pHuman_Sword_State = nullptr;

private:
	HRESULT						SetUp_Components();
	HRESULT						SetUp_ShaderResources();

public:
	static CHuman_Sword*		Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject*		Clone(const wstring& wstrPrototypeTag, void* pArg = nullptr) override;
	virtual void				Free() override;
};



END
