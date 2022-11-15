#pragma once
#include "Component.h"

// ��ü�� ���� �����̽� �󿡼��� ���¸� �����ϰ� �����ϰ� ����
// ���º�ȯ�� ���� �������̽��� �����Ѵ�. (Go_Straight..)

BEGIN(Engine)

class ENGINE_DLL CTransform final : public CComponent
{
public:
	// State_Translationt �� Position�� ������
	// State_Translationt : (���� �Ǵ� �θ�) �������� �󸶳� ������ �ִ���
	// Position : ������������� �������� ��ġ

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

	_float3	Get_Scaled() const		// ��ü�� ���� �����ϰ��� float������ ������ �̾� �� �� �ְ� 
	{
		return _float3(XMVectorGetX(XMVector3Length(Get_State(STATE_RIGHT))),	// �� ���� ����(ũ��)�� ������ ��
			XMVectorGetX(XMVector3Length(Get_State(STATE_UP))),
			XMVectorGetX(XMVector3Length(Get_State(STATE_LOOK))));
	}

	void	Set_State(STATE eState, _fvector vState)
	{
		_float4			vTmp;
		XMStoreFloat4(&vTmp, vState);
		memcpy(&m_WorldMatrix.m[eState][0], &vTmp, sizeof(vTmp));
	}

	void	Set_Scaled(STATE eState, _float fScale); // fScale������ ���̸� �����Ѵ�. 
	void	Set_Scaled(_float3 vScale);	// fScale������ ���̸� �����Ѵ�. 
	void	Scaling(STATE eState, _float fScale); // ���� ũ�⸦ fScale ����� �ø���. 



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