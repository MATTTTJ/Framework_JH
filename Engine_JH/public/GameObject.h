#pragma once
#include "Transform.h"

BEGIN(Engine)

class ENGINE_DLL CGameObject abstract :	public CBase
{
public:
	typedef struct tagGameObjectDesc
	{
		CTransform::TRANSFORMDESC		TransformDesc;
	}GAMEOBJECTDESC;

protected:
	CGameObject(ID3D11Device*	pDevice, ID3D11DeviceContext* pContext);
	CGameObject(const CGameObject& rhs);
	virtual ~CGameObject() = default;

public:
	const _bool&		Get_HasModel() { return m_bHasModel; }
	const _float4x4&	Get_WorldFloat4x4() const { return m_pTransformCom->Get_WorldFloat4x4(); }
	const wstring&		Get_PrototypeGameObjectTag() { return m_wstrPrototypeGameObjectTag; }
	public:
	static const wstring		m_wstrTransformComTag;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize_Clone(const wstring& wstrPrototypeTag, void* pArg);
	virtual void	Tick(_double dTimeDelta);
	virtual void	Late_Tick(_double dTimeDelta);
	virtual HRESULT Render();

public: /* imgui */
		// �� ������Ʈ�� ������ �ִ� component�� Imgui_RenderProtpery�Լ��� �����ϴ� �Լ�.
	void Imgui_RenderComponentProperties();

	// �� ������Ʈ���� ������ �����͸� imgui�� �ۼ��Ѵ�.
	virtual void Imgui_RenderProperty() {}


protected:
	ID3D11Device*			m_pDevice = nullptr;
	ID3D11DeviceContext*	m_pContext = nullptr;

protected:
	/* ��ü���� ����ؾ� �� ������Ʈ���� �����Ѵ�.*/
	map<const wstring, class CComponent*>			m_Components;

	class CTransform*								m_pTransformCom = nullptr;

	_bool					m_bHasModel = false; 
	_bool					m_bIsClone = false;
	wstring					m_wstrPrototypeGameObjectTag = L"";

protected:
	HRESULT					Add_Component(_uint iLevelIndex, const wstring& wstrPrototypeTag, const wstring& wstrComponentTag, class CComponent** ppOut, void* pArg = nullptr);
	class CComponent*		Find_Component(const wstring& pComopnentTag);

public:
	virtual CGameObject* Clone(const wstring& wstrPrototypeTag, void* pArg = nullptr) = 0;
	virtual void Free() override;
};

END