#pragma once
#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CShader;
class CRenderer;
class CCollider;
class CTexture;
class CShader;
class CVIBuffer_Rect;
END

BEGIN(Client)
class CTrigger final :	public CGameObject
{
public:
	enum TRIGGERTYPE
	{
		TRIGGER_A_TO_KNIGHT,
		TRIGGER_KNIGHT_TO_B,
		TRIGGER_B_TO_BUG,
		TRIGGER_BUG_TO_C,
		TRIGGER_C_TO_BOSS,
		TRIGGERTYPE_END
	};

	typedef struct tagTriggerOption
	{
		TRIGGERTYPE		m_eType = TRIGGERTYPE_END;
	}TRIGGERDESC;

private:
	CTrigger(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CTrigger(const CTrigger& rhs);
	virtual ~CTrigger() = default;

public:
	void					Set_Owner(CGameObject* pObject)
	{
		m_pPlayer = (CPlayer*)pObject;
	}

	virtual HRESULT			Initialize_Prototype() override;
	virtual HRESULT			Initialize_Clone(const wstring& wstrPrototypeTag, void* pArg) override;
	virtual void			Tick(_double TimeDelta) override;
	virtual void			Late_Tick(_double TimeDelta) override;
	virtual HRESULT			Render() override;

private: // For Portal
	HRESULT					Room_A_To_Knight();
	HRESULT					Knight_To_Room_B();
	HRESULT					Room_B_To_Bug();
	HRESULT					Bug_To_Room_C();
	HRESULT					Room_C_To_Boss(); 

private: // For Spawn Monster

private:
	class CPlayer*			m_pPlayer = nullptr;
	CRenderer*				m_pRendererCom = nullptr;
	CCollider*				m_pTriggerCollCom = nullptr;
	CShader*				m_pShaderCom = nullptr;
	CTexture*				m_pTextureCom = nullptr;
	CTexture*				m_pDistortionTexCom = nullptr;
	CTexture*				m_pCircleTexCom = nullptr;

	CVIBuffer_Rect*			m_pVIBufferCom = nullptr;
	TRIGGERDESC				m_tTriggerOption;

	_float					m_fTime = 0.f;
	_uint					m_iTextureIndex = 0;
	_bool					m_bSpawnTrigger = false;
private:
	HRESULT					SetUp_Components();
	HRESULT					SetUp_ShaderResources();

public:
	static	CTrigger*		Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject*	Clone(const wstring& wstrPrototypeTag, void* pArg = nullptr) override;
	virtual void			Free() override;
};

END