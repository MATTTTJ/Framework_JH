#pragma once
#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CShader;
class CRenderer;
class CModel;
END

BEGIN(Client)
class CMagicStone final : public CGameObject
{
public:
	enum STONETYPE { STONE_A, STONE_B, STONE_C, STONE_D ,STONETYPE_END};

	typedef struct tagMagicStoneDesc
	{
		GAMEOBJECTDESC	GameObjectDesc;

		CGameObject*	m_pTarget = nullptr;
	}MAGICSTONEDESC;

private:
	CMagicStone(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CMagicStone(const CMagicStone& rhs);
	virtual ~CMagicStone() = default;

public:
	virtual HRESULT			Initialize_Prototype() override;
	virtual HRESULT			Initialize_Clone(const wstring& wstrPrototypeTag, void* pArg) override;
	virtual void			Tick(_double TimeDelta) override;
	virtual void			Late_Tick(_double TimeDelta) override;
	virtual HRESULT			Render() override;
	void					Ready_DangerEffect();
	_bool					Collision_To_Bullet();
	void					Fire_To_Player(_double TimeDelta);
	virtual HRESULT			Render_ShadowDepth();
private:
	CShader*				m_pShaderCom = nullptr;
	CRenderer*				m_pRendererCom = nullptr;
	CModel*					m_pModelCom[STONETYPE_END] = {nullptr};
	_int					m_iRand = 0;
	CCollider*				m_pMagicStoneColliderCom = nullptr;

	_float					m_fMinSize = 0.1f;
	_float					m_fMaxSize = 1.f;
	_bool					m_bCanGrow = true;

	_float4					m_vDir;
	
	_float					m_fCurFireCoolTime = 0.f;
	_float					m_fFireCoolTime = 15.f;
	_bool					m_bFire = false;

	class CPlayer*			m_pPlayer = nullptr;

	MAGICSTONEDESC	m_tMagicStoneDesc;
	list<CGameObject*>		m_BulletList;
private:
	HRESULT					SetUp_Components();
	HRESULT					SetUp_ShaderResources();

public:
	static	CMagicStone*		Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject*	Clone(const wstring& wstrPrototypeTag, void* pArg = nullptr) override;
	virtual void			Free() override;
};

END