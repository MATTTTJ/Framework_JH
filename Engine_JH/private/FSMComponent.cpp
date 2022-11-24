#include "..\public\FSMComponent.h"
#include "GameUtils.h"
#include <sstream>

/*-------------------
 *    FSM_TRANSITION
 --------------------*/
tagFSM_Transition::tagFSM_Transition(const _tchar* pTransitionName, const std::function<_bool()>& Predicator,
                                     _uint iPriority): pTransitionName(pTransitionName),
                                                       iPriority(iPriority),
                                                       Predicator(Predicator)
{
}

tagFSM_Transition::tagFSM_Transition(const tagFSM_Transition& other): pTransitionName(other.pTransitionName),
                                                                      iPriority(other.iPriority),
                                                                      Predicator(other.Predicator)
{
}

tagFSM_Transition::tagFSM_Transition(tagFSM_Transition&& other) noexcept: pTransitionName(other.pTransitionName),
                                                                          iPriority(other.iPriority),
                                                                          Predicator(std::move(other.Predicator))
{
}

tagFSM_Transition& tagFSM_Transition::operator=(const tagFSM_Transition& other)
{
	if (this == &other)
		return *this;
	pTransitionName = other.pTransitionName;
	iPriority = other.iPriority;
	Predicator = other.Predicator;
	return *this;
}

tagFSM_Transition& tagFSM_Transition::operator=(tagFSM_Transition&& other) noexcept
{
	if (this == &other)
		return *this;
	pTransitionName = other.pTransitionName;
	iPriority = other.iPriority;
	Predicator = std::move(other.Predicator);
	return *this;
}

/*-------------------
 *    FSM_STATE
 --------------------*/
tagFSM_State::tagFSM_State(const tagFSM_State& other): pStateName(other.pStateName),
                                                       OnStart(other.OnStart),
                                                       Tick(other.Tick),
                                                       OnExit(other.OnExit),
                                                       mapTransition(other.mapTransition)
{
}

tagFSM_State::tagFSM_State(tagFSM_State&& other) noexcept: pStateName(other.pStateName),
                                                           OnStart(std::move(other.OnStart)),
                                                           Tick(std::move(other.Tick)),
                                                           OnExit(std::move(other.OnExit)),
                                                           mapTransition(std::move(other.mapTransition))
{
}

tagFSM_State& tagFSM_State::operator=(const tagFSM_State& other)
{
	if (this == &other)
		return *this;
	pStateName = other.pStateName;
	OnStart = other.OnStart;
	Tick = other.Tick;
	OnExit = other.OnExit;
	mapTransition = other.mapTransition;
	return *this;
}

tagFSM_State& tagFSM_State::operator=(tagFSM_State&& other) noexcept
{
	if (this == &other)
		return *this;
	pStateName = other.pStateName;
	OnStart = std::move(other.OnStart);
	Tick = std::move(other.Tick);
	OnExit = std::move(other.OnExit);
	mapTransition = std::move(other.mapTransition);
	return *this;
}

/*-------------------
 *    CFSMComponent
 --------------------*/
CFSMComponent::CFSMComponent(): CComponent(nullptr, nullptr)
{
}

CFSMComponent::CFSMComponent(const CFSMComponent& rhs): CComponent(rhs)
{
}

void CFSMComponent::Tick(_double TimeDelta)
{
	int iLoopCnt = 5000;
	// 다른 state로 전이가 불가해질 때 가지 반복하여 전이
	while (--iLoopCnt)
	{
		const auto CurItr = find_if(m_mapState.begin(), m_mapState.end(), CTag_Finder(m_pCurStateName));
		assert(CurItr != m_mapState.end());

		const FSM_STATE& tCurState = CurItr->second;

		const _tchar* pNextStateName = nullptr;
		// 현 상태에서 다음 상태로 이동 할 수 있는지 확인
		for (auto& Pair : tCurState.mapTransition)
		{
			for (auto& transition : Pair.second/*vector<FSM_TRANSITION>*/)
			{
				if (transition.Predicator())
				{
					pNextStateName = Pair.first;
					m_pLastTransitionName = transition.pTransitionName;
					break;
				}
			}

			if (pNextStateName) 
				break;
		}

		// 다음 상태 노드 발견 하면
		if (pNextStateName)
		{
			const auto NextItr = find_if(m_mapState.begin(), m_mapState.end(), CTag_Finder(pNextStateName));
			assert(NextItr != m_mapState.end());
			const FSM_STATE& tNextState = NextItr->second;

			if (tCurState.OnExit != nullptr) // 현 노드 종료
				tCurState.OnExit();

			if (tNextState.OnStart != nullptr) // 다음 노드 시작
				tNextState.OnStart();

			StateHistoryUpdate(tNextState.pStateName);

			m_pCurStateName = pNextStateName;
		}
		else // 다음 상태 발견 못하면 tick하고 종료
		{
			if (tCurState.Tick != nullptr)
				tCurState.Tick(TimeDelta);
			break;
		}
	}

	assert(iLoopCnt > 0); // 무한루프 체크
}

HRESULT CFSMComponent::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	CFSMComponentBuilder* pBuilder = static_cast<CFSMComponentBuilder*>(pArg);

	m_pCurStateName = pBuilder->GetInitStateName();
	m_mapState = std::move(pBuilder->GetStates());

	return S_OK;
}

CFSMComponent* CFSMComponent::Create()
{
	return new CFSMComponent;
}

CComponent* CFSMComponent::Clone(void* pArg)
{
	CFSMComponent* pInstance = new CFSMComponent(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CFSMComponent");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CFSMComponent::Imgui_RenderProperty()
{
	char szCurNode[256];
	CGameUtils::wc2c(m_pCurStateName, szCurNode);
	ImGui::Text("Cur State : %s", szCurNode);

	if (ImGui::BeginListBox("Node Transition History"))
	{
		for (const auto& e : m_strDebugQue)
			ImGui::Selectable(e.c_str());
		ImGui::EndListBox();
	}

	_int iInput = static_cast<_int>(m_iDebugQueSize);
	ImGui::InputInt("Debug History Size(unsigned)", &iInput);
	if (iInput >= 0) m_iDebugQueSize = iInput;

	m_bStoreHistory = true;
}

void CFSMComponent::StateHistoryUpdate(const _tchar* pNextStateName)
{
	// Imgui_RenderProperty가 실행 될 때만 상태 변경을 기록한다.
	if (m_bStoreHistory) // 디버그용
	{
		char by[256], to[256];
		// CGameUtils::wc2c(m_pCurNodeName, from);
		CGameUtils::wc2c(m_pLastTransitionName, by);
		CGameUtils::wc2c(pNextStateName, to);

		std::stringstream ss;
		ss << by << " => " << to;
		m_strDebugQue.push_front(ss.str());

		if (m_strDebugQue.size() > m_iDebugQueSize)
			m_strDebugQue.pop_back();

		m_bStoreHistory = false;
	}
}
