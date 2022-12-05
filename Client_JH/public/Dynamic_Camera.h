#pragma once
#include "Client_Defines.h"
#include "Camera.h"

BEGIN(Client)

class CDynamic_Camera final : public CCamera
{
public:
	CDynamic_Camera(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CDynamic_Camera(const CDynamic_Camera& rhs);
	virtual ~CDynamic_Camera() = default;

	virtual HRESULT			Initialize_Prototype()		 override;
	virtual HRESULT			Initialize_Clone(const wstring& wstrPrototypeTag, void* pArg) override;
	virtual void			Tick(_double TimeDelta)		 override;
	virtual void			Late_Tick(_double TimeDelta) override;
	virtual HRESULT			Render()					 override;

public:
	HRESULT					SetUp_Components();

public:
	static CDynamic_Camera*	Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject*	Clone(const wstring& wstrPrototypeTag, void* pArg = nullptr) override;
	virtual void			Free() override;
};

END