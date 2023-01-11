#include "stdafx.h"
#include "..\public\Loader.h"
#include "GameInstance.h"
#include "BackGround.h"
#include "Default_Pistol.h"
#include "Effect_Point_Instancing.h"
#include "ForkLift.h"
#include "Home.h"
#include "Human_Sword.h"
#include "LaiLuo_Home.h"
#include "Monster.h"
#include "MonsterUI.h"
#include "NumberUI.h"
#include "Player.h"
#include "PlayerUI_Base.h"
#include "Terrain.h"
#include "Weapon.h"
#include "Sky.h"

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
	m_wstrLoadingText = L"버퍼를 로딩중입니다.";
	m_wstrLoadingText = L"모델을 로딩중입니다.";
	m_wstrLoadingText = L"셰이더를 로딩중입니다.";
	m_wstrLoadingText = L"객체원형을 생성중입니다.";

	FAILED_CHECK_RETURN(pGameInstance->Add_Prototype(L"Prototype_GameObject_BackGround",	CBackGround::Create(m_pDevice, m_pContext)), E_FAIL);

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
	FAILED_CHECK_RETURN(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Texture_Sky", CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/SkyBox/Sky_%d.dds", 4)), E_FAIL);

	//플레이어 배경
	FAILED_CHECK_RETURN(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Texture_UI_PlayerInfo", CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/UI/Player_UI/InGame_PlayerUI/PlayerInfo.png", 1)), E_FAIL);

	//플레이어 체력
	FAILED_CHECK_RETURN(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Texture_UI_PlayerHp_BaseGrey", CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/UI/Player_UI/InGame_PlayerUI/Bar_Character_HPBar_Grey.png", 1)), E_FAIL);
	FAILED_CHECK_RETURN(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Texture_UI_PlayerHp_White", CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/UI/Player_UI/InGame_PlayerUI/Bar_Character_HPBar_White.png", 1)), E_FAIL);
	FAILED_CHECK_RETURN(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Texture_UI_PlayerHp", CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/UI/Player_UI/InGame_PlayerUI/Bar_Character_HPBar_2.png", 1)), E_FAIL);

	//플레이어 실드
	FAILED_CHECK_RETURN(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Texture_UI_PlayerShield_BaseGrey", CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/UI/Player_UI/InGame_PlayerUI/Bar_Character_SFBar_Grey.png", 1)), E_FAIL);
	FAILED_CHECK_RETURN(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Texture_UI_PlayerShield_White", CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/UI/Player_UI/InGame_PlayerUI/Bar_Character_SFBar_White.png", 1)), E_FAIL);
	FAILED_CHECK_RETURN(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Texture_UI_PlayerShield", CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/UI/Player_UI/InGame_PlayerUI/Bar_Character_SFBar_1.png", 1)), E_FAIL);

	// 플레이어 스킬
	FAILED_CHECK_RETURN(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Texture_UI_PlayerSkillOn", CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/UI/Player_UI/InGame_PlayerUI/On.png", 1)), E_FAIL);
	FAILED_CHECK_RETURN(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Texture_UI_PlayerSkillOff", CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/UI/Player_UI/InGame_PlayerUI/Off.png", 1)), E_FAIL);

	// 플레이어 스킬 베이스
	FAILED_CHECK_RETURN(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Texture_UI_PlayerSkill_Base", CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/UI/Player_UI/InGame_PlayerUI/Skill_Base.png", 1)), E_FAIL);


	// 플레이어 스킬 글로우 
	FAILED_CHECK_RETURN(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Texture_UI_PlayerSkillGlow", CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/UI/Player_UI/InGame_PlayerUI/Test_Glow.png", 1)), E_FAIL);
	FAILED_CHECK_RETURN(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Texture_UI_PlayerSkillGlow_1", CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/UI/Player_UI/InGame_PlayerUI/SkillGlow.png", 1)), E_FAIL);

	// 플레이어 총알 3종
	FAILED_CHECK_RETURN(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Texture_UI_PlayerBullet_Pistol", CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/UI/Player_UI/InGame_PlayerUI/PistolBullet.png", 1)), E_FAIL);
	FAILED_CHECK_RETURN(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Texture_UI_PlayerBullet_Rifle", CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/UI/Player_UI/InGame_PlayerUI/RifleBullet.png", 1)), E_FAIL);
	FAILED_CHECK_RETURN(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Texture_UI_PlayerBullet_Injector", CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/UI/Player_UI/InGame_PlayerUI/InjectorBullet.png", 1)), E_FAIL);

	// 플레이어 쓰로우
	FAILED_CHECK_RETURN(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Texture_UI_PlayerThrowOn", CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/UI/Player_UI/InGame_PlayerUI/img_Throw_On.png", 1)), E_FAIL);
	FAILED_CHECK_RETURN(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Texture_UI_PlayerThrowOff", CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/UI/Player_UI/InGame_PlayerUI/img_Throw_Off.png", 1)), E_FAIL);

	// 플레이어 대시
	FAILED_CHECK_RETURN(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Texture_UI_PlayerDashOn", CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/UI/Player_UI/InGame_PlayerUI/Dash_On.png", 1)), E_FAIL);
	FAILED_CHECK_RETURN(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Texture_UI_PlayerDashOff", CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/UI/Player_UI/InGame_PlayerUI/Dash_Off.png", 1)), E_FAIL);

	// 플레이어 총 베이스
	FAILED_CHECK_RETURN(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Texture_UI_PlayerGunInfo", CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/UI/Player_UI/InGame_PlayerUI/img_weaponbg.png", 1)), E_FAIL);

	// 플레이어 총 번호
	FAILED_CHECK_RETURN(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Texture_UI_PlayerGunNumber_1", CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/UI/Player_UI/InGame_PlayerUI/WeaponNum1.png", 1)), E_FAIL);
	FAILED_CHECK_RETURN(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Texture_UI_PlayerGunNumber_2", CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/UI/Player_UI/InGame_PlayerUI/WeaponNum2.png", 1)), E_FAIL);
	FAILED_CHECK_RETURN(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Texture_UI_PlayerGunNumber_3", CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/UI/Player_UI/InGame_PlayerUI/WeaponNum3.png", 1)), E_FAIL);

	// 플레이어 들고있는 총 사진
	FAILED_CHECK_RETURN(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Texture_UI_Default_Pistol", CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/UI/Player_UI/InGame_PlayerUI/1202.png", 1)), E_FAIL);
	FAILED_CHECK_RETURN(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Texture_UI_Flame_Bullet", CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/UI/Player_UI/InGame_PlayerUI/1205.png", 1)), E_FAIL);
	FAILED_CHECK_RETURN(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Texture_UI_Fire_Dragon", CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/UI/Player_UI/InGame_PlayerUI/1412.png", 1)), E_FAIL);
	FAILED_CHECK_RETURN(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Texture_UI_Poison", CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/UI/Player_UI/InGame_PlayerUI/1509.png", 1)), E_FAIL);

	// 숫자 텍스쳐
	FAILED_CHECK_RETURN(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Texture_Numbering_0", CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/UI/Player_UI/InGame_PlayerUI/0.png", 1)), E_FAIL);
	FAILED_CHECK_RETURN(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Texture_Numbering_1", CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/UI/Player_UI/InGame_PlayerUI/1.png", 1)), E_FAIL);
	FAILED_CHECK_RETURN(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Texture_Numbering_2", CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/UI/Player_UI/InGame_PlayerUI/2.png", 1)), E_FAIL);
	FAILED_CHECK_RETURN(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Texture_Numbering_3", CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/UI/Player_UI/InGame_PlayerUI/3.png", 1)), E_FAIL);
	FAILED_CHECK_RETURN(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Texture_Numbering_4", CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/UI/Player_UI/InGame_PlayerUI/4.png", 1)), E_FAIL);
	FAILED_CHECK_RETURN(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Texture_Numbering_5", CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/UI/Player_UI/InGame_PlayerUI/5.png", 1)), E_FAIL);
	FAILED_CHECK_RETURN(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Texture_Numbering_6", CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/UI/Player_UI/InGame_PlayerUI/6.png", 1)), E_FAIL);
	FAILED_CHECK_RETURN(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Texture_Numbering_7", CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/UI/Player_UI/InGame_PlayerUI/7.png", 1)), E_FAIL);
	FAILED_CHECK_RETURN(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Texture_Numbering_8", CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/UI/Player_UI/InGame_PlayerUI/8.png", 1)), E_FAIL);
	FAILED_CHECK_RETURN(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Texture_Numbering_9", CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/UI/Player_UI/InGame_PlayerUI/9.png", 1)), E_FAIL);
	FAILED_CHECK_RETURN(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Texture_Numbering_Slash", CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/UI/Player_UI/InGame_PlayerUI/-.png", 1)), E_FAIL);

	// 골드 텍스쳐
	FAILED_CHECK_RETURN(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Texture_Gold", CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/UI/Player_UI/InGame_PlayerUI/icon_warcash.png", 1)), E_FAIL);

	// 에메랄드 텍스쳐
	FAILED_CHECK_RETURN(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Texture_Emerald", CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/UI/Player_UI/InGame_PlayerUI/icon_cash.png", 1)), E_FAIL);

	// 빨간 총알 텍스쳐
	FAILED_CHECK_RETURN(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Texture_Bullet", CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/Bullet/bullet_red.png", 1)), E_FAIL);


	// 몬스터 체력 베이스 텍스쳐
	FAILED_CHECK_RETURN(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_MonsterUI_Base", CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/UI/Monster_UI/Monster_HP/bg_monsterHP_normal.png", 1)), E_FAIL);

	// 몬스터 노말 텍스쳐
	FAILED_CHECK_RETURN(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Monster_NormalTex", CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Meshes/Monster/Normal_Human_Sword/monster_body_2001_s.png", 1)), E_FAIL);

	// 몬스터 아웃라인 글로우 텍스쳐 
	FAILED_CHECK_RETURN(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Monster_GlowTex", CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Meshes/Monster/Normal_Human_Sword/monster_body_2001_Glow.png", 1)), E_FAIL);
	FAILED_CHECK_RETURN(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Monster_GlowTex_Test", CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Meshes/Monster/Normal_Human_Sword/Glow_Test.png", 1)), E_FAIL);













	m_wstrLoadingText = L"네비게이션 정보 생성중입니다.";
	FAILED_CHECK_RETURN(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Navigation", CNavigation::Create(m_pDevice, m_pContext, L"C:\\Users\\Jihoon\\Documents\\Visual Studio 2015\\Projects\\Framework_JH\\Client_JH\\Bin\\Save Data\\Navigation\\Navigation_Test.json")), E_FAIL);
	// FAILED_CHECK_RETURN(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Navigation", CNavigation::Create(m_pDevice, m_pContext, L"C:\\Users\\Hoon\\Desktop\\3D_JH\\Framework_JH\\Client_JH\\Bin\\Save Data\\Navigation\\Navigation_Test.json")), E_FAIL);

	m_wstrLoadingText = L"버퍼를 로딩중입니다.";

	FAILED_CHECK_RETURN(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_VIBuffer_Cube", CVIBuffer_Cube::Create(m_pDevice, m_pContext)), E_FAIL);
	FAILED_CHECK_RETURN(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_VIBuffer_Point_Instancing", CVIBuffer_Point_Instancing::Create(m_pDevice, m_pContext, 30)), E_FAIL);
	FAILED_CHECK_RETURN(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_VIBuffer_Bullet_Instancing", CVIBuffer_Point_Instancing::Create(m_pDevice, m_pContext,1)), E_FAIL);

	m_wstrLoadingText = L"콜라이더를 로딩중입니다.";

	FAILED_CHECK_RETURN(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Collider_AABB", CCollider::Create(m_pDevice, m_pContext, CCollider::COLLIDER_AABB)), E_FAIL);
	FAILED_CHECK_RETURN(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Collider_OBB", CCollider::Create(m_pDevice, m_pContext, CCollider::COLLIDER_OBB)), E_FAIL);
	FAILED_CHECK_RETURN(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Collider_SPHERE", CCollider::Create(m_pDevice, m_pContext, CCollider::COLLIDER_SPHERE)), E_FAIL);
	FAILED_CHECK_RETURN(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Collider_Muzzle_SPHERE", CCollider::Create(m_pDevice, m_pContext, CCollider::COLLIDER_MUZZLE)), E_FAIL);


	m_wstrLoadingText = L"모델을 로딩중입니다. ";

	_matrix PivotMatrix = XMMatrixIdentity();
	PivotMatrix = XMMatrixRotationY(XMConvertToRadians(180.f));

	PivotMatrix = XMMatrixScaling(0.009f, 0.009f, 0.009f) * XMMatrixRotationY(XMConvertToRadians(180.f)) *XMMatrixRotationX(XMConvertToRadians(90.f));
	FAILED_CHECK_RETURN(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Model_Home", CModel::Create(m_pDevice, m_pContext, CModel::MODEL_NONANIM, "../Bin/Resources/Meshes/StartMap/Start_Map.model", PivotMatrix)), E_FAIL);

	PivotMatrix = XMMatrixRotationY(XMConvertToRadians(180.f));
	FAILED_CHECK_RETURN(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Model_LaiHome", CModel::Create(m_pDevice, m_pContext, CModel::MODEL_ANIM, "../Bin/Resources/Meshes/LaiLuo_Home/LaiHome2.fbx", PivotMatrix)), E_FAIL);

	PivotMatrix = XMMatrixRotationY(XMConvertToRadians(180.f));
	FAILED_CHECK_RETURN(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Model_Default_Pistol", CModel::Create(m_pDevice, m_pContext, CModel::MODEL_ANIM, "../Bin/Resources/Meshes/Weapon/Default_Pistol/Default_Pistol_Normal.model", PivotMatrix)), E_FAIL);
	FAILED_CHECK_RETURN(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Model_Flame_Bullet", CModel::Create(m_pDevice, m_pContext, CModel::MODEL_ANIM, "../Bin/Resources/Meshes/Weapon/Flame_Bullet/Flame_Bullet_Normal.model", PivotMatrix)), E_FAIL);
	FAILED_CHECK_RETURN(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Model_Fire_Dragon", CModel::Create(m_pDevice, m_pContext, CModel::MODEL_ANIM, "../Bin/Resources/Meshes/Weapon/Fire_Dragon/Fire_Dragon_Normal.model", PivotMatrix)), E_FAIL);
	FAILED_CHECK_RETURN(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Model_Poison", CModel::Create(m_pDevice, m_pContext, CModel::MODEL_ANIM, "../Bin/Resources/Meshes/Weapon/Poison/Poison_Normal.model", PivotMatrix)), E_FAIL);

	// 노말맵 몬스터 모델
	FAILED_CHECK_RETURN(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Model_Human_Sword", CModel::Create(m_pDevice, m_pContext, CModel::MODEL_ANIM, "../Bin/Resources/Meshes/Monster/Normal_Human_Sword/Normal_Human_Sword.model", PivotMatrix)), E_FAIL);

	m_wstrLoadingText = L"셰이더를 로딩중입니다. ";

	FAILED_CHECK_RETURN(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Shader_VtxNorTex", CShader::Create(m_pDevice, m_pContext, L"../Bin/ShaderFiles/Shader_VtxNorTex.hlsl", CShader::DECLARATION_VTXNORTEX, VTXNORTEX_DECLARATION::Elements, VTXNORTEX_DECLARATION::iNumElements)), E_FAIL);
	FAILED_CHECK_RETURN(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Shader_VtxNonAnim", CShader::Create(m_pDevice, m_pContext, L"../Bin/ShaderFiles/Shader_VtxNonAnim.hlsl", CShader::DECLARATION_VTXMODEL, VTXMODEL_DECLARATION::Elements, VTXMODEL_DECLARATION::iNumElements)), E_FAIL);
	FAILED_CHECK_RETURN(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Shader_VtxAnimModel", CShader::Create(m_pDevice, m_pContext, L"../Bin/ShaderFiles/Shader_VtxAnimModel.hlsl", CShader::DECLARATION_VTXANIMMODEL, VTXANIMMODEL_DECLARATION::Elements, VTXANIMMODEL_DECLARATION::iNumElements)), E_FAIL);
	FAILED_CHECK_RETURN(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Shader_VtxCubeTex", CShader::Create(m_pDevice, m_pContext, L"../Bin/ShaderFiles/Shader_VtxCubeTex.hlsl", CShader::DECLARATION_VTXCUBE, VTXCUBETEX_DECLARATION::Elements, VTXCUBETEX_DECLARATION::iNumElements)), E_FAIL);
	FAILED_CHECK_RETURN(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Shader_VtxPointInstance", CShader::Create(m_pDevice, m_pContext, L"../Bin/ShaderFiles/Shader_VtxPointInstance.hlsl",CShader::DECLARATION_VTXPOINTINSTANCE, VTXPOINTINSTANCE_DECLARATION::Elements, VTXPOINTINSTANCE_DECLARATION::iNumElements)), E_FAIL);


	m_wstrLoadingText = L"객체원형을 생성중입니다. ";

	FAILED_CHECK_RETURN(pGameInstance->Add_Prototype(L"Prototype_GameObject_Terrain", CTerrain::Create(m_pDevice, m_pContext)), E_FAIL);
	FAILED_CHECK_RETURN(pGameInstance->Add_Prototype(L"Prototype_GameObject_Player", CPlayer::Create(m_pDevice, m_pContext)), E_FAIL);
	FAILED_CHECK_RETURN(pGameInstance->Add_Prototype(L"Prototype_GameObject_ForkLift", CForkLift::Create(m_pDevice, m_pContext)), E_FAIL);
	FAILED_CHECK_RETURN(pGameInstance->Add_Prototype(L"Prototype_GameObject_Home", CHome::Create(m_pDevice, m_pContext)), E_FAIL);
	FAILED_CHECK_RETURN(pGameInstance->Add_Prototype(L"Prototype_GameObject_LaiHome", CLaiLuo_Home::Create(m_pDevice, m_pContext)), E_FAIL);
	FAILED_CHECK_RETURN(pGameInstance->Add_Prototype(L"Prototype_GameObject_Weapon",CWeapon::Create(m_pDevice, m_pContext)), E_FAIL);
	FAILED_CHECK_RETURN(pGameInstance->Add_Prototype(L"Prototype_GameObject_Sky", CSky::Create(m_pDevice, m_pContext)), E_FAIL);
	FAILED_CHECK_RETURN(pGameInstance->Add_Prototype(L"Prototype_GameObject_Effect_Point_Instancing", CEffect_Point_Instancing::Create(m_pDevice, m_pContext)), E_FAIL);
	FAILED_CHECK_RETURN(pGameInstance->Add_Prototype(L"Prototype_GameObject_PlayerUI_Base", CPlayerUI_Base::Create(m_pDevice, m_pContext)), E_FAIL);
	FAILED_CHECK_RETURN(pGameInstance->Add_Prototype(L"Prototype_GameObject_PlayerUI_Skill", CPlayerUI_Skill::Create(m_pDevice, m_pContext)), E_FAIL);
	FAILED_CHECK_RETURN(pGameInstance->Add_Prototype(L"Prototype_GameObject_PlayerUI_Throw", CPlayer_UI_Throw::Create(m_pDevice, m_pContext)), E_FAIL);
	FAILED_CHECK_RETURN(pGameInstance->Add_Prototype(L"Prototype_GameObject_PlayerUI_Dash", CPlayer_UI_Dash::Create(m_pDevice, m_pContext)), E_FAIL);
	FAILED_CHECK_RETURN(pGameInstance->Add_Prototype(L"Prototype_GameObject_PlayerUI_Weapon", CPlayer_UI_Weapon::Create(m_pDevice, m_pContext)), E_FAIL);
	FAILED_CHECK_RETURN(pGameInstance->Add_Prototype(L"Prototype_GameObject_PlayerUI_Number_1", CPlayer_UI_Weapon_Number::Create(m_pDevice, m_pContext)), E_FAIL);
	FAILED_CHECK_RETURN(pGameInstance->Add_Prototype(L"Prototype_GameObject_PlayerUI_WeaponPic", CPlayer_UI_Weapon_Pic::Create(m_pDevice, m_pContext)), E_FAIL);
	FAILED_CHECK_RETURN(pGameInstance->Add_Prototype(L"Prototype_GameObject_PlayerUI_BulletPic", CPlayer_UI_BulletType::Create(m_pDevice, m_pContext)), E_FAIL);
	FAILED_CHECK_RETURN(pGameInstance->Add_Prototype(L"Prototype_GameObject_PlayerUI_BulletCnt", CPlayer_UI_CountMachine::Create(m_pDevice, m_pContext)), E_FAIL);
	FAILED_CHECK_RETURN(pGameInstance->Add_Prototype(L"Prototype_GameObject_PlayerUI_GoldCnt", CPlayer_UI_CountMachine::Create(m_pDevice, m_pContext)), E_FAIL);
	FAILED_CHECK_RETURN(pGameInstance->Add_Prototype(L"Prototype_GameObject_PlayerUI_ThrowCnt", CPlayer_UI_CountMachine::Create(m_pDevice, m_pContext)), E_FAIL);
	FAILED_CHECK_RETURN(pGameInstance->Add_Prototype(L"Prototype_GameObject_PlayerUI_EmeraldCnt", CPlayer_UI_CountMachine::Create(m_pDevice, m_pContext)), E_FAIL);
	FAILED_CHECK_RETURN(pGameInstance->Add_Prototype(L"Prototype_GameObject_Player_NumberUI", CNumberUI::Create(m_pDevice, m_pContext)), E_FAIL);
	FAILED_CHECK_RETURN(pGameInstance->Add_Prototype(L"Prototype_GameObject_Player_GoldTex", CPlayerUI_Gold::Create(m_pDevice, m_pContext)), E_FAIL);
	FAILED_CHECK_RETURN(pGameInstance->Add_Prototype(L"Prototype_GameObject_Player_EmeraldTex", CPlayerUI_Emerald::Create(m_pDevice, m_pContext)), E_FAIL);
	FAILED_CHECK_RETURN(pGameInstance->Add_Prototype(L"Prototype_GameObject_Player_Skill_BaseTex", CPlayerUI_Skill_Base::Create(m_pDevice, m_pContext)), E_FAIL);
	FAILED_CHECK_RETURN(pGameInstance->Add_Prototype(L"Prototype_GameObject_MonsterUI_Base", CMonsterUI::Create(m_pDevice, m_pContext)), E_FAIL);

	// 노말맵 몬스터 
	FAILED_CHECK_RETURN(pGameInstance->Add_Prototype(L"Prototype_GameObject_Normal_Human_Sword", CHuman_Sword::Create(m_pDevice, m_pContext)), E_FAIL);

	// Bullet
	FAILED_CHECK_RETURN(pGameInstance->Add_Prototype(L"Prototype_GameObject_Player_Default_PistolTex", CDefault_Pistol::Create(m_pDevice, m_pContext)), E_FAIL);

	m_wstrLoadingText = L"로딩끝. ";

	m_bIsLoadingFinished = true;

	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CLoader::Loading_For_MapTool()
{
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
