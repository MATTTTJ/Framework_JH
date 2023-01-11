#pragma once
#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CTransform final : public CComponent
{
public:
	/* STATE_TRANSLATION : (원점 또는 부모) 기준으로부터 얼마나 떨어져있어. */
	/* Position : 절대적인 월드공간에서의 위치. */
	enum STATE { STATE_RIGHT, STATE_UP, STATE_LOOK, STATE_TRANSLATION, STATE_END };

	enum DIRECTION { DIR_W, DIR_A, DIR_S, DIR_D, DIR_WA, DIR_WD, DIR_SA, DIR_SD, DIR_END };

	enum TRANSTYPE { TRANS_PLAYER, TRANS_BULLET, TRANS_MONSTER , TRANS_FIX,  TRANSTYPE_END };

	typedef struct tagTransformDesc
	{
		_float		fSpeedPerSec = 5.f;
		_float		fRotationPerSec = XMConvertToRadians(0.f);

		_float3		vInitPos = { 0.f, 0.f, 0.f };
	}TRANSFORMDESC;

protected:
	CTransform(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CTransform(const CTransform& rhs);
	virtual ~CTransform() = default;

public:
	void				FinalUpdate();	/* For UI .*/

	void				SetParent(CTransform* pParent)	{ m_pParent = pParent; }
	const _matrix		Get_WorldMatrix_Inverse() const {	return XMMatrixInverse(nullptr, XMLoadFloat4x4(&m_WorldMatrix)); }
	const _matrix		Get_WorldMatrix() const {	return XMLoadFloat4x4(&m_WorldMatrix);	}

	_matrix				Get_UIWorldMatrix()
	{
		if (nullptr == m_pParent)		
			return XMLoadFloat4x4(&m_WorldMatrix);

	return XMLoadFloat4x4(&m_WorldWithParentMatrix);
	}

	const _float4x4&	Get_WorldFloat4x4() const {	return m_WorldMatrix; }
	_vector&			Get_State(STATE eState) const {	return XMLoadFloat4x4(&m_WorldMatrix).r[eState]; }
	_float3				Get_Scaled() const {
		return _float3(XMVectorGetX(XMVector3Length(Get_State(STATE_RIGHT))),
			XMVectorGetX(XMVector3Length(Get_State(STATE_UP))),
			XMVectorGetX(XMVector3Length(Get_State(STATE_LOOK))));
	}
	void				Set_WorldMatrix(_float4x4 WorldMatrix) { m_WorldMatrix = WorldMatrix; }
	void				Set_State(STATE eState, _float4 vState) {
		_float4		vTmp;
		XMStoreFloat4(&vTmp, vState);
		memcpy(&m_WorldMatrix.m[eState][0], &vTmp, sizeof vTmp);
	}
	void Set_Scaled(STATE eState, _float fScale); /* fScale값으로 길이를 변형한다. */
	void Set_Scaled(_float3 vScale); /* fScale값으로 길이를 변형한다. */
	void Scaling(STATE eState, _float fScale); /* fScale배수로 늘린다. */


public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize_Clone(class CGameObject* pOwner, void* pArg) override;
	virtual void	Imgui_RenderProperty() override;

	HRESULT Bind_ShaderResource(class CShader* pShaderCom, const wstring& pConstantName);

public:
	// void Go_Straight(_double TimeDelta, class CNavigation* pNaviCom = nullptr);
	void Go_Straight(_double TimeDelta, TRANSTYPE eType = TRANSTYPE_END, class CNavigation* pNaviCom = nullptr);
	void Go_Backward(_double TimeDelta,TRANSTYPE eType = TRANSTYPE_END, class CNavigation* pNaviCom = nullptr);
	void Go_Left(_double TimeDelta, TRANSTYPE eType = TRANSTYPE_END, class CNavigation* pNaviCom = nullptr);
	void Go_Right(_double TimeDelta, TRANSTYPE eType = TRANSTYPE_END, class CNavigation* pNaviCom = nullptr);
	void Turn(_fvector vAxis, _double TimeDelta); /* Dynamic */
	void Rotation(_fvector vAxis, _float fRadian); /* Static */
	void RotateToTarget(const _vector& vTargetPos);
	void LookAt(_fvector vTargetPos);
	void LookAt_Monster(_fvector vTargetPos, _double TimeDelta, _float fLimitRange);
	void Chase(_fvector vTargetPos, _double TimeDelta, _float fLimit = 0.1f);

	void				Jump(_double dTimeDelta, _float& fGravity, _float& fCurJumpSpeed);
	void				Dash(_double dTimeDelta, _float& fFriction, _float& fCurDashTickCount, _fmatrix matCamWorld, DIRECTION eDir);

	void Speed_Up(_bool bKeyState);
public:

private:
	_float4x4				m_WorldMatrix;
	TRANSFORMDESC			m_TransformDesc;
	_double					m_dInitSpeed = 0.0;
	CTransform*				m_pParent = nullptr;		// 부모 트랜스폼
	_float4x4				m_WorldWithParentMatrix;	// 자식 * 부모 행렬

	TRANSTYPE				m_eType = TRANSTYPE_END;
public:
	static CTransform* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CComponent* Clone(class CGameObject* pOwner, void* pArg = nullptr) override;
	virtual void Free() override;

};


END