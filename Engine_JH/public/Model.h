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
	_uint		Get_NumMeshes() const {	return m_iNumMeshes; }

	void		Set_AnimIndex(_uint AnimIndex) { m_iCurrentAnimIndex = AnimIndex; }

	class	CBone*	Get_BonePtr(const string& strBoneName);

public:
	virtual HRESULT				Initialize_Prototype(TYPE eType, const char* pModelFilePath);
	virtual HRESULT				Initialize_Clone(void* pArg);

public:
	void						Play_Animation(_double TimeDelta);
	HRESULT						Bind_Material(class CShader* pShader, _uint iMeshIndex, aiTextureType eType, const wstring& pConstantName);
	HRESULT						Render(CShader* pShader, _uint iMeshIndex);

public:
	const aiScene*				m_pAIScene = nullptr;
	Assimp::Importer			m_Importer;
	TYPE						m_eType = TYPE_END;

public:
	// �ϳ��� ���� ��ü�� ������ �������� �޽÷� �����Ǿ��ִ�. 
	_uint						m_iNumMeshes = 0;
	vector<class CMesh*>		m_Meshes;

	_uint						m_iNumMaterials = 0;
	vector<MODELMATERIAL>		m_Materials;

	// ��ü ���� ����
	_uint						m_iNumBones = 0;
	vector<class CBone*>		m_Bones;

	_uint						m_iCurrentAnimIndex = 0;
	_uint						m_iNumAnimation = 0; // �ִϸ��̼��� ���� 
	vector<class CAnimation*>	m_Animations;

public:
	HRESULT						Ready_Bones(aiNode* pAINode);
	HRESULT						Ready_MeshContainers();
	HRESULT						Ready_Materials(const char* pModelFilePath);
	HRESULT						Ready_Animation();
public:
	static	CModel*				Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, TYPE eType, const char* pModelFilePath);
	virtual CComponent*			Clone(void* pArg) override;
	virtual void				Free() override;
};


END
