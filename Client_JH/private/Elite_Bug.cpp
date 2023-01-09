#include "stdafx.h"
#include "..\public\Elite_Bug.h"
#include "GameInstance.h"
#include "Player.h"
// #include "Human_Sword_State.h"

CElite_Bug::CElite_Bug(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CMonster(pDevice, pContext)
{
}

CElite_Bug::CElite_Bug(const CElite_Bug& rhs)
	:CMonster(rhs)
{
}

HRESULT CElite_Bug::Initialize_Prototype()
{
	m_bHasModel = true;

	FAILED_CHECK_RETURN(__super::Initialize_Prototype(), E_FAIL);

	return S_OK;
}

HRESULT CElite_Bug::Initialize_Clone(const wstring& wstrPrototypeTag, void* pArg)
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

void CElite_Bug::Tick(_double TimeDelta)
{
	CMonster::Tick(TimeDelta);
}

void CElite_Bug::Late_Tick(_double TimeDelta)
{
	CMonster::Late_Tick(TimeDelta);
}

HRESULT CElite_Bug::Render()
{
	return S_OK;
}

void CElite_Bug::Collider_Tick(_double TimeDelta)
{
	CMonster::Collider_Tick(TimeDelta);
}


void CElite_Bug::Set_On_NaviMesh()
{
	CMonster::Set_On_NaviMesh();
}

void CElite_Bug::Collision_Event(CBullet* pBullet)
{
	CMonster::Collision_Event(pBullet);
}

HRESULT CElite_Bug::Ready_UI()
{
	return S_OK;
}

HRESULT CElite_Bug::SetUp_Components()
{
	return S_OK;
}

HRESULT CElite_Bug::SetUp_ShaderResources()
{
	return S_OK;
}


CElite_Bug* CElite_Bug::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	return nullptr;
}

CGameObject* CElite_Bug::Clone(const wstring& wstrPrototypeTag, void* pArg)
{
	return nullptr;
}

void CElite_Bug::Free()
{
	CMonster::Free();
}
