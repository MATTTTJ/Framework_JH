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

	switch (pLoader->Get_NextLevelID())
	{
		case LEVEL_LOGO:
			pLoader->Loading_For_Logo();
			break;
		case LEVEL_GAMEPLAY:
			pLoader->Loading_For_GamePlay();
			break;
	}

	return 0;
}

HRESULT CLoader::Initialize(LEVEL eNextLevelID)
{
	m_eNextLevelID = eNextLevelID;

	m_hTread = (HANDLE)_beginthreadex(nullptr, 0,
		LoadingThread, this, 0, nullptr);

	if (m_hTread == 0)
		return E_FAIL;

	return S_OK;
}

HRESULT CLoader::Loading_For_Logo()
{
	lstrcpy(m_szLoadingText, TEXT("텍스쳐를 로딩중입니다. "));
	for (_uint i = 0; i < 999999999; ++i)
		int a = 10;

	lstrcpy(m_szLoadingText, TEXT("버퍼를 로딩중입니다. "));
	for (_uint i = 0; i < 999999999; ++i)
		int a = 10;

	lstrcpy(m_szLoadingText, TEXT("모델을 로딩중입니다. "));
	for (_uint i = 0; i < 999999999; ++i)
		int a = 10;

	lstrcpy(m_szLoadingText, TEXT("셰이더를 로딩중입니다. "));
	for (_uint i = 0; i < 999999999; ++i)
		int a = 10;

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
	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);
}
