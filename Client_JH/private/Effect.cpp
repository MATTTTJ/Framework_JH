#include "stdafx.h"
#include "..\public\Effect.h"
#include "GameInstance.h"
#include "Monster.h"
#include "Player.h"

CEffect::CEffect(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CGameObject(pDevice, pContext)
{
}

CEffect::CEffect(const CEffect& rhs)
	:CGameObject(rhs)
{
}

HRESULT CEffect::Initialize_Prototype()
{
	FAILED_CHECK_RETURN(__super::Initialize_Prototype(), E_FAIL);

	return S_OK;
}

HRESULT CEffect::Initialize_Clone(const wstring& wstrPrototypeTag, void* pArg)
{
	FAILED_CHECK_RETURN(__super::Initialize_Clone(wstrPrototypeTag, pArg), E_FAIL);

	if (nullptr == m_pOwner)
	{
		m_pOwner = CGameInstance::GetInstance()->Get_CloneObjectList(LEVEL_GAMEPLAY, L"Layer_Player")->front();
	}
	else
	{
		if (CGameInstance::GetInstance()->Get_CloneObjectList(LEVEL_GAMEPLAY, L"Layer_Player")->empty())
		{
			m_pOwner = nullptr;
			return E_FAIL;
		}
	}
	FAILED_CHECK_RETURN(SetUp_Components(), E_FAIL);



	return S_OK;
}

void CEffect::Tick(_double dTimeDelta)
{
	__super::Tick(dTimeDelta);
}

void CEffect::Late_Tick(_double dTimeDelta)
{
	__super::Late_Tick(dTimeDelta);
}

HRESULT CEffect::Render()
{
	return S_OK;
}

HRESULT CEffect::SetUp_Components()
{
	return S_OK;
}

HRESULT CEffect::SetUp_ShaderResources()
{
	return S_OK;
}

CGameObject* CEffect::Clone(const wstring& wstrPrototypeTag, void* pArg)
{
	return nullptr;
}

void CEffect::Free()
{
	__super::Free();
}
