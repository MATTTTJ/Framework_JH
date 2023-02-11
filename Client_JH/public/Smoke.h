#pragma once
#include "Client_Defines.h"
#include "GameObject.h"
BEGIN(Engine)
class CRenderer;
class CShader;
class CTexture;
class CVIBuffer_Rect;
END

BEGIN(Client)

class CSmoke final :	public CGameObject
{
public:
	typedef struct tagEffectDesc
	{
		GAMEOBJECTDESC		m_tGameObjectDesc;
		CGameObject*		m_pOwner = nullptr;
	}EFFECTDESC;

private:
	CSmoke(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CSmoke(const CSmoke& rhs);
	virtual ~CSmoke() = default;

public:
	virtual HRESULT						Initialize_Prototype() override;
	virtual HRESULT						Initialize_Clone(const wstring& wstrPrototypeTag, void* pArg) override;
	virtual void						Tick(_double dTimeDelta) override;
	virtual void						Late_Tick(_double dTimeDelta) override;
	virtual HRESULT						Render() override;

private:
	HRESULT								SetUp_Component();
	HRESULT								SetUp_ShaderResources();

private:
	CRenderer*							m_pRendererCom = nullptr;
	CShader*							m_pShaderCom = nullptr;
	CVIBuffer_Rect*						m_pVIBufferCom = nullptr;
	CTexture*							m_pTextureCom = nullptr;
	GAMEOBJECTDESC						m_tEffectDesc;
	_float								m_fMaxMoveDistance =1.f;
	_bool								m_bChangeTime = false;
	_float4								m_vDir;
	_float4								m_vLook;


	_uint								m_iTextureIndex = 0;
	_bool								m_bCloseCam = false;
	_bool								m_bFlowSmoke = false;
	_float								m_fFlowTime = 0.f;
private:
	_int								m_iPlayOnFrameCnt = 0;
	_int								m_iFrameCnt = 0;
	_float								m_fAlpha = 1.f;
public:
	static CSmoke*						Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject*				Clone(const wstring& wstrPrototypeTag, void* pArg = nullptr) override;
	virtual void						Free() override;
};
END
