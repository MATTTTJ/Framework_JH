#pragma once
#include "Client_Defines.h"
#include "Base.h"
#include "Transform.h"

BEGIN(Engine)
class CGameInstance;
class CState;
class CModel;
END

BEGIN(Client)
class CWeapon_State final : public CBase
{
public:
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

	struct WEAPON_OPTION
	{
		_uint		iCurBullet;
		_uint		iMaxBullet;

		_uint		iAttack;
		wstring		wstrWeaponName;
	};

	enum WEAPONTYPE { DEFAULT_PISTOL, FLAME_BULLET, FIRE_DRAGON, POISON, WEAPONTYPE_END };

public:
	CWeapon_State();
	virtual ~CWeapon_State() = default;

public:
	HRESULT						Initialize(class CPlayer* pPlayer, CState* pStateMachineCom, CModel* pModel, CTransform* pTransform);
	void						Tick(_double dTimeDelta);
	void						Late_Tick(_double dTimeDelta);
private:
	HRESULT						SetUp_State_Idle(void);
	HRESULT						SetUp_State_Fire(void);
	HRESULT						SetUp_state_Reload(void);
	HRESULT						SetUp_State_Throw(void);
	HRESULT						SetUp_State_Roar(void);

private:
	void						Start_Idle(_double TimeDelta);
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

private:
	WEAPON_OPTION				m_tWeaponOption[WEAPONTYPE_END];


public:
	static CWeapon_State*		Create(class CPlayer* pPlayer, CState* pStateMachineCom, CModel * pModel, CTransform * pTransform);
	virtual void				Free() override;
};




END
