#pragma once
#include "Client_Defines.h"
#include "ImguiObject.h"

BEGIN(Engine)
class CGameInstance;
class CGameObject;
class CLayer;
END

BEGIN(Client)

class CImgui_MapEditor final : public CImguiObject
{
private:
	CImgui_MapEditor();
	virtual ~CImgui_MapEditor() = default;

public:
	virtual HRESULT Initialize(void* pArg) override;
	virtual void	Imgui_RenderWindow() override;

private:
	_uint								m_iProtypeCnt = 0;
	map<const wstring, CGameObject*>*	m_mapPrototypes = nullptr;

	_uint								m_iCurLevel = 0;
	map<const wstring, CLayer*>*		m_mapLayers = nullptr;

	map<const wstring, CGameObject*>	m_mapPrototypeModels;

private:
	void			CheckNewPrototype();
	void			CheckCurrentLevel();
public:
	static CImgui_MapEditor*	Create(void* pArg = nullptr);
	virtual void				Free() override;
};
END
