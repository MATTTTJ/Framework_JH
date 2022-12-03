#pragma once
#include "Client_Defines.h"
#include "ImguiObject.h"

BEGIN(Client)
class CImgui_MapEditor final : public CImguiObject
{
private:
	CImgui_MapEditor(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CImgui_MapEditor() = default;

public:
	virtual HRESULT Initialize(void* pArg) override;
	virtual void	Imgui_RenderWindow() override;

private:
	ID3D11Device*			m_pDevice;
	ID3D11DeviceContext*	m_pContext;

	_uint					m_iCurrentLevel = 0;
	_uint					m_iNextLevel = 0;
	

public:
	static CImgui_MapEditor*	Create(void* pArg = nullptr);
	virtual void				Free() override;
};
END
