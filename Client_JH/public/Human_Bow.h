#pragma once
#include "Client_Defines.h"
#include "Monster.h"

BEGIN(Engine)
class CRenderer;
class CShader;
class CNavigation; 
END

BEGIN(Client)

class CHuman_Bow final  :	public CMonster
{
	friend class CHuman_Bow_State;

public:
	CHuman_Bow(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CHuman_Bow(const CHuman_Bow& rhs);
	virtual ~CHuman_Bow() = default;

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
	virtual HRESULT			Render_ShadowDepth();
	virtual HRESULT			Render_OutLineFlag();
public:
	_bool						Collision_Detected(CCollider* pOtherCollider);
	_bool						Collider_AttRange(CCollider* pOtherCollider);


	virtual void				Set_On_NaviMesh() override;

	virtual void				Collision_Body(CBullet* pBullet) override;
	virtual void				Collision_Head(CBullet* pBullet) override;
	virtual void				Collision_Hide(CBullet* pBullet) override;
	virtual void				Collision_PlayerEyes() override;

private:
	HRESULT						SetUp_Components();
	HRESULT						SetUp_ShaderResources();

private:
	CRenderer*					m_pRendererCom = nullptr;
	CShader*					m_pShaderCom = nullptr;
	class CHuman_Bow_State*		m_pHuman_Bow_State = nullptr;

	// CTexture*					m_pNormalTexCom = nullptr;
public:
	static CHuman_Bow*		Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject*	Clone(const wstring& wstrPrototypeTag, void* pArg = nullptr) override;
	virtual void			Free() override;
};



END
