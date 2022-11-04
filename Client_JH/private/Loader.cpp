#include "stdafx.h"
#include "Loader.h"


CLoader::CLoader(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:m_pDevice(pDevice),
	 m_pContext(pContext)
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
}

_uint APIENTRY	LoadingThread(void* pArg)
{
	CLoader*	pLoader = (CLoader*)pArg;

	EnterCriticalSection(&pLoader->Get_Critical_Section());

	switch (pLoader->Get_NextLevelID())
	{
		case LEVEL_LOGO:
			pLoader->Loading_For_Logo();
			break;
		case LEVEL_GAMEPLAY:
			pLoader->Loading_For_GamePlay();
			break;
	}

	LeaveCriticalSection(&pLoader->Get_Critical_Section());

	return 0;
}

HRESULT CLoader::Initialize(LEVEL eNextLevelID)
{
	m_eNextLevelID = eNextLevelID;

	InitializeCriticalSection(&m_Critical_Section);

	m_hThread = (HANDLE)_beginthreadex(nullptr, 0,
		LoadingThread, this, 0, nullptr);

	if (m_hThread == 0)
		return E_FAIL;

	return S_OK;
}

HRESULT CLoader::Loading_For_Logo()
{
	lstrcpy(m_szLoadingText, TEXT("텍스쳐를 로딩중입니다. "));

	lstrcpy(m_szLoadingText, TEXT("버퍼를 로딩중입니다. "));

	lstrcpy(m_szLoadingText, TEXT("모델을 로딩중입니다. "));

	lstrcpy(m_szLoadingText, TEXT("셰이더를 로딩중입니다. "));

	lstrcpy(m_szLoadingText, TEXT("로딩끝. "));

	m_bIsLoadingFinished = true;

	return S_OK;
}

HRESULT CLoader::Loading_For_GamePlay()
{
	return S_OK;
}

CLoader* CLoader::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, LEVEL eNextLevelID)
{
	CLoader* pInst = new CLoader(pDevice,pContext);

	if(FAILED(pInst->Initialize(eNextLevelID)))
	{
		MSG_BOX("Failed to Created : CLoader");
		Safe_Release(pInst);
	}

	return pInst;
}

void CLoader::Free()
{
	WaitForSingleObject(m_hThread, INFINITE);
	CloseHandle(m_hThread);
	DeleteObject(m_hThread);
	DeleteCriticalSection(&m_Critical_Section);

	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);
}
