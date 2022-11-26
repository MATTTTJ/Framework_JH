#pragma once
#include "Client_Defines.h"
#include "Level.h"

BEGIN(Client)
class CLevel_GamePlay :	public CLevel
{
private:
	CLevel_GamePlay(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CLevel_GamePlay() = default;

public:
	virtual HRESULT Initialize() override;
	virtual void Tick(_double TimeDelta) override;
	virtual void Late_Tick(_double TimeDelta) override;
	virtual HRESULT Render() override;

private:
	HRESULT		Ready_Light();
	HRESULT		Ready_Layer_BackGround(const _tchar* pLayerTag);
	HRESULT		Ready_Layer_Camera(const _tchar* pLayerTag);
	HRESULT		Ready_Layer_Player(const _tchar* pLayerTag);

public:
	static CLevel_GamePlay* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void Free() override;
};



END
