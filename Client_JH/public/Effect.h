#pragma once
#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CRenderer;
class CShader;
class CTexture;
class CVIBuffer_Rect;
class CCollider;
class CVIBuffer_Point_Instancing;
END


BEGIN(Client)
class CEffect abstract : public CGameObject
{
public:
	CEffect(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CEffect(const CEffect& rhs);
	virtual ~CEffect() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize_Clone(const wstring& wstrPrototypeTag, void* pArg) override;
	virtual void	Tick(_double dTimeDelta) override;
	virtual void	Late_Tick(_double dTimeDelta) override;
	virtual HRESULT Render() override;

private:
	virtual HRESULT			SetUp_Components();
	virtual HRESULT			SetUp_ShaderResources();

private:
	CGameObject*				m_pOwner = nullptr;
	CRenderer*					m_pRendererCom = nullptr;
	CShader*					m_pShaderCom = nullptr;
	CTexture*					m_pTextureCom = nullptr;
	CVIBuffer_Rect*				m_pVIBufferCom = nullptr;
	CVIBuffer_Point_Instancing*	m_pPointBuffer = nullptr;



public:
	virtual CGameObject* Clone(const wstring& wstrPrototypeTag, void* pArg = nullptr) PURE;
	virtual void		 Free() override;
};

END;