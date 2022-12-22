#include "stdafx.h"
#include "..\public\MyFont.h"

CMyFont::CMyFont(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:m_pDevice(pDevice),
	m_pContext(pContext)
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
}

HRESULT CMyFont::Initialize(const _tchar* pFontFilePath)
{
	m_pFont = new SpriteFont(m_pDevice, pFontFilePath);

	m_pBatch = new SpriteBatch(m_pContext);

	return S_OK;
}

HRESULT CMyFont::Render(const _tchar* pText, const _float2& vPosition, _float fRadian, _float2 vScale, _fvector vColor)
{
	m_pContext->GSSetShader(nullptr, nullptr, 0);

	m_pBatch->Begin();

	m_pFont->DrawString(m_pBatch, pText, vPosition, vColor, fRadian, _float2(0.f, 0.f), vScale);

	m_pBatch->End();

	return S_OK;
}

CMyFont* CMyFont::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const _tchar* pFontFilePath)
{
	CMyFont*		pInst = new CMyFont(pDevice, pContext);

	if(FAILED(pInst->Initialize(pFontFilePath)))
	{
		MSG_BOX("Failed to Create : CMyFont");
		Safe_Release(pInst);
		return nullptr;
	}

	return pInst;
}

void CMyFont::Free()
{
	Safe_Delete(m_pFont);
	Safe_Delete(m_pBatch);

	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);
}
