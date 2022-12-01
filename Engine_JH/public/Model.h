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
	_uint					Get_NumMeshes() const
	{
		return m_iNumMeshes;
	}
	class	CBone*			Get_BonePtr(const char* pBoneName);
public:
	virtual HRESULT			Initialize_Prototype(TYPE eType, const char* pModelFilePath);
	virtual HRESULT			Initialize_Clone(void* pArg);

public:
	void					Play_Animation(_double TimeDelta);
	// 어떤 메쉬에 어떤 텍스쳐를 쓸건지 묶는 함수
	HRESULT					Bind_Material(class CShader* pShader, _uint iMeshIndex, aiTextureType eType, const char* pConstantName);
	// 모델 자체에서 메쉬에 대한 렌더를 돌림
	HRESULT					Render(CShader* pShader, _uint iMeshIndex);

public:
	const aiScene*				m_pAIScene = nullptr;
	Assimp::Importer			m_Importer;
	TYPE						m_eType = TYPE_END;

public:
	// 하나의 모델은 교체가 가능한 여러개의 메시로 구성되어있다. 
	_uint						m_iNumMeshes = 0;
	vector<class CMesh*>		m_Meshes;

	_uint						m_iNumMaterials = 0;
	vector<MODELMATERIAL>		m_Materials;

	// 뼈의 갯수
	_uint						m_iNumBones = 0;
	vector<class CBone*>		m_Bones;

	// 애니메이션의 갯수
	_uint						m_iNumAnimation = 0;
	vector<class CAnimation*>	m_Animations;

public:
	HRESULT						Ready_Bones(aiNode* pAINode);
	HRESULT						Ready_MeshContainers();
	HRESULT						Ready_Materials(const char* pModelFilePath);

public:
	static	CModel*				Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, TYPE eType, const char* pModelFilePath);
	virtual CComponent*			Clone(void* pArg) override;
	virtual void				Free() override;
};


END
