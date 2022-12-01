#pragma once
#include "Client_Defines.h"
#include "Level.h"

BEGIN(Client)
class CLevel_MapTool :	public CLevel
{
public:
	CLevel_MapTool(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CLevel_MapTool() = default;

	virtual HRESULT Initialize() override;
	virtual void	Tick(_double TimeDelta) override;
	virtual void	Late_Tick(_double TimeDelta) override;
	virtual HRESULT Render() override;

private:
	HRESULT		Ready_Light();
	HRESULT		Ready_Layer_Terrain(const _tchar* pLayerTag);
	HRESULT		Ready_Layer_StaticMesh(const _tchar* pLayerTag);


	HRESULT		Ready_Layer_Camera(const _tchar* pLayerTag);


public:
	static CLevel_MapTool*	Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void			Free() override;
};
END
