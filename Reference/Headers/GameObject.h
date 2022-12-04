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

	public:
	static const _tchar*		m_pTransformComTag;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize_Clone(void* pArg);
	virtual void	Tick(_double TimeDelta);
	virtual void	Late_Tick(_double TimeDelta);
	virtual HRESULT Render();

public: /* imgui */
		// 이 오브젝트가 가지고 있는 component의 Imgui_RenderProtpery함수를 실행하는 함수.
	void Imgui_RenderComponentProperties();

	// 이 오브젝트에서 보여줄 데이터를 imgui로 작성한다.
	virtual void Imgui_RenderProperty() {}


protected:
	ID3D11Device*			m_pDevice = nullptr;
	ID3D11DeviceContext*	m_pContext = nullptr;

protected:
	/* 객체들이 사용해야 할 컴포넌트들을 보관한다.*/
	map<const wstring, class CComponent*>			m_Components;

	class CTransform*								m_pTransformCom = nullptr;

	_bool					m_bHasModel = false; 

protected:
	HRESULT					Add_Component(_uint iLevelIndex, const wstring& pPrototypeTag, const wstring& pComponentTag, class CComponent** ppOut, void* pArg = nullptr);
	class CComponent*		Find_Component(const wstring& pComopnentTag);

public:
	virtual CGameObject* Clone(void* pArg = nullptr) = 0;
	virtual void Free() override;
};

END