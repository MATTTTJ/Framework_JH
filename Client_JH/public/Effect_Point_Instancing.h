#pragma once
#include "Client_Defines.h"
#include "GameObject.h"


BEGIN(Engine)
class CShader;
class CTexture;
class CRenderer;
class CVIBuffer_Point_Instancing;
END

BEGIN(Client)
class CEffect_Point_Instancing final :	public CGameObject
{
public:
	CEffect_Point_Instancing(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CEffect_Point_Instancing(const CEffect_Point_Instancing& rhs);
	virtual ~CEffect_Point_Instancing() = default;

	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize_Clone(const wstring& wstrPrototypeTag, void* pArg) override;
	virtual void	Tick(_double dTimeDelta) override;
	virtual void	Late_Tick(_double dTimeDelta) override;
	virtual HRESULT Render() override;

private:
	CShader*					m_pShaderCom = nullptr;
	CRenderer*					m_pRendererCom = nullptr;
	CTexture*					m_pTextureCom = nullptr;
	CVIBuffer_Point_Instancing*	m_pVIBufferCom = nullptr;

private:
	HRESULT SetUp_Components();
	HRESULT SetUp_ShaderResources();

public:
	static CEffect_Point_Instancing*		Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject*					Clone(const wstring& wstrPrototypeTag, void* pArg = nullptr) override;
	virtual void							Free() override;
};

END