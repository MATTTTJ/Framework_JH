#pragma once
#include "Client_Defines.h"
#include "Bullet.h"


BEGIN(Client)
class CFlame_Bullet final : public CBullet
{
protected:
	CFlame_Bullet(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CFlame_Bullet(const CFlame_Bullet& rhs);
	virtual ~CFlame_Bullet() = default;

public:
	CCollider*						Get_ColliderPtr() { return m_pBulletColliderCom; }

public:
	virtual HRESULT				Initialize_Prototype() override;
	virtual HRESULT				Initialize_Clone(const wstring& wstrPrototypeTag, void* pArg) override;
	virtual void				Tick(_double dTimeDelta) override;
	virtual void				Late_Tick(_double dTimeDelta) override;
	virtual HRESULT				Render() override;
	// _bool						Collision_Body();
	
private:
	HRESULT						SetUp_Component();
	HRESULT						SetUp_ShaderResources();

private:
	_float4x4					m_matPivot;
	_float						m_fStrong;
	_float4x4					m_SocketMatrix;
	CTexture*					m_pTestTexture = nullptr;
	_float4						m_vDir;
	_float4						m_vLook;

	_float4						m_vTest;

	_float4						m_vUp;
public:
	static CFlame_Bullet*		Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject*		Clone(const wstring& wstrPrototypeTag, void* pArg = nullptr) override;
	virtual void				Free() override;
};	
END