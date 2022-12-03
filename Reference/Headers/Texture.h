#pragma once
#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CTexture final : public CComponent
{
protected:
	CTexture(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CTexture(const CTexture& rhs);
	virtual ~CTexture() = default;

public:
	virtual HRESULT Initialize_Prototype(const wstring pTextureFilePath, _uint iNumTextures);
	virtual HRESULT Initialize_Clone(void* pArg) override;

public:
	HRESULT	Bind_ShaderResource(class CShader* pShaderCom, const wstring pConstantName, _uint iTextureIndex = 0);
	HRESULT	Bind_ShaderResources(class CShader* pShaderCom, const wstring pConstantName);

private:
	_uint										m_iNumTextures = 0;
	ID3D11ShaderResourceView**					m_pTextures = nullptr;

public:
	static CTexture*	Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring pTextureFilePath, _uint iNumTextures = 1);
	virtual CComponent*	Clone(void* pArg = nullptr) override;
	virtual void		Free() override;
};

END