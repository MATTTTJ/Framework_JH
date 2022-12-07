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
	aiMesh*						m_pAIMesh = nullptr; // 깊은 복사를 하면서 데이터를 따로 관리하려함
	string						m_strName = "";
	CModel::MODELTYPE			m_eType = CModel::MODELTYPE_END;

	// 이 메시는 m_iMaterialIndex번째 머테리얼을 사용한다. 
	_uint					m_iMaterialIndex = 0;

	// 이 메시의 정점들에게 영향을 주는 뼈의 갯수
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