#pragma once
#include "Base.h"

BEGIN(Engine)

class CLight_Manager final : public CBase
{
	DECLARE_SINGLETON(CLight_Manager)
public:
	CLight_Manager();
	virtual ~CLight_Manager() = default;

public:
	const LIGHTDESC*		Get_LightDesc(_uint iIndex);

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