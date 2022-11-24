#pragma once
#include "Component.h"

BEGIN(Engine)

/*-------------------
 *    FSM_TRANSITION
 --------------------*/
// FSM의 Transition(상태변경조건)을 정의하는 구조체
typedef struct ENGINE_DLL tagFSM_Transition
{
	// 이 변경 조건의 이름
	const _tchar* pTransitionName;
	// 이 변경 조건의 순위
	_uint iPriority = 0;
	// 이 변경 조건 확인 함수
	std::function<_bool()> Predicator;

	tagFSM_Transition() = delete;

	// 람다 및 함수 포인터 바인드용 생성자
	tagFSM_Transition(const _tchar* pTransitionName, const std::function<_bool()>& Predicator, _uint iPriority = 0);

	// 멤버함수 바인드용 생성자
	template<typename T>
	tagFSM_Transition(const _tchar* pTransitionName, T* obj, bool (T::*memFunc)(), _uint iPriority = 0)
		: pTransitionName(pTransitionName), iPriority(iPriority)
	{ 
		Predicator = [obj, memFunc]()
		{
			return (obj->*memFunc)();
		};
	}

	// 복사 및 이동 생성자
	tagFSM_Transition(const tagFSM_Transition& other);
	tagFSM_Transition(tagFSM_Transition&& other) noexcept;
	tagFSM_Transition& operator=(const tagFSM_Transition& other);
	tagFSM_Transition& operator=(tagFSM_Transition&& other) noexcept;
} FSM_TRANSITION;

/*-------------------
 *    FSM_STATE
 --------------------*/
// FSM의 상태를 정의하는 구조체
typedef struct ENGINE_DLL tagFSM_State
{
	// 상태 이름
	const _tchar* pStateName;
	// 이 상태를 시작할 때 실행하는 함수
	std::function<void()> OnStart = nullptr;
	// 이 상태를 반복할 때 실행하는 함수
	std::function<void(_double)> Tick = nullptr;
	// 이 상태를 나갈때 실행하는 함수
	std::function<void()> OnExit = nullptr;

	// 각 transition vector는 priority로 정렬되어서 priority가 높은 transition을 먼저 확인한다.
	// 이 상태에서 다른 상태로 전이되는 조건(transtition)의 맵
	map<const _tchar*, vector<FSM_TRANSITION>> mapTransition;
	//key : 이동할 node이름, value : 이동 조건

	tagFSM_State(const _tchar* szNodeName) : pStateName(szNodeName) {}

	// 복사 및 이동 생성자
	tagFSM_State(const tagFSM_State& other);
	tagFSM_State(tagFSM_State&& other) noexcept;
	tagFSM_State& operator=(const tagFSM_State& other);
	tagFSM_State& operator=(tagFSM_State&& other) noexcept;

} FSM_STATE;

/*-------------------
 *    CFSMComponent
 --------------------*/
class ENGINE_DLL CFSMComponent : public CComponent
{
protected:
	CFSMComponent();
	CFSMComponent(const CFSMComponent& rhs);

protected:
	virtual ~CFSMComponent() override = default;

public:
	void Tick(_double TimeDelta);

	// CFSMComponentBuilder을 통해 FMS을 만들고 이를 pArg로 전해준다.
	virtual HRESULT Initialize(void* pArg) override;
	static CFSMComponent* Create();
	virtual CComponent* Clone(void* pArg) override;
	virtual void Imgui_RenderProperty() override;

private:
	void StateHistoryUpdate(const _tchar* pNextStateName);

private:
	map<const _tchar*, FSM_STATE> m_mapState;
	const _tchar* m_pCurStateName; // set init node at initialize

	// for debug
	const _tchar* m_pLastTransitionName = nullptr;
	list<string> m_strDebugQue;
	_uint m_iDebugQueSize = 10;
	bool m_bStoreHistory = false;
	// for debug
};

/*-------------------
 *    CFSMComponentBuilder
 --------------------*/
/*
 * FSM의 노드(상태)와 트랜지션(상태변경조건)을 정의하는 빌더 패턴 클래스.
 * 빌더패턴?  복잡한 객체를 생성하는 클래스(CFSMComponentBuilder)와 표현하는 클래스CFSMComponent)를 분리하여,
 * 동일한 절차에서도 서로 다른 표현을 생성하는 방법을 제공한다. 자세한건 구글링
 *
 * 사용예시
 * 	CFSMComponentBuilder builder = CFSMComponentBuilder() // 빌더 생성
	.InitState(TEXT("Idle"))								  // 최초 시작 노드의 이름
	.AddState(TEXT("Idle"))								  // Idle 상태노드 정의 시작
		.OnStart(this, &CBackGround::Idle_OnStart)	      // Idle 상태 시작할 때 CBackGround::Idle_OnStart함수 실행(상태당 하나의 함수 정의, 생략가능)
		.Tick(this, &CBackGround::Idle_Tick)		      // Idle 상태 유지 될 때 프레임마다 CBackGround::Idle_Tick함수 실행(상태당 하나의 함수 정의, 생략가능)
		.OnExit(this, &CBackGround::Idle_OnExit)          // Idle 상태에서 다른 상태로 이동할 때 실행하는 함수 정의(상태당 하나의 함수 정의, 생략가능)
		.Transition(TEXT("Walk"), FSM_TRANSITION(TEXT("Idle To Walk KeyInput"), this, &CBackGround::Idle2Walk_KeyInput))
													      // Idle에서 Walk로 전이하는 조건 정의, CBackGround::Predic_Idle2Walk함수 실행결과
													      // true면 Walk로 전이한다. 이하 반복
		.Transition(TEXT("Walk"), FSM_TRANSITION(TEXT("Idle To Walk Pushed"), this, &CBackGround::Idle2Walk_Pushed))
													      // 다수 transition정의 가능
	.AddState(TEXT("Walk")) // Walk상태 노드 정의 시작
		.Transition(TEXT("Idle"), FSM_TRANSITION(L"Walk To Idle", this, &CBackGround::Predic_Walk2Idle))
	.Build();										      // 모든 상태를 만들면 Build()함수로 최종 Builder를 만든다.(종료함수)

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_FSM"), TEXT("Com_FSM"),
		(CComponent**)&m_pFSM, &builder)))			      // 만들어진 CFSMComponentBuilder의 포인터를 pArg로 넘겨준다.
													      // 이렇게 넘겨진 클래스는 "이동" 되므로 다시 사용할 수 없다.

  위 예시는 멤버함수를 사용하지만 람다 및 함수 포인터도 인자로 사용가능
 */
class ENGINE_DLL CFSMComponentBuilder
{
public:
	CFSMComponentBuilder& InitState(const _tchar* szNodeName)
	{
		m_pInitStateName = szNodeName;
		return *this;
	}

	CFSMComponentBuilder& AddState(const _tchar* szNodeName)
	{
		m_mapStates.insert({szNodeName, FSM_STATE(szNodeName)});
		m_pBuildStateName = szNodeName;
		return *this;
	}
	CFSMComponentBuilder& Transition(const _tchar* szNextNodeNmae, const FSM_TRANSITION& tTransition)
	{
		const auto itr = find_if(m_mapStates.begin(), m_mapStates.end(), CTag_Finder(m_pBuildStateName));
		assert(itr != m_mapStates.end());

		auto NextNodeItr = find_if(itr->second.mapTransition.begin(), itr->second.mapTransition.begin(), CTag_Finder(szNextNodeNmae));
		if (NextNodeItr == itr->second.mapTransition.end())
		{
			itr->second.mapTransition.insert({szNextNodeNmae, vector<FSM_TRANSITION>{tTransition}});
		}
		else
		{
			NextNodeItr->second.push_back(tTransition);
		}

		return *this;
	}
	CFSMComponentBuilder& OnStart(const std::function<void()>& onStart)
	{
		const auto itr = find_if(m_mapStates.begin(), m_mapStates.end(), CTag_Finder(m_pBuildStateName));
		assert(itr != m_mapStates.end());

		itr->second.OnStart = onStart;
		return *this;
	}

	template<typename T>
	CFSMComponentBuilder& OnStart(T* obj, void (T::*memFunc)())
	{
		const auto itr = find_if(m_mapStates.begin(), m_mapStates.end(), CTag_Finder(m_pBuildStateName));
		assert(itr != m_mapStates.end());

		itr->second.OnStart = [obj, memFunc]()
		{
			(obj->*memFunc)();
		};
		return *this;
	}

	CFSMComponentBuilder& Tick(const std::function<void(_double)>& tick)
	{
		const auto itr = find_if(m_mapStates.begin(), m_mapStates.end(), CTag_Finder(m_pBuildStateName));
		assert(itr != m_mapStates.end());

		itr->second.Tick = tick;
		return *this;
	}

	template<typename T>
	CFSMComponentBuilder& Tick(T* obj, void (T::*memFunc)(_double))
	{
		const auto itr = find_if(m_mapStates.begin(), m_mapStates.end(), CTag_Finder(m_pBuildStateName));
		assert(itr != m_mapStates.end());

		itr->second.Tick = [obj, memFunc](_double TimeDelta)
		{
			(obj->*memFunc)(TimeDelta);
		};
		return *this;
	}

	CFSMComponentBuilder& OnExit(const std::function<void()>& onExit)
	{
		const auto itr = find_if(m_mapStates.begin(), m_mapStates.end(), CTag_Finder(m_pBuildStateName));
		assert(itr != m_mapStates.end());

		itr->second.OnExit = onExit;
		return *this;
	}

	template<typename T>
	CFSMComponentBuilder& OnExit(T* obj, void (T::*memFunc)())
	{
		const auto itr = find_if(m_mapStates.begin(), m_mapStates.end(), CTag_Finder(m_pBuildStateName));
		assert(itr != m_mapStates.end());

		itr->second.OnExit = [obj, memFunc]()
		{
			(obj->*memFunc)();
		};
		return *this;
	}

	CFSMComponentBuilder& Build()
	{
		assert(m_pInitStateName != nullptr);

		for (auto& NodePair : m_mapStates)
		{
			for (auto& TranPair : NodePair.second.mapTransition)
			{
				std::sort(TranPair.second.begin(), TranPair.second.end(), 
					[](const FSM_TRANSITION& left, const FSM_TRANSITION& right)
				{
					return left.iPriority < right.iPriority; // 오름차순
				});
			}
		}

		return *this;
	}

	map<const _tchar*, FSM_STATE>& GetStates()
	{
		return m_mapStates;
	}

	const _tchar* GetInitStateName() const
	{
		return m_pInitStateName;
	}


private:
	const _tchar* m_pBuildStateName = nullptr;

	map<const _tchar*, FSM_STATE> m_mapStates;
	const _tchar* m_pInitStateName = nullptr;
};

END