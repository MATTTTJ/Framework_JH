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
		return m_dwRefCnt--;		 // ���������ڷ� ���ִ� ������, 0�̵Ǹ� ������ �Ǿ���ϴµ� ���� �����ڷ� �ϸ� 0�� �ʰ� ���ÿ� ���Ͻ��ѹ����� ������ ������ ���� �ʴ´�.
									 // �׷��Ƿ� ǥ�ô� 1�� �Ǵ��� ���� ���� ���� 0�� �ǵ��� ���� �����ڷ� ó����
}

