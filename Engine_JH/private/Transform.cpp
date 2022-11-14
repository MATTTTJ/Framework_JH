#include "..\public\Transform.h"

CTransform::CTransform(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CComponent(pDevice,pContext)
{
}

CTransform::CTransform(const CTransform& rhs)
	: CComponent(rhs)
	, m_WorldMatrix(rhs.m_WorldMatrix)
{
}

HRESULT CTransform::Initialize_Prototype()
{
	XMStoreFloat4x4(&m_WorldMatrix, XMMatrixIdentity());

	return S_OK;
}

HRESULT CTransform::Initialize_Clone(void* pArg)
{
	if (nullptr != pArg)
		memcpy(&m_TransformDesc, pArg, sizeof(TRANSFORMDESC));

	return S_OK;
}

CTransform* CTransform::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CTransform*		pInst = new CTransform(pDevice, pContext);

	if (FAILED(pInst->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CTransform");
		Safe_Release(pInst);
	}
	return pInst;
}

CComponent * CTransform::Clone(void * pArg)
{
	CTransform*		pInst = new CTransform(*this);

	if (FAILED(pInst->Initialize_Clone(pArg)))
	{
		MSG_BOX("Failed to Cloned : CTransform");
		Safe_Release(pInst);
	}

	return pInst;
}

void CTransform::Free()
{
	__super::Free();
}
