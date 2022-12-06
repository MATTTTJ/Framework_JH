#pragma once
#include "Client_Defines.h"
#include "ImguiObject.h"
class CImgui_LevelSwitcher final : public CImguiObject
{
public:
	CImgui_LevelSwitcher(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CImgui_LevelSwitcher() = default;

	virtual HRESULT					Initialize(void* pArg) override;
	virtual void					Imgui_RenderWindow() override;

private:
	ID3D11Device*					m_pDevice;
	ID3D11DeviceContext*			m_pContext;

	_uint							m_iCurrentLevel = 0;
	_uint							m_iNextLevel = 0;
	map<string, _uint>				m_mapLevels;
	LEVEL							m_Level;

	char*							m_pLevelName[LEVEL_END] = { "Level_Loading", "Level_Logo", "Level_Gameplay" };
public:
	static CImgui_LevelSwitcher*	Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, void* pArg = nullptr);
	virtual void					Free() override;
};

