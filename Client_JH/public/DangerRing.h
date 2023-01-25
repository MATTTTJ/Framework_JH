#pragma once
#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CShader;
class CTexture;
class CRenderer;
class CVIBuffer_Rect;
END

BEGIN(Client)
class CDangerRing final : public CGameObject
{
public:
	typedef struct tagRingDesc
	{
		enum RINGTYPE { RINGTYPE_PILLAR, RINGTYPE_ROCKETARM, RINGTYPE_END};
		RINGTYPE	m_eType;
		_float		m_fMinSize;
		_float		m_fMaxSize;

		GAMEOBJECTDESC	GameObjectDesc;
	}RINGDESC;

private:
	CDangerRing(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CDangerRing(const CDangerRing& rhs);
	virtual ~CDangerRing() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize_Clone(const wstring& wstrPrototypeTag, void* pArg = nullptr) override;
	virtual void Tick(_double TimeDelta) override;
	virtual void Late_Tick(_double TimeDelta) override;
	virtual HRESULT Render() override;

private:
	CShader*				m_pShaderCom = nullptr;
	CRenderer*				m_pRendererCom = nullptr;
	CTexture*				m_pTextureCom = nullptr;
	CVIBuffer_Rect*			m_pVIBufferCom = nullptr;
	// CTexture*				m_pTextureGlowCom = nullptr;


private:
	_float					m_fCurLifeTime = 0.f;
	_float					m_fLifeTime = 1.5f;
	_float					m_fCurSize = 1.f;
	_float					m_fMaxSize = 7.f;

	_bool					m_bBaseRing = false;
private:
	HRESULT SetUp_Components();
	HRESULT SetUp_ShaderResources();

private:
	RINGDESC		m_tRingDesc;

public:
	static CDangerRing* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(const wstring& wstrPrototypeTag, void* pArg = nullptr) override;
	virtual void Free() override;
};

END