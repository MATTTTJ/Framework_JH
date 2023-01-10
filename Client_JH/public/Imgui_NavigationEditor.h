#pragma once
#include "Client_Defines.h"
#include "ImguiObject.h"

BEGIN(Engine)
class CGameInstance;
class CNavigation;
class CCell;
class CVIBuffer_Cell_Circle;
END

BEGIN(Client)

class CImgui_NavigationEditor final : public CImguiObject
{
public:
	enum POINT { POINT_A, POINT_B, POINT_C, POINT_END };

public:
	CImgui_NavigationEditor(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CImgui_NavigationEditor() = default;

	virtual HRESULT			Initialize(void* pArg) override;
	virtual void			Imgui_RenderWindow() override;
	virtual void			Render() override;
	void					Cell_Sorting(void);
private:
	ID3D11Device*			m_pDevice = nullptr;
	ID3D11DeviceContext*	m_pContext = nullptr;

	CGameInstance*			m_pGameInstance = nullptr;

private:
	_uint					m_iCurPoint = 0;
	_float3					m_vPoint[POINT_END] = { _float3(0.f,0.f,0.f), _float3(0.f,0.f,0.f), _float3(0.f,0.f,0.f) };

	CNavigation*			m_pNavigationCom = nullptr;
	_uint					m_iNumCell = 0;
	_int					m_iSelectedCell = -1;
	_int					m_iCellIndex = 0;
#ifdef _DEBUG
	CVIBuffer_Cell_Circle*		m_pVIBufferCircleCom[POINT_END] = { nullptr, nullptr, nullptr };
#endif // _DEBUG

public:
	static CImgui_NavigationEditor* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, void* pArg = nullptr);
	virtual void					Free() override;
};

END