#pragma once
#include "Component.h"

BEGIN(Engine)

class CShader final : public CComponent
{
public:
	CShader(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CShader(const CShader& rhs);
	virtual ~CShader() = default;

	virtual HRESULT Initialize_Prototype(const _tchar * pShaderFilePath);
	virtual HRESULT Initialize_Clone(void* pArg) override;

public:
	static CShader*		Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const _tchar * pShaderFilePath);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END