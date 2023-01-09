#include "stdafx.h"
#include "..\public\Human_Bow.h"
#include "GameInstance.h"
#include "Player.h"
// #include "Human_Sword_State.h"

CHuman_Bow::CHuman_Bow(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CMonster(pDevice, pContext)
{
}

CHuman_Bow::CHuman_Bow(const CHuman_Bow& rhs)
	:CMonster(rhs)
{
}

HRESULT CHuman_Bow::Initialize_Prototype()
{
	m_bHasModel = true;

	FAILED_CHECK_RETURN(__super::Initialize_Prototype(), E_FAIL);

	return S_OK;
}

HRESULT CHuman_Bow::Initialize_Clone(const wstring& wstrPrototypeTag, void* pArg)
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

void CHuman_Bow::Tick(_double TimeDelta)
{
	CMonster::Tick(TimeDelta);
}

void CHuman_Bow::Late_Tick(_double TimeDelta)
{
	CMonster::Late_Tick(TimeDelta);
}

HRESULT CHuman_Bow::Render()
{
	return S_OK;
}

void CHuman_Bow::Collider_Tick(_double TimeDelta)
{
	CMonster::Collider_Tick(TimeDelta);
}



void CHuman_Bow::Set_On_NaviMesh()
{
	CMonster::Set_On_NaviMesh();
}

void CHuman_Bow::Collision_Event(CBullet* pBullet)
{
	CMonster::Collision_Event(pBullet);
}

HRESULT CHuman_Bow::Ready_UI()
{
	return S_OK;
}

HRESULT CHuman_Bow::SetUp_Components()
{
	return S_OK;
}

HRESULT CHuman_Bow::SetUp_ShaderResources()
{
	return S_OK;
}


CHuman_Bow* CHuman_Bow::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	return nullptr;
}

CGameObject* CHuman_Bow::Clone(const wstring& wstrPrototypeTag, void* pArg)
{
	return nullptr;
}

void CHuman_Bow::Free()
{
	CMonster::Free();
}
