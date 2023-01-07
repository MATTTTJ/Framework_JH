#include "stdafx.h"
#include "..\public\Human_Spear.h"
#include "GameInstance.h"
#include "Player.h"
// #include "Human_Sword_State.h"

CHuman_Spear::CHuman_Spear(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CMonster(pDevice, pContext)
{
}

CHuman_Spear::CHuman_Spear(const CHuman_Spear& rhs)
	:CMonster(rhs)
{
}

HRESULT CHuman_Spear::Initialize_Prototype()
{
	m_bHasModel = true;

	FAILED_CHECK_RETURN(__super::Initialize_Prototype(), E_FAIL);

	return S_OK;
}

HRESULT CHuman_Spear::Initialize_Clone(const wstring& wstrPrototypeTag, void* pArg)
{
	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	if (nullptr != pArg)
		memcpy(&TransformDesc, pArg, sizeof(CTransform::TRANSFORMDESC));
	else
	{
		TransformDesc.fSpeedPerSec = 5.0f;
		TransformDesc.fRotationPerSec = XMConvertToRadians(90.f);
	}

	FAILED_CHECK_RETURN(__super::Initialize_Clone(wstrPrototypeTag, &TransformDesc), E_FAIL);

	FAILED_CHECK_RETURN(SetUp_Components(), E_FAIL);

	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(0.f, 0.f, 10.f, 1.f));

	// m_pStat = C

	return S_OK;
}

void CHuman_Spear::Tick(_double TimeDelta)
{
	CMonster::Tick(TimeDelta);
}

void CHuman_Spear::Late_Tick(_double TimeDelta)
{
	CMonster::Late_Tick(TimeDelta);
}

HRESULT CHuman_Spear::Render()
{
	return S_OK;
}

void CHuman_Spear::Collider_Tick(_double TimeDelta)
{
	CMonster::Collider_Tick(TimeDelta);
}

void CHuman_Spear::Collider_Late_Tick(_double TimeDelta)
{
	CMonster::Collider_Late_Tick(TimeDelta);
}

void CHuman_Spear::Set_On_NaviMesh()
{
	CMonster::Set_On_NaviMesh();
}

HRESULT CHuman_Spear::Ready_UI()
{
	return S_OK;
}

HRESULT CHuman_Spear::SetUp_Components()
{
	return S_OK;
}

HRESULT CHuman_Spear::SetUp_ShaderResources()
{
	return S_OK;
}

void CHuman_Spear::Collision_Event(CPlayer* pPlayer)
{
	CMonster::Collision_Event(pPlayer);
}

CHuman_Spear* CHuman_Spear::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	return nullptr;
}

CGameObject* CHuman_Spear::Clone(const wstring& wstrPrototypeTag, void* pArg)
{
	return nullptr;
}

void CHuman_Spear::Free()
{
	CMonster::Free();
}
