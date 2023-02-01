#pragma once
#include "Component.h"

BEGIN(Engine)

enum class EShaderParamType
{
	PARAM_INT,
	PARAM_FLOAT,
	PARAM_FLOAT2,
	PARAM_FLOAT4,
	PARAM_FLOAT4X4,
	PARAM_TEXTURE,
	PARAM_END
};

enum
{
	PARAM_INT_CNT = 4,
	PARAM_FLOAT_CNT = 4,
	PARAM_FLOAT2_CNT = 4,
	PARAM_FLOAT4_CNT = 4,
	PARAM_FLOAT4X4_CNT = 2,
	PARAM_TEXTURE_CNT = 8,
	PARAM_TEXTURE_ON_CNT = 8,
	PARAM_TYPE_CNT = static_cast<_uint>(EShaderParamType::PARAM_END)
};

struct ShaderParams
{
	map<_uint, _int> ParamInt;
	map<_uint, _float> ParamFloat;
	map<_uint, _float2> ParamFloat2;
	map<_uint, _float4> ParamFloat4;
	map<_uint, _float4x4> ParamFloat4x4;
	map<_uint, pair<class CTexture*, _uint>> ParamTexture;
};


class ENGINE_DLL CShader final : public CComponent
{
public:
	enum DECLARATIONTYPE
	{
		DECLARATION_VTXTEX,
		DECLARATION_VTXNORTEX,
		DECLARATION_VTXMODEL,
		DECLARATION_VTXANIMMODEL,
		DECLARATION_VTXPOS,
		DECLARATION_VTXCUBE,
		DECLARATION_VTXPOINTINSTANCE,
		DECLARATION_VTXRECTINSTANCE,
		DECLARATION_END
	};




protected:
	CShader(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CShader(const CShader& rhs);
	virtual ~CShader() = default;

public:
	const DECLARATIONTYPE&		Get_DeclarationType() const { return m_eType; }
	const _uint&				Get_ElementsCnt() const { return m_iElementCnt; }

public:
	_uint						GetNumPasses() const { return m_iNumPasses; }

public:
	virtual HRESULT Initialize_Prototype(const wstring& wstrShaderFilePath, DECLARATIONTYPE eType, const D3D11_INPUT_ELEMENT_DESC* pElements, const _uint iNumElements);
	virtual HRESULT Initialize_Clone(class CGameObject* pOwner, void* pArg) override;
	virtual void	Imgui_RenderProperty() override;
public:
	static const array<const char*, PARAM_INT_CNT> s_ParamIntName;
	static const array<const char*, PARAM_FLOAT_CNT> s_ParamFloatName;
	static const array<const char*, PARAM_FLOAT2_CNT> s_ParamFloat2Name;
	static const array<const char*, PARAM_FLOAT4_CNT> s_ParamFloat4Name;
	static const array<const char*, PARAM_FLOAT4X4_CNT> s_ParamFloat4x4Name;
	static const array<const char*, PARAM_TEXTURE_CNT> s_ParamTextureName;
	static const array<const char*, PARAM_TEXTURE_ON_CNT> s_ParamTextureOnName;

public:
	static void Tick_ShaderTime(_double TimeDelta)
	{
		s_fTime += static_cast<_float>(TimeDelta);
		s_fTime = s_fTime - static_cast<_int>(s_fTime);
	}

#ifdef _DEBUG
	void ReCompileShader();
#endif

public:
	HRESULT Begin(_uint iPassIndex);

	HRESULT	Set_RawValue(const wstring& pConstantName, const void* pData, _uint iLength);
	HRESULT	Set_Matrix(const wstring& pConstantName, const _float4x4* pMatrix);
	HRESULT	Set_MatrixArray(const wstring& pConstantName, const _float4x4* pMatrix, _uint iNumMatrices);
	HRESULT	Set_ShaderResourceView(const wstring& pConstantName, ID3D11ShaderResourceView* pSRV);
	HRESULT	Set_ShaderResourceViewArray(const wstring& pConstantName, ID3D11ShaderResourceView** ppSRV, _uint iNumTextures);

public:
	HRESULT Set_Param_Int(_uint iIdx, const _int* iValue);
	HRESULT Set_Param_float(_uint iIdx, const  _float* fValue);
	HRESULT Set_Param_Texture(_uint iIdx, class CTexture* pTexture, _uint iTexIdx = 0);
	HRESULT Set_Param_float2(_uint iIdx, const  _float2* vValue);
	HRESULT Set_Param_float4(_uint iIdx, const  _float4* vValue);
	HRESULT Set_Param_float4x4(_uint iIdx, const  _float4x4* matValue);
	HRESULT Set_Invalidate_Textures();
	HRESULT Set_Params(const ShaderParams& tParam);
	HRESULT Set_ShaderTime();


private:
	ID3DX11Effect*				m_pEffect = nullptr;
	_uint						m_iNumPasses = 0;
	vector<ID3D11InputLayout*>	m_InputLayouts;
	static _float				s_fTime;
private:
	_uint						m_iElementCnt = 0;
	DECLARATIONTYPE				m_eType = DECLARATION_END;

#ifdef _DEBUG
private:
	wstring						m_shaderFilePath;
	D3D11_INPUT_ELEMENT_DESC*	m_pElements;
	_uint						m_iNumElements;
#endif

public:
	static CShader* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& pShaderFilePath, DECLARATIONTYPE eType, const D3D11_INPUT_ELEMENT_DESC* pElements, const _uint iNumElements);
	virtual CComponent* Clone(class CGameObject* pOwner, void* pArg = nullptr) override;
	virtual void Free() override;
};




END
