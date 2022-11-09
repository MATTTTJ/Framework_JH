#pragma once
#include "GameObject.h"

BEGIN(Engine)
class CRenderer;
END


BEGIN(Client)

class CBackGround final : public CGameObject
{
private:
	CBackGround(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CBackGround(const CBackGround& rhs);
	virtual ~CBackGround() = default;

public:
	virtual HRESULT		Initialize_Prototype() override;
	virtual HRESULT		Initialize_Clone(void* pArg) override;
	virtual void		Tick(_double TimeDelta) override;
	virtual void		Late_Tick(_double TimeDelta) override;
	virtual HRESULT		Render() override;


private:
	CRenderer*		m_pRendererCom = nullptr;

private:
	HRESULT			SetUp_Component();

public:
	static CBackGround*			Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject*		Clone(void* pArg) override;
	virtual void				Free() override;
};

END