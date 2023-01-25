#pragma once
#include "Client_Defines.h"
#include "Base.h"
#include "Normal_Boss.h"

BEGIN(Engine)
class CGameObject;
class CGameInstance;
class CState;
class CModel;
class CNavigation;
class CTransform;
END

BEGIN(Client)
class CBoss_Golem_State final : public CBase
{
public:
	enum DAMAGEDTYPE { HITHEAD, HITBODY, HIT, HIT_END };

	enum ANIMATION
	{
		GOLEM_DIE,
		GOLEM_INCOMBAT_IDLE,
		GOLEM_INTRO2,
		GOLEM_INTRO1,
		GOLEM_INTRO0,
		GOLEM_FIRE_LAZER, GOLEM_READY_LAZER,
		GOLEM_FIRE_RIGHTARM, GOLEM_FIRE_LEFTARM, GOLEM_END_FIREARM, GOLEM_READY_ARMFIRE,
		GOLEM_END_MAGICSTONE, GOLEM_START_MAGICSTONE, GOLEM_START_SPAWNPILLARS, GOLEM_MELEE_N_SPAWNPILLARS,
		GOLEM_ANIMEND
	};

	// 1. GOLEM_DIE
	// 	2. GOLEM_INCOMBAT_IDLE
	// 	3. GOLEM_INTRO2
	// 	4. GOLEM_INTRO1
	// 	5. GOLEM_FIRE_LAZER
	// 	6. GOLEM_READY_LAZER
	// 	7. GOLEM_FIRE_RIGHTARM
	// 	8. GOLEM_FIRE_LEFTARM
	// 	9. GOLEM_END_FIREARM
	// 	10. GOLEM_READY_ARMFIRE
	// 	12. GOLEM_END_MAGICSTONE
	// 	13. GOLEM_START_MAGICSTONE
	// 	14. GOLEM_START_SPAWNPILLARS
	// 	15. GOLEM_MELEE_N_SPAWNPILLARS

public:
	CBoss_Golem_State();
	virtual ~CBoss_Golem_State() = default;

public:
	const _bool&				Is_DamagedState() { return m_bDamaged[HIT]; }
	void						Set_DamagedState(DAMAGEDTYPE eType) { m_bDamaged[eType] = true; }
	HRESULT						Initialize(CNormal_Boss* pOwner, CState* pStateMachineCom, CModel* pModel, CTransform* pTransform, CNavigation* pNavigation);
	void						Tick(_double dTimeDelta);
	void						Late_Tick(_double dTimeDelta);
	void						Reset_Damaged();

private:
	HRESULT						SetUp_State_No_Detected();
	HRESULT						SetUp_State_Idle();

private:
	class CGameInstance*		m_pGameInstance = nullptr;
private:
	CPlayer*					m_pPlayer = nullptr;
	class CFadeInOut*			m_pFadeInOut = nullptr;
	class CNormal_Boss*			m_pMonster = nullptr;
	CState*						m_pState = nullptr;
	CModel*						m_pModelCom = nullptr;
	CTransform*					m_pTransformCom = nullptr;
	CNavigation*				m_pNavigationCom = nullptr;
	class CDynamic_Camera*		m_pDynamic_Camera = nullptr;
	CBullet*					m_pBullet = nullptr;
	_bool						m_bPlayAnimation = true;
	_bool						m_bDeadOnce = false;
	_bool						m_bFirstState[CNormal_Boss::FIRSTSTATE_END] = { false, false, false };

	_bool						m_bDamaged[HIT_END] = { false, false, false };
	_bool						m_bCamSet = false;


	
private:					// Pattern CoolTime 
							// 구르기
	_float						m_fCurLazerCoolTime = 0.f;
	_float						m_fLazerCoolTime = 3.f;
	_bool						m_bShotLazer = false;
	_uint						m_iLazerCount = 0;
	// IDLE 상태에서 3초 뒤 레이저

	_uint						m_iSpawnStonePillarsCount = 0;
	_bool						m_bDestroy_Pillars_Once = false;

	// 레이저 공격임
	_bool						m_bSpawnOnce = false;

	_uint						m_iSpawnMagicStone = 0;

private:
	// vector<class CGameObject*>	m_vecBullet;

private:
	void						Start_Common(_double dTimeDelta) {}
	void						Start_Intro0(_double dTimeDelta);
	void						Start_Intro1(_double dTimeDelta);
	void						Start_Intro2(_double dTimeDelta);
	void						Start_Idle(_double dTimeDelta);
	void						Start_Ready_Arm_Fire(_double dTimeDelta);
	void						Start_Fire_Arm_RightArm(_double dTimeDelta);
	void						Start_Fire_Arm_LeftArm(_double dTimeDelta);
	void						Start_End_Arm_Fire(_double dTimeDelta);
	void						Start_Ready_Lazer(_double dTimeDelta);
	void						Start_Fire_Lazer(_double dTimeDelta);
	void						Start_Ready_MagicStone(_double dTimeDelta);
	void						Start_End_MagicStone(_double dTimeDelta);
	void						Start_Start_SpawnPillars(_double dTimeDelta);
	void						Start_Melee_And_Ready_SpawnPillars(_double dTimeDelta);
	void						Start_Dead(_double dTimeDelta);




private:
	void						Tick_Common(_double dTimeDelta);
	void						Tick_Intro0(_double dTimeDelta);
	void						Tick_Intro1(_double dTimeDelta);
	void						Tick_Intro2(_double dTimeDelta);
	void						Tick_Idle(_double dTimeDelta);
	void						Tick_Ready_Arm_Fire(_double dTimeDelta);
	void						Tick_Fire_Arm_RightArm(_double dTimeDelta);
	void						Tick_Fire_Arm_LeftArm(_double dTimeDelta);
	void						Tick_End_Arm_Fire(_double dTimeDelta);
	void						Tick_Ready_Lazer(_double dTimeDelta);
	void						Tick_Fire_Lazer(_double dTimeDelta);
	void						Tick_Ready_MagicStone(_double dTimeDelta);
	void						Tick_End_MagicStone(_double dTimeDelta);
	void						Tick_Start_SpawnPillars(_double dTimeDelta);
	void						Tick_Melee_And_Ready_SpawnPillars(_double dTimeDelta);
	void						Tick_Dead(_double dTimeDelta);

private:
	void						End_Common(_double TimeDelta);
	void						End_Intro0(_double TimeDelta);
	void						End_Intro1(_double dTimeDelta);
	void						End_Intro2(_double dTimeDelta);
	void						End_Idle(_double dTimeDelta);
	void						End_Ready_Arm_Fire(_double dTimeDelta);
	void						End_Fire_Arm_RightArm(_double dTimeDelta);
	void						End_Fire_Arm_LeftArm(_double dTimeDelta);
	void						End_End_Arm_Fire(_double dTimeDelta);
	void						End_Ready_Lazer(_double dTimeDelta);
	void						End_Fire_Lazer(_double dTimeDelta);
	void						End_Ready_MagicStone(_double dTimeDelta);
	void						End_End_MagicStone(_double dTimeDelta);
	void						End_Start_SpawnPillars(_double dTimeDelta);
	void						End_Melee_And_Ready_SpawnPillars(_double dTimeDelta);
	void						End_Dead(_double dTimeDelta);

private:
	_bool						Animation_Finish();

	_bool						Player_Detected(); // NoDetected -> Detected Anim Play
	_bool						Player_DetectedAndFar(); // Stand State -> Player Detected (Follow)
	_bool						Player_DetectedAndClose(); // Ground Spawn

	_bool						Player_CloseAndCanAttack(); // Ground Spawn
	_bool						Player_FarAndCanAttack(); // Ground Spawn

	_bool						IS_Dead();

	_bool						Is_Can_Spawn_Pillars();
	_bool						Is_Can_Spawn_MagicStone();
	_bool						Is_Can_Fire_Lazer();
	_bool						Is_Can_Fire_Arm();
	_bool						Is_Can_Spawn_Monster();
	_bool						Is_Pillars_Empty();

	void						Spawn_StonePillars();
	void						Spawn_MagicStone();
	void						Spawn_RocketHand(_bool TrueIsLeft);

public:
	static CBoss_Golem_State*	Create(class CNormal_Boss* pOwner, CState* pStateMachineCom, CModel* pModel, CTransform* pTransform, CNavigation* pNavigation);
	virtual void				Free() override;
};




END
