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
class CDamage_Font final :	public CGameObject
{
public:
	enum NUMBUR { ZERO, ONE, TWO, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, NINE, NUM_END };


public:
	CDamage_Font(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CDamage_Font(const CDamage_Font& rhs);
	virtual ~CDamage_Font() = default;

public:
	virtual HRESULT				Initialize_Prototype() override;
	virtual HRESULT				Initialize_Clone(const wstring& wstrPrototypeTag, void* pArg) override;
	virtual void				Tick(_double dTimeDelta) override;
	virtual void				Late_Tick(_double dTimeDelta) override;
	virtual HRESULT				Render() override;

private:
	HRESULT						SetUp_Components();
	HRESULT						SetUp_ShaderResources();

private:
	CRenderer*					m_pRendererCom = nullptr;
	CShader*					m_pShaderCom = nullptr;
	CTexture*					m_pNumberingTexCom[NUM_END] = { nullptr };
	CVIBuffer_Point_Instancing* m_pPointBuffer = nullptr;



	_float						m_fCurLifeTime = 0.f;
	_float						m_fLifeTime = 10.f;


	_float						m_fSizeX;
	_float						m_fSizeY;
	_float2						m_vPSize;
	_float						m_fX;
	_float						m_fY;
	_float4						m_vLook;
	_float4						m_vCamPos;
private:
	int							m_iNumCnt;
	int							m_iNumber;
	_float4						m_vNumColor;
	_int						m_iType = 0;

public:
	static CDamage_Font*		Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject*		Clone(const wstring& wstrPrototypeTag, void* pArg = nullptr) override;
	virtual void				Free() override;
};

END