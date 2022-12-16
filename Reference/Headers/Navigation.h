#pragma once
#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CNavigation :	public CComponent
{
public:
	typedef struct tagNavigationDesc
	{
		_int		iCurrentIndex = -1;
	}NAVIDESC;

private:
	CNavigation(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CNavigation(const CNavigation& rhs);
	virtual ~CNavigation() = default;

public:
	virtual HRESULT			Initialize_Prototype(const wstring pNavigationDataFilePath);
	virtual HRESULT			Initialize_Clone(CGameObject* pOwner, void* pArg) override;
	// virtual void			Imgui_RenderProperty() override;

public:
	_bool					IsMove_OnNavigation(_fvector vTargetPos);

#ifdef _DEBUG
public:
	HRESULT					Render();
#endif

private:
	vector<class CCell*>	m_vecCells;
	NAVIDESC				m_tNaviDesc;

private:
	HRESULT					Ready_Neighbor();

#ifdef _DEBUG
private:
	class CShader*			m_pShader = nullptr;
#endif // _DEBUG

public:
	static CNavigation*		Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring wstrNavigationFilePath);
	virtual CComponent*		Clone(CGameObject* pOwner, void* pArg) override;
	virtual void			Free() override;
};

END