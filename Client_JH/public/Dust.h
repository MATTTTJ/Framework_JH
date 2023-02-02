#pragma once
#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CRenderer;
class CShader;
class CTexture;
class CVIBuffer_Point_Instancing;
END

BEGIN(Client)
class CDust final :	public CGameObject
{
public:
	typedef struct tagEffectDesc
	{
		GAMEOBJECTDESC		m_tGameObjectDesc;
		CGameObject*		m_pOwner = nullptr;
	}EFFECTDESC;

private:
	CDust(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CDust(const CDust& rhs);
	virtual ~CDust() = default;

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
	CTexture*							m_pNoiseTextureCom = nullptr;
	CTexture*							m_pAlphaTextureCom = nullptr;



	CVIBuffer_Point_Instancing*			m_pPointBuffer = nullptr;
	CGameObject*						m_pOwner = nullptr;

	EFFECTDESC						m_tEffectDesc;

private:
	_float4								m_vLook;
	_float4								m_vUp;
	_float2								m_vPSize;
	_float4								m_vDir;

	_int 								m_iUV_Max_Width_Num;
	_int								m_iUV_Max_Height_Num;
	_int 								m_iUV_Cur_Width_Num;
	_int								m_iUV_Cur_Height_Num;

	_int								m_iPlayOnFrameCnt = 0;
	_int								m_iFrameCnt = 0;
	_float								m_fAlpha = 1.f;
public:
	static CDust*		Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject*				Clone(const wstring& wstrPrototypeTag, void* pArg = nullptr) override;
	virtual void						Free() override;
};
END