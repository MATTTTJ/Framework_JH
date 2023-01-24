#pragma once
#include "Client_Defines.h"
#include "Bullet.h"



BEGIN(Client)
class CLaser final :	public CBullet
{
public:
	CLaser(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CLaser(const CLaser& rhs);
	virtual ~CLaser() = default;

public:
	CCollider*						Get_ColliderPtr() { return m_pBulletColliderCom; }

public:
	void						Set_Render(_bool TrueIsRender) { m_bRender = TrueIsRender; }
	virtual HRESULT				Initialize_Prototype() override;
	virtual HRESULT				Initialize_Clone(const wstring& wstrPrototypeTag, void* pArg) override;
	virtual void				Tick(_double dTimeDelta) override;
	virtual void				Late_Tick(_double dTimeDelta) override;
	virtual HRESULT				Render() override;
	// _bool						Collision_Body();
	void						Set_Laser_Alpha_Zero(_bool	TrueIsAhpha1) {m_bLaserAlpha = TrueIsAhpha1;}

	void						Set_Look(_float4 vLook);
	void						Set_Up(_float4 vUp);
	void						Set_PSize(_float fSize);
	void						Set_Pos(_float4 BulletPos)
	{
		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(BulletPos.x, BulletPos.y, BulletPos.z, 1.f));
		
		// m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(BulletPos.x, 1.3f, BulletPos.z, 1.f));
	}

private:
	HRESULT						SetUp_Component();
	HRESULT						SetUp_ShaderResources();

private:
	_float4x4					m_matPivot;
	_float						m_fStrong;
	_float4x4					m_SocketMatrix;
	_bool						m_bRender = false;
	_float4						m_vDir;
	_float4						m_vLook;
	_bool						m_bLaserAlpha = false;
	_float						m_fLaserAlpha = 0.f;
	_float4						m_vTest;

	_float4						m_vUp;
public:
	static CLaser*		Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject*		Clone(const wstring& wstrPrototypeTag, void* pArg = nullptr) override;
	virtual void				Free() override;
};

END