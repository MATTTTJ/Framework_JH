#include "stdafx.h"
#include "..\public\Imgui_ProtoMgr.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "GameUtils.h"

CImgui_ProtoMgr::CImgui_ProtoMgr()
{
}

HRESULT CImgui_ProtoMgr::Initialize(void* pArg)
{
	return S_OK;
}

void CImgui_ProtoMgr::Imgui_RenderWindow()
{
}

void CImgui_ProtoMgr::Component_Editor()
{
}

void CImgui_ProtoMgr::GameObject_Editor()
{
}

COMPONENTTYPE CImgui_ProtoMgr::CheckComponentType(_int iSelectLevel, const char* pComponentTag)
{
	return COMPONENTTYPE_END;
}

void CImgui_ProtoMgr::SortComponentByType(char***& ppComponentTag, _uint* pComponentCnt)
{
}

_uint CImgui_ProtoMgr::FindPrototypeComponentLevel(const _tchar* pComponentTag)
{
	return 0;
}

CImgui_ProtoMgr* CImgui_ProtoMgr::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, void* pArg)
{
	return nullptr;
}

void CImgui_ProtoMgr::Free()
{
}
