#include "stdafx.h"
#include "BackGround.h"

CBackGround::CBackGround(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CGameObject(pDevice,pContext)
{
}

CBackGround::CBackGround(const CBackGround& rhs)
	: CGameObject(rhs)

{
}

HRESULT CBackGround::Initialize_Prototype()
{
	if (FAILED(CGameObject::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CBackGround::Initialize_Clone(void* pArg)
{
	if (FAILED(CGameObject::Initialize_Clone(pArg)))
		return E_FAIL;

	return S_OK;
}

void CBackGround::Tick(_double TimeDelta)
{
	CGameObject::Tick(TimeDelta);
}

void CBackGround::Late_Tick(_double TimeDelta)
{
	CGameObject::Late_Tick(TimeDelta);
}

HRESULT CBackGround::Render()
{
	if (FAILED(CGameObject::Render()))
		return E_FAIL;

	return S_OK;
}

CBackGround* CBackGround::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CBackGround*		pInst = new CBackGround(pDevice, pContext);

	if(FAILED(pInst->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Create CBackGround");
		Safe_Release(pInst);
	}
	return pInst;
}

CGameObject* CBackGround::Clone(void* pArg)
{
	CBackGround*		pInst = new CBackGround(*this);

	if(FAILED(pInst->Initialize_Clone(pArg)))
	{
		MSG_BOX("Failed to Cloned CBackGround");
		Safe_Release(pInst);
	}
	return pInst;
}

void CBackGround::Free()
{
	CGameObject::Free();
}
