#pragma once
#include "Base.h"

BEGIN(Engine)

class CLight final : public CBase
{
public:
	enum TRANSFORMSTATE { D3DTS_VIEW, D3DTS_PROJ, D3DTS_END };

public:
	CLight(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CLight() = default;

public:
	const LIGHTDESC*		Get_LightDesc() const
	{
		return &m_LightDesc;
	}

	void Set_LightPos(_fvector vPos) { XMStoreFloat4(&m_LightDesc.vPosition, vPos); }
	void Set_LightRange(_float fRange) { m_LightDesc.fRange = fRange; }
	void Set_LightDirection(_float4 vDirection) { m_LightDesc.vDirection = vDirection; }



public:
	HRESULT					Initialize(const LIGHTDESC& LightDesc);
	HRESULT					Render(class CVIBuffer_Rect* pVIBuffer, class CShader* pShader);

	// TYPE_DIRECTIONAL ¿ë 
	void					Update(_fmatrix CamWorldMatrix, _float4* vFrustumCorners);

	_float4x4				Get_Transform(TRANSFORMSTATE eState) { return m_TransformState[eState]; }
	_float4x4				Get_Transform_TP(TRANSFORMSTATE eState) { return m_TransformState_TP[eState]; }

private:
	ID3D11Device*			m_pDevice = nullptr;
	ID3D11DeviceContext*	m_pContext = nullptr;
	LIGHTDESC				m_LightDesc;

private:
	_float4x4				m_TransformState[D3DTS_END];
	_float4x4				m_TransformState_TP[D3DTS_END];

public:
	static CLight*			Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const LIGHTDESC& LightDesc);
	virtual void			Free() override;
};

END