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
	static const _tchar*		m_pTransformComTag;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize_Clone(void* pArg);
	virtual void	Tick(_double TimeDelta);
	virtual void	Late_Tick(_double TimeDelta);
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
	map<const _tchar*, class CComponent*>			m_Components;

	class CTransform*								m_pTransformCom = nullptr;

protected:
	HRESULT					Add_Component(_uint iLevelIndex, const _tchar* pPrototypeTag, const _tchar* pComponentTag, class CComponent** ppOut, void* pArg = nullptr);
	class CComponent*		Find_Component(const _tchar* pComopnentTag);

public:
	virtual CGameObject* Clone(void* pArg = nullptr) = 0;
	virtual void Free() override;
};

END