#pragma once
#include "Client_Defines.h"
#include "GameObject.h"



BEGIN(Engine)
class CShader;
class CTexture;
class CRenderer;
class CVIBuffer_Cube;
END

BEGIN(Client)
class CSky final : public CGameObject
{
public:
	CSky(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CSky(const CSky& rhs);
	virtual ~CSky() = default;

public:
	void					Set_PlayerTransform(CTransform* pTrans) { m_pTargetTransform = pTrans; }

	virtual HRESULT			Initialize_Prototype() override;
	virtual HRESULT			Initialize_Clone(const wstring& wstrPrototypeTag, void* pArg) override;
	virtual void			Tick(_double dTimeDelta) override;
	virtual void			Late_Tick(_double dTimeDelta) override;
	virtual HRESULT			Render() override;


private:
	CShader*				m_pShaderCom = nullptr;
	CTexture*				m_pTextureCom = nullptr;
	CRenderer*				m_pRendererCom = nullptr;
	CVIBuffer_Cube*			m_pVIBufferCom = nullptr;
	CTransform*				m_pTargetTransform = nullptr;
private:
	_float					m_vColorDown = 1.f; 
	_float4					m_vInitPos;
	_float					m_fUpSpeed;


	_float					m_fMaxRange = 0.f;
	_bool					m_bRangeChange = false;
	_int					m_iLightNumber = 0;

	_float					m_fSize = 0.3f;
private:
	HRESULT SetUp_Components();
	HRESULT SetUp_ShaderResources();

public:
	static CSky*			Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject*	Clone(const wstring& wstrPrototypeTag, void* pArg) override;
	virtual void			Free() override;
};




END
