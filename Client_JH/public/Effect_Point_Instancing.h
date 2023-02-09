#pragma once
#include "Client_Defines.h"
#include "GameObject.h"


BEGIN(Engine)
class CShader;
class CTexture;
class CRenderer;
class CVIBuffer_Point_Instancing;
END

BEGIN(Client)
class CEffect_Point_Instancing final :	public CGameObject
{
public:
	enum COLOR { COLOR_ORANGE, COLOR_SKY, COLOR_GREEN, COLOR_BLUE,COLOR_RED, COLOR_END};

	typedef struct tagSparkOption
	{
		COLOR m_eColor;
		CGameObject::GAMEOBJECTDESC m_tGameObjectDesc;
	}SPARKOPTION;

public:
	CEffect_Point_Instancing(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CEffect_Point_Instancing(const CEffect_Point_Instancing& rhs);
	virtual ~CEffect_Point_Instancing() = default;

	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize_Clone(const wstring& wstrPrototypeTag, void* pArg) override;
	virtual void	Tick(_double dTimeDelta) override;
	virtual void	Late_Tick(_double dTimeDelta) override;
	virtual HRESULT Render() override;

private:
	CShader*					m_pShaderCom = nullptr;
	CRenderer*					m_pRendererCom = nullptr;
	CTexture*					m_pTexture_A_Com = nullptr;
	CTexture*					m_pTexture_B_Com = nullptr;
	SPARKOPTION					m_tSparkOption;
	_uint						m_iColorType = COLOR_END;
	_float2						m_vPSize;
	_float						m_fSizeable;
	_float						m_fJumpPower;
	_float						m_fDownSpeed;
	CVIBuffer_Point_Instancing*	m_pVIBufferCom = nullptr;

private:
	HRESULT SetUp_Components();
	HRESULT SetUp_ShaderResources();

public:
	static CEffect_Point_Instancing*		Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject*					Clone(const wstring& wstrPrototypeTag, void* pArg = nullptr) override;
	virtual void							Free() override;
};

END