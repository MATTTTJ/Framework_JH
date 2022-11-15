#pragma once
#include "Component.h"

// 객체의 월드 스페이스 상에서의 상태를 저장하고 소유하고 있음
// 상태변환을 위한 인터페이스를 제공한다. (Go_Straight..)

BEGIN(Engine)

class ENGINE_DLL CTransform final : public CComponent
{
public:
	// State_Translationt 과 Position의 차이점
	// State_Translationt : (원점 또는 부모) 기준으로 얼마나 떨어져 있는지
	// Position : 월드공간에서의 절대적인 위치

	enum STATE { STATE_RIGHT, STATE_UP, STATE_LOOK, STATE_TRANSLATION, STATE_END};

	typedef struct tagTransformDesc
	{
		float fSpeedPerSec;
		float fRotationPerSec;
	}TRANSFORMDESC;

protected:
	CTransform(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CTransform(const CTransform& rhs);
	virtual ~CTransform() = default;

public:
	_vector	Get_State(STATE eState) const
	{
		return XMLoadFloat4x4(&m_WorldMatrix).r[eState];
	}

	_float3	Get_Scaled() const		// 객체의 현재 스케일값을 float값으로 나누어 뽑아 쓸 수 있게 
	{
		return _float3(XMVectorGetX(XMVector3Length(Get_State(STATE_RIGHT))),	// 각 행의 길이(크기)는 스케일 값
			XMVectorGetX(XMVector3Length(Get_State(STATE_UP))),
			XMVectorGetX(XMVector3Length(Get_State(STATE_LOOK))));
	}

	void	Set_State(STATE eState, _fvector vState)
	{
		_float4			vTmp;
		XMStoreFloat4(&vTmp, vState);
		memcpy(&m_WorldMatrix.m[eState][0], &vTmp, sizeof(vTmp));
	}

	void	Set_Scaled(STATE eState, _float fScale); // fScale값으로 길이를 변형한다. 
	void	Set_Scaled(_float3 vScale);	// fScale값으로 길이를 변형한다. 
	void	Scaling(STATE eState, _float fScale); // 현재 크기를 fScale 배수로 늘린다. 



public:
	virtual HRESULT			Initialize_Prototype();
	virtual HRESULT			Initialize_Clone(void* pArg);

public:
	void Go_Straight(_double TimeDelta);
	void Go_Backward(_double TimeDelta);
	void Go_Left(_double TimeDelta);
	void Go_Right(_double TimeDelta);

	void Turn(_fvector vAxis, _double TimeDelta);
	void Rotation(_fvector vAxis, _float fRadian);

	void LookAt(_fvector vTargetPos);

	void Chase(_fvector vTargetPos, _double TimeDelta, _float fLimit = 0.1f);

public:
	HRESULT	Bind_ShaderResource(class CShader* pShaderCom, const char* pConstantName);

private:
	_float4x4				m_WorldMatrix;
	TRANSFORMDESC			m_TransformDesc;

public:
	static CTransform*		Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CComponent*		Clone(void* pArg = nullptr) override;
	virtual void			Free() override;

};

END