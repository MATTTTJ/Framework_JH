#pragma once
#include "Client_Defines.h"
#include "Monster.h"

BEGIN(Engine)
class CRenderer;
class CShader;
class CNavigation; 
END

BEGIN(Client)

class CHuman_Spear final  :	public CMonster
{
	// friend class CHuman_Spear_State;

public:
	CHuman_Spear(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CHuman_Spear(const CHuman_Spear& rhs);
	virtual ~CHuman_Spear() = default;

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
	static CHuman_Spear*	Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject*	Clone(const wstring& wstrPrototypeTag, void* pArg = nullptr) override;
	virtual void			Free() override;
};



END
