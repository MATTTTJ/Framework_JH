#include "stdafx.h"
#include "..\public\Trigger.h"
#include "GameInstance.h"
#include "Monster.h"
#include "Player.h"

CTrigger::CTrigger(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CGameObject(pDevice, pContext)
{
}

CTrigger::CTrigger(const CTrigger& rhs)
	: CGameObject(rhs)
{
	m_pGameInstance = CGameInstance::GetInstance();
}

HRESULT CTrigger::Initialize_Prototype()
{
	m_bHasModel = true;

	FAILED_CHECK_RETURN(__super::Initialize_Prototype(), E_FAIL);

	return S_OK;
}

HRESULT CTrigger::Initialize_Clone(const wstring& wstrPrototypeTag, void* pArg)
{
	ZeroMemory(&m_tTriggerOption, sizeof(TRIGGERDESC));

	if (nullptr != pArg)
	{
		m_tTriggerOption = *(TRIGGERDESC*)pArg;

		if(m_tTriggerOption.m_eType == 0)
		{
			m_iTextureIndex = 0;
		}
		else if(m_tTriggerOption.m_eType == 1)
		{
			m_iTextureIndex = 1;
		}
		else if (m_tTriggerOption.m_eType == 2)
		{
			m_iTextureIndex = 2;
		}
		else if (m_tTriggerOption.m_eType == 3)
		{
			m_iTextureIndex = 3;
		}
	}
	else
	{
		m_tTriggerOption.m_eType = TRIGGERTYPE_END;
	}
	GAMEOBJECTDESC	GameObjectDesc;
	ZeroMemory(&GameObjectDesc, sizeof(GAMEOBJECTDESC));
	GameObjectDesc.TransformDesc.fRotationPerSec = XMConvertToRadians(-20.f);
	FAILED_CHECK_RETURN(__super::Initialize_Clone(wstrPrototypeTag, &GameObjectDesc), E_FAIL);

	FAILED_CHECK_RETURN(SetUp_Components(), E_FAIL);


	return S_OK;
}

void CTrigger::Tick(_double TimeDelta)
{
	__super::Tick(TimeDelta);

	if (m_fTime >= 0.95f)
		m_bReverseTime = true;

	if (m_fTime <= 0.1f)
		m_bReverseTime = false;

	if (m_bReverseTime == true)
		m_fTime -= 0.005f;
	else
		m_fTime += 0.005f;


	m_pTransformCom->Turn(m_pTransformCom->Get_State(CTransform::STATE_LOOK), TimeDelta);

	if(m_tTriggerOption.m_eType == TRIGGER_A_TO_KNIGHT)
	{
		Room_A_To_Knight();
	}
	else if (m_tTriggerOption.m_eType == TRIGGER_KNIGHT_TO_B)
	{
		Knight_To_Room_B();
	}
	else if (m_tTriggerOption.m_eType == TRIGGER_B_TO_BUG)
	{
		Room_B_To_Bug();
	}
	else if (m_tTriggerOption.m_eType == TRIGGER_BUG_TO_C)
	{
		Bug_To_Room_C();
	}
	else if (m_tTriggerOption.m_eType == TRIGGER_C_TO_BOSS)
	{
		Room_C_To_Boss();
	}

	if(m_bSpawnTrigger == true)
	{
		CTrigger* pTrigger = nullptr;
		_matrix PivotMatrix = XMMatrixIdentity();
		CTrigger::TRIGGERDESC TriggerDesc;

		if (m_tTriggerOption.m_eType == TRIGGER_A_TO_KNIGHT)
		{
			// PivotMatrix.r[3] = XMVectorSet(25.232f, 0.02f, 59.929f, 1.f);
			_float	vPos[3], vScale[3], vAngle[3];
			// ImGuizmo::DecomposeMatrixToComponents((_float*)&PivotMatrix, vPos, vAngle, vScale);
			vPos[0] = 25.232f;	vPos[1] = 2.6f; vPos[2] = 59.39f;
			vScale[0] = 4.5f;	vScale[1] = 4.5f;	vScale[2] = 1.0f;
			vAngle[0] = 0.f;	vAngle[1] = -0.f;	vAngle[2] = 0.f;
			ImGuizmo::RecomposeMatrixFromComponents(vPos, vAngle, vScale, (_float*)&PivotMatrix);
			TriggerDesc.m_eType = CTrigger::TRIGGER_KNIGHT_TO_B;
			pTrigger = (CTrigger*)m_pGameInstance->Clone_GameObjectReturnPtr_M(LEVEL_GAMEPLAY, L"Layer_Trigger", L"Prototype_GameObject_Trigger", PivotMatrix, &TriggerDesc);
			pTrigger->Set_Owner(m_pPlayer);
			m_bSpawnTrigger = false;
			Set_Dead(true);
		}
		else if (m_tTriggerOption.m_eType == TRIGGER_KNIGHT_TO_B)
		{
			PivotMatrix.r[3] = XMVectorSet(-31.863f, 0.013f, 61.017f, 1.f);

			_float	vPos[3], vScale[3], vAngle[3];
			// ImGuizmo::DecomposeMatrixToComponents((_float*)&PivotMatrix, vPos, vAngle, vScale);
			vPos[0] = -31.863f;	vPos[1] = 2.6f; vPos[2] = 61.017f;
			vScale[0] = 4.5f;	vScale[1] = 4.5f;	vScale[2] = 1.0f;
			vAngle[0] = 0.f;	vAngle[1] = -0.f;	vAngle[2] = 0.f;
			ImGuizmo::RecomposeMatrixFromComponents(vPos, vAngle, vScale, (_float*)&PivotMatrix);
			TriggerDesc.m_eType = CTrigger::TRIGGER_B_TO_BUG;
			pTrigger = (CTrigger*)m_pGameInstance->Clone_GameObjectReturnPtr_M(LEVEL_GAMEPLAY, L"Layer_Trigger", L"Prototype_GameObject_Trigger", PivotMatrix, &TriggerDesc);
			pTrigger->Set_Owner(m_pPlayer);
			m_bSpawnTrigger = false;
			Set_Dead(true);
		}
		else if (m_tTriggerOption.m_eType == TRIGGER_B_TO_BUG)
		{
			// PivotMatrix.r[3] = XMVectorSet(-60.485f, 2.031f, 110.085f, 1.f);
			_float	vPos[3], vScale[3], vAngle[3];
			// ImGuizmo::DecomposeMatrixToComponents((_float*)&PivotMatrix, vPos, vAngle, vScale);
			vPos[0] = -60.485f;	vPos[1] = 4.6f; vPos[2] = 109.374f;
			vScale[0] = 4.5f;	vScale[1] = 4.5f;	vScale[2] = 1.0f;
			vAngle[0] = 0.f;	vAngle[1] = -0.f;	vAngle[2] = 0.f;
			ImGuizmo::RecomposeMatrixFromComponents(vPos, vAngle, vScale, (_float*)&PivotMatrix);
			TriggerDesc.m_eType = CTrigger::TRIGGER_BUG_TO_C;
			pTrigger = (CTrigger*)m_pGameInstance->Clone_GameObjectReturnPtr_M(LEVEL_GAMEPLAY, L"Layer_Trigger", L"Prototype_GameObject_Trigger", PivotMatrix, &TriggerDesc);
			pTrigger->Set_Owner(m_pPlayer);
			m_bSpawnTrigger = false;
			Set_Dead(true);
		}
		else if (m_tTriggerOption.m_eType == TRIGGER_BUG_TO_C)
		{
			// PivotMatrix.r[3] = XMVectorSet(-26.953f, -1.484f, 102.057f, 1.f);
			_float	vPos[3], vScale[3], vAngle[3];
			// ImGuizmo::DecomposeMatrixToComponents((_float*)&PivotMatrix, vPos, vAngle, vScale);
			vPos[0] = -26.953f;	vPos[1] = 1.2f; vPos[2] = 101.292f;
			vScale[0] = 4.5f;	vScale[1] = 4.5f;	vScale[2] = 1.0f;
			vAngle[0] = 0.f;	vAngle[1] = -0.f;	vAngle[2] = 0.f;
			ImGuizmo::RecomposeMatrixFromComponents(vPos, vAngle, vScale, (_float*)&PivotMatrix);
			TriggerDesc.m_eType = CTrigger::TRIGGER_C_TO_BOSS;
			pTrigger = (CTrigger*)m_pGameInstance->Clone_GameObjectReturnPtr_M(LEVEL_GAMEPLAY, L"Layer_Trigger", L"Prototype_GameObject_Trigger", PivotMatrix, &TriggerDesc);
			pTrigger->Set_Owner(m_pPlayer);
			m_bSpawnTrigger = false;
			Set_Dead(true);
		}
	}

	m_pTriggerCollCom->Update(m_pTransformCom->Get_WorldMatrix());
}

void CTrigger::Late_Tick(_double TimeDelta)
{
	__super::Late_Tick(TimeDelta);
	__super::Compute_CamDistance();

	if (nullptr != m_pRendererCom)
	{
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONLIGHT, this);

#ifdef _DEBUG
		m_pRendererCom->Add_DebugRenderGroup(m_pTriggerCollCom);
#endif
	}
}

HRESULT CTrigger::Render()
{
	FAILED_CHECK_RETURN(__super::Render(), E_FAIL);

	FAILED_CHECK_RETURN(SetUp_ShaderResources(), E_FAIL);

	m_pShaderCom->Begin(12);

	m_pVIBufferCom->Render();

	return S_OK;
}

HRESULT CTrigger::Room_A_To_Knight()
{
	CCollider* pCollider = (CCollider*)m_pPlayer->Find_Component(L"Com_BODYSPHERE");
	NULL_CHECK_RETURN(pCollider, E_FAIL);
	if (m_pTriggerCollCom->Collision(pCollider))
	{
		m_pPlayer->Set_PortalUI(true);

		if(m_pGameInstance->Key_Down(DIK_F))
		{
			m_pGameInstance->Delete_Light(2);
			m_pGameInstance->Delete_Light(3);
			m_pGameInstance->Delete_Light(4);
			m_pGameInstance->Delete_Light(5);
			m_pGameInstance->Delete_Light(6);

			LIGHTDESC			LightDesc;
			CGameObject::GAMEOBJECTDESC	tFireLightDesc;
			ZeroMemory(&LightDesc, sizeof LightDesc);
			// 4 
			_bool& LightBool = m_pGameInstance->Set_LightEnable(4);
			m_pGameInstance->Set_LightPos(4, XMVectorSet(-1.284f, 3.465f, 38.244f, 1.f));

			LightBool = true;
			ZeroMemory(&tFireLightDesc, sizeof(CGameObject::GAMEOBJECTDESC));
			tFireLightDesc.TransformDesc.vInitPos = _float3(-1.284f, 3.71f, 38.244f);
			tFireLightDesc.m_iNumber = 4;
			tFireLightDesc.m_vTexSize = _float2(1.f, 2.f);
			for (_uint i = 0; i<15; ++i)
				FAILED_CHECK_RETURN(m_pGameInstance->Clone_GameObject(LEVEL_GAMEPLAY, L"Layer_FireLight", L"Prototype_GameObject_BlueLight", &tFireLightDesc), E_FAIL);

			


			// Num 5
			_bool& BlueLightBool = m_pGameInstance->Set_LightEnable(5);
			m_pGameInstance->Set_LightPos(5, XMVectorSet(14.999f, 1.211f, 40.403f, 1.f));
			m_pGameInstance->Set_LightRange(5, 10.f);
			BlueLightBool = true;
		
#pragma region TriggerSetting

			m_pPlayer->Set_RoomType(CPlayer::ROOM_KIGHT);
			// _matrix PlayerMat = m_pPlayer->Get_WorldFloat4x4();
			_matrix PivotMat = XMMatrixIdentity();
			_float	vPos[3], vScale[3], vAngle[3];
			vPos[0] = 0.7f;	vPos[1] = 0.36f; vPos[2] = 36.5f;
			vScale[0] = 1.f;	vScale[1] = 1.f;	vScale[2] = 1.0f;
			vAngle[0] = 0.f;	vAngle[1] = 90.f;	vAngle[2] = 0.f;
			ImGuizmo::RecomposeMatrixFromComponents(vPos, vAngle, vScale, (_float*)&PivotMat);
			// PlayerMat.r[3] = XMVectorSet(0.7f, 0.36f, 36.5f, 1.f);
			m_pPlayer->Set_CellIndex(360);
			m_pPlayer->Set_WorldMatrix(PivotMat);

			CMonster::MONSTEROPTION MonsterDesc;
			CMonster* pMonster = nullptr;
			_matrix PivotMatrix = XMMatrixIdentity();
			ZeroMemory(&MonsterDesc, sizeof(CMonster::MONSTEROPTION));
			PivotMatrix.r[3] = XMVectorSet(24.5f, 0.4f, 47.37f, 1.f);
			MonsterDesc.m_bFirstSpawnType[CMonster::STATE_UPSPAWN] = true;
			MonsterDesc.MonsterDesc.m_iHP = MonsterDesc.MonsterDesc.m_iMaxHP = 500;
			MonsterDesc.MonsterDesc.m_iDamage = 15;
			MonsterDesc.MonsterDesc.m_iShield = MonsterDesc.MonsterDesc.m_iMaxShield = 0;
			MonsterDesc.MonsterDesc.TransformDesc.fSpeedPerSec = 2.5f;
			MonsterDesc.MonsterDesc.TransformDesc.fRotationPerSec = XMConvertToRadians(180.f);
			MonsterDesc.m_iCellIndex = 420;
			pMonster = dynamic_cast<CMonster*>(m_pGameInstance->Clone_GameObjectReturnPtr_M(LEVEL_GAMEPLAY, L"Layer_Monster", L"Prototype_GameObject_Normal_Elite_Knight", PivotMatrix, &MonsterDesc));
			pMonster->Set_Player(m_pPlayer);

			ZeroMemory(&MonsterDesc, sizeof(CMonster::MONSTEROPTION));
			PivotMatrix.r[3] = XMVectorSet(11.9f, 0.9f, 43.3f, 1.f);
			MonsterDesc.m_bFirstSpawnType[CMonster::STATE_ALREADYSPAWN] = true;
			MonsterDesc.MonsterDesc.m_iHP = MonsterDesc.MonsterDesc.m_iMaxHP = 300;
			MonsterDesc.MonsterDesc.m_iDamage = 15;
			MonsterDesc.MonsterDesc.m_iShield = MonsterDesc.MonsterDesc.m_iMaxShield = 0;
			MonsterDesc.MonsterDesc.TransformDesc.fSpeedPerSec = 5.f;
			MonsterDesc.m_iCellIndex = 385;
			pMonster = dynamic_cast<CMonster*>(m_pGameInstance->Clone_GameObjectReturnPtr_M(LEVEL_GAMEPLAY, L"Layer_Monster", L"Prototype_GameObject_Normal_Human_Spear", PivotMatrix, &MonsterDesc));
			pMonster->Set_Player(m_pPlayer);

			ZeroMemory(&MonsterDesc, sizeof(CMonster::MONSTEROPTION));
			PivotMatrix.r[3] = XMVectorSet(13.f, 0.f, 35.1f, 1.f);
			MonsterDesc.m_bFirstSpawnType[CMonster::STATE_ALREADYSPAWN] = true;			// No Detected == Patrol
			MonsterDesc.MonsterDesc.m_iHP = MonsterDesc.MonsterDesc.m_iMaxHP = 300;
			MonsterDesc.MonsterDesc.m_iDamage = 15;
			MonsterDesc.MonsterDesc.m_iShield = MonsterDesc.MonsterDesc.m_iMaxShield = 0;
			MonsterDesc.MonsterDesc.TransformDesc.fSpeedPerSec = 2.5f;
			MonsterDesc.MonsterDesc.TransformDesc.fRotationPerSec = XMConvertToRadians(180.f);
			MonsterDesc.m_iCellIndex = 388;
			pMonster = dynamic_cast<CMonster*>(m_pGameInstance->Clone_GameObjectReturnPtr_M(LEVEL_GAMEPLAY, L"Layer_Monster", L"Prototype_GameObject_Normal_Human_Granade", PivotMatrix, &MonsterDesc));
			pMonster->Set_Player(m_pPlayer);

			ZeroMemory(&MonsterDesc, sizeof(CMonster::MONSTEROPTION));
			PivotMatrix.r[3] = XMVectorSet(17.f, 0.f, 44.2f, 1.f);
			MonsterDesc.m_bFirstSpawnType[CMonster::STATE_ALREADYSPAWN] = true;
			MonsterDesc.MonsterDesc.m_iHP = MonsterDesc.MonsterDesc.m_iMaxHP = 300;
			MonsterDesc.MonsterDesc.m_iDamage = 15;
			MonsterDesc.MonsterDesc.m_iShield = MonsterDesc.MonsterDesc.m_iMaxShield = 0;
			MonsterDesc.MonsterDesc.TransformDesc.fSpeedPerSec = 2.5f;
			MonsterDesc.MonsterDesc.TransformDesc.fRotationPerSec = XMConvertToRadians(180.f);
			MonsterDesc.m_iCellIndex = 402;
			pMonster = dynamic_cast<CMonster*>(m_pGameInstance->Clone_GameObjectReturnPtr_M(LEVEL_GAMEPLAY, L"Layer_Monster", L"Prototype_GameObject_Normal_Human_Bow", PivotMatrix, &MonsterDesc));
			pMonster->Set_Player(m_pPlayer);
#pragma endregion
			m_bSpawnTrigger = true;
		}
		return S_OK;
	}
	else
	{
		m_pPlayer->Set_PortalUI(false);
	}

	return S_OK;
}


// Elite_Knight
// m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(0.7f, 0.36f, 36.5f, 1.f));
// m_pNavigationCom->Set_CellIndex(360);

HRESULT CTrigger::Knight_To_Room_B()
{
	CCollider* pCollider = (CCollider*)m_pPlayer->Find_Component(L"Com_BODYSPHERE");
	NULL_CHECK_RETURN(pCollider, E_FAIL);
	if (m_pTriggerCollCom->Collision(pCollider))
	{
		m_pPlayer->Set_PortalUI(true);

		if (m_pGameInstance->Key_Down(DIK_F))
		{

			// CGameObject* pGameObject = nullptr;
			// CGameObject::GAMEOBJECTDESC	GameObjectDesc;
			// GameObjectDesc.m_iCountType = 3;
			// pGameObject = m_pGameInstance->Clone_GameObjectReturnPtr(m_pGameInstance->Get_StaticLevelIndex(), L"Layer_Logo", L"Prototype_GameObject_BackGround", &GameObjectDesc);
			// NULL_CHECK_RETURN(pGameObject, E_FAIL);

			m_pGameInstance->Delete_Light(4);
			m_pGameInstance->Delete_Light(5);

			LIGHTDESC			LightDesc;
			CGameObject::GAMEOBJECTDESC	tFireLightDesc;
			ZeroMemory(&LightDesc, sizeof LightDesc);
			//2
			_bool& LightBool = m_pGameInstance->Set_LightEnable(2);
			m_pGameInstance->Set_LightPos(2, XMVectorSet(-4.834f, 1.252f, 32.592f, 1.f));

			LightBool = true;
			ZeroMemory(&tFireLightDesc, sizeof(CGameObject::GAMEOBJECTDESC));
			tFireLightDesc.TransformDesc.vInitPos = _float3(-4.834f, 1.26f, 32.592f);
			tFireLightDesc.m_iNumber = 2;
			tFireLightDesc.m_vTexSize = _float2(1.f, 2.f);
			FAILED_CHECK_RETURN(m_pGameInstance->Clone_GameObject(LEVEL_GAMEPLAY, L"Layer_FireLight", L"Prototype_GameObject_Effect_Fire_Light", &tFireLightDesc), E_FAIL);
			// 2

			//3
			_bool& LightBool2 = m_pGameInstance->Set_LightEnable(3);
			m_pGameInstance->Set_LightPos(3, XMVectorSet(-23.667f, 1.64f, 59.58f, 1.f));

			LightBool2 = true;
			ZeroMemory(&tFireLightDesc, sizeof(CGameObject::GAMEOBJECTDESC));
			tFireLightDesc.TransformDesc.vInitPos = _float3(-23.667f, 1.36f, 59.58f);
			tFireLightDesc.m_iNumber = 3;
			tFireLightDesc.m_vTexSize = _float2(1.f, 2.f);
			FAILED_CHECK_RETURN(m_pGameInstance->Clone_GameObject(LEVEL_GAMEPLAY, L"Layer_FireLight", L"Prototype_GameObject_Effect_Fire_Light", &tFireLightDesc), E_FAIL);
			//3

			//6
			_bool& LightBool3 = m_pGameInstance->Set_LightEnable(6);
			m_pGameInstance->Set_LightPos(6, XMVectorSet(-29.583f, 1.64f, 32.59f, 1.f));

			LightBool3 = true;
			ZeroMemory(&tFireLightDesc, sizeof(CGameObject::GAMEOBJECTDESC));
			tFireLightDesc.TransformDesc.vInitPos = _float3(-29.583f, 1.35f, 32.59f);
			tFireLightDesc.m_iNumber = 6;
			tFireLightDesc.m_vTexSize = _float2(1.f, 2.f);
			FAILED_CHECK_RETURN(m_pGameInstance->Clone_GameObject(LEVEL_GAMEPLAY, L"Layer_FireLight", L"Prototype_GameObject_Effect_Fire_Light", &tFireLightDesc), E_FAIL);


			//6

			//7
			ZeroMemory(&tFireLightDesc, sizeof(CGameObject::GAMEOBJECTDESC));
			tFireLightDesc.TransformDesc.vInitPos = _float3(-37.623f, 1.25f, 59.634f);
			tFireLightDesc.m_iNumber = 7;
			tFireLightDesc.m_vTexSize = _float2(1.f, 2.f);
			FAILED_CHECK_RETURN(m_pGameInstance->Clone_GameObject(LEVEL_GAMEPLAY, L"Layer_FireLight", L"Prototype_GameObject_Effect_Fire_Light", &tFireLightDesc), E_FAIL);

			ZeroMemory(&LightDesc, sizeof LightDesc);

			LightDesc.eType = LIGHTDESC::LIGHT_POINT;
			LightDesc.isEnable = true;
			/*LightDesc.vDirection = _float4(1.f, -1.f, 1.0f, 0.f);*/
			LightDesc.vPosition = _float4(-37.623f, 1.26f, 59.634f, 1.f);
			LightDesc.fRange = 8.0f;
			LightDesc.vDiffuse = _float4(0.9f, 0.3f, 0.15f, 1.f);
			LightDesc.vAmbient = _float4(0.2f, 0.2f, 0.2f, 0.2f);
			LightDesc.vSpecular = _float4(0.1f, 0.1f, 0.1f, 1.f);
			FAILED_CHECK_RETURN(m_pGameInstance->Add_Light(m_pDevice, m_pContext, LightDesc), E_FAIL);


			// 8
			ZeroMemory(&LightDesc, sizeof LightDesc);

			LightDesc.eType = LIGHTDESC::LIGHT_POINT;
			LightDesc.isEnable = true;
			/*LightDesc.vDirection = _float4(1.f, -1.f, 1.0f, 0.f);*/
			LightDesc.vPosition = _float4(-9.6f, 1.75f, 46.3f, 1.f);
			LightDesc.fRange = 10.0f;
			LightDesc.vDiffuse = _float4(0.6f, 0.87f, 0.9f, 1.f);
			LightDesc.vAmbient = _float4(0.2f, 0.2f, 0.2f, 0.2f);
			LightDesc.vSpecular = _float4(0.1f, 0.1f, 0.1f, 1.f);
			FAILED_CHECK_RETURN(m_pGameInstance->Add_Light(m_pDevice, m_pContext, LightDesc), E_FAIL);
			//7

			// Num 3
			// _bool& BlueLightBool = CGameInstance::GetInstance()->Set_LightEnable(5);
			// CGameInstance::GetInstance()->Set_LightPos(5, XMVectorSet(14.999f, 1.211f, 40.403f, 1.f));
			// CGameInstance::GetInstance()->Set_LightRange(5, 10.f);
			// BlueLightBool = true;


#pragma region TriggerSetting
			m_pPlayer->Set_RoomType(CPlayer::ROOM_B);
			_matrix PlayerMat = m_pPlayer->Get_WorldFloat4x4();
			PlayerMat.r[3] = XMVectorSet(-10.018f, 0.032f, 33.904f, 1.f);
			m_pPlayer->Set_CellIndex(194);
			m_pPlayer->Set_WorldMatrix(PlayerMat);

			CMonster::MONSTEROPTION MonsterDesc;
			CMonster* pMonster = nullptr;
			_matrix PivotMatrix = XMMatrixIdentity();

			ZeroMemory(&MonsterDesc, sizeof(CMonster::MONSTEROPTION));
			PivotMatrix.r[3] = XMVectorSet(-13.3f, 0.f, 48.4f, 1.f);
			MonsterDesc.m_bFirstSpawnType[CMonster::STATE_ALREADYSPAWN] = true;
			MonsterDesc.MonsterDesc.m_iHP = MonsterDesc.MonsterDesc.m_iMaxHP = 300;
			MonsterDesc.MonsterDesc.m_iDamage = 15;
			MonsterDesc.MonsterDesc.m_iShield = MonsterDesc.MonsterDesc.m_iMaxShield = 0;
			MonsterDesc.MonsterDesc.TransformDesc.fSpeedPerSec = 5.f;
			MonsterDesc.m_iCellIndex = 205;
			pMonster = dynamic_cast<CMonster*>(m_pGameInstance->Clone_GameObjectReturnPtr_M(LEVEL_GAMEPLAY, L"Layer_Monster", L"Prototype_GameObject_Normal_Human_Spear", PivotMatrix, &MonsterDesc));
			pMonster->Set_Player(m_pPlayer);

			ZeroMemory(&MonsterDesc, sizeof(CMonster::MONSTEROPTION));
			PivotMatrix.r[3] = XMVectorSet(-15.8f, 0.18f, 55.5f, 1.f);
			MonsterDesc.m_bFirstSpawnType[CMonster::STATE_ALREADYSPAWN] = true;
			MonsterDesc.MonsterDesc.m_iHP = MonsterDesc.MonsterDesc.m_iMaxHP = 300;
			MonsterDesc.MonsterDesc.m_iDamage = 15;
			MonsterDesc.MonsterDesc.m_iShield = MonsterDesc.MonsterDesc.m_iMaxShield = 0;
			MonsterDesc.MonsterDesc.TransformDesc.fSpeedPerSec = 5.f;
			MonsterDesc.m_iCellIndex = 221;
			pMonster = dynamic_cast<CMonster*>(m_pGameInstance->Clone_GameObjectReturnPtr_M(LEVEL_GAMEPLAY, L"Layer_Monster", L"Prototype_GameObject_Normal_Human_Spear", PivotMatrix, &MonsterDesc));
			pMonster->Set_Player(m_pPlayer);

			// ZeroMemory(&MonsterDesc, sizeof(CMonster::MONSTEROPTION));
			// PivotMatrix.r[3] = XMVectorSet(-23.7f, 1.4f, 45.7f, 1.f);
			// MonsterDesc.m_bFirstSpawnType[CMonster::STATE_ALREADYSPAWN] = true;
			// MonsterDesc.MonsterDesc.m_iHP = MonsterDesc.MonsterDesc.m_iMaxHP = 300;
			// MonsterDesc.MonsterDesc.m_iDamage = 15;
			// MonsterDesc.MonsterDesc.m_iShield = MonsterDesc.MonsterDesc.m_iMaxShield = 0;
			// MonsterDesc.MonsterDesc.TransformDesc.fSpeedPerSec = 5.f;
			// MonsterDesc.m_iCellIndex = 237;
			// pMonster = dynamic_cast<CMonster*>(m_pGameInstance->Clone_GameObjectReturnPtr_M(LEVEL_GAMEPLAY, L"Layer_Monster", L"Prototype_GameObject_Normal_Human_Spear", PivotMatrix, &MonsterDesc));
			// pMonster->Set_Player(m_pPlayer);

			ZeroMemory(&MonsterDesc, sizeof(CMonster::MONSTEROPTION));
			PivotMatrix.r[3] = XMVectorSet(-32.3f, 1.4f, 52.8f, 1.f);
			MonsterDesc.m_bFirstSpawnType[CMonster::STATE_ALREADYSPAWN] = true;
			MonsterDesc.MonsterDesc.m_iHP = MonsterDesc.MonsterDesc.m_iMaxHP = 300;
			MonsterDesc.MonsterDesc.m_iDamage = 15;
			MonsterDesc.MonsterDesc.m_iShield = MonsterDesc.MonsterDesc.m_iMaxShield = 0;
			MonsterDesc.MonsterDesc.TransformDesc.fSpeedPerSec = 5.f;
			MonsterDesc.m_iCellIndex = 270;
			pMonster = dynamic_cast<CMonster*>(m_pGameInstance->Clone_GameObjectReturnPtr_M(LEVEL_GAMEPLAY, L"Layer_Monster", L"Prototype_GameObject_Normal_Human_Spear", PivotMatrix, &MonsterDesc));
			pMonster->Set_Player(m_pPlayer);

			ZeroMemory(&MonsterDesc, sizeof(CMonster::MONSTEROPTION));
			PivotMatrix.r[3] = XMVectorSet(-12.2f, 0.f, 57.4f, 1.f);
			MonsterDesc.m_bFirstSpawnType[CMonster::STATE_ALREADYSPAWN] = true;
			MonsterDesc.MonsterDesc.m_iHP = MonsterDesc.MonsterDesc.m_iMaxHP = 300;
			MonsterDesc.MonsterDesc.m_iDamage = 15;
			MonsterDesc.MonsterDesc.m_iShield = MonsterDesc.MonsterDesc.m_iMaxShield = 0;
			MonsterDesc.MonsterDesc.TransformDesc.fSpeedPerSec = 2.5f;
			MonsterDesc.MonsterDesc.TransformDesc.fRotationPerSec = XMConvertToRadians(180.f);
			MonsterDesc.m_iCellIndex = 220;
			pMonster = dynamic_cast<CMonster*>(m_pGameInstance->Clone_GameObjectReturnPtr_M(LEVEL_GAMEPLAY, L"Layer_Monster", L"Prototype_GameObject_Normal_Human_Bow", PivotMatrix, &MonsterDesc));
			pMonster->Set_Player(m_pPlayer);

			ZeroMemory(&MonsterDesc, sizeof(CMonster::MONSTEROPTION));
			PivotMatrix.r[3] = XMVectorSet(-34.2f, 0.f, 37.8f, 1.f);
			MonsterDesc.m_bFirstSpawnType[CMonster::STATE_ALREADYSPAWN] = true;
			MonsterDesc.MonsterDesc.m_iHP = MonsterDesc.MonsterDesc.m_iMaxHP = 300;
			MonsterDesc.MonsterDesc.m_iDamage = 15;
			MonsterDesc.MonsterDesc.m_iShield = MonsterDesc.MonsterDesc.m_iMaxShield = 0;
			MonsterDesc.MonsterDesc.TransformDesc.fSpeedPerSec = 2.5f;
			MonsterDesc.MonsterDesc.TransformDesc.fRotationPerSec = XMConvertToRadians(180.f);
			MonsterDesc.m_iCellIndex = 256;
			pMonster = dynamic_cast<CMonster*>(m_pGameInstance->Clone_GameObjectReturnPtr_M(LEVEL_GAMEPLAY, L"Layer_Monster", L"Prototype_GameObject_Normal_Human_Bow", PivotMatrix, &MonsterDesc));
			pMonster->Set_Player(m_pPlayer);

			// ZeroMemory(&MonsterDesc, sizeof(CMonster::MONSTEROPTION));
			// PivotMatrix.r[3] = XMVectorSet(-20.1f, 0.f, 37.6f, 1.f);
			// MonsterDesc.m_bFirstSpawnType[CMonster::STATE_ALREADYSPAWN] = true;			// No Detected == Patrol
			// MonsterDesc.MonsterDesc.m_iHP = MonsterDesc.MonsterDesc.m_iMaxHP = 300;
			// MonsterDesc.MonsterDesc.m_iDamage = 15;
			// MonsterDesc.MonsterDesc.m_iShield = MonsterDesc.MonsterDesc.m_iMaxShield = 0;
			// MonsterDesc.MonsterDesc.TransformDesc.fSpeedPerSec = 2.5f;
			// MonsterDesc.MonsterDesc.TransformDesc.fRotationPerSec = XMConvertToRadians(180.f);
			// MonsterDesc.m_iCellIndex = 243;
			// pMonster = dynamic_cast<CMonster*>(m_pGameInstance->Clone_GameObjectReturnPtr_M(LEVEL_GAMEPLAY, L"Layer_Monster", L"Prototype_GameObject_Normal_Human_Granade", PivotMatrix, &MonsterDesc));
			// pMonster->Set_Player(m_pPlayer);

			ZeroMemory(&MonsterDesc, sizeof(CMonster::MONSTEROPTION));
			PivotMatrix.r[3] = XMVectorSet(-30.1f, 1.4f, 47.0f, 1.f);
			MonsterDesc.m_bFirstSpawnType[CMonster::STATE_ALREADYSPAWN] = true;			// No Detected == Patrol
			MonsterDesc.MonsterDesc.m_iHP = MonsterDesc.MonsterDesc.m_iMaxHP = 300;
			MonsterDesc.MonsterDesc.m_iDamage = 15;
			MonsterDesc.MonsterDesc.m_iShield = MonsterDesc.MonsterDesc.m_iMaxShield = 0;
			MonsterDesc.MonsterDesc.TransformDesc.fSpeedPerSec = 2.5f;
			MonsterDesc.MonsterDesc.TransformDesc.fRotationPerSec = XMConvertToRadians(180.f);
			MonsterDesc.m_iCellIndex = 265;
			pMonster = dynamic_cast<CMonster*>(m_pGameInstance->Clone_GameObjectReturnPtr_M(LEVEL_GAMEPLAY, L"Layer_Monster", L"Prototype_GameObject_Normal_Human_Granade", PivotMatrix, &MonsterDesc));
			pMonster->Set_Player(m_pPlayer);

			ZeroMemory(&MonsterDesc, sizeof(CMonster::MONSTEROPTION)); // 위치 확인 필요
			PivotMatrix.r[3] = XMVectorSet(-30.1f, 1.4f, 47.0f, 1.f);
			MonsterDesc.m_bFirstSpawnType[CMonster::STATE_ALREADYSPAWN] = true;			// No Detected == Patrol
			MonsterDesc.MonsterDesc.m_iHP = MonsterDesc.MonsterDesc.m_iMaxHP = 300;
			MonsterDesc.MonsterDesc.m_iDamage = 15;
			MonsterDesc.MonsterDesc.m_iShield = MonsterDesc.MonsterDesc.m_iMaxShield = 0;
			MonsterDesc.MonsterDesc.TransformDesc.fSpeedPerSec = 2.5f;
			MonsterDesc.MonsterDesc.TransformDesc.fRotationPerSec = XMConvertToRadians(180.f);
			MonsterDesc.m_iCellIndex = 269;
			pMonster = dynamic_cast<CMonster*>(m_pGameInstance->Clone_GameObjectReturnPtr_M(LEVEL_GAMEPLAY, L"Layer_Monster", L"Prototype_GameObject_Normal_Human_Granade", PivotMatrix, &MonsterDesc));
			pMonster->Set_Player(m_pPlayer);

			ZeroMemory(&MonsterDesc, sizeof(CMonster::MONSTEROPTION));
			PivotMatrix.r[3] = XMVectorSet(-7.7f, -0.85f, 42.f, 1.f);
			MonsterDesc.m_bFirstSpawnType[CMonster::STATE_ALREADYSPAWN] = true;
			MonsterDesc.MonsterDesc.m_iHP = MonsterDesc.MonsterDesc.m_iMaxHP = 100;
			MonsterDesc.MonsterDesc.m_iDamage = 15;
			MonsterDesc.MonsterDesc.m_iShield = MonsterDesc.MonsterDesc.m_iMaxShield = 0;
			MonsterDesc.MonsterDesc.TransformDesc.fSpeedPerSec = 2.5f;
			MonsterDesc.MonsterDesc.TransformDesc.fRotationPerSec = XMConvertToRadians(180.f);
			MonsterDesc.m_iCellIndex = 201;
			pMonster = dynamic_cast<CMonster*>(m_pGameInstance->Clone_GameObjectReturnPtr_M(LEVEL_GAMEPLAY, L"Layer_Monster", L"Prototype_GameObject_Normal_Human_Explode", PivotMatrix, &MonsterDesc));
			pMonster->Set_Player(m_pPlayer);

			ZeroMemory(&MonsterDesc, sizeof(CMonster::MONSTEROPTION));
			PivotMatrix.r[3] = XMVectorSet(-20.7f, 0.f, 51.8f, 1.f);
			MonsterDesc.m_bFirstSpawnType[CMonster::STATE_ALREADYSPAWN] = true;
			MonsterDesc.MonsterDesc.m_iHP = MonsterDesc.MonsterDesc.m_iMaxHP = 100;
			MonsterDesc.MonsterDesc.m_iDamage = 15;
			MonsterDesc.MonsterDesc.m_iShield = MonsterDesc.MonsterDesc.m_iMaxShield = 0;
			MonsterDesc.MonsterDesc.TransformDesc.fSpeedPerSec = 2.5f;
			MonsterDesc.MonsterDesc.TransformDesc.fRotationPerSec = XMConvertToRadians(180.f);
			MonsterDesc.m_iCellIndex = 232;
			pMonster = dynamic_cast<CMonster*>(m_pGameInstance->Clone_GameObjectReturnPtr_M(LEVEL_GAMEPLAY, L"Layer_Monster", L"Prototype_GameObject_Normal_Human_Explode", PivotMatrix, &MonsterDesc));
			pMonster->Set_Player(m_pPlayer);
#pragma endregion

			m_bSpawnTrigger = true;

		}
		return S_OK;
	}
	else
	{
		m_pPlayer->Set_PortalUI(false);
	}

	return S_OK;
}

//Elite_Bug
// m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(-59.f, -0.9f, 64.6f, 1.f));
// m_pNavigationCom->Set_CellIndex(442);


HRESULT CTrigger::Room_B_To_Bug()
{
	CCollider* pCollider = (CCollider*)m_pPlayer->Find_Component(L"Com_BODYSPHERE");
	NULL_CHECK_RETURN(pCollider, E_FAIL);
	if (m_pTriggerCollCom->Collision(pCollider))
	{
		m_pPlayer->Set_PortalUI(true);

		if (m_pGameInstance->Key_Down(DIK_F))
		{
			// CGameObject* pGameObject = nullptr;
			// CGameObject::GAMEOBJECTDESC	GameObjectDesc;
			// GameObjectDesc.m_iCountType = 4;
			// pGameObject = m_pGameInstance->Clone_GameObjectReturnPtr(m_pGameInstance->Get_StaticLevelIndex(), L"Layer_Logo", L"Prototype_GameObject_BackGround", &GameObjectDesc);
			// NULL_CHECK_RETURN(pGameObject, E_FAIL);

			m_pGameInstance->Delete_Light(2);
			m_pGameInstance->Delete_Light(3);
			m_pGameInstance->Delete_Light(6);
			m_pGameInstance->Delete_Light(7);
			m_pGameInstance->Delete_Light(8);

			LIGHTDESC			LightDesc;
			CGameObject::GAMEOBJECTDESC	tFireLightDesc;
			ZeroMemory(&LightDesc, sizeof LightDesc);
			// 4 
			_bool& LightBool = m_pGameInstance->Set_LightEnable(4);
			m_pGameInstance->Set_LightPos(4, XMVectorSet(-56.2f, 2.369f, 62.046f, 1.f));

			LightBool = true;
			ZeroMemory(&tFireLightDesc, sizeof(CGameObject::GAMEOBJECTDESC));
			tFireLightDesc.TransformDesc.vInitPos = _float3(-56.2f, 2.7f, 62.046f);
			tFireLightDesc.m_iNumber = 4;
			tFireLightDesc.m_vTexSize = _float2(1.f, 2.f);
			for (_uint i = 0; i<15; ++i)
				FAILED_CHECK_RETURN(m_pGameInstance->Clone_GameObject(LEVEL_GAMEPLAY, L"Layer_FireLight", L"Prototype_GameObject_BlueLight", &tFireLightDesc), E_FAIL);




			// Num 5
			_bool& BlueLightBool = m_pGameInstance->Set_LightEnable(5);
			m_pGameInstance->Set_LightPos(5, XMVectorSet(-61.219f, 2.549f, 62.046f, 1.f));
			m_pGameInstance->Set_LightRange(5, 10.f);
			BlueLightBool = true;

			ZeroMemory(&tFireLightDesc, sizeof(CGameObject::GAMEOBJECTDESC));
			tFireLightDesc.TransformDesc.vInitPos = _float3(-61.219f, 2.369f, 62.046f);
			tFireLightDesc.m_iNumber = 5;
			tFireLightDesc.m_vTexSize = _float2(1.f, 2.f);
			for (_uint i = 0; i<15; ++i)
				FAILED_CHECK_RETURN(m_pGameInstance->Clone_GameObject(LEVEL_GAMEPLAY, L"Layer_FireLight", L"Prototype_GameObject_BlueLight", &tFireLightDesc), E_FAIL);


			// // Num 8

			_bool& BlueLightBool8 = m_pGameInstance->Set_LightEnable(8);
			m_pGameInstance->Set_LightPos(8, XMVectorSet(-60.378f, 1.116f, 94.038f, 1.f));
			m_pGameInstance->Set_LightRange(8, 20.f);
			BlueLightBool8 = true;

			// ZeroMemory(&LightDesc, sizeof LightDesc);
			//
			// LightDesc.eType = LIGHTDESC::LIGHT_POINT;
			// LightDesc.isEnable = true;
			// /*LightDesc.vDirection = _float4(1.f, -1.f, 1.0f, 0.f);*/
			// LightDesc.vPosition = _float4(-60.378f, 1.116f, 94.038f, 1.f);
			// LightDesc.fRange = 20.0f;
			// LightDesc.vDiffuse = _float4(0.6f, 0.87f, 0.9f, 1.f);
			// LightDesc.vAmbient = _float4(0.2f, 0.2f, 0.2f, 0.2f);
			// LightDesc.vSpecular = LightDesc.vDiffuse;
			// FAILED_CHECK_RETURN(m_pGameInstance->Add_Light(m_pDevice, m_pContext, LightDesc), E_FAIL);

			// Num 9

			ZeroMemory(&tFireLightDesc, sizeof(CGameObject::GAMEOBJECTDESC));
			tFireLightDesc.TransformDesc.vInitPos = _float3(-63.017f, 5.4f, 109.864f);
			tFireLightDesc.m_iNumber = 9;
			tFireLightDesc.m_vTexSize = _float2(0.7f, 2.f);
			for (_uint i = 0; i<15; ++i)
				FAILED_CHECK_RETURN(m_pGameInstance->Clone_GameObject(LEVEL_GAMEPLAY, L"Layer_FireLight", L"Prototype_GameObject_BlueLight", &tFireLightDesc), E_FAIL);

			ZeroMemory(&LightDesc, sizeof LightDesc);

			LightDesc.eType = LIGHTDESC::LIGHT_POINT;
			LightDesc.isEnable = true;


			LightDesc.vPosition = _float4(-63.017f, 5.42f, 109.864f, 1.f);
			LightDesc.fRange = 8.0f;
			LightDesc.vDiffuse = _float4(0.6f, 0.87f, 0.9f, 1.f);
			LightDesc.vAmbient = _float4(0.2f, 0.2f, 0.2f, 0.2f);
			LightDesc.vSpecular = _float4(0.1f, 0.1f, 0.1f, 1.f);
			FAILED_CHECK_RETURN(m_pGameInstance->Add_Light(m_pDevice, m_pContext, LightDesc), E_FAIL);

			// Num 10

			ZeroMemory(&tFireLightDesc, sizeof(CGameObject::GAMEOBJECTDESC));
			tFireLightDesc.TransformDesc.vInitPos = _float3(-58.f, 5.42f, 109.864f);
			tFireLightDesc.m_iNumber = 10;
			tFireLightDesc.m_vTexSize = _float2(0.7f, 2.f);
			for (_uint i = 0; i<15; ++i)
				FAILED_CHECK_RETURN(m_pGameInstance->Clone_GameObject(LEVEL_GAMEPLAY, L"Layer_FireLight", L"Prototype_GameObject_BlueLight", &tFireLightDesc), E_FAIL);

			ZeroMemory(&LightDesc, sizeof LightDesc);

			LightDesc.eType = LIGHTDESC::LIGHT_POINT;
			LightDesc.isEnable = true;


			LightDesc.vPosition = _float4(-58.f, 5.4f, 109.864f, 1.f);
			LightDesc.fRange = 8.0f;
			LightDesc.vDiffuse = _float4(0.6f, 0.87f, 0.9f, 1.f);
			LightDesc.vAmbient = _float4(0.2f, 0.2f, 0.2f, 0.2f);
			LightDesc.vSpecular = _float4(0.1f, 0.1f, 0.1f, 1.f);
			FAILED_CHECK_RETURN(m_pGameInstance->Add_Light(m_pDevice, m_pContext, LightDesc), E_FAIL);


#pragma region TriggerSetting

			m_pPlayer->Set_RoomType(CPlayer::ROOM_BUG);
			_matrix PlayerMat = m_pPlayer->Get_WorldFloat4x4();
			PlayerMat.r[3] = XMVectorSet(-59.f, -0.9f, 64.6f, 1.f);
			m_pPlayer->Set_CellIndex(442);
			m_pPlayer->Set_WorldMatrix(PlayerMat);

			CMonster::MONSTEROPTION MonsterDesc;
			CMonster* pMonster = nullptr;

			_matrix PivotMatrix = XMMatrixRotationAxis(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(180.f));
			ZeroMemory(&MonsterDesc, sizeof(CMonster::MONSTEROPTION));
			PivotMatrix.r[3] = XMVectorSet(-64.3f, -0.7f, 73.8f, 1.f);
			MonsterDesc.m_bFirstSpawnType[CMonster::STATE_NODETECTED] = true;
			MonsterDesc.MonsterDesc.m_iHP = MonsterDesc.MonsterDesc.m_iMaxHP = 300;
			MonsterDesc.MonsterDesc.m_iDamage = 15;
			MonsterDesc.MonsterDesc.m_iShield = MonsterDesc.MonsterDesc.m_iMaxShield = 0;
			MonsterDesc.MonsterDesc.TransformDesc.fSpeedPerSec = 5.f;
			MonsterDesc.m_iCellIndex = 445;
			pMonster = dynamic_cast<CMonster*>(m_pGameInstance->Clone_GameObjectReturnPtr_M(LEVEL_GAMEPLAY, L"Layer_Monster", L"Prototype_GameObject_Normal_Human_Sword", PivotMatrix, &MonsterDesc));
			pMonster->Set_Player(m_pPlayer);

			// ZeroMemory(&MonsterDesc, sizeof(CMonster::MONSTEROPTION));
			// PivotMatrix.r[3] = XMVectorSet(-64.5f, -0.9f, 69.2f, 1.f);
			// MonsterDesc.m_bFirstSpawnType[CMonster::STATE_NODETECTED] = true;
			// MonsterDesc.MonsterDesc.m_iHP = MonsterDesc.MonsterDesc.m_iMaxHP = 300;
			// MonsterDesc.MonsterDesc.m_iDamage = 15;
			// MonsterDesc.MonsterDesc.m_iShield = MonsterDesc.MonsterDesc.m_iMaxShield = 0;
			// MonsterDesc.MonsterDesc.TransformDesc.fSpeedPerSec = 5.f;
			// MonsterDesc.m_iCellIndex = 446;
			// pMonster = dynamic_cast<CMonster*>(m_pGameInstance->Clone_GameObjectReturnPtr_M(LEVEL_GAMEPLAY, L"Layer_Monster", L"Prototype_GameObject_Normal_Human_Sword", PivotMatrix, &MonsterDesc));
			// pMonster->Set_Player(m_pPlayer);

			ZeroMemory(&MonsterDesc, sizeof(CMonster::MONSTEROPTION));
			PivotMatrix.r[3] = XMVectorSet(-62.5f, -0.9f, 71.6f, 1.f);
			MonsterDesc.m_bFirstSpawnType[CMonster::STATE_NODETECTED] = true;
			MonsterDesc.MonsterDesc.m_iHP = MonsterDesc.MonsterDesc.m_iMaxHP = 300;
			MonsterDesc.MonsterDesc.m_iDamage = 15;
			MonsterDesc.MonsterDesc.m_iShield = MonsterDesc.MonsterDesc.m_iMaxShield = 0;
			MonsterDesc.MonsterDesc.TransformDesc.fSpeedPerSec = 5.f;
			MonsterDesc.m_iCellIndex = 446;
			pMonster = dynamic_cast<CMonster*>(m_pGameInstance->Clone_GameObjectReturnPtr_M(LEVEL_GAMEPLAY, L"Layer_Monster", L"Prototype_GameObject_Normal_Human_Sword", PivotMatrix, &MonsterDesc));
			pMonster->Set_Player(m_pPlayer);

			ZeroMemory(&MonsterDesc, sizeof(CMonster::MONSTEROPTION));
			PivotMatrix.r[3] = XMVectorSet(-56.2f, -0.9f, 71.98f, 1.f);
			MonsterDesc.m_bFirstSpawnType[CMonster::STATE_NODETECTED] = true;
			MonsterDesc.MonsterDesc.m_iHP = MonsterDesc.MonsterDesc.m_iMaxHP = 300;
			MonsterDesc.MonsterDesc.m_iDamage = 15;
			MonsterDesc.MonsterDesc.m_iShield = MonsterDesc.MonsterDesc.m_iMaxShield = 0;
			MonsterDesc.MonsterDesc.TransformDesc.fSpeedPerSec = 5.f;
			MonsterDesc.m_iCellIndex = 448;
			pMonster = dynamic_cast<CMonster*>(m_pGameInstance->Clone_GameObjectReturnPtr_M(LEVEL_GAMEPLAY, L"Layer_Monster", L"Prototype_GameObject_Normal_Human_Sword", PivotMatrix, &MonsterDesc));
			pMonster->Set_Player(m_pPlayer);

			// ZeroMemory(&MonsterDesc, sizeof(CMonster::MONSTEROPTION));
			// PivotMatrix.r[3] = XMVectorSet(-56.8f, -1.f, 76.5f, 1.f);
			// MonsterDesc.m_bFirstSpawnType[CMonster::STATE_NODETECTED] = true;
			// MonsterDesc.MonsterDesc.m_iHP = MonsterDesc.MonsterDesc.m_iMaxHP = 300;
			// MonsterDesc.MonsterDesc.m_iDamage = 15;
			// MonsterDesc.MonsterDesc.m_iShield = MonsterDesc.MonsterDesc.m_iMaxShield = 0;
			// MonsterDesc.MonsterDesc.TransformDesc.fSpeedPerSec = 5.f;
			// MonsterDesc.m_iCellIndex = 449;
			// pMonster = dynamic_cast<CMonster*>(m_pGameInstance->Clone_GameObjectReturnPtr_M(LEVEL_GAMEPLAY, L"Layer_Monster", L"Prototype_GameObject_Normal_Human_Sword", PivotMatrix, &MonsterDesc));
			// pMonster->Set_Player(m_pPlayer);

			ZeroMemory(&MonsterDesc, sizeof(CMonster::MONSTEROPTION));
			PivotMatrix.r[3] = XMVectorSet(-59.2f, -0.9f, 78.5f, 1.f);
			MonsterDesc.m_bFirstSpawnType[CMonster::STATE_NODETECTED] = true;
			MonsterDesc.MonsterDesc.m_iHP = MonsterDesc.MonsterDesc.m_iMaxHP = 300;
			MonsterDesc.MonsterDesc.m_iDamage = 15;
			MonsterDesc.MonsterDesc.m_iShield = MonsterDesc.MonsterDesc.m_iMaxShield = 0;
			MonsterDesc.MonsterDesc.TransformDesc.fSpeedPerSec = 5.f;
			MonsterDesc.m_iCellIndex = 449;
			pMonster = dynamic_cast<CMonster*>(m_pGameInstance->Clone_GameObjectReturnPtr_M(LEVEL_GAMEPLAY, L"Layer_Monster", L"Prototype_GameObject_Normal_Human_Sword", PivotMatrix, &MonsterDesc));
			pMonster->Set_Player(m_pPlayer);

			// ZeroMemory(&MonsterDesc, sizeof(CMonster::MONSTEROPTION));
			// PivotMatrix.r[3] = XMVectorSet(-68.f, -0.9f, 85.6f, 1.f);
			// MonsterDesc.m_bFirstSpawnType[CMonster::STATE_NODETECTED] = true;
			// MonsterDesc.MonsterDesc.m_iHP = MonsterDesc.MonsterDesc.m_iMaxHP = 300;
			// MonsterDesc.MonsterDesc.m_iDamage = 15;
			// MonsterDesc.MonsterDesc.m_iShield = MonsterDesc.MonsterDesc.m_iMaxShield = 0;
			// MonsterDesc.MonsterDesc.TransformDesc.fSpeedPerSec = 5.f;
			// MonsterDesc.m_iCellIndex = 455;
			// pMonster = dynamic_cast<CMonster*>(m_pGameInstance->Clone_GameObjectReturnPtr_M(LEVEL_GAMEPLAY, L"Layer_Monster", L"Prototype_GameObject_Normal_Human_Sword", PivotMatrix, &MonsterDesc));
			// pMonster->Set_Player(m_pPlayer);

			ZeroMemory(&MonsterDesc, sizeof(CMonster::MONSTEROPTION));
			PivotMatrix.r[3] = XMVectorSet(-63.3f, -0.97f, 87.2f, 1.f);
			MonsterDesc.m_bFirstSpawnType[CMonster::STATE_NODETECTED] = true;
			MonsterDesc.MonsterDesc.m_iHP = MonsterDesc.MonsterDesc.m_iMaxHP = 300;
			MonsterDesc.MonsterDesc.m_iDamage = 15;
			MonsterDesc.MonsterDesc.m_iShield = MonsterDesc.MonsterDesc.m_iMaxShield = 0;
			MonsterDesc.MonsterDesc.TransformDesc.fSpeedPerSec = 5.f;
			MonsterDesc.m_iCellIndex = 453;
			pMonster = dynamic_cast<CMonster*>(m_pGameInstance->Clone_GameObjectReturnPtr_M(LEVEL_GAMEPLAY, L"Layer_Monster", L"Prototype_GameObject_Normal_Human_Sword", PivotMatrix, &MonsterDesc));
			pMonster->Set_Player(m_pPlayer);

			// ZeroMemory(&MonsterDesc, sizeof(CMonster::MONSTEROPTION));
			// PivotMatrix.r[3] = XMVectorSet(-57.1f, -0.97f, 83.7f, 1.f);
			// MonsterDesc.m_bFirstSpawnType[CMonster::STATE_NODETECTED] = true;
			// MonsterDesc.MonsterDesc.m_iHP = MonsterDesc.MonsterDesc.m_iMaxHP = 300;
			// MonsterDesc.MonsterDesc.m_iDamage = 15;
			// MonsterDesc.MonsterDesc.m_iShield = MonsterDesc.MonsterDesc.m_iMaxShield = 0;
			// MonsterDesc.MonsterDesc.TransformDesc.fSpeedPerSec = 5.f;
			// MonsterDesc.m_iCellIndex = 453;
			// pMonster = dynamic_cast<CMonster*>(m_pGameInstance->Clone_GameObjectReturnPtr_M(LEVEL_GAMEPLAY, L"Layer_Monster", L"Prototype_GameObject_Normal_Human_Sword", PivotMatrix, &MonsterDesc));
			// pMonster->Set_Player(m_pPlayer);

			ZeroMemory(&MonsterDesc, sizeof(CMonster::MONSTEROPTION));
			PivotMatrix.r[3] = XMVectorSet(-63.2f, 0.95f, 99.3f, 1.f);
			MonsterDesc.m_bFirstSpawnType[CMonster::STATE_NODETECTED] = true;
			MonsterDesc.MonsterDesc.m_iHP = MonsterDesc.MonsterDesc.m_iMaxHP = 300;
			MonsterDesc.MonsterDesc.m_iDamage = 15;
			MonsterDesc.MonsterDesc.m_iShield = MonsterDesc.MonsterDesc.m_iMaxShield = 0;
			MonsterDesc.MonsterDesc.TransformDesc.fSpeedPerSec = 5.f;
			MonsterDesc.m_iCellIndex = 466;
			pMonster = dynamic_cast<CMonster*>(m_pGameInstance->Clone_GameObjectReturnPtr_M(LEVEL_GAMEPLAY, L"Layer_Monster", L"Prototype_GameObject_Normal_Human_Sword", PivotMatrix, &MonsterDesc));
			pMonster->Set_Player(m_pPlayer);

			// ZeroMemory(&MonsterDesc, sizeof(CMonster::MONSTEROPTION));
			// PivotMatrix.r[3] = XMVectorSet(-59.566f, -0.961f, 74.761f, 1.f);
			// MonsterDesc.m_bFirstSpawnType[CMonster::STATE_NODETECTED] = true;
			// MonsterDesc.MonsterDesc.m_iHP = MonsterDesc.MonsterDesc.m_iMaxHP = 300;
			// MonsterDesc.MonsterDesc.m_iDamage = 15;
			// MonsterDesc.MonsterDesc.m_iShield = MonsterDesc.MonsterDesc.m_iMaxShield = 0;
			// MonsterDesc.MonsterDesc.TransformDesc.fSpeedPerSec = 5.f;
			// MonsterDesc.m_iCellIndex = 448;
			// pMonster = dynamic_cast<CMonster*>(m_pGameInstance->Clone_GameObjectReturnPtr_M(LEVEL_GAMEPLAY, L"Layer_Monster", L"Prototype_GameObject_Normal_Human_Spear", PivotMatrix, &MonsterDesc));
			// pMonster->Set_Player(m_pPlayer);

			ZeroMemory(&MonsterDesc, sizeof(CMonster::MONSTEROPTION));
			PivotMatrix = XMMatrixIdentity();
			PivotMatrix.r[3] = XMVectorSet(-57.4f, -0.9f, 67.2f, 1.f);
			MonsterDesc.m_bFirstSpawnType[CMonster::STATE_NODETECTED] = true;
			MonsterDesc.MonsterDesc.m_iHP = MonsterDesc.MonsterDesc.m_iMaxHP = 300;
			MonsterDesc.MonsterDesc.m_iDamage = 15;
			MonsterDesc.MonsterDesc.m_iShield = MonsterDesc.MonsterDesc.m_iMaxShield = 0;
			MonsterDesc.MonsterDesc.TransformDesc.fSpeedPerSec = 5.f;
			MonsterDesc.m_iCellIndex = 443;
			pMonster = dynamic_cast<CMonster*>(m_pGameInstance->Clone_GameObjectReturnPtr_M(LEVEL_GAMEPLAY, L"Layer_Monster", L"Prototype_GameObject_Normal_Human_Spear", PivotMatrix, &MonsterDesc));
			pMonster->Set_Player(m_pPlayer);

			ZeroMemory(&MonsterDesc, sizeof(CMonster::MONSTEROPTION));
			PivotMatrix.r[3] = XMVectorSet(-67.2f, 0.96f, 93.4f, 1.f);
			MonsterDesc.m_bFirstSpawnType[CMonster::STATE_NODETECTED] = true;
			MonsterDesc.MonsterDesc.m_iHP = MonsterDesc.MonsterDesc.m_iMaxHP = 300;
			MonsterDesc.MonsterDesc.m_iDamage = 15;
			MonsterDesc.MonsterDesc.m_iShield = MonsterDesc.MonsterDesc.m_iMaxShield = 0;
			MonsterDesc.MonsterDesc.TransformDesc.fSpeedPerSec = 5.f;
			MonsterDesc.m_iCellIndex = 460;
			pMonster = dynamic_cast<CMonster*>(m_pGameInstance->Clone_GameObjectReturnPtr_M(LEVEL_GAMEPLAY, L"Layer_Monster", L"Prototype_GameObject_Normal_Human_Spear", PivotMatrix, &MonsterDesc));
			pMonster->Set_Player(m_pPlayer);

			ZeroMemory(&MonsterDesc, sizeof(CMonster::MONSTEROPTION));
			PivotMatrix.r[3] = XMVectorSet(-62.4f, -0.9f, 79.65f, 1.f);
			MonsterDesc.m_bFirstSpawnType[CMonster::STATE_ALREADYSPAWN] = true;
			MonsterDesc.MonsterDesc.m_iHP = MonsterDesc.MonsterDesc.m_iMaxHP = 300;
			MonsterDesc.MonsterDesc.m_iDamage = 15;
			MonsterDesc.MonsterDesc.m_iShield = MonsterDesc.MonsterDesc.m_iMaxShield = 0;
			MonsterDesc.MonsterDesc.TransformDesc.fSpeedPerSec = 2.5f;
			MonsterDesc.MonsterDesc.TransformDesc.fRotationPerSec = XMConvertToRadians(180.f);
			MonsterDesc.m_iCellIndex = 449;
			pMonster = dynamic_cast<CMonster*>(m_pGameInstance->Clone_GameObjectReturnPtr_M(LEVEL_GAMEPLAY, L"Layer_Monster", L"Prototype_GameObject_Normal_Human_Bow", PivotMatrix, &MonsterDesc));
			pMonster->Set_Player(m_pPlayer);

			ZeroMemory(&MonsterDesc, sizeof(CMonster::MONSTEROPTION));
			PivotMatrix.r[3] = XMVectorSet(-63.2f, 0.95f, 99.3f, 1.f);
			MonsterDesc.m_bFirstSpawnType[CMonster::STATE_ALREADYSPAWN] = true;
			MonsterDesc.MonsterDesc.m_iHP = MonsterDesc.MonsterDesc.m_iMaxHP = 300;
			MonsterDesc.MonsterDesc.m_iDamage = 15;
			MonsterDesc.MonsterDesc.m_iShield = MonsterDesc.MonsterDesc.m_iMaxShield = 0;
			MonsterDesc.MonsterDesc.TransformDesc.fSpeedPerSec = 2.5f;
			MonsterDesc.MonsterDesc.TransformDesc.fRotationPerSec = XMConvertToRadians(180.f);
			MonsterDesc.m_iCellIndex = 466;
			pMonster = dynamic_cast<CMonster*>(m_pGameInstance->Clone_GameObjectReturnPtr_M(LEVEL_GAMEPLAY, L"Layer_Monster", L"Prototype_GameObject_Normal_Human_Bow", PivotMatrix, &MonsterDesc));
			pMonster->Set_Player(m_pPlayer);

			ZeroMemory(&MonsterDesc, sizeof(CMonster::MONSTEROPTION));
			PivotMatrix.r[3] = XMVectorSet(-51.5f, -0.94f, 85.2f, 1.f);
			MonsterDesc.m_bFirstSpawnType[CMonster::STATE_ALREADYSPAWN] = true;			// No Detected == Patrol
			MonsterDesc.MonsterDesc.m_iHP = MonsterDesc.MonsterDesc.m_iMaxHP = 300;
			MonsterDesc.MonsterDesc.m_iDamage = 15;
			MonsterDesc.MonsterDesc.m_iShield = MonsterDesc.MonsterDesc.m_iMaxShield = 0;
			MonsterDesc.MonsterDesc.TransformDesc.fSpeedPerSec = 2.5f;
			MonsterDesc.MonsterDesc.TransformDesc.fRotationPerSec = XMConvertToRadians(180.f);
			MonsterDesc.m_iCellIndex = 452;
			pMonster = dynamic_cast<CMonster*>(m_pGameInstance->Clone_GameObjectReturnPtr_M(LEVEL_GAMEPLAY, L"Layer_Monster", L"Prototype_GameObject_Normal_Human_Granade", PivotMatrix, &MonsterDesc));
			pMonster->Set_Player(m_pPlayer);

			ZeroMemory(&MonsterDesc, sizeof(CMonster::MONSTEROPTION));
			PivotMatrix.r[3] = XMVectorSet(-70.f, 1.49f, 102.3f, 1.f);
			MonsterDesc.m_bFirstSpawnType[CMonster::STATE_ALREADYSPAWN] = true;			// No Detected == Patrol
			MonsterDesc.MonsterDesc.m_iHP = MonsterDesc.MonsterDesc.m_iMaxHP = 300;
			MonsterDesc.MonsterDesc.m_iDamage = 15;
			MonsterDesc.MonsterDesc.m_iShield = MonsterDesc.MonsterDesc.m_iMaxShield = 0;
			MonsterDesc.MonsterDesc.TransformDesc.fSpeedPerSec = 2.5f;
			MonsterDesc.MonsterDesc.TransformDesc.fRotationPerSec = XMConvertToRadians(180.f);
			MonsterDesc.m_iCellIndex = 480;
			pMonster = dynamic_cast<CMonster*>(m_pGameInstance->Clone_GameObjectReturnPtr_M(LEVEL_GAMEPLAY, L"Layer_Monster", L"Prototype_GameObject_Normal_Human_Granade", PivotMatrix, &MonsterDesc));
			pMonster->Set_Player(m_pPlayer);

			ZeroMemory(&MonsterDesc, sizeof(CMonster::MONSTEROPTION));
			PivotMatrix.r[3] = XMVectorSet(-70.7f, 1.49f, 102.3f, 1.f);
			MonsterDesc.m_bFirstSpawnType[CMonster::STATE_NODETECTED] = true;
			MonsterDesc.MonsterDesc.m_iHP = MonsterDesc.MonsterDesc.m_iMaxHP = 500;
			MonsterDesc.MonsterDesc.m_iDamage = 15;
			MonsterDesc.MonsterDesc.m_iShield = MonsterDesc.MonsterDesc.m_iMaxShield = 0;
			MonsterDesc.MonsterDesc.TransformDesc.fSpeedPerSec = 2.5f;
			MonsterDesc.MonsterDesc.TransformDesc.fRotationPerSec = XMConvertToRadians(180.f);
			MonsterDesc.m_iCellIndex = 479;
			pMonster = dynamic_cast<CMonster*>(m_pGameInstance->Clone_GameObjectReturnPtr_M(LEVEL_GAMEPLAY, L"Layer_Monster", L"Prototype_GameObject_Normal_Elite_Bug", PivotMatrix, &MonsterDesc));
			pMonster->Set_Player(m_pPlayer);
#pragma endregion

			m_bSpawnTrigger = true;

		}
		return S_OK;
	}
	else
	{
		m_pPlayer->Set_PortalUI(false);
	}

	return S_OK;
}

HRESULT CTrigger::Bug_To_Room_C()
{
	CCollider* pCollider = (CCollider*)m_pPlayer->Find_Component(L"Com_BODYSPHERE");
	NULL_CHECK_RETURN(pCollider, E_FAIL);
	if (m_pTriggerCollCom->Collision(pCollider))
	{
		m_pPlayer->Set_PortalUI(true);

		if (m_pGameInstance->Key_Down(DIK_F))
		{
			// CGameObject* pGameObject = nullptr;
			// CGameObject::GAMEOBJECTDESC	GameObjectDesc;
			// GameObjectDesc.m_iCountType = 5;
			// pGameObject = m_pGameInstance->Clone_GameObjectReturnPtr(m_pGameInstance->Get_StaticLevelIndex(), L"Layer_Logo", L"Prototype_GameObject_BackGround", &GameObjectDesc);
			// NULL_CHECK_RETURN(pGameObject, E_FAIL);

			m_pGameInstance->Delete_Light(4);
			m_pGameInstance->Delete_Light(5);
			m_pGameInstance->Delete_Light(8);
			m_pGameInstance->Delete_Light(9);
			m_pGameInstance->Delete_Light(10);

			LIGHTDESC			LightDesc;
			CGameObject::GAMEOBJECTDESC	tFireLightDesc;
			//Num2
			_bool& LightBool = m_pGameInstance->Set_LightEnable(2);
			m_pGameInstance->Set_LightPos(2, XMVectorSet(-37.9f, 1.36f, 62.449f, 1.f));

			LightBool = true;
			ZeroMemory(&tFireLightDesc, sizeof(CGameObject::GAMEOBJECTDESC));
			tFireLightDesc.TransformDesc.vInitPos = _float3(-37.9f, 1.36f, 62.449f);
			tFireLightDesc.m_iNumber = 2;
			tFireLightDesc.m_vTexSize = _float2(1.f, 2.f);
			FAILED_CHECK_RETURN(m_pGameInstance->Clone_GameObject(LEVEL_GAMEPLAY, L"Layer_FireLight", L"Prototype_GameObject_Effect_Fire_Light", &tFireLightDesc), E_FAIL);

			// Num3
			_bool& LightBool3 = m_pGameInstance->Set_LightEnable(3);
			m_pGameInstance->Set_LightPos(3, XMVectorSet(-30.824f, -0.213f, 95.397f, 1.f));

			LightBool3 = true;
			ZeroMemory(&tFireLightDesc, sizeof(CGameObject::GAMEOBJECTDESC));
			tFireLightDesc.TransformDesc.vInitPos = _float3(-30.824f, -0.213f, 95.397f);
			tFireLightDesc.m_iNumber = 3;
			tFireLightDesc.m_vTexSize = _float2(1.f, 2.f);
			FAILED_CHECK_RETURN(m_pGameInstance->Clone_GameObject(LEVEL_GAMEPLAY, L"Layer_FireLight", L"Prototype_GameObject_Effect_Fire_Light", &tFireLightDesc), E_FAIL);

			//Num11
			ZeroMemory(&LightDesc, sizeof LightDesc);

			LightDesc.eType = LIGHTDESC::LIGHT_POINT;
			LightDesc.isEnable = true;


			LightDesc.vPosition = _float4(-29.974f, -0.5f, 77.489f, 1.f);
			LightDesc.fRange = 30.0f;
			LightDesc.vDiffuse = _float4(0.6f, 0.87f, 0.9f, 1.f);
			LightDesc.vAmbient = _float4(0.2f, 0.2f, 0.2f, 0.2f);
			LightDesc.vSpecular = _float4(0.1f, 0.1f, 0.1f, 1.f);
			FAILED_CHECK_RETURN(m_pGameInstance->Add_Light(m_pDevice, m_pContext, LightDesc), E_FAIL);

			// _bool& LightBool4 = m_pGameInstance->Set_LightEnable(4);
			// m_pGameInstance->Set_LightPos(4, XMVectorSet(-29.974f, -0.5f, 77.489f, 1.f));
			// m_pGameInstance->Set_LightRange(4, 50.f);
			// LightBool4 = true;


#pragma region TriggerSetting

			m_pPlayer->Set_RoomType(CPlayer::ROOM_C);
			_matrix PlayerMat = m_pPlayer->Get_WorldFloat4x4();
			PlayerMat.r[3] = XMVectorSet( -32.028f, 0.03f, 62.767f, 1.f);
			m_pPlayer->Set_CellIndex(279);
			m_pPlayer->Set_WorldMatrix(PlayerMat);

			CMonster::MONSTEROPTION MonsterDesc;
			CMonster* pMonster = nullptr;
			_matrix PivotMatrix = XMMatrixIdentity();

			ZeroMemory(&MonsterDesc, sizeof(CMonster::MONSTEROPTION));
			PivotMatrix.r[3] = XMVectorSet(-44.7f, 0.f, 72.2f, 1.f);
			MonsterDesc.m_bFirstSpawnType[CMonster::STATE_ALREADYSPAWN] = true;
			MonsterDesc.MonsterDesc.m_iHP = MonsterDesc.MonsterDesc.m_iMaxHP = 300;
			MonsterDesc.MonsterDesc.m_iDamage = 15;
			MonsterDesc.MonsterDesc.m_iShield = MonsterDesc.MonsterDesc.m_iMaxShield = 0;
			MonsterDesc.MonsterDesc.TransformDesc.fSpeedPerSec = 5.f;
			MonsterDesc.m_iCellIndex = 294;
			pMonster = dynamic_cast<CMonster*>(m_pGameInstance->Clone_GameObjectReturnPtr_M(LEVEL_GAMEPLAY, L"Layer_Monster", L"Prototype_GameObject_Normal_Human_Sword", PivotMatrix, &MonsterDesc));
			pMonster->Set_Player(m_pPlayer);

			ZeroMemory(&MonsterDesc, sizeof(CMonster::MONSTEROPTION));
			PivotMatrix.r[3] = XMVectorSet(-37.7f, -1.4f, 80.7f, 1.f);
			MonsterDesc.m_bFirstSpawnType[CMonster::STATE_ALREADYSPAWN] = true;
			MonsterDesc.MonsterDesc.m_iHP = MonsterDesc.MonsterDesc.m_iMaxHP = 300;
			MonsterDesc.MonsterDesc.m_iDamage = 15;
			MonsterDesc.MonsterDesc.m_iShield = MonsterDesc.MonsterDesc.m_iMaxShield = 0;
			MonsterDesc.MonsterDesc.TransformDesc.fSpeedPerSec = 5.f;
			MonsterDesc.m_iCellIndex = 323;
			pMonster = dynamic_cast<CMonster*>(m_pGameInstance->Clone_GameObjectReturnPtr_M(LEVEL_GAMEPLAY, L"Layer_Monster", L"Prototype_GameObject_Normal_Human_Spear", PivotMatrix, &MonsterDesc));
			pMonster->Set_Player(m_pPlayer);

			ZeroMemory(&MonsterDesc, sizeof(CMonster::MONSTEROPTION));
			PivotMatrix.r[3] = XMVectorSet(-21.6f, 0.f, 65.7f, 1.f);
			MonsterDesc.m_bFirstSpawnType[CMonster::STATE_ALREADYSPAWN] = true;
			MonsterDesc.MonsterDesc.m_iHP = MonsterDesc.MonsterDesc.m_iMaxHP = 300;
			MonsterDesc.MonsterDesc.m_iDamage = 15;
			MonsterDesc.MonsterDesc.m_iShield = MonsterDesc.MonsterDesc.m_iMaxShield = 0;
			MonsterDesc.MonsterDesc.TransformDesc.fSpeedPerSec = 2.5f;
			MonsterDesc.MonsterDesc.TransformDesc.fRotationPerSec = XMConvertToRadians(180.f);
			MonsterDesc.m_iCellIndex = 282;
			pMonster = dynamic_cast<CMonster*>(m_pGameInstance->Clone_GameObjectReturnPtr_M(LEVEL_GAMEPLAY, L"Layer_Monster", L"Prototype_GameObject_Normal_Human_Bow", PivotMatrix, &MonsterDesc));
			pMonster->Set_Player(m_pPlayer);

			ZeroMemory(&MonsterDesc, sizeof(CMonster::MONSTEROPTION));
			PivotMatrix.r[3] = XMVectorSet(-45.3f, 0.f, 66.4f, 1.f);
			MonsterDesc.m_bFirstSpawnType[CMonster::STATE_ALREADYSPAWN] = true;
			MonsterDesc.MonsterDesc.m_iHP = MonsterDesc.MonsterDesc.m_iMaxHP = 300;
			MonsterDesc.MonsterDesc.m_iDamage = 15;
			MonsterDesc.MonsterDesc.m_iShield = MonsterDesc.MonsterDesc.m_iMaxShield = 0;
			MonsterDesc.MonsterDesc.TransformDesc.fSpeedPerSec = 2.5f;
			MonsterDesc.MonsterDesc.TransformDesc.fRotationPerSec = XMConvertToRadians(180.f);
			MonsterDesc.m_iCellIndex = 291;
			pMonster = dynamic_cast<CMonster*>(m_pGameInstance->Clone_GameObjectReturnPtr_M(LEVEL_GAMEPLAY, L"Layer_Monster", L"Prototype_GameObject_Normal_Human_Bow", PivotMatrix, &MonsterDesc));
			pMonster->Set_Player(m_pPlayer);

			ZeroMemory(&MonsterDesc, sizeof(CMonster::MONSTEROPTION));
			PivotMatrix.r[3] = XMVectorSet(-29.8f, -1.4f, 81.1f, 1.f);
			MonsterDesc.m_bFirstSpawnType[CMonster::STATE_ALREADYSPAWN] = true;
			MonsterDesc.MonsterDesc.m_iHP = MonsterDesc.MonsterDesc.m_iMaxHP = 100;
			MonsterDesc.MonsterDesc.m_iDamage = 15;
			MonsterDesc.MonsterDesc.m_iShield = MonsterDesc.MonsterDesc.m_iMaxShield = 0;
			MonsterDesc.MonsterDesc.TransformDesc.fSpeedPerSec = 2.5f;
			MonsterDesc.MonsterDesc.TransformDesc.fRotationPerSec = XMConvertToRadians(180.f);
			MonsterDesc.m_iCellIndex = 328;
			pMonster = dynamic_cast<CMonster*>(m_pGameInstance->Clone_GameObjectReturnPtr_M(LEVEL_GAMEPLAY, L"Layer_Monster", L"Prototype_GameObject_Normal_Human_Explode", PivotMatrix, &MonsterDesc));
			pMonster->Set_Player(m_pPlayer);

			ZeroMemory(&MonsterDesc, sizeof(CMonster::MONSTEROPTION));
			PivotMatrix.r[3] = XMVectorSet(-22.85f, -1.4f, 79.5f, 1.f);
			MonsterDesc.m_bFirstSpawnType[CMonster::STATE_NODETECTED] = true;
			MonsterDesc.MonsterDesc.m_iHP = MonsterDesc.MonsterDesc.m_iMaxHP = 100;
			MonsterDesc.MonsterDesc.m_iDamage = 15;
			MonsterDesc.MonsterDesc.m_iShield = MonsterDesc.MonsterDesc.m_iMaxShield = 0;
			MonsterDesc.MonsterDesc.TransformDesc.fSpeedPerSec = 1.5f;
			MonsterDesc.MonsterDesc.TransformDesc.fRotationPerSec = XMConvertToRadians(180.f);
			MonsterDesc.m_iCellIndex = 313;
			pMonster = dynamic_cast<CMonster*>(m_pGameInstance->Clone_GameObjectReturnPtr_M(LEVEL_GAMEPLAY, L"Layer_Monster", L"Prototype_GameObject_Normal_Little_Bug", PivotMatrix, &MonsterDesc));
			pMonster->Set_Player(m_pPlayer);

			PivotMatrix.r[3] = XMVectorSet(-20.6f, -1.4f, 79.5f, 1.f);
			MonsterDesc.m_bFirstSpawnType[CMonster::STATE_NODETECTED] = true;
			MonsterDesc.MonsterDesc.m_iHP = MonsterDesc.MonsterDesc.m_iMaxHP = 100;
			MonsterDesc.MonsterDesc.m_iDamage = 15;
			MonsterDesc.MonsterDesc.m_iShield = MonsterDesc.MonsterDesc.m_iMaxShield = 0;
			MonsterDesc.MonsterDesc.TransformDesc.fSpeedPerSec = 1.5f;
			MonsterDesc.MonsterDesc.TransformDesc.fRotationPerSec = XMConvertToRadians(180.f);
			MonsterDesc.m_iCellIndex = 313;
			pMonster = dynamic_cast<CMonster*>(m_pGameInstance->Clone_GameObjectReturnPtr_M(LEVEL_GAMEPLAY, L"Layer_Monster", L"Prototype_GameObject_Normal_Little_Bug", PivotMatrix, &MonsterDesc));
			pMonster->Set_Player(m_pPlayer);
#pragma endregion
			m_bSpawnTrigger = true;

		}
		return S_OK;
	}
	else
	{
		m_pPlayer->Set_PortalUI(false);
	}

	return S_OK;
}

// Normal_Boss
// m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(113.1f, 0.104f, 63.115f, 1.f));
// m_pNavigationCom->Set_CellIndex(504);
HRESULT CTrigger::Room_C_To_Boss()
{
	CCollider* pCollider = (CCollider*)m_pPlayer->Find_Component(L"Com_BODYSPHERE");
	NULL_CHECK_RETURN(pCollider, E_FAIL);
	if (m_pTriggerCollCom->Collision(pCollider))
	{
		m_pPlayer->Set_PortalUI(true);

		if (m_pGameInstance->Key_Down(DIK_F))
		{
			// CGameObject* pGameObject = nullptr;
			// CGameObject::GAMEOBJECTDESC	GameObjectDesc;
			// GameObjectDesc.m_iCountType = 6;
			// pGameObject = m_pGameInstance->Clone_GameObjectReturnPtr(m_pGameInstance->Get_StaticLevelIndex(), L"Layer_Logo", L"Prototype_GameObject_BackGround", &GameObjectDesc);
			// NULL_CHECK_RETURN(pGameObject, E_FAIL);

			m_pGameInstance->Delete_Light(2);
			m_pGameInstance->Delete_Light(3);
			m_pGameInstance->Delete_Light(11);

			LIGHTDESC			LightDesc;
			CGameObject::GAMEOBJECTDESC	tFireLightDesc;

			//Num2
			_bool& LightBool = m_pGameInstance->Set_LightEnable(2);
			m_pGameInstance->Set_LightPos(2, XMVectorSet(104.693f, 1.3f, 73.738f, 1.f));

			LightBool = true;
			ZeroMemory(&tFireLightDesc, sizeof(CGameObject::GAMEOBJECTDESC));
			tFireLightDesc.TransformDesc.vInitPos = _float3(104.693f, 1.3f, 73.738f);
			tFireLightDesc.m_iNumber = 2;
			tFireLightDesc.m_vTexSize = _float2(1.f, 2.f);
			FAILED_CHECK_RETURN(m_pGameInstance->Clone_GameObject(LEVEL_GAMEPLAY, L"Layer_FireLight", L"Prototype_GameObject_Effect_Fire_Light", &tFireLightDesc), E_FAIL);

			// Num3
			_bool& LightBool3 = m_pGameInstance->Set_LightEnable(3);
			m_pGameInstance->Set_LightPos(3, XMVectorSet(104.720f, 1.3f, 84.802f, 1.f));

			LightBool3 = true;
			ZeroMemory(&tFireLightDesc, sizeof(CGameObject::GAMEOBJECTDESC));
			tFireLightDesc.TransformDesc.vInitPos = _float3(104.720f, 1.3f, 84.802f);
			tFireLightDesc.m_iNumber = 3;
			tFireLightDesc.m_vTexSize = _float2(1.f, 2.f);
			FAILED_CHECK_RETURN(m_pGameInstance->Clone_GameObject(LEVEL_GAMEPLAY, L"Layer_FireLight", L"Prototype_GameObject_Effect_Fire_Light", &tFireLightDesc), E_FAIL);

			// Num 6
			_bool& LightBool6 = m_pGameInstance->Set_LightEnable(6);
			m_pGameInstance->Set_LightPos(6, XMVectorSet(121.618f, 1.3f, 84.842f, 1.f));

			LightBool6 = true;
			ZeroMemory(&tFireLightDesc, sizeof(CGameObject::GAMEOBJECTDESC));
			tFireLightDesc.TransformDesc.vInitPos = _float3(121.618f, 1.3f, 84.842f);
			tFireLightDesc.m_iNumber = 6;
			tFireLightDesc.m_vTexSize = _float2(1.f, 2.f);
			FAILED_CHECK_RETURN(m_pGameInstance->Clone_GameObject(LEVEL_GAMEPLAY, L"Layer_FireLight", L"Prototype_GameObject_Effect_Fire_Light", &tFireLightDesc), E_FAIL);

			// Num7
			_bool& LightBool7 = m_pGameInstance->Set_LightEnable(7);
			m_pGameInstance->Set_LightPos(7, XMVectorSet(121.628f, 1.3f, 73.727f, 1.f));

			LightBool7 = true;
			ZeroMemory(&tFireLightDesc, sizeof(CGameObject::GAMEOBJECTDESC));
			tFireLightDesc.TransformDesc.vInitPos = _float3(121.628f, 1.3f, 73.727f);
			tFireLightDesc.m_iNumber = 7;
			tFireLightDesc.m_vTexSize = _float2(1.f, 2.f);
			FAILED_CHECK_RETURN(m_pGameInstance->Clone_GameObject(LEVEL_GAMEPLAY, L"Layer_FireLight", L"Prototype_GameObject_Effect_Fire_Light", &tFireLightDesc), E_FAIL);

			// Blue Light

			// Num 4
			_bool& LightBool4 = m_pGameInstance->Set_LightEnable(4);
			m_pGameInstance->Set_LightPos(4, XMVectorSet(116.673f, 4.945f, 61.882f, 1.f));

			LightBool4 = true;
			ZeroMemory(&tFireLightDesc, sizeof(CGameObject::GAMEOBJECTDESC));
			tFireLightDesc.TransformDesc.vInitPos = _float3(116.673f, 4.945f, 61.882f);
			tFireLightDesc.m_iNumber = 4;
			tFireLightDesc.m_vTexSize = _float2(1.f, 2.f);
			for (_uint i = 0; i<15; ++i)
				FAILED_CHECK_RETURN(m_pGameInstance->Clone_GameObject(LEVEL_GAMEPLAY, L"Layer_FireLight", L"Prototype_GameObject_BlueLight", &tFireLightDesc), E_FAIL);

			// Num 5
			_bool& BlueLightBool5 = m_pGameInstance->Set_LightEnable(5);
			m_pGameInstance->Set_LightPos(5, XMVectorSet(109.6f, 4.945f, 61.882f, 1.f));
			m_pGameInstance->Set_LightRange(5, 10.f);
			BlueLightBool5 = true;

			ZeroMemory(&tFireLightDesc, sizeof(CGameObject::GAMEOBJECTDESC));
			tFireLightDesc.TransformDesc.vInitPos = _float3(109.6f, 4.945f, 61.882f);
			tFireLightDesc.m_iNumber = 5;
			tFireLightDesc.m_vTexSize = _float2(1.f, 2.f);
			for (_uint i = 0; i<15; ++i)
				FAILED_CHECK_RETURN(m_pGameInstance->Clone_GameObject(LEVEL_GAMEPLAY, L"Layer_FireLight", L"Prototype_GameObject_BlueLight", &tFireLightDesc), E_FAIL);

			// Num 9
			_bool& LightBool9 = m_pGameInstance->Set_LightEnable(9);
			m_pGameInstance->Set_LightPos(9, XMVectorSet(109.034f, 6.74f, 96.769f, 1.f));

			LightBool9 = true;
			ZeroMemory(&tFireLightDesc, sizeof(CGameObject::GAMEOBJECTDESC));
			tFireLightDesc.TransformDesc.vInitPos = _float3(109.034f, 6.74f, 96.769f);
			tFireLightDesc.m_iNumber = 9;
			tFireLightDesc.m_vTexSize = _float2(1.f, 2.f);
			for (_uint i = 0; i<15; ++i)
				FAILED_CHECK_RETURN(m_pGameInstance->Clone_GameObject(LEVEL_GAMEPLAY, L"Layer_FireLight", L"Prototype_GameObject_BlueLight", &tFireLightDesc), E_FAIL);

			// Num 10
			_bool& BlueLightBool10 = m_pGameInstance->Set_LightEnable(10);
			m_pGameInstance->Set_LightPos(10, XMVectorSet(117.173f, 6.74f, 96.741f, 1.f));
			m_pGameInstance->Set_LightRange(10, 10.f);
			BlueLightBool10 = true;

			ZeroMemory(&tFireLightDesc, sizeof(CGameObject::GAMEOBJECTDESC));
			tFireLightDesc.TransformDesc.vInitPos = _float3(117.173f, 6.74f, 96.741f);
			tFireLightDesc.m_iNumber = 10;
			tFireLightDesc.m_vTexSize = _float2(1.f, 2.f);
			for (_uint i = 0; i<15; ++i)
				FAILED_CHECK_RETURN(m_pGameInstance->Clone_GameObject(LEVEL_GAMEPLAY, L"Layer_FireLight", L"Prototype_GameObject_BlueLight", &tFireLightDesc), E_FAIL);




			m_pGameInstance->Get_CloneObjectList(LEVEL_GAMEPLAY, L"Layer_BackGround")->front()->Set_Dead(true);

			FAILED_CHECK_RETURN(m_pGameInstance->Clone_GameObject(LEVEL_GAMEPLAY, L"Layer_BackGround", L"Prototype_GameObject_Normal_BossMap"), E_FAIL);
			m_pGameInstance->Set_LightRange(0, 100.f);
			_float4 vDirection;
			_matrix	RotationMatrix = XMMatrixRotationAxis(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(-221.33f));
			XMStoreFloat4(&vDirection, XMVector3TransformNormal(XMVectorSet(-cosf(XMConvertToRadians(60.f)), -sinf(XMConvertToRadians(60.f)), 0.f, 0.f), RotationMatrix));

			m_pGameInstance->Set_LightDirection(1, vDirection);

			m_pPlayer->Set_RoomType(CPlayer::ROOM_BOSS);
			_matrix PlayerMat = m_pPlayer->Get_WorldFloat4x4();
			PlayerMat.r[3] = XMVectorSet(113.1f, 0.104f, 63.115f, 1.f);
			m_pPlayer->Set_CellIndex(504);
			m_pPlayer->Set_WorldMatrix(PlayerMat);

			CMonster::MONSTEROPTION MonsterDesc;
			CMonster* pMonster = nullptr;
			_matrix PivotMatrix = XMMatrixIdentity() * XMMatrixRotationAxis(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(180.f));
			PivotMatrix.r[3] = XMVectorSet(110.f, 1.3f, 179.f, 1.f);
			MonsterDesc.m_bFirstSpawnType[CMonster::STATE_NODETECTED] = true;
			MonsterDesc.MonsterDesc.m_iHP = MonsterDesc.MonsterDesc.m_iMaxHP = 1000;
			MonsterDesc.MonsterDesc.m_iDamage = 15;
			MonsterDesc.MonsterDesc.m_iShield = MonsterDesc.MonsterDesc.m_iMaxShield = 0;
			MonsterDesc.MonsterDesc.TransformDesc.fSpeedPerSec = 5.f;
			MonsterDesc.m_iCellIndex = 0;
			pMonster = dynamic_cast<CMonster*>(m_pGameInstance->Clone_GameObjectReturnPtr_M(LEVEL_GAMEPLAY, L"Layer_ZBossMonster", L"Prototype_GameObject_Normal_Boss", PivotMatrix, &MonsterDesc));
			pMonster->Set_Player(m_pPlayer);

			CGameObject::GAMEOBJECTDESC	EffectDesc;
			ZeroMemory(&EffectDesc, sizeof(CGameObject::GAMEOBJECTDESC));
			EffectDesc.TransformDesc.vInitPos = _float3(110.f, 1.3f, 163.f);
			EffectDesc.m_vTexSize = _float2(105.8f, 12.1f);
			EffectDesc.m_iHP = 1;
			EffectDesc.TransformDesc.fRotationPerSec = 0.f;
			FAILED_CHECK_RETURN(m_pGameInstance->Clone_GameObject(LEVEL_GAMEPLAY, L"Layer_BossSmoke", L"Prototype_GameObject_Effect_Smoke", &EffectDesc), E_FAIL);

			ZeroMemory(&EffectDesc, sizeof(CGameObject::GAMEOBJECTDESC));
			EffectDesc.TransformDesc.vInitPos = _float3(110.f, 1.3f, 161.f);
			EffectDesc.m_vTexSize = _float2(105.8f, 11.1f);
			EffectDesc.m_iHP = 1;
			EffectDesc.TransformDesc.fRotationPerSec = 0.f;
			FAILED_CHECK_RETURN(m_pGameInstance->Clone_GameObject(LEVEL_GAMEPLAY, L"Layer_BossSmoke", L"Prototype_GameObject_Effect_Smoke", &EffectDesc), E_FAIL);

			ZeroMemory(&EffectDesc, sizeof(CGameObject::GAMEOBJECTDESC));
			EffectDesc.TransformDesc.vInitPos = _float3(110.f, 1.3f, 159.f);
			EffectDesc.m_vTexSize = _float2(105.8f, 10.1f);
			EffectDesc.m_iHP = 1;
			EffectDesc.TransformDesc.fRotationPerSec = 0.f;
			FAILED_CHECK_RETURN(m_pGameInstance->Clone_GameObject(LEVEL_GAMEPLAY, L"Layer_BossSmoke", L"Prototype_GameObject_Effect_Smoke", &EffectDesc), E_FAIL);

		}
		return S_OK;
	}
	else
	{
		m_pPlayer->Set_PortalUI(false);
	}

	return S_OK;
}

HRESULT CTrigger::SetUp_Components()
{
	FAILED_CHECK_RETURN(__super::Add_Component(CGameInstance::Get_StaticLevelIndex(), L"Prototype_Component_Renderer", L"Com_Renderer", (CComponent**)&m_pRendererCom, this), E_FAIL);

	CCollider::COLLIDERDESC			ColliderDesc;
	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));
	ColliderDesc.vSize = _float3(1.5f, 1.5f, 1.5f);
	ColliderDesc.vPosition = _float3(0.f, 0.f, 0.f);
	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Collider_SPHERE", L"Com_ExplodeSPHERE", (CComponent**)&m_pTriggerCollCom, this, &ColliderDesc), E_FAIL);

	FAILED_CHECK_RETURN(__super::Add_Component(CGameInstance::Get_StaticLevelIndex(), L"Prototype_Component_Shader_VtxTex", L"Com_Shader", (CComponent**)&m_pShaderCom, this), E_FAIL);

	FAILED_CHECK_RETURN(__super::Add_Component(CGameInstance::Get_StaticLevelIndex(), L"Prototype_Component_VIBuffer_Rect", L"Com_VIBuffer", (CComponent**)&m_pVIBufferCom, this), E_FAIL);

	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_PortalTex", L"Com_Texture", (CComponent**)&m_pTextureCom, this), E_FAIL);
	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_PortalDistortionTex", L"Com_DistortionTexture", (CComponent**)&m_pDistortionTexCom, this), E_FAIL);
	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_PortalCircleTex", L"Com_CircleTexture", (CComponent**)&m_pCircleTexCom, this), E_FAIL);


	return S_OK;
}

HRESULT CTrigger::SetUp_ShaderResources()
{
	NULL_CHECK_RETURN(m_pShaderCom, E_FAIL);

	FAILED_CHECK_RETURN(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, L"g_WorldMatrix"), E_FAIL);
	FAILED_CHECK_RETURN(m_pShaderCom->Set_Matrix(L"g_ViewMatrix", &m_pGameInstance->Get_TransformFloat4x4(CPipeLine::D3DTS_VIEW)), E_FAIL);
	FAILED_CHECK_RETURN(m_pShaderCom->Set_Matrix(L"g_ProjMatrix", &m_pGameInstance->Get_TransformFloat4x4(CPipeLine::D3DTS_PROJ)), E_FAIL);
	FAILED_CHECK_RETURN(m_pShaderCom->Set_ShaderResourceView(L"g_DepthTexture", m_pGameInstance->Get_DepthTargetSRV()), E_FAIL);
	// FAILED_CHECK_RETURN(m_pShaderCom->Set_RawValue(L"g_fAlpha", &m_fMaxMoveDistance, sizeof(_float)), E_FAIL);
	// FAILED_CHECK_RETURN(m_pShaderCom->Set_ShaderResourceView(L"g_Texture", CGameInstance::GetInstance()->Get_DiffuseTargetSRV()), E_FAIL);
	FAILED_CHECK_RETURN(m_pShaderCom->Set_RawValue(L"g_DisTime", &m_fTime, sizeof(_float)), E_FAIL);
	FAILED_CHECK_RETURN(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, L"g_Texture", m_iTextureIndex), E_FAIL);
	FAILED_CHECK_RETURN(m_pCircleTexCom->Bind_ShaderResource(m_pShaderCom, L"g_CircleTexture"), E_FAIL);

	FAILED_CHECK_RETURN(m_pDistortionTexCom->Bind_ShaderResource(m_pShaderCom, L"g_DistortionTexture"), E_FAIL);

	return S_OK;
}

CTrigger* CTrigger::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CTrigger*		pInstance = new CTrigger(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CTrigger");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CTrigger::Clone(const wstring& wstrPrototypeTag, void* pArg)
{
	CTrigger*		pInstance = new CTrigger(*this);

	if (FAILED(pInstance->Initialize_Clone(wstrPrototypeTag, pArg)))
	{
		MSG_BOX("Failed to Created : CTrigger");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CTrigger::Free()
{
	if (m_tTriggerOption.m_eType == TRIGGER_A_TO_KNIGHT)
	{
		
	}
	else if (m_tTriggerOption.m_eType == TRIGGER_KNIGHT_TO_B)
	{
		
	}
	else if(m_tTriggerOption.m_eType == TRIGGER_B_TO_BUG)
	{
		
	}
	else if(m_tTriggerOption.m_eType == TRIGGER_BUG_TO_C)
	{
		
	}
	else if(m_tTriggerOption.m_eType == TRIGGER_C_TO_BOSS)
	{
		
	}
	
	__super::Free();
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTriggerCollCom);
	Safe_Release(m_pDistortionTexCom);
	Safe_Release(m_pCircleTexCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);
}
