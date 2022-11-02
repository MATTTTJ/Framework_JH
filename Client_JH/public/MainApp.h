#pragma once
#include "Client_Defines.h"
#include "Base.h"

BEGIN(Engine)
class CGameInstance;
END

BEGIN(Client)

 class CMainApp  final : public CBase
{
private:
	CMainApp();
	virtual ~CMainApp() = default;

public:
	HRESULT	Initialize();
	void Tick(_double TimeDelta);
	HRESULT	Render();

private:
	CGameInstance*	m_pGameInstance = nullptr;

private:
	ID3D11Device*			m_pDevice = nullptr;
	ID3D11DeviceContext*	m_pContext = nullptr;

public:
	static CMainApp*	Create();
	virtual void Free() override;
};

END
