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
	const wstring&				Get_FilePath() const { return m_wstrFilePath; }

public:
	virtual HRESULT				Initialize_Prototype();
	virtual HRESULT				Initialize_Clone(void* pArg);

	virtual void				Imgui_RenderProperty() {}


protected:
	ID3D11Device*				m_pDevice = nullptr;
	ID3D11DeviceContext*		m_pContext = nullptr;
	_bool						m_bIsCloned;

	wstring						m_wstrFilePath = L"";
public:
	virtual CComponent*			Clone(void* pArg = nullptr) = 0;
	virtual void				Free() override;
};

END