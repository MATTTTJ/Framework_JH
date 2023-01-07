#pragma once
#include "Client_Defines.h"
#include "Base.h"

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
	HRESULT						Initialize(class CHuman_Bow* pOwner, CState* pStateMachineCom, CModel* pModel, CTransform* pTransform, CNavigation* pNavigation);
	void						Tick(_double dTimeDelta);
	void						Late_Tick(_double dTimeDelta);


private:
	HRESULT						SetUp_State_No_Detected();
	HRESULT						SetUp_State_Idle();
	HRESULT						SetUp_State_Walk();
	HRESULT						SetUp_State_Run();
	HRESULT						SetUp_State_Attack_A();
	HRESULT						SetUp_State_Attack_B();
	HRESULT						SetUp_State_Hide();
	HRESULT						SetUp_State_Death();

private:
	class CGameInstance*		m_pGameInstance = nullptr;


private:
	class CPlayer*				m_pPlayer = nullptr;
	CState*						m_pState = nullptr;
	CModel*						m_pModelCom = nullptr;
	CTransform*					m_pTransformCom = nullptr;
	CNavigation*				m_pNavigationCom = nullptr;

	_bool						m_bDeadOnce = false;
private:
	// vector<class CGameObject*>	m_vecBullet;

private:
	void						Start_No_Detected(_double dTimeDelta);
	void						Start_Idle(_double dTimeDelta);
	void						Start_Walk(_double dTimeDelta);
	void						Start_Run(_double dTimeDelta);
	void						Start_Attack_A(_double dTimeDelta);
	void						Start_Attack_B(_double dTimeDelta);
	void						Start_Hide(_double dTimeDelta);
	void						Start_Death(_double dTimeDelta);

private:
	void						Tick_No_Detected(_double dTimeDelta);
	void						Tick_Idle(_double dTimeDelta);
	void						Tick_Walk(_double dTimeDelta);
	void						Tick_Run(_double dTimeDelta);
	void						Tick_Attack_A(_double dTimeDelta);
	void						Tick_Attack_B(_double dTimeDelta);
	void						Tick_Hide(_double dTimeDelta);
	void						Tick_Death(_double dTimeDelta);

private:
	void						End_No_Detected(_double dTimeDelta);
	void						End_Idle(_double dTimeDelta);
	void						End_Walk(_double dTimeDelta);
	void						End_Run(_double dTimeDelta);
	void						End_Attack_A(_double dTimeDelta);
	void						End_Attack_B(_double dTimeDelta);
	void						End_Hide(_double dTimeDelta);
	void						End_Death(_double dTimeDelta);

private:
	_bool						Player_Detected();
	_bool						Player_DetectedAndFar();
	_bool						Player_DetectedAndClose();
	_bool						Player_NotDetected();
	// _bool					Check_IdleFinishCount();
	// _bool					Check_WalkTime();

public:
	static CHuman_Bow_State*	Create(class CHuman_Bow* pOwner, CState* pStateMachineCom, CModel* pModel, CTransform* pTransform, CNavigation* pNavigation);
	virtual void				Free() override;
};

END