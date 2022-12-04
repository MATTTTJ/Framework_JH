#pragma once
#include "Client_Defines.h"
#include "ImguiObject.h"

BEGIN(Engine)
class CComponent;
class CGameObject;
END

BEGIN(Client)

class CImgui_ProtoMgr final : public CImguiObject
{
public:
	CImgui_ProtoMgr();
	virtual ~CImgui_ProtoMgr() = default;

public:
	virtual HRESULT Initialize(void* pArg) override;
	virtual void	Imgui_RenderWindow() override;

private:
	ID3D11Device*						m_pDevice = nullptr;
	ID3D11DeviceContext*				m_pContext = nullptr;

private:
	_uint								m_iCurLevel = 0;

	map<const wstring, CComponent*>*	m_mapProtoComponenets = nullptr;
	_uint								m_iProtoComponentCnt = 0;

	map<const wstring, CGameObject*>*	m_mapProtoObjects = nullptr;
	_uint								m_iProtoObjCnt = 0;

	char*								m_pLevelName[LEVEL_END + 1] = { "Level_Loading", "Level_Logo", "Level_Gameplay", "Level_MapEditor" };
	_int*								m_iSelectTextureCom = nullptr;

private:
	void								Component_Editor();
	void								GameObject_Editor();
	COMPONENTTYPE						CheckComponentType(_int iSelectLevel, const char* pComponentTag);
	void								SortComponentByType(char***& ppComponentTag, _uint* pComponentCnt);
	_uint								FindPrototypeComponentLevel(const _tchar* pComponentTag);

public:
	static CImgui_ProtoMgr*				Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, void* pArg = nullptr);
	virtual void						Free() override;
};

END