#include "stdafx.h"
#include "..\public\Loader.h"

#include "Arrow.h"
#include "GameInstance.h"
#include "BackGround.h"
#include "Blade.h"
#include "Boom.h"
#include "DangerRing.h"
#include "Default_Pistol.h"
#include "Effect_Point_Instancing.h"
#include "ForkLift.h"
#include "Home.h"
#include "Human_Bow.h"
#include "Human_Explode.h"
#include "Human_Granade.h"
#include "Human_Spear.h"
#include "Human_Sword.h"
#include "Elite_Bug.h"
#include "Elite_Knight.h"
#include "FadeInOut.h"
#include "LaiLuo_Home.h"
#include "Laser.h"
#include "LaserBullet.h"
#include "Little_Bug.h"
#include "MagicStone.h"
#include "Monster.h"
#include "MonsterUI.h"
#include "NormalMap.h"
#include "Normal_Boss.h"
#include "Normal_BossMap.h"
#include "NumberUI.h"
#include "Player.h"
#include "PlayerUI_Base.h"
#include "RocketArm.h"
#include "Terrain.h"
#include "Weapon.h"
#include "Sky.h"
#include "StoneLight.h"
#include "StonePillar.h"

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
	FAILED_CHECK_RETURN(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Player_Arm_s", CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Meshes/Weapon/Default_Pistol/hero_fpp_104_A_s.png", 1)), E_FAIL);
	FAILED_CHECK_RETURN(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Player_Default_Pistol_s", CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Meshes/Weapon/Default_Pistol/Test.png", 1)), E_FAIL);

	// 플레이어 스페큘러 텍스쳐
	// FAILED_CHECK_RETURN(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Texture_UI_PlayerInfo", CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/UI/Player_UI/InGame_PlayerUI/PlayerInfo.png", 1)), E_FAIL);


	//플레이어 배경
	FAILED_CHECK_RETURN(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Texture_UI_PlayerInfo", CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/UI/Player_UI/InGame_PlayerUI/PlayerInfo_Base.png", 1)), E_FAIL);
	FAILED_CHECK_RETURN(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Texture_UI_PlayerHp", CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/UI/Player_UI/InGame_PlayerUI/PlayerInfo_HP.png", 1)), E_FAIL);
	FAILED_CHECK_RETURN(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Texture_UI_PlayerShield", CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/UI/Player_UI/InGame_PlayerUI/PlayerInfo_Shield.png", 1)), E_FAIL);

	//플레이어 체력
	// FAILED_CHECK_RETURN(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Texture_UI_PlayerHp_BaseGrey", CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/UI/Player_UI/InGame_PlayerUI/Bar_Character_HPBar_Grey.png", 1)), E_FAIL);
	// FAILED_CHECK_RETURN(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Texture_UI_PlayerHp_White", CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/UI/Player_UI/InGame_PlayerUI/Bar_Character_HPBar_White.png", 1)), E_FAIL);
	// FAILED_CHECK_RETURN(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Texture_UI_PlayerHp", CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/UI/Player_UI/InGame_PlayerUI/Bar_Character_HPBar_2.png", 1)), E_FAIL);

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

	// 레이저 텍스쳐
	FAILED_CHECK_RETURN(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Texture_Laser", CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Meshes/Monster/3909_NormalBoss/rules_green.png", 1)), E_FAIL);


	// 몬스터 체력 베이스 텍스쳐
	FAILED_CHECK_RETURN(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_MonsterUI_Base", CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/UI/Monster_UI/Monster_HP/HP_Base.png", 1)), E_FAIL);
	FAILED_CHECK_RETURN(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_MonsterUI_Base_Elite", CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/UI/Monster_UI/Monster_HP/HP_Base_Elite.png", 1)), E_FAIL);

	FAILED_CHECK_RETURN(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_MonsterUI_HP_Red", CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/UI/Monster_UI/Monster_HP/HP_RED_AlphaTest.png", 1)), E_FAIL);
	FAILED_CHECK_RETURN(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_MonsterUI_HP_White", CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/UI/Monster_UI/Monster_HP/HP_White_AlphaTest.png", 1)), E_FAIL);
	FAILED_CHECK_RETURN(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_MonsterUI_Shield_Blue", CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/UI/Monster_UI/Monster_HP/Bar_Character_SFBar_1.png", 1)), E_FAIL);
	FAILED_CHECK_RETURN(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_MonsterUI_Shield_White", CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/UI/Monster_UI/Monster_HP/Bar_Character_SFBar_White.png", 1)), E_FAIL);
	// 보스 텍스쳐
	FAILED_CHECK_RETURN(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_BossUI_Base", CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/UI/Monster_UI/Monster_HP/Boss_Base.png", 1)), E_FAIL);
	FAILED_CHECK_RETURN(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_BossUI_Red", CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/UI/Monster_UI/Monster_HP/Boss_Hp.png", 1)), E_FAIL);
	FAILED_CHECK_RETURN(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_BossUI_White", CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/UI/Monster_UI/Monster_HP/BOSS_WHITE.png", 1)), E_FAIL);

	//페이드 텍스쳐
	FAILED_CHECK_RETURN(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Texture_Fade_In_Out", CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/UI/Monster_UI/Monster_HP/FadeInOut.png", 1)), E_FAIL);


	// 몬스터 노말 텍스쳐
	FAILED_CHECK_RETURN(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Monster_NormalTex", CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Meshes/Monster/Normal_Human_Sword/monster_body_2001_s.png", 1)), E_FAIL);
	FAILED_CHECK_RETURN(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Elite_Bug_NormalTex", CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Meshes/Monster/Normal_Elite_Bug/monster_body_2001_n #1777379.png", 1)), E_FAIL);
	FAILED_CHECK_RETURN(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Elite_Knight_NormalTex", CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Meshes/Monster/Normal_Elite_Knight/monster_body_3123_n.png", 1)), E_FAIL);
	FAILED_CHECK_RETURN(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Human_Tall_Bow_NormalTex", CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Meshes/Monster/Normal_Tall_Human_Bow/monster_body_2161_01_n.png", 1)), E_FAIL);
	FAILED_CHECK_RETURN(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Bow_NormalTex", CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Meshes/Monster/Normal_Tall_Human_Bow/monster_body_2161_02_n.png", 1)), E_FAIL);

	// 몬스터 아웃라인 글로우 텍스쳐 
	FAILED_CHECK_RETURN(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Sword_NormalTex", CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Meshes/Monster/Normal_Human_Sword/monster_body_2001_s.png", 1)), E_FAIL);


	FAILED_CHECK_RETURN(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Danger_Ring", CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Meshes/Monster/3909_NormalBoss/ring.png", 1)), E_FAIL);











	m_wstrLoadingText = L"네비게이션 정보 생성중입니다.";
	// FAILED_CHECK_RETURN(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Navigation", CNavigation::Create(m_pDevice, m_pContext, L"C:\\Users\\Jihoon\\Documents\\Visual Studio 2015\\Projects\\Framework_JH\\Client_JH\\Bin\\Save Data\\Navigation\\Navigation_Test.json")), E_FAIL);
	// FAILED_CHECK_RETURN(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Navigation", CNavigation::Create(m_pDevice, m_pContext, L"C:\\Users\\Hoon\\Desktop\\3D_JH\\Framework_JH\\Client_JH\\Bin\\Save Data\\Navigation\\Navigation_Test.json")), E_FAIL);
	FAILED_CHECK_RETURN(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Navigation", CNavigation::Create(m_pDevice, m_pContext, L"../Bin/Save Data/Navigation/Navigation_With3909_Final.json")), E_FAIL);
	// FAILED_CHECK_RETURN(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Navigation_Boss", CNavigation::Create(m_pDevice, m_pContext, L"../Bin/Save Data/Navigation/3909NaviData_Fin.json")), E_FAIL);

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
	// PivotMatrix = XMMatrixRotationY(XMConvertToRadians(180.f));

	// PivotMatrix = XMMatrixScaling(0.009f, 0.009f, 0.009f) * XMMatrixRotationY(XMConvertToRadians(180.f)) *XMMatrixRotationX(XMConvertToRadians(90.f));
	// FAILED_CHECK_RETURN(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Model_Home", CModel::Create(m_pDevice, m_pContext, CModel::MODEL_NONANIM, "../Bin/Resources/Meshes/StartMap/StartMap_Normal.model", PivotMatrix)), E_FAIL);
	FAILED_CHECK_RETURN(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Model_NormalMap", CModel::Create(m_pDevice, m_pContext, CModel::MODEL_NONANIM, "../Bin/Resources/Meshes/StartMap/Remove_CTex_Start0.model", PivotMatrix)), E_FAIL);


	// PivotMatrix = XMMatrixScaling(0.0045f, 0.0045f, 0.0045f) * XMMatrixRotationY(XMConvertToRadians(45.f)) *XMMatrixRotationX(XMConvertToRadians(90.f));
	// PivotMatrix.r[3] += XMVectorSet(82.f, 0.f, -30.f, 117.f);

	FAILED_CHECK_RETURN(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Model_Normal_BossMap", CModel::Create(m_pDevice, m_pContext, CModel::MODEL_NONANIM, "../Bin/Resources/Meshes/Normal_3909Area/Remove_CTex_3909.model", PivotMatrix)), E_FAIL);

	PivotMatrix = XMMatrixRotationY(XMConvertToRadians(180.f));
	FAILED_CHECK_RETURN(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Model_LaiHome", CModel::Create(m_pDevice, m_pContext, CModel::MODEL_ANIM, "../Bin/Resources/Meshes/LaiLuo_Home/LaiHome2.fbx", PivotMatrix)), E_FAIL);

	PivotMatrix = XMMatrixRotationY(XMConvertToRadians(180.f));
	FAILED_CHECK_RETURN(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Model_Default_Pistol", CModel::Create(m_pDevice, m_pContext, CModel::MODEL_ANIM, "../Bin/Resources/Meshes/Weapon/Default_Pistol/Default_Pistol_Alpha.model", PivotMatrix)), E_FAIL);
	FAILED_CHECK_RETURN(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Model_Flame_Bullet", CModel::Create(m_pDevice, m_pContext, CModel::MODEL_ANIM, "../Bin/Resources/Meshes/Weapon/Flame_Bullet/Flame_Bullet_Spec.model", PivotMatrix)), E_FAIL);
	FAILED_CHECK_RETURN(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Model_Fire_Dragon", CModel::Create(m_pDevice, m_pContext, CModel::MODEL_ANIM, "../Bin/Resources/Meshes/Weapon/Fire_Dragon/Fire_Dragon_Alpha0.model", PivotMatrix)), E_FAIL);
	FAILED_CHECK_RETURN(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Model_Poison", CModel::Create(m_pDevice, m_pContext, CModel::MODEL_ANIM, "../Bin/Resources/Meshes/Weapon/Poison/Poison_Normal.model", PivotMatrix)), E_FAIL);

	// 노말맵 몬스터 모델
	FAILED_CHECK_RETURN(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Model_Human_Sword", CModel::Create(m_pDevice, m_pContext, CModel::MODEL_ANIM, "../Bin/Resources/Meshes/Monster/Normal_Human_Sword/Normal_Human_Sword.model", PivotMatrix)), E_FAIL);
	FAILED_CHECK_RETURN(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Model_Human_Spear", CModel::Create(m_pDevice, m_pContext, CModel::MODEL_ANIM, "../Bin/Resources/Meshes/Monster/Normal_Human_Spear/Normal_Human_Spear.model", PivotMatrix)), E_FAIL);
	FAILED_CHECK_RETURN(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Model_Human_Granade", CModel::Create(m_pDevice, m_pContext, CModel::MODEL_ANIM, "../Bin/Resources/Meshes/Monster/Normal_Human_Granade/Normal_Granade.model", PivotMatrix)), E_FAIL);
	FAILED_CHECK_RETURN(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Model_Human_Bow", CModel::Create(m_pDevice, m_pContext, CModel::MODEL_ANIM, "../Bin/Resources/Meshes/Monster/Normal_Tall_Human_Bow/Normal_Tall_Human_Bow.model", PivotMatrix)), E_FAIL);
	FAILED_CHECK_RETURN(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Model_Human_Explode", CModel::Create(m_pDevice, m_pContext, CModel::MODEL_ANIM, "../Bin/Resources/Meshes/Monster/Normal_Explode_Fire/Normal_Explode_Fire.model", PivotMatrix)), E_FAIL);
	FAILED_CHECK_RETURN(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Model_Elite_Knight", CModel::Create(m_pDevice, m_pContext, CModel::MODEL_ANIM, "../Bin/Resources/Meshes/Monster/Normal_Elite_Knight/Normal_Elite_Knight.model", PivotMatrix)), E_FAIL);
	FAILED_CHECK_RETURN(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Model_Elite_Bug", CModel::Create(m_pDevice, m_pContext, CModel::MODEL_ANIM, "../Bin/Resources/Meshes/Monster/Normal_Elite_Bug/Normal_Elite_Bug.model", PivotMatrix)), E_FAIL);
	FAILED_CHECK_RETURN(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Model_Little_Bug", CModel::Create(m_pDevice, m_pContext, CModel::MODEL_ANIM, "../Bin/Resources/Meshes/Monster/Normal_Elite_Bug/Little_Bug/Little_Bug.model", PivotMatrix)), E_FAIL);
	// 노말맵 보스
	FAILED_CHECK_RETURN(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Model_Normal_Boss", CModel::Create(m_pDevice, m_pContext, CModel::MODEL_ANIM, "../Bin/Resources/Meshes/Monster/3909_NormalBoss/3909_Fix_Test.model", PivotMatrix)), E_FAIL);

	// ~노말맵 몬스터 모델

	// 총알류
	FAILED_CHECK_RETURN(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Model_Arrow", CModel::Create(m_pDevice, m_pContext, CModel::MODEL_NONANIM, "../Bin/Resources/Meshes/Monster/Normal_Tall_Human_Bow/Arrow.model", PivotMatrix)), E_FAIL);
	FAILED_CHECK_RETURN(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Model_Boom", CModel::Create(m_pDevice, m_pContext, CModel::MODEL_NONANIM, "../Bin/Resources/Meshes/Monster/Normal_Human_Granade/Boom.model", PivotMatrix)), E_FAIL);
	FAILED_CHECK_RETURN(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Model_Blade", CModel::Create(m_pDevice, m_pContext, CModel::MODEL_NONANIM, "../Bin/Resources/Meshes/Monster/Normal_Elite_Knight/Bullet/Knight_Bullet_Fix1.model", PivotMatrix)), E_FAIL);

	// 운석
	FAILED_CHECK_RETURN(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Model_MagicStone_A", CModel::Create(m_pDevice, m_pContext, CModel::MODEL_NONANIM, "../Bin/Resources/Meshes/Monster/3909_NormalBoss/MagicStone/MagicStone_A.model", PivotMatrix)), E_FAIL);
	FAILED_CHECK_RETURN(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Model_MagicStone_B", CModel::Create(m_pDevice, m_pContext, CModel::MODEL_NONANIM, "../Bin/Resources/Meshes/Monster/3909_NormalBoss/MagicStone/MagicStone_B.model", PivotMatrix)), E_FAIL);
	FAILED_CHECK_RETURN(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Model_MagicStone_C", CModel::Create(m_pDevice, m_pContext, CModel::MODEL_NONANIM, "../Bin/Resources/Meshes/Monster/3909_NormalBoss/MagicStone/MagicStone_C.model", PivotMatrix)), E_FAIL);
	FAILED_CHECK_RETURN(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Model_MagicStone_D", CModel::Create(m_pDevice, m_pContext, CModel::MODEL_NONANIM, "../Bin/Resources/Meshes/Monster/3909_NormalBoss/MagicStone/MagicStone_D.model", PivotMatrix)), E_FAIL);

	// 돌기둥
	FAILED_CHECK_RETURN(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Model_StonePillar", CModel::Create(m_pDevice, m_pContext, CModel::MODEL_NONANIM, "../Bin/Resources/Meshes/Monster/3909_NormalBoss/StonePillar/StonePillar_1.model", PivotMatrix)), E_FAIL);
	// 기둥 라이트
	FAILED_CHECK_RETURN(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Model_StonePillar_Light", CModel::Create(m_pDevice, m_pContext, CModel::MODEL_NONANIM, "../Bin/Resources/Meshes/Monster/3909_NormalBoss/StoneLight.model", PivotMatrix)), E_FAIL);

	// 보스 주먹 
		FAILED_CHECK_RETURN(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Model_LeftArm", CModel::Create(m_pDevice, m_pContext, CModel::MODEL_NONANIM, "../Bin/Resources/Meshes/Monster/3909_NormalBoss/LeftHand_Fix.model", PivotMatrix)), E_FAIL);
	FAILED_CHECK_RETURN(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Model_RightArm", CModel::Create(m_pDevice, m_pContext, CModel::MODEL_NONANIM, "../Bin/Resources/Meshes/Monster/3909_NormalBoss/RightHand_Fix.model", PivotMatrix)), E_FAIL);


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
	// FAILED_CHECK_RETURN(pGameInstance->Add_Prototype(L"Prototype_GameObject_Home", CHome::Create(m_pDevice, m_pContext)), E_FAIL);
	FAILED_CHECK_RETURN(pGameInstance->Add_Prototype(L"Prototype_GameObject_NormalMap", CNormalMap::Create(m_pDevice, m_pContext)), E_FAIL);
	FAILED_CHECK_RETURN(pGameInstance->Add_Prototype(L"Prototype_GameObject_Normal_BossMap", CNormal_BossMap::Create(m_pDevice, m_pContext)), E_FAIL);

	FAILED_CHECK_RETURN(pGameInstance->Add_Prototype(L"Prototype_GameObject_LaiHome", CLaiLuo_Home::Create(m_pDevice, m_pContext)), E_FAIL);
	FAILED_CHECK_RETURN(pGameInstance->Add_Prototype(L"Prototype_GameObject_Weapon",CWeapon::Create(m_pDevice, m_pContext)), E_FAIL);
	FAILED_CHECK_RETURN(pGameInstance->Add_Prototype(L"Prototype_GameObject_Sky", CSky::Create(m_pDevice, m_pContext)), E_FAIL);
	FAILED_CHECK_RETURN(pGameInstance->Add_Prototype(L"Prototype_GameObject_Effect_Point_Instancing", CEffect_Point_Instancing::Create(m_pDevice, m_pContext)), E_FAIL);
	FAILED_CHECK_RETURN(pGameInstance->Add_Prototype(L"Prototype_GameObject_PlayerUI_Base", CPlayerUI_Base::Create(m_pDevice, m_pContext)), E_FAIL);
	FAILED_CHECK_RETURN(pGameInstance->Add_Prototype(L"Prototype_GameObject_PlayerUI_Hp", CPlayerUI_Hp_Red::Create(m_pDevice, m_pContext)), E_FAIL);
	FAILED_CHECK_RETURN(pGameInstance->Add_Prototype(L"Prototype_GameObject_PlayerUI_Shield", CPlayerUI_Hp_Shield::Create(m_pDevice, m_pContext)), E_FAIL);




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
	FAILED_CHECK_RETURN(pGameInstance->Add_Prototype(L"Prototype_GameObject_MonsterUI_Base_Elite", CMonsterUI_Elite::Create(m_pDevice, m_pContext)), E_FAIL);

	FAILED_CHECK_RETURN(pGameInstance->Add_Prototype(L"Prototype_GameObject_MonsterUI_HP_Red", CMonsterUI_HP_Red::Create(m_pDevice, m_pContext)), E_FAIL);
	FAILED_CHECK_RETURN(pGameInstance->Add_Prototype(L"Prototype_GameObject_MonsterUI_HP_White", CMonsterUI_HP_White::Create(m_pDevice, m_pContext)), E_FAIL);

	FAILED_CHECK_RETURN(pGameInstance->Add_Prototype(L"Prototype_GameObject_BossUI_Base", CBoss_UI_Base::Create(m_pDevice, m_pContext)), E_FAIL);
	FAILED_CHECK_RETURN(pGameInstance->Add_Prototype(L"Prototype_GameObject_BossUI_Red", CBoss_UI_Red::Create(m_pDevice, m_pContext)), E_FAIL);
	FAILED_CHECK_RETURN(pGameInstance->Add_Prototype(L"Prototype_GameObject_BossUI_White", CBoss_UI_White::Create(m_pDevice, m_pContext)), E_FAIL);


	// 노말맵 몬스터 
	FAILED_CHECK_RETURN(pGameInstance->Add_Prototype(L"Prototype_GameObject_Normal_Human_Sword", CHuman_Sword::Create(m_pDevice, m_pContext)), E_FAIL);
	FAILED_CHECK_RETURN(pGameInstance->Add_Prototype(L"Prototype_GameObject_Normal_Human_Spear", CHuman_Spear::Create(m_pDevice, m_pContext)), E_FAIL);
	FAILED_CHECK_RETURN(pGameInstance->Add_Prototype(L"Prototype_GameObject_Normal_Human_Bow", CHuman_Bow::Create(m_pDevice, m_pContext)), E_FAIL);
	FAILED_CHECK_RETURN(pGameInstance->Add_Prototype(L"Prototype_GameObject_Normal_Human_Granade", CHuman_Granade::Create(m_pDevice, m_pContext)), E_FAIL);
	FAILED_CHECK_RETURN(pGameInstance->Add_Prototype(L"Prototype_GameObject_Normal_Human_Explode", CHuman_Explode::Create(m_pDevice, m_pContext)), E_FAIL);
	FAILED_CHECK_RETURN(pGameInstance->Add_Prototype(L"Prototype_GameObject_Normal_Elite_Bug", CElite_Bug::Create(m_pDevice, m_pContext)), E_FAIL);
	FAILED_CHECK_RETURN(pGameInstance->Add_Prototype(L"Prototype_GameObject_Normal_Little_Bug", CLittle_Bug::Create(m_pDevice, m_pContext)), E_FAIL);
	FAILED_CHECK_RETURN(pGameInstance->Add_Prototype(L"Prototype_GameObject_Normal_Elite_Knight", CElite_Knight::Create(m_pDevice, m_pContext)), E_FAIL);

	// 노말맵 보스
	FAILED_CHECK_RETURN(pGameInstance->Add_Prototype(L"Prototype_GameObject_Normal_Boss", CNormal_Boss::Create(m_pDevice, m_pContext)), E_FAIL);

	// FadeInOut
	FAILED_CHECK_RETURN(pGameInstance->Add_Prototype(L"Prototype_GameObject_Fade_In_Out", CFadeInOut::Create(m_pDevice, m_pContext)), E_FAIL);
	// Ring
	FAILED_CHECK_RETURN(pGameInstance->Add_Prototype(L"Prototype_GameObject_Danger_Ring", CDangerRing::Create(m_pDevice, m_pContext)), E_FAIL);

	// 노말맵 총알 게임 오브젝트
	FAILED_CHECK_RETURN(pGameInstance->Add_Prototype(L"Prototype_GameObject_Normal_Human_Bow_Arrow", CArrow::Create(m_pDevice, m_pContext)), E_FAIL);
	FAILED_CHECK_RETURN(pGameInstance->Add_Prototype(L"Prototype_GameObject_Normal_Human_Granade_Boom", CBoom::Create(m_pDevice, m_pContext)), E_FAIL);
	FAILED_CHECK_RETURN(pGameInstance->Add_Prototype(L"Prototype_GameObject_Normal_Elite_Knight_Blade", CBlade::Create(m_pDevice, m_pContext)), E_FAIL);
	FAILED_CHECK_RETURN(pGameInstance->Add_Prototype(L"Prototype_GameObject_Normal_Boss_Laser", CLaser::Create(m_pDevice, m_pContext)), E_FAIL);
	FAILED_CHECK_RETURN(pGameInstance->Add_Prototype(L"Prototype_GameObject_Normal_Boss_Laser_Bullet", CLaserBullet::Create(m_pDevice, m_pContext)), E_FAIL);
	FAILED_CHECK_RETURN(pGameInstance->Add_Prototype(L"Prototype_GameObject_Normal_Boss_StonePillar", CStonePillar::Create(m_pDevice, m_pContext)), E_FAIL);
	FAILED_CHECK_RETURN(pGameInstance->Add_Prototype(L"Prototype_GameObject_Normal_Boss_MagicStone", CMagicStone::Create(m_pDevice, m_pContext)), E_FAIL);
	FAILED_CHECK_RETURN(pGameInstance->Add_Prototype(L"Prototype_GameObject_Normal_Boss_RocketArm", CRocketArm::Create(m_pDevice, m_pContext)), E_FAIL);


	FAILED_CHECK_RETURN(pGameInstance->Add_Prototype(L"Prototype_GameObject_Normal_Boss_StonePillar_Light", CStoneLight::Create(m_pDevice, m_pContext)), E_FAIL);


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
