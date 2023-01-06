#pragma once
#include "Client_Defines.h"
#include "Camera.h"

BEGIN(Engine)
class CCollider;
END

BEGIN(Client)

class CStatic_Camera final :	public CCamera
{
private:
	CStatic_Camera(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CStatic_Camera(const CStatic_Camera& rhs);
	virtual ~CStatic_Camera() = default;

public:
	HRESULT					Initialize_Prototype() override;
	HRESULT					Initialize_Clone(const wstring& wstrPrototypeTag, void* pArg) override;
	void					Tick(_double dTimeDelta) override;
	void					Late_Tick(_double dTimeDelta) override;
	HRESULT					Render() override;
	void					Camera_Update(_double dTimeDelta, _float4x4 PlayerWorld);
	void					Mouse_Fix();
	void					Set_FixControl() { m_bFix = !m_bFix; }
	void					Set_CamPos(_float4 vPos, _float4 vLook);
	_float4*				Get_CamLook(void) { return &m_vCameraLook; }
private:
	CCollider*		m_pColliderCom = nullptr;
	CGameObject*			m_pOwner = nullptr;
	_bool					m_bFix = false;
private:
	HRESULT					SetUp_Component();
	_float4					m_vCameraLook = { 0.f, 0.f, 0.f, -1.f };
	_float					m_fCamHeight;
public:
	static CStatic_Camera*	Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject*	Clone(const wstring& wstrPrototypeTag, void* pArg = nullptr) override;
	virtual void			Free() override;
};

END