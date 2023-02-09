#pragma once
#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CShader;
class CRenderer;
class CModel;
END

BEGIN(Client)

class CLobby_Owner final : public CGameObject
{
private:
	CLobby_Owner(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CLobby_Owner(const CLobby_Owner& rhs);
	virtual ~CLobby_Owner() = default;

public:
	virtual HRESULT			Initialize_Prototype() override;
	virtual HRESULT			Initialize_Clone(const wstring& wstrPrototypeTag, void* pArg) override;
	virtual void			Tick(_double TimeDelta) override;
	virtual void			Late_Tick(_double TimeDelta) override;
	virtual HRESULT			Render() override;
	virtual HRESULT			Render_ShadowDepth();
	virtual HRESULT			Render_OutLineFlag();

private:
	CShader*				m_pShaderCom	= nullptr;
	CRenderer*				m_pRendererCom	= nullptr;
	CModel*					m_pModelCom		= nullptr;
	_bool					m_bNormalTexOn = false;
	_float					m_fDegree = 0.f;
private:
	HRESULT					SetUp_Components();
	HRESULT					SetUp_ShaderResources();

public:
	static	CLobby_Owner*		Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject*	Clone(const wstring& wstrPrototypeTag, void* pArg = nullptr) override;
	virtual void			Free() override;
};

END