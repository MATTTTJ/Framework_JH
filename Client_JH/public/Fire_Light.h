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
class CFire_Light final :	public CGameObject
{
public:
	typedef struct tagEffectDesc
	{
		GAMEOBJECTDESC		m_tGameObjectDesc;
		CGameObject*		m_pOwner = nullptr;
	}EFFECTDESC;

private:
	CFire_Light(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CFire_Light(const CFire_Light& rhs);
	virtual ~CFire_Light() = default;

public:
	void								Set_PlayerTransform(CTransform* pTrans);
	
	virtual HRESULT						Initialize_Prototype() override;
	virtual HRESULT						Initialize_Clone(const wstring& wstrPrototypeTag, void* pArg) override;
	virtual void						Tick(_double dTimeDelta) override;
	virtual void						Late_Tick(_double dTimeDelta) override;
	virtual HRESULT						Render() override;
	void								Set_Lobby(_bool trueIsLobby) { m_bLobby = trueIsLobby; }
	void								Set_Boss(_bool trueIsBoss) { m_bBoss = trueIsBoss; }
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

	CTransform*							m_pTargetTransform = nullptr;
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
	_int								m_iLightNumber = 0;

	_float								m_fMaxRange = 0.f;
	_float								m_fMinRange = 9.f;
	_bool								m_bRangeChange = false;
	_bool								m_bLobby = false;
	_bool								m_bBoss = false;
public:
	static CFire_Light*		Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject*				Clone(const wstring& wstrPrototypeTag, void* pArg = nullptr) override;
	virtual void						Free() override;
};
END