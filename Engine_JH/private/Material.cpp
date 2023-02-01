#include "stdafx.h"
#include "..\public\Material.h"
#include "GameInstance.h"
#include "Shader.h"
#include "GameUtils.h"
#include "Graphic_Device.h"



CMaterial::CMaterial(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CComponent(pDevice, pContext)
{
}

CMaterial::CMaterial(const CMaterial& rhs)
	: CComponent(rhs)
	, m_pShader(rhs.m_pShader)
	, m_tParam(rhs.m_tParam)
	, m_iPassIdx(rhs.m_iPassIdx)
{
	Safe_AddRef(m_pShader);
	for (auto e : m_tParam.ParamTexture)
		Safe_AddRef(e.second.first);
}


HRESULT CMaterial::Initialize_Prototype(const char* pMtrlFilePath)
{
	// NULL_CHECK_RETURN(pMtrlFilePath, E_FAIL);
	//
	// json JsonTemp;
	//
	// ifstream fin;
	//
	// fin.open(pMtrlFilePath);
	//
	// if (fin.is_open())
	// {
	// 	fin >> (JsonTemp);
	// }
	// else
	// {
	// 	return E_FAIL;
	// }
	//
	// fin.close();
	//
	// const Json& json = JsonTemp;
	//
	// m_tParam.ParamInt = json["Int"];
	// m_tParam.ParamFloat = json["Float"];
	// m_tParam.ParamFloat2 = json["Float2"];
	// m_tParam.ParamFloat4 = json["Float4"];
	// m_tParam.ParamFloat4x4 = json["Float4x4"];
	// m_iPassIdx = json["Pass"];
	//
	// map<_uint, string> mapTexProtoTag = json["Texture"];
	// for (auto e : mapTexProtoTag)
	// {
	// 	m_tParam.ParamTexture[e.first] = { dynamic_cast<CTexture*>(CGameInstance::GetInstance()->Clone_Component(2, CGameUtils::s2ws(e.second).c_str(),nullptr)) ,0};
	// }
	//
	// string shaderProtoTag = json["ShaderProtoTag"];
	// m_pShader = dynamic_cast<CShader*>(CGameInstance::GetInstance()->Clone_Component(2, CGameUtils::s2ws(shaderProtoTag).c_str(), nullptr));

	return S_OK;
}

void CMaterial::SaveToJson(Json& json)
{
	// json["Int"] = m_tParam.ParamInt;
	// json["Float"] = m_tParam.ParamFloat;
	// json["Float2"] = m_tParam.ParamFloat2;
	// json["Float4"] = m_tParam.ParamFloat4;
	// json["Float4x4"] = m_tParam.ParamFloat4x4;
	//
	// map<_uint, string> mapTexProtoTag;
	// for (auto& e : m_tParam.ParamTexture)
	// {
	// 	mapTexProtoTag[e.first] = CGameUtils::ws2s(e.second.first->GetPrototypeTag());
	// }
	//
	// json["Texture"] = mapTexProtoTag;
	//
	// json["Pass"] = m_iPassIdx;
	//
	// char szProtoTag[MAX_PATH];
	// CGameUtils::wc2c(m_pShader->GetPrototypeTag(), szProtoTag);
	// json["ShaderProtoTag"] = szProtoTag;
}

void CMaterial::Imgui_RenderProperty()
{
	// static char protoTag[MAX_PATH];
	// CGameUtils::wc2c(m_pPrototypeTag, protoTag);
	// ImGui::Text("Material ProtoTag : %s", protoTag);
	//
	// ImGui::Separator();
	//
	// m_pShader->Imgui_RenderProperty();
	// ImGui::Separator();
	//
	// ImGui::Separator();
	// _int iPass = (_int)m_iPassIdx;
	// ImGui::InputInt("Pass", &iPass);
	// if (0 <= iPass && iPass < m_pShader->GetNumPasses())
	// 	m_iPassIdx = (_uint)iPass;
	// else
	// 	m_iPassIdx = 0;
	//
	//
	// ImGui::Separator();
	// ImGui::Text("Param Int");
	//
	// for (auto& e : m_tParam.ParamInt)
	// {
	// 	string name = to_string(e.first) + "Int";
	// 	ImGui::InputInt(name.c_str(), &e.second);
	// }
	// if (ImGui::Button("Add Int") && m_tParam.ParamInt.size() < PARAM_INT_CNT)
	// {
	// 	m_tParam.ParamInt.emplace((_uint)m_tParam.ParamInt.size(), 0);
	// }
	// ImGui::SameLine();
	// if (ImGui::Button("Delete Int") && m_tParam.ParamInt.empty() == false)
	// {
	// 	m_tParam.ParamInt.erase(static_cast<_uint>(m_tParam.ParamInt.size()) - 1u);
	// }
	//
	//
	// ImGui::Separator();
	// ImGui::Text("Param float");
	// for (auto& e : m_tParam.ParamFloat)
	// {
	// 	string name = to_string(e.first) + "Float";
	// 	ImGui::InputFloat(name.c_str(), &e.second);
	// }
	// if (ImGui::Button("Add Float") && m_tParam.ParamFloat.size() < PARAM_FLOAT_CNT)
	// {
	// 	m_tParam.ParamFloat.emplace(static_cast<_uint>(m_tParam.ParamFloat.size()), 0.f);
	// }
	// ImGui::SameLine();
	// if (ImGui::Button("Delete Float") && m_tParam.ParamFloat.empty() == false)
	// {
	// 	m_tParam.ParamFloat.erase(static_cast<_uint>(m_tParam.ParamFloat.size()) - 1u);
	// }
	//
	// ImGui::Separator();
	// ImGui::Text("Param float2");
	// for (auto& e : m_tParam.ParamFloat2)
	// {
	// 	string name = to_string(e.first) + "Float2";
	// 	ImGui::InputFloat2(name.c_str(), (_float*)&e.second);
	// }
	// if (ImGui::Button("Add Float2") && m_tParam.ParamFloat2.size() < PARAM_FLOAT2_CNT)
	// {
	// 	m_tParam.ParamFloat2.emplace(static_cast<_uint>(m_tParam.ParamFloat2.size()), _float2{});
	// }
	// ImGui::SameLine();
	// if (ImGui::Button("Delete Float2") && m_tParam.ParamFloat2.empty() == false)
	// {
	// 	m_tParam.ParamFloat2.erase(static_cast<_uint>(m_tParam.ParamFloat2.size()) - 1u);
	// }
	//
	// ImGui::Separator();
	// ImGui::Text("Param float4");
	// for (auto& e : m_tParam.ParamFloat4)
	// {
	// 	string name = to_string(e.first) + "Float4";
	// 	ImGui::InputFloat4(name.c_str(), (_float*)&e.second);
	// }
	// if (ImGui::Button("Add Float4") && m_tParam.ParamFloat4.size() < PARAM_FLOAT4_CNT)
	// {
	// 	m_tParam.ParamFloat4.emplace(static_cast<_uint>(m_tParam.ParamFloat4.size()), _float4{});
	// }
	// ImGui::SameLine();
	// if (ImGui::Button("Delete Float4") && m_tParam.ParamFloat4.empty() == false)
	// {
	// 	m_tParam.ParamFloat4.erase(static_cast<_uint>(m_tParam.ParamFloat4.size()) - 1u);
	// }
	//
	// ImGui::Separator();
	// ImGui::Text("Param Texture");
	// for (auto& e : m_tParam.ParamTexture)
	// {
	// 	string name;
	// 	if (e.second.first == nullptr)
	// 		name = "null";
	// 	else
	// 		name = e.second.first->GetTexName(e.second.second);
	//
	// 	ImGui::Text("%d %s", e.first, name.c_str());
	// 	ImGui::SameLine();
	//
	// 	const string key = "Switch Texture" + to_string(e.first);
	// 	CImguiUtils::FileDialog_FileSelector(key.c_str(), ".png,.dds", "../Bin/Resources/",
	// 		[&](const string& fileName)
	// 	{
	// 		Safe_Release(e.second.first);
	// 		size_t iPos = fileName.find("\\Bin\\Resources");
	// 		string relative = ".." + fileName.substr(iPos, fileName.size() - iPos);
	//
	// 		e.second.first = dynamic_cast<CTexture*>(CGameInstance::GetInstance()->Clone_Component(CGameUtils::s2ws(relative).c_str()));
	// 		Assert(e.second.first != nullptr);
	// 	});
	// }
	// if (ImGui::Button("Add Texture") && m_tParam.ParamTexture.size() < PARAM_TEXTURE_CNT)
	// {
	// 	m_tParam.ParamTexture.emplace(static_cast<_uint>(m_tParam.ParamTexture.size()), pair<CTexture*, _uint>{nullptr, 0u});
	// }
	// ImGui::SameLine();
	// if (ImGui::Button("Delete Texture") && m_tParam.ParamTexture.empty() == false)
	// {
	// 	Safe_Release(m_tParam.ParamTexture.find(static_cast<_uint>(m_tParam.ParamTexture.size()) - 1u)->second.first);
	// 	m_tParam.ParamTexture.erase(static_cast<_uint>(m_tParam.ParamTexture.size()) - 1u);
	// }
	//
	// ImGui::Separator();
	//
	//
	// {
	// 	char CurShaderProtoTag[MAX_PATH];
	// 	CGameUtils::wc2c(m_pShader->GetPrototypeTag(), CurShaderProtoTag);
	// 	ImGui::Text("Current Shader : %s", CurShaderProtoTag);
	//
	// 	static char ShaderProtoTag[MAX_PATH];
	// 	ImGui::InputText("New Shader ProtoTag", ShaderProtoTag, MAX_PATH);
	// 	ImGui::SameLine();
	// 	if (ImGui::Button("Change Shader"))
	// 	{
	// 		auto pNewShaderCom = CGameInstance::GetInstance()->Clone_Component(CGameUtils::s2ws(ShaderProtoTag).c_str());
	//
	// 		if (auto pNewShader = dynamic_cast<CShader*>(pNewShaderCom))
	// 		{
	// 			Safe_Release(m_pShader);
	// 			m_pShader = pNewShader;
	// 		}
	// 		else
	// 		{
	// 			Safe_Release(pNewShaderCom);
	// 		}
	// 	}
	// }
	//
	// ImGui::Separator();
	//
	// CImguiUtils::FileDialog_FileSelector("Save Material to", ".json", "../Bin/Resources/Meshes/Valorant/Materials/", [this](const string& filePath)
	// {
	// 	Json json;
	// 	SaveToJson(json);
	// 	std::ofstream file(filePath);
	// 	file << json;
	// });
}

void CMaterial::BindMatrices(CTransform* pTransform)
{
}

void CMaterial::BindMatrices(const _float4x4& WorldMatrix)
{
}

void CMaterial::Begin()
{
}

void CMaterial::SetShaderResources()
{
}

void CMaterial::Free()
{
	CComponent::Free();
}

CMaterial* CMaterial::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const char* pMtrlFilePath)
{
	return nullptr;
}

CComponent* CMaterial::Clone(CGameObject* pOwner, void* pArg)
{
	return nullptr;
}

// void Engine::LoadValorantMaterials(const char* pMaterialJsonPath)
// {
// }
//
// void Engine::LoadEmptyMaterials()
// {
// }
