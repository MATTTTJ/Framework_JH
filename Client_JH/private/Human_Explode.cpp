#include "stdafx.h"
#include "..\public\Human_Explode.h"
#include "GameInstance.h"
#include "Player.h"
// #include "Human_Sword_State.h"

CHuman_Explode::CHuman_Explode(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CMonster(pDevice, pContext)
{
}

CHuman_Explode::CHuman_Explode(const CHuman_Explode& rhs)
	:CMonster(rhs)
{
}

HRESULT CHuman_Explode::Initialize_Prototype()
{
	m_bHasModel = true;

	FAILED_CHECK_RETURN(__super::Initialize_Prototype(), E_FAIL);

	return S_OK;
}

HRESULT CHuman_Explode::Initialize_Clone(const wstring& wstrPrototypeTag, void* pArg)
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

void CHuman_Explode::Tick(_double TimeDelta)
{
	CMonster::Tick(TimeDelta);
}

void CHuman_Explode::Late_Tick(_double TimeDelta)
{
	CMonster::Late_Tick(TimeDelta);
}

HRESULT CHuman_Explode::Render()
{
	return S_OK;
}

void CHuman_Explode::Collider_Tick(_double TimeDelta)
{
	CMonster::Collider_Tick(TimeDelta);
}

void CHuman_Explode::Collider_Late_Tick(_double TimeDelta)
{
	CMonster::Collider_Late_Tick(TimeDelta);
}

void CHuman_Explode::Set_On_NaviMesh()
{
	CMonster::Set_On_NaviMesh();
}

HRESULT CHuman_Explode::Ready_UI()
{
	return S_OK;
}

HRESULT CHuman_Explode::SetUp_Components()
{
	return S_OK;
}

HRESULT CHuman_Explode::SetUp_ShaderResources()
{
	return S_OK;
}

void CHuman_Explode::Collision_Event(CPlayer* pPlayer)
{
	CMonster::Collision_Event(pPlayer);
}

CHuman_Explode* CHuman_Explode::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	return nullptr;
}

CGameObject* CHuman_Explode::Clone(const wstring& wstrPrototypeTag, void* pArg)
{
	return nullptr;
}

void CHuman_Explode::Free()
{
	CMonster::Free();
}
