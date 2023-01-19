#pragma once
#include "Client_Defines.h"
#include "Bullet.h"

BEGIN(Client)

class CBlade :
	public CBullet
{
public:
	CBlade(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CBlade(const CBlade& rhs);
	virtual ~CBlade() = default;


public:
	CCollider*					Get_ColliderPtr() { return m_pBulletColliderCom; }

public:
	virtual HRESULT				Initialize_Prototype() override;
	virtual HRESULT				Initialize_Clone(const wstring& wstrPrototypeTag, void* pArg) override;
	virtual void				Tick(_double dTimeDelta) override;
	virtual void				Late_Tick(_double dTimeDelta) override;
	virtual HRESULT				Render() override;

private:
	HRESULT						SetUp_Component();
	HRESULT						SetUp_ShaderResources();
private:

public:
	static	CBlade*				Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject*		Clone(const wstring& wstrPrototypeTag, void* pArg = nullptr) override;
	virtual void				Free() override;

};

END