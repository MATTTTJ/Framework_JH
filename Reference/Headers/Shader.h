#pragma once
#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CShader final : public CComponent
{


protected:
	CShader(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CShader(const CShader& rhs);
	virtual ~CShader() = default;

public:
	virtual HRESULT Initialize_Prototype(const wstring& pShaderFilePath, const D3D11_INPUT_ELEMENT_DESC* pElements, const _uint iNumElements);
	virtual HRESULT Initialize_Clone(void* pArg) override;

public:
	HRESULT Begin(_uint iPassIndex);

	HRESULT	Set_RawValue(const wstring& pConstantName, const void* pData, _uint iLength);
	HRESULT	Set_Matrix(const wstring& pConstantName, const _float4x4* pMatrix);
	HRESULT	Set_MatrixArray(const wstring& pConstantName, const _float4x4* pMatrix, _uint iNumMatrices);
	HRESULT	Set_ShaderResourceView(const wstring& pConstantName, ID3D11ShaderResourceView* pSRV);
	HRESULT	Set_ShaderResourceViewArray(const wstring& pConstantName, ID3D11ShaderResourceView** ppSRV, _uint iNumTextures);


private:
	ID3DX11Effect*				m_pEffect = nullptr;
	vector<ID3D11InputLayout*>	m_InputLayouts;

private:
	_uint						m_iNumPasses = 0;

public:
	static CShader* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& pShaderFilePath, const D3D11_INPUT_ELEMENT_DESC* pElements, const _uint iNumElements);
	virtual CComponent* Clone(void* pArg = nullptr) override;
	virtual void Free() override;
};


END