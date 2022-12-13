#include "..\Public\StateMachine.h"
#include "GameUtils.h"
#include <sstream>

/* FSM_TRANSITION */
tagFSM_Transition::tagFSM_Transition(const wstring & wstrTransitionName, const std::function<_bool()>& Predicator, _uint iPriority)
	: wstrTransitionName(wstrTransitionName)
	, iPriority(iPriority)
	, Predicator(Predicator)
{
}

tagFSM_Transition::tagFSM_Transition(const tagFSM_Transition & rhs)
	: wstrTransitionName(rhs.wstrTransitionName)
	, iPriority(rhs.iPriority)
	, Predicator(rhs.Predicator)
{
}

tagFSM_Transition::tagFSM_Transition(tagFSM_Transition && rhs) noexcept
	: wstrTransitionName(rhs.wstrTransitionName)
	, iPriority(rhs.iPriority)
	, Predicator(std::move(rhs.Predicator))
{
}

tagFSM_Transition & tagFSM_Transition::operator=(const tagFSM_Transition & rhs)
{
	if (this == &rhs)
		return *this;

	wstrTransitionName = rhs.wstrTransitionName;
	iPriority = rhs.iPriority;
	Predicator = rhs.Predicator;

	return *this;
}

tagFSM_Transition & tagFSM_Transition::operator=(tagFSM_Transition && rhs) noexcept
{
	if (this == &rhs)
		return *this;

	wstrTransitionName = rhs.wstrTransitionName;
	iPriority = rhs.iPriority;
	Predicator = std::move(rhs.Predicator);

	return *this;
}

/* FSM_STATE */
tagFSM_State::tagFSM_State(const tagFSM_State & rhs)
	: wstrStateName(rhs.wstrStateName)
	, OnStart(rhs.OnStart)
	, Tick(rhs.Tick)
	, OnExit(rhs.OnExit)
	, mapTransition(rhs.mapTransition)
{
}

tagFSM_State::tagFSM_State(tagFSM_State && rhs) noexcept
	: wstrStateName(rhs.wstrStateName)
	, OnStart(std::move(rhs.OnStart))
	, Tick(std::move(rhs.Tick))
	, OnExit(std::move(rhs.OnExit))
	, mapTransition(std::move(rhs.mapTransition))
{
}

tagFSM_State & tagFSM_State::operator=(const tagFSM_State & rhs)
{
	if (this == &rhs)
		return *this;

	wstrStateName = rhs.wstrStateName;
	OnStart = rhs.OnStart;
	Tick = rhs.Tick;
	OnExit = rhs.OnExit;
	mapTransition = rhs.mapTransition;

	return *this;
}

tagFSM_State & tagFSM_State::operator=(tagFSM_State && rhs) noexcept
{
	if (this == &rhs)
		return *this;

	wstrStateName = rhs.wstrStateName;
	OnStart = std::move(rhs.OnStart);
	Tick = std::move(rhs.Tick);
	OnExit = std::move(rhs.OnExit);
	mapTransition = std::move(rhs.mapTransition);

	return *this;
}

CStateMachine::CStateMachine()
	: CComponent(nullptr, nullptr)
{
}

CStateMachine::CStateMachine(const CStateMachine & rhs)
	: CComponent(rhs)
{
}

HRESULT CStateMachine::Initialize_Clone(CGameObject* pOwner, void * pArg)
{
	FAILED_CHECK_RETURN(__super::Initialize_Clone(pOwner, pArg), E_FAIL);

	CStateMachineBuilder*		pBuilder = static_cast<CStateMachineBuilder*>(pArg);

	m_wstrCurrentStateName = pBuilder->Get_InitStateName();
	m_mapState = std::move(pBuilder->GetStates());

	return S_OK;
}

void CStateMachine::Imgui_RenderProperty()
{
	char	szCurrentNode[256];
	CGameUtils::wc2c(m_wstrCurrentStateName.c_str(), szCurrentNode);

	ImGui::Text("Current State : %s", szCurrentNode);

	if (ImGui::BeginListBox("Node Transition History"))
	{
		for (const auto& e : m_strDebugQue)
			ImGui::Selectable(e.c_str());
		ImGui::EndListBox();
	}

	_int	iInput = (_int)m_iDebugQueSize;
	ImGui::InputInt("Debug History Size", &iInput);

	if (iInput >= 0)
		m_iDebugQueSize = iInput;

	m_bStoreHistory = true;
}

void CStateMachine::Tick(_double dTimeDelta)
{
	_int	iLoopCount = 5000;

	while (--iLoopCount)
	{
		const auto CurrentIter = find_if(m_mapState.begin(), m_mapState.end(), CTag_Finder(m_wstrCurrentStateName));
		assert(CurrentIter != m_mapState.end());

		const FSM_STATE& tCurrentState = CurrentIter->second;

		wstring		wstrNextStateName = L"";
		for (auto& Pair : tCurrentState.mapTransition)
		{
			for (auto& Transition : Pair.second)
			{
				if (Transition.Predicator())
				{
					wstrNextStateName = Pair.first;
					m_wstrLastTransitionName = Transition.wstrTransitionName;
					break;
				}
			}

			if (wstrNextStateName != L"")
				break;
		}

		if (wstrNextStateName != L"")
		{
			const auto NextIter = find_if(m_mapState.begin(), m_mapState.end(), CTag_Finder(wstrNextStateName));
			assert(NextIter != m_mapState.end());

			const FSM_STATE&	tNextState = NextIter->second;

			if (tCurrentState.OnExit != nullptr)
				tCurrentState.OnExit();

			if (tNextState.OnStart != nullptr)
				tNextState.OnStart();

			StateHistoryUpdate(tNextState.wstrStateName);

			m_wstrCurrentStateName = wstrNextStateName;
		}
		else
		{
			if (tCurrentState.Tick != nullptr)
				tCurrentState.Tick(dTimeDelta);
			break;
		}
	}

	assert(iLoopCount > 0);
}

void CStateMachine::StateHistoryUpdate(const wstring & wstrNextStateName)
{
	if (!m_bStoreHistory)
		return;

	char From[256], To[256];
	CGameUtils::wc2c(m_wstrLastTransitionName.c_str(), From);
	CGameUtils::wc2c(wstrNextStateName.c_str(), To);

	std::stringstream ss;
	ss << From << " => " << To;
	m_strDebugQue.push_front(ss.str());

	if (m_strDebugQue.size() > m_iDebugQueSize)
		m_strDebugQue.pop_back();

	m_bStoreHistory = false;
}

CStateMachine * CStateMachine::Create()
{
	return new CStateMachine;
}

CComponent * CStateMachine::Clone(CGameObject * pOwner, void * pArg)
{
	CStateMachine*		pInstance = new CStateMachine(*this);

	if (FAILED(pInstance->Initialize_Clone(pOwner, pArg)))
	{
		MSG_BOX("Failed to Clone : CStateMachine");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CStateMachine::Free()
{
}