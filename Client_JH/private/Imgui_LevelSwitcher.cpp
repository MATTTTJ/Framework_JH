#include "stdafx.h"
#include "Imgui_LevelSwitcher.h"
#include "Effect_Manager.h"
#include "GameInstance.h"
#include "Json_Utils.h"
#include "Level_Loading.h"

CImgui_LevelSwitcher::CImgui_LevelSwitcher(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: m_pDevice(pDevice)
	, m_pContext(pContext)
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
}

HRESULT CImgui_LevelSwitcher::Initialize(void* pArg)
{
	m_szWindowName = "My Tool";
	m_pAlpha = &ImGui::GetStyle().Colors[ImGuiCol_WindowBg].w;
	*m_pAlpha = 0.2f;
	return S_OK;
}

void CImgui_LevelSwitcher::Imgui_RenderWindow()
{
	m_iCurrentLevel = CGameInstance::GetInstance()->Get_CurLevelIndex();

	ImGui::BeginTabBar("My Tool", ImGuiTabBarFlags_None);
	Render_LevelSwitch();
	Render_EffectTool();
	Render_ImguiSetting();
	ImGui::EndTabBar();

}

void CImgui_LevelSwitcher::Render_LevelSwitch()
{
	if (ImGui::BeginTabItem("Level_Switch"))
	{
		m_eTabId = TAB_LEVELSWITCH;

		ImGui::Text("Current Level : %s", m_pLevelName[m_iCurrentLevel]);

		ImGui::NewLine();
		ImGui::BulletText("Change Level");
		ImGui::Combo(" ", &m_iNextLevel, m_pLevelName, LEVEL_END);

		ImGui::SameLine();
		if (ImGui::Button("Change"))
		{
			if (m_iNextLevel == LEVEL_LOADING || m_iCurrentLevel == m_iNextLevel)
				return;

			FAILED_CHECK_RETURN(CGameInstance::GetInstance()->Open_Level(LEVEL_LOADING, CLevel_Loading::Create(m_pDevice, m_pContext, (LEVEL)m_iNextLevel)), );
		}
		ImGui::EndTabItem();
	}
}

void CImgui_LevelSwitcher::Render_ImguiSetting()
{
	if (ImGui::BeginTabItem("Default_Setting"))
	{
		m_eTabId = TAB_SETTING;

		IMGUI_LEFT_LABEL(ImGui::SliderFloat, "Alpha", m_pAlpha, 0.f, 1.f);
		ImGui::Text("(CTRL+Click to Input Directly)");
		// ImGui::ShowDemoWindow();

		ImGui::EndTabItem();
	}
}

void CImgui_LevelSwitcher::Render_EffectTool()
{
	static _uint item_current_idx = 0;
	m_iEffectTagCnt = (_uint)m_EffectTagList.size();
	if (ImGui::BeginTabItem("Effect_Tool"))
	{
		m_eTabId = TAB_EFFECT;

		if (ImGui::Button("Open"))
			LoadData();
		ImGui::SameLine();
		if (ImGui::Button("Save"))
			SaveData();

		ImGui::Separator();
		static char szEffectTag[MAX_PATH] = "";
		IMGUI_LEFT_LABEL(ImGui::InputTextWithHint, "Add Effect", "Input EffectTag here and Press Add Button", szEffectTag, MAX_PATH);
		ImGui::SameLine();
		if (ImGui::Button("Add") && strcmp(szEffectTag, ""))
		{
			CEffect::EFFECTINFO tInfo;
			ZeroMemory(&tInfo, sizeof(CEffect::EFFECTINFO));

			string szStr = szEffectTag;

			//wcscpy_s(tInfo.pEffectTag, wstring(szStr.begin(), szStr.end()).c_str());

			//lstrcpyW(&tInfo.pEffectTag, &szWstr.c_str());

			m_EffectTagList.push_back(szStr);
			m_vecEffectInfo.push_back(tInfo);
			ZeroMemory(szEffectTag, MAX_PATH);
		}

		


		ImGui::BulletText("Effect List");

		// if (m_iEffectTagCnt == 0)
		// 	ImGui::Text("There is No Effect");
		// else
		// {
			if (ImGui::BeginListBox("##listbox 1", ImVec2(-FLT_MIN, 5 * ImGui::GetTextLineHeightWithSpacing())))
			{
				for (_uint i = 0; i < m_EffectTagList.size(); i++)
				{
					const _bool is_Selected = (item_current_idx == i);
					if (ImGui::Selectable(m_EffectTagList[i].c_str(), is_Selected))
						item_current_idx = i;
		
					if (is_Selected)
						ImGui::SetItemDefaultFocus();
				}
			}
			ImGui::EndListBox();
		// }
		if (m_iEffectTagCnt == 0)
			ImGui::Text("There is No Effect");
		else
		{
			CEffect::EFFECTINFO* tInfo = &m_vecEffectInfo[item_current_idx];

			if (m_EffectTagList.size() > 0)
			{
				ImGui::BulletText("Current Selected Effect : %s", m_EffectTagList[item_current_idx].c_str());
				ImGui::SameLine();
				if (ImGui::Button("Delete"))
				{
					auto iter = m_vecEffectInfo.begin();
					auto TagIter = m_EffectTagList.begin();

					m_vecEffectInfo.erase(iter + item_current_idx);
					m_EffectTagList.erase(TagIter + item_current_idx);
					item_current_idx = 0;
				}


				ImGui::Separator();

				//EFFECT_PARTICLE_POINT, EFFECT_PARTICLE_RECT, EFFECT_PARTICLE_MESH, EFFECT_RECT, EFFECT_POINT, EFFECT_MESH
				ImGui::BulletText("Choose Effect Type");
				ImGui::RadioButton("Particle_Point", (_int*)&tInfo->eType, 0); ImGui::SameLine();
				ImGui::RadioButton("Particle_Rect", (_int*)&tInfo->eType, 1); ImGui::SameLine();
				ImGui::RadioButton("Particle_Mesh", (_int*)&tInfo->eType, 2); ImGui::SameLine();
				ImGui::RadioButton("Point", (_int*)&tInfo->eType, 3); ImGui::SameLine();
				ImGui::RadioButton("Rect", (_int*)&tInfo->eType, 4); ImGui::SameLine();
				ImGui::RadioButton("Mesh", (_int*)&tInfo->eType, 5);

				IMGUI_LEFT_LABEL(ImGui::InputInt, "TextureIndex", (_int*)&(tInfo->iTextureIndex));
				IMGUI_LEFT_LABEL(ImGui::InputInt, "PassIndex   ", (_int*)&(tInfo->iPassIndex));

				ImGui::Checkbox("Blur", &tInfo->bIsBlur); ImGui::SameLine();
				ImGui::Checkbox("Bloom", &tInfo->bIsBloom); ImGui::SameLine();
				ImGui::Checkbox("Glow", &tInfo->bIsGlow);

				ImGui::Separator();
				ImGui::Separator();

				_float vScale[3] = { tInfo->vScale.x, tInfo->vScale.y, tInfo->vScale.z };
				IMGUI_LEFT_LABEL(ImGui::InputFloat3, "Scale    ", vScale);
				tInfo->vScale.x = vScale[0];
				tInfo->vScale.y = vScale[1];
				tInfo->vScale.z = vScale[2];

				_float vRotation[3] = { tInfo->vRotation.x, tInfo->vRotation.y, tInfo->vRotation.z };
				IMGUI_LEFT_LABEL(ImGui::InputFloat3, "Rotation ", vRotation);
				tInfo->vRotation.x = vRotation[0];
				tInfo->vRotation.y = vRotation[1];
				tInfo->vRotation.z = vRotation[2];

				_float vOffsetPos[3] = { tInfo->vOffsetPos.x, tInfo->vOffsetPos.y, tInfo->vOffsetPos.z };
				IMGUI_LEFT_LABEL(ImGui::InputFloat3, "OffsetPos", vOffsetPos);
				tInfo->vOffsetPos.x = vOffsetPos[0];
				tInfo->vOffsetPos.y = vOffsetPos[1];
				tInfo->vOffsetPos.z = vOffsetPos[2];

				ImGui::Checkbox("BillBoard", &tInfo->bIsBillBoard);
				ImGui::SameLine();
				ImGui::Checkbox("Rotation", &tInfo->bIsRotation);
				ImGui::SameLine();
				ImGui::Checkbox("ParentRotation", &tInfo->bIsParentRotation);
				ImGui::SameLine();
				ImGui::Checkbox("Sprite", &tInfo->bIsSprite);

				if (tInfo->bIsRotation)
				{
					_float vRotation[3] = { tInfo->vRotationToTime.x, tInfo->vRotationToTime.y, tInfo->vRotationToTime.z };
					IMGUI_LEFT_LABEL(ImGui::InputFloat3, "RotationToTime   ", vRotation);
					tInfo->vRotationToTime.x = vRotation[0];
					tInfo->vRotationToTime.y = vRotation[1];
					tInfo->vRotationToTime.z = vRotation[2];
				}

				if (tInfo->bIsSprite)
				{
					_float vMaxUV[2] = { tInfo->vMaxUV.x, tInfo->vMaxUV.y };
					IMGUI_LEFT_LABEL(ImGui::InputFloat2, "MaxUV            ", vMaxUV);
					tInfo->vMaxUV.x = vMaxUV[0];
					tInfo->vMaxUV.y = vMaxUV[1];

					IMGUI_LEFT_LABEL(ImGui::InputFloat, "UVchangeTime     ", &tInfo->fUVchangeTime);
					IMGUI_LEFT_LABEL(ImGui::InputFloat, "UVchangeStartTime", &tInfo->fUVchangeStartTime);
				}

				_float vColor[4] = { tInfo->vColor.x, tInfo->vColor.y, tInfo->vColor.z ,tInfo->vColor.w };
				IMGUI_LEFT_LABEL(ImGui::ColorEdit4, "Color        ", vColor, ImGuiColorEditFlags_PickerHueWheel);
				tInfo->vColor.x = vColor[0];
				tInfo->vColor.y = vColor[1];
				tInfo->vColor.z = vColor[2];
				tInfo->vColor.w = vColor[3];


				_float vUVScale[2] = { tInfo->vUVScale.x, tInfo->vUVScale.y };
				_float vUVPos[2] = { tInfo->vUVPos.x, tInfo->vUVPos.y };

				IMGUI_LEFT_LABEL(ImGui::InputFloat2, "UVScale      ", vUVScale);
				tInfo->vUVScale.x = vUVScale[0];
				tInfo->vUVScale.y = vUVScale[1];

				IMGUI_LEFT_LABEL(ImGui::InputFloat2, "UVPos        ", vUVPos);
				tInfo->vUVPos.x = vUVPos[0];
				tInfo->vUVPos.y = vUVPos[1];

				IMGUI_LEFT_LABEL(ImGui::InputFloat, "ApearTime    ", &tInfo->fAppearTime);
				IMGUI_LEFT_LABEL(ImGui::InputFloat, "DeleteTime   ", &tInfo->fDelTime);

				IMGUI_LEFT_LABEL(ImGui::InputFloat, "ChangeStartT ", &tInfo->fStartChangeTime);
				IMGUI_LEFT_LABEL(ImGui::InputFloat, "ChangeEndT   ", &tInfo->fEndChangeTime);
				IMGUI_LEFT_LABEL(ImGui::InputFloat, "ChangeRatio  ", &tInfo->fChangeRatio);
				IMGUI_LEFT_LABEL(ImGui::InputFloat, "FadeInStartT ", &tInfo->fFadeInStartTime);
				IMGUI_LEFT_LABEL(ImGui::InputFloat, "FadeInRatio  ", &tInfo->fFadeInRatio);
				IMGUI_LEFT_LABEL(ImGui::InputFloat, "FadeOutStartT", &tInfo->fFadeOutStartTime);
				IMGUI_LEFT_LABEL(ImGui::InputFloat, "FadeOutRatio ", &tInfo->fFadeOutRatio);

				ImGui::Separator();
			}

			//메시, 파티클 매시
			if (tInfo->eType == CEffect::EFFECT_MESH || tInfo->eType == CEffect::EFFECT_PARTICLE_MESH)
			{
				ImGui::Begin("MeshType Explorer");
				if (m_PrototypeTagList.size() == 0)
					ImGui::BulletText("There is no Prototype");
				else
				{
					if (ImGui::BeginListBox("##listbox 2", ImVec2(-FLT_MIN, 5 * ImGui::GetTextLineHeightWithSpacing())))
					{


						for (_uint i = 0; i < m_PrototypeTagList.size(); ++i)
						{
							wstring szMeshPrototypeTag = wstring(tInfo->pMeshPrototypeTag);

							const bool is_selected = m_PrototypeTagList[i].c_str() == string(szMeshPrototypeTag.begin(), szMeshPrototypeTag.end());

							if (ImGui::Selectable(m_PrototypeTagList[i].c_str(), is_selected))
							{
								wcscpy_s(tInfo->pMeshPrototypeTag, wstring(m_PrototypeTagList[i].begin(), m_PrototypeTagList[i].end()).c_str());
							}

							if (is_selected)
								ImGui::SetItemDefaultFocus();
						}
						ImGui::EndListBox();
					}
				}
					ImGui::End();
			}
			//
			//파티클
			if (tInfo->eType <= CEffect::EFFECT_PARTICLE_MESH)
			{
				ImGui::Begin("ParticleType Explorer");

				PARTICLEDESC* tParticle = &tInfo->tParticleDesc;

				_float vMinPos[3] = { tParticle->vMinPos.x, tParticle->vMinPos.y, tParticle->vMinPos.z };
				IMGUI_LEFT_LABEL(ImGui::InputFloat3, "MinPos        ", vMinPos);
				tParticle->vMinPos.x = vMinPos[0];
				tParticle->vMinPos.y = vMinPos[1];
				tParticle->vMinPos.z = vMinPos[2];

				_float vMaxPos[3] = { tParticle->vMinPos.x, tParticle->vMinPos.y, tParticle->vMinPos.z };
				IMGUI_LEFT_LABEL(ImGui::InputFloat3, "MaxPos        ", vMaxPos);
				tParticle->vMaxPos.x = vMaxPos[0];
				tParticle->vMaxPos.y = vMaxPos[1];
				tParticle->vMaxPos.z = vMaxPos[2];

				_float vMinDir[3] = { tParticle->vMinDir.x, tParticle->vMinDir.y, tParticle->vMinDir.z };
				IMGUI_LEFT_LABEL(ImGui::InputFloat3, "MinDir        ", vMinDir);
				tParticle->vMinDir.x = vMinDir[0];
				tParticle->vMinDir.y = vMinDir[1];
				tParticle->vMinDir.z = vMinDir[2];

				_float vMaxDir[3] = { tParticle->vMaxDir.x, tParticle->vMaxDir.y, tParticle->vMaxDir.z };
				IMGUI_LEFT_LABEL(ImGui::InputFloat3, "MaxDir        ", vMaxDir);
				tParticle->vMaxDir.x = vMaxDir[0];
				tParticle->vMaxDir.y = vMaxDir[1];
				tParticle->vMaxDir.z = vMaxDir[2];

				_float vAcceleration[3] = { tParticle->vAcceleration.x, tParticle->vAcceleration.y, tParticle->vAcceleration.z };
				IMGUI_LEFT_LABEL(ImGui::InputFloat3, "Acceleration  ", vAcceleration);
				tParticle->vAcceleration.x = vAcceleration[0];
				tParticle->vAcceleration.y = vAcceleration[1];
				tParticle->vAcceleration.z = vAcceleration[2];

				_float vMinScale[3] = { tParticle->vMinScale.x, tParticle->vMinScale.y, tParticle->vMinScale.z };
				IMGUI_LEFT_LABEL(ImGui::InputFloat3, "MinScale      ", vMinScale);
				tParticle->vMinScale.x = vMinScale[0];
				tParticle->vMinScale.y = vMinScale[1];
				tParticle->vMinScale.z = vMinScale[2];

				_float vMaxScale[3] = { tParticle->vMaxScale.x, tParticle->vMaxScale.y, tParticle->vMaxScale.z };
				IMGUI_LEFT_LABEL(ImGui::InputFloat3, "MaxScale      ", vMaxScale);
				tParticle->vMaxScale.x = vMaxScale[0];
				tParticle->vMaxScale.y = vMaxScale[1];
				tParticle->vMaxScale.z = vMaxScale[2];

				_float vScaleVariation[3] = { tParticle->vScaleVariation.x, tParticle->vScaleVariation.y, tParticle->vScaleVariation.z };
				IMGUI_LEFT_LABEL(ImGui::InputFloat3, "ScaleVariation", vScaleVariation);
				tParticle->vScaleVariation.x = vScaleVariation[0];
				tParticle->vScaleVariation.y = vScaleVariation[1];
				tParticle->vScaleVariation.z = vScaleVariation[2];

				_float vMinColor[4] = { tParticle->vMinColor.x, tParticle->vMinColor.y, tParticle->vMinColor.z ,tParticle->vMinColor.w };
				IMGUI_LEFT_LABEL(ImGui::ColorEdit4, "MinColor      ", vMinColor, ImGuiColorEditFlags_PickerHueWheel);
				tParticle->vMinColor.x = vMinColor[0];
				tParticle->vMinColor.y = vMinColor[1];
				tParticle->vMinColor.z = vMinColor[2];
				tParticle->vMinColor.w = vMinColor[3];

				_float vMaxColor[4] = { tParticle->vMaxColor.x, tParticle->vMaxColor.y, tParticle->vMaxColor.z ,tParticle->vMaxColor.w };
				IMGUI_LEFT_LABEL(ImGui::ColorEdit4, "MaxColor      ", vMaxColor, ImGuiColorEditFlags_PickerHueWheel);
				tParticle->vMaxColor.x = vMaxColor[0];
				tParticle->vMaxColor.y = vMaxColor[1];
				tParticle->vMaxColor.z = vMaxColor[2];
				tParticle->vMaxColor.w = vMaxColor[3];

				_float vColorVariation[4] = { tParticle->vColorVariation.x, tParticle->vColorVariation.y, tParticle->vColorVariation.z ,tParticle->vColorVariation.w };
				IMGUI_LEFT_LABEL(ImGui::ColorEdit4, "ColorVariation", vColorVariation, ImGuiColorEditFlags_PickerHueWheel);
				tParticle->vColorVariation.x = vColorVariation[0];
				tParticle->vColorVariation.y = vColorVariation[1];
				tParticle->vColorVariation.z = vColorVariation[2];
				tParticle->vColorVariation.w = vColorVariation[3];

				IMGUI_LEFT_LABEL(ImGui::InputFloat, "MinSpeed      ", &tParticle->fMinSpeed);
				IMGUI_LEFT_LABEL(ImGui::InputFloat, "MaxSpeed      ", &tParticle->fMaxSpeed);

				IMGUI_LEFT_LABEL(ImGui::InputFloat, "MinLifeTime   ", &tParticle->fMinLifeTime);
				IMGUI_LEFT_LABEL(ImGui::InputFloat, "MaxLifeTime   ", &tParticle->fMaxLifeTime);

				IMGUI_LEFT_LABEL(ImGui::InputFloat, "VemitRate     ", &tParticle->fVemitRate);
				IMGUI_LEFT_LABEL(ImGui::InputFloat, "GenerationTime", &tParticle->fGenerationTime);
				IMGUI_LEFT_LABEL(ImGui::InputInt, "TotalCnt      ", (_int*)&tParticle->iTotalCnt);

				ImGui::End();
			}
		}
			////////////////////////////////////////////////
			ImGui::EndTabItem();
	}
	
}

void CImgui_LevelSwitcher::LoadData()
{
	if (m_eTabId == TAB_EFFECT)
	{
		m_vecEffectInfo.clear();
		m_EffectTagList.clear();
		
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

			wcscpy_s(tEffect.pEffectTag, wstring(szEffectTag.begin(), szEffectTag.end()).c_str());

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

			if (tEffect.eType == CEffect::EFFECT_MESH || tEffect.eType == CEffect::EFFECT_PARTICLE_MESH)
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
			string pEffectTag = JsonData[i]["pEffectTag"];
			m_EffectTagList.push_back(pEffectTag);
			m_vecEffectInfo.push_back(tEffect);
		}

		MSG_BOX(" [ Loading Complete ] ");
	}
}


void CImgui_LevelSwitcher::SaveData()
{
	if(m_eTabId == TAB_EFFECT)
	{
		_tchar		szJsonFileName[MAX_PATH] = TEXT("Effect_Data");

		json	Json;

		for (_uint i = 0; i < m_vecEffectInfo.size(); ++i)
		{
			json	JsonEffect;

			wstring szTag = m_vecEffectInfo[i].pEffectTag;

			JsonEffect.emplace("pEffectTag", m_EffectTagList[i]);

			if (m_vecEffectInfo[i].eType == CEffect::EFFECT_MESH || m_vecEffectInfo[i].eType == CEffect::EFFECT_PARTICLE_MESH)
			{
				wstring szMeshPrototypeTag = m_vecEffectInfo[i].pMeshPrototypeTag;

				JsonEffect.emplace("pMeshPrototypeTag", string(szMeshPrototypeTag.begin(), szMeshPrototypeTag.end()));
			}

			JsonEffect.emplace("vScaleX", m_vecEffectInfo[i].vScale.x);
			JsonEffect.emplace("vScaleY", m_vecEffectInfo[i].vScale.y);
			JsonEffect.emplace("vScaleZ", m_vecEffectInfo[i].vScale.z);

			JsonEffect.emplace("vRotationX", m_vecEffectInfo[i].vRotation.x);
			JsonEffect.emplace("vRotationY", m_vecEffectInfo[i].vRotation.y);
			JsonEffect.emplace("vRotationZ", m_vecEffectInfo[i].vRotation.z);

			JsonEffect.emplace("vOffsetPosX", m_vecEffectInfo[i].vOffsetPos.x);
			JsonEffect.emplace("vOffsetPosY", m_vecEffectInfo[i].vOffsetPos.y);
			JsonEffect.emplace("vOffsetPosZ", m_vecEffectInfo[i].vOffsetPos.z);

			JsonEffect.emplace("fFadeInStartTime", m_vecEffectInfo[i].fFadeInStartTime);
			JsonEffect.emplace("fFadeInRatio", m_vecEffectInfo[i].fFadeInRatio);
			JsonEffect.emplace("fFadeOutStartTime", m_vecEffectInfo[i].fFadeOutStartTime);
			JsonEffect.emplace("fFadeOutRatio", m_vecEffectInfo[i].fFadeOutRatio);

			JsonEffect.emplace("bIsBillBoard", m_vecEffectInfo[i].bIsBillBoard);
			JsonEffect.emplace("bIsRotation", m_vecEffectInfo[i].bIsRotation);
			if (m_vecEffectInfo[i].bIsRotation)
			{
				JsonEffect.emplace("vRotationXToTime", m_vecEffectInfo[i].vRotationToTime.x);
				JsonEffect.emplace("vRotationYToTime", m_vecEffectInfo[i].vRotationToTime.y);
				JsonEffect.emplace("vRotationZToTime", m_vecEffectInfo[i].vRotationToTime.z);
			}
			JsonEffect.emplace("bIsParentRotation", m_vecEffectInfo[i].bIsParentRotation);

			JsonEffect.emplace("bIsSprite", m_vecEffectInfo[i].bIsSprite);
			if (m_vecEffectInfo[i].bIsSprite)
			{
				JsonEffect.emplace("vMaxUVX", m_vecEffectInfo[i].vMaxUV.x);
				JsonEffect.emplace("vMaxUVY", m_vecEffectInfo[i].vMaxUV.y);
				JsonEffect.emplace("fUVchangeTime", m_vecEffectInfo[i].fUVchangeTime);
				JsonEffect.emplace("fUVchangeStartTime", m_vecEffectInfo[i].fUVchangeStartTime);
			}

			JsonEffect.emplace("fAppearTime", m_vecEffectInfo[i].fAppearTime);
			JsonEffect.emplace("fDelTime", m_vecEffectInfo[i].fDelTime);
			JsonEffect.emplace("fStartChangeTime", m_vecEffectInfo[i].fStartChangeTime);
			JsonEffect.emplace("fEndChangeTime", m_vecEffectInfo[i].fEndChangeTime);
			JsonEffect.emplace("fChangeRatio", m_vecEffectInfo[i].fChangeRatio);
			JsonEffect.emplace("vColorR", m_vecEffectInfo[i].vColor.x);
			JsonEffect.emplace("vColorG", m_vecEffectInfo[i].vColor.y);
			JsonEffect.emplace("vColorB", m_vecEffectInfo[i].vColor.z);
			JsonEffect.emplace("vColorA", m_vecEffectInfo[i].vColor.w);
			JsonEffect.emplace("vUVScaleX", m_vecEffectInfo[i].vUVScale.x);
			JsonEffect.emplace("vUVScaleY", m_vecEffectInfo[i].vUVScale.y);
			JsonEffect.emplace("vUVPosX", m_vecEffectInfo[i].vUVPos.x);
			JsonEffect.emplace("vUVPosY", m_vecEffectInfo[i].vUVPos.y);
			JsonEffect.emplace("eType", m_vecEffectInfo[i].eType);
			JsonEffect.emplace("iTextureIndex", m_vecEffectInfo[i].iTextureIndex);
			JsonEffect.emplace("iPassIndex", m_vecEffectInfo[i].iPassIndex);
			JsonEffect.emplace("bIsBlur", m_vecEffectInfo[i].bIsBlur);
			JsonEffect.emplace("bIsBloom", m_vecEffectInfo[i].bIsBloom);
			JsonEffect.emplace("bIsGlow", m_vecEffectInfo[i].bIsGlow);

			if (m_vecEffectInfo[i].eType <= CEffect::EFFECT_PARTICLE_MESH)
			{
				JsonEffect.emplace("vMinPosX", m_vecEffectInfo[i].tParticleDesc.vMinPos.x);
				JsonEffect.emplace("vMinPosY", m_vecEffectInfo[i].tParticleDesc.vMinPos.y);
				JsonEffect.emplace("vMinPosZ", m_vecEffectInfo[i].tParticleDesc.vMinPos.z);

				JsonEffect.emplace("vMaxPosX", m_vecEffectInfo[i].tParticleDesc.vMaxPos.x);
				JsonEffect.emplace("vMaxPosY", m_vecEffectInfo[i].tParticleDesc.vMaxPos.y);
				JsonEffect.emplace("vMaxPosZ", m_vecEffectInfo[i].tParticleDesc.vMaxPos.z);

				JsonEffect.emplace("fMaxSpeed", m_vecEffectInfo[i].tParticleDesc.fMaxSpeed);
				JsonEffect.emplace("fMinSpeed", m_vecEffectInfo[i].tParticleDesc.fMinSpeed);

				JsonEffect.emplace("vMinDirX", m_vecEffectInfo[i].tParticleDesc.vMinDir.x);
				JsonEffect.emplace("vMinDirY", m_vecEffectInfo[i].tParticleDesc.vMinDir.y);
				JsonEffect.emplace("vMinDirZ", m_vecEffectInfo[i].tParticleDesc.vMinDir.z);

				JsonEffect.emplace("vMaxDirX", m_vecEffectInfo[i].tParticleDesc.vMaxDir.x);
				JsonEffect.emplace("vMaxDirY", m_vecEffectInfo[i].tParticleDesc.vMaxDir.y);
				JsonEffect.emplace("vMaxDirZ", m_vecEffectInfo[i].tParticleDesc.vMaxDir.z);

				JsonEffect.emplace("vAccelerationX", m_vecEffectInfo[i].tParticleDesc.vAcceleration.x);
				JsonEffect.emplace("vAccelerationY", m_vecEffectInfo[i].tParticleDesc.vAcceleration.y);
				JsonEffect.emplace("vAccelerationZ", m_vecEffectInfo[i].tParticleDesc.vAcceleration.z);

				JsonEffect.emplace("vMinScaleX", m_vecEffectInfo[i].tParticleDesc.vMinScale.x);
				JsonEffect.emplace("vMinScaleY", m_vecEffectInfo[i].tParticleDesc.vMinScale.y);
				JsonEffect.emplace("vMinScaleZ", m_vecEffectInfo[i].tParticleDesc.vMinScale.z);

				JsonEffect.emplace("vMaxScaleX", m_vecEffectInfo[i].tParticleDesc.vMaxScale.x);
				JsonEffect.emplace("vMaxScaleY", m_vecEffectInfo[i].tParticleDesc.vMaxScale.y);
				JsonEffect.emplace("vMaxScaleZ", m_vecEffectInfo[i].tParticleDesc.vMaxScale.z);

				JsonEffect.emplace("vScaleVariationX", m_vecEffectInfo[i].tParticleDesc.vScaleVariation.x);
				JsonEffect.emplace("vScaleVariationY", m_vecEffectInfo[i].tParticleDesc.vScaleVariation.y);
				JsonEffect.emplace("vScaleVariationZ", m_vecEffectInfo[i].tParticleDesc.vScaleVariation.z);

				JsonEffect.emplace("vMinColorR", m_vecEffectInfo[i].tParticleDesc.vMinColor.x);
				JsonEffect.emplace("vMinColorG", m_vecEffectInfo[i].tParticleDesc.vMinColor.y);
				JsonEffect.emplace("vMinColorB", m_vecEffectInfo[i].tParticleDesc.vMinColor.z);
				JsonEffect.emplace("vMinColorA", m_vecEffectInfo[i].tParticleDesc.vMinColor.w);

				JsonEffect.emplace("vMaxColorR", m_vecEffectInfo[i].tParticleDesc.vMaxColor.x);
				JsonEffect.emplace("vMaxColorG", m_vecEffectInfo[i].tParticleDesc.vMaxColor.y);
				JsonEffect.emplace("vMaxColorB", m_vecEffectInfo[i].tParticleDesc.vMaxColor.z);
				JsonEffect.emplace("vMaxColorA", m_vecEffectInfo[i].tParticleDesc.vMaxColor.w);

				JsonEffect.emplace("vColorVariationR", m_vecEffectInfo[i].tParticleDesc.vColorVariation.x);
				JsonEffect.emplace("vColorVariationG", m_vecEffectInfo[i].tParticleDesc.vColorVariation.y);
				JsonEffect.emplace("vColorVariationB", m_vecEffectInfo[i].tParticleDesc.vColorVariation.z);
				JsonEffect.emplace("vColorVariationA", m_vecEffectInfo[i].tParticleDesc.vColorVariation.w);

				JsonEffect.emplace("fMinLifeTime", m_vecEffectInfo[i].tParticleDesc.fMinLifeTime);
				JsonEffect.emplace("fMaxLifeTime", m_vecEffectInfo[i].tParticleDesc.fMaxLifeTime);

				JsonEffect.emplace("fVemitRate", m_vecEffectInfo[i].tParticleDesc.fVemitRate);
				JsonEffect.emplace("fGenerationTime", m_vecEffectInfo[i].tParticleDesc.fGenerationTime);
				JsonEffect.emplace("iTotalCnt", m_vecEffectInfo[i].tParticleDesc.iTotalCnt);
			}

			Json["data"].push_back(JsonEffect);
			JsonEffect.clear();
		}

		if (FAILED(CJson_Utils::Save_Json(CJson_Utils::Complete_Path(szJsonFileName).c_str(), Json)))
		{
			MSG_BOX("EffectTool -> SAVE FAILED");
			return;
		}

		// CEffect_Manager::GetInstance()->Reset(LEVEL_GAMEPLAY);

		MSG_BOX(" [ Save Complete ] ");
		Json.clear();
	}

}

void CImgui_LevelSwitcher::InputFloat2(_float2* pVector, const char* str)
{
	_float vVec2[3] = { pVector->x, pVector->y };
	ImGui::InputFloat2(str, vVec2);
	pVector->x = vVec2[0];
	pVector->y = vVec2[1];
}

void CImgui_LevelSwitcher::InputFloat3(_float3* pVector, const char* str)
{
	_float vVec3f[3] = { pVector->x, pVector->y, pVector->z };
	ImGui::InputFloat3(str, vVec3f);
	pVector->x = vVec3f[0];
	pVector->y = vVec3f[1];
	pVector->z = vVec3f[2];
}

void CImgui_LevelSwitcher::InputColor(_float4* pVector, const char* str)
{
	_float vColor[4] = { pVector->x, pVector->y, pVector->z, pVector->w };
	ImGui::ColorEdit4(str, vColor, ImGuiColorEditFlags_PickerHueWheel);
	pVector->x = vColor[0];
	pVector->y = vColor[1];
	pVector->z = vColor[2];
	pVector->w = vColor[3];
}

CImgui_LevelSwitcher* CImgui_LevelSwitcher::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, void* pArg)
{
	CImgui_LevelSwitcher*		pInstance = new CImgui_LevelSwitcher(pDevice, pContext);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Create : CImgui_LevelSwitcher");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CImgui_LevelSwitcher::Free()
{
	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);

	// for (auto& Pair : m_mapLevels)
	// 	Safe_Release(Pair.second);
	//
	// m_mapLevels.clear();
}
