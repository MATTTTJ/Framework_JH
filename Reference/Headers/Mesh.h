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
	_uint Get_MaterialIndex() const {
		return m_iMaterialIndex;
	}


public:
	virtual HRESULT			Initialize_Prototype(CModel::TYPE eTYPE, aiMesh* pAIMesh, CModel* pModel);
	virtual HRESULT			Initialize_Clone(void* pArg) override;

public:
	void SetUp_BoneMatrix(_float4x4* pBoneMatrices);

private:
	CModel::TYPE			m_eType;

	// �� �޽ô� m_iMaterialIndex��° ���׸����� ����Ѵ�. 
	_uint					m_iMaterialIndex = 0;

	// �� �޽��� �����鿡�� ������ �ִ� ���� ����
	_uint					m_iNumBones = 0;
	vector<class CBone*>	m_Bones;
private:
	HRESULT					Ready_VertexBuffer_NonAnimModel(aiMesh* pAIMesh);
	HRESULT					Ready_VertexBuffer_AnimModel(aiMesh* pAIMesh, CModel* pModel);

public:
	static CMesh*			Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext,CModel::TYPE eType, aiMesh* pAIMesh, CModel* pModel);
	virtual CComponent*		Clone(void* pArg = nullptr) override;
	virtual void			Free();

};

END