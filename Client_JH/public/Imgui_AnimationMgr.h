#pragma once
#include "Client_Defines.h"
#include "ImguiObject.h"

BEGIN(Engine)
class CGameObject;
END


BEGIN(Client)

class CImgui_AnimationMgr final : public CImguiObject
{
private:
	CImgui_AnimationMgr();
	virtual ~CImgui_AnimationMgr() = default;

public:
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Imgui_RenderWindow() override;

private:
	vector<CGameObject*>*		m_vecAnimObjects = nullptr;

public:
	static CImgui_AnimationMgr*		Create(void* pArg = nullptr);
	virtual void					Free() override;
};

END