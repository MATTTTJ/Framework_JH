#pragma once
#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL CComponent abstract : public CBase
{
protected:
	CComponent(ID3D11Device* pDevice,ID3D11DeviceContext* pContext);
	CComponent(const CComponent& rhs);
	virtual ~CComponent() = default;

public:
	class CGameObject*			Get_Owner() const { return m_pOwner; }
	const wstring&				Get_FilePath() const { return m_wstrFilePath; }

public:
	virtual HRESULT				Initialize_Prototype();
	virtual HRESULT				Initialize_Clone(class CGameObject* pOwner, void* pArg);

	virtual void				Imgui_RenderProperty() {}

// #ifdef _DEBUG
public:
	virtual HRESULT				Render() { return S_OK; };
// #endif // 

protected:
	ID3D11Device*				m_pDevice = nullptr;
	ID3D11DeviceContext*		m_pContext = nullptr;

	class CGameObject*			m_pOwner;
	wstring						m_wstrFilePath = L"";

	_bool						m_bIsCloned;

public:
	virtual CComponent*			Clone(class CGameObject* pOwner, void* pArg = nullptr) = 0;
	virtual void				Free() override;
};

END