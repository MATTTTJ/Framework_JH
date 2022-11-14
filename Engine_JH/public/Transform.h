#pragma once
#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CTransform final : public CComponent
{
public:
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

	void	Set_State(STATE eState, _fvector vState)
	{
		_float4			vTmp;
		XMStoreFloat4(&vTmp, vState);
		memcpy(&m_WorldMatrix.m[eState][0], &vTmp, sizeof(vTmp));
	}

	virtual HRESULT			Initialize_Prototype();
	virtual HRESULT			Initialize_Clone(void* pArg);


private:
	_float4x4				m_WorldMatrix;
	TRANSFORMDESC			m_TransformDesc;

public:
	static CTransform*		Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CComponent*		Clone(void* pArg = nullptr) override;
	virtual void			Free() override;

};

END