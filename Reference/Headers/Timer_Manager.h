#pragma once
#include "Timer.h"

BEGIN(Engine)

class CTimer_Manager final : public CBase
{
	DECLARE_SINGLETON(CTimer_Manager)
public:
	CTimer_Manager();
	virtual ~CTimer_Manager() = default;

public:
	_double		Get_TimeDelta(const wstring pTimerTag);


public:
	HRESULT		Ready_Timer(const wstring pTimerTag);
	void		Update_Timer(const wstring pTimerTag);

private:
	CTimer*		Find_Timer(const wstring pTimerTag);

private:
	map<const wstring, CTimer*>		m_mapTimers;

public:
	virtual void Free(void);
};

END