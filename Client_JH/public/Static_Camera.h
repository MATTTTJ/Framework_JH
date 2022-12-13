#pragma once
#include "Client_Defines.h"
#include "Camera.h"

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

private:
	CGameObject*			m_pOwner = nullptr;

private:
	HRESULT					SetUp_Component();

public:
	static CStatic_Camera*	Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject*	Clone(const wstring& wstrPrototypeTag, void* pArg = nullptr) override;
	virtual void			Free() override;
};

END