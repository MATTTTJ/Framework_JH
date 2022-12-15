#include "stdafx.h"
#include "..\public\Collider.h"
#include "DebugDraw.h"
#include "PipeLine.h"
#include "GameObject.h"

CCollider::CCollider(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CComponent(pDevice, pContext)
{

}

CCollider::CCollider(const CCollider & rhs)
	: CComponent(rhs)
	, m_eType(rhs.m_eType)
#ifdef _DEBUG
	, m_pBatch(rhs.m_pBatch)
	, m_pEffect(rhs.m_pEffect)
	, m_pInputLayout(rhs.m_pInputLayout)
#endif // _DEBUG
{
#ifdef _DEBUG
	Safe_AddRef(m_pInputLayout);
#endif // _DEBUG

}

HRESULT CCollider::Initialize_Prototype(COLLIDERTYPE eType)
{
	m_eType = eType;

#ifdef _DEBUG
	m_pBatch = new PrimitiveBatch<VertexPositionColor>(m_pContext);
	m_pEffect = new BasicEffect(m_pDevice);
	m_pEffect->SetVertexColorEnabled(true);

	const void*		pShaderByteCode;
	size_t			iShaderByteCodeSize;

	m_pEffect->GetVertexShaderBytecode(&pShaderByteCode, &iShaderByteCodeSize);

	if (FAILED(m_pDevice->CreateInputLayout(VertexPositionColor::InputElements, VertexPositionColor::InputElementCount, pShaderByteCode, iShaderByteCodeSize, &m_pInputLayout)))
		return E_FAIL;

#endif

	return S_OK;
}

HRESULT CCollider::Initialize_Clone(CGameObject* pOwner, void * pArg)
{
	FAILED_CHECK_RETURN(__super::Initialize_Clone(pOwner, pArg), E_FAIL);

	COLLIDERDESC		ColliderDesc;
	memcpy(&ColliderDesc, pArg, sizeof(COLLIDERDESC));

	switch (m_eType)
	{
	case CCollider::COLLIDER_AABB:
		m_pAABB_Original = new BoundingBox(_float3(0.f, 0.f, 0.f), _float3(0.5f, 0.5f, 0.5f));
		m_pAABB_Original->Transform(*m_pAABB_Original,
			XMMatrixScaling(ColliderDesc.vSize.x, ColliderDesc.vSize.y, ColliderDesc.vSize.z) *
			XMMatrixTranslation(ColliderDesc.vPosition.x, ColliderDesc.vPosition.y, ColliderDesc.vPosition.z));
		m_pAABB = new BoundingBox(*m_pAABB_Original);
		break;

	case CCollider::COLLIDER_OBB:
		m_pOBB_Original = new BoundingOrientedBox(_float3(0.f, 0.f, 0.f), _float3(ColliderDesc.vSize.x * 0.5f, ColliderDesc.vSize.y * 0.5f, ColliderDesc.vSize.z * 0.5f), _float4(0.f, 0.f, 0.f, 1.f));
		m_pOBB_Original->Transform(*m_pOBB_Original,
			// XMMatrixScaling(ColliderDesc.vSize.x, ColliderDesc.vSize.y, ColliderDesc.vSize.z) *			
			XMMatrixRotationX(ColliderDesc.vRotation.x) *
			XMMatrixRotationY(ColliderDesc.vRotation.y) *
			XMMatrixRotationZ(ColliderDesc.vRotation.z) *
			XMMatrixTranslation(ColliderDesc.vPosition.x, ColliderDesc.vPosition.y, ColliderDesc.vPosition.z));
		m_pOBB = new BoundingOrientedBox(*m_pOBB_Original);
		break;

	case CCollider::COLLIDER_SPHERE:
		m_pSphere_Original = new BoundingSphere(_float3(0.f, 0.f, 0.f), 0.5f);

		m_pSphere_Original->Transform(*m_pSphere_Original,
			XMMatrixScaling(ColliderDesc.vSize.x, ColliderDesc.vSize.y, ColliderDesc.vSize.z) *
			XMMatrixRotationX(ColliderDesc.vRotation.x) *
			XMMatrixRotationY(ColliderDesc.vRotation.y) *
			XMMatrixRotationZ(ColliderDesc.vRotation.z) *
			XMMatrixTranslation(ColliderDesc.vPosition.x, ColliderDesc.vPosition.y, ColliderDesc.vPosition.z));
		m_pSphere = new BoundingSphere(*m_pSphere_Original);
		break;
	
	}

	return S_OK;
}
void CCollider::Update(_fmatrix TransformMatrix)
{
	switch (m_eType)
	{
	case CCollider::COLLIDER_AABB:
		m_pAABB_Original->Transform(*m_pAABB, Remove_Rotation(TransformMatrix));
		break;
	case CCollider::COLLIDER_OBB:
		m_pOBB_Original->Transform(*m_pOBB, TransformMatrix);
		break;
	case CCollider::COLLIDER_SPHERE:
		m_pSphere_Original->Transform(*m_pSphere, TransformMatrix);
		break;
	}
}

_bool CCollider::Collision(CCollider* pTargetCollider)
{
	m_bIsColl = false;

	switch (m_eType)
	{
	case CCollider::COLLIDER_AABB:
		if (COLLIDER_AABB == pTargetCollider->m_eType)
			m_bIsColl = m_pAABB->Intersects(*pTargetCollider->m_pAABB);
		if (COLLIDER_OBB == pTargetCollider->m_eType)
			m_bIsColl = m_pAABB->Intersects(*pTargetCollider->m_pOBB);
		if (COLLIDER_SPHERE == pTargetCollider->m_eType)
			m_bIsColl = m_pAABB->Intersects(*pTargetCollider->m_pSphere);
		break;
	case CCollider::COLLIDER_OBB:
		if (COLLIDER_AABB == pTargetCollider->m_eType)
			m_bIsColl = m_pOBB->Intersects(*pTargetCollider->m_pAABB);
		if (COLLIDER_OBB == pTargetCollider->m_eType)
			m_bIsColl = m_pOBB->Intersects(*pTargetCollider->m_pOBB);
		if (COLLIDER_SPHERE == pTargetCollider->m_eType)
			m_bIsColl = m_pOBB->Intersects(*pTargetCollider->m_pSphere);
		break;
	case CCollider::COLLIDER_SPHERE:
		if (COLLIDER_AABB == pTargetCollider->m_eType)
			m_bIsColl = m_pSphere->Intersects(*pTargetCollider->m_pAABB);
		if (COLLIDER_OBB == pTargetCollider->m_eType)
			m_bIsColl = m_pSphere->Intersects(*pTargetCollider->m_pOBB);
		if (COLLIDER_SPHERE == pTargetCollider->m_eType)
			m_bIsColl = m_pSphere->Intersects(*pTargetCollider->m_pSphere);
		break;
	}
	return m_bIsColl;

}

CGameObject* CCollider::CollisionReturnObj(CCollider* pTargetCollider)
{
	m_bIsColl = false;
	CGameObject*		pTarget = nullptr;

	switch(m_eType)
	{
	case COLLIDER_AABB:
		if (pTargetCollider->Get_ColliderType() == COLLIDER_AABB && 
			m_pAABB->Intersects(*pTargetCollider->m_pAABB))
			pTarget = pTargetCollider->Get_Owner();
		if (pTargetCollider->Get_ColliderType() == COLLIDER_OBB &&
			m_pAABB->Intersects(*pTargetCollider->m_pOBB))
			pTarget = pTargetCollider->Get_Owner();
		if (pTargetCollider->Get_ColliderType() == COLLIDER_SPHERE &&
			m_pAABB->Intersects(*pTargetCollider->m_pSphere))
			pTarget = pTargetCollider->Get_Owner();
		break;

	case COLLIDER_OBB:
		if (pTargetCollider->Get_ColliderType() == COLLIDER_AABB &&
			m_pOBB->Intersects(*pTargetCollider->m_pAABB))
			pTarget = pTargetCollider->Get_Owner();
		if (pTargetCollider->Get_ColliderType() == COLLIDER_OBB &&
			m_pOBB->Intersects(*pTargetCollider->m_pOBB))
			pTarget = pTargetCollider->Get_Owner();
		if (pTargetCollider->Get_ColliderType() == COLLIDER_SPHERE &&
			m_pOBB->Intersects(*pTargetCollider->m_pSphere))
			pTarget = pTargetCollider->Get_Owner();
		break;
	case COLLIDER_SPHERE:
		if (pTargetCollider->Get_ColliderType() == COLLIDER_AABB &&
			m_pSphere->Intersects(*pTargetCollider->m_pAABB))
			pTarget = pTargetCollider->Get_Owner();
		if (pTargetCollider->Get_ColliderType() == COLLIDER_OBB &&
			m_pSphere->Intersects(*pTargetCollider->m_pOBB))
			pTarget = pTargetCollider->Get_Owner();
		if (pTargetCollider->Get_ColliderType() == COLLIDER_SPHERE &&
			m_pSphere->Intersects(*pTargetCollider->m_pSphere))
			pTarget = pTargetCollider->Get_Owner();
		break;
	}

	if (pTarget != nullptr)
		m_bIsColl = true;

	return pTarget;
}


_bool CCollider::Collision_AABB(CCollider* pTargetCollider)
{
	if (COLLIDER_AABB != m_eType ||
		COLLIDER_AABB != pTargetCollider->m_eType)
		return false;

	_float3			vSourMin, vSourMax;
	_float3			vDestMin, vDestMax;

	vSourMin = Compute_Min();
	vSourMax = Compute_Max();
	vDestMin = pTargetCollider->Compute_Min();
	vDestMax = pTargetCollider->Compute_Max();

	m_bIsColl = false;

	/* x축 선상에서 비교. */
	if (max(vSourMin.x, vDestMin.x) > min(vSourMax.x, vDestMax.x))
		return m_bIsColl;

	/* y축 선상에서 비교. */
	if (max(vSourMin.y, vDestMin.y) > min(vSourMax.y, vDestMax.y))
		return m_bIsColl;

	/* z축 선상에서 비교. */
	if (max(vSourMin.z, vDestMin.z) > min(vSourMax.z, vDestMax.z))
		return m_bIsColl;

	m_bIsColl = true;

	return m_bIsColl;
}

_bool CCollider::Collision_OBB(CCollider* pTargetCollider)
{
	if (COLLIDER_OBB != m_eType ||
		COLLIDER_OBB != pTargetCollider->m_eType)
		return false;

	m_bIsColl = true;

	OBBDESC		OBBDesc[2];

	OBBDesc[0] = Compute_OBBDesc();
	OBBDesc[1] = pTargetCollider->Compute_OBBDesc();

	/* 0 : 두 박스의 센터를 잇는 벡터를 투영시킨 길이.  */
	/* 1 : 첫번째 박스의 CenterAxis세개를 투영시킨 길이의 합.  */
	/* 2 : 두번째 박스의 CenterAxis세개를 투영시킨 길이의 합.  */
	_float			fDistance[3];

	for (_uint i = 0; i < 2; ++i)
	{
		for (_uint j = 0; j < 3; ++j)
		{
			fDistance[0] = fabs(XMVectorGetX(XMVector3Dot(XMLoadFloat3(&OBBDesc[1].vCenter) - XMLoadFloat3(&OBBDesc[0].vCenter),
				XMLoadFloat3(&OBBDesc[i].vAlignAxis[j]))));

			fDistance[1] = fabs(XMVectorGetX(XMVector3Dot(XMLoadFloat3(&OBBDesc[0].vCenterAxis[0]),
				XMLoadFloat3(&OBBDesc[i].vAlignAxis[j])))) +
				fabs(XMVectorGetX(XMVector3Dot(XMLoadFloat3(&OBBDesc[0].vCenterAxis[1]),
					XMLoadFloat3(&OBBDesc[i].vAlignAxis[j])))) +
				fabs(XMVectorGetX(XMVector3Dot(XMLoadFloat3(&OBBDesc[0].vCenterAxis[2]),
					XMLoadFloat3(&OBBDesc[i].vAlignAxis[j]))));

			fDistance[2] = fabs(XMVectorGetX(XMVector3Dot(XMLoadFloat3(&OBBDesc[1].vCenterAxis[0]),
				XMLoadFloat3(&OBBDesc[i].vAlignAxis[0])))) +
				fabs(XMVectorGetX(XMVector3Dot(XMLoadFloat3(&OBBDesc[1].vCenterAxis[1]),
					XMLoadFloat3(&OBBDesc[i].vAlignAxis[0])))) +
				fabs(XMVectorGetX(XMVector3Dot(XMLoadFloat3(&OBBDesc[1].vCenterAxis[2]),
					XMLoadFloat3(&OBBDesc[i].vAlignAxis[0]))));

			if (fDistance[0] > fDistance[1] + fDistance[2])
			{
				m_bIsColl = false;
				return m_bIsColl;
			}
		}
	}

	_vector			vAlignAxis[9];

	for (_uint i = 0; i < 3; ++i)
	{
		for (_uint j = 0; j < 3; ++j)
		{
			_uint		iIndex = i * 3 + j;

			vAlignAxis[iIndex] = XMVector3Cross(XMLoadFloat3(&OBBDesc[0].vAlignAxis[i]), XMLoadFloat3(&OBBDesc[1].vAlignAxis[j]));
		}
	}

	for (_uint i = 0; i < 9; ++i)
	{
		fDistance[0] = fabs(XMVectorGetX(XMVector3Dot(XMLoadFloat3(&OBBDesc[1].vCenter) - XMLoadFloat3(&OBBDesc[0].vCenter),
			vAlignAxis[i])));

		fDistance[1] = fabs(XMVectorGetX(XMVector3Dot(XMLoadFloat3(&OBBDesc[0].vCenterAxis[0]),
			vAlignAxis[i]))) +
			fabs(XMVectorGetX(XMVector3Dot(XMLoadFloat3(&OBBDesc[0].vCenterAxis[1]),
				vAlignAxis[i]))) +
			fabs(XMVectorGetX(XMVector3Dot(XMLoadFloat3(&OBBDesc[0].vCenterAxis[2]),
				vAlignAxis[i])));

		fDistance[2] = fabs(XMVectorGetX(XMVector3Dot(XMLoadFloat3(&OBBDesc[1].vCenterAxis[0]),
			vAlignAxis[i]))) +
			fabs(XMVectorGetX(XMVector3Dot(XMLoadFloat3(&OBBDesc[1].vCenterAxis[1]),
				vAlignAxis[i]))) +
			fabs(XMVectorGetX(XMVector3Dot(XMLoadFloat3(&OBBDesc[1].vCenterAxis[2]),
				vAlignAxis[i])));

		if (fDistance[0] > fDistance[1] + fDistance[2])
		{
			m_bIsColl = false;
			return m_bIsColl;
		}
	}

	return m_bIsColl;
}

#ifdef _DEBUG
HRESULT CCollider::Render()
{
	m_vColor = m_bIsColl == true ? _float4(1.f, 0.f, 0.f, 1.f) : _float4(0.f, 1.f, 0.f, 1.f);

	m_pEffect->SetWorld(XMMatrixIdentity());

	CPipeLine*		pPipeLine = GET_INSTANCE(CPipeLine);

	m_pEffect->SetView(pPipeLine->Get_TransformMatrix(CPipeLine::D3DTS_VIEW));
	m_pEffect->SetProjection(pPipeLine->Get_TransformMatrix(CPipeLine::D3DTS_PROJ));

	RELEASE_INSTANCE(CPipeLine);

	m_pEffect->Apply(m_pContext);

	m_pContext->IASetInputLayout(m_pInputLayout);

	m_pBatch->Begin();

	switch (m_eType)
	{
	case CCollider::COLLIDER_AABB:
		DX::Draw(m_pBatch, *m_pAABB, XMLoadFloat4(&m_vColor));
		break;
	case CCollider::COLLIDER_OBB:
		DX::Draw(m_pBatch, *m_pOBB, XMLoadFloat4(&m_vColor));
		break;
	case CCollider::COLLIDER_SPHERE:
		DX::Draw(m_pBatch, *m_pSphere, XMLoadFloat4(&m_vColor));
		break;
	}

	m_pBatch->End();

	return S_OK;
}
#endif // _DEBUG

_matrix CCollider::Remove_Rotation(_fmatrix TransformMatrix)
{
	_matrix			ResultMatrix = TransformMatrix;

	ResultMatrix.r[0] = XMVectorSet(1.f, 0.f, 0.f, 0.f) * XMVectorGetX(XMVector3Length(TransformMatrix.r[0]));
	ResultMatrix.r[1] = XMVectorSet(0.f, 1.f, 0.f, 0.f) * XMVectorGetX(XMVector3Length(TransformMatrix.r[1]));
	ResultMatrix.r[2] = XMVectorSet(0.f, 0.f, 1.f, 0.f) * XMVectorGetX(XMVector3Length(TransformMatrix.r[2]));

	return ResultMatrix;
}

_float3 CCollider::Compute_Min()
{
	return _float3(m_pAABB->Center.x - m_pAABB->Extents.x,
		m_pAABB->Center.y - m_pAABB->Extents.y,
		m_pAABB->Center.z - m_pAABB->Extents.z);
}

_float3 CCollider::Compute_Max()
{
	return _float3(m_pAABB->Center.x + m_pAABB->Extents.x,
		m_pAABB->Center.y + m_pAABB->Extents.y,
		m_pAABB->Center.z + m_pAABB->Extents.z);
}

CCollider::OBBDESC CCollider::Compute_OBBDesc()
{
	OBBDESC			OBBDesc;
	ZeroMemory(&OBBDesc, sizeof OBBDesc);

	OBBDesc.vCenter = m_pOBB->Center;

	_float3		vCorner[8];

	m_pOBB->GetCorners(vCorner);

	XMStoreFloat3(&OBBDesc.vCenterAxis[0], (XMLoadFloat3(&vCorner[2]) - XMLoadFloat3(&vCorner[3])) * 0.5f);
	XMStoreFloat3(&OBBDesc.vCenterAxis[1], (XMLoadFloat3(&vCorner[2]) - XMLoadFloat3(&vCorner[1])) * 0.5f);
	XMStoreFloat3(&OBBDesc.vCenterAxis[2], (XMLoadFloat3(&vCorner[2]) - XMLoadFloat3(&vCorner[6])) * 0.5f);

	XMStoreFloat3(&OBBDesc.vAlignAxis[0], XMVector3Normalize(XMLoadFloat3(&OBBDesc.vCenterAxis[0])));
	XMStoreFloat3(&OBBDesc.vAlignAxis[1], XMVector3Normalize(XMLoadFloat3(&OBBDesc.vCenterAxis[1])));
	XMStoreFloat3(&OBBDesc.vAlignAxis[2], XMVector3Normalize(XMLoadFloat3(&OBBDesc.vCenterAxis[2])));

	return OBBDesc;
}


CCollider * CCollider::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, COLLIDERTYPE eType)
{
	CCollider*		pInstance = new CCollider(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype(eType)))
	{
		MSG_BOX("Failed to Created : CCollider");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CComponent * CCollider::Clone(CGameObject* pOwner, void * pArg)
{
	CCollider*		pInstance = new CCollider(*this);

	if (FAILED(pInstance->Initialize_Clone(pOwner, pArg)))
	{
		MSG_BOX("Failed to Created : CCollider");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CCollider::Free()
{
	__super::Free();

	// Safe_Release(m_pOwner);

	Safe_Delete(m_pAABB_Original);
	Safe_Delete(m_pAABB);
	Safe_Delete(m_pOBB_Original);
	Safe_Delete(m_pOBB);
	Safe_Delete(m_pSphere_Original);
	Safe_Delete(m_pSphere);

#ifdef _DEBUG

	Safe_Release(m_pInputLayout);

	if (false == m_bIsCloned)
	{
		Safe_Delete(m_pBatch);
		Safe_Delete(m_pEffect);
	}
#endif // _DEBUG

}
