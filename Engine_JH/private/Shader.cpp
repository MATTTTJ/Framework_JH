#include "stdafx.h"
#include "..\public\Shader.h"
#include "GameUtils.h"
#include "GameInstance.h"
#include "Texture.h"

const array<const char*, PARAM_INT_CNT> CShader::s_ParamIntName{
	"g_int_0","g_int_1","g_int_2","g_int_3"
};
const array<const char*, PARAM_FLOAT_CNT> CShader::s_ParamFloatName{
	"g_float_0","g_float_1","g_float_2","g_float_3"
};
const array<const char*, PARAM_FLOAT2_CNT> CShader::s_ParamFloat2Name{
	"g_vec2_0","g_vec2_1","g_vec2_2","g_vec2_3"
};
const array<const char*, PARAM_FLOAT4_CNT> CShader::s_ParamFloat4Name{
	"g_vec4_0","g_vec4_1","g_vec4_2","g_vec4_3"
};
const array<const char*, PARAM_FLOAT4X4_CNT> CShader::s_ParamFloat4x4Name{
	"g_mat_0","g_mat_1"
};
const array<const char*, PARAM_TEXTURE_CNT> CShader::s_ParamTextureName{
	"g_tex_0","g_tex_1","g_tex_2","g_tex_3","g_tex_4","g_tex_5", "g_tex_6", "g_tex_7"
};
const array<const char*, PARAM_TEXTURE_ON_CNT> CShader::s_ParamTextureOnName{
	"g_tex_on_0","g_tex_on_1","g_tex_on_2","g_tex_on_3","g_tex_on_4","g_tex_on_5", "g_tex_on_6", "g_tex_on_7"
};

_float CShader::s_fTime = 0.f;

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

HRESULT CShader::Initialize_Prototype(const wstring& wstrShaderFilePath, DECLARATIONTYPE eType, const D3D11_INPUT_ELEMENT_DESC* pElements, const _uint iNumElements)
{
	_uint			iHlslFlag = 0;

#ifdef _DEBUG
	iHlslFlag = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;

	m_wstrFilePath = wstrShaderFilePath;
	m_eType = eType;
	m_iElementCnt = iNumElements;

#else
	iHlslFlag = D3DCOMPILE_OPTIMIZATION_LEVEL1;

#endif
	if (FAILED(D3DX11CompileEffectFromFile(wstrShaderFilePath.c_str(),
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

HRESULT CShader::Initialize_Clone(CGameObject* pOwner, void * pArg)
{
	// FAILED_CHECK_RETURN(__super::Initialize_Clone(pOwner, pArg), E_FAIL);
	return S_OK;
}

void CShader::Imgui_RenderProperty()
{
	int iNumPasses = (int)(m_iNumPasses);
	ImGui::InputInt("NumPass", &iNumPasses);
	if (iNumPasses < 0) iNumPasses = 0;
	m_iNumPasses = iNumPasses;

#ifdef _DEBUG
	if (ImGui::Button("ReCompile"))
	{
		ReCompileShader();
	}
#endif
}

#ifdef _DEBUG
void CShader::ReCompileShader()
{
	for (auto& pInputLayout : m_InputLayouts)
		Safe_Release(pInputLayout);

	m_InputLayouts.clear();

	Safe_Release(m_pEffect);


	_uint			iHlslFlag = 0;
	iHlslFlag = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
	wstring Test = L"../Bin/ShaderFiles/Shader_VtxTex.hlsl";
	FAILED_CHECK_RETURN(D3DX11CompileEffectFromFile(m_wstrFilePath.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, iHlslFlag, 0, m_pDevice, &m_pEffect, nullptr), );

	ID3DX11EffectTechnique*	pTechnique = m_pEffect->GetTechniqueByIndex(0);

	D3DX11_TECHNIQUE_DESC	TechniqueDesc;
	pTechnique->GetDesc(&TechniqueDesc);

	m_iNumPasses = TechniqueDesc.Passes;

	for (_uint i = 0; i < m_iNumPasses; ++i)
	{
		ID3D11InputLayout*		pInputLayout = nullptr;

		ID3DX11EffectPass*		pPass = pTechnique->GetPassByIndex(i);
		if (nullptr == pPass)
			return;

		D3DX11_PASS_DESC		PassDesc;
		pPass->GetDesc(&PassDesc);

		if (FAILED(m_pDevice->CreateInputLayout(VTXTEX_DECLARATION::Elements, 2, PassDesc.pIAInputSignature, PassDesc.IAInputSignatureSize, &pInputLayout)))
			return;

		m_InputLayouts.push_back(pInputLayout);
	}
}
#endif

HRESULT CShader::Set_Param_Int(_uint iIdx, const _int* iValue)
{
	if (iIdx < PARAM_INT_CNT)
		return E_FAIL;

	ID3DX11EffectVariable*		pVariable = m_pEffect->GetVariableByName(s_ParamIntName[iIdx]);
	if (nullptr == pVariable)
		return E_FAIL;

	return pVariable->SetRawValue(iValue, 0, sizeof(_int));
}

HRESULT CShader::Set_Param_float(_uint iIdx, const _float* fValue)
{
	if (iIdx < PARAM_FLOAT_CNT)
		return E_FAIL;

	ID3DX11EffectVariable*		pVariable = m_pEffect->GetVariableByName(s_ParamFloatName[iIdx]);
	if (nullptr == pVariable)
		return E_FAIL;

	return pVariable->SetRawValue(fValue, 0, sizeof(_float));
}

HRESULT CShader::Set_Param_Texture(_uint iIdx, CTexture* pTexture, _uint iTexIdx)
{
	if (iIdx < PARAM_TEXTURE_CNT)
		return E_FAIL;
	_tchar wscName[MAX_PATH] = L"";
	CGameUtils::ctwc(s_ParamTextureName[iIdx], wscName);
	
	if (FAILED(pTexture->Bind_ShaderResource(this, wstring(wscName), iTexIdx)))
		return E_FAIL;

	ID3DX11EffectVariable*		pVariable = m_pEffect->GetVariableByName(s_ParamTextureOnName[iIdx]);
	if (nullptr == pVariable)
		return E_FAIL;

	static constexpr _int iOn = 1;
	return pVariable->SetRawValue(&iOn, 0, sizeof(_int));
}

HRESULT CShader::Set_Param_float2(_uint iIdx, const _float2* vValue)
{
	if (iIdx < PARAM_FLOAT2_CNT)
		return E_FAIL;

	ID3DX11EffectVariable*		pVariable = m_pEffect->GetVariableByName(s_ParamFloat2Name[iIdx]);
	if (nullptr == pVariable)
		return E_FAIL;

	return pVariable->SetRawValue(vValue, 0, sizeof(_float2));
}

HRESULT CShader::Set_Param_float4(_uint iIdx, const _float4* vValue)
{
	if (iIdx < PARAM_FLOAT4_CNT)
		return E_FAIL;

	ID3DX11EffectVariable*		pVariable = m_pEffect->GetVariableByName(s_ParamFloat4Name[iIdx]);
	if (nullptr == pVariable)
		return E_FAIL;

	return pVariable->SetRawValue(vValue, 0, sizeof(_float4));
}

HRESULT CShader::Set_Param_float4x4(_uint iIdx, const _float4x4* matValue)
{
	if (iIdx < PARAM_FLOAT4X4_CNT)
		return E_FAIL;

	ID3DX11EffectMatrixVariable*	pVariable = m_pEffect->GetVariableByName(s_ParamFloat4x4Name[iIdx])->AsMatrix();
	if (nullptr == pVariable)
		return E_FAIL;

	return pVariable->SetMatrix((_float*)matValue);
}

HRESULT CShader::Set_Invalidate_Textures()
{
	for (int i = 0; i < PARAM_TEXTURE_CNT; ++i)
	{
		ID3DX11EffectVariable*		pVariable = m_pEffect->GetVariableByName(s_ParamTextureOnName[i]);
		if (nullptr == pVariable)
			return E_FAIL;

		static constexpr _int iOff = 0;
		if (FAILED(pVariable->SetRawValue(&iOff, 0, sizeof(_int))))
			return E_FAIL;
	}

	return S_OK;
}

HRESULT CShader::Set_Params(const ShaderParams& tParam)
{
	if (FAILED(Set_Invalidate_Textures()))
		return E_FAIL;

	Set_ShaderTime();

	for (auto e : tParam.ParamInt)
	{
		if (FAILED(Set_Param_Int(e.first, &e.second)))
			return E_FAIL;
	}

	for (auto e : tParam.ParamFloat)
	{
		if (FAILED(Set_Param_float(e.first, &e.second)))
			return E_FAIL;
	}

	for (auto e : tParam.ParamFloat2)
	{
		if (FAILED(Set_Param_float2(e.first, &e.second)))
			return E_FAIL;
	}

	for (auto e : tParam.ParamFloat4)
	{
		if (FAILED(Set_Param_float4(e.first, &e.second)))
			return E_FAIL;
	}

	for (auto e : tParam.ParamFloat4x4)
	{
		if (FAILED(Set_Param_float4x4(e.first, &e.second)))
			return E_FAIL;
	}

	for (auto e : tParam.ParamTexture)
	{
		if (e.second.first == nullptr)
			continue;

		if (FAILED(Set_Param_Texture(e.first, e.second.first, e.second.second)))
			return E_FAIL;
	}

	return S_OK;
}

HRESULT CShader::Set_ShaderTime()
{
	return Set_RawValue(L"g_Time", &s_fTime, sizeof(_float));
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


	CONTEXT_LOCK
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


CShader * CShader::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, const wstring& pShaderFilePath, DECLARATIONTYPE eType, const D3D11_INPUT_ELEMENT_DESC* pElements, const _uint iNumElements)
{
	CShader*		pInstance = new CShader(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype(pShaderFilePath, eType, pElements, iNumElements)))
	{
		MSG_BOX("Failed to Created : CShader");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CComponent * CShader::Clone(CGameObject* pOwner, void * pArg)
{
	CShader*		pInstance = new CShader(*this);

	if (FAILED(pInstance->Initialize_Clone(pOwner, pArg)))
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
