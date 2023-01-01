#include "stdafx.h"
#include "..\public\Canvas.h"

CCanvas::CCanvas(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CUI(pDevice,pContext)
{
}

CCanvas::CCanvas(const CCanvas& rhs)
	:CUI(rhs)
{
}

HRESULT CCanvas::Initialize_Prototype()
{
	FAILED_CHECK_RETURN(__super::Initialize_Prototype(), E_FAIL);

	return S_OK;
}

HRESULT CCanvas::Initialize_Clone(const wstring& wstrPrototypeTag, void* pArg)
{
	FAILED_CHECK_RETURN(__super::Initialize_Clone(wstrPrototypeTag, pArg), E_FAIL);

	return S_OK;
}

void CCanvas::Tick(_double TimeDelta)
{
	__super::Tick(TimeDelta);

}

void CCanvas::Late_Tick(_double TimeDelta)
{
	__super::Late_Tick(TimeDelta);
}

HRESULT CCanvas::Render()
{
	return S_OK;
}

HRESULT CCanvas::Add_ChildUI(CUI* pUI)
{
	NULL_CHECK_RETURN(pUI, E_FAIL);

	m_vecChildrenUI.push_back(pUI);
	Safe_AddRef(pUI);

	return S_OK;
}

void CCanvas::Free()
{
	__super::Free();

	for(auto& pChildUI : m_vecChildrenUI)
	{
		Safe_Release(pChildUI);
	}
}
