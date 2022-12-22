#pragma once
#include "Base.h"
BEGIN(Engine)

class CFontMgr final : public CBase
{
	DECLARE_SINGLETON(CFontMgr);
public:
	CFontMgr();
	virtual ~CFontMgr() = default;

public:
	HRESULT		Add_Font(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const _tchar* pFontTag, const _tchar* pFontFilePath);
	HRESULT		Render_Font(const _tchar* pFontTag, const _tchar* pText, const _float2& vPos, _float fRadian, _float2 vScale, _fvector vColor);

private:
	map<const _tchar* , class CMyFont*>		m_mapFont;

private:
	class CMyFont*		Find_Font(const _tchar* pFontTag);

public:
	virtual void Free() override;
};

END
