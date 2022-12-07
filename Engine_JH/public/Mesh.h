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
	const _uint&	Get_MaterialIndex() const {return m_iMaterialIndex;}
	const string&	Get_MeshName() const { return m_strName; }


public:
	virtual HRESULT			Initialize_Prototype(CModel::MODELTYPE eTYPE, aiMesh* pAIMesh, CModel* pModel);
	virtual HRESULT			Initialize_Clone(void* pArg) override;

public:
	void SetUp_MeshBones(class CModel* pModel);
	void SetUp_BoneMatrix(_float4x4* pBoneMatrices, _fmatrix PivotMatrix);

private:
	aiMesh*						m_pAIMesh = nullptr; // ���� ���縦 �ϸ鼭 �����͸� ���� �����Ϸ���
	string						m_strName = "";
	CModel::MODELTYPE			m_eType = CModel::MODELTYPE_END;

	// �� �޽ô� m_iMaterialIndex��° ���׸����� ����Ѵ�. 
	_uint					m_iMaterialIndex = 0;

	// �� �޽��� �����鿡�� ������ �ִ� ���� ����
	_uint					m_iNumMeshBones = 0;
	vector<class CBone*>	m_vecMeshBones;
private:
	HRESULT					Ready_VertexBuffer_NonAnimModel(aiMesh* pAIMesh, CModel* pModel);
	HRESULT					Ready_VertexBuffer_AnimModel(aiMesh* pAIMesh, CModel* pModel);

public:
	static CMesh*			Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext,CModel::MODELTYPE eType, aiMesh* pAIMesh, CModel* pModel);
	virtual CComponent*		Clone(void* pArg = nullptr) override;
	virtual void			Free();

};

END