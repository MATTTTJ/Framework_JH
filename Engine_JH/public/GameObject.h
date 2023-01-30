#pragma once
#include "Transform.h"

BEGIN(Engine)

class ENGINE_DLL CGameObject abstract :	public CBase
{
public:
	typedef struct tagGameObjectDesc
	{
		CTransform::TRANSFORMDESC		TransformDesc;


		//체력, 데미지
		_int					m_iHP = 0;
		_int					m_iMaxHP = 0;

		_int					m_iShield = 0;
		_int					m_iMaxShield = 0;

		_uint					m_iDamage = 0;
		

		// For CountUI
		_float2					m_vTexSize = { 0.f, 0.f};
		_int					m_iNumCnt = 0;
		_int					m_iNumber = 0;
		_float4					m_vNumColor = {1.f, 1.f, 1.f, 1.f};
		_int					m_iCountType = 0;
		_float4					m_vBulletLook = { 0.f, 0.f, 0.f, 0.f }; 
	}GAMEOBJECTDESC;


public:
	_float Get_CamDistance() const
	{
		return m_fCamDistance;
	}
protected:
	CGameObject(ID3D11Device*	pDevice, ID3D11DeviceContext* pContext);
	CGameObject(const CGameObject& rhs);
	virtual ~CGameObject() = default;

public:
	const _bool				Check_Dead() { return m_bIsDead; }
	void					Set_Dead(_bool bDead) { m_bIsDead = bDead; }


	GAMEOBJECTDESC			Set_GameObjectDesc(GAMEOBJECTDESC Desc);

	const _bool&			Get_IsClone() { return m_bIsClone; }
	const _bool&			Get_HasModel() { return m_bHasModel; }
	const _float4x4&		Get_WorldFloat4x4() const { return m_pTransformCom->Get_WorldFloat4x4(); }
	const wstring&			Get_PrototypeGameObjectTag() { return m_wstrPrototypeGameObjectTag; }
	void					Set_WorldMatrix(_float4x4 WorldMatrix) { if (m_pTransformCom != nullptr)		m_pTransformCom->Set_WorldMatrix(WorldMatrix); }
	class CComponent*		Get_Component(const wstring& wstrComponentTag);
	class CComponent*		Find_Component(const wstring& pComopnentTag);
	CTransform*				Get_Transform() { return m_pTransformCom; }
	virtual HRESULT			Render_ShadowDepth() { return NOERROR; }
	virtual HRESULT			Render_OutLineFlag() { return NOERROR; }
public:
	virtual HRESULT			Initialize_Prototype();
	virtual HRESULT			Initialize_Clone(const wstring& wstrPrototypeTag, void* pArg);
	virtual void			Tick(_double dTimeDelta);
	virtual void			Late_Tick(_double dTimeDelta);
	virtual HRESULT			Render();

public: 
	void					Imgui_RenderComponentProperties();
	virtual void			Imgui_RenderProperty() {}
	virtual pair<_bool, _float3>		Picking_Mesh() { return pair<_bool, _float3>{false, _float3(0.f, 0.f, 0.f)}; }

protected:
	HRESULT					Add_Component(_uint iLevelIndex, const wstring& wstrPrototypeTag, const wstring& wstrComponentTag, class CComponent** ppOut, class CGameObject* pOwner, void* pArg = nullptr);
	void					Compute_CamDistance();
protected:
	ID3D11Device*			m_pDevice = nullptr;
	ID3D11DeviceContext*	m_pContext = nullptr;

protected:
	_bool					m_bHasModel = false; 
	_bool					m_bIsClone = false;
	_bool					m_bIsDead = false;
	_float					m_fCamDistance = { 0.0 };

	wstring					m_wstrPrototypeGameObjectTag = L"";

	map<const wstring, class CComponent*>	m_mapComponents;
	class CTransform*						m_pTransformCom = nullptr;

public:
	static const wstring	m_wstrTransformComTag;

public:
	virtual CGameObject*	Clone(const wstring& wstrPrototypeTag, void* pArg = nullptr) = 0;
	virtual void			Free();
};

END