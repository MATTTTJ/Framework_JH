#pragma once
#include "GameObject.h"

BEGIN(Engine)
	class ENGINE_DLL CCamera abstract : public CGameObject
{
public:
	typedef struct tagCameraDesc : public CGameObject::GAMEOBJECTDESC
	{
		_float4	vEye, vAt, vUp;
	}CAMERADESC;

public:
	CCamera(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CCamera(const CCamera& rhs);
	virtual ~CCamera() = default;

	virtual HRESULT			Initialize_Prototype();
	virtual HRESULT			Initialize_Clone(void* pArg);
	virtual void			Tick(_double TimeDelta);
	virtual void			Late_Tick(_double TimeDelta);
	virtual HRESULT			Render();


private:
	class CPipeLine*		m_pPipeLine = nullptr;
	CAMERADESC				m_CameraDesc;

public:

	virtual CGameObject*	Clone(void* pArg = nullptr) = 0;
	virtual void			Free() override;
};

END