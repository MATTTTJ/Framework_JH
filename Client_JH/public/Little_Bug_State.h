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
		LITTLE_ATTACK, //������
		LITTLE_HITBODY, // �ٵ� �ݶ��̴� �ޱ�
		LITTLE_JUMPSTART,// ������
		LITTLE_LAND,// ������
		LITTLE_PATROL, // �÷��̾� ���� ������ �� (�����ʿ� �� ��Ƶα�)
		LITTLE_RUN,// �÷��̾� ���� ��
		LITTLE_STAND,// ������ 
		LITTLE_IN_COMBAT_IDLE,// �θ𿡰� ��ȯ���� �� 
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
	// ����
	_float						m_fCurAttackCoolTime = {0.f};
	_float						m_fAttackCoolTime = {10.f};
	_bool						m_bCanAttack = {false};
	_bool						m_bAttackOnce = {false};
	// �ǰ� �ִϸ��̼�
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


// ����Ʈ ���״� �⺻������ ���� ����ؼ� ������ ��
// �����ϴ� ������ ���� �ð��ڿ� �����ð����� �����ϴ� ������ ��
// �����ϴ� ������ ���� ���� �Ұ� 
