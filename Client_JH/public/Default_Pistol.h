#pragma once
#include "Client_Defines.h"
#include "Bullet.h"

BEGIN(Engine)
class CVIBuffer_Point_Instancing;

END
BEGIN(Client)
class CDefault_Pistol final : public CBullet
{
protected:
	CDefault_Pistol(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CDefault_Pistol(const CDefault_Pistol& rhs);
	virtual ~CDefault_Pistol() = default;

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

	_float4						m_vDir;
	_float4						m_vLook;

	_float4						m_vTest;

	_float4						m_vUp;
public:
	static CDefault_Pistol*		Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject*		Clone(const wstring& wstrPrototypeTag, void* pArg = nullptr) override;
	virtual void				Free() override;
};	
END