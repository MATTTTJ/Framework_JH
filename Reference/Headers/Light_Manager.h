#pragma once
#include "Base.h"
#include "Light.h"

BEGIN(Engine)
	class CLight_Manager final : public CBase
{
	DECLARE_SINGLETON(CLight_Manager)
public:
	CLight_Manager();
	virtual ~CLight_Manager() = default;

public:
	const LIGHTDESC*		Get_LightDesc(_uint iIndex);

	void Set_LightPos(_uint iIndex, _fvector vPos);
	void Set_LightRange(_uint iIndex, _float fRange);
	void Set_LightDirection(_uint iIndex, _float4 vDirection);

	_float4x4 Get_Transform(_uint iIndex, _uint eState); //1. ºä, 2. Åõ¿µ
	_float4x4 Get_Transform_TP(_uint iIndex, _uint eState);

	void		CascadeUpdate(_float4x4 CamWorldMatrix, _float4* vFrustumCorners);
	HRESULT		Delete_Light(_uint iLightIndex);

	_bool&		Set_LightEnable(_uint iIndex)
	{
		return m_vecLight[iIndex]->Set_Enable();
	}
public:
	HRESULT					Add_Light(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const LIGHTDESC& LightDesc);
	void					Render_Light(class CVIBuffer_Rect* pVIBuffer, class CShader* pShader);
	void					Clear();
	
private:
	vector<class CLight*>			m_vecLight;
	typedef vector<class CLight*>	LIGHTS;

public:
	virtual void Free() override;
};

END