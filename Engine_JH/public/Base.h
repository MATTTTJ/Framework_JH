#pragma once
#include "Engine_Defines.h"


BEGIN(Engine)
class ENGINE_DLL CBase abstract
{
protected:
	CBase();
	virtual ~CBase() = default;

public:
	unsigned long	AddRef();
	unsigned long	Release();

private:
	unsigned long	m_dwRefCnt = 0;

public:
	virtual void Free() = 0; // PURE


};
END
