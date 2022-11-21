#pragma once
#include "Base.h"

BEGIN(Engine)

class CLight final : public CBase
{
public:
	CLight(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CLight() = default;

public:
	const LIGHTDESC*		Get_LightDesc() const
	{
		return &m_LightDesc;
	}

public:
	HRESULT					Initialize(const LIGHTDESC& LightDesc);

private:
	ID3D11Device*			m_pDevice;
	ID3D11DeviceContext*	m_pContext;

private:
	LIGHTDESC				m_LightDesc;


public:
	static CLight*			Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const LIGHTDESC& LightDesc);
	virtual void			Free() override;
};

END