#pragma once
#include "Client_Defines.h"
#include "Monster.h"

BEGIN(Engine)
class CRenderer;
class CShader;
class CNavigation; 
END

BEGIN(Client)

class CElite_Bug final  :	public CMonster
{
	friend class CElite_Bug_State;

public:
	CElite_Bug(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CElite_Bug(const CElite_Bug& rhs);
	virtual ~CElite_Bug() = default;

public:
	void						Get_Damaged() { m_tMonsterOption.MonsterDesc.m_iHP -= 30; }
	virtual HRESULT				Initialize_Prototype() override;
	virtual HRESULT				Initialize_Clone(const wstring& wstrPrototypeTag, void* pArg) override;
	virtual void				Tick(_double TimeDelta) override;
	virtual void				Late_Tick(_double TimeDelta) override;
	virtual HRESULT				Render() override;
	HRESULT						Ready_UI();

public:
	virtual void			Collider_Tick(_double TimeDelta) override;

public:
	_bool						Collision_Detected(CCollider* pOtherCollider);
	_bool						Collider_AttRange(CCollider* pOtherCollider);


	virtual void				Set_On_NaviMesh() override;

	virtual void				Collision_Body(CBullet* pBullet) override;
	virtual void				Collision_Head(CBullet* pBullet) override;
	virtual void				Collision_Hide(CBullet* pBullet) override;
	virtual void				Collision_PlayerEyes() override;
	virtual HRESULT				Render_ShadowDepth();
	virtual HRESULT				Render_OutLineFlag();
private:
	HRESULT						SetUp_Components();
	HRESULT						SetUp_ShaderResources();

private:
	CRenderer*					m_pRendererCom = nullptr;
	CShader*					m_pShaderCom = nullptr;
	class CElite_Bug_State*		m_pElite_Bug_State = nullptr;

public:
	static CElite_Bug*	Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject*	Clone(const wstring& wstrPrototypeTag, void* pArg = nullptr) override;
	virtual void			Free() override;
};



END
