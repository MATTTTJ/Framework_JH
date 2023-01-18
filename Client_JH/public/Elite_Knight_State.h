#pragma once
#include "Client_Defines.h"
#include "Base.h"
#include "Elite_Knight.h"

BEGIN(Engine)
class CGameObject;
class CGameInstance;
class CState;
class CModel;
class CNavigation;
class CTransform;
END

BEGIN(Client)
class CElite_Knight_State final : public CBase
{
public:
	enum DAMAGEDTYPE { HITHEAD, HITBODY, HIT, HIT_END };

	enum ANIMATION
	{
		KNIGHT_IN_COMBAT_IDLE,
		KNIGHT_PATROL,
		KNIGHT_RUN,
		KNIGHT_STAND,
		KNIGHT_WALK_F,KNIGHT_WALK_R, KNIGHT_WALK_B,KNIGHT_WALK_L,
		KNIGHT_TURNLEFT_180, KNIGHT_TURNLEFT_90,KNIGHT_TURNRIGHT_90, KNIGHT_TURNRIGHT_180,
		KNIGHT_SPAWN,
		KNIGHT_HITHEAD,
		KNIGHT_ATTACK_WIND,KNIGHT_ATTACK_HIDE,KNIGHT_SHIELD_SHIELD,
		KNIGHT_ANIMEND
	};

public:
	CElite_Knight_State();
	virtual ~CElite_Knight_State() = default;

public:
	const _bool&				Is_DamagedState() { return m_bDamaged[HIT]; }
	void						Set_DamagedState(DAMAGEDTYPE eType) { m_bDamaged[eType] = true; }
	HRESULT						Initialize(CElite_Knight* pOwner, CState* pStateMachineCom, CModel* pModel, CTransform* pTransform, CNavigation* pNavigation);
	void						Tick(_double dTimeDelta);
	void						Late_Tick(_double dTimeDelta);
	void						Reset_Damaged();

private:
	HRESULT						SetUp_State_UpSpawn();
	HRESULT						SetUp_State_Idle();
	HRESULT						SetUp_State_Attack_WIND();
	HRESULT						SetUp_State_Attack_IN_SHIELD();
	HRESULT						SetUp_State_Attack_USE_SHIELD();
	HRESULT						SetUp_State_Hide();
	HRESULT						SetUp_State_Death();

private:
	class CGameInstance*		m_pGameInstance = nullptr;
private:
	CPlayer*					m_pPlayer = nullptr;

	class CElite_Knight*		m_pMonster = nullptr;
	CState*						m_pState = nullptr;
	CModel*						m_pModelCom = nullptr;
	CTransform*					m_pTransformCom = nullptr;
	CNavigation*				m_pNavigationCom = nullptr;

	_bool						m_bPlayAnimation = true;
	_bool						m_bDeadOnce = false;
	_bool						m_bFirstState[CElite_Knight::FIRSTSTATE_END] = { false, false, false };

	_bool						m_bDamaged[HIT_END] = { false, false, false };

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
	// vector<class CGameObject*>	m_vecBullet;

private:
	void						Start_UpSpawn(_double dTimeDelta);
	void						Start_Idle(_double dTimeDelta);
	void						Start_Run(_double dTimeDelta);
	void						Start_Attack_WIND(_double dTimeDelta);
	void						Start_Attack_IN_SHIELD(_double dTimeDelta);
	void						Start_Attack_USE_SHIELD(_double dTimeDelta);
	void						Start_Hide(_double dTimeDelta);
	void						Start_Death(_double dTimeDelta);
	void						Start_Damaged(_double dTimeDelta);
	void						Start_Detected(_double dTimeDelta);
	void						Start_Turn(_double dTimeDelta);

private:
	void						Tick_UpSpawn(_double dTimeDelta);
	void						Tick_Idle(_double dTimeDelta);
	void						Tick_Run(_double dTimeDelta);
	void						Tick_Attack_WIND(_double dTimeDelta);
	void						Tick_Attack_IN_SHIELD(_double dTimeDelta);
	void						Tick_Attack_USE_SHIELD(_double dTimeDelta);
	void						Tick_Hide(_double dTimeDelta);
	void						Tick_Death(_double dTimeDelta);
	void						Tick_Damaged(_double dTimeDelta);
	void						Tick_Detected(_double dTimeDelta);
	void						Tick_Turn(_double dTimeDelta);

private:
	void						End_Common(_double TimeDelta);
	void						End_UpSpawn(_double dTimeDelta);
	void						End_Idle(_double dTimeDelta);
	void						End_Run(_double dTimeDelta);
	void						End_Attack_WIND(_double dTimeDelta);
	void						End_Attack_IN_SHIELD(_double dTimeDelta);
	void						End_Attack_USE_SHIELD(_double dTimeDelta);
	void						End_Hide(_double dTimeDelta);
	void						End_Death(_double dTimeDelta);
	void						End_Damaged(_double dTimeDelta);
	void						End_Detected(_double dTimeDelta);
	void						End_Turn(_double dTimeDelta);

private:
	_bool						Animation_Finish();
	_bool						Player_Detected(); // NoDetected -> Detected Anim Play
	_bool						Player_DetectedAndFar(); // Stand State -> Player Detected (Follow)
														 // _bool						Player_DetectedAndFar_AnimFinish(); 
	_bool						Player_DetectedAndClose(); // Ground Spawn
	_bool						Player_CloseAndCanAttack(); // Ground Spawn
	_bool						Player_FarAndCanAttack(); // Ground Spawn
	_bool						Turn_To_Player_Right_180();
	_bool						Turn_To_Player_Right_90();
	_bool						Turn_To_Player_Left_180();
	_bool						Turn_To_Player_Left_90();

	_bool						Bullet_Hide_Collision();
	_bool						Player_NotDetected(); // When NoDetected (Collider)
	_bool						Is_Damaged();
	_bool						IS_Dead();

public:
	static CElite_Knight_State*	Create(class CElite_Knight* pOwner, CState* pStateMachineCom, CModel* pModel, CTransform* pTransform, CNavigation* pNavigation);
	virtual void				Free() override;
};



END
