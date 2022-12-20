#include "stdafx.h"
#include "..\Public\State.h"

CState::CState(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	:CComponent(pDevice, pDeviceContext)
{
}

CState::CState(const CState & rhs)
	:CComponent(rhs)
{
}

HRESULT CState::Initialize_Proto(void)
{
	FAILED_CHECK_RETURN(__super::Initialize_Prototype(), E_FAIL);

	return S_OK;
}

HRESULT CState::Initialize_Clone(CGameObject* pOwner, void * pArg)
{
	FAILED_CHECK_RETURN(__super::Initialize_Clone(pOwner, pArg), E_FAIL);

	return S_OK;
}

void CState::Tick(_double TimeDelta)
{
	auto CurState = find_if(m_mapState.begin(), m_mapState.end(), CTag_Finder(m_wstrCurStateName));

	for (auto Changer : m_mapChanger[m_wstrCurStateName])
	{
		if (true == Changer.Changer_Func())
		{
			m_wstrNextStateName = Changer.wstrNextState;
			break;
		}
	}
	
	if (m_wstrNextStateName != L"")
	{
		if(nullptr != CurState->second.State_End)
			CurState->second.State_End(TimeDelta);

		m_wstrBeforeStateName = m_wstrCurStateName;
		m_wstrCurStateName = m_wstrNextStateName;

		if (nullptr != m_mapState[m_wstrCurStateName].State_Start)
			m_mapState[m_wstrCurStateName].State_Start(TimeDelta);

		m_wstrNextStateName = L"";
	}
	else
	{
		if (nullptr != CurState->second.State_Tick)
			CurState->second.State_Tick(TimeDelta);
	}
}

CState & CState::Set_Root(const wstring& wstrStateName)
{
	m_wstrRootStateName = wstrStateName;

	return *this;
}

CState & CState::Add_State(const wstring& wstrStateName)
{
	STATE eState;
	ZeroMemory(&eState, sizeof(eState));

	m_mapState.emplace(wstrStateName, eState);
	m_wstrCurStateName = wstrStateName;

	return *this;
}

CState * CState::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
{
	CState* pInstance = new CState(pDevice, pDeviceContext);

	if (FAILED(pInstance->Initialize_Proto()))
	{
		MSG_BOX("State Create Fail");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

CComponent * CState::Clone(CGameObject* pOwner, void * pArg)
{
	CState* pInstance = new CState(*this);

	if (FAILED(pInstance->Initialize_Clone(pOwner, pArg)))
	{
		MSG_BOX("State Clone Fail");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CState::Free(void)
{
	__super::Free();
}
