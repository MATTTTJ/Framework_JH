#pragma once

#include "Client_Defines.h"
#include "Base.h"

BEGIN(Client)

class CLoader final : public CBase
{
private:
	CLoader(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CLoader() = default;

public:
	LEVEL	Get_NextLevelID() const {
		return m_eNextLevelID;
	}

	const wstring&	Get_LoadingText() const {
		return m_wstrLoadingText;
	}

	const _bool&	IsFinished() const {
		return m_bIsLoadingFinished;
	}

	CRITICAL_SECTION	Get_Critical_Section() const {
		return m_Critical_Section;
	}

public:
	HRESULT	Initialize(LEVEL eNextLevelID);
	HRESULT	Loading_For_Logo();
	HRESULT	Loading_For_GamePlay();
	HRESULT	Loading_For_MapTool();
private:
	ID3D11Device*			m_pDevice = nullptr;
	ID3D11DeviceContext*	m_pContext = nullptr;
	CRITICAL_SECTION		m_Critical_Section;
private:
	HANDLE		m_hThread;
	LEVEL		m_eNextLevelID = LEVEL_END;
	_bool		m_bIsLoadingFinished = false;
	wstring		m_wstrLoadingText = L"";

public:
	static CLoader* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, LEVEL eNextLevelID);
	virtual void Free() override;
};

END

