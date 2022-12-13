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
	HRESULT						Save_Model(const char* pSaveFileDirectory);
	const MODELTYPE&			Get_ModelType() { return m_eType; }
	_uint						Get_NumMeshes() const {	return m_iNumMeshes; }
	_uint						Get_NumAnimation() const { return m_iNumAnimation; }
	_matrix						Get_PivotMatrix() const { return XMLoadFloat4x4(&m_PivotMatrix); }
	_float4x4					Get_PivotFloat4x4() const { return m_PivotMatrix; }
	_matrix						Get_BoneMatrix(const string& strBoneName);
	_matrix						Get_OffsetMatrix(const string& strBoneName);
	class	CBone*				Get_BonePtr(const string& strBoneName);
	void						Set_CurAnimIndex(_uint AnimIndex);


public:
	virtual HRESULT				Initialize_Prototype(MODELTYPE eType, const char* pModelFilePath, _fmatrix PivotMatrix);
	virtual HRESULT				Initialize_Clone(void* pArg);
	virtual void				Imgui_RenderProperty() override;
public:
	void						Play_Animation(_double TimeDelta, _bool bFinish = false);
	HRESULT						Bind_Material(class CShader* pShader, _uint iMeshIndex, aiTextureType eType, const wstring& pConstantName);
	HRESULT						Render(CShader* pShader, _uint iMeshIndex, const wstring & wstrBoneConstantName = L"", _uint iPassIndex = 0);

private:
	const aiScene*				m_pAIScene = nullptr;
	Importer					m_Importer;
	MODELTYPE					m_eType = MODELTYPE_END;
	_float4x4					m_PivotMatrix;

	_uint						m_iNumMeshes = 0;
	vector<class CMesh*>		m_vecMeshes;

	_uint						m_iNumMaterials = 0;
	vector<MODELMATERIAL>		m_vecMaterials;

	// ÀüÃ¼ »ÀÀÇ °¹¼ö
	_uint						m_iNumBones = 0;
	vector<class CBone*>		m_vecBones;

	_uint						m_iCurrentAnimIndex = 0;
	_uint						m_iLastAnimIndex = 9999;
	_uint						m_iNumAnimation = 0; // ¾Ö´Ï¸ÞÀÌ¼ÇÀÇ °¹¼ö 
	vector<class CAnimation*>	m_vecAnimations;

	DWORD						m_dwBeginBoneData = 0;

	_bool						m_bIsAnimChange = false;
	_bool						m_bIsAnimFinished = false;

public:
	HRESULT						Ready_Bones(aiNode* pAINode, CBone* pParent);
	HRESULT						Ready_Bones(HANDLE& hFile, DWORD& dwByte, class CBone* pParent);
	HRESULT						Ready_MeshContainers();
	HRESULT						Ready_Materials(const char* pModelFilePath);
	HRESULT						Ready_Animation();

	HRESULT						Load_MeshMaterial(const wstring& wstrModelFilePath);
	HRESULT						Load_BoneAnimation(HANDLE& hFile, DWORD& dwByte);
public:
	static	CModel*				Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, MODELTYPE eType, const char* pModelFilePath, _fmatrix PivotMatrix = XMMatrixRotationY(XMConvertToRadians(180.0f)));
	virtual CComponent*			Clone(void* pArg = nullptr) override;
	virtual void				Free() override;
};


END
