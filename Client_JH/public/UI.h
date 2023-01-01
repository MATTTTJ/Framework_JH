#pragma once
#include "Client_Defines.h"
#include "GameObject.h"
#include "Weapon_State.h"

BEGIN(Engine)
	class CRenderer;
class CShader;
class CTexture;
class CVIBuffer_Rect;
END

BEGIN(Client)

class CUI abstract : public CGameObject
{
protected:
	CUI(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUI(const CUI& rhs);
	virtual ~CUI() = default;

protected:
	virtual HRESULT				Initialize_Prototype() override;
	virtual HRESULT				Initialize_Clone(const wstring& wstrPrototypeTag, void* pArg) override;
	virtual void				Tick(_double dTimeDelta) override;
	virtual void				Late_Tick(_double dTimeDelta) override;
	virtual HRESULT				Render() override;

protected:
	CRenderer*					m_pRendererCom = nullptr;
	CShader*					m_pShaderCom = nullptr;
	CTexture*					m_pTextureCom = nullptr;

	CTexture*					m_pOnTextureCom = nullptr;
	CTexture*					m_pOffTextureCom = nullptr;
	CTexture*					m_pWeaponTextureCom[CWeapon_State::WEAPONTYPE_END] = { nullptr };

	CVIBuffer_Rect*				m_pVIBufferCom = nullptr;

	CGameObject*				m_pOwner = nullptr;

protected:
	_float						m_fX, m_fY, m_fSizeX, m_fSizeY;
	_float4x4					m_ViewMatrix;
	_float4x4					m_ProjMatrix;

public:
	virtual CGameObject*		Clone(const wstring& wstrPrototypeTag, void* pArg = nullptr) PURE;
	virtual void				Free() override;
};

END