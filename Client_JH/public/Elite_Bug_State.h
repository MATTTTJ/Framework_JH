#pragma once
#include "Client_Defines.h"
#include "Base.h"
#include "Elite_Bug.h"

BEGIN(Engine)
class CGameObject;
class CGameInstance;
class CState;
class CModel;
class CNavigation;
class CTransform;
END

BEGIN(Client)
class CElite_Bug_State final : public CBase
{
public:
	enum DAMAGEDTYPE { HITHEAD, HITBODY, HIT, HIT_END };

	enum ANIMATION
	{
		BUG_STAND,
		BUG_IN_COMBAT_IDLE,
		BUG_PATROL,
		BUG_HITBODY,
		BUG_RUN_F,
		BUG_JUMP,BUG_LAND,
		BUG_SPAWN_JUNIOR,
		BUG_READYRUSH,BUG_READYRUSH_LOOP,
		BUG_STARTRUSH, BUG_RUSH_LOOP, BUG_RUSH_END,
		BUG_ANIMEND
	};

public:
	CElite_Bug_State();
	virtual ~CElite_Bug_State() = default;

public:
	const _bool&				Is_DamagedState() { return m_bDamaged[HIT]; }
	void						Set_DamagedState(DAMAGEDTYPE eType) { m_bDamaged[eType] = true; }
	HRESULT						Initialize(CElite_Bug* pOwner, CState* pStateMachineCom, CModel* pModel, CTransform* pTransform, CNavigation* pNavigation);
	void						Tick(_double dTimeDelta);
	void						Late_Tick(_double dTimeDelta);
	void						Reset_Damaged();


private:
	HRESULT						SetUp_State_Idle();
	HRESULT						SetUp_State_No_Detected();
	HRESULT						SetUp_State_Death();

private:
	class CGameInstance*		m_pGameInstance = nullptr;

private:
	CPlayer*					m_pPlayer = nullptr;

	class CElite_Bug*			m_pMonster = nullptr;
	CState*						m_pState = nullptr;
	CModel*						m_pModelCom = nullptr;
	CTransform*					m_pTransformCom = nullptr;
	CNavigation*				m_pNavigationCom = nullptr;

	_bool						m_bPlayAnimation = {true};
	_bool						m_bDeadOnce = {false};
	_bool						m_bFirstState[CElite_Bug::FIRSTSTATE_END] = { false, false, false };

	_bool						m_bDamaged[HIT_END] = { false, false, false };
	_float4						m_fLastPos;
private:
	// 리틀 버그 생성 한번씩만
	_bool						m_bSpawnOnce = false;

	//Patrol
	_float						m_fPatrolTurnTime = 2.5f;
	_float						m_fCurPatrolTurnTime = 0.f;
	_bool						m_bTurnPatrolDirection = false;

	// 리틀 버그 생성
	_float						m_fCurSpawnJuniorCoolTime = {0.f};
	_float						m_fSpawnJuniorCoolTime = { 15.f };
	_bool						m_bCanSpawnJunior = {false};

	// 차징 시간
	_float						m_fCurChargingTime = 0.f;
	_float						m_fChargingTime = 3.f;
	_bool						m_bCanRushStart = false;

	// 러쉬 시간
	_float						m_fCurRushTime = 0.f;
	_float						m_fRushTime = 1.0f;
	_bool						m_bStopRush = false;


	// 패턴 쿨타임
	_float						m_fCurHideCoolTime = {0.f};
	_float						m_fHideCoolTime = {8.f};
	_bool						m_bCanHide = {false};

	// 공격
	_float						m_fCurAttackCoolTime = {0.f};
	_float						m_fAttackCoolTime = {10.f};
	_bool						m_bCanAttack = {false};
	_bool						m_bAttackOnce = {false};
	// 피격 애니메이션
	_float						m_fCurDamagedAnimCoolTime = {0.f};
	_float						m_fDamagedAnimCoolTime = {5.f};
	_bool						m_bDamagedAnim = {false};

private:
	void						Start_Idle(_double dTimeDelta);
	void						Start_Run(_double dTimeDelta);
	void						Start_No_Detected(_double dTimeDelta);
	void						Start_Death(_double dTimeDelta);
	void						Start_Ready_Rush(_double dTimeDelta);
	void						Start_Ready_Rush_Loop(_double dTimeDelta);
	void						Start_Starting_Rush(_double dTimeDelta);
	void						Start_Rush_Loop(_double dTimeDelta);
	void						Start_Ending_Rush(_double dTimeDelta);
	void						Start_Spawn_Junior(_double dTimeDelta);

private:
	void						Tick_Idle(_double dTimeDelta);
	void						Tick_Run(_double dTimeDelta);
	void						Tick_Patrol(_double dTimeDelta);
	void						Tick_Death(_double dTimeDelta);
	void						Tick_Ready_Rush(_double dTimeDelta);
	void						Tick_Ready_Rush_Loop(_double dTimeDelta);
	void						Tick_Starting_Rush(_double dTimeDelta);
	void						Tick_Rush_Loop(_double dTimeDelta);
	void						Tick_Ending_Rush(_double dTimeDelta);
	void						Tick_Spawn_Junior(_double dTimeDelta);

private:
	void						End_Common(_double TimeDelta);
	void						End_Idle(_double dTimeDelta);
	void						End_Run(_double dTimeDelta);
	void						End_Patrol(_double dTimeDelta);
	void						End_Death(_double dTimeDelta);
	void						End_Ready_Rush(_double dTimeDelta);
	void						End_Ready_Rush_Loop(_double dTimeDelta);
	void						End_Starting_Rush(_double dTimeDelta);
	void						End_Rush_Loop(_double dTimeDelta);
	void						End_Ending_Rush(_double dTimeDelta);
	void						End_Spawn_Junior(_double dTimeDelta);


private:
	_bool						Animation_Finish();
	_bool						Player_Detected(); // NoDetected -> Detected Anim Play
	_bool						Player_DetectedAndFar(); // Stand State -> Player Detected (Follow)
														 // _bool						Player_DetectedAndFar_AnimFinish(); 
	_bool						Player_DetectedAndClose(); // Ground Spawn
	_bool						Player_CloseAndCanAttack(); // Ground Spawn
	_bool						Player_NotDetected(); // When NoDetected (Collider)
	_bool						Is_Damaged();
	_bool						IS_Dead();
	_bool						Is_CanSpawnJunior();
	_bool						Is_ChargingTimeForRush();
	_bool						Is_StopRush();
	// _bool					Check_IdleFinishCount();
	// _bool					Check_WalkTime();
public:
	static CElite_Bug_State*	Create(class CElite_Bug* pOwner, CState* pStateMachineCom, CModel* pModel, CTransform* pTransform, CNavigation* pNavigation);
	virtual void				Free() override;
};

END


// 엘리트 버그는 기본공격이 없고 계속해서 돌진을 함
// 돌진하는 로직은 일정 시간뒤에 일정시간동안 돌진하는 것으로 함
// 돌진하는 동안은 방향 변경 불가 
