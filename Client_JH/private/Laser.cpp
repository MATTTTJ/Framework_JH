#include "stdafx.h"
#include "..\public\Laser.h"
#include "GameInstance.h"
#include "Monster.h"
#include "Normal_Boss.h"
#include "Player.h"
#include "Static_Camera.h"



CLaser::CLaser(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CBullet(pDevice, pContext)
{
}

CLaser::CLaser(const CLaser& rhs)
	: CBullet(rhs)
{
}

HRESULT CLaser::Initialize_Prototype()
{
	FAILED_CHECK_RETURN(__super::Initialize_Prototype(), E_FAIL);

	return S_OK;
}

HRESULT CLaser::Initialize_Clone(const wstring& wstrPrototypeTag, void* pArg)
{

	if (nullptr != pArg)
	{
		m_tBulletOption = *(BULLETOPTION*)pArg;

		m_tBulletOption.BulletDesc.TransformDesc.fSpeedPerSec = 70.f;
		m_tBulletOption.m_eType = CBullet::BULLETOPTION::TYPE_FIRE;
		m_tBulletOption.BulletDesc.m_iDamage = 30;
		m_pOwner = m_tBulletOption.m_pOwner;
		FAILED_CHECK_RETURN(__super::Initialize_Clone(wstrPrototypeTag, &m_tBulletOption), E_FAIL);
		FAILED_CHECK_RETURN(SetUp_Component(), E_FAIL);
		m_vPSize = _float2{ 5.f, 0.1f};
		_float4 Pos = _float4(m_tBulletOption.BulletDesc.TransformDesc.vInitPos.x, m_tBulletOption.BulletDesc.TransformDesc.vInitPos.y, m_tBulletOption.BulletDesc.TransformDesc.vInitPos.z, 1.f)
			+ _float4(m_tBulletOption.BulletDesc.m_vBulletLook);
		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, Pos);

		m_pTransformCom->LookAt(m_pTransformCom->Get_State(CTransform::STATE_TRANSLATION) + XMLoadFloat4(&m_tBulletOption.BulletDesc.m_vBulletLook));
	}
	else
	{
		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(0.f, 0.f, 0.f, 1.f));
		m_pTransformCom->Set_Scaled(_float3(1000.f, 1000.f, 1.f));
	}

	
	m_vLook = dynamic_cast<CNormal_Boss*>(m_pOwner)->Get_TransformState(CTransform::STATE_UP);

	return S_OK;
}

void CLaser::Tick(_double dTimeDelta)
{
	__super::Tick(dTimeDelta);


	if (m_bLaserAlpha == false)
	{
		m_fLaserAlpha -= (_float)(dTimeDelta);

		if (m_fLaserAlpha <= 0.f)
			m_fLaserAlpha = 0.f;
	}
	else
	{
		m_fLaserAlpha += (_float)dTimeDelta;
		if (m_fLaserAlpha >= 1.f)
			m_fLaserAlpha = 1.f;
	}
}

void CLaser::Late_Tick(_double dTimeDelta)
{
	__super::Late_Tick(dTimeDelta);


	
	m_pBulletColliderCom->Update(m_pTransformCom->Get_WorldMatrix());

}

HRESULT CLaser::Render()
{
	// if (m_bRender == false)
	// 	return S_OK;
	//
	FAILED_CHECK_RETURN(__super::Render(), E_FAIL);

	FAILED_CHECK_RETURN(SetUp_ShaderResources(), E_FAIL);

	m_pShaderCom->Begin(5);

	m_pPointBuffer->Render();

	return S_OK;
}


void CLaser::Set_Look(_float4 vLook)
{
	m_vLook = vLook;
}

void CLaser::Set_Up(_float4 vUp)
{
	m_vUp = vUp;
}

void CLaser::Set_PSize(_float fSize)
{
	 m_vPSize.x = fSize + 5.f; 
}

HRESULT CLaser::SetUp_Component()
{
	FAILED_CHECK_RETURN(__super::Add_Component(CGameInstance::Get_StaticLevelIndex(), L"Prototype_Component_Renderer", L"Com_Renderer", (CComponent**)&m_pRendererCom, this), E_FAIL);
	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Shader_VtxPointInstance", L"Com_Shader", (CComponent**)&m_pShaderCom, this), E_FAIL);
	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_VIBuffer_Point_Instancing", L"Com_VIBuffer", (CComponent**)&m_pPointBuffer, this), E_FAIL);
	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Texture_Laser", L"Com_Texture", (CComponent**)&m_pTextureCom, this), E_FAIL);

	return S_OK;
}

HRESULT CLaser::SetUp_ShaderResources()
{
	NULL_CHECK_RETURN(m_pShaderCom, E_FAIL);

	FAILED_CHECK_RETURN(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, L"g_WorldMatrix"), E_FAIL);
	FAILED_CHECK_RETURN(m_pShaderCom->Set_Matrix(L"g_ViewMatrix", &CGameInstance::GetInstance()->Get_TransformFloat4x4(CPipeLine::D3DTS_VIEW)), E_FAIL);
	FAILED_CHECK_RETURN(m_pShaderCom->Set_Matrix(L"g_ProjMatrix", &CGameInstance::GetInstance()->Get_TransformFloat4x4(CPipeLine::D3DTS_PROJ)), E_FAIL);
	FAILED_CHECK_RETURN(m_pShaderCom->Set_RawValue(L"g_vLook", &m_vLook, sizeof(_float4)), E_FAIL);
	FAILED_CHECK_RETURN(m_pShaderCom->Set_RawValue(L"g_vUp", &m_vUp, sizeof(_float4)), E_FAIL);
	FAILED_CHECK_RETURN(m_pShaderCom->Set_RawValue(L"g_vPSize", &m_vPSize, sizeof(_float2)), E_FAIL);
	FAILED_CHECK_RETURN(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, L"g_Texture"), E_FAIL);
	
	FAILED_CHECK_RETURN(m_pShaderCom->Set_RawValue(L"g_fLaserAlpha", &m_fLaserAlpha, sizeof(_float)), E_FAIL);

	return S_OK;
}

CLaser* CLaser::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CLaser*		pInstance = new CLaser(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CLaser");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CLaser::Clone(const wstring& wstrPrototypeTag, void* pArg)
{
	CLaser*		pInstance = new CLaser(*this);

	if (FAILED(pInstance->Initialize_Clone(wstrPrototypeTag, pArg)))
	{
		MSG_BOX("Failed to Cloned : CLaser");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CLaser::Free()
{
	__super::Free();

	Safe_Release(m_pRendererCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pBulletColliderCom);
	Safe_Release(m_pPointBuffer);


}
