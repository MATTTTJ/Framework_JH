#include "Base.h"



CBase::CBase()
	: m_dwRefCnt(0)
{
}

unsigned long CBase::AddRef()
{
	return ++m_dwRefCnt;
}

unsigned long CBase::Release()
{
	if (m_dwRefCnt == 0)
	{
		Free();

		delete this;

		return 0;
	}
	else
		return m_dwRefCnt--;		 // 후위연산자로 해주는 이유는, 0이되면 삭제가 되어야하는데 전위 연산자로 하면 0이 됨과 동시에 리턴시켜버리기 때문에 삭제가 되지 않는다.
									 // 그러므로 표시는 1로 되더라도 실제 변수 값은 0이 되도록 후위 연산자로 처리함
}

