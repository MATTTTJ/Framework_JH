#pragma once
#include "Client_Defines.h"
#include "Base.h"
#include "Human_Explode.h"

BEGIN(Engine)
class CGameObject;
class CGameInstance;
class CState;
class CModel;
class CNavigation;
class CTransform;
END

BEGIN(Client)
class CHuman_Explode_State final : public CBase
{
public:
	enum DAMAGEDTYPE { HITHEAD, HITBODY, HIT, HIT_END };

	enum ANIMATION
	{
		EXPLODE_EXPLODE,
		EXPLODE_HIDE_LEFT, EXPLODE_HIDE_RIGHT,
		EXPLODE_PATROL,
		EXPLODE_RUN,
		EXPLODE_IN_COMBAT_IDLE,
		EXPLODE_ANIMEND
	};

public:
	CHuman_Explode_State();
	virtual ~CHuman_Explode_State() = default;

public:
	const _bool&				Is_DamagedState() { return m_bDamaged[HIT]; }
	void						Set_DamagedState(DAMAGEDTYPE eType) { m_bDamaged[eType] = true; }
	HRESULT						Initialize(CHuman_Explode* pOwner, CState* pStateMachineCom, CModel* pModel, CTransform* pTransform, CNavigation* pNavigation);
	void						Tick(_double dTimeDelta);
	void						Late_Tick(_double dTimeDelta);
	void						Reset_Damaged();


private:
	HRESULT						SetUp_State_Idle();
	HRESULT						SetUp_State_Run();
	HRESULT						SetUp_State_JustStand();
	HRESULT						SetUp_State_Patrol();
	HRESULT						SetUp_State_Attack_A();
	HRESULT						SetUp_State_Hide();

private:
	class CGameInstance*		m_pGameInstance = nullptr;

private:
	CPlayer*					m_pPlayer = nullptr;

	class CHuman_Explode*			m_pMonster = nullptr;
	CState*						m_pState = nullptr;
	CModel*						m_pModelCom = nullptr;
	CTransform*					m_pTransformCom = nullptr;
	CNavigation*				m_pNavigationCom = nullptr;

	_bool						m_bPlayAnimation = true;
	_bool						m_bDeadOnce = false;
	_bool						m_bFirstState[CHuman_Explode::FIRSTSTATE_END] = { false, false, false };

	_bool						m_bDamaged[HIT_END] = { false, false, false };


private:					// Pattern CoolTime 
							// ±¸¸£±â
	_float						m_fCurHideCoolTime = 0.f;
	_float						m_fHideCoolTime = 8.f;
	_bool						m_bCanHide = false;
	_bool						m_bAttackOnce = false;

	// Patrol

	_float						m_fPatrolTurnTime = 2.5f;
	_float						m_fCurPatrolTurnTime = 0.f;
	_bool						m_bTurnPatrolDirection = false;

private:
	void						Start_Idle(_double dTimeDelta);
	void						Start_Run(_double dTimeDelta);
	void						Start_JustStand(_double dTimeDelta);
	void						Start_Patrol(_double dTimeDelta);
	void						Start_Attack_A(_double dTimeDelta);
	void						Start_Hide(_double dTimeDelta);
	void						Start_Dead(_double dTimeDelta);


private:
	void						Tick_Idle(_double dTimeDelta);
	void						Tick_Run(_double dTimeDelta);
	void						Tick_JustStand(_double dTimeDelta);
	void						Tick_Patrol(_double dTimeDelta);
	void						Tick_Attack_A(_double dTimeDelta);
	void						Tick_Hide(_double dTimeDelta);
	void						Tick_Dead(_double dTimeDelta);

private:
	void						End_Idle(_double dTimeDelta);
	void						End_Run(_double dTimeDelta);
	void						End_JustStand(_double dTimeDelta);
	void						End_Patrol(_double dTimeDelta);
	void						End_Attack_A(_double dTimeDelta);
	void						End_Hide(_double dTimeDelta);
	void						End_Dead(_double dTimeDelta);
	void						End_Common(_double dTimeDelta);
private:
	_bool						Animation_Finish();
	_bool						Player_Detected(); // NoDetected -> Detected Anim Play
	_bool						Player_DetectedAndFar(); // Stand State -> Player Detected (Follow)
														 // _bool						Player_DetectedAndFar_AnimFinish(); 
	_bool						Player_DetectedAndClose(); // Ground Spawn
	_bool						Player_CloseAndCanAttack(); // Ground Spawn
	_bool						Bullet_Hide_Collision();
	_bool						Player_NotDetected(); // When NoDetected (Collider)
	_bool						Is_Damaged();
	_bool						IS_Dead();

public:
	static CHuman_Explode_State*	Create(class CHuman_Explode* pOwner, CState* pStateMachineCom, CModel* pModel, CTransform* pTransform, CNavigation* pNavigation);
	virtual void					Free() override;
};



END
