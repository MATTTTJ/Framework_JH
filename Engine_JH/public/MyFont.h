#pragma once
#include "Base.h"

BEGIN(Engine)

class CMyFont final :	public CBase
{
private:
	explicit CMyFont(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CMyFont() = default;

public:
	HRESULT		Ready_Font(const _tchar* pFontType, const _uint& iWidth, const _uint& iHeight, const _uint& iWeight);
	void		Render_Font(const _tchar* pString, const _float2* pPos, const _float4 Color);

private:
	ID3D11Device*				m_pDevice;
	ID3D11DeviceContext*		m_pContext;
};

END