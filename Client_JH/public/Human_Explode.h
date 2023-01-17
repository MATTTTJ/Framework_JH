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

class CHuman_Explode final  :	public CMonster
{
	friend class CHuman_Explode_State;

public:
	CHuman_Explode(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CHuman_Explode(const CHuman_Explode& rhs);
	virtual ~CHuman_Explode() = default;

public:
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

	virtual void				Set_On_NaviMesh() override;

	virtual void				Collision_Body(CBullet* pBullet) override;
	virtual void				Collision_Head(CBullet* pBullet) override;
	virtual void				Collision_Hide(CBullet* pBullet) override;
	virtual void				Collision_PlayerEyes() override;

private:
	CRenderer*					m_pRendererCom = nullptr;
	CShader*					m_pShaderCom = nullptr;
	CNavigation*				m_pNavigationCom = nullptr;
	class CHuman_Explode_State*	m_pHuman_Explode_State = nullptr;

private:
	HRESULT						SetUp_Components();
	HRESULT						SetUp_ShaderResources();


public:
	static CHuman_Explode*	Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject*	Clone(const wstring& wstrPrototypeTag, void* pArg = nullptr) override;
	virtual void			Free() override;
};



END
