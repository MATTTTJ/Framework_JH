#pragma once
#include "Client_Defines.h"
#include "Monster.h"

BEGIN(Engine)
class CRenderer;
class CShader;
class CNavigation; 
END

BEGIN(Client)

class CHuman_Granade final  :	public CMonster
{
	// friend class CHuman_Granade_State;

public:
	CHuman_Granade(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CHuman_Granade(const CHuman_Granade& rhs);
	virtual ~CHuman_Granade() = default;

public:
	virtual HRESULT			Initialize_Prototype() override;
	virtual HRESULT			Initialize_Clone(const wstring& wstrPrototypeTag, void* pArg) override;
	virtual void			Tick(_double TimeDelta) override;
	virtual void			Late_Tick(_double TimeDelta) override;
	virtual HRESULT			Render() override;

public:
	virtual void			Collider_Tick(_double TimeDelta) override;
	virtual void			Collider_Late_Tick(_double TimeDelta) override;

	virtual void			Set_On_NaviMesh() override;
	virtual void			Collision_Event(CPlayer* pPlayer) override;

private:
	HRESULT					SetUp_Components();
	HRESULT					SetUp_ShaderResources();
	HRESULT					Ready_UI();

public:
	static CHuman_Granade*	Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject*	Clone(const wstring& wstrPrototypeTag, void* pArg = nullptr) override;
	virtual void			Free() override;
};



END
