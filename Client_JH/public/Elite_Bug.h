#pragma once
#include "Client_Defines.h"
#include "Monster.h"

BEGIN(Engine)
class CRenderer;
class CShader;
class CNavigation; 
END

BEGIN(Client)

class CElite_Bug final  :	public CMonster
{
	friend class CElite_Bug_State;

public:
	CElite_Bug(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CElite_Bug(const CElite_Bug& rhs);
	virtual ~CElite_Bug() = default;

public:
	virtual HRESULT			Initialize_Prototype() override;
	virtual HRESULT			Initialize_Clone(const wstring& wstrPrototypeTag, void* pArg) override;
	virtual void			Tick(_double TimeDelta) override;
	virtual void			Late_Tick(_double TimeDelta) override;
	virtual HRESULT			Render() override;

public:
	virtual void			Collider_Tick(_double TimeDelta) override;

	virtual void			Set_On_NaviMesh() override;
	virtual void Collision_Body(CBullet* pBullet) override;

private:
	HRESULT					SetUp_Components();
	HRESULT					SetUp_ShaderResources();
	HRESULT					Ready_UI();

public:
	static CElite_Bug*	Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject*	Clone(const wstring& wstrPrototypeTag, void* pArg = nullptr) override;
	virtual void			Free() override;
};



END
