#pragma once
#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CState : public CComponent
{
private:
	typedef struct tagState
	{
		std::function<void(_double)>		State_Start = nullptr;
		std::function<void(_double)>		State_Tick = nullptr;
		std::function<void(_double)>		State_End = nullptr;
	}STATE;

	typedef struct tagChanger
	{
		const wstring						wstrNextState = L"";
		std::function<_bool(void)>			Changer_Func = nullptr;
	}CHANGER;

private:
	CState(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	CState(const CState& rhs);
	virtual ~CState() = default;

public:
	virtual HRESULT	Initialize_Proto(void);
	virtual HRESULT Initialize_Clone(CGameObject* pOwner, void* pArg = nullptr);
	void			Tick(_double TimeDelta);

public:
	CState&	Set_Root(const wstring&	wstrStateName);
	CState&	Add_State(const wstring& wstrStateName);
	CState&	Finish_Setting(void)
	{
		m_wstrCurStateName = m_wstrRootStateName;
		return *this;
	}
	
public:
	template<typename T>
	CState& Init_Start(T* obj, void (T::*memFunc)(_double))
	{
		const auto itr = find_if(m_mapState.begin(), m_mapState.end(), CTag_Finder(m_wstrCurStateName));
		assert(itr != m_mapState.end());

		itr->second.State_Start = [obj, memFunc](_double TimeDelta)
		{
			(obj->*memFunc)(TimeDelta);
		};
		return *this;
	}

	template<typename T>
	CState& Init_Tick(T* obj, void (T::*memFunc)(_double))
	{
		const auto iter = find_if(m_mapState.begin(), m_mapState.end(), CTag_Finder(m_wstrCurStateName));
		assert(iter != m_mapState.end());

		iter->second.State_Tick = [obj, memFunc](_double TimeDelta)
		{
			(obj->*memFunc)(TimeDelta);
		};
		return *this;
	}

	template<typename T>
	CState& Init_End(T* obj, void (T::*memFunc)(_double))
	{
		const auto iter = find_if(m_mapState.begin(), m_mapState.end(), CTag_Finder(m_wstrCurStateName));
		assert(iter != m_mapState.end());

		iter->second.State_End = [obj, memFunc](_double TimeDelta)
		{
			(obj->*memFunc)(TimeDelta);
		};
		return *this;
	}

	template<typename T>
	CState& Init_Changer(const wstring wstrNextState, T* obj, _bool(T::*memFunc)())
	{
		const auto iter = find_if(m_mapChanger.begin(), m_mapChanger.end(), CTag_Finder(m_wstrCurStateName));

		CHANGER	Changer;
		ZeroMemory(&Changer, sizeof(CHANGER));

		Changer.wstrNextState = wstrNextState;
		Changer.Changer_Func = [obj, memFunc]()
		{
			return (obj->*memFunc)();
		};

		if (iter == m_mapChanger.end())
			m_mapChanger.emplace(m_wstrCurStateName, list<CHANGER>{Changer});
		else
			iter->second.push_back(Changer);

		return *this;
	}

private:
	map<const wstring, STATE>	m_mapState;
	wstring				m_wstrRootStateName = L"";
	wstring				m_wstrCurStateName = L"";
	wstring				m_wstrNextStateName = L"";
	wstring				m_wstrBeforeStateName = L"";

private:
	map<const wstring, list<CHANGER>>	m_mapChanger;

public:
	static	CState*		Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CComponent* Clone(CGameObject* pOwner, void* pArg = nullptr);
	virtual void		Free(void);
};

END