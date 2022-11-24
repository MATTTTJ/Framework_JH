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
	// �ٸ� state�� ���̰� �Ұ����� �� ���� �ݺ��Ͽ� ����
	while (--iLoopCnt)
	{
		const auto CurItr = find_if(m_mapState.begin(), m_mapState.end(), CTag_Finder(m_pCurStateName));
		assert(CurItr != m_mapState.end());

		const FSM_STATE& tCurState = CurItr->second;

		const _tchar* pNextStateName = nullptr;
		// �� ���¿��� ���� ���·� �̵� �� �� �ִ��� Ȯ��
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

		// ���� ���� ��� �߰� �ϸ�
		if (pNextStateName)
		{
			const auto NextItr = find_if(m_mapState.begin(), m_mapState.end(), CTag_Finder(pNextStateName));
			assert(NextItr != m_mapState.end());
			const FSM_STATE& tNextState = NextItr->second;

			if (tCurState.OnExit != nullptr) // �� ��� ����
				tCurState.OnExit();

			if (tNextState.OnStart != nullptr) // ���� ��� ����
				tNextState.OnStart();

			StateHistoryUpdate(tNextState.pStateName);

			m_pCurStateName = pNextStateName;
		}
		else // ���� ���� �߰� ���ϸ� tick�ϰ� ����
		{
			if (tCurState.Tick != nullptr)
				tCurState.Tick(TimeDelta);
			break;
		}
	}

	assert(iLoopCnt > 0); // ���ѷ��� üũ
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
	// Imgui_RenderProperty�� ���� �� ���� ���� ������ ����Ѵ�.
	if (m_bStoreHistory) // ����׿�
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
