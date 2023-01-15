#include "stdafx.h"
#include "..\public\Imgui_AnimationMgr.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "GameUtils.h"

CImgui_AnimationMgr::CImgui_AnimationMgr()
{
}

HRESULT CImgui_AnimationMgr::Initialize(void* pArg)
{
	m_szWindowName = "Animation Manager";

 	m_vecAnimObjects = CGameInstance::GetInstance()->Get_AnimObject();
	NULL_CHECK_RETURN(m_vecAnimObjects, E_FAIL);

	return S_OK;
}

void CImgui_AnimationMgr::Imgui_RenderWindow()
{
	_uint		iObjectCount = (_uint)m_vecAnimObjects->size();

	ImGui::BulletText("Animation Objects");

	if (iObjectCount == 0)
	{
		ImGui::Text("There is no Object has Animation Model Component");
		return;
	}

	static _int	iSelectObject = -1;
	char**		ppObjectTag = new char*[iObjectCount];

	for (_uint i = 0; i < iObjectCount; ++i)
	{
		_uint	iTagLength = (_uint)lstrlen((*m_vecAnimObjects)[i]->Get_PrototypeGameObjectTag().c_str()) + 1;
		ppObjectTag[i] = new char[iTagLength];
		CGameUtils::wc2c((*m_vecAnimObjects)[i]->Get_PrototypeGameObjectTag().c_str(), ppObjectTag[i]);
	}

	ImGui::ListBox("##", &iSelectObject, ppObjectTag, iObjectCount);

	if (iSelectObject != -1)
	{
		CGameObject*	pGameObject = (*m_vecAnimObjects)[iSelectObject];

		CModel*	pDefaultPistol = dynamic_cast<CModel*>(pGameObject->Get_Component(L"Com_Model"));

		if (pDefaultPistol != nullptr)
		{
			ImGui::Separator();
			pDefaultPistol->Imgui_RenderAnimation();
			ImGui::Separator();
			pDefaultPistol->Imgui_RenderMeshes();
		}
	}

	for (_uint i = 0; i < iObjectCount; ++i)
		Safe_Delete_Array(ppObjectTag[i]);
	Safe_Delete_Array(ppObjectTag);
}

CImgui_AnimationMgr* CImgui_AnimationMgr::Create(void* pArg)
{
	CImgui_AnimationMgr*	pInstance = new CImgui_AnimationMgr;

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Create : CImgui_AnimationMgr");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CImgui_AnimationMgr::Free()
{
}

