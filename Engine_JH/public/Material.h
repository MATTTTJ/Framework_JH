#pragma once
#include "Component.h"
#include "Model.h"
#include "Shader.h"

BEGIN(Engine)
class ENGINE_DLL CMaterial :	public CComponent
{
protected:
	CMaterial(ID3D11Device* pDevice, ID3D11DeviceContext*	pContext);
	CMaterial(const CMaterial& rhs);
	virtual ~CMaterial() = default;

public:
	virtual HRESULT		Initialize_Prototype(const char* pMtrlFilePath);
	virtual void		SaveToJson(Json& json);
	virtual void		Imgui_RenderProperty() override;

public:
	class CShader*		GetShader() { return m_pShader; }

	virtual void		SetAnimType(CModel::MODELTYPE eType) {}
	void				BindMatrices(class CTransform* pTransform);
	void				BindMatrices(const _float4x4& WorldMatrix);
	virtual void		Begin();
	ShaderParams&		GetParam() { return m_tParam; }

protected:
	virtual void		SetShaderResources();

protected:
	class CShader*		m_pShader = nullptr;
	ShaderParams		m_tParam;
	_uint				m_iPassIdx = 0;

public:
	virtual void Free() override;
	static CMaterial* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const char* pMtrlFilePath);
	virtual CComponent* Clone(CGameObject* pOwner, void* pArg = nullptr) override;

};

// void ENGINE_DLL LoadValorantMaterials(const char* pMaterialJsonPath);
// void ENGINE_DLL LoadEmptyMaterials();

END