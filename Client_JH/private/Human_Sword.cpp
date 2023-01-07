#include "stdafx.h"
#include "..\public\Human_Sword.h"
#include "GameInstance.h"
#include "Player.h"
// #include "Human_Sword_State.h"

CHuman_Sword::CHuman_Sword(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CMonster(pDevice, pContext)
{
}

CHuman_Sword::CHuman_Sword(const CHuman_Sword& rhs)
	:CMonster(rhs)
{
}

HRESULT CHuman_Sword::Initialize_Prototype()
{
	m_bHasModel = true;

	FAILED_CHECK_RETURN(__super::Initialize_Prototype(), E_FAIL);

	return S_OK;
}

HRESULT CHuman_Sword::Initialize_Clone(const wstring& wstrPrototypeTag, void* pArg)
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

void CHuman_Sword::Tick(_double TimeDelta)
{
	CMonster::Tick(TimeDelta);
}

void CHuman_Sword::Late_Tick(_double TimeDelta)
{
	CMonster::Late_Tick(TimeDelta);
}

HRESULT CHuman_Sword::Render()
{
	return S_OK;
}

void CHuman_Sword::Collider_Tick(_double TimeDelta)
{
	CMonster::Collider_Tick(TimeDelta);
}

void CHuman_Sword::Collider_Late_Tick(_double TimeDelta)
{
	CMonster::Collider_Late_Tick(TimeDelta);
}

void CHuman_Sword::Set_On_NaviMesh()
{
	CMonster::Set_On_NaviMesh();
}

void CHuman_Sword::Ready_UI()
{
}

HRESULT CHuman_Sword::SetUp_Components()
{
	return S_OK;
}

HRESULT CHuman_Sword::SetUp_ShaderResources()
{
	return S_OK;
}

void CHuman_Sword::Collision_Event(CPlayer* pPlayer)
{
	CMonster::Collision_Event(pPlayer);
}

CHuman_Sword* CHuman_Sword::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	return nullptr;
}

CGameObject* CHuman_Sword::Clone(const wstring& wstrPrototypeTag, void* pArg)
{
	return nullptr;
}

void CHuman_Sword::Free()
{
	CMonster::Free();
}
