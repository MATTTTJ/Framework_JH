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

	_int							m_iCurrentLevel = 0;
	_int							m_iNextLevel = 0;
	char*							m_pLevelName[LEVEL_END] = { "Loading", "Logo", "Test Stage" };
public:
	static CImgui_LevelSwitcher*	Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, void* pArg = nullptr);
	virtual void					Free() override;
};

