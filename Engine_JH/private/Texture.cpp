#include "..\public\Texture.h"
#include "Shader.h"

CTexture::CTexture(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CComponent(pDevice, pContext)
{

}

CTexture::CTexture(const CTexture & rhs)
	: CComponent(rhs)
	, m_Textures(rhs.m_Textures)
	, m_iNumTextures(rhs.m_iNumTextures)
{
	for (auto& pTexture : m_Textures)
		Safe_AddRef(pTexture);

}

HRESULT CTexture::Initialize_Prototype(const _tchar* pTextureFilePath, _uint iNumTextures)
{

	m_Textures.reserve(iNumTextures);

	m_iNumTextures = iNumTextures;

	for (_uint i = 0; i < m_iNumTextures; ++i)
	{
		ID3D11ShaderResourceView*		pSRV = nullptr;

		_tchar	szTexturePath[MAX_PATH] = TEXT("");

		wsprintf(szTexturePath, pTextureFilePath, i);

		/* 문자열 분해하기. */
		_tchar			szExt[MAX_PATH] = TEXT("");

		_wsplitpath_s(szTexturePath, nullptr, 0, nullptr, 0, nullptr, 0, szExt, MAX_PATH);

		HRESULT		hr = 0;

		if (!lstrcmp(szExt, TEXT(".tga")))
			return E_FAIL;
		else if (!lstrcmp(szExt, TEXT(".dds")))
			hr = DirectX::CreateDDSTextureFromFile(m_pDevice, szTexturePath, nullptr, &pSRV);
		else
			hr = DirectX::CreateWICTextureFromFile(m_pDevice, szTexturePath, nullptr, &pSRV);

		if (FAILED(hr))
			return E_FAIL;

		m_Textures.push_back(pSRV);
	}

	return S_OK;
}

HRESULT CTexture::Initialize_Clone(void * pArg)
{
	return S_OK;
}

HRESULT CTexture::Bind_ShaderResource(CShader * pShaderCom, const char * pConstantName, _uint iTextureIndex)
{
	if (nullptr == pShaderCom ||
		iTextureIndex >= m_iNumTextures)
		return E_FAIL;

	return pShaderCom->Set_ShaderResourceView(pConstantName, m_Textures[iTextureIndex]);
}



CTexture * CTexture::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, const _tchar * pTextureFilePath, _uint iNumTextures)
{
	CTexture*		pInstance = new CTexture(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype(pTextureFilePath, iNumTextures)))
	{
		MSG_BOX("Failed to Created : CTexture");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CComponent * CTexture::Clone(void * pArg)
{
	CTexture*		pInstance = new CTexture(*this);

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSG_BOX("Failed to Cloned : CTexture");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CTexture::Free()
{
	__super::Free();

	for (auto& pTexture : m_Textures)
		Safe_Release(pTexture);

	m_Textures.clear();
}
