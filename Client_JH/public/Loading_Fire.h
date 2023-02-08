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
class CLoading_Fire final :	public CGameObject
{
public:
	typedef struct tagEffectDesc
	{
		GAMEOBJECTDESC		m_tGameObjectDesc;
		CGameObject*		m_pOwner = nullptr;
	}EFFECTDESC;

private:
	CLoading_Fire(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CLoading_Fire(const CLoading_Fire& rhs);
	virtual ~CLoading_Fire() = default;

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
	CTexture*							m_pTextureCom = nullptr;
	CVIBuffer_Rect*						m_pVIBufferCom = nullptr;

	CGameObject*						m_pOwner = nullptr;

	EFFECTDESC							m_tEffectDesc;
	_float4x4							m_ViewMatrix;
	_float4x4							m_ProjMatrix;

private:
	_float								m_fFrame = 0.f;

public:
	static CLoading_Fire*		Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject*				Clone(const wstring& wstrPrototypeTag, void* pArg = nullptr) override;
	virtual void						Free() override;
};
END