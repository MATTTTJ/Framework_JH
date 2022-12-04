#include "stdafx.h"
#include "..\public\Imgui_ProtoMgr.h"


CImgui_ProtoMgr::CImgui_ProtoMgr()
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
}

void CImgui_ProtoMgr::SortComponentByType(char***& ppComponentTag, _uint* pComponentCnt)
{
}

_uint CImgui_ProtoMgr::FindPrototypeComponentLevel(const _tchar* pComponentTag)
{
}

CImgui_ProtoMgr* CImgui_ProtoMgr::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, void* pArg)
{
}

void CImgui_ProtoMgr::Free()
{
	CImguiObject::Free();
}
