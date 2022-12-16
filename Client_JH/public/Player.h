#pragma once
#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CShader;
class CRenderer;
class CCollider;
class CModel;
class CNavigation;
END

BEGIN(Client)

class CPlayer final : public CGameObject
{
	enum COLLIDERTYPE { COLLIDER_AABB, COLLIDER_OBB, COLLIDER_SPHERE, COLLIDERTYPE_END };

private:
	CPlayer(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CPlayer(const CPlayer& rhs);
	virtual ~CPlayer() = default;

public:
	_matrix					Get_BoneMatrix(const string& strBoneName);
	_matrix					Get_OffsetMatrix(const string& strBoneName);
	_matrix					Get_PivotMatrix();
	_vector					Get_TransformState(CTransform::STATE eState);


public:
	virtual HRESULT			Initialize_Prototype() override;
	virtual HRESULT			Initialize_Clone(const wstring& wstrPrototypeTag, void* pArg) override;
	virtual void			Tick(_double dTimeDelta) override;
	virtual void			Late_Tick(_double TimeDelta) override;
	virtual HRESULT			Render() override;

private:
	CShader*				m_pShaderCom	= nullptr;
	CRenderer*				m_pRendererCom	= nullptr;
	CModel*					m_pModelCom		= nullptr;
	CCollider*				m_pColliderCom[COLLIDERTYPE_END] = { nullptr };

	CNavigation*			m_pNavigationCom = nullptr;

private:
	vector<CGameObject*>	m_vecPlayerParts;

private:
	HRESULT					SetUp_Components();
	HRESULT					SetUp_ShaderResources();

	HRESULT					Ready_Parts();

public:
	static	CPlayer*		Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject*	Clone(const wstring& wstrPrototypeTag, void* pArg = nullptr) override;
	virtual void			Free() override;
};

END