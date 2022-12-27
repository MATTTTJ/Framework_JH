#include "stdafx.h"
#include "..\public\Player.h"
#include "GameInstance.h"
#include "Weapon.h"
#include "Bone.h"
#include "Static_Camera.h"
#include "State.h"

CPlayer::CPlayer(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CGameObject(pDevice, pContext)
{
	
}

CPlayer::CPlayer(const CPlayer & rhs)
	: CGameObject(rhs)
{
	m_tWeaponDesc[WEAPON_DEFAULT].m_wstrWeaponName = L"WEAPON_DEFAULT";
	m_tWeaponDesc[WEAPON_FLAMEBULLET].m_wstrWeaponName = L"WEAPON_FLAMEBULLET";
	m_tWeaponDesc[WEAPON_FIREDRAGON].m_wstrWeaponName = L"WEAPON_FIREDRAGON";
	m_tWeaponDesc[WEAPON_POISON].m_wstrWeaponName = L"WEAPON_POISON";
}

_matrix CPlayer::Get_BoneMatrix(const string& strBoneName)
{
	if (nullptr == m_pModelCom)
		return XMMatrixIdentity();

	return m_pModelCom->Get_BoneMatrix(strBoneName);
}

_matrix CPlayer::Get_OffsetMatrix(const string& strBoneName)
{
	if (nullptr == m_pModelCom)
		return XMMatrixIdentity();

	return m_pModelCom->Get_OffsetMatrix(strBoneName);
}

_matrix CPlayer::Get_PivotMatrix()
{
	if (nullptr == m_pModelCom)
		return XMMatrixIdentity();

	return m_pModelCom->Get_PivotMatrix();
}

_vector CPlayer::Get_TransformState(CTransform::STATE eState)
{
	if (m_pModelCom == nullptr)
		return XMVectorSet(0.f, 1.f, 0.f, 0.f);

	return m_pTransformCom->Get_State(eState);
}

HRESULT CPlayer::Initialize_Prototype()
{
	m_bHasModel = true;

	FAILED_CHECK_RETURN(__super::Initialize_Prototype(), E_FAIL);

	return S_OK;
}

HRESULT CPlayer::Initialize_Clone(const wstring& wstrPrototypeTag, void * pArg)
{
	CGameObject::GAMEOBJECTDESC		GameObjectDesc;
	ZeroMemory(&GameObjectDesc, sizeof(GAMEOBJECTDESC));

	GameObjectDesc.TransformDesc.fSpeedPerSec = 7.0;
	GameObjectDesc.TransformDesc.fRotationPerSec = XMConvertToRadians(90.f);

	FAILED_CHECK_RETURN(__super::Initialize_Clone(wstrPrototypeTag, &GameObjectDesc), E_FAIL);

	FAILED_CHECK_RETURN(SetUp_Components(), E_FAIL);
	FAILED_CHECK_RETURN(SetUp_State(), E_FAIL);

	// m_pModelCom->Set_CurAnimIndex(rand() % 20);
	// m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(-1.4452f, -0.42242f, -1.11702f, 1.f));
	// FAILED_CHECK_RETURN(Ready_Parts(), E_FAIL);

	// m_pModelCom->Set_CurAnimIndex(3);

	return S_OK;
}

void CPlayer::Tick(_double dTimeDelta)
{
	__super::Tick(dTimeDelta);

	if (nullptr != m_pState)
		m_pState->Tick(dTimeDelta);

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	_long		MouseMove = 0;

	if (pGameInstance->Get_DIKeyState(DIK_1))
	{
		// m_pModelCom = m_pWeaponModelCom[WEAPON_DEFAULT];
		m_pModelCom = m_tWeaponDesc[WEAPON_DEFAULT].m_pWeaponModelCom;
		m_wstrCurWeaponName = L"";
		m_wstrCurWeaponName = m_tWeaponDesc[WEAPON_DEFAULT].m_wstrWeaponName;
	}
	if (pGameInstance->Get_DIKeyState(DIK_2))
	{
		m_pModelCom = m_tWeaponDesc[WEAPON_FLAMEBULLET].m_pWeaponModelCom;
		m_wstrCurWeaponName = L"";
		m_wstrCurWeaponName = m_tWeaponDesc[WEAPON_FLAMEBULLET].m_wstrWeaponName;
	}
	if (pGameInstance->Get_DIKeyState(DIK_3))
	{
		m_pModelCom = m_tWeaponDesc[WEAPON_FIREDRAGON].m_pWeaponModelCom;
		m_wstrCurWeaponName = L"";
		m_wstrCurWeaponName = m_tWeaponDesc[WEAPON_FIREDRAGON].m_wstrWeaponName;
	}
	if (pGameInstance->Get_DIKeyState(DIK_4))
	{
		m_pModelCom = m_tWeaponDesc[WEAPON_POISON].m_pWeaponModelCom;
		m_wstrCurWeaponName = L"";
		m_wstrCurWeaponName = m_tWeaponDesc[WEAPON_POISON].m_wstrWeaponName;
	}
	// if (pGameInstance->Get_DIKeyState(DIK_DOWN))
	// {
	// 	m_pTransformCom->Go_Backward(dTimeDelta);
	// }
	//
	// if (pGameInstance->Get_DIKeyState(DIK_UP))
	// {
	// 	m_pTransformCom->Go_Straight(dTimeDelta);
	// }
	// if (pGameInstance->Get_DIKeyState(DIK_R))
	// {
	// 	m_pModelCom->Set_CurAnimIndex(1);
	// }
	// if (pGameInstance->Get_DIMouseState(DIM_LB) & 0x80)
	// {
	// 	if(m_wstrCurWeaponName == L"WEAPON_DEFAULT")
	// 	{
	// 		m_pModelCom->Set_CurAnimIndex(DEFAULT_PISTOL_FIRE);
	// 	}
	// 	else if(m_wstrCurWeaponName == L"WEAPON_FLAMEBULLET")
	// 	{
	// 		m_pModelCom->Set_CurAnimIndex(FLAME_BULLET_FIRE);
	// 	}
	// 	else if (m_wstrCurWeaponName == L"WEAPON_FIREDRAGON")
	// 	{
	// 		m_pModelCom->Set_CurAnimIndex(FIRE_DRAGON_FIRE);
	// 	}
	// 	else if (m_wstrCurWeaponName == L"WEAPON_POISON")
	// 	{
	// 		if (m_iPoisonAttCnt == 2)
	// 			m_iPoisonAttCnt = 0;
	//
	// 		m_pModelCom->Set_CurAnimIndex(POISON_FIRE_A + m_iPoisonAttCnt);
	// 		m_iPoisonAttCnt++;
	// 	}
	// }
	// else
	// {
	// 	if (m_wstrCurWeaponName == L"WEAPON_DEFAULT")
	// 	{
	// 		m_pModelCom->Set_CurAnimIndex(DEFAULT_PISTOL_IDLE);
	// 	}
	// 	else if (m_wstrCurWeaponName == L"WEAPON_FLAMEBULLET")
	// 	{
	// 		m_pModelCom->Set_CurAnimIndex(FLAME_BULLET_IDLE);
	// 	}
	// 	else if (m_wstrCurWeaponName == L"WEAPON_FIREDRAGON")
	// 	{
	// 		m_pModelCom->Set_CurAnimIndex(FIRE_DRAGON_IDLE);
	// 	}
	// 	else if (m_wstrCurWeaponName == L"WEAPON_POISON")
	// 		m_pModelCom->Set_CurAnimIndex(POISON_IDLE);
	// }

	m_pModelCom->Play_Animation(dTimeDelta, 0.1, 1.0);

	for (_uint i = 0; i < COLLIDERTYPE_END; ++i)
	{
		m_pColliderCom[i]->Update(m_pTransformCom->Get_WorldMatrix());
	}

	_float4   fCamLook = *dynamic_cast<CStatic_Camera*>(CGameInstance::GetInstance()->Get_CloneObjectList(LEVEL_GAMEPLAY, L"Layer_Camera")->back())->Get_CamLook();

	_vector		vCamLook = XMLoadFloat4(&fCamLook);
	_vector		vPos = m_pTransformCom->Get_State(CTransform::STATE_TRANSLATION);
	_vector		vLookat = vPos + vCamLook;

	m_pTransformCom->LookAt(vLookat);

	dynamic_cast<CStatic_Camera*>(CGameInstance::GetInstance()->Get_CloneObjectList(LEVEL_GAMEPLAY, L"Layer_Camera")->
		back())->Camera_Update(m_pTransformCom->Get_State(CTransform::STATE_TRANSLATION), 
			m_pTransformCom->Get_State(CTransform::STATE_LOOK), 
			dTimeDelta);

	

	RELEASE_INSTANCE(CGameInstance);
}

void CPlayer::Late_Tick(_double TimeDelta)
{
	__super::Late_Tick(TimeDelta);

	_uint iNumParts = 0;

	iNumParts = (_uint)m_vecPlayerParts.size();

	for (_uint i = 0; i < iNumParts; ++i)
	{
		m_vecPlayerParts[i]->Late_Tick(TimeDelta);
	}

	if (nullptr != m_pRendererCom)
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHABLEND, this);
}

HRESULT CPlayer::Render()
{
	FAILED_CHECK_RETURN(__super::Render(), E_FAIL);
	FAILED_CHECK_RETURN(SetUp_ShaderResources(), E_FAIL);

	_uint iNumMeshes = m_pModelCom->Get_NumMeshes();

	for (_uint i = 0; i < iNumMeshes; ++i)
	{
		/* 이 모델을 그리기위한 셰이더에 머테리얼 텍스쳐를 전달하낟. */
		m_pModelCom->Bind_Material(m_pShaderCom, i, aiTextureType_DIFFUSE, L"g_DiffuseTexture");

		m_pModelCom->Render(m_pShaderCom, i, L"g_BoneMatrices");
	}

#ifdef _DEBUG
	for (_uint i = 0; i < COLLIDERTYPE_END; ++i)
	{
		if (nullptr != m_pColliderCom[i])
			m_pColliderCom[i]->Render();
	}

#endif

	return S_OK;
}

HRESULT CPlayer::SetUp_Components()
{
	FAILED_CHECK_RETURN(__super::Add_Component(CGameInstance::Get_StaticLevelIndex(), L"Prototype_Component_Renderer", L"Com_Renderer",	(CComponent**)&m_pRendererCom, this), E_FAIL);
	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Shader_VtxAnimModel", L"Com_Shader",	(CComponent**)&m_pShaderCom, this), E_FAIL);

	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Model_Default_Pistol", L"Com_Default_Pistol_Model", (CComponent**)&m_tWeaponDesc[WEAPON_DEFAULT].m_pWeaponModelCom, this), E_FAIL);
	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Model_Fire_Dragon", L"Com_Fire_Dragon_Model", (CComponent**)&m_tWeaponDesc[WEAPON_FIREDRAGON].m_pWeaponModelCom, this), E_FAIL);
	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Model_Flame_Bullet", L"Com_Flame_Bullet_Model", (CComponent**)&m_tWeaponDesc[WEAPON_FLAMEBULLET].m_pWeaponModelCom, this), E_FAIL);
	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Model_Poison", L"Com_Poison_Model", (CComponent**)&m_tWeaponDesc[WEAPON_POISON].m_pWeaponModelCom, this), E_FAIL);

	FAILED_CHECK_RETURN(__super::Add_Component(CGameInstance::Get_StaticLevelIndex(), L"Prototype_Component_State", L"Com_State", (CComponent**)&m_pState, this), E_FAIL);


	m_pModelCom = m_tWeaponDesc[WEAPON_DEFAULT].m_pWeaponModelCom;
	m_wstrCurWeaponName = L"";
	m_wstrCurWeaponName = m_tWeaponDesc[WEAPON_DEFAULT].m_wstrWeaponName;
	CCollider::COLLIDERDESC			ColliderDesc;

	/* For.Com_AABB */
	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));
	ColliderDesc.vSize = _float3(0.7f, 1.5f, 0.7f);
	ColliderDesc.vPosition = _float3(0.f, ColliderDesc.vSize.y * 0.5f, 0.f);
	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Collider_AABB", L"Com_AABB", (CComponent**)&m_pColliderCom[COLLIDER_AABB], this, &ColliderDesc), E_FAIL);

	/* For.Com_OBB */
	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));
	ColliderDesc.vSize = _float3(1.0f, 1.0f, 1.0f);
	ColliderDesc.vRotation = _float3(0.f, XMConvertToRadians(45.0f), 0.f);
	ColliderDesc.vPosition = _float3(0.f, ColliderDesc.vSize.y * 0.5f, 0.f);
	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Collider_OBB", L"Com_OBB", (CComponent**)&m_pColliderCom[COLLIDER_OBB], this, &ColliderDesc), E_FAIL);

	/* For.Com_SPHERE */
	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));
	ColliderDesc.vSize = _float3(0.7f, 0.7f, 0.7f);
	ColliderDesc.vPosition = _float3(0.f, ColliderDesc.vSize.y * 0.5f, 0.f);
	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Collider_SPHERE", L"Com_SPHERE", (CComponent**)&m_pColliderCom[COLLIDER_SPHERE], this, &ColliderDesc), E_FAIL);

	return S_OK;
}

HRESULT CPlayer::SetUp_ShaderResources()
{
	NULL_CHECK_RETURN(m_pShaderCom, E_FAIL);

	FAILED_CHECK_RETURN(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, L"g_WorldMatrix"), E_FAIL);

	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);
	
	FAILED_CHECK_RETURN(m_pShaderCom->Set_Matrix(L"g_ViewMatrix", &pGameInstance->Get_TransformFloat4x4(CPipeLine::D3DTS_VIEW)), E_FAIL);
	FAILED_CHECK_RETURN(m_pShaderCom->Set_RawValue(L"g_PixelOffset", &(_float4(1 / (_float)g_iWinSizeX, 1 / (_float)g_iWinSizeY, 0.f, 0.f)) , sizeof(_float4)), E_FAIL);
	FAILED_CHECK_RETURN(m_pShaderCom->Set_Matrix(L"g_ProjMatrix", &pGameInstance->Get_TransformFloat4x4(CPipeLine::D3DTS_PROJ)), E_FAIL);

	/* For.Lights */
	const LIGHTDESC* pLightDesc = pGameInstance->Get_LightDesc(0);
	NULL_CHECK_RETURN(pLightDesc, E_FAIL);

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CPlayer::SetUp_State()
{
	m_pPlayerState = CPlayer_State::Create(this);
	m_pWeaponState = CWeapon_State::Create(this);
	NULL_CHECK_RETURN(m_pPlayerState, E_FAIL);
	NULL_CHECK_RETURN(m_pWeaponState, E_FAIL);

	return S_OK;
}

HRESULT CPlayer::Ready_Parts()
{
	CGameObject*		pPartObject = nullptr;

	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	CWeapon::WEAPONDESC	Weapondesc;
	ZeroMemory(&Weapondesc, sizeof(CWeapon::WEAPONDESC));

	Weapondesc.PivotMatrix = m_pModelCom->Get_PivotFloat4x4();
	Weapondesc.pSocket = m_pModelCom->Get_BonePtr("Bip001 R Finger12");
	Weapondesc.pTargetTransform = m_pTransformCom;
	Safe_AddRef(Weapondesc.pSocket);
	Safe_AddRef(m_pTransformCom);

	pPartObject = pGameInstance->Clone_GameObject(L"Prototype_GameObject_Weapon", &Weapondesc);
	NULL_CHECK_RETURN(pPartObject, E_FAIL);

	m_vecPlayerParts.push_back(pPartObject);

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

CPlayer * CPlayer::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CPlayer*		pInstance = new CPlayer(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CPlayer");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CPlayer::Clone(const wstring& wstrPrototypeTag, void * pArg)
{
	CPlayer*		pInstance = new CPlayer(*this);

	if (FAILED(pInstance->Initialize_Clone(wstrPrototypeTag, pArg)))
	{
		MSG_BOX("Failed to Cloned : CPlayer");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CPlayer::Free()
{
	__super::Free();

	for (_uint i = 0; i < COLLIDERTYPE_END; ++i)
		Safe_Release(m_pColliderCom[i]);

	for (auto& pPart : m_vecPlayerParts)
		Safe_Release(pPart);

	Safe_Release(m_pState);
	Safe_Release(m_pPlayerState);
	Safe_Release(m_pWeaponState);

	// Safe_Release(m_pModelCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pRendererCom);

	if (m_bIsClone)
	{
		for (_uint i = 0; i < WEAPON_END; ++i)
			Safe_Release(m_tWeaponDesc[i].m_pWeaponModelCom);
	}
}
