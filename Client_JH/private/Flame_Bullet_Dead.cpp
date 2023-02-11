#include "stdafx.h"
#include "..\public\Flame_Bullet_Dead.h"
#include "GameInstance.h"
#include "Player.h"
#include "Monster.h"

CFlame_Bullet_Dead::CFlame_Bullet_Dead(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CGameObject(pDevice, pContext)
{
}

CFlame_Bullet_Dead::CFlame_Bullet_Dead(const CFlame_Bullet_Dead& rhs)
	: CGameObject(rhs)
{
}

HRESULT CFlame_Bullet_Dead::Initialize_Prototype()
{
	FAILED_CHECK_RETURN(__super::Initialize_Prototype(), E_FAIL);

	return S_OK;
}

HRESULT CFlame_Bullet_Dead::Initialize_Clone(const wstring& wstrPrototypeTag, void* pArg)
{
	if (nullptr != pArg)
	{
		m_tEffectDesc = *(EFFECTDESC*)pArg;
		
		m_pOwner = m_tEffectDesc.m_pOwner;
		CGameObject::GAMEOBJECTDESC tmp;
		ZeroMemory(&tmp, sizeof(GAMEOBJECTDESC));
		tmp.TransformDesc.fSpeedPerSec = 1.f;


		FAILED_CHECK_RETURN(__super::Initialize_Clone(wstrPrototypeTag, &tmp), E_FAIL);
		FAILED_CHECK_RETURN(SetUp_Component(), E_FAIL);

		m_iBirthType = m_tEffectDesc.m_tGameObjectDesc.m_iCountType;

		m_vPSize = _float2{ 1.f, 1.f };
		m_pTransformCom->LookAt(m_pTransformCom->Get_State(CTransform::STATE_TRANSLATION) + XMVector3Normalize(XMVectorSet(m_tEffectDesc.m_tGameObjectDesc.m_vBulletLook.x, m_tEffectDesc.m_tGameObjectDesc.m_vBulletLook.y, m_tEffectDesc.m_tGameObjectDesc.m_vBulletLook.z, 0.f)));
		// m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION,
		// 	dynamic_cast<CPlayer*>(m_pOwner)->Get_MuzzlePtr()->Get_SphereCenter());
		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, (XMVectorSet(m_tEffectDesc.m_tGameObjectDesc.TransformDesc.vInitPos.x, m_tEffectDesc.m_tGameObjectDesc.TransformDesc.vInitPos.y, m_tEffectDesc.m_tGameObjectDesc.TransformDesc.vInitPos.z, 1.f)));
	}
	else
	{
		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(0.f, 0.f, 0.f, 1.f));
		m_pTransformCom->Set_Scaled(_float3(1000.f, 1000.f, 1.f));
	}

	// _float4 vUp = m_pTransformCom->Get_State(CTransform::STATE_UP);
	// m_pTransformCom->Rotation(m_pTransformCom->Get_State(CTransform::STATE_RIGHT), XMConvertToRadians(45.f));
	// m_vLook = XMVector3Normalize(XMVector3Cross(m_vDir, vUp));

	if (m_iBirthType == 0)
	{
		// _matrix matpivot = XMMatrixRotationY(XMConvertToRadians(180.f));

		// m_vDir = XMVector3Normalize((dynamic_cast<CPlayer*>(m_pOwner)->Get_CurWeaponModelCom()->Get_BoneMatrix("Att001") * matpivot * XMLoadFloat4x4(&dynamic_cast<CPlayer*>(m_pOwner)->Get_WorldFloat4x4())).r[3] -
		// 	(dynamic_cast<CPlayer*>(m_pOwner)->Get_CurWeaponModelCom()->Get_BoneMatrix("1205_Bone001") * matpivot * XMLoadFloat4x4(&dynamic_cast<CPlayer*>(m_pOwner)->Get_WorldFloat4x4())).r[3]);
		m_iUV_Max_Width_Num = 4;
		m_iUV_Max_Height_Num = 4;
		m_iFrameCnt = 1;
		m_vPSize = _float2{ 2.5f, 2.5f };
		m_iUV_Cur_Width_Num = 0;
		m_iUV_Cur_Height_Num = 0;
	}
	else if (m_iBirthType == 1)
	{
		m_iUV_Max_Width_Num = 4;
		m_iUV_Max_Height_Num = 4;
		m_iFrameCnt = 5;
		m_vPSize = _float2{ 1.f, 1.f };
		m_iUV_Cur_Width_Num = 0;
		m_iUV_Cur_Height_Num = 0;

		_float4 min = { -1.0f, 0.0f, -1.0f, 0.f };
		_float4 max = _float4(1.0f, 1.0f, 1.0f, 0.f);
		_float4 Dir;
		CGameUtils::GetRandomVector(&Dir, &min, &max);

		m_fUpSpeed = CGameUtils::GetRandomFloat(0.f, 0.1f);

		m_pTransformCom->LookAt(m_pTransformCom->Get_State(CTransform::STATE_TRANSLATION) + XMLoadFloat4(&Dir));
	}
	if (m_iBirthType == 3)
	{
		// _matrix matpivot = XMMatrixRotationY(XMConvertToRadians(180.f));

		// m_vDir = XMVector3Normalize((dynamic_cast<CPlayer*>(m_pOwner)->Get_CurWeaponModelCom()->Get_BoneMatrix("Att001") * matpivot * XMLoadFloat4x4(&dynamic_cast<CPlayer*>(m_pOwner)->Get_WorldFloat4x4())).r[3] -
		// 	(dynamic_cast<CPlayer*>(m_pOwner)->Get_CurWeaponModelCom()->Get_BoneMatrix("1205_Bone001") * matpivot * XMLoadFloat4x4(&dynamic_cast<CPlayer*>(m_pOwner)->Get_WorldFloat4x4())).r[3]);
		m_iUV_Max_Width_Num = 4;
		m_iUV_Max_Height_Num = 4;
		m_iFrameCnt = 1;
		m_vPSize = _float2{ 1.5f, 1.5f };
		m_iUV_Cur_Width_Num = 0;
		m_iUV_Cur_Height_Num = 0;
	}
	if (m_iBirthType == 4)
	{
		// _matrix matpivot = XMMatrixRotationY(XMConvertToRadians(180.f));

		// m_vDir = XMVector3Normalize((dynamic_cast<CPlayer*>(m_pOwner)->Get_CurWeaponModelCom()->Get_BoneMatrix("Att001") * matpivot * XMLoadFloat4x4(&dynamic_cast<CPlayer*>(m_pOwner)->Get_WorldFloat4x4())).r[3] -
		// 	(dynamic_cast<CPlayer*>(m_pOwner)->Get_CurWeaponModelCom()->Get_BoneMatrix("1205_Bone001") * matpivot * XMLoadFloat4x4(&dynamic_cast<CPlayer*>(m_pOwner)->Get_WorldFloat4x4())).r[3]);
		m_iUV_Max_Width_Num = 3;
		m_iUV_Max_Height_Num = 7;
		m_iFrameCnt = 1;
		m_vPSize = _float2{ 3.f, 3.f };
		m_iUV_Cur_Width_Num = 0;
		m_iUV_Cur_Height_Num = 0;
		m_pMonster = (CMonster*)m_tEffectDesc.m_pOwner;
	}
	return S_OK;
}

void CFlame_Bullet_Dead::Tick(_double dTimeDelta)
{
	if (m_bIsDead == true)
		return;

	__super::Tick(dTimeDelta);

	// _float3 fsize = m_pTransformCom->Get_Scaled();

	// fsize += _float3(_float(dTimeDelta), _float(dTimeDelta), _float(dTimeDelta));

	// m_pTransformCom->Set_Scaled(fsize);
	if (m_iBirthType == 0)
	{
		m_fTime += (_float)dTimeDelta;

		if (m_fTime > 1.f)
			m_fTime = 1.f;

		m_vPSize += _float2(_float(dTimeDelta) * 5.f, _float(dTimeDelta)* 5.f);

		m_iPlayOnFrameCnt++;

		if (m_iPlayOnFrameCnt == m_iFrameCnt)
		{
			if (m_iUV_Cur_Height_Num == m_iUV_Max_Height_Num - 1 && m_iUV_Cur_Width_Num == m_iUV_Max_Width_Num - 1)
			{
				Set_Dead(true);
				return;
			}

			m_iUV_Cur_Width_Num++;

			m_iPlayOnFrameCnt = 0;

			if (m_iUV_Cur_Width_Num == m_iUV_Max_Width_Num)
			{
				m_iUV_Cur_Width_Num = 0;
				m_iUV_Cur_Height_Num++;
			}
		}
	}

	else if(m_iBirthType ==1)
	{
		m_pTransformCom->Go_Straight(dTimeDelta, CTransform::TRANS_BULLET);

		_float4 BoomPos = m_pTransformCom->Get_State(CTransform::STATE_TRANSLATION);

		BoomPos.y += m_fUpSpeed * CGameUtils::GetRandomFloat(0.3f, 1.2f) * (_float)dTimeDelta;

		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, BoomPos);
		m_pTransformCom->Go_Straight(dTimeDelta, CTransform::TRANS_BULLET);

		m_fUpSpeed += 0.03f;

		m_iPlayOnFrameCnt++;

		if (m_iPlayOnFrameCnt == m_iFrameCnt)
		{
			if (m_iUV_Cur_Height_Num == m_iUV_Max_Height_Num - 1 && m_iUV_Cur_Width_Num == m_iUV_Max_Width_Num - 1)
			{
				Set_Dead(true);
				return;
			}

			m_iUV_Cur_Width_Num++;

			m_iPlayOnFrameCnt = 0;

			if (m_iUV_Cur_Width_Num == m_iUV_Max_Width_Num)
			{
				m_iUV_Cur_Width_Num = 0;
				m_iUV_Cur_Height_Num++;
			}
		}
	}
	else if (m_iBirthType == 3)
	{
		m_fTime += (_float)dTimeDelta;

		if (m_fTime > 1.f)
			m_fTime = 1.f;

		m_vPSize += _float2(_float(dTimeDelta) * 5.f, _float(dTimeDelta)* 5.f);

		m_iPlayOnFrameCnt++;

		if (m_iPlayOnFrameCnt == m_iFrameCnt)
		{
			if (m_iUV_Cur_Height_Num == m_iUV_Max_Height_Num - 1 && m_iUV_Cur_Width_Num == m_iUV_Max_Width_Num - 1)
			{
				Set_Dead(true);
				return;
			}

			m_iUV_Cur_Width_Num++;

			m_iPlayOnFrameCnt = 0;

			if (m_iUV_Cur_Width_Num == m_iUV_Max_Width_Num)
			{
				m_iUV_Cur_Width_Num = 0;
				m_iUV_Cur_Height_Num++;
			}
		}
	}

	else if (m_iBirthType == 4)
	{
		m_fTime += (_float)dTimeDelta;

		if (m_fTime > 1.f)
			m_fTime = 1.f;

		m_vPSize += _float2(_float(dTimeDelta) * 3.f, _float(dTimeDelta)* 3.f);
		if (m_pMonster->Check_Dead() == false)
		{
			_float4 Pos = m_pMonster->Get_TransformState(CTransform::STATE_TRANSLATION);
			m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, Pos);
		}
		m_iPlayOnFrameCnt++;

		if (m_iPlayOnFrameCnt == m_iFrameCnt)
		{
			if (m_iUV_Cur_Height_Num == m_iUV_Max_Height_Num - 1 && m_iUV_Cur_Width_Num == m_iUV_Max_Width_Num - 1)
			{
				m_iUV_Cur_Height_Num = 0;
				m_iUV_Cur_Width_Num = 0;
			}

			m_iUV_Cur_Width_Num++;

			m_iPlayOnFrameCnt = 0;

			if (m_iUV_Cur_Width_Num == m_iUV_Max_Width_Num)
			{
				m_iUV_Cur_Width_Num = 0;
				m_iUV_Cur_Height_Num++;
			}
		}
	}
	

}

void CFlame_Bullet_Dead::Late_Tick(_double dTimeDelta)
{
	__super::Late_Tick(dTimeDelta);

	__super::Compute_CamDistance();

	if (m_pRendererCom != nullptr)
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_EFFECT, this);
}

HRESULT CFlame_Bullet_Dead::Render()
{
	FAILED_CHECK_RETURN(__super::Render(), E_FAIL);

	FAILED_CHECK_RETURN(SetUp_ShaderResources(), E_FAIL);



	if(m_iBirthType == 0)
		m_pShaderCom->Begin(15);
	else if(m_iBirthType == 4)
		m_pShaderCom->Begin(16);
	else
		m_pShaderCom->Begin(14);



	m_pPointBuffer->Render();

	return S_OK;
}

HRESULT CFlame_Bullet_Dead::SetUp_Component()
{
	FAILED_CHECK_RETURN(__super::Add_Component(CGameInstance::Get_StaticLevelIndex(), L"Prototype_Component_Renderer", L"Com_Renderer", (CComponent**)&m_pRendererCom, this), E_FAIL);
	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Shader_VtxPointInstance", L"Com_Shader", (CComponent**)&m_pShaderCom, this), E_FAIL);
	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_VIBuffer_Bullet_Instancing", L"Com_VIBuffer", (CComponent**)&m_pPointBuffer, this), E_FAIL);
	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Texture_Default_FlameBullet_Tex", L"Com_Texture", (CComponent**)&m_pTextureCom, this), E_FAIL);

	return S_OK;
}

HRESULT CFlame_Bullet_Dead::SetUp_ShaderResources()
{
	
	NULL_CHECK_RETURN(m_pShaderCom, E_FAIL);

	FAILED_CHECK_RETURN(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, L"g_WorldMatrix"), E_FAIL);
	FAILED_CHECK_RETURN(m_pShaderCom->Set_Matrix(L"g_ViewMatrix", &CGameInstance::GetInstance()->Get_TransformFloat4x4(CPipeLine::D3DTS_VIEW)), E_FAIL);
	FAILED_CHECK_RETURN(m_pShaderCom->Set_Matrix(L"g_ProjMatrix", &CGameInstance::GetInstance()->Get_TransformFloat4x4(CPipeLine::D3DTS_PROJ)), E_FAIL);
	FAILED_CHECK_RETURN(m_pShaderCom->Set_RawValue(L"g_vPSize", &m_vPSize, sizeof(_float2)), E_FAIL);
	FAILED_CHECK_RETURN(m_pShaderCom->Set_RawValue(L"g_iUV_Max_Width_Num", &m_iUV_Max_Width_Num, sizeof(_int)), E_FAIL);
	FAILED_CHECK_RETURN(m_pShaderCom->Set_RawValue(L"g_iUV_Max_Height_Num", &m_iUV_Max_Height_Num, sizeof(_int)), E_FAIL);
	FAILED_CHECK_RETURN(m_pShaderCom->Set_RawValue(L"g_iUV_Cur_Width_Num", &m_iUV_Cur_Width_Num, sizeof(_int)), E_FAIL);
	FAILED_CHECK_RETURN(m_pShaderCom->Set_RawValue(L"g_iUV_Cur_Height_Num", &m_iUV_Cur_Height_Num, sizeof(_int)), E_FAIL);
	FAILED_CHECK_RETURN(m_pShaderCom->Set_ShaderResourceView(L"g_DepthTexture", CGameInstance::GetInstance()->Get_DepthTargetSRV()), E_FAIL);


	if (FAILED(m_pShaderCom->Set_RawValue(L"g_fTime", &m_fTime, sizeof(_float))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_RawValue(L"g_vCamPosition", &CGameInstance::GetInstance()->Get_CamPos(), sizeof(_float4))))
		return E_FAIL;

	FAILED_CHECK_RETURN(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, L"g_Texture", (_uint)m_iBirthType), E_FAIL);
	FAILED_CHECK_RETURN(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, L"g_GlowTexture", (_uint)2), E_FAIL);

	return S_OK;
}

CFlame_Bullet_Dead* CFlame_Bullet_Dead::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CFlame_Bullet_Dead*		pInstance = new CFlame_Bullet_Dead(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CFlame_Bullet_Dead");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CFlame_Bullet_Dead::Clone(const wstring& wstrPrototypeTag, void* pArg)
{
	CFlame_Bullet_Dead*		pInstance = new CFlame_Bullet_Dead(*this);

	if (FAILED(pInstance->Initialize_Clone(wstrPrototypeTag, pArg)))
	{
		MSG_BOX("Failed to Cloned : CFlame_Bullet_Dead");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CFlame_Bullet_Dead::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
	Safe_Release(m_pPointBuffer);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pRendererCom);
}
