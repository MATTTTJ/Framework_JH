#pragma once
#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CRenderer;
class CShader;
class CModel;
class CTexture;
END

BEGIN(Client)
class CSphere_Effect final : public CGameObject
{
public:
	CSphere_Effect(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CSphere_Effect(const CSphere_Effect& rhs);
	virtual ~CSphere_Effect() = default;

public:
	virtual HRESULT			Initialize_Prototype() override;
	virtual HRESULT			Initialize_Clone(const wstring& wstrPrototypeTag, void* pArg) override;
	virtual void			Tick(_double dTimeDelta) override;
	virtual void			Late_Tick(_double dTimeDelta) override;
	virtual HRESULT			Render() override;

private:
	CRenderer*				m_pRendererCom = nullptr;
	CShader*				m_pShaderCom = nullptr;
	CTexture*				m_pTextureCom = nullptr;
	CModel*					m_pModelCom = nullptr;
	_uint					m_iTextureIndex = 0;

	_float3					m_vInitScale;
	_float					m_fTime = 0.f;
	_float					m_fLifeTime = 1.f;
	_float					m_fCurLifeTime = 0.f;

private:
	HRESULT					SetUp_Component();
	HRESULT					SetUp_ShaderResource();
		
public:
	static	CSphere_Effect*		Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject*	Clone(const wstring& wstrPrototypeTag, void* pArg = nullptr) override;
	virtual void			Free() override;
};

END