#pragma once
#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CShader;
class CRenderer;
class CModel;
END


BEGIN(Client)
class CStonePillar final :	public CGameObject
{
private:
	CStonePillar(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CStonePillar(const CStonePillar& rhs);
	virtual ~CStonePillar() = default;

public:
	virtual HRESULT			Initialize_Prototype() override;
	virtual HRESULT			Initialize_Clone(const wstring& wstrPrototypeTag, void* pArg) override;
	virtual void			Tick(_double TimeDelta) override;
	virtual void			Late_Tick(_double TimeDelta) override;
	virtual HRESULT			Render() override;
	void					Ready_DangerEffect();
	// virtual pair<_bool, _float3> Picking_Mesh() override;
	_bool					Collision_To_Golem();
	_bool					Collision_To_MagicStone();
	_bool					Collision_To_Bullet();
private:
	CShader*				m_pShaderCom = nullptr;
	CRenderer*				m_pRendererCom = nullptr;
	CModel*					m_pModelCom = nullptr;
	CCollider*				m_pStonePillarColliderCom = nullptr;
	_float					m_fAddHeight = 0.f;
	_float					m_fCurHeight = 0.f;
	_float					m_fMaxHeight = -7.5f;
	CGameObject::GAMEOBJECTDESC	m_tStonePillarDesc;

	_float					m_fCurGrowTime = 0.f;
	_float					m_fGrowTime = 2.f;

	// CNavigation*			m_pNavigationCom = nullptr;
	// _bool					m_bNormalTexOn = false;

private:
	HRESULT					SetUp_Components();
	HRESULT					SetUp_ShaderResources();

public:
	static	CStonePillar*		Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject*	Clone(const wstring& wstrPrototypeTag, void* pArg = nullptr) override;
	virtual void			Free() override;
};

END