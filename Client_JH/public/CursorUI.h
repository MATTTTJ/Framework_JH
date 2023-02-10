#pragma once
#include "Client_Defines.h"
#include "UI.h"


BEGIN(Engine)

class CCursorUI final :	public CUI
{
private:
	CCursorUI(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CCursorUI(const CCursorUI& rhs);
	virtual ~CCursorUI() = default;

protected:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize_Clone(const wstring& wstrPrototypeTag, void* pArg) override;
	virtual void	Tick(_double dTimeDelta) override;
	virtual void	Late_Tick(_double dTimeDelta) override;
	virtual HRESULT Render() override;

private:
	HRESULT			SetUp_Component();
	HRESULT			SetUp_ShaderResources();

private:
	_bool			m_bShowCursor = false;
	_bool			m_bLastShowCursor = false;

	_uint			m_iTextureIndex = 0;
public:
	static CCursorUI*	Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(const wstring& wstrPrototypeTag, void* pArg = nullptr) override;
	virtual void Free() override;
};

END