#pragma once
#include "Client_Defines.h"
#include "GameObject.h"
#include "Player_State.h"

BEGIN(Engine)
class CShader;
class CRenderer;
class CCollider;
class CModel;
class CNavigation;
END

BEGIN(Client)

class CPlayer final : public CGameObject
{
	enum COLLIDERTYPE { COLLIDER_AABB, COLLIDER_OBB, COLLIDER_SPHERE, COLLIDERTYPE_END };

	enum WEAPONTYPE { WEAPON_DEFAULT, WEAPON_FLAMEBULLET, WEAPON_FIREDRAGON, WEAPON_POISON, WEAPON_END };

	enum DEFAULT_PISTOL_ANIMATION
	{
		DEFAULT_PISTOL_IDLE, DEFAULT_PISTOL_RELOAD, DEFAULT_PISTOL_FIRE
	};

	enum FLAME_BULLET_ANIMATION
	{
		FLAME_BULLET_RELOAD, FLAME_BULLET_FIRE, FLAME_BULLET_IDLE
	};

	enum FIREDRAGON_ANIMATION
	{
		FIRE_DRAGON_RELOAD, FIRE_DRAGON_IDLE, FIRE_DRAGON_FIRE
	};

	enum POISON_ANIMATION
	{
		POISON_FIRE_A, POISON_FIRE_B, POISON_FIRE_C, POISON_IDLE
	};

	enum TESLA_ANIMATION
	{
		TESLA_RELOAD = 0, TESLA_FIRE = 5, TESLA_IDLE = 4
	};

	typedef struct tagWeaponType
	{
		CModel*			m_pWeaponModelCom;
		wstring			m_wstrWeaponName;
	
	}WEAPONDESC;
private:
	CPlayer(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CPlayer(const CPlayer& rhs);
	virtual ~CPlayer() = default;
	friend CPlayer_State;
public:
	_matrix					Get_BoneMatrix(const string& strBoneName);
	_matrix					Get_OffsetMatrix(const string& strBoneName);
	_matrix					Get_PivotMatrix();
	_vector					Get_TransformState(CTransform::STATE eState);


public:
	virtual HRESULT			Initialize_Prototype() override;
	virtual HRESULT			Initialize_Clone(const wstring& wstrPrototypeTag, void* pArg) override;
	virtual void			Tick(_double dTimeDelta) override;
	virtual void			Late_Tick(_double TimeDelta) override;
	virtual HRESULT			Render() override;

private:
	CShader*				m_pShaderCom	= nullptr;
	CRenderer*				m_pRendererCom	= nullptr;
	CModel*					m_pModelCom		= nullptr;
	CCollider*				m_pColliderCom[COLLIDERTYPE_END] = { nullptr };

	CModel*					m_pWeaponModelCom[WEAPON_END] = { nullptr };

	WEAPONDESC				m_tWeaponDesc[WEAPON_END];
	wstring					m_wstrCurWeaponName = L"";


	CNavigation*			m_pNavigationCom = nullptr;


	_uint					m_iPoisonAttCnt = 0;
private:
	vector<CGameObject*>	m_vecPlayerParts;

private:
	HRESULT					SetUp_Components();
	HRESULT					SetUp_ShaderResources();

	HRESULT					Ready_Parts();

public:
	static	CPlayer*		Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject*	Clone(const wstring& wstrPrototypeTag, void* pArg = nullptr) override;
	virtual void			Free() override;
};

END