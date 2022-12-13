#pragma once
#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CShader;
class CRenderer;
class CModel;
class CBone;
END


BEGIN(Engine)

class CWeapon :	public CGameObject
{
public:
	typedef struct tagWeaponDesc
	{
		_float4x4		PivotMatrix;
		CBone*			pSocket;
		CTransform*		pTargetTransform;
	}WEAPONDESC;

private:
	CWeapon(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CWeapon(const CWeapon& rhs);
	virtual ~CWeapon() = default;

public:
	virtual HRESULT			Initialize_Prototype() override;
	virtual HRESULT			Initialize_Clone(const wstring& wstrPrototypeTag, void* pArg) override;
	virtual void			Tick(_double dTimeDelta) override;
	virtual void			Late_Tick(_double dTimeDelta) override;
	virtual HRESULT			Render() override;

private:
	HRESULT					SetUp_Component();
	HRESULT					SetUp_ShaderResource();

private:
	CShader*				m_pShaderCom = nullptr;
	CRenderer*				m_pRendererCom = nullptr;
	CModel*					m_pModelCom = nullptr;

	WEAPONDESC				m_WeaponDesc;

	_float4x4				m_SocketMatrix;


public:
	static CWeapon*			Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject*	Clone(const wstring& wstrPrototypeTag, void* pArg) override;
	virtual void			Free() override;
};

END