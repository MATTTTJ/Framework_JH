#pragma once
#include "Client_Defines.h"
// #include "Base.h"
//
// BEGIN(Client)
//
// class CPlayer_State final :	public CBase
// {
// public:
// 	typedef struct tagPlayerWeaponType
// 	{
// 		wstring m_wstrCurWeaponName = L"";
//
// 	}PLAYERWEAPONTYPES;
//
// 	enum DEFAULT_PISTOL_ANIMATION
// 	{
// 		DEFAULT_PISTOL_IDLE, DEFAULT_PISTOL_RELOAD, DEFAULT_PISTOL_FIRE
// 	};
// 	
// 	enum FLAME_BULLET_ANIMATION
// 	{
// 		FLAME_BULLET_RELOAD, FLAME_BULLET_FIRE, FLAME_BULLET_IDLE
// 	};
// 	
// 	enum FIREDRAGON_ANIMATION
// 	{
// 		FIRE_DRAGON_RELOAD, FIRE_DRAGON_IDLE, FIRE_DRAGON_FIRE
// 	};
// 	
// 	enum POISON_ANIMATION
// 	{
// 		POISON_FIRE_A, POISON_FIRE_B, POISON_FIRE_C, POISON_IDLE
// 	};
// 	
// 	enum TESLA_ANIMATION
// 	{
// 		TESLA_RELOAD = 0, TESLA_FIRE = 5, TESLA_IDLE = 4
// 	};
//
// public:
// 	CPlayer_State();
// 	virtual ~CPlayer_State() = default;
//
// public:
// 	HRESULT		Initialize(class CPlayer* pPlayer);
//
//
// private:
// 	HRESULT		SetUp_State_Idle(void);
// 	HRESULT		SetUp_State_Walk(void);
// 	HRESULT		SetUp_State_Dash(void);
// 	HRESULT		SetUp_State_Fire(void);
// 	HRESULT		SetUp_State_Reload(void);
// 	HRESULT		SetUp_State_Damaged(void);
// 	HRESULT		SetUp_State_SelectPage(void);
// 	// HRESULT		SetUp_State_Throw(void);
// 	// HRESULT		SetUp_State_Roar(void);
//
// 	// State :: Start
// private:
// 	void		Start_Idle(_double TimeDelta);
// 	void		Start_Fire(_double TimeDelta);
// 	void		Strat_Reload(_double TimeDelta);
// 	void		Start_Walk_F(_double TimeDelta);
// 	void		Start_Walk_B(_double TimeDelta);
// 	void		Start_Walk_L(_double TimeDelta);
// 	void		Start_Walk_R(_double TimeDelta);
// 	void		Start_Walk_FL(_double TimeDelta);
// 	void		Start_Walk_FR(_double TimeDelta);
// 	void		Start_Walk_BL(_double TimeDelta);
// 	void		Start_Walk_BR(_double TimeDelta);
// 	void		Start_Dash_F(_double TimeDelta);
// 	void		Start_Dash_B(_double TimeDelta);
// 	void		Start_Dash_L(_double TimeDelta);
// 	void		Start_Dash_R(_double TimeDelta);
// 	void		Start_Dash_FL(_double TimeDelta);
// 	void		Start_Dash_FR(_double TimeDelta);
// 	void		Start_Dash_BL(_double TimeDelta);
// 	void		Start_Dash_BR(_double TimeDelta);
// 	// void		Start_Throw(_double TimeDelta);
// 	// void		Start_Roar(_double TimeDelta);
//
// 	// State :: Tick
// private:
// 	void		Tick_Idle(_double TimeDelta);
// 	void		Tick_Fire(_double TimeDelta);
// 	void		Tick_Reload(_double TimeDelta);
// 	void		Tick_Damaged(_double TimeDelta);
// 	void		Tick_Walk_F(_double TimeDelta);
// 	void		Tick_Walk_B(_double TimeDelta);
// 	void		Tick_Walk_L(_double TimeDelta);
// 	void		Tick_Walk_R(_double TimeDelta);
// 	void		Tick_Walk_FL(_double TimeDelta);
// 	void		Tick_Walk_FR(_double TimeDelta);
// 	void		Tick_Walk_BL(_double TimeDelta);
// 	void		Tick_Walk_BR(_double TimeDelta);
// 	void		Tick_Dash_F(_double TimeDelta);
// 	void		Tick_Dash_B(_double TimeDelta);
// 	void		Tick_Dash_L(_double TimeDelta);
// 	void		Tick_Dash_R(_double TimeDelta);
// 	void		Tick_Dash_FL(_double TimeDelta);
// 	void		Tick_Dash_FR(_double TimeDelta);
// 	void		Tick_Dash_BL(_double TimeDelta);
// 	void		Tick_Dash_BR(_double TimeDelta);
//
// 	// State :: End
// private:
// 	void		End_Common(_double TimeDelta);
//
// 	//State : Changer
// private:
// 	_bool		KeyInput_None(void);
// 	_bool		KeyInput_R(void);
// 	_bool		KeyInput_W(void);
// 	_bool		KeyInput_A(void);
// 	_bool		KeyInput_S(void);
// 	_bool		KeyInput_D(void);
// 	_bool		KeyInput_WA(void);
// 	_bool		KeyInput_WD(void);
// 	_bool		KeyInput_SA(void);
// 	_bool		KeyInput_SD(void);
// 	_bool		KeyInput_Shift(void);
// 	_bool		KeyUp_Shift(void);
// 	_bool		KeyInput_Space(void);
// 	_bool		MouseDown_LB(void);
// 	_bool		MouseCharge_LB(void);
// 	_bool		isFinish(void);
//
// private:
//
//
// private:
// 	class CPlayer*		m_pPlayer;
// 	
// public:
// 	static CPlayer_State*	Create(class CPlayer* pPlayer);
// 	virtual void Free();
//
//
// };
//
// END