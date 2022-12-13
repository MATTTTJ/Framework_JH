#pragma once
#include "Client_Defines.h"
#include "ImguiObject.h"

BEGIN(Engine)
class	CComponent;
class   CModel;
END

BEGIN(Client)
class CImgui_ModelSave final : public CImguiObject
{
private:
	CImgui_ModelSave(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CImgui_ModelSave() = default;


public:
	virtual HRESULT			Initialize(void* pArg) override;
	virtual void			Imgui_RenderWindow() override;

private:
	ID3D11Device*			m_pDevice = nullptr;
	ID3D11DeviceContext*	m_pContext = nullptr;

	CModel*					m_pModel = nullptr;
	CModel*					m_pCloneModel = nullptr;

public:
	static	CImgui_ModelSave*	Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, void* pArg = nullptr);
	virtual void				Free() override;
};

END