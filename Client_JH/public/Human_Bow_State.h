#pragma once
#include "Client_Defines.h"
#include "Base.h"
#include "Human_Bow.h"

BEGIN(Engine)
class CGameObject;
class CGameInstance;
class CState;
class CModel;
class CNavigation;
class CTransform;
END

BEGIN(Client)
class CHuman_Bow_State final : public CBase
{
public:
	CHuman_Bow_State();
	virtual ~CHuman_Bow_State() = default;

public:
	enum DAMAGEDTYPE { HITHEAD, HITBODY, HIT, HIT_END };

	enum ANIMATION
	{
		BOW_JUSTSTAND,
		BOW_IN_COMBAT_IDLE, BOW_SQUAT,
		BOW_RUN_F,
		BOW_RUN_PATROL,
		BOW_RAPID_SHOT, BOW_HITHEAD,
		BOW_HIDE_LEFT, BOW_HIDE_RIGHT,
		BOW_JUMP,
		BOW_LAND, BOW_MELEE_ATTACK,
		BOW_WALK_F,
		BOW_WALK_R, BOW_WALK_B,
		BOW_WALK_L, BOW_FIRE,
		BOW_RELOAD,
		BOW_HITBODY,
		BOW_ANIMEND
	};

public:
	const _bool&				Is_DamagedState() { return m_bDamaged[HIT]; }
	void						Set_DamagedState(DAMAGEDTYPE eType) { m_bDamaged[eType] = true; }
	HRESULT						Initialize(CHuman_Bow* pOwner, CState* pStateMachineCom, CModel* pModel, CTransform* pTransform, CNavigation* pNavigation);
	void						Tick(_double dTimeDelta);
	void						Late_Tick(_double dTimeDelta);
	void						Reset_Damaged();



private:
	HRESULT						SetUp_State_Idle();
	HRESULT						SetUp_State_Run();
	HRESULT						SetUp_State_JustStand();
	HRESULT						SetUp_State_HitBody();
	HRESULT						SetUp_State_Patrol();
	HRESULT						SetUp_State_Detected();
	HRESULT						SetUp_State_UpSpawn();
	HRESULT						SetUp_State_Attack_A();
	HRESULT						SetUp_State_Hide();
	HRESULT						SetUp_State_Death();
private:
	class CGameInstance*		m_pGameInstance = nullptr;


private:
	CPlayer*					m_pPlayer = nullptr;

	class CHuman_Bow*			m_pMonster = nullptr;
	CState*						m_pState = nullptr;
	CModel*						m_pModelCom = nullptr;
	CTransform*					m_pTransformCom = nullptr;
	CNavigation*				m_pNavigationCom = nullptr;

	_bool						m_bPlayAnimation = true;
	_bool						m_bDeadOnce = false;
	_bool						m_bFirstState[CHuman_Bow::FIRSTSTATE_END] = { false, false, false };

	_bool						m_bDamaged[HIT_END] = { false, false, false };
	vector<class CBullet*>*		m_vecLiveBullet;

private:					// Pattern CoolTime 
							// 구르기
	_float						m_fCurHideCoolTime = 0.f;
	_float						m_fHideCoolTime = 8.f;
	_bool						m_bCanHide = false;

	_float						m_fPatrolTurnTime = 2.5f;
	_float						m_fCurPatrolTurnTime = 0.f;
	_bool						m_bTurnPatrolDirection = false; 
	_int						m_iTurnint = 0;
	// 공격
	_float						m_fCurAttackCoolTime = 0.f;
	_float						m_fAttackCoolTime = 3.f;
	_bool						m_bCanAttack = false;
	_bool						m_bAttackOnce = false;
	// 피격 애니메이션
	_float						m_fCurDamagedAnimCoolTime = 0.f;
	_float						m_fDamagedAnimCoolTime = 5.f;
	_bool						m_bDamagedAnim = false;

	_bool						m_bShotOnce = false;

private:
	void						Start_Idle(_double dTimeDelta);
	void						Start_Run(_double dTimeDelta);
	void						Start_JustStand(_double dTimeDelta);
	void						Start_Patrol(_double dTimeDelta);
	void						Start_Damaged(_double dTimeDelta);
	void						Start_Detected(_double dTimeDelta);
	void						Start_Attack_A(_double dTimeDelta);
	void						Start_Melee_Attack(_double dTimeDelta);
	void						Start_Hide(_double dTimeDelta);
	void						Start_Death(_double dTimeDelta);
	void						Start_Reload(_double dTimeDelta);


private:
	void						Tick_Idle(_double dTimeDelta);
	void						Tick_Run(_double dTimeDelta);
	void						Tick_JustStand(_double dTimeDelta);
	void						Tick_Patrol(_double dTimeDelta);
	void						Tick_Damaged(_double dTimeDelta);
	void						Tick_Detected(_double dTimeDelta);
	void						Tick_Melee_Attack(_double dTimeDelta);
	void						Tick_Reload(_double dTimeDelta);

	void						Tick_Attack_A(_double dTimeDelta);
	void						Tick_Hide(_double dTimeDelta);
	void						Tick_Death(_double dTimeDelta);

private:
	void						End_Common(_double TimeDelta);
	void						End_Idle(_double dTimeDelta);
	void						End_Run(_double dTimeDelta);
	void						End_JustStand(_double dTimeDelta);
	void						End_Patrol(_double dTimeDelta);
	void						End_Damaged(_double dTimeDelta);
	void						End_Detected(_double dTimeDelta);
	void						END_Melee_Attack(_double dTimeDelta);

	void						End_Attack_A(_double dTimeDelta);
	void						End_Hide(_double dTimeDelta);
	void						End_Death(_double dTimeDelta);

private:
	_bool						Animation_Finish();
	_bool						Player_Detected(); // NoDetected -> Detected Anim Play
	_bool						Player_DetectedAndFar(); // Stand State -> Player Detected (Follow)
														 // _bool						Player_DetectedAndFar_AnimFinish(); 
	_bool						Player_DetectedAndInBowRange(); // Arrow
	_bool						Player_DetectedAndClose(); // Melee
	_bool						Player_CloseAndCanAttack(); // Ground Spawn
	_bool						Bullet_Hide_Collision();
	_bool						Player_NotDetected(); // When NoDetected (Collider)
	_bool						Is_Damaged();
	_bool						IS_Dead();
	// _bool					Check_IdleFinishCount();
	// _bool					Check_WalkTime();

public:
	static CHuman_Bow_State*	Create(class CHuman_Bow* pOwner, CState* pStateMachineCom, CModel* pModel, CTransform* pTransform, CNavigation* pNavigation);
	virtual void				Free() override;
};




END
