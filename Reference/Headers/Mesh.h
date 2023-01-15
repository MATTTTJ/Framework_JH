#pragma once
#include "VIBuffer.h"
#include "Model.h"

BEGIN(Engine)

class CMesh final :	public CVIBuffer
{
public:
	CMesh(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CMesh(const CMesh& rhs);
	virtual ~CMesh() = default;

public:
	HRESULT					Save_Mesh(HANDLE& hFile, DWORD& dwByte);
	HRESULT					Save_MeshBones(HANDLE& hFile, DWORD& dwByte);
	HRESULT					Load_Mesh(HANDLE& hFile, DWORD& dwByte);
	const _uint&			Get_MaterialIndex() const {return m_iMaterialIndex;}
	string&					Get_MeshName() { return m_strName; }
	void					Check_MeshSize(_float& Xmin, _float& Xmax, _float& Ymin, _float& Ymax, _float& Zmin, _float& Zmax);


public:
	virtual HRESULT			Initialize_Prototype(CModel::MODELTYPE eTYPE, aiMesh* pAIMesh, CModel* pModel);
	virtual HRESULT			Initialize_Clone(class CGameObject* pOwner, void* pArg) override;

public:
	void					SetUp_MeshBones(class CModel* pModel);
	HRESULT					SetUp_MeshBones(HANDLE& hFile, DWORD& dwByte, class CModel* pModel);
	void					SetUp_BoneMatrix(_float4x4* pBoneMatrices, _fmatrix PivotMatrix);
	pair<_bool, _float>		Picking(HWND& hWnd, class CTransform* pTransformCom, _float3& vPickingPoint);

private:
	aiMesh*					m_pAIMesh = nullptr; // ���� ���縦 �ϸ鼭 �����͸� ���� �����Ϸ���
	string					m_strName = "";
	CModel::MODELTYPE		m_eType = CModel::MODELTYPE_END;

	// �� �޽ô� m_iMaterialIndex��° ���׸����� ����Ѵ�. 
	_uint					m_iMaterialIndex = 0;

	// �� �޽��� �����鿡�� ������ �ִ� ���� ����
	_uint					m_iNumMeshBones = 0;
	vector<class CBone*>	m_vecMeshBones;

	VTXMODEL*				m_pNonAnimVertices = nullptr;
	VTXANIMMODEL*			m_pAnimVertices = nullptr;
	FACEINDICES32*			m_pIndices = nullptr;

private:
	HRESULT					Ready_VertexBuffer_NonAnimModel(aiMesh* pAIMesh, CModel* pModel);
	HRESULT					Ready_VertexBuffer_AnimModel(aiMesh* pAIMesh, CModel* pModel);

public:
	static CMesh*			Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext,CModel::MODELTYPE eType, aiMesh* pAIMesh, CModel* pModel);
	virtual CComponent*		Clone(class CGameObject* pOwner, void* pArg = nullptr) override;
	virtual void			Free() override;

};

END