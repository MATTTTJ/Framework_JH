#include "stdafx.h"
#include "..\public\Loader.h"
#include "GameInstance.h"
#include "BackGround.h"
#include "ForkLift.h"
#include "Home.h"
#include "LaiLuo_Home.h"
#include "Monster.h"
#include "Player.h"
#include "Terrain.h"
#include "Weapon.h"


CLoader::CLoader(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: m_pDevice(pDevice)
	, m_pContext(pContext)
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
}

_uint APIENTRY LoadingThread(void* pArg)
{
	CLoader*		pLoader = (CLoader*)pArg;

	EnterCriticalSection(&pLoader->Get_Critical_Section());

	switch (pLoader->Get_NextLevelID())
	{
	case LEVEL_LOGO:
		pLoader->Loading_For_Logo();
		break;
	case LEVEL_GAMEPLAY:
		pLoader->Loading_For_GamePlay();
		break;
	// case LEVEL_MAPEDITOR:
	// 	pLoader->Loading_For_MapTool();
	// 	break;
	}

	LeaveCriticalSection(&pLoader->Get_Critical_Section());

	return 0;
}

HRESULT CLoader::Initialize(LEVEL eNextLevelID)
{
	m_eNextLevelID = eNextLevelID;

	InitializeCriticalSection(&m_Critical_Section);

	/* 로딩을 하기위한 추가적인 흐름을 만든다 (Thread).*/
	m_hThread = (HANDLE)_beginthreadex(nullptr, 0, LoadingThread, this, 0, nullptr);
	if (0 == m_hThread)
		return E_FAIL;

	return S_OK;
}

/* 로고를 위한 원형을 생성한다. */
HRESULT CLoader::Loading_For_Logo()
{
	CGameInstance*		pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	m_wstrLoadingText = L"텍스쳐를 로딩중입니다.";

	// if (FAILED(pGameInstance->Add_Prototype(LEVEL_LOGO, L"Prototype_Component_Texture_Logo",
	// 	CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/Default%d.jpg", 2))))
	// 	return E_FAIL;


	// FAILED_CHECK_RETURN(pGameInstance->Add_Prototype(LEVEL_LOADING, L"Prototype_Component_FirstLoadingTexture", CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/2DTexture/Loading/Loading%d.png", 0)), E_FAIL);
	// FAILED_CHECK_RETURN(pGameInstance->Add_Prototype(LEVEL_LOADING, L"Prototype_Component_SecondLoadingTexture", CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/2DTexture/Loading/Loading%d.png", 1)), E_FAIL);
	// FAILED_CHECK_RETURN(pGameInstance->Add_Prototype(LEVEL_LOADING, L"Prototype_Component_ThirdLoadingTexture", CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/2DTexture/Loading/Loading%d.png", 2)), E_FAIL);
	m_wstrLoadingText = L"버퍼를 로딩중입니다.";

	m_wstrLoadingText = L"모델을 로딩중입니다.";

	m_wstrLoadingText = L"셰이더를 로딩중입니다.";

	m_wstrLoadingText = L"객체원형을 생성중입니다.";

	/* For.Prototype_GameObject_BackGround */
	if (FAILED(pGameInstance->Add_Prototype(L"Prototype_GameObject_BackGround",
		CBackGround::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	// _matrix PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.f));
	// if (FAILED(pGameInstance->Add_Prototype(LEVEL_LOGO, L"Prototype_Component_Model_Home",
	// 	CModel::Create(m_pDevice, m_pContext, CModel::MODEL_NONANIM, "../Bin/Resources/Meshes/Lobby_Home/Test/ForkLift.fbx", PivotMatrix))))
	// 	return E_FAIL;

	m_wstrLoadingText = L"로딩끝.";

	m_bIsLoadingFinished = true;

	Safe_Release(pGameInstance);

	return S_OK;
}

/* 게임플레이를 위한 원형을 생성한다. */
HRESULT CLoader::Loading_For_GamePlay()
{
	CGameInstance*		pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	m_wstrLoadingText = L"텍스쳐를 로딩중입니다.";
	/* For.Prototype_Component_Texture_Terrain */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Texture_Terrain",
		CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/Terrain/Tile%d.dds", 2))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Brush*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Texture_Brush",
		CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/Terrain/Brush.png", 1))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Filter */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Texture_Filter",
		CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/Terrain/Filter.bmp", 1))))
		return E_FAIL;

	m_wstrLoadingText = L"버퍼를 로딩중입니다.";

	/* For.Prototype_Component_VIBuffer_Terrain */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_VIBuffer_Terrain",
		CVIBuffer_Terrain::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/Terrain/Height.bmp"))))
		return E_FAIL;

	m_wstrLoadingText = L"콜라이더를 로딩중입니다.";
	/* For.Prototype_Component_Collider_AABB*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Collider_AABB",
		CCollider::Create(m_pDevice, m_pContext, CCollider::COLLIDER_AABB))))
		return E_FAIL;
	/* For.Prototype_Component_Collider_OBB*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Collider_OBB",
		CCollider::Create(m_pDevice, m_pContext, CCollider::COLLIDER_OBB))))
		return E_FAIL;
	/* For.Prototype_Component_Collider_SPHERE*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Collider_SPHERE",
		CCollider::Create(m_pDevice, m_pContext, CCollider::COLLIDER_SPHERE))))
		return E_FAIL;

	m_wstrLoadingText = L"모델을 로딩중입니다. ";

	_matrix PivotMatrix = XMMatrixIdentity();
	/* For.Prototype_Component_Model_Fiona */
	PivotMatrix = XMMatrixRotationY(XMConvertToRadians(180.f));
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Model_Fiona",
		CModel::Create(m_pDevice, m_pContext, CModel::MODEL_ANIM, "../Bin/Resources/Meshes/Fiona/Fiona.fbx", PivotMatrix))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Home */
	PivotMatrix = XMMatrixScaling(0.009f, 0.009f, 0.009f) * XMMatrixRotationY(XMConvertToRadians(180.f)) *XMMatrixRotationX(XMConvertToRadians(90.f));
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Model_Home",
		CModel::Create(m_pDevice, m_pContext, CModel::MODEL_NONANIM, "../Bin/Resources/Meshes/Lobby_Home/TestHome1.fbx", PivotMatrix))))
		return E_FAIL; 

	/* For.Prototype_Component_Model_LaiHome */
	PivotMatrix = XMMatrixRotationY(XMConvertToRadians(180.f));
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Model_LaiHome",
		CModel::Create(m_pDevice, m_pContext, CModel::MODEL_ANIM, "../Bin/Resources/Meshes/LaiLuo_Home/LaiHome2.fbx", PivotMatrix))))
		return E_FAIL;

	// PivotMatrix = XMMatrixRotationY(XMConvertToRadians(180.f));
	PivotMatrix = XMMatrixIdentity();
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Model_LaiLuo",
		CModel::Create(m_pDevice, m_pContext, CModel::MODEL_ANIM, "../Bin/Resources/Meshes/Test.model", PivotMatrix))))
		return E_FAIL;

	// PivotMatrix = XMMatrixRotationY(XMConvertToRadians(180.f));

	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.f));
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Model_ForkLift",
		CModel::Create(m_pDevice, m_pContext, CModel::MODEL_NONANIM, "../Bin/Resources/Meshes/ForkLift/ForkLift.fbx", PivotMatrix))))
		return E_FAIL;

	

	m_wstrLoadingText = L"셰이더를 로딩중입니다. ";

	/* For.Prototype_Component_Shader_VtxNorTex */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Shader_VtxNorTex",
		CShader::Create(m_pDevice, m_pContext, L"../Bin/ShaderFiles/Shader_VtxNorTex.hlsl",CShader::DECLARATION_VTXNORTEX, VTXNORTEX_DECLARATION::Elements, VTXNORTEX_DECLARATION::iNumElements))))
		return E_FAIL;

	/* For.Prototype_Component_Shader_VtxModel */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Shader_VtxModel",
		CShader::Create(m_pDevice, m_pContext, L"../Bin/ShaderFiles/Shader_VtxModel.hlsl",CShader::DECLARATION_VTXMODEL, VTXMODEL_DECLARATION::Elements, VTXMODEL_DECLARATION::iNumElements))))
		return E_FAIL;

	/* For.Prototype_Component_Shader_VtxAnimModel*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxAnimModel"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxAnimModel.hlsl"), CShader::DECLARATION_VTXANIMMODEL, VTXANIMMODEL_DECLARATION::Elements, VTXANIMMODEL_DECLARATION::iNumElements))))
		return E_FAIL;

	m_wstrLoadingText = L"객체원형을 생성중입니다. ";

	/* For.Prototype_GameObject_Terrain */
	// if (FAILED(pGameInstance->Add_Prototype(L"Prototype_GameObject_Terrain",
	// 	CTerrain::Create(m_pDevice, m_pContext))))
	// 	return E_FAIL;
	//
	// /* For.Prototype_GameObject_Player */
	// if (FAILED(pGameInstance->Add_Prototype(L"Prototype_GameObject_Player",
	// 	CPlayer::Create(m_pDevice, m_pContext))))
	// 	return E_FAIL;
	FAILED_CHECK_RETURN(pGameInstance->Add_Prototype(L"Prototype_GameObject_Terrain", CTerrain::Create(m_pDevice, m_pContext)), E_FAIL);
	FAILED_CHECK_RETURN(pGameInstance->Add_Prototype(L"Prototype_GameObject_Player", CPlayer::Create(m_pDevice, m_pContext)), E_FAIL);
	FAILED_CHECK_RETURN(pGameInstance->Add_Prototype(L"Prototype_GameObject_ForkLift", CForkLift::Create(m_pDevice, m_pContext)), E_FAIL);
	FAILED_CHECK_RETURN(pGameInstance->Add_Prototype(L"Prototype_GameObject_Monster", CMonster::Create(m_pDevice, m_pContext)), E_FAIL);
	FAILED_CHECK_RETURN(pGameInstance->Add_Prototype(L"Prototype_GameObject_Home", CHome::Create(m_pDevice, m_pContext)), E_FAIL);
	FAILED_CHECK_RETURN(pGameInstance->Add_Prototype(L"Prototype_GameObject_LaiHome", CLaiLuo_Home::Create(m_pDevice, m_pContext)), E_FAIL);
	FAILED_CHECK_RETURN(pGameInstance->Add_Prototype(L"Prototype_GameObject_Weapon",CWeapon::Create(m_pDevice, m_pContext)), E_FAIL);

	m_wstrLoadingText = L"로딩끝. ";

	m_bIsLoadingFinished = true;

	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CLoader::Loading_For_MapTool()
{
	CGameInstance*		pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	m_wstrLoadingText = L"텍스쳐를 로딩중입니다.";
	/* For.Prototype_Component_Texture_Terrain */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Texture_Terrain",
		CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/Terrain/Tile%d.dds", 2))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Brush*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Texture_Brush",
		CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/Terrain/Brush.png", 1))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Filter */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Texture_Filter",
		CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/Terrain/Filter.bmp", 1))))
		return E_FAIL;

	m_wstrLoadingText = L"버퍼를 로딩중입니다.";

	/* For.Prototype_Component_VIBuffer_Terrain */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_VIBuffer_Terrain",
		CVIBuffer_Terrain::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/Terrain/Height.bmp"))))
		return E_FAIL;

	m_wstrLoadingText = L"모델을 로딩중입니다. ";

	// /* For.Prototype_Component_Model_Fiona */
	// if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Model_Fiona",
	// 	CModel::Create(m_pDevice, m_pContext, CModel::MODEL_ANIM, "../Bin/Resources/Meshes/Fiona/Fiona.fbx"))))
	// 	return E_FAIL;

	m_wstrLoadingText = L"셰이더를 로딩중입니다. ";

	/* For.Prototype_Component_Shader_VtxNorTex */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Shader_VtxNorTex",
		CShader::Create(m_pDevice, m_pContext, L"../Bin/ShaderFiles/Shader_VtxNorTex.hlsl",CShader::DECLARATION_VTXNORTEX, VTXNORTEX_DECLARATION::Elements, VTXNORTEX_DECLARATION::iNumElements))))
		return E_FAIL;




	m_wstrLoadingText = L"객체원형을 생성중입니다. ";

	/* For.Prototype_GameObject_Terrain */
	if (FAILED(pGameInstance->Add_Prototype(L"Prototype_GameObject_Terrain",
		CTerrain::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	m_wstrLoadingText = L"로딩끝. ";

	m_bIsLoadingFinished = true;

	Safe_Release(pGameInstance);

	return S_OK;
}

CLoader * CLoader::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, LEVEL eNextLevelID)
{
	CLoader*		pInstance = new CLoader(pDevice, pContext);

	if (FAILED(pInstance->Initialize(eNextLevelID)))
	{
		MSG_BOX("Failed to Created : CLoader");
		Safe_Release(pInstance);
	}
	return pInstance;
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
