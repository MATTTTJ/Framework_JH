#pragma once
#include "GameObject.h"

BEGIN(Engine)
	class ENGINE_DLL CCamera abstract : public CGameObject
{
public:
	typedef struct tagCameraDesc : public CGameObject::GAMEOBJECTDESC
	{
		_float4	vEye, vAt, vUp;
		_float	fFovy, fAspect, fNear, fFar;
	}CAMERADESC;

public:
	CCamera(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CCamera(const CCamera& rhs);
	virtual ~CCamera() = default;

public:
	_bool&					Get_RenderState() { return m_bRender; }

	virtual HRESULT			Initialize_Prototype();
	virtual HRESULT			Initialize_Clone(const wstring& wstrPrototypeTag, void* pArg) override;
	virtual void 			Tick(_double TimeDelta);
	virtual void			Late_Tick(_double TimeDelta);
	virtual HRESULT			Render();



public:
	CAMERADESC				Get_CamDesc() { return m_CameraDesc; }

private:
	void					SetCascade();

private:
	class CPipeLine*		m_pPipeLine = nullptr;
	CAMERADESC				m_CameraDesc;

protected:
	_bool					m_bRender = false;
public:

	virtual CGameObject*	Clone(const wstring& wstrPrototypeTag, void* pArg = nullptr) = 0;
	virtual void			Free() override;
};

END