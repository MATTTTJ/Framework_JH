#include "stdafx.h"
#include "..\public\Bullet.h"
#include "GameInstance.h"
#include "Monster.h"
#include "Player.h"
#include "Collider.h"
#include "DamageFont_Mgr.h"
#include "Default_Bullet_Dead.h"
#include "Normal_Boss.h"

CBullet::CBullet(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CGameObject(pDevice,pContext)
{
}

CBullet::CBullet(const CBullet& rhs)
	:CGameObject(rhs)
{
}

HRESULT CBullet::Initialize_Prototype()
{
	FAILED_CHECK_RETURN(__super::Initialize_Prototype(), E_FAIL);

	return S_OK;
}

HRESULT CBullet::Initialize_Clone(const wstring& wstrPrototypeTag, void* pArg)
{
	FAILED_CHECK_RETURN(__super::Initialize_Clone(wstrPrototypeTag, pArg), E_FAIL);
	FAILED_CHECK_RETURN(SetUp_Components(), E_FAIL);

	return S_OK;
}

void CBullet::Tick(_double dTimeDelta)
{
	__super::Tick(dTimeDelta);

	_float4 Pos = m_pTransformCom->Get_State(CTransform::STATE_TRANSLATION);
	if(Pos.x >= 300.f || Pos.y >= 80.f || Pos.z >= 300.f)
	{
		Set_Dead(true);
	}
	if (Pos.x <= -300.f || Pos.y <= -80.f || Pos.z <= -300.f)
	{
		Set_Dead(true);
	}
}

void CBullet::Late_Tick(_double dTimeDelta)
{
	__super::Late_Tick(dTimeDelta);

	if (m_tBulletOption.m_eOwner == OWNER_PLAYER)
	{
		Collision_Body();
		Collision_Head();
		Collision_HideCollider();
		Collision_Shield();
		Collision_LArm();
		Collision_RArm();

		if (dynamic_cast<CPlayer*>(m_pOwner)->Get_CurRoomType(CPlayer::ROOM_BOSS) == true)
		{
			Collision_To_BossMonster();
		}
	}
	

	if (nullptr != m_pRendererCom &&
		true == CGameInstance::GetInstance()->isInFrustum_WorldSpace(m_pTransformCom->Get_State(CTransform::STATE_TRANSLATION), 2.f))
	{
		if(m_tBulletOption.m_eOwner == OWNER_PLAYER)
		{
			// m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_EFFECT, this);
		}
		else if(m_tBulletOption.m_eOwner == OWNER_MONSTER)
		{
			m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHABLEND, this);
		}
		else if (m_tBulletOption.m_eOwner == OWNER_BOSS)
		{
			
		}

		m_pRendererCom->Add_DebugRenderGroup(m_pBulletColliderCom);
		
	}

	
}

HRESULT CBullet::Render()
{
	FAILED_CHECK_RETURN(__super::Render(), E_FAIL);

	return S_OK;
}

_bool CBullet::Collision_Body()
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	// 몬스터 리스트를 가져와서 순회해야할것같음
	list<CGameObject*>* CloneMonsters = CGameInstance::GetInstance()->Get_CloneObjectList(LEVEL_GAMEPLAY, L"Layer_Monster");
	if (CloneMonsters == nullptr || CloneMonsters->size() == 0)
		return false;
	else
	{
		m_MonsterList = *pGameInstance->Get_CloneObjectList(LEVEL_GAMEPLAY, L"Layer_Monster");
		_int ListSize = 0;
		ListSize = (_int)m_MonsterList.size();

		if (ListSize != 0)
		{
			auto iter = m_MonsterList.begin();
			for (auto& iter = m_MonsterList.begin(); iter != m_MonsterList.end();)
			{
				if (iter == m_MonsterList.end())
					return false;

				CCollider* pCollider = (CCollider*)(*iter)->Find_Component(L"Com_HitBodySphere");

				if (pCollider == nullptr)
					++iter;

				else if (m_pBulletColliderCom->Collision(pCollider))
				{
					CMonster* pMonster = (CMonster*)pCollider->Get_Owner();
					NULL_CHECK_RETURN(pMonster, false);

					pMonster->Set_HitColor();
					pMonster->Collision_Body(this); // 총알이 어디 충돌했는지 판단하니까
					if (Check_Dead() == false && m_bIsClone == true)
					{
						CDefault_Bullet_Dead::EFFECTDESC EffectDesc;
						_float4 Position;
						XMStoreFloat4(&Position, m_pTransformCom->Get_State(CTransform::STATE_TRANSLATION));

						EffectDesc.m_tGameObjectDesc.TransformDesc.vInitPos = _float3(Position.x, Position.y, Position.z);
						EffectDesc.m_tGameObjectDesc.m_vBulletLook = XMVector3Normalize(CGameInstance::GetInstance()->Get_CamLook());
						CDefault_Bullet_Dead* pEffect = nullptr;
						pEffect = (CDefault_Bullet_Dead*)(CGameInstance::GetInstance()->Clone_GameObjectReturnPtr(LEVEL_GAMEPLAY, L"Layer_Effect", L"Prototype_GameObject_Effect_Dust", &EffectDesc));
						pEffect = (CDefault_Bullet_Dead*)(CGameInstance::GetInstance()->Clone_GameObjectReturnPtr(LEVEL_GAMEPLAY, L"Layer_Effect", L"Prototype_GameObject_Effect_Default_Bullet_Dead", &EffectDesc));



						// Create_DamageFont();
					}

					Set_Dead(true);

					// m_bCollOnce = true;
					return true;
				}
				else
					++iter;
			}
			return false;
		}
	}
	
	return false;
}

_bool CBullet::Collision_Head()
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	// 몬스터 리스트를 가져와서 순회해야할것같음
	list<CGameObject*>* CloneMonsters = CGameInstance::GetInstance()->Get_CloneObjectList(LEVEL_GAMEPLAY, L"Layer_Monster");
	if (CloneMonsters == nullptr || CloneMonsters->size() == 0)
		return false;
	else
	{
		m_MonsterList = *pGameInstance->Get_CloneObjectList(LEVEL_GAMEPLAY, L"Layer_Monster");
		_int ListSize = 0;
		ListSize = (_int)m_MonsterList.size();

		if (ListSize != 0)
		{
			auto iter = m_MonsterList.begin();
			for (auto& iter = m_MonsterList.begin(); iter != m_MonsterList.end();)
			{
				if (iter == m_MonsterList.end())
					return false;

				CCollider* pCollider = (CCollider*)(*iter)->Find_Component(L"Com_HitHeadSphere");

				if (pCollider == nullptr)
					++iter;

				else if (m_pBulletColliderCom->Collision(pCollider))
				{
					CMonster* pMonster = (CMonster*)pCollider->Get_Owner();
					NULL_CHECK_RETURN(pMonster, false);

					pMonster->Set_HitColor();
					pMonster->Collision_Head(this); // 총알이 어디 충돌했는지 판단하니까
					Set_Dead(true);

					// m_bCollOnce = true;
					return true;
				}
				else
					++iter;
			}
			return false;
		}
	}

	return false;
}

_bool CBullet::Collision_HideCollider()
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	// 몬스터 리스트를 가져와서 순회해야할것같음
	list<CGameObject*>* CloneMonsters = CGameInstance::GetInstance()->Get_CloneObjectList(LEVEL_GAMEPLAY, L"Layer_Monster");
	if (CloneMonsters == nullptr || CloneMonsters->size() == 0)
		return false;
	else
	{
		m_MonsterList = *pGameInstance->Get_CloneObjectList(LEVEL_GAMEPLAY, L"Layer_Monster");
		_int ListSize = 0;
		ListSize = (_int)m_MonsterList.size();

		if (ListSize != 0)
		{
			auto iter = m_MonsterList.begin();
			for (auto& iter = m_MonsterList.begin(); iter != m_MonsterList.end();)
			{
				if (iter == m_MonsterList.end())
					return false;

				CCollider* pCollider = (CCollider*)(*iter)->Find_Component(L"Com_AttackRangeSphere");

				if (pCollider == nullptr)
					++iter;
				else if (m_pBulletColliderCom->Collision(pCollider))
				{
					CMonster* pMonster = (CMonster*)pCollider->Get_Owner();
					NULL_CHECK_RETURN(pMonster, false);

					pMonster->Collision_Hide(this); // 총알이 어디 충돌했는지 판단하니까

					return true;
				}
				else
					++iter;
			}
			return false;
		}
		return false;

	}
}

_bool CBullet::Collision_Shield()
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	// 몬스터 리스트를 가져와서 순회해야할것같음
	list<CGameObject*>* CloneMonsters = CGameInstance::GetInstance()->Get_CloneObjectList(LEVEL_GAMEPLAY, L"Layer_Monster");
	if (CloneMonsters == nullptr || CloneMonsters->size() == 0)
		return false;
	else
	{
		m_MonsterList = *pGameInstance->Get_CloneObjectList(LEVEL_GAMEPLAY, L"Layer_Monster");
		_int ListSize = 0;
		ListSize = (_int)m_MonsterList.size();

		if (ListSize != 0)
		{
			auto iter = m_MonsterList.begin();
			for (auto& iter = m_MonsterList.begin(); iter != m_MonsterList.end();)
			{
				if (iter == m_MonsterList.end())
					return false;

				CCollider* pCollider = (CCollider*)(*iter)->Find_Component(L"Com_Shield");

				if (pCollider == nullptr)
					++iter;

				else if (m_pBulletColliderCom->Collision(pCollider))
				{
					CMonster* pMonster = (CMonster*)pCollider->Get_Owner();
					NULL_CHECK_RETURN(pMonster, false);

					pMonster->Set_HitColor();
					pMonster->Collision_Shield(this); // 총알이 어디 충돌했는지 판단하니까
					Set_Dead(true);

					// m_bCollOnce = true;
					return true;
				}
				else
					++iter;
			}
			return false;
		}
	}

	return false;
}

_bool CBullet::Collision_LArm()
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	// 몬스터 리스트를 가져와서 순회해야할것같음
	list<CGameObject*>* CloneMonsters = CGameInstance::GetInstance()->Get_CloneObjectList(LEVEL_GAMEPLAY, L"Layer_Monster");
	if (CloneMonsters == nullptr || CloneMonsters->size() == 0)
		return false;
	else
	{
		m_MonsterList = *pGameInstance->Get_CloneObjectList(LEVEL_GAMEPLAY, L"Layer_Monster");
		_int ListSize = 0;
		ListSize = (_int)m_MonsterList.size();

		if (ListSize != 0)
		{
			auto iter = m_MonsterList.begin();
			for (auto& iter = m_MonsterList.begin(); iter != m_MonsterList.end();)
			{
				if (iter == m_MonsterList.end())
					return false;

				CCollider* pCollider = (CCollider*)(*iter)->Find_Component(L"Com_LArmSphere");

				if (pCollider == nullptr)
					++iter;

				else if (m_pBulletColliderCom->Collision(pCollider))
				{
					CMonster* pMonster = (CMonster*)pCollider->Get_Owner();
					NULL_CHECK_RETURN(pMonster, false);

					pMonster->Set_HitColor();
					pMonster->Collision_Armor(this); // 총알이 어디 충돌했는지 판단하니까
					Set_Dead(true);

					// m_bCollOnce = true;
					return true;
				}
				else
					++iter;
			}
			return false;
		}
	}

	return false;
}

_bool CBullet::Collision_RArm()
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	// 몬스터 리스트를 가져와서 순회해야할것같음
	list<CGameObject*>* CloneMonsters = CGameInstance::GetInstance()->Get_CloneObjectList(LEVEL_GAMEPLAY, L"Layer_Monster");
	if (CloneMonsters == nullptr || CloneMonsters->size() == 0)
		return false;
	else
	{
		m_MonsterList = *pGameInstance->Get_CloneObjectList(LEVEL_GAMEPLAY, L"Layer_Monster");
		_int ListSize = 0;
		ListSize = (_int)m_MonsterList.size();

		if (ListSize != 0)
		{
			auto iter = m_MonsterList.begin();
			for (auto& iter = m_MonsterList.begin(); iter != m_MonsterList.end();)
			{
				if (iter == m_MonsterList.end())
					return false;

				CCollider* pCollider = (CCollider*)(*iter)->Find_Component(L"Com_RArmSphere");

				if (pCollider == nullptr)
					++iter;

				else if (m_pBulletColliderCom->Collision(pCollider))
				{
					CMonster* pMonster = (CMonster*)pCollider->Get_Owner();
					NULL_CHECK_RETURN(pMonster, false);

					pMonster->Set_HitColor();
					pMonster->Collision_Armor(this); // 총알이 어디 충돌했는지 판단하니까
					Set_Dead(true);
					// m_bCollOnce = true;
					return true;
				}
				else
					++iter;
			}
			return false;
		}
	}
	return false;
}

_bool CBullet::Collision_To_BossMonster()
{
	if ((CGameInstance::GetInstance()->Get_CloneObjectList(LEVEL_GAMEPLAY, L"Layer_ZBossMonster")->front()) == nullptr)
		return false;

	CMonster* pMonster = (CMonster*)(CGameInstance::GetInstance()->Get_CloneObjectList(LEVEL_GAMEPLAY, L"Layer_ZBossMonster")->front());

	if (pMonster != nullptr)
	{
		CCollider* pCollider = (CCollider*)(CGameInstance::GetInstance()->Get_CloneObjectList(LEVEL_GAMEPLAY, L"Layer_ZBossMonster")->front())->Find_Component(L"Com_BossDownBodyCom");
		NULL_CHECK_RETURN(pCollider, false);
		if (m_pBulletColliderCom->Collision(pCollider))
		{
			CMonster* pMonster = (CMonster*)pCollider->Get_Owner();
			NULL_CHECK_RETURN(pMonster, false);
			pMonster->Collision_Body(this); // 총알이 어디 충돌했는지 판단하니까
			Set_Dead(true);
			// m_bCollOnce = true;
			return true;
		}

		pCollider = (CCollider*)(CGameInstance::GetInstance()->Get_CloneObjectList(LEVEL_GAMEPLAY, L"Layer_ZBossMonster")->front())->Find_Component(L"Com_HitHeadSphere");
		NULL_CHECK_RETURN(pCollider, false);
		if (m_pBulletColliderCom->Collision(pCollider))
		{
			CMonster* pMonster = (CMonster*)pCollider->Get_Owner();
			NULL_CHECK_RETURN(pMonster, false);
			pMonster->Collision_Body(this); // 총알이 어디 충돌했는지 판단하니까
			Set_Dead(true);
			// m_bCollOnce = true;
			return true;
		}

		pCollider = (CCollider*)(CGameInstance::GetInstance()->Get_CloneObjectList(LEVEL_GAMEPLAY, L"Layer_ZBossMonster")->front())->Find_Component(L"Com_LArmSphere");
		NULL_CHECK_RETURN(pCollider, false);
		if (m_pBulletColliderCom->Collision(pCollider))
		{
			CMonster* pMonster = (CMonster*)pCollider->Get_Owner();
			NULL_CHECK_RETURN(pMonster, false);
			pMonster->Collision_Body(this); // 총알이 어디 충돌했는지 판단하니까
			Set_Dead(true);
			// m_bCollOnce = true;
			return true;
		}

		pCollider = (CCollider*)(CGameInstance::GetInstance()->Get_CloneObjectList(LEVEL_GAMEPLAY, L"Layer_ZBossMonster")->front())->Find_Component(L"Com_RArmSphere");
		NULL_CHECK_RETURN(pCollider, false);
		if (m_pBulletColliderCom->Collision(pCollider))
		{
			CMonster* pMonster = (CMonster*)pCollider->Get_Owner();
			NULL_CHECK_RETURN(pMonster, false);
			pMonster->Collision_Body(this); // 총알이 어디 충돌했는지 판단하니까
			Set_Dead(true);
			// m_bCollOnce = true;
			return true;
		}

		pCollider = (CCollider*)(CGameInstance::GetInstance()->Get_CloneObjectList(LEVEL_GAMEPLAY, L"Layer_ZBossMonster")->front())->Find_Component(L"Com_RHandSphere");
		NULL_CHECK_RETURN(pCollider, false);
		if (m_pBulletColliderCom->Collision(pCollider))
		{
			CMonster* pMonster = (CMonster*)pCollider->Get_Owner();
			NULL_CHECK_RETURN(pMonster, false);
			pMonster->Collision_Body(this); // 총알이 어디 충돌했는지 판단하니까
			Set_Dead(true);
			// m_bCollOnce = true;
			return true;
		}

		pCollider = (CCollider*)(CGameInstance::GetInstance()->Get_CloneObjectList(LEVEL_GAMEPLAY, L"Layer_ZBossMonster")->front())->Find_Component(L"Com_LHandSphere");
		NULL_CHECK_RETURN(pCollider, false);
		if (m_pBulletColliderCom->Collision(pCollider))
		{
			CMonster* pMonster = (CMonster*)pCollider->Get_Owner();
			NULL_CHECK_RETURN(pMonster, false);
			pMonster->Collision_Body(this); // 총알이 어디 충돌했는지 판단하니까
			Set_Dead(true);
			// m_bCollOnce = true;
			return true;
		}

		pCollider = (CCollider*)(CGameInstance::GetInstance()->Get_CloneObjectList(LEVEL_GAMEPLAY, L"Layer_ZBossMonster")->front())->Find_Component(L"Com_ForeRSphere");
		NULL_CHECK_RETURN(pCollider, false);
		if (m_pBulletColliderCom->Collision(pCollider))
		{
			CMonster* pMonster = (CMonster*)pCollider->Get_Owner();
			NULL_CHECK_RETURN(pMonster, false);
			pMonster->Collision_Body(this); // 총알이 어디 충돌했는지 판단하니까
			Set_Dead(true);
			// m_bCollOnce = true;
			return true;
		}

		pCollider = (CCollider*)(CGameInstance::GetInstance()->Get_CloneObjectList(LEVEL_GAMEPLAY, L"Layer_ZBossMonster")->front())->Find_Component(L"Com_ForeLSphere");
		NULL_CHECK_RETURN(pCollider, false);
		if (m_pBulletColliderCom->Collision(pCollider))
		{
			CMonster* pMonster = (CMonster*)pCollider->Get_Owner();
			NULL_CHECK_RETURN(pMonster, false);
			pMonster->Collision_Body(this); // 총알이 어디 충돌했는지 판단하니까
			Set_Dead(true);
			// m_bCollOnce = true;
			return true;
		}

		pCollider = (CCollider*)(CGameInstance::GetInstance()->Get_CloneObjectList(LEVEL_GAMEPLAY, L"Layer_ZBossMonster")->front())->Find_Component(L"Com_ELBOW_RSphere");
		NULL_CHECK_RETURN(pCollider, false);
		if (m_pBulletColliderCom->Collision(pCollider))
		{
			CMonster* pMonster = (CMonster*)pCollider->Get_Owner();
			NULL_CHECK_RETURN(pMonster, false);
			pMonster->Collision_Body(this); // 총알이 어디 충돌했는지 판단하니까
			Set_Dead(true);
			// m_bCollOnce = true;
			return true;
		}

		pCollider = (CCollider*)(CGameInstance::GetInstance()->Get_CloneObjectList(LEVEL_GAMEPLAY, L"Layer_ZBossMonster")->front())->Find_Component(L"Com_ELBOW_LSphere");
		NULL_CHECK_RETURN(pCollider, false);
		if (m_pBulletColliderCom->Collision(pCollider))
		{
			CMonster* pMonster = (CMonster*)pCollider->Get_Owner();
			NULL_CHECK_RETURN(pMonster, false);
			pMonster->Collision_Body(this); // 총알이 어디 충돌했는지 판단하니까
			Set_Dead(true);
			// m_bCollOnce = true;
			return true;
		}

		pCollider = (CCollider*)(CGameInstance::GetInstance()->Get_CloneObjectList(LEVEL_GAMEPLAY, L"Layer_ZBossMonster")->front())->Find_Component(L"Com_BossUpBodyCom");
		NULL_CHECK_RETURN(pCollider, false);
		if (m_pBulletColliderCom->Collision(pCollider))
		{
			CMonster* pMonster = (CMonster*)pCollider->Get_Owner();
			NULL_CHECK_RETURN(pMonster, false);
			pMonster->Collision_Body(this); // 총알이 어디 충돌했는지 판단하니까
			Set_Dead(true);
			// m_bCollOnce = true;
			return true;
		}

		pCollider = (CCollider*)(CGameInstance::GetInstance()->Get_CloneObjectList(LEVEL_GAMEPLAY, L"Layer_ZBossMonster")->front())->Find_Component(L"Com_HeartSphere");
		NULL_CHECK_RETURN(pCollider, false);
		if (m_pBulletColliderCom->Collision(pCollider))
		{
			CMonster* pMonster = (CMonster*)pCollider->Get_Owner();
			NULL_CHECK_RETURN(pMonster, false);
			pMonster->Collision_Head(this); // 총알이 어디 충돌했는지 판단하니까
			Set_Dead(true);
			// m_bCollOnce = true;
			return true;
		}
	}
	else 
		return false;
}

_bool CBullet::Collision_To_Player(CCollider* pBulletCollider)
{
	NULL_CHECK_RETURN(m_pOwner, false);

	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	CCollider* pCollider = (CCollider*)pGameInstance->Get_ComponentPtr(LEVEL_GAMEPLAY, L"Layer_Player", L"Com_BODYSPHERE");
	NULL_CHECK_RETURN(pCollider, false);

	if (pBulletCollider->Collision(pCollider))
	{
		CPlayer* pPlayer = dynamic_cast<CPlayer*>(pCollider->Get_Owner());
		NULL_CHECK_RETURN(pPlayer, false);
		pPlayer->Collision_Event(dynamic_cast<CMonster*>(m_pOwner));
		Set_Dead(true);

		return true;
	}
	return false;
}

HRESULT CBullet::SetUp_Components()
{

	CCollider::COLLIDERDESC			ColliderDesc;

	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));
	ColliderDesc.vPosition = _float3(0.f, 0.f, 0.f);
	ColliderDesc.vSize = _float3(0.3f, 0.3f, 0.5f);
	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Collider_SPHERE", L"Com_BulletSPHERE", (CComponent**)&m_pBulletColliderCom, this, &ColliderDesc), E_FAIL);

	return S_OK;
}

HRESULT CBullet::SetUp_ShaderResources()
{
	NULL_CHECK_RETURN(m_pShaderCom, E_FAIL);

	FAILED_CHECK_RETURN(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, L"g_WorldMatrix"), E_FAIL);
	FAILED_CHECK_RETURN(m_pShaderCom->Set_Matrix(L"g_ViewMatrix", &CGameInstance::GetInstance()->Get_TransformFloat4x4(CPipeLine::D3DTS_VIEW)), E_FAIL);
	FAILED_CHECK_RETURN(m_pShaderCom->Set_Matrix(L"g_ProjMatrix", &CGameInstance::GetInstance()->Get_TransformFloat4x4(CPipeLine::D3DTS_PROJ)), E_FAIL);

	return S_OK;
}

void CBullet::Create_DamageFont()
{

	// string s = to_string(m_tBulletOption.BulletDesc.m_iDamage);
	// _float4 vPos = m_pTransformCom->Get_State(CTransform::STATE_TRANSLATION);
	// _float3 Right = XMVector3Normalize(m_pTransformCom->Get_State(CTransform::STATE_RIGHT));
	// _float4 Dir;
	// _float4 MinVec{ -1.f,-1.f,-1.f,0.f };
	// _float4 MaxVec{ 1.f,1.f,1.f,0.f };
	// CGameUtils::GetRandomVector(&Dir, &MinVec, &MaxVec);
	// vector<int> v, v2;
	//
	// for (char c : s)
	// {
	// 	v.push_back(c);
	// }
	//
	// for (int i = 0; i < v.size(); i++)
	// {
	// 	v2.push_back(s[i] - '0');
	// }
	//
	// for (int i = 0; i < v2.size(); ++i)
	// {
	// 	for (int Number : v2)
	// 	{
	// 		if (v2[i] != Number)
	// 			continue;
	//
	// 		GAMEOBJECTDESC		tDamageFontDesc;
	// 		if (v2.size() == 1)
	// 			tDamageFontDesc.TransformDesc.vInitPos = _float3(vPos.x, vPos.y, vPos.z);
	// 		else
	// 			tDamageFontDesc.TransformDesc.vInitPos = _float3(vPos.x, vPos.y, vPos.z) + (i* Right);
	//
	// 		tDamageFontDesc.m_iNumCnt = i;
	// 		tDamageFontDesc.m_iNumber = Number;
	// 		// tmp.m_vNumColor = vColor;
	// 		tDamageFontDesc.m_vTexSize = _float2(1.f, 1.f);
	// 		tDamageFontDesc.m_iCountType = 1;
	// 		tDamageFontDesc.m_vBulletLook = XMVector3Normalize(Dir);
	// 		CDamageFont_Mgr* pMgr = CDamageFont_Mgr::GetInstance();
	//
	// 		pMgr->Add_DamageFont(LEVEL_GAMEPLAY, L"Layer_DamageFont", &tDamageFontDesc);
	// 	}
	// }
}


void CBullet::Free()
{
	__super::Free();
	
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pBulletColliderCom);
	Safe_Release(m_pBoomColliderCom);
	Safe_Release(m_pBladeColliderCom);
	Safe_Release(m_pPointBuffer);
	Safe_Release(m_pModelCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pShaderCom);

	// CDamageFont_Mgr::GetInstance()->DestroyInstance();

}
