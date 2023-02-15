#pragma once
#include "Client_Defines.h"
#include "Level.h"

BEGIN(Engine)
class CShader;
class CGameObject;
END

BEGIN(Client)

class CLevel_Logo :	public CLevel
{
private:
	CLevel_Logo(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CLevel_Logo() = default;

public:
	virtual HRESULT Initialize() override;
	virtual void	Tick(_double TimeDelta) override;
	virtual void	Late_Tick(_double TimeDelta) override;
	virtual HRESULT Render() override;


private:
	HRESULT			Ready_Layer_BackGround(const wstring& wstrLayerTag);
	// HRESULT			Ready_Layer_Lai_Home(const wstring& wstrLayerTag);
	// HRESULT			Ready_Layer_Camera(const wstring& wstrLayerTag);
	// HRESULT			Ready_Layer_Light();


	HRESULT			SetUp_Component();

private:
	_float				m_fCurLogoTime = 0.f;
	_float				m_fLogoTime = 3.f;
	_bool				m_bLevelChange = false;

	vector<CGameObject*>	m_vecLogo;
public:
	static CLevel_Logo*	Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void Free() override;
};

END