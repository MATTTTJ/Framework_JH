#include "stdafx.h"
#include "..\public\Elite_Knight.h"
#include "GameInstance.h"
#include "Player.h"
// #include "Human_Sword_State.h"

CElite_Knight::CElite_Knight(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CMonster(pDevice, pContext)
{
}

CElite_Knight::CElite_Knight(const CElite_Knight& rhs)
	:CMonster(rhs)
{
}

HRESULT CElite_Knight::Initialize_Prototype()
{
	m_bHasModel = true;

	FAILED_CHECK_RETURN(__super::Initialize_Prototype(), E_FAIL);

	return S_OK;
}

HRESULT CElite_Knight::Initialize_Clone(const wstring& wstrPrototypeTag, void* pArg)
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

void CElite_Knight::Tick(_double TimeDelta)
{
	CMonster::Tick(TimeDelta);
}

void CElite_Knight::Late_Tick(_double TimeDelta)
{
	CMonster::Late_Tick(TimeDelta);
}

HRESULT CElite_Knight::Render()
{
	return S_OK;
}

void CElite_Knight::Collider_Tick(_double TimeDelta)
{
	CMonster::Collider_Tick(TimeDelta);
}



void CElite_Knight::Set_On_NaviMesh()
{
	CMonster::Set_On_NaviMesh();
}

void CElite_Knight::Collision_Body(CBullet* pBullet)
{
	CMonster::Collision_Body(pBullet);
}

HRESULT CElite_Knight::Ready_UI()
{
	return S_OK;
}

HRESULT CElite_Knight::SetUp_Components()
{
	return S_OK;
}

HRESULT CElite_Knight::SetUp_ShaderResources()
{
	return S_OK;
}



CElite_Knight* CElite_Knight::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	return nullptr;
}

CGameObject* CElite_Knight::Clone(const wstring& wstrPrototypeTag, void* pArg)
{
	return nullptr;
}

void CElite_Knight::Free()
{
	CMonster::Free();
}
