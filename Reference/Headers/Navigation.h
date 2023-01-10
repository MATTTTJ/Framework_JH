#pragma once
#include "Component.h"
#include "Shader.h"

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
	const _float&				Get_CellHeight() const { return m_fCellHeight.y; }
	_uint						Get_CellCount() { return (_uint)m_vecCell.size(); }
	class CCell*				Get_Cell(_int iIndex) { return m_vecCell[iIndex]; }
	vector<class CCell*>*		Get_vecCell() { return &m_vecCell; }
	_bool						Is_OverHeight() { return m_bIsOverHeight; }

public:
	virtual HRESULT			Initialize_Prototype(const wstring& wstrFilePath);
	virtual HRESULT			Initialize_Clone(CGameObject* pOwner, void* pArg) override;
	// virtual void			Imgui_RenderProperty() override;

public:
	HRESULT					Add_Cell(_float3* vPoints);
	HRESULT					Delete_Cell(_int iIndex);
	HRESULT					Save_Cell(string strSaveFilePath);
	HRESULT					Find_NearBy_Point(_float3& vPoint);
	_bool					IsMove_OnNavigation(_fvector vTargetPos, _float4& vBlockedLine, _float4& vBlockedLineNormal);

#ifdef _DEBUG
public:
	HRESULT					Render_Selected_Cell(_int iIndex);
	HRESULT					Render();
#endif

private:
	vector<class CCell*>	m_vecCell;
	NAVIDESC				m_tNaviDesc;

private:
	HRESULT					Ready_Neighbor();

private:
	_float4					m_fCellHeight = { 0.f, 0.f,0.f,1.f };
	_bool					m_bIsOverHeight = false;

#ifdef _DEBUG
private:
	class CShader*			m_pShaderCom = nullptr;
#endif // _DEBUG

public:
	static CNavigation*		Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& wstrFilePath = L"");
	virtual CComponent*		Clone(CGameObject* pOwner, void* pArg = nullptr) override;
	virtual void			Free() override;
};

END