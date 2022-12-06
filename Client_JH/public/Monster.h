// #pragma once
// #include "Client_Defines.h"
// #include "GameObject.h"
//
// BEGIN(Engine)
// class CShader;
// class CRenderer;
// class CModel;
// END
//
//
// class CMonster abstract : public CGameObject
// {
// public:
// 	CMonster();
// 	virtual ~CMonster();
// 	virtual HRESULT Initialize_Prototype() override;
// 	virtual HRESULT Initialize_Clone(const wstring& wstrPrototypeTag, void* pArg) override;
// 	virtual void Tick(_double dTimeDelta) override;
// 	virtual void Late_Tick(_double dTimeDelta) override;
// 	virtual HRESULT Render() override;
// 	virtual void Imgui_RenderProperty() override;
//
// private:
// 	CShader*				m_pShaderCom = nullptr;
// 	CRenderer*				m_pRendererCom = nullptr;
// 	CModel*					m_pModelCom = nullptr;
//
// public:
// 	static CMonster*	Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
// 	virtual CGameObject* Clone(const wstring& wstrPrototypeTag, void* pArg) override;
// 	virtual void Free() override;
};

