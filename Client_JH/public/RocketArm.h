#pragma once
#include "Client_Defines.h"
#include "Bullet.h"

BEGIN(Client)
class CRocketArm final : public CBullet
{
public:
	enum ARMTYPE { ARMTYPE_LEFT, ARMTYPE_RIGHT, ARMTYPE_END };

protected:
	CRocketArm(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CRocketArm(const CRocketArm& rhs);
	virtual ~CRocketArm() = default;

public:
	CCollider*					Get_ColliderPtr() { return m_pBulletColliderCom; }

public:
	virtual HRESULT				Initialize_Prototype() override;
	virtual HRESULT				Initialize_Clone(const wstring& wstrPrototypeTag, void* pArg) override;
	virtual void				Tick(_double dTimeDelta) override;
	virtual void				Late_Tick(_double dTimeDelta) override;
	virtual HRESULT				Render() override;
	void						Ready_Danger();
private:
	HRESULT						SetUp_Component();
	HRESULT						SetUp_ShaderResources();

private:
	CModel*						m_pArmModel[ARMTYPE_END] = { nullptr };
	_uint						m_iModelType = 0;
	CCollider*					m_pArmCollider = nullptr;
	_float4						m_vTargetPos;
public:
	static	CRocketArm*			Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject*		Clone(const wstring& wstrPrototypeTag, void* pArg = nullptr) override;
	virtual void				Free() override;
};

END