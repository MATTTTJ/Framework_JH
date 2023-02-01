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
class CDefault_Bullet_Birth final :	public CGameObject
{
public:
	typedef struct tagEffectDesc
	{
		GAMEOBJECTDESC		m_tGameObjectDesc;
		CGameObject*		m_pOwner = nullptr;
	}EFFECTDESC;

private:
	CDefault_Bullet_Birth(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CDefault_Bullet_Birth(const CDefault_Bullet_Birth& rhs);
	virtual ~CDefault_Bullet_Birth() = default;

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

public:
	static CDefault_Bullet_Birth*		Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject*				Clone(const wstring& wstrPrototypeTag, void* pArg = nullptr) override;
	virtual void						Free() override;
};
END