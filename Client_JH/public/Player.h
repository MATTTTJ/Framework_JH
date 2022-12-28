#pragma once
#include <Model.h>

#include "Client_Defines.h"
#include "GameObject.h"
#include "Player_State.h"
#include "Weapon_State.h"

BEGIN(Engine)
	class CShader;
class CRenderer;
class CCollider;
class CModel;
class CNavigation;
class CAnimation;
class CState;
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

	typedef struct tagPlayerOption
	{
		_uint							m_iHp;
		_uint							m_iMaxHp;
		_uint							m_iShieldPoint;
		_uint							m_iMaxShieldPoint;
		_uint							m_iGold;

		_uint							m_iPistol_BulletCnt; // DEFAULT, FLAME, POISON 종류 
		_uint							m_iInjector_BulletCnt; // DRAGON 종류 
		_uint							m_iRifle_BulletCnt; // Rifle 종류 
	}PLAYEROPTION;

private:
	CPlayer(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CPlayer(const CPlayer& rhs);
	virtual ~CPlayer() = default;
	friend CPlayer_State;
	friend CWeapon_State;
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
	vector<CGameObject*>	m_vecPlayerParts;

	CModel*					m_pWeaponModelCom[WEAPON_END] = { nullptr };
	WEAPONDESC				m_tWeaponDesc[WEAPON_END];
	wstring					m_wstrCurWeaponName = L"";

	CNavigation*			m_pNavigationCom = nullptr;

	_uint					m_iPoisonAttCnt = 0;

	CState*					m_pState = nullptr;
	class CPlayer_State*	m_pPlayerState = nullptr;
	class CWeapon_State*	m_pWeaponState = nullptr;

private: // State
	CModel::LERPTYPE		m_eLerpType = CModel::LERP_BEGIN;

	_bool					m_bJump = false;
	_float					m_fGravity;
	_float					m_fInitJumpSpeed;
	_float					m_fCurJumpSpeed;

	_bool					m_bDash = false;
	_float					m_fMaxDashTickCount;
	_float					m_fCurDashTickCount;

	PLAYEROPTION			m_PlayerOption;
private:
	HRESULT					SetUp_Components();
	HRESULT					SetUp_ShaderResources();

public:
	static	CPlayer*		Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject*	Clone(const wstring& wstrPrototypeTag, void* pArg = nullptr) override;
	virtual void			Free() override;
};

END