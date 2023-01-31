#pragma once
#include "Client_Defines.h"
#include "Effect.h"

BEGIN(Engine)
class CShader;
class CTexture;
class CVIBuffer_Point;
END

BEGIN(Client)
class CEffect_Point final :	public CEffect
{
public:
	CEffect_Point(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CEffect_Point(const CEffect_Point& rhs);
	virtual ~CEffect_Point() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize_Clone(const wstring& wstrPrototypeTag, void* pArg = nullptr) override;
	virtual void Tick(_float fTimeDelta);
	virtual void Late_Tick(_float fTimeDelta);
	virtual HRESULT Render();

private:
	CShader*						m_pShaderCom = nullptr;
	CTexture*						m_pTextureCom = nullptr;
	CVIBuffer_Point*				m_pVIBufferCom = nullptr;

private:
	virtual HRESULT SetUp_Components();
	virtual HRESULT SetUp_ShaderResource();

public:
	static CEffect_Point* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(const wstring& wstrPrototypeTag, void* pArg= nullptr) override;
	virtual void Free() override;
};

END