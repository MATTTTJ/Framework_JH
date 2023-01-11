#include "stdafx.h"
#include "..\public\Monster.h"
#include "GameInstance.h"
#include "State.h"
#include "UI.h"
#include "Collider.h"

CMonster::CMonster(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CGameObject(pDevice, pContext)
{

}

CMonster::CMonster(const CMonster & rhs)
	: CGameObject(rhs)
{

}

_matrix CMonster::Get_BoneMatrix(const string& strBoneName)
{
	if (nullptr == m_pModelCom)
		return XMMatrixIdentity();

	return m_pModelCom->Get_BoneMatrix(strBoneName);
}

_vector CMonster::Get_TransformState(CTransform::STATE eState)
{
	if (m_pModelCom == nullptr)
		return XMVectorSet(0.f, 1.f, 0.f, 0.f);

	return m_pTransformCom->Get_State(eState);
}

_matrix CMonster::Get_CombindMatrix(const string& strBoneName)
{
	if (nullptr == m_pModelCom)
		return XMMatrixIdentity();

	return m_pModelCom->Get_CombindMatrix(strBoneName);
}

HRESULT CMonster::Ready_UI()
{


	
	return S_OK;
}

HRESULT CMonster::Initialize_Prototype()
{
	FAILED_CHECK_RETURN(__super::Initialize_Prototype(), E_FAIL);

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
	FAILED_CHECK_RETURN(__super::Render(), E_FAIL);

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

	Safe_Release(m_pModelCom);
	Safe_Release(m_pState);
	for(_uint i =0; i<COLLTYPE_END; ++i)
	{
		Safe_Release(m_pColliderCom[i]);
	}
	for(_uint i =0; i<TEXTURETYPE_END; ++i)
	{
		Safe_Release(m_pTextureCom[i]);
	}
}
