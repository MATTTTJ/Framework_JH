#pragma once
#include "Component.h"

BEGIN(Engine)

/*-------------------
 *    FSM_TRANSITION
 --------------------*/
// FSM�� Transition(���º�������)�� �����ϴ� ����ü
typedef struct ENGINE_DLL tagFSM_Transition
{
	// �� ���� ������ �̸�
	const _tchar* pTransitionName;
	// �� ���� ������ ����
	_uint iPriority = 0;
	// �� ���� ���� Ȯ�� �Լ�
	std::function<_bool()> Predicator;

	tagFSM_Transition() = delete;

	// ���� �� �Լ� ������ ���ε�� ������
	tagFSM_Transition(const _tchar* pTransitionName, const std::function<_bool()>& Predicator, _uint iPriority = 0);

	// ����Լ� ���ε�� ������
	template<typename T>
	tagFSM_Transition(const _tchar* pTransitionName, T* obj, bool (T::*memFunc)(), _uint iPriority = 0)
		: pTransitionName(pTransitionName), iPriority(iPriority)
	{ 
		Predicator = [obj, memFunc]()
		{
			return (obj->*memFunc)();
		};
	}

	// ���� �� �̵� ������
	tagFSM_Transition(const tagFSM_Transition& other);
	tagFSM_Transition(tagFSM_Transition&& other) noexcept;
	tagFSM_Transition& operator=(const tagFSM_Transition& other);
	tagFSM_Transition& operator=(tagFSM_Transition&& other) noexcept;
} FSM_TRANSITION;

/*-------------------
 *    FSM_STATE
 --------------------*/
// FSM�� ���¸� �����ϴ� ����ü
typedef struct ENGINE_DLL tagFSM_State
{
	// ���� �̸�
	const _tchar* pStateName;
	// �� ���¸� ������ �� �����ϴ� �Լ�
	std::function<void()> OnStart = nullptr;
	// �� ���¸� �ݺ��� �� �����ϴ� �Լ�
	std::function<void(_double)> Tick = nullptr;
	// �� ���¸� ������ �����ϴ� �Լ�
	std::function<void()> OnExit = nullptr;

	// �� transition vector�� priority�� ���ĵǾ priority�� ���� transition�� ���� Ȯ���Ѵ�.
	// �� ���¿��� �ٸ� ���·� ���̵Ǵ� ����(transtition)�� ��
	map<const _tchar*, vector<FSM_TRANSITION>> mapTransition;
	//key : �̵��� node�̸�, value : �̵� ����

	tagFSM_State(const _tchar* szNodeName) : pStateName(szNodeName) {}

	// ���� �� �̵� ������
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

	// CFSMComponentBuilder�� ���� FMS�� ����� �̸� pArg�� �����ش�.
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
 * FSM�� ���(����)�� Ʈ������(���º�������)�� �����ϴ� ���� ���� Ŭ����.
 * ��������?  ������ ��ü�� �����ϴ� Ŭ����(CFSMComponentBuilder)�� ǥ���ϴ� Ŭ����CFSMComponent)�� �и��Ͽ�,
 * ������ ���������� ���� �ٸ� ǥ���� �����ϴ� ����� �����Ѵ�. �ڼ��Ѱ� ���۸�
 *
 * ��뿹��
 * 	CFSMComponentBuilder builder = CFSMComponentBuilder() // ���� ����
	.InitState(TEXT("Idle"))								  // ���� ���� ����� �̸�
	.AddState(TEXT("Idle"))								  // Idle ���³�� ���� ����
		.OnStart(this, &CBackGround::Idle_OnStart)	      // Idle ���� ������ �� CBackGround::Idle_OnStart�Լ� ����(���´� �ϳ��� �Լ� ����, ��������)
		.Tick(this, &CBackGround::Idle_Tick)		      // Idle ���� ���� �� �� �����Ӹ��� CBackGround::Idle_Tick�Լ� ����(���´� �ϳ��� �Լ� ����, ��������)
		.OnExit(this, &CBackGround::Idle_OnExit)          // Idle ���¿��� �ٸ� ���·� �̵��� �� �����ϴ� �Լ� ����(���´� �ϳ��� �Լ� ����, ��������)
		.Transition(TEXT("Walk"), FSM_TRANSITION(TEXT("Idle To Walk KeyInput"), this, &CBackGround::Idle2Walk_KeyInput))
													      // Idle���� Walk�� �����ϴ� ���� ����, CBackGround::Predic_Idle2Walk�Լ� ������
													      // true�� Walk�� �����Ѵ�. ���� �ݺ�
		.Transition(TEXT("Walk"), FSM_TRANSITION(TEXT("Idle To Walk Pushed"), this, &CBackGround::Idle2Walk_Pushed))
													      // �ټ� transition���� ����
	.AddState(TEXT("Walk")) // Walk���� ��� ���� ����
		.Transition(TEXT("Idle"), FSM_TRANSITION(L"Walk To Idle", this, &CBackGround::Predic_Walk2Idle))
	.Build();										      // ��� ���¸� ����� Build()�Լ��� ���� Builder�� �����.(�����Լ�)

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_FSM"), TEXT("Com_FSM"),
		(CComponent**)&m_pFSM, &builder)))			      // ������� CFSMComponentBuilder�� �����͸� pArg�� �Ѱ��ش�.
													      // �̷��� �Ѱ��� Ŭ������ "�̵�" �ǹǷ� �ٽ� ����� �� ����.

  �� ���ô� ����Լ��� ��������� ���� �� �Լ� �����͵� ���ڷ� ��밡��
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
					return left.iPriority < right.iPriority; // ��������
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