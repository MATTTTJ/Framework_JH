#include "stdafx.h"
#include "..\Public\Effect_Manager.h"
#include "GameInstance.h"
#include "Json_Utils.h"

IMPLEMENT_SINGLETON(CEffect_Manager);

CEffect_Manager::CEffect_Manager()
{
}

HRESULT CEffect_Manager::Initialize(LEVEL eLevel)
{
	FAILED_CHECK_RETURN(CreatEffect(eLevel), E_FAIL);

	return S_OK;
}

CEffect* CEffect_Manager::GetEffect(LEVEL eLevel, const wstring wstrEffectTag)
{
	return Find_Effect(eLevel, wstrEffectTag);
}

void CEffect_Manager::Reset(LEVEL eLevel)
{
	for(auto pair : m_mapEffects)
	{
		pair.second.clear();
	}
	m_mapEffects.clear();

	CreatEffect(eLevel);
}

HRESULT CEffect_Manager::CreatEffect(LEVEL eLevel)
{
	LoadEffectData(eLevel);

	return S_OK;
}

CEffect* CEffect_Manager::Find_Effect(LEVEL eLevel, const wstring wstrEffectTag)
{
	auto Effects = m_mapEffects.find(wstrEffectTag);

	if (Effects == m_mapEffects.end())
		return nullptr;
	// 맞는 태그 찾았으면 주소 줘
	EFFECTS* Effectlist = &Effects->second;

	for(auto iter :*Effectlist)
	{
		// 사용가능한 상태(대기상태) 인지 체크
		if (iter->IsDisabled())
			return iter;
	}

	CEffect::EFFECTINFO		tInfo = Effectlist->front()->GetEffectInfo();

	return Add_Effect(eLevel, tInfo, Effectlist);
}

CEffect* CEffect_Manager::Add_Effect(LEVEL eLevel, CEffect::EFFECTINFO tInfo, list<CEffect*>* pEffectList)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();

	CEffect* pEffect = nullptr;

	if(CEffect::EFFECT_POINT == tInfo.eType)
	{
		pEffect = dynamic_cast<CEffect*>(pGameInstance->Clone_GameObjectReturnPtr(eLevel, L"Layer_Effect", L"Prototype_GameObject_Effect_Point", &tInfo));
		NULL_CHECK_RETURN(pEffect, nullptr);
	}
	else if(CEffect::EFFECT_MESH == tInfo.eType)
	{
		pEffect = dynamic_cast<CEffect*>(pGameInstance->Clone_GameObjectReturnPtr(eLevel, L"Layer_Effect", L"Prototype_GameObject_Effect_Mesh", &tInfo));
		NULL_CHECK_RETURN(pEffect, nullptr);
	}
	else if (CEffect::EFFECT_PARTICLE_POINT == tInfo.eType)
	{
		pEffect = dynamic_cast<CEffect*>(pGameInstance->Clone_GameObjectReturnPtr(eLevel, L"Layer_Effect", L"Prototype_GameObject_Effect_Point_Instance", &tInfo));
		NULL_CHECK_RETURN(pEffect, nullptr);
	}
	else if (CEffect::EFFECT_PARTICLE_RECT == tInfo.eType)
	{
		pEffect = dynamic_cast<CEffect*>(pGameInstance->Clone_GameObjectReturnPtr(eLevel, L"Layer_Effect", L"Prototype_GameObject_Effect_Rect_Instance", &tInfo));
		NULL_CHECK_RETURN(pEffect, nullptr);
	}
	else if (CEffect::EFFECT_PARTICLE_MESH == tInfo.eType)
	{
		pEffect = dynamic_cast<CEffect*>(pGameInstance->Clone_GameObjectReturnPtr(eLevel, L"Layer_Effect", L"Prototype_GameObject_Effect_Mesh_Instance", &tInfo));
		NULL_CHECK_RETURN(pEffect, nullptr);
	}

	pEffectList->push_back(pEffect);

	return pEffect;
}

void CEffect_Manager::LoadEffectData(LEVEL eLevel)
{
	_tchar		szJsonFileName[MAX_PATH] = TEXT("Effect_Data");

	json	Json;

	if (FAILED(CJson_Utils::Load_Json(CJson_Utils::Complete_Path(szJsonFileName).c_str(), &Json)))
		return;

	json	JsonData = Json["data"];
	for (_uint i = 0; i < JsonData.size(); ++i)
	{
		CEffect::EFFECTINFO tEffect;
		ZeroMemory(&tEffect, sizeof(CEffect::EFFECTINFO));

		string szEffectTag = JsonData[i]["pEffectTag"];
		wstring wszEffectTag = L"";
		wszEffectTag.assign(szEffectTag.begin(), szEffectTag.end());
		wcscpy_s(tEffect.pEffectTag, wszEffectTag.c_str());
		// wcscpy_s(tEffect.pEffectTag, wstring(szEffectTag.begin(), szEffectTag.end()).c_str());
		//tEffect.pEffectTag = wstring(szEffectTag.begin(), szEffectTag.end()).c_str();

		//tEffect.iEffectTag = (nullptr != JsonData[i]["iEffectTag"]) ? JsonData[i]["iEffectTag"] : 0;

		tEffect.vScale.x = (nullptr != JsonData[i]["vScaleX"]) ? JsonData[i]["vScaleX"] : 0.f;
		tEffect.vScale.y = (nullptr != JsonData[i]["vScaleY"]) ? JsonData[i]["vScaleY"] : 0.f;
		tEffect.vScale.z = (nullptr != JsonData[i]["vScaleZ"]) ? JsonData[i]["vScaleZ"] : 0.f;

		tEffect.vRotation.x = (nullptr != JsonData[i]["vRotationX"]) ? JsonData[i]["vRotationX"] : 0.f;
		tEffect.vRotation.y = (nullptr != JsonData[i]["vRotationY"]) ? JsonData[i]["vRotationY"] : 0.f;
		tEffect.vRotation.z = (nullptr != JsonData[i]["vRotationZ"]) ? JsonData[i]["vRotationZ"] : 0.f;

		tEffect.vOffsetPos.x = (nullptr != JsonData[i]["vOffsetPosX"]) ? JsonData[i]["vOffsetPosX"] : 0.f;
		tEffect.vOffsetPos.y = (nullptr != JsonData[i]["vOffsetPosY"]) ? JsonData[i]["vOffsetPosY"] : 0.f;
		tEffect.vOffsetPos.z = (nullptr != JsonData[i]["vOffsetPosZ"]) ? JsonData[i]["vOffsetPosZ"] : 0.f;

		tEffect.fFadeInStartTime = (nullptr != JsonData[i]["fFadeInStartTime"]) ? JsonData[i]["fFadeInStartTime"] : 0.f;
		tEffect.fFadeInRatio = (nullptr != JsonData[i]["fFadeInRatio"]) ? JsonData[i]["fFadeInRatio"] : 0.f;
		tEffect.fFadeOutStartTime = (nullptr != JsonData[i]["fFadeOutStartTime"]) ? JsonData[i]["fFadeOutStartTime"] : 0.f;
		tEffect.fFadeOutRatio = (nullptr != JsonData[i]["fFadeOutRatio"]) ? JsonData[i]["fFadeOutRatio"] : 0.f;

		tEffect.bIsBillBoard = (nullptr != JsonData[i]["bIsBillBoard"]) ? JsonData[i]["bIsBillBoard"] : false;
		tEffect.bIsRotation = (nullptr != JsonData[i]["bIsRotation"]) ? JsonData[i]["bIsRotation"] : false;

		if (tEffect.bIsRotation)
		{
			tEffect.vRotationToTime.x = (nullptr != JsonData[i]["vRotationXToTime"]) ? JsonData[i]["vRotationXToTime"] : 0.f;
			tEffect.vRotationToTime.y = (nullptr != JsonData[i]["vRotationYToTime"]) ? JsonData[i]["vRotationYToTime"] : 0.f;
			tEffect.vRotationToTime.z = (nullptr != JsonData[i]["vRotationZToTime"]) ? JsonData[i]["vRotationZToTime"] : 0.f;
		}
		tEffect.bIsParentRotation = (nullptr != JsonData[i]["bIsParentRotation"]) ? JsonData[i]["bIsParentRotation"] : false;

		tEffect.bIsSprite = (nullptr != JsonData[i]["bIsSprite"]) ? JsonData[i]["bIsSprite"] : false;

		if (tEffect.bIsSprite)
		{
			tEffect.vMaxUV.x = (nullptr != JsonData[i]["vMaxUVX"]) ? JsonData[i]["vMaxUVX"] : 0.f;
			tEffect.vMaxUV.y = (nullptr != JsonData[i]["vMaxUVY"]) ? JsonData[i]["vMaxUVY"] : 0.f;
			tEffect.fUVchangeTime = (nullptr != JsonData[i]["fUVchangeTime"]) ? JsonData[i]["fUVchangeTime"] : 0.f;
			tEffect.fUVchangeStartTime = (nullptr != JsonData[i]["fUVchangeStartTime"]) ? JsonData[i]["fUVchangeStartTime"] : 0.f;
		}

		tEffect.fAppearTime = JsonData[i]["fAppearTime"];
		tEffect.fDelTime = JsonData[i]["fDelTime"];
		tEffect.fStartChangeTime = JsonData[i]["fStartChangeTime"];
		tEffect.fEndChangeTime = JsonData[i]["fEndChangeTime"];
		tEffect.fChangeRatio = JsonData[i]["fChangeRatio"];

		tEffect.vColor.x = JsonData[i]["vColorR"];
		tEffect.vColor.y = JsonData[i]["vColorG"];
		tEffect.vColor.z = JsonData[i]["vColorB"];
		tEffect.vColor.w = JsonData[i]["vColorA"];

		tEffect.vUVScale.x = JsonData[i]["vUVScaleX"];
		tEffect.vUVScale.y = JsonData[i]["vUVScaleY"];

		tEffect.vUVPos.x = JsonData[i]["vUVPosX"];
		tEffect.vUVPos.y = JsonData[i]["vUVPosY"];

		tEffect.eType = JsonData[i]["eType"];
		tEffect.iTextureIndex = JsonData[i]["iTextureIndex"];
		tEffect.iPassIndex = JsonData[i]["iPassIndex"];
		tEffect.bIsBlur = JsonData[i]["bIsBlur"];
		tEffect.bIsBloom = JsonData[i]["bIsBloom"];
		tEffect.bIsGlow = JsonData[i]["bIsGlow"];

		if (tEffect.eType == CEffect::EFFECT_PARTICLE_MESH || tEffect.eType == CEffect::EFFECT_MESH)
		{
			string szMeshPrototypeTag = (nullptr != JsonData[i]["pMeshPrototypeTag"]) ? JsonData[i]["pMeshPrototypeTag"] : "";

			wcscpy_s(tEffect.pMeshPrototypeTag, wstring(szMeshPrototypeTag.begin(), szMeshPrototypeTag.end()).c_str());
		}

		if (tEffect.eType <= 2)
		{
			tEffect.tParticleDesc.vMinPos.x = JsonData[i]["vMinPosX"];
			tEffect.tParticleDesc.vMinPos.y = JsonData[i]["vMinPosY"];
			tEffect.tParticleDesc.vMinPos.z = JsonData[i]["vMinPosZ"];

			tEffect.tParticleDesc.vMaxPos.x = JsonData[i]["vMaxPosX"];
			tEffect.tParticleDesc.vMaxPos.y = JsonData[i]["vMaxPosY"];
			tEffect.tParticleDesc.vMaxPos.z = JsonData[i]["vMaxPosZ"];

			tEffect.tParticleDesc.fMaxSpeed = JsonData[i]["fMaxSpeed"];
			tEffect.tParticleDesc.fMinSpeed = JsonData[i]["fMinSpeed"];

			tEffect.tParticleDesc.vMinDir.x = JsonData[i]["vMinDirX"];
			tEffect.tParticleDesc.vMinDir.y = JsonData[i]["vMinDirY"];
			tEffect.tParticleDesc.vMinDir.z = JsonData[i]["vMinDirZ"];

			tEffect.tParticleDesc.vMaxDir.x = JsonData[i]["vMaxDirX"];
			tEffect.tParticleDesc.vMaxDir.y = JsonData[i]["vMaxDirY"];
			tEffect.tParticleDesc.vMaxDir.z = JsonData[i]["vMaxDirZ"];

			tEffect.tParticleDesc.vAcceleration.x = JsonData[i]["vAccelerationX"];
			tEffect.tParticleDesc.vAcceleration.y = JsonData[i]["vAccelerationY"];
			tEffect.tParticleDesc.vAcceleration.z = JsonData[i]["vAccelerationZ"];

			tEffect.tParticleDesc.vMinScale.x = JsonData[i]["vMinScaleX"];
			tEffect.tParticleDesc.vMinScale.y = JsonData[i]["vMinScaleY"];
			tEffect.tParticleDesc.vMinScale.z = JsonData[i]["vMinScaleZ"];

			tEffect.tParticleDesc.vMaxScale.x = JsonData[i]["vMaxScaleX"];
			tEffect.tParticleDesc.vMaxScale.y = JsonData[i]["vMaxScaleY"];
			tEffect.tParticleDesc.vMaxScale.z = JsonData[i]["vMaxScaleZ"];

			tEffect.tParticleDesc.vScaleVariation.x = JsonData[i]["vScaleVariationX"];
			tEffect.tParticleDesc.vScaleVariation.y = JsonData[i]["vScaleVariationY"];
			tEffect.tParticleDesc.vScaleVariation.z = JsonData[i]["vScaleVariationZ"];

			tEffect.tParticleDesc.vMinColor.x = JsonData[i]["vMinColorR"];
			tEffect.tParticleDesc.vMinColor.y = JsonData[i]["vMinColorG"];
			tEffect.tParticleDesc.vMinColor.z = JsonData[i]["vMinColorB"];
			tEffect.tParticleDesc.vMinColor.w = JsonData[i]["vMinColorA"];

			tEffect.tParticleDesc.vMaxColor.x = JsonData[i]["vMaxColorR"];
			tEffect.tParticleDesc.vMaxColor.y = JsonData[i]["vMaxColorG"];
			tEffect.tParticleDesc.vMaxColor.z = JsonData[i]["vMaxColorB"];
			tEffect.tParticleDesc.vMaxColor.w = JsonData[i]["vMaxColorA"];

			tEffect.tParticleDesc.vColorVariation.x = JsonData[i]["vColorVariationR"];
			tEffect.tParticleDesc.vColorVariation.y = JsonData[i]["vColorVariationG"];
			tEffect.tParticleDesc.vColorVariation.z = JsonData[i]["vColorVariationB"];
			tEffect.tParticleDesc.vColorVariation.w = JsonData[i]["vColorVariationA"];

			tEffect.tParticleDesc.fMinLifeTime = JsonData[i]["fMinLifeTime"];
			tEffect.tParticleDesc.fMaxLifeTime = JsonData[i]["fMaxLifeTime"];
			tEffect.tParticleDesc.fVemitRate = JsonData[i]["fVemitRate"];
			tEffect.tParticleDesc.fGenerationTime = JsonData[i]["fGenerationTime"];
			tEffect.tParticleDesc.iTotalCnt = JsonData[i]["iTotalCnt"];
		}

		EFFECTS Effteclist;
		Add_Effect(eLevel, tEffect, &Effteclist);

		// _tchar wszEffectTag[MAX_PATH] = L"";
		// wstring wstrEffectTag;

		// CGameUtils::ctwc(szEffectTag.c_str(), wszEffectTag);
		// wstrEffectTag = wstring(wszEffectTag);
		m_mapEffects.emplace(wszEffectTag, Effteclist);
	}
}

void CEffect_Manager::Free()
{
	for(auto pair :m_mapEffects)
	{
		pair.second.clear();
	}
	m_mapEffects.clear();
}
