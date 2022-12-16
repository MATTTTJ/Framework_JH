#pragma once
#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CShader final : public CComponent
{
public:
	enum DECLARATIONTYPE { DECLARATION_VTXTEX, DECLARATION_VTXNORTEX,DECLARATION_VTXMODEL,DECLARATION_VTXANIMMODEL, DECLARATION_VTXPOS, DECLARATION_END	};

protected:
	CShader(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CShader(const CShader& rhs);
	virtual ~CShader() = default;

public:
	const DECLARATIONTYPE&		Get_DeclarationType() const { return m_eType; }
	const _uint&				Get_ElementsCnt() const { return m_iElementCnt; }
public:
	virtual HRESULT Initialize_Prototype(const wstring& wstrShaderFilePath, DECLARATIONTYPE eType, const D3D11_INPUT_ELEMENT_DESC* pElements, const _uint iNumElements);
	virtual HRESULT Initialize_Clone(class CGameObject* pOwner, void* pArg) override;

public:
	HRESULT Begin(_uint iPassIndex);

	HRESULT	Set_RawValue(const wstring& pConstantName, const void* pData, _uint iLength);
	HRESULT	Set_Matrix(const wstring& pConstantName, const _float4x4* pMatrix);
	HRESULT	Set_MatrixArray(const wstring& pConstantName, const _float4x4* pMatrix, _uint iNumMatrices);
	HRESULT	Set_ShaderResourceView(const wstring& pConstantName, ID3D11ShaderResourceView* pSRV);
	HRESULT	Set_ShaderResourceViewArray(const wstring& pConstantName, ID3D11ShaderResourceView** ppSRV, _uint iNumTextures);


private:
	ID3DX11Effect*				m_pEffect = nullptr;
	_uint						m_iNumPasses = 0;
	vector<ID3D11InputLayout*>	m_InputLayouts;

private:
	_uint						m_iElementCnt = 0;
	DECLARATIONTYPE				m_eType = DECLARATION_END;

public:
	static CShader* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& pShaderFilePath, DECLARATIONTYPE eType, const D3D11_INPUT_ELEMENT_DESC* pElements, const _uint iNumElements);
	virtual CComponent* Clone(class CGameObject* pOwner, void* pArg = nullptr) override;
	virtual void Free() override;
};


END