#pragma once
#include "Client_Defines.h"
#include "Monster.h"
#include "Model.h"

BEGIN(Engine)
class CRenderer;
class CShader;
class CState;
class CNavigation;
class CModel;
class CCollider;
END

BEGIN(Client)

class CNormal_Boss final : public CMonster
{
	friend class CBoss_Golem_State;

public:
	CNormal_Boss(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CNormal_Boss(const CNormal_Boss& rhs);
	virtual ~CNormal_Boss() = default;

public:
	HRESULT						Render_UI();
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

	_bool						Collision_StonePillars();
private:
	CRenderer*					m_pRendererCom = nullptr;
	CShader*					m_pShaderCom = nullptr;
	CNavigation*				m_pNavigationCom = nullptr;
	class CBoss_Golem_State*	m_pGolem_State = nullptr;
	CModel::LERPTYPE			m_eLerpType = CModel::LERP_BEGIN;
	CCollider*					m_pLeftArmColliderCom = nullptr;
	CCollider*					m_pRightArmColliderCom = nullptr;

	_bool						m_bRenderUI = false;

	list<CGameObject*>			m_StonePillarList;
private:
	HRESULT						SetUp_Components();
	HRESULT						SetUp_ShaderResources();

public:
	static CNormal_Boss*		Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject*		Clone(const wstring& wstrPrototypeTag, void* pArg = nullptr) override;
	virtual void				Free() override;

};

END