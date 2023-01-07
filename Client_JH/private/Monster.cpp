#include "stdafx.h"
#include "..\public\Monster.h"

CMonster::CMonster(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CGameObject(pDevice, pContext)
{

}

CMonster::CMonster(const CMonster & rhs)
	: CGameObject(rhs)
{

}

HRESULT CMonster::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CMonster::Initialize_Clone(const wstring& wstrPrototypeTag, void * pArg)
{
	FAILED_CHECK_RETURN(__super::Initialize_Clone(wstrPrototypeTag, pArg), E_FAIL);

	return S_OK;
}

void CMonster::Tick(_double TimeDelta)
{
	__super::Tick(TimeDelta);
}

void CMonster::Late_Tick(_double TimeDelta)
{
	__super::Late_Tick(TimeDelta);
}

HRESULT CMonster::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;
#ifdef _DEBUG
	// for (_uint i = 0; i < COLLTYPE_END; ++i)
	// {
	// 	if (nullptr != m_pColliderCom[i])
	// 		m_pColliderCom[i]->Render();
	// }

#endif
	return S_OK;
}

void CMonster::Free()
{
	__super::Free();
}
