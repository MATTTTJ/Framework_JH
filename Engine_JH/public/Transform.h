#pragma once
#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CTransform final : public CComponent
{
public:
	/* STATE_TRANSLATION : (원점 또는 부모) 기준으로부터 얼마나 떨어져있어. */
	/* Position : 절대적인 월드공간에서의 위치. */
	enum STATE { STATE_RIGHT, STATE_UP, STATE_LOOK, STATE_TRANSLATION, STATE_END };

	typedef struct tagTransformDesc
	{
		float		fSpeedPerSec;
		float		fRotationPerSec;
	}TRANSFORMDESC;

protected:
	CTransform(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CTransform(const CTransform& rhs);
	virtual ~CTransform() = default;

public:
	const _matrix		Get_WorldMatrix_Inverse() const {	return XMMatrixInverse(nullptr, XMLoadFloat4x4(&m_WorldMatrix)); }
	const _matrix		Get_WorldMatrix() const {	return XMLoadFloat4x4(&m_WorldMatrix);	}
	const _float4x4&	Get_WorldFloat4x4() const {	return m_WorldMatrix; }
	_vector&			Get_State(STATE eState) const {	return XMLoadFloat4x4(&m_WorldMatrix).r[eState]; }
	_float3				Get_Scaled() const {
		return _float3(XMVectorGetX(XMVector3Length(Get_State(STATE_RIGHT))),
			XMVectorGetX(XMVector3Length(Get_State(STATE_UP))),
			XMVectorGetX(XMVector3Length(Get_State(STATE_LOOK))));
	}
	void				Set_WorldMatrix(_float4x4 WorldMatrix) { m_WorldMatrix = WorldMatrix; }
	void				Set_State(STATE eState, _fvector vState) {
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
	void Go_Straight(_double TimeDelta);
	void Go_Backward(_double TimeDelta);
	void Go_Left(_double TimeDelta);
	void Go_Right(_double TimeDelta);
	void Turn(_fvector vAxis, _double TimeDelta); /* Dynamic */
	void Rotation(_fvector vAxis, _float fRadian); /* Static */
	void LookAt(_fvector vTargetPos);
	void Chase(_fvector vTargetPos, _double TimeDelta, _float fLimit = 0.1f);

	void Speed_Up(_bool bKeyState);
public:

private:
	_float4x4				m_WorldMatrix;
	TRANSFORMDESC			m_TransformDesc;
	_double					m_dInitSpeed = 0.0;

public:
	static CTransform* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CComponent* Clone(class CGameObject* pOwner, void* pArg = nullptr) override;
	virtual void Free() override;

};


END