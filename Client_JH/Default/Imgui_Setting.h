#pragma once
#include "Client_Defines.h"
#include "ImguiObject.h"

BEGIN(Client)

class CImgui_Setting final : public CImguiObject
{
public:
	CImgui_Setting() = default;
	virtual ~CImgui_Setting() = default;

public:
	virtual HRESULT		Initialize(void* pArg) override;
	virtual void		Imgui_RenderWindow() override;

private:
	_float*		m_pAlpha = nullptr;

public:
	static CImgui_Setting*		Create(void* pArg = nullptr);
	virtual void				Free() override;
};

END