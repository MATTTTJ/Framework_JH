#pragma once
#include "Client_Defines.h"
#include "Base.h"
#include "Transform.h"

BEGIN(Engine)
class CGameObject;
class CGameInstance;
class CState;
class CModel;
class CNavigation;
END

BEGIN(Client)
class CWeapon_State final : public CBase
{
public:
	enum DEFAULT_PISTOL_ANIMATION
	{
		DEFAULT_PISTOL_FIRE, DEFAULT_PISTOL_IDLE, DEFAULT_PISTOL_RELOAD
	};

	enum FLAME_BULLET_ANIMATION
	{	
		FLAME_BULLET_FIRE, FLAME_BULLET_IDLE, FLAME_BULLET_RELOAD
	};

	enum FIREDRAGON_ANIMATION
	{
		PLAYER_NOTTHING, PLAYER_ROAR, PLAYER_THROW, FIRE_DRAGON_FIRE, FIRE_DRAGON_RELOAD, FIRE_DRAGON_IDLE
	};

	enum POISON_ANIMATION
	{
		POISON_FIRE_A, POISON_FIRE_B, POISON_FIRE_C, POISON_IDLE
	};

	enum TESLA_ANIMATION
	{
		TESLA_RELOAD = 0, TESLA_FIRE = 5, TESLA_IDLE = 4
	};

	struct WEAPON_OPTION
	{
		_int		iCurBullet = 0;
		_int		iMaxBullet = 0;

		_uint		iAttack = 0;
		wstring		wstrWeaponName = L"";
	};

	enum WEAPONTYPE { DEFAULT_PISTOL, FLAME_BULLET, FIRE_DRAGON, POISON, WEAPONTYPE_END };
	enum WEAPONNUMBER { WEAPON_FIRST, WEAPON_SECOND, WEAPON_THIRD, WEAPON_NUMBEREND };
public:
	CWeapon_State();
	virtual ~CWeapon_State() = default;

public:
	const _uint&				Get_CurWeaponBulletCnt(wstring WeaponName);
	vector<class CBullet*>		Get_LiveBullet() { return m_vecBullet; }

	HRESULT						Initialize(class CPlayer* pPlayer, CState* pStateMachineCom, CModel* pModel, CTransform* pTransform, CNavigation* pNavigation);
	void						Tick(_double dTimeDelta);
	void						Late_Tick(_double dTimeDelta);
	
private:
	HRESULT						SetUp_State_Weapon_Idle(void);
	HRESULT						SetUp_State_Fire(void);
	HRESULT						SetUp_state_Reload(void);
	HRESULT						SetUp_State_Throw(void);
	HRESULT						SetUp_State_Roar(void);

private:
	void						Start_Weapon_Idle(_double TimeDelta);
	void						Start_Fire(_double TimeDelta);
	void						Start_Reload(_double TimeDelta);
	void						Start_Throw(_double TimeDelta);
	void						Start_Roar(_double TimeDelta);

private:
	void						Tick_Idle(_double TimeDelta);
	void						Tick_Fire(_double TimeDelta);
	void						Tick_Reload(_double TimeDelta);
	void						Tick_Throw(_double TimeDelta);
	void						Tick_Roar(_double TimeDelta);

private:
	void						End_Common(_double TimeDelta);
	void						End_Reload(_double TimeDelta);
private:
	_bool						None_Input(void);
	_bool						KeyInput_None(void);
	_bool						KeyInput_R(void);
	_bool						KeyInput_Q(void);
	_bool						KeyInput_E(void);
	_bool						MouseInput_None(void);
	_bool						MouseInput_LB(void);
	_bool						Animation_Finish(void);

private:
	class CGameInstance*		m_pGameInstance = nullptr;

private:
	class CPlayer*				m_pPlayer = nullptr;
	CState*						m_pState = nullptr;
	CModel*						m_pModelCom = nullptr;
	CTransform*					m_pTransformCom = nullptr;
	CNavigation*				m_pNavigationCom = nullptr;

	_bool						m_bDeadOnce = false;
private:
	WEAPON_OPTION				m_tWeaponOption[WEAPONTYPE_END];
	vector<class CBullet*>		m_vecBullet;
	// _bool						m_bIsWeaponNumber[WEAPON_NUMBEREND] = { false };
	// WEAPONNUMBER				m_eType;
public:
	static CWeapon_State*		Create(class CPlayer* pPlayer, CState* pStateMachineCom, CModel * pModel, CTransform * pTransform, CNavigation* pNavigation);
	virtual void				Free() override;
};


END
