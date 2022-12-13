#pragma once
#include "Component.h"

BEGIN(Engine)

typedef struct ENGINE_DLL tagFSM_Transition
{
	wstring					wstrTransitionName;
	_uint						iPriority = 0;
	std::function<_bool()>	Predicator;

	tagFSM_Transition() = delete;
	tagFSM_Transition(const wstring& wstrTransitionName, const std::function<_bool()>& Predicator, _uint iPriority = 0);

	template<typename T>
	tagFSM_Transition(const wstring& wstrTransitionName, T* Obj, _bool(T::*memFunc)(), _uint iPriority = 0)
		: wstrTransitionName(wstrTransitionName), iPriority(iPriority)
	{
		Predicator = [Obj, memFunc]()
		{
			return (Obj->*memFunc)();
		};
	}

	tagFSM_Transition(const tagFSM_Transition& rhs);
	tagFSM_Transition(tagFSM_Transition&& rhs) noexcept;
	tagFSM_Transition& operator=(const tagFSM_Transition& rhs);
	tagFSM_Transition& operator=(tagFSM_Transition&& rhs) noexcept;
} FSM_TRANSITION;

typedef struct ENGINE_DLL tagFSM_State
{
	wstring							wstrStateName;
	std::function<void()>				OnStart = nullptr;
	std::function<void(_double)>	Tick = nullptr;
	std::function<void()>				OnExit = nullptr;

	map<const wstring, vector<FSM_TRANSITION>> mapTransition;

	tagFSM_State(const wstring& wstrNodeName) : wstrStateName(wstrNodeName) {}
	
	tagFSM_State(const tagFSM_State& rhs);
	tagFSM_State(tagFSM_State&& rhs) noexcept;
	tagFSM_State& operator=(const tagFSM_State& rhs);
	tagFSM_State& operator=(tagFSM_State&& rhs) noexcept;
} FSM_STATE;

class ENGINE_DLL CStateMachine : public CComponent
{
protected:
	CStateMachine();
	CStateMachine(const CStateMachine& rhs);
	virtual ~CStateMachine() override = default;

public:
	virtual HRESULT			Initialize_Clone(class CGameObject* pOwner, void *pArg) override;
	virtual void				Imgui_RenderProperty() override;

public:
	void						Tick(_double dTimeDelta);
	
private:
	map<const wstring, FSM_STATE>		m_mapState;
	wstring									m_wstrCurrentStateName;

	wstring									m_wstrLastTransitionName = L"";
	list<string>								m_strDebugQue;
	_uint										m_iDebugQueSize = 10;
	_bool										m_bStoreHistory = false;

private:
	void						StateHistoryUpdate(const wstring& wstrNextStateName);

public:
	static CStateMachine*	Create();
	virtual CComponent*	Clone(class CGameObject* pOwner, void* pArg = nullptr) override;
	virtual void				Free() override;
};

class ENGINE_DLL CStateMachineBuilder
{
public:
	CStateMachineBuilder& InitializeState(const wstring& wstrNodeName)
	{
		m_wstrInitStateName = wstrNodeName;
		return *this;
	}
	CStateMachineBuilder& AddState(const wstring& wstrNodeName)
	{
		m_mapStates.insert({ wstrNodeName, FSM_STATE(wstrNodeName) });
		m_wstrBuildStateName = wstrNodeName;
		return *this;
	}
	CStateMachineBuilder& Transition(const wstring& wstrNextNodeName, const FSM_TRANSITION& tTransition)
	{
		const auto iter = find_if(m_mapStates.begin(), m_mapStates.end(), CTag_Finder(m_wstrBuildStateName));
		assert(iter != m_mapStates.end());

		auto NextNodeIter = find_if(iter->second.mapTransition.begin(), iter->second.mapTransition.end(), CTag_Finder(wstrNextNodeName));
		if (NextNodeIter == iter->second.mapTransition.end())
			iter->second.mapTransition.insert({ wstrNextNodeName, vector<FSM_TRANSITION>{tTransition} });
		else
			NextNodeIter->second.push_back(tTransition);

		return *this;
	}
	CStateMachineBuilder& OnStart(const std::function<void()>& onStart)
	{
		const auto iter = find_if(m_mapStates.begin(), m_mapStates.end(), CTag_Finder(m_wstrBuildStateName));
		assert(iter != m_mapStates.end());

		iter->second.OnStart = onStart;
		
		return *this;
	}
	template<typename T>
	CStateMachineBuilder& OnStart(T* Obj, void(T::*memFunc)())
	{
		const auto iter = find_if(m_mapStates.begin(), m_mapStates.end(), CTag_Finder(m_wstrBuildStateName));
		assert(iter != m_mapStates.end());

		iter->second.OnStart = [Obj, memFunc]()
		{
			(Obj->*memFunc)();
		};

		return *this;
	}
	CStateMachineBuilder& Tick(const std::function<void(_double)>& tick)
	{
		const auto iter = find_if(m_mapStates.begin(), m_mapStates.end(), CTag_Finder(m_wstrBuildStateName));
		assert(iter != m_mapStates.end());

		iter->second.Tick = tick;

		return *this;
	}
	template <typename T>
	CStateMachineBuilder& Tick(T* Obj, void (T::*memFunc)(_double))
	{
		const auto iter = find_if(m_mapStates.begin(), m_mapStates.end(), CTag_Finder(m_wstrBuildStateName));
		assert(iter != m_mapStates.end());

		iter->second.Tick = [Obj, memFunc](_double dTimeDelta)
		{
			(Obj->*memFunc)(dTimeDelta);
		};

		return *this;
	}
	CStateMachineBuilder& OnExit(const std::function<void()>& onExit)
	{
		const auto iter = find_if(m_mapStates.begin(), m_mapStates.end(), CTag_Finder(m_wstrBuildStateName));
		assert(iter != m_mapStates.end());

		iter->second.OnExit = onExit;

		return *this;
	}
	template<typename T>
	CStateMachineBuilder& OnExit(T* Obj, void(T::*memFunc)())
	{
		const auto iter = find_if(m_mapStates.begin(), m_mapStates.end(), CTag_Finder(m_wstrBuildStateName));
		assert(iter != m_mapStates.end());

		iter->second.OnExit = [Obj, memFunc]()
		{
			(Obj->*memFunc)();
		};

		return *this;
	}
	CStateMachineBuilder& Build()
	{
		assert(m_wstrInitStateName != L"");

		for (auto& NodePair : m_mapStates)
		{
			for (auto& TranPair : NodePair.second.mapTransition)
			{
				std::sort(TranPair.second.begin(), TranPair.second.end(), [](const FSM_TRANSITION& Left, const FSM_TRANSITION& Right) {
					return Left.iPriority < Right.iPriority;
				});
			}
		}

		return *this;
	}

	map<const wstring, FSM_STATE>&		GetStates() { return m_mapStates; }
	const wstring&							Get_InitStateName() const { return m_wstrInitStateName; }

private:
	wstring								m_wstrBuildStateName = L"";

	map<const wstring, FSM_STATE>	m_mapStates;
	wstring								m_wstrInitStateName = L"";
};

END