#pragma once
#include "GameObject.h"
BEGIN(Engine)
class ENGINE_DLL CCustomObject final :	public CGameObject
{
public:
	enum LEVEL { LEVEL_LOADING, LEVEL_LOGO, LEVEL_GAMEPLAY, LEVEL_MAPEDITOR };
	enum OBJECTUSAGE { PLAYER, MONSTER, UI, INVENTORY, COMMONOBJECT, OBJECTUSAGE_END };
	typedef struct tagProtoTyoeInfo
	{
		
	}PROTOTYPEINFO;

private:
	CCustomObject(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CCustomObject(const CCustomObject& rhs);
	virtual ~CCustomObject() = default;

public:
	vector<pair<_uint, wstring>>		Get_PrototypeSaveData();
	const _uint&						Get_TextureComCount() const { return m_iNumTextureCom; }
	class CModel*						Get_ModelComponent() const { return m_pModelCom; }


public:
	virtual HRESULT			Initialize_Prototype(const vector<pair<_uint, wstring>>& vecPrototypeInfo, _uint iNumTextureCom);
	virtual HRESULT			Initialize_Clone(const wstring& wstrPrototypeTag, void* pArg) override;
	virtual void			Tick(_double dTimeDelta) override;
	virtual void			Late_Tick(_double TimeDelta) override;
	virtual HRESULT			Render() override;

private:
	_uint					m_iRendererComLevel = 1000;
	_uint					m_iVIBufferComLevel = 1000;
	_uint					m_iShaderComLevel = 1000;
	_uint					m_iTransformComLevel = 1000;
	_uint					m_iTextureComLevel = 1000;
	_uint					m_iModelComLevel = 1000;

	wstring					m_wstrRendererComTag = L"";
	wstring					m_wstrVIBufferComTag = L"";
	wstring					m_wstrShaderComTag = L"";
	wstring					m_wstrProtoTransformComTag = L"";
	wstring					m_wstrModelComTag = L"";

	_uint					m_iNumTextureCom = 0;
	wstring*				m_wstrTextureComTag = nullptr;

	class CRenderer*		m_pRendererCom = nullptr;
	class CShader*			m_pShaderCom = nullptr;
	class CTexture**		m_pTextureCom = nullptr;
	class CVIBuffer*		m_pVIBufferCom = nullptr;
	class CModel*			m_pModelCom = nullptr;

private:
	HRESULT					SetUp_Component();
	HRESULT					SetUp_ShaderResource();

public:
	static CCustomObject*	Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const vector<pair<_uint, wstring>>& vecPrototypeInfo, _uint iNumTextureCom = 0);
	virtual CGameObject*	Clone(const wstring& wstrPrototypeTag, void* pArg) override;
	virtual void			Free() override;

};
END