#pragma once
#include "Client_Defines.h"
#include "Level.h"

BEGIN(Client)
class CLevel_MapEditor :	public CLevel
{
public:
	CLevel_MapEditor(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CLevel_MapEditor() = default;

	virtual HRESULT Initialize() override;
	virtual void	Tick(_double TimeDelta) override;
	virtual void	Late_Tick(_double TimeDelta) override;
	virtual HRESULT Render() override;

private:
	HRESULT		Ready_Light();
	HRESULT		Ready_Layer_Background(const wstring wstrLayerTag);
	HRESULT		Ready_Layer_StaticMesh(const wstring wstrLayerTag);


	HRESULT		Ready_Layer_Camera(const wstring wstrLayerTag);


public:
	static CLevel_MapEditor*	Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void			Free() override;
};
END
