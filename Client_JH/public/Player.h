#pragma once
#include <Model.h>
#include "Client_Defines.h"
#include "GameObject.h"
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
	enum COLLIDERTYPE { COLLIDER_AABB, COLLIDER_OBB, COLLIDER_SPHERE, COLLIDER_MUZZLE, COLLIDERTYPE_END };

	enum PLAYERUITYPE { UI_BASE, UI_HP, UI_SHIELD, UI_BULLETCOUNT, UI_SKILL, UI_THROW, UI_DASH, UI_END };

	enum WEAPONTYPE { WEAPON_DEFAULT, WEAPON_FLAMEBULLET, WEAPON_FIREDRAGON, WEAPON_POISON, WEAPON_END };


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
		_uint							m_iEmeraldCnt;
		_uint							m_iGold;

		_uint							m_iPistol_BulletCnt; // DEFAULT, FLAME, POISON 종류 
		_uint							m_iInjector_BulletCnt; // DRAGON 종류 
		_uint							m_iRifle_BulletCnt; // Rifle 종류
		_uint							m_iThrowCnt;
		wstring							m_wstrWeaponNumber;
		wstring							m_wstrCurWeaponName;
	}PLAYEROPTION;

private:
	CPlayer(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CPlayer(const CPlayer& rhs);
	virtual ~CPlayer() = default;
	friend CWeapon_State;
public:
	CCollider*				Get_OBBPtr() { return m_pColliderCom[COLLIDER_OBB]; }
	_matrix					Get_BoneMatrix(const string& strBoneName);
	_matrix					Get_OffsetMatrix(const string& strBoneName);
	_matrix					Get_PivotMatrix();
	_vector					Get_TransformState(CTransform::STATE eState);
	_matrix					Get_CombindMatrix(const string& strBoneName);
	const wstring&			Get_CurWeaponName(void) const { return m_PlayerOption.m_wstrCurWeaponName; }
	const wstring&			Get_CurWeaponNumber(void) const { return m_PlayerOption.m_wstrWeaponNumber; }
	CWeapon_State*			Get_WeaponStatePtr(void) { return m_pWeaponState; }

	CModel*					Get_CurWeaponModelCom(void);
	_uint					Get_EmeraldCnt(void) { return m_PlayerOption.m_iEmeraldCnt; }
	_uint					Get_GoldCnt(void) { return m_PlayerOption.m_iGold; }
	_uint					Get_ThrowCnt(void) { return m_PlayerOption.m_iThrowCnt; }
	_uint					Get_RifleBulletCnt(void) { return m_PlayerOption.m_iRifle_BulletCnt; }
	_uint					Get_PistolBulletCnt(void) { return m_PlayerOption.m_iPistol_BulletCnt; }
	_uint					Get_InjectorBulletCnt(void) { return m_PlayerOption.m_iInjector_BulletCnt; }

	void					Set_Camera(_double TimeDelta);
	CGameObject*			Collision_AimBox_To_Monster();

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
	CCollider*				m_pFirstAimColliderCom = nullptr;
	CCollider*				m_pSecondAimColliderCom = nullptr;

	class CPlayerUI_Base*			m_pPlayerUICom[UI_END] = { nullptr };

	vector<CGameObject*>	m_vecPlayerParts;

	CModel*					m_pWeaponModelCom[WEAPON_END] = { nullptr };
	WEAPONDESC				m_tWeaponDesc[WEAPON_END];

	CNavigation*			m_pNavigationCom = nullptr;

	_uint					m_iPoisonAttCnt = 0;

	CState*					m_pState = nullptr;
	class CPlayer_State*	m_pPlayerState = nullptr;
	class CWeapon_State*	m_pWeaponState = nullptr;

private: // State
	CModel::LERPTYPE		m_eLerpType = CModel::LERP_BEGIN;
	_float4					m_vCamLook;
	_float4					m_vCamLookAt;
	_float3					m_vAimColliderPos;
	_bool					m_bJump = false;
	_float					m_fGravity;
	_float					m_fInitJumpSpeed;
	_float					m_fCurJumpSpeed;

	_bool					m_bDash = false;
	_float					m_fMaxDashTickCount;
	_float					m_fCurDashTickCount;

	PLAYEROPTION			m_PlayerOption;
	vector<CGameObject*>	m_vecPlayerUI;

private:
	HRESULT					SetUp_Components();
	HRESULT					SetUp_ShaderResources();
	HRESULT					Ready_UI();

public:
	static	CPlayer*		Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject*	Clone(const wstring& wstrPrototypeTag, void* pArg = nullptr) override;
	virtual void			Free() override;
};

END