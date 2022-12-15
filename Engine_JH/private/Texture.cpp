#include "stdafx.h"
#include "..\public\Texture.h"
#include "Shader.h"

CTexture::CTexture(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CComponent(pDevice, pContext)
{

}

CTexture::CTexture(const CTexture & rhs)
	: CComponent(rhs)
	, m_pTextures(rhs.m_pTextures)
	, m_iNumTextures(rhs.m_iNumTextures)
{
	for (_uint i = 0; i < m_iNumTextures; ++i)
		Safe_AddRef(m_pTextures[i]);

}

HRESULT CTexture::Initialize_Prototype(const wstring wstrTextureFilePath, _uint iNumTextures)
{
	m_pTextures = new ID3D11ShaderResourceView*[iNumTextures];

	m_wstrFilePath = wstrTextureFilePath;
	m_iNumTextures = iNumTextures;

	for (_uint i = 0; i < m_iNumTextures; ++i)
	{
		_tchar	szTexturePath[MAX_PATH] = L"";

		wsprintf(szTexturePath, wstrTextureFilePath.c_str(), i);

		/* 문자열 분해하기. */
		_tchar			szExt[MAX_PATH] = L"";

		_wsplitpath_s(szTexturePath, nullptr, 0, nullptr, 0, nullptr, 0, szExt, MAX_PATH);

		HRESULT		hr = 0;

		if (!lstrcmp(szExt, L".tga"))
			return E_FAIL;
		else if (!lstrcmp(szExt, L".dds"))
			hr = DirectX::CreateDDSTextureFromFile(m_pDevice, szTexturePath, nullptr, &m_pTextures[i]);
		else //png
			hr = DirectX::CreateWICTextureFromFile(m_pDevice, szTexturePath, nullptr, &m_pTextures[i]);

		if (FAILED(hr))
			return E_FAIL;

	}

	return S_OK;
}

HRESULT CTexture::Initialize_Clone(CGameObject* pOwner, void * pArg)
{
	FAILED_CHECK_RETURN(__super::Initialize_Clone(pOwner, pArg), E_FAIL);


	return S_OK;
}

HRESULT CTexture::Bind_ShaderResources(CShader * pShaderCom, const wstring pConstantName)
{
	return pShaderCom->Set_ShaderResourceViewArray(pConstantName, m_pTextures, m_iNumTextures);
}

HRESULT CTexture::Bind_ShaderResource(CShader * pShaderCom, const wstring pConstantName, _uint iTextureIndex)
{
	if (nullptr == pShaderCom ||
		iTextureIndex >= m_iNumTextures)
		return E_FAIL;

	return pShaderCom->Set_ShaderResourceView(pConstantName, m_pTextures[iTextureIndex]);
}



CTexture * CTexture::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, const wstring pTextureFilePath, _uint iNumTextures)
{
	CTexture*		pInstance = new CTexture(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype(pTextureFilePath, iNumTextures)))
	{
		MSG_BOX("Failed to Created : CTexture");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CComponent * CTexture::Clone(CGameObject* pOwner, void * pArg)
{
	CTexture*		pInstance = new CTexture(*this);

	if (FAILED(pInstance->Initialize_Clone(pOwner, pArg)))
	{
		MSG_BOX("Failed to Cloned : CTexture");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CTexture::Free()
{
	__super::Free();

	for (_uint i = 0; i < m_iNumTextures; ++i)
		Safe_Release(m_pTextures[i]);

	if (false == m_bIsCloned)
		Safe_Delete_Array(m_pTextures);
}
