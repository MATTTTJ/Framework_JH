#pragma once

namespace Engine
{
	class CTag_Finder
	{
	public:
		explicit CTag_Finder(const wchar_t* pTag)
			:	m_pTargetTag(pTag)
		{
		}
		~CTag_Finder(){}
	public:
		template<typename T>
		bool operator()(const T& pair)
		{
			if( 0 == lstrcmpW(m_pTargetTag,pair.first))
			{
				return true;
			}

			return false;
		}

	private:
		const wchar_t*		m_pTargetTag = nullptr;
	};


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
	void	Safe_Delete_Array(T& pPointer)
	{
		if (nullptr != pPointer)
		{
			delete[] pPointer;
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
