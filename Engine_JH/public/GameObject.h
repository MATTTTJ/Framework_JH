#pragma once
#include "Transform.h"

BEGIN(Engine)

class ENGINE_DLL CGameObject abstract :	public CBase
{
public:
	typedef struct tagGameObjectDesc
	{
		CTransform::TRANSFORMDESC		TransformDesc;


		// For CountUI
		_int					m_iNumCnt;
		_int					m_iNumber;
	}GAMEOBJECTDESC;

protected:
	CGameObject(ID3D11Device*	pDevice, ID3D11DeviceContext* pContext);
	CGameObject(const CGameObject& rhs);
	virtual ~CGameObject() = default;

public:
	const _bool&			Get_HasModel() { return m_bHasModel; }
	const _float4x4&		Get_WorldFloat4x4() const { return m_pTransformCom->Get_WorldFloat4x4(); }
	const wstring&			Get_PrototypeGameObjectTag() { return m_wstrPrototypeGameObjectTag; }
	void					Set_WorldMatrix(_float4x4 WorldMatrix) { if (m_pTransformCom != nullptr)		m_pTransformCom->Set_WorldMatrix(WorldMatrix); }
	class CComponent*		Get_Component(const wstring& wstrComponentTag);
	class CComponent*		Find_Component(const wstring& pComopnentTag);
	CTransform*				Get_Transform() { return m_pTransformCom; }

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

protected:
	ID3D11Device*			m_pDevice = nullptr;
	ID3D11DeviceContext*	m_pContext = nullptr;

protected:
	_bool					m_bHasModel = false; 
	_bool					m_bIsClone = false;
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