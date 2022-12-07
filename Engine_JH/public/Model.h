#pragma once
#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CModel final : public CComponent
{
public:
	enum MODELTYPE { MODEL_NONANIM, MODEL_ANIM, MODELTYPE_END };
public:
	CModel(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CModel(const CModel& rhs);
	virtual ~CModel() = default;

public:
	const MODELTYPE&		Get_ModelType() { return m_eType; }
	_uint		Get_NumMeshes() const {	return m_iNumMeshes; }
	_matrix		Get_PivotMatrix() const { return XMLoadFloat4x4(&m_PivotMatrix); }
	void		Set_AnimIndex(_uint AnimIndex) { m_iCurrentAnimIndex = AnimIndex; }
	class	CBone*	Get_BonePtr(const string& strBoneName);

public:
	virtual HRESULT				Initialize_Prototype(MODELTYPE eType, const char* pModelFilePath, _fmatrix PivotMatrix);
	virtual HRESULT				Initialize_Clone(void* pArg);
	virtual void				Imgui_RenderProperty() override;
public:
	void						Play_Animation(_double TimeDelta);
	HRESULT						Bind_Material(class CShader* pShader, _uint iMeshIndex, aiTextureType eType, const wstring& pConstantName);
	HRESULT						Render(CShader* pShader, _uint iMeshIndex, const wstring & wstrBoneConstantName = L"");

public:
	const aiScene*				m_pAIScene = nullptr;
	Importer					m_Importer;
	MODELTYPE					m_eType = MODELTYPE_END;

public:
	// 하나의 모델은 교체가 가능한 여러개의 메시로 구성되어있다. 
	_uint						m_iNumMeshes = 0;
	vector<class CMesh*>		m_vecMeshes;

	_uint						m_iNumMaterials = 0;
	vector<MODELMATERIAL>		m_vecMaterials;

	// 전체 뼈의 갯수
	_uint						m_iNumBones = 0;
	vector<class CBone*>		m_vecBones;

	_uint						m_iCurrentAnimIndex = 0;
	_uint						m_iNumAnimation = 0; // 애니메이션의 갯수 
	vector<class CAnimation*>	m_vecAnimations;


	_float4x4					m_PivotMatrix; 
public:
	HRESULT						Ready_Bones(aiNode* pAINode, CBone* pParent);
	HRESULT						Ready_MeshContainers();
	HRESULT						Ready_Materials(const char* pModelFilePath);
	HRESULT						Ready_Animation();
public:
	static	CModel*				Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, MODELTYPE eType, const char* pModelFilePath, _fmatrix PivotMatrix = XMMatrixRotationY(XMConvertToRadians(180.0f)));
	virtual CComponent*			Clone(void* pArg) override;
	virtual void				Free() override;
};


END
