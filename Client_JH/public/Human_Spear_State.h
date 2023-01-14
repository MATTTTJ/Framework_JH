#pragma once
#include "Client_Defines.h"
#include "Base.h"
#include "Human_Spear.h"

BEGIN(Engine)
class CGameObject;
class CGameInstance;
class CState;
class CModel;
class CNavigation;
class CTransform;
END

BEGIN(Client)
class CHuman_Spear_State final : public CBase
{

public:
	enum DAMAGEDTYPE { HITHEAD, HITBODY, HIT, HIT_END };

	enum ANIMATION
	{
		SPEAR_INCOMBAT_IDLE,
		SPEAR_PATROL, SPEAR_JUSTSTAND,
		SPEAR_GUARD,
		SPEAR_HITHEAD,
		SPEAR_RUN_A, SPEAR_RUN_B,
		SPEAR_NODETECTED, SPEAR_DETECTED,
		SPEAR_UPSPAWN,
		SPEAR_JUMP, SPEAR_LAND,
		SPEAR_HITBODY,
		SPEAR_HIDE_RIGHT, SPEAR_HIDE_LEFT,
		SPEAR_ATTACK_A, SPEAR_ATTACK_B,
		SPEAR_ANIMEND
	};




public:
	CHuman_Spear_State();
	virtual ~CHuman_Spear_State() = default;

public:
	const _bool&				Is_DamagedState() { return m_bDamaged[HIT]; }
	void						Set_DamagedState(DAMAGEDTYPE eType) { m_bDamaged[eType] = true; }
	HRESULT						Initialize(CHuman_Spear* pOwner, CState* pStateMachineCom, CModel* pModel, CTransform* pTransform, CNavigation* pNavigation);
	void						Tick(_double dTimeDelta);
	void						Late_Tick(_double dTimeDelta);
	void						Reset_Damaged();


public:


private:
	HRESULT						SetUp_State_Idle();
	HRESULT						SetUp_State_Run();
	HRESULT						SetUp_State_JustStand();
	HRESULT						SetUp_State_HitBody();
	HRESULT						SetUp_State_No_Detected();
	HRESULT						SetUp_State_Detected();
	HRESULT						SetUp_State_UpSpawn();
	HRESULT						SetUp_State_Attack_A();
	HRESULT						SetUp_State_Hide();
	HRESULT						SetUp_State_Death();
private:
	class CGameInstance*		m_pGameInstance = nullptr;


private:
	CPlayer*					m_pPlayer = nullptr;

	class CHuman_Spear*			m_pMonster = nullptr;
	CState*						m_pState = nullptr;
	CModel*						m_pModelCom = nullptr;
	CTransform*					m_pTransformCom = nullptr;
	CNavigation*				m_pNavigationCom = nullptr;

	_bool						m_bPlayAnimation = true;
	_bool						m_bDeadOnce = false;
	_bool						m_bFirstState[CHuman_Spear::FIRSTSTATE_END] = { false, false, false };

	_bool						m_bDamaged[HIT_END] = { false, false, false };
	vector<class CBullet*>*		m_vecLiveBullet;

private:					// Pattern CoolTime 
							// 구르기
	_float						m_fCurHideCoolTime = 0.f;
	_float						m_fHideCoolTime = 8.f;
	_bool						m_bCanHide = false;

	// 공격
	_float						m_fCurAttackCoolTime = 0.f;
	_float						m_fAttackCoolTime = 3.f;
	_bool						m_bCanAttack = false;
	_bool						m_bAttackOnce = false;
	// 피격 애니메이션
	_float						m_fCurDamagedAnimCoolTime = 0.f;
	_float						m_fDamagedAnimCoolTime = 5.f;
	_bool						m_bDamagedAnim = false;

private:
	void						Start_Idle(_double dTimeDelta);
	void						Start_Run(_double dTimeDelta);
	void						Start_JustStand(_double dTimeDelta);
	void						Start_Damaged(_double dTimeDelta);
	void						Start_No_Detected(_double dTimeDelta);
	void						Start_Detected(_double dTimeDelta);
	void						Start_Detected_Guard(_double dTimeDelta);

	void						Start_UpSpawn(_double dTimeDelta);
	void						Start_Attack_A(_double dTimeDelta);
	void						Start_Hide(_double dTimeDelta);
	void						Start_Death(_double dTimeDelta);

private:
	void						Tick_Idle(_double dTimeDelta);
	void						Tick_Run(_double dTimeDelta);
	void						Tick_JustStand(_double dTimeDelta);
	void						Tick_Damaged(_double dTimeDelta);
	void						Tick_No_Detected(_double dTimeDelta);
	void						Tick_Detected(_double dTimeDelta);
	void						Tick_Detected_Guard(_double dTimeDelta);

	void						Tick_UpSpawn(_double dTimeDelta);
	void						Tick_Attack_A(_double dTimeDelta);
	void						Tick_Hide(_double dTimeDelta);
	void						Tick_Death(_double dTimeDelta);

private:
	void						End_Common(_double TimeDelta);
	void						End_Idle(_double dTimeDelta);
	void						End_Run(_double dTimeDelta);
	void						End_JustStand(_double dTimeDelta);
	void						End_Damaged(_double dTimeDelta);
	void						End_No_Detected(_double dTimeDelta);
	void						End_Detected(_double dTimeDelta);
	void						End_Detected_Guard(_double dTimeDelta);

	void						End_UpSpawn(_double dTimeDelta);
	void						End_Attack_A(_double dTimeDelta);
	void						End_Hide(_double dTimeDelta);
	void						End_Death(_double dTimeDelta);

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
	// _bool					Check_IdleFinishCount();
	// _bool					Check_WalkTime();

public:
	static CHuman_Spear_State*	Create(class CHuman_Spear* pOwner, CState* pStateMachineCom, CModel* pModel, CTransform* pTransform, CNavigation* pNavigation);
	virtual void				Free() override;
};




END
