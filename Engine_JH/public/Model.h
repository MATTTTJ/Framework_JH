#pragma once
#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CModel final : public CComponent
{
public:
	enum TYPE { TYPE_NONANIM, TYPE_ANIM, TYPE_END };
public:
	CModel(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CModel(const CModel& rhs);
	virtual ~CModel() = default;

public:
	virtual HRESULT			Initialize_Prototype(TYPE eType, const char* pModelFilePath);
	virtual HRESULT			Initialize_Clone(void* pArg);

public:
	const aiScene*			m_pAIScene = nullptr;
	Assimp::Importer		m_Importer;
	TYPE					m_eType = TYPE_END;

public:
	HRESULT					Render();

	// 하나의 모델은 교체가 가능한 여러개의 메시로 구성되어있다. 
	_uint					m_iNumMeshes = 0;
	vector<class CMesh*>	m_Meshes;

	_uint					m_iNumMaterials = 0;
	vector<MODELMATERIAL>	m_Materials;

public:
	HRESULT					Ready_MeshContainers();
	HRESULT					Ready_Materials(const char* pModelFilePath);

public:
	static	CModel*			Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, TYPE eType, const char* pModelFilePath);
	virtual CComponent*		Clone(void* pArg) override;
	virtual void			Free() override;
};


END
