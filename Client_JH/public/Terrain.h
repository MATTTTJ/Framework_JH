#pragma once
#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CShader;
class CTexture;
class CRenderer;
class CTransform;
class CVIBuffer_Terrain;
END

BEGIN(Client)

class CTerrain final : public CGameObject
{
private:
	CTerrain(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CTerrain(const CTerrain& rhs);
	virtual ~CTerrain() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize_Clone(void* pArg) override;
	virtual void	Tick(_double TimeDelta) override;
	virtual void	Late_Tick(_double TimeDelta) override;
	virtual HRESULT Render() override;

private:
	CShader*				m_pShaderCom	= nullptr;
	CTexture*				m_pTextureCom	= nullptr;
	CRenderer*				m_pRendererCom	= nullptr;
	CVIBuffer_Terrain*		m_pVIBufferCom	= nullptr;
	CTransform*				m_pTransformCom = nullptr;

private:
	HRESULT		SetUp_Components();
	HRESULT		SetUp_ShaderResources();

public:
	static CTerrain*		Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject*	Clone(void* pArg = nullptr) override;
	virtual void			Free() override;
};

END