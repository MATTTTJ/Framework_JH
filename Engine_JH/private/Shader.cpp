#include "..\public\Shader.h"

#include "GameUtils.h"

CShader::CShader(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CComponent(pDevice, pContext)
{

}

CShader::CShader(const CShader & rhs)
	: CComponent(rhs)
	, m_pEffect(rhs.m_pEffect)
	, m_InputLayouts(rhs.m_InputLayouts)
	, m_iNumPasses(rhs.m_iNumPasses)
{
	Safe_AddRef(m_pEffect);

	for (auto& pInputLayout : m_InputLayouts)
		Safe_AddRef(pInputLayout);

}

HRESULT CShader::Initialize_Prototype(const wstring& pShaderFilePath, const D3D11_INPUT_ELEMENT_DESC* pElements, const _uint iNumElements)
{
	_uint			iHlslFlag = 0;

#ifdef _DEBUG
	iHlslFlag = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#else
	iHlslFlag = D3DCOMPILE_OPTIMIZATION_LEVEL1;

#endif
	if (FAILED(D3DX11CompileEffectFromFile(pShaderFilePath.c_str(),
														nullptr, 
														D3D_COMPILE_STANDARD_FILE_INCLUDE, 
														iHlslFlag, 
														0, 
														m_pDevice, 
														&m_pEffect, nullptr)))
		return E_FAIL;

	ID3DX11EffectTechnique*	pTechnique = m_pEffect->GetTechniqueByIndex(0);

	D3DX11_TECHNIQUE_DESC	TechniqueDesc;
	pTechnique->GetDesc(&TechniqueDesc);

	m_iNumPasses = TechniqueDesc.Passes;

	for (_uint i = 0; i < m_iNumPasses; ++i)
	{
		ID3D11InputLayout*		pInputLayout = nullptr;

		ID3DX11EffectPass*		pPass = pTechnique->GetPassByIndex(i);
		NULL_CHECK_RETURN(pPass, E_FAIL);


		D3DX11_PASS_DESC		PassDesc;
		pPass->GetDesc(&PassDesc);

		if (FAILED(m_pDevice->CreateInputLayout(pElements, iNumElements, PassDesc.pIAInputSignature, PassDesc.IAInputSignatureSize, &pInputLayout)))
			return E_FAIL;

		m_InputLayouts.push_back(pInputLayout);
	}

	return S_OK;
}

HRESULT CShader::Initialize_Clone(void * pArg)
{
	return S_OK;
}

HRESULT CShader::Begin(_uint iPassIndex)
{
	if (nullptr == m_pContext ||
		iPassIndex >= m_iNumPasses)
		return E_FAIL;

	ID3DX11EffectTechnique*	pTechnique = m_pEffect->GetTechniqueByIndex(0);
	NULL_CHECK_RETURN(pTechnique, E_FAIL);

	ID3DX11EffectPass*	pPass = pTechnique->GetPassByIndex(iPassIndex);
	NULL_CHECK_RETURN(pPass, E_FAIL);

	pPass->Apply(0, m_pContext);

	m_pContext->IASetInputLayout(m_InputLayouts[iPassIndex]);

	return S_OK;
}

HRESULT CShader::Set_RawValue(const wstring& pConstantName, const void* pData, _uint iLength)
{
	NULL_CHECK_RETURN(m_pEffect, E_FAIL);

	char	szConstantName[MAX_PATH];
	CGameUtils::wc2c(pConstantName.c_str(), szConstantName);

	ID3DX11EffectVariable*		pVariable = m_pEffect->GetVariableByName(szConstantName);
	NULL_CHECK_RETURN(pVariable, E_FAIL);

	return pVariable->SetRawValue(pData, 0, iLength);
}

HRESULT CShader::Set_Matrix(const wstring& pConstantName, const _float4x4* pMatrix)
{
	NULL_CHECK_RETURN(m_pEffect, E_FAIL);

	char	szConstantName[MAX_PATH];
	CGameUtils::wc2c(pConstantName.c_str(), szConstantName);

	ID3DX11EffectMatrixVariable*	pVariable = m_pEffect->GetVariableByName(szConstantName)->AsMatrix();
	NULL_CHECK_RETURN(pVariable, E_FAIL);

	return pVariable->SetMatrix((_float*)pMatrix);
}

HRESULT CShader::Set_MatrixArray(const wstring& pConstantName, const _float4x4* pMatrix, _uint iNumMatrices)
{
	NULL_CHECK_RETURN(m_pEffect, E_FAIL);

	char	szConstantName[MAX_PATH];
	CGameUtils::wc2c(pConstantName.c_str(), szConstantName);

	ID3DX11EffectMatrixVariable*	pVariable = m_pEffect->GetVariableByName(szConstantName)->AsMatrix();
	NULL_CHECK_RETURN(pVariable, E_FAIL);

	return pVariable->SetMatrixArray((_float*)pMatrix,0,iNumMatrices);
}

HRESULT CShader::Set_ShaderResourceViewArray(const wstring& pConstantName, ID3D11ShaderResourceView ** ppSRV, _uint iNumTextures)
{
	NULL_CHECK_RETURN(m_pEffect, E_FAIL);

	char	szConstantName[MAX_PATH];
	CGameUtils::wc2c(pConstantName.c_str(), szConstantName);

	ID3DX11EffectShaderResourceVariable*		pVariable = m_pEffect->GetVariableByName(szConstantName)->AsShaderResource();
	NULL_CHECK_RETURN(pVariable, E_FAIL);

	return pVariable->SetResourceArray(ppSRV, 0, iNumTextures);
}

HRESULT CShader::Set_ShaderResourceView(const wstring& pConstantName, ID3D11ShaderResourceView * pSRV)
{
	NULL_CHECK_RETURN(m_pEffect, E_FAIL);

	char	szConstantName[MAX_PATH];
	CGameUtils::wc2c(pConstantName.c_str(), szConstantName);

	ID3DX11EffectShaderResourceVariable*		pVariable = m_pEffect->GetVariableByName(szConstantName)->AsShaderResource();
	NULL_CHECK_RETURN(pVariable, E_FAIL);

	return pVariable->SetResource(pSRV);
}


CShader * CShader::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, const wstring& pShaderFilePath, const D3D11_INPUT_ELEMENT_DESC* pElements, const _uint iNumElements)
{
	CShader*		pInstance = new CShader(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype(pShaderFilePath, pElements, iNumElements)))
	{
		MSG_BOX("Failed to Created : CShader");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CComponent * CShader::Clone(void * pArg)
{
	CShader*		pInstance = new CShader(*this);

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSG_BOX("Failed to Cloned : CShader");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CShader::Free()
{
	__super::Free();

	for (auto& pInputLayout : m_InputLayouts)
		Safe_Release(pInputLayout);

	m_InputLayouts.clear();

	Safe_Release(m_pEffect);
}
