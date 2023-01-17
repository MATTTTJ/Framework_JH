#pragma once
#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CModel final : public CComponent
{
public:
	enum MODELTYPE { MODEL_NONANIM, MODEL_ANIM, MODELTYPE_END };
	enum LERPTYPE { LERP_BEGIN, LERP_CONTINUE, LERPTYPE_END };
public:
	CModel(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CModel(const CModel& rhs);
	virtual ~CModel() = default;

public:
	HRESULT						Save_Model(const char* pSaveFileDirectory);
	const _int&					Get_CurAnimation() const { return m_iCurAnimIndex; }
	class CMesh*				Get_Mesh(const string& strMeshName);
	const MODELTYPE&			Get_ModelType() { return m_eType; }
	_uint						Get_NumMeshes() const {	return m_iNumMeshes; }
	_uint						Get_NumAnimation() const { return m_iNumAnimation; }
	_matrix						Get_PivotMatrix() const { return XMLoadFloat4x4(&m_PivotMatrix); }
	_float4x4					Get_PivotFloat4x4() const { return m_PivotMatrix; }
	_matrix						Get_BoneMatrix(const string& strBoneName);
	_matrix						Get_OffsetMatrix(const string& strBoneName);
	class	CBone*				Get_BonePtr(const string& strBoneName);
	_matrix						Get_CombindMatrix(const string& strBoneName);
	void						Set_CurAnimIndex(_uint AnimIndex);
	void						Set_BlendAnimIndex(_uint BlendAnimIndex);
	_bool						Get_AnimationFinish();
	_float						Get_AnimationProgress();
	_uint						Get_LastAnimationIndex() { return m_iLastAnimIndex; }
	class CAnimation*			Find_Anim(const string& strAnim);
	void						Reset_Animation();
	HRESULT						Check_MeshSize(const string & strMeshName, _float & Xmin, _float & Xmax, _float & Ymin, _float & Ymax, _float & Zmin, _float & Zmax);
	void						Set_LerpTime(_float fLerpTime) { m_fAnimChangeTime = fLerpTime; }

	void						Reset_IsFinished(void) { m_bIsAnimFinished = false; }

public:
	virtual HRESULT				Initialize_Prototype(MODELTYPE eType, const char* pModelFilePath, _fmatrix PivotMatrix);
	virtual HRESULT				Initialize_Clone(class CGameObject* pOwner, void* pArg);
	virtual void				Imgui_RenderProperty() override;
	void						Imgui_RenderAnimation();
	void						Imgui_RenderMeshes();
	void						Imgui_RenderTexture();
public:
	void						Play_Animation(_double TimeDelta, LERPTYPE eType = LERP_BEGIN);
	HRESULT						Bind_Material(class CShader* pShader, _uint iMeshIndex, aiTextureType eType, const wstring& pConstantName);
	HRESULT						Render(CShader* pShader, _uint iMeshIndex, const wstring & wstrBoneConstantName = L"", _uint iPassIndex = 0);
	HRESULT						Render_2Pass(CShader* pShader, _uint iMeshIndex, const wstring& NoRenderTag = L"", const wstring & wstrBoneConstantName = L"", _uint iPassIndex = 0);

	pair<_bool, _float3>		Picking(HWND& hWnd, class CTransform* pTransformCom);

private:
	const aiScene*				m_pAIScene = nullptr;
	Assimp::Importer					m_Importer;
	MODELTYPE					m_eType = MODELTYPE_END;
	_float4x4					m_PivotMatrix;

	_uint						m_iNumMeshes = 0;
	vector<class CMesh*>		m_vecMeshes;

	_uint						m_iNumMaterials = 0;
	vector<MODELMATERIAL>		m_vecMaterials;

	// ÀüÃ¼ »ÀÀÇ °¹¼ö
	_uint						m_iNumBones = 0;
	vector<class CBone*>		m_vecBones;

	_uint						m_iLastAnimIndex = 0;
	_int						m_iCurAnimIndex = 0;
	_int						m_iLastBlendIndex = 0;
	_int						m_iCurBlendIndex = 0;
	_float						m_fAnimBlendingTime = 1.f;
	_float						m_fCurAnimBlendingTime = 1.f;
	// AnimChange
	_float						m_fAnimChangeTime = 0.05f;
	_float						m_fCurAnimChangeTime = 0.05f;

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
	virtual CComponent*			Clone(class CGameObject* pOwner, void* pArg = nullptr) override;
	virtual void				Free() override;
};




END
