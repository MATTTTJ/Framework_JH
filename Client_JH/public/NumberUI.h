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

class CNumberUI final :	public CGameObject
{
public:
	enum NUMUITYPE { NUMUI_GOLD, NUMUI_BULLET, NUMUI_THROW, NUMUI_END };
	enum NUMBUR { ZERO, ONE, TWO, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, NINE, SLASH, NUM_END };

public:
	CNumberUI(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CNumberUI(const CNumberUI& rhs);
	virtual ~CNumberUI() = default;

	virtual HRESULT					Initialize_Prototype() override;
	virtual HRESULT					Initialize_Clone(const wstring& wstrPrototypeTag, void* pArg) override;
	virtual void					Tick(_double dTimeDelta) override;
	virtual void					Late_Tick(_double dTimeDelta) override;
	virtual HRESULT					Render() override;

private:
	HRESULT							SetUp_Components();
	HRESULT							SetUp_ShaderResources();

private:
	CRenderer*					m_pRendererCom = nullptr;
	CShader*					m_pShaderCom = nullptr;
	CVIBuffer_Rect*				m_pVIBufferCom = nullptr;
	CTexture*					m_pNumberingTexCom[NUM_END] = { nullptr };
	_float						m_fSizeX;
	_float						m_fSizeY;
	_float						m_fX;
	_float						m_fY;
	_float4x4					m_ViewMatrix;
	_float4x4					m_ProjMatrix;
	int								m_iNumCnt;
	int								m_iNumber;
	_float4							m_vNumColor;
	_int							m_iType = 0;
public:
	static CNumberUI*				Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject*			Clone(const wstring& wstrPrototypeTag, void* pArg = nullptr) override;
	virtual void					Free() override;
};

END