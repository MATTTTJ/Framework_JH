#include "stdafx.h"
#include "..\public\Monster.h"
#include "GameInstance.h"
#include "GameUtils.h"
#include "State.h"
#include "UI.h"
#include "Collider.h"
#include "Player.h"

CMonster::CMonster(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CGameObject(pDevice, pContext)
{

}

CMonster::CMonster(const CMonster & rhs)
	: CGameObject(rhs)
{
	m_vOnAimOutLineColor =  _float4{ 0.98f, 0.35f, 0.15f, 1.f };
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

	_float4	fDir;


	list<CGameObject*>* CloneMonsters = CGameInstance::GetInstance()->Get_CloneObjectList(LEVEL_GAMEPLAY, L"Layer_Monster");
	if (CloneMonsters == nullptr || CloneMonsters->size() == 0)
		return;
	else
	{
		for (auto& pMonster : *CloneMonsters)
		{
			CMonster* ppMonster = dynamic_cast<CMonster*>(pMonster);

			if (ppMonster == this || ppMonster== nullptr)
				continue;

		
			if (m_pColliderCom[COLLTYPE_HITBODY]->Get_ColliderType() != CCollider::COLLIDER_SPHERE)
				continue;

			if (ppMonster->Get_CollPtr(CMonster::COLLTYPE_HITBODY)->Get_ColliderType() == CCollider::COLLIDER_SPHERE && 
				CGameUtils::CollisionSphereSphere(m_pColliderCom[CMonster::COLLTYPE_HITBODY], ppMonster->Get_CollPtr(CMonster::COLLTYPE_HITBODY), fDir))
			{
				_vector vPos = m_pTransformCom->Get_State(CTransform::STATE_TRANSLATION);
				fDir.y = 0.f;
				_vector	vMovePos = XMVectorAdd(vPos, fDir);

				_float4 vBlockedLine = { 0.f, 0.f, 0.f, 0.f };
				_float4 vBlockedLineNormal = { 0.f ,0.f, 0.f, 0.f };

				if(m_pNavigationCom == nullptr)
				{
					m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, vPos);
					continue;
				}

				if (true == m_pNavigationCom->IsMove_OnNavigation(vPos, vBlockedLine, vBlockedLineNormal))
					m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, vMovePos);
				else
				{
					_vector vInDir = vMovePos - vPos;
					_vector vOutDir = vPos - vMovePos;
					_float	fLength = XMVectorGetX(XMVector3Dot(vOutDir, vBlockedLineNormal));

					_vector vSlidingDir = vInDir + XMLoadFloat4(&vBlockedLineNormal) * fLength;

					vMovePos = vPos + vSlidingDir;

					if (m_pNavigationCom->IsMove_OnNavigation(vMovePos, vBlockedLine, vBlockedLineNormal))
					{
						m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, vMovePos);
					}
				}
			}
		}

	}

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
