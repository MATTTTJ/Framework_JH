#pragma once
#include "Client_Defines.h"
#include "Base.h"
#include "Little_Bug.h"

BEGIN(Engine)
class CGameObject;
class CGameInstance;
class CState;
class CModel;
class CNavigation;
class CTransform;
END

BEGIN(Client)
class CLittle_Bug_State final : public CBase
{
public:
	enum DAMAGEDTYPE { HITHEAD, HITBODY, HIT, HIT_END };

	enum ANIMATION
	{
		LITTLE_ATTACK, //근접시
		LITTLE_HITBODY, // 바디에 콜라이더 달기
		LITTLE_JUMPSTART,// 사용안함
		LITTLE_LAND,// 사용안함
		LITTLE_PATROL, // 플레이어 감지 못했을 때 (보스맵에 좀 깔아두기)
		LITTLE_RUN,// 플레이어 감지 시
		LITTLE_STAND,// 사용안함 
		LITTLE_IN_COMBAT_IDLE,// 부모에게 소환됐을 때 
		BUG_ANIMEND
	};

public:
	CLittle_Bug_State();
	virtual ~CLittle_Bug_State() = default;

public:
	const _bool&				Is_DamagedState() { return m_bDamaged[HIT]; }
	void						Set_DamagedState(DAMAGEDTYPE eType) { m_bDamaged[eType] = true; }
	HRESULT						Initialize(CLittle_Bug* pOwner, CState* pStateMachineCom, CModel* pModel, CTransform* pTransform, CNavigation* pNavigation);
	void						Tick(_double dTimeDelta);
	void						Late_Tick(_double dTimeDelta);
	void						Reset_Damaged();


private:
	HRESULT						SetUp_State_Idle();
	HRESULT						SetUp_State_Death();

private:
	class CGameInstance*		m_pGameInstance = nullptr;

private:
	CPlayer*					m_pPlayer = nullptr;

	class CLittle_Bug*			m_pMonster = nullptr;
	CState*						m_pState = nullptr;
	CModel*						m_pModelCom = nullptr;
	CTransform*					m_pTransformCom = nullptr;
	CNavigation*				m_pNavigationCom = nullptr;

	_bool						m_bPlayAnimation = {true};
	_bool						m_bDeadOnce = {false};
	_bool						m_bFirstState[CLittle_Bug::FIRSTSTATE_END] = { false, false, false };

	_bool						m_bDamaged[HIT_END] = { false, false, false };

private:
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
	void						Start_Damaged(_double dTimeDelta);
	void						Start_Attack_A(_double dTimeDelta);
	void						Start_Death(_double dTimeDelta);

private:
	void						Tick_Idle(_double dTimeDelta);
	void						Tick_Death(_double dTimeDelta);
	void						Tick_Run(_double dTimeDelta);
	void						Tick_Damaged(_double dTimeDelta);
	void						Tick_Attack_A(_double dTimeDelta);


private:
	void						End_Common(_double TimeDelta);
	void						End_Idle(_double dTimeDelta);
	void						End_Death(_double dTimeDelta);
	void						End_Damaged(_double dTimeDelta);
	void						End_Attack_A(_double dTimeDelta);



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

public:
	static CLittle_Bug_State*	Create(class CLittle_Bug* pOwner, CState* pStateMachineCom, CModel* pModel, CTransform* pTransform, CNavigation* pNavigation);
	virtual void				Free() override;
};



END


// 엘리트 버그는 기본공격이 없고 계속해서 돌진을 함
// 돌진하는 로직은 일정 시간뒤에 일정시간동안 돌진하는 것으로 함
// 돌진하는 동안은 방향 변경 불가 
