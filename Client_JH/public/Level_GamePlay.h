#pragma once
#include "Client_Defines.h"
#include "Level.h"
BEGIN(Engine)
class CGameObject;
class CVIBuffer_Terrain;
END

BEGIN(Client)
class CLevel_GamePlay :	public CLevel
{
private:
	CLevel_GamePlay(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CLevel_GamePlay() = default;

public:
	virtual HRESULT Initialize() override;
	virtual void	Tick(_double TimeDelta) override;
	virtual void	Late_Tick(_double TimeDelta) override;
	virtual HRESULT Render() override;

private:
	HRESULT			Ready_Layer_Env(const wstring wstrLayerTag);

	HRESULT			Ready_Light();
	HRESULT			Ready_Layer_BackGround(const wstring wstrLayerTag);
	HRESULT			Ready_Layer_Camera(const wstring wstrLayerTag);
	HRESULT			Ready_Layer_Monster(const wstring wstrLayerTag);
	HRESULT			Ready_Layer_Player(const wstring wstrLayerTag);
	HRESULT			Ready_Layer_Effect(const wstring wstrLayerTag);
	HRESULT			Ready_Layer_UI(const wstring wstrLayerTag);
	HRESULT			Ready_Layer_BossMonster(const wstring wstrLayerTag);
	HRESULT			Ready_Layer_Trigger(const wstring wstrLayerTag);
private:
	// For Lobby
	HRESULT			Ready_Layer_LaiHome(const wstring wstrLayerTag);
	HRESULT			Ready_Layer_Lobby(const wstring wstrLayerTag);
	HRESULT			Ready_Layer_LobbyButton(const wstring wstrLayerTag);


private:
	vector<CGameObject*> m_vecLobbyObject;

	vector<CGameObject*>	m_vecButton;

	_bool					m_bButton = false;
public:
	static CLevel_GamePlay* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void Free() override;
};



END
