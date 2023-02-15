#include "stdafx.h"
#include "..\public\Default_Bullet_Dead.h"
#include "GameInstance.h"
#include "Player.h"

CDefault_Bullet_Dead::CDefault_Bullet_Dead(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CGameObject(pDevice, pContext)
{
}

CDefault_Bullet_Dead::CDefault_Bullet_Dead(const CDefault_Bullet_Dead& rhs)
	: CGameObject(rhs)
{
}

HRESULT CDefault_Bullet_Dead::Initialize_Prototype()
{
	FAILED_CHECK_RETURN(__super::Initialize_Prototype(), E_FAIL);

	return S_OK;
}

HRESULT CDefault_Bullet_Dead::Initialize_Clone(const wstring& wstrPrototypeTag, void* pArg)
{
	if (nullptr != pArg)
	{
		m_tEffectDesc = *(EFFECTDESC*)pArg;

		// m_pOwner = m_tEffectDesc.m_pOwner;
		FAILED_CHECK_RETURN(__super::Initialize_Clone(wstrPrototypeTag, &m_tEffectDesc), E_FAIL);
		FAILED_CHECK_RETURN(SetUp_Component(), E_FAIL);

		m_vPSize = _float2{ 1.f, 1.f };
		m_pTransformCom->LookAt(m_pTransformCom->Get_State(CTransform::STATE_TRANSLATION) + XMVector3Normalize(XMVectorSet(m_tEffectDesc.m_tGameObjectDesc.m_vBulletLook.x, m_tEffectDesc.m_tGameObjectDesc.m_vBulletLook.y, m_tEffectDesc.m_tGameObjectDesc.m_vBulletLook.z, 0.f)));
		// m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION,
		// 	dynamic_cast<CPlayer*>(m_pOwner)->Get_MuzzlePtr()->Get_SphereCenter());
		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, (XMVectorSet(m_tEffectDesc.m_tGameObjectDesc.TransformDesc.vInitPos.x, m_tEffectDesc.m_tGameObjectDesc.TransformDesc.vInitPos.y, m_tEffectDesc.m_tGameObjectDesc.TransformDesc.vInitPos.z, 1.f)));

		 _matrix matpivot = XMMatrixRotationY(XMConvertToRadians(180.f));

		// m_vDir = XMVector3Normalize((dynamic_cast<CPlayer*>(m_pOwner)->Get_CurWeaponModelCom()->Get_BoneMatrix("Att") * matpivot * XMLoadFloat4x4(&dynamic_cast<CPlayer*>(m_pOwner)->Get_WorldFloat4x4())).r[3] -
			// (dynamic_cast<CPlayer*>(m_pOwner)->Get_CurWeaponModelCom()->Get_BoneMatrix("1202_Bone004") * matpivot * XMLoadFloat4x4(&dynamic_cast<CPlayer*>(m_pOwner)->Get_WorldFloat4x4())).r[3]);

	}
	else
	{
		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(0.f, 0.f, 0.f, 1.f));
		m_pTransformCom->Set_Scaled(_float3(1000.f, 1000.f, 1.f));
	}

	// _float4 vUp = m_pTransformCom->Get_State(CTransform::STATE_UP);
	// m_pTransformCom->Rotation(m_pTransformCom->Get_State(CTransform::STATE_RIGHT), XMConvertToRadians(45.f));
	// m_vLook = XMVector3Normalize(XMVector3Cross(m_vDir, vUp));

	m_iUV_Max_Width_Num = 2;
	m_iUV_Max_Height_Num = 2;
	m_iFrameCnt = 2;
	m_vPSize = _float2{ 1.5f, 1.5f };
	m_iUV_Cur_Width_Num = rand() % 2;
	m_iUV_Cur_Height_Num = rand() % 2;
	return S_OK;
}

void CDefault_Bullet_Dead::Tick(_double dTimeDelta)
{
	if (m_bIsDead == true)
		return;

	__super::Tick(dTimeDelta);


	m_iPlayOnFrameCnt++;

	if (m_iPlayOnFrameCnt == 3)
		Set_Dead(true);

	m_vPSize = _float2(m_vPSize.x + (_float)dTimeDelta * 4.f, m_vPSize.y + (_float)dTimeDelta * 4.f);
}

void CDefault_Bullet_Dead::Late_Tick(_double dTimeDelta)
{
	__super::Late_Tick(dTimeDelta);
	__super::Compute_CamDistance();

	if (m_pRendererCom != nullptr)
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_EFFECT, this);
}

HRESULT CDefault_Bullet_Dead::Render()
{
	FAILED_CHECK_RETURN(__super::Render(), E_FAIL);

	FAILED_CHECK_RETURN(SetUp_ShaderResources(), E_FAIL);

	m_pShaderCom->Begin(8);

	m_pPointBuffer->Render();

	return S_OK;
}

HRESULT CDefault_Bullet_Dead::SetUp_Component()
{
	FAILED_CHECK_RETURN(__super::Add_Component(CGameInstance::Get_StaticLevelIndex(), L"Prototype_Component_Renderer", L"Com_Renderer", (CComponent**)&m_pRendererCom, this), E_FAIL);
	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Shader_VtxPointInstance", L"Com_Shader", (CComponent**)&m_pShaderCom, this), E_FAIL);
	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_VIBuffer_Bullet_Instancing", L"Com_VIBuffer", (CComponent**)&m_pPointBuffer, this), E_FAIL);
	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Texture_Default_Bullet_DeadTex", L"Com_Texture", (CComponent**)&m_pTextureCom, this), E_FAIL);

	return S_OK;
}

HRESULT CDefault_Bullet_Dead::SetUp_ShaderResources()
{
	
	NULL_CHECK_RETURN(m_pShaderCom, E_FAIL);

	FAILED_CHECK_RETURN(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, L"g_WorldMatrix"), E_FAIL);
	FAILED_CHECK_RETURN(m_pShaderCom->Set_Matrix(L"g_ViewMatrix", &CGameInstance::GetInstance()->Get_TransformFloat4x4(CPipeLine::D3DTS_VIEW)), E_FAIL);
	FAILED_CHECK_RETURN(m_pShaderCom->Set_Matrix(L"g_ProjMatrix", &CGameInstance::GetInstance()->Get_TransformFloat4x4(CPipeLine::D3DTS_PROJ)), E_FAIL);
	FAILED_CHECK_RETURN(m_pShaderCom->Set_RawValue(L"g_iUV_Max_Width_Num", &m_iUV_Max_Width_Num, sizeof(_int)), E_FAIL);
	FAILED_CHECK_RETURN(m_pShaderCom->Set_RawValue(L"g_iUV_Max_Height_Num", &m_iUV_Max_Height_Num, sizeof(_int)), E_FAIL);
	FAILED_CHECK_RETURN(m_pShaderCom->Set_RawValue(L"g_iUV_Cur_Width_Num", &m_iUV_Cur_Width_Num, sizeof(_int)), E_FAIL);
	FAILED_CHECK_RETURN(m_pShaderCom->Set_RawValue(L"g_iUV_Cur_Height_Num", &m_iUV_Cur_Height_Num, sizeof(_int)), E_FAIL);
	FAILED_CHECK_RETURN(m_pShaderCom->Set_RawValue(L"g_vPSize", &m_vPSize, sizeof(_float2)), E_FAIL);
	FAILED_CHECK_RETURN(m_pShaderCom->Set_RawValue(L"g_vCamPosition", &CGameInstance::GetInstance()->Get_CamPos(), sizeof(_float4)), E_FAIL);
	FAILED_CHECK_RETURN(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, L"g_Texture"), E_FAIL);

	return S_OK;
}

CDefault_Bullet_Dead* CDefault_Bullet_Dead::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CDefault_Bullet_Dead*		pInstance = new CDefault_Bullet_Dead(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CDefault_Bullet_Dead");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CDefault_Bullet_Dead::Clone(const wstring& wstrPrototypeTag, void* pArg)
{
	CDefault_Bullet_Dead*		pInstance = new CDefault_Bullet_Dead(*this);

	if (FAILED(pInstance->Initialize_Clone(wstrPrototypeTag, pArg)))
	{
		MSG_BOX("Failed to Cloned : CDefault_Bullet_Dead");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CDefault_Bullet_Dead::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
	Safe_Release(m_pPointBuffer);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pRendererCom);
}