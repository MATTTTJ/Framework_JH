#include "stdafx.h"
#include "..\public\Human_Granade.h"
#include "GameInstance.h"
#include "Player.h"
// #include "Human_Sword_State.h"

CHuman_Granade::CHuman_Granade(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CMonster(pDevice, pContext)
{
}

CHuman_Granade::CHuman_Granade(const CHuman_Granade& rhs)
	:CMonster(rhs)
{
}

HRESULT CHuman_Granade::Initialize_Prototype()
{
	m_bHasModel = true;

	FAILED_CHECK_RETURN(__super::Initialize_Prototype(), E_FAIL);

	return S_OK;
}

HRESULT CHuman_Granade::Initialize_Clone(const wstring& wstrPrototypeTag, void* pArg)
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

void CHuman_Granade::Tick(_double TimeDelta)
{
	CMonster::Tick(TimeDelta);
}

void CHuman_Granade::Late_Tick(_double TimeDelta)
{
	CMonster::Late_Tick(TimeDelta);
}

HRESULT CHuman_Granade::Render()
{
	return S_OK;
}

void CHuman_Granade::Collider_Tick(_double TimeDelta)
{
	CMonster::Collider_Tick(TimeDelta);
}

void CHuman_Granade::Collider_Late_Tick(_double TimeDelta)
{
	CMonster::Collider_Late_Tick(TimeDelta);
}

void CHuman_Granade::Set_On_NaviMesh()
{
	CMonster::Set_On_NaviMesh();
}

HRESULT CHuman_Granade::Ready_UI()
{
	return S_OK;
}

HRESULT CHuman_Granade::SetUp_Components()
{
	return S_OK;
}

HRESULT CHuman_Granade::SetUp_ShaderResources()
{
	return S_OK;
}

void CHuman_Granade::Collision_Event(CPlayer* pPlayer)
{
	CMonster::Collision_Event(pPlayer);
}

CHuman_Granade* CHuman_Granade::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	return nullptr;
}

CGameObject* CHuman_Granade::Clone(const wstring& wstrPrototypeTag, void* pArg)
{
	return nullptr;
}

void CHuman_Granade::Free()
{
	CMonster::Free();
}
