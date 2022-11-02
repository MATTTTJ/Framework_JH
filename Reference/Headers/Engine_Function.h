#pragma once

namespace Engine
{
	template<typename T>
	void	Safe_Delete(T& pPointer)
	{
		if(pPointer != nullptr)
		{
			delete pPointer;
			pPointer = nullptr;
		}
	}

	template<typename T>
	unsigned long	Safe_AddRef(T& pInstance)
	{
		unsigned long dwRefCnt = 0;

		if(pInstance != nullptr)
			dwRefCnt = pInstance->AddRef();

		return dwRefCnt;
	}

	template<typename T>
	unsigned long	Safe_Release(T& pInstance)
	{
		unsigned long dwRefCnt = 0;
		if (pInstance != nullptr)
		{
			dwRefCnt = pInstance->Release();

			if (dwRefCnt == 0)
				pInstance = nullptr;
		}
		return dwRefCnt;
	}
}
