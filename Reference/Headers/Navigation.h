#pragma once
#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CNavigation :	public CComponent
{
private:
	CNavigation(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CNavigation(const CNavigation& rhs);
	virtual ~CNavigation() = default;

public:
	virtual HRESULT			Initialize_Prototype(const wstring pNavigationDataFilePath);
	virtual HRESULT			Initialize_Clone(CGameObject* pOwner, void* pArg) override;
	// virtual void			Imgui_RenderProperty() override;


private:
	_int					m_iCurrentIndex = 0;
	vector<class CCell*>	m_Cells;

public:
	static CNavigation*		Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring wstrNavigationFilePath);
	virtual CComponent*		Clone(CGameObject* pOwner, void* pArg) override;
	virtual void			Free() override;
};

END