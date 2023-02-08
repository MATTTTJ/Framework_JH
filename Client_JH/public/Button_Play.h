#pragma once
#include "Client_Defines.h"
#include "GameObject.h"
BEGIN(Engine)
class CRenderer;
class CShader;
class CVIBuffer_Rect;
class CTexture;
END

BEGIN(Client)

class CButton_Play final :	public CGameObject
{
public:
	CButton_Play(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CButton_Play(const CButton_Play& rhs);
	virtual ~CButton_Play() = default;


	virtual HRESULT					Initialize_Prototype() override;
	virtual HRESULT					Initialize_Clone(const wstring& wstrPrototypeTag, void* pArg) override;
	virtual void					Tick(_double dTimeDelta) override;
	virtual void					Late_Tick(_double dTimeDelta) override;
	virtual HRESULT					Render() override;
	_bool							Check_Clicked() { return m_bClicked; }
private:
	HRESULT							SetUp_Components();
	HRESULT							SetUp_ShaderResources();

private:
	CRenderer*					m_pRendererCom = nullptr;
	CShader*					m_pShaderCom = nullptr;
	CVIBuffer_Rect*				m_pVIBufferCom = nullptr;
	CTexture*					m_pTextureCom =  nullptr ;
	_float						m_fSizeX;
	_float						m_fSizeY;
	_float						m_fX;
	_float						m_fY;
	_float4x4					m_ViewMatrix;
	_float4x4					m_ProjMatrix;
	_int						m_iType = 0;
	_float4						m_vInitPos;
	RECT						m_Rect;
	_bool						m_bMouseHover = false;
	_float						m_fHoverScale = 1.f;

	_bool						m_bClicked = false;
public:
	static CButton_Play*			Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject*			Clone(const wstring& wstrPrototypeTag, void* pArg = nullptr) override;
	virtual void					Free() override;
};

END