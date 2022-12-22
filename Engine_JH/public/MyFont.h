#pragma once
#include "Base.h"

BEGIN(Engine)

class CMyFont final :	public CBase
{
private:
	CMyFont(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CMyFont() = default;

public:

	HRESULT					Initialize(const _tchar* pFontFilePath);
	HRESULT					Render(const _tchar* pText, const _float2& vPosition, _float fRadian, _float2 vScale, _fvector vColor);

private:
	ID3D11Device*			m_pDevice = nullptr;
	ID3D11DeviceContext*	m_pContext = nullptr;

private:
	SpriteFont*				m_pFont = nullptr;
	SpriteBatch*			m_pBatch = nullptr;

public:
	static CMyFont*			Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const _tchar* pFontFilePath);
	void					Free() override;

};

END