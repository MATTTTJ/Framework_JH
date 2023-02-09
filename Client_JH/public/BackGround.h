#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CShader;
class CTexture;
class CRenderer;
class CVIBuffer_Rect;
END

BEGIN(Client)

class CBackGround final : public CGameObject
{
private:
	CBackGround(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CBackGround(const CBackGround& rhs);
	virtual ~CBackGround() = default;

public:
	virtual HRESULT Initialize_Prototype()			override;
	virtual HRESULT Initialize_Clone(const wstring& wstrPrototypeTag, void* pArg)	override;
	virtual void	Tick(_double TimeDelta)			override;
	virtual void	Late_Tick(_double TimeDelta)	override;
	virtual HRESULT Render()						override;
	CShader*		Get_ShaderCom() { return m_pShaderCom; }

public:
	


private:
	CShader*				m_pShaderCom	= nullptr;
	CRenderer*				m_pRendererCom	= nullptr;
	CTexture*				m_pTextureCom	= nullptr;
	CVIBuffer_Rect*			m_pVIBufferCom	= nullptr;

private:
	_float4x4				m_ViewMatrix;
	_float4x4				m_ProjMatrix;
	_uint					m_iTextureIndex = 0;
	_float					m_fX, m_fY, m_fSizeX, m_fSizeY;

	_float					m_fCurLifeTime = 0.f;
	_float					m_fLifeTime = 3.f;
	_bool					m_bLifeTimeDone = false;

private:
	HRESULT SetUp_Components();
	HRESULT SetUp_ShaderResources();

public:
	static CBackGround*	 Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(const wstring& wstrPrototypeTag, void* pArg = nullptr) override;
	virtual void Free() override;
};

END