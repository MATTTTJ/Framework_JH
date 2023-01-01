#include "stdafx.h"
#include "..\public\UI.h"
#include "GameInstance.h"
#include "Player.h"

CUI::CUI(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject(pDevice, pContext)
{
}

CUI::CUI(const CUI& rhs)
	: CGameObject(rhs)
{
}

HRESULT CUI::Initialize_Prototype()
{
	FAILED_CHECK_RETURN(__super::Initialize_Prototype(), E_FAIL);

	return S_OK;
}

HRESULT CUI::Initialize_Clone(const wstring& wstrPrototypeTag, void* pArg)
{
	FAILED_CHECK_RETURN(__super::Initialize_Clone(wstrPrototypeTag, pArg), E_FAIL);

	XMStoreFloat4x4(&m_ViewMatrix, XMMatrixIdentity());
	return S_OK;
}

void CUI::Tick(_double dTimeDelta)
{
	__super::Tick(dTimeDelta);

	XMStoreFloat4x4(&m_ProjMatrix, XMMatrixOrthographicLH((_float)g_iWinSizeX, (_float)g_iWinSizeY, 0.f, 1.f));

	if (m_pOwner == nullptr)
	{
		m_pOwner = CGameInstance::GetInstance()->Get_CloneObjectList(LEVEL_GAMEPLAY, L"Layer_Player")->front();
	}

	else
	{
		if (CGameInstance::GetInstance()->Get_CloneObjectList(LEVEL_GAMEPLAY, L"Layer_Player")->empty())
		{
			m_pOwner = nullptr;
			return;
		}
	}
}

void CUI::Late_Tick(_double dTimeDelta)
{
	__super::Late_Tick(dTimeDelta);
}

HRESULT CUI::Render()
{
	FAILED_CHECK_RETURN(__super::Render(), E_FAIL);

	return S_OK;
}

void CUI::Free()
{
	__super::Free();

	Safe_Release(m_pRendererCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pOffTextureCom);
	Safe_Release(m_pOnTextureCom);
	Safe_Release(m_pVIBufferCom);
	
	for(_int i = 0; i < CWeapon_State::WEAPONTYPE_END; ++i)
	{
		if (m_pWeaponTextureCom[i] == nullptr)
			return;

		Safe_Release(m_pWeaponTextureCom[i]);
	}
}