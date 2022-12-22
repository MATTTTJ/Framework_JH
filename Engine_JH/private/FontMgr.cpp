#include "stdafx.h"
#include "..\public\FontMgr.h"

#include "MyFont.h"

IMPLEMENT_SINGLETON(CFontMgr)

CFontMgr::CFontMgr()
{
}

HRESULT CFontMgr::Add_Font(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const _tchar* pFontTag,
	const _tchar* pFontFilePath)
{
	if (nullptr != Find_Font(pFontTag))
		return E_FAIL;

	CMyFont*		pFont = CMyFont::Create(pDevice, pContext, pFontFilePath);

	NULL_CHECK_RETURN(pFontTag, E_FAIL);

	m_mapFont.emplace(pFontTag, pFont);

	return S_OK;
}

HRESULT CFontMgr::Render_Font(const _tchar* pFontTag, const _tchar* pText, const _float2& vPos, _float fRadian,
	_float2 vScale, _fvector vColor)
{
	CMyFont*		pFont = Find_Font(pFontTag);

	NULL_CHECK_RETURN(pFont, E_FAIL);

	return pFont->Render(pText, vPos, fRadian, vScale, vColor);
}

CMyFont* CFontMgr::Find_Font(const _tchar* pFontTag)
{
	auto iter = find_if(m_mapFont.begin(), m_mapFont.end(), CTag_Finder(pFontTag));

	if (iter == m_mapFont.end())
		return nullptr;

	return iter->second;
}

void CFontMgr::Free()
{
	for (auto& Pair : m_mapFont)
		Safe_Release(Pair.second);

	m_mapFont.clear();
}
