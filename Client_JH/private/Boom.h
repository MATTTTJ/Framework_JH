#pragma once
#include "Client_Defines.h"
#include "Bullet.h"


BEGIN(Client)

class CBoom final :	public CBullet
{
public:
	CBoom(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CBoom(const CBoom& rhs);
	virtual ~CBoom() = default;

public:
	CCollider*					Get_ColliderPtr() { return m_pBulletColliderCom; }

public:
	virtual HRESULT				Initialize_Prototype() override;
	virtual HRESULT				Initialize_Clone(const wstring& wstrPrototypeTag, void* pArg) override;
	virtual void				Tick(_double dTimeDelta) override;
	virtual void				Late_Tick(_double dTimeDelta) override;
	virtual HRESULT				Render() override;
	// _bool						Collision_Body();

	// TODO :: 폭발 터지는 로직

private:
	HRESULT						SetUp_Component();
	HRESULT						SetUp_ShaderResources();

private:

private:
	_float4x4					m_matPivot;
	_float						m_fStrong;
	_float4x4					m_SocketMatrix;
	_bool						m_bFallTime = false;
	_float						m_fJumpPower;
	_float4						m_fDeadHeight = {0.f, 0.f, 0.f, 0.f};

	_float						m_fCurBoomTime = 0.f;
	_float						m_fBoomTime = 3.f;

public:
	static	CBoom*				Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject*		Clone(const wstring& wstrPrototypeTag, void* pArg = nullptr) override;
	virtual void				Free() override;
};

END