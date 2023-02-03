#include "stdafx.h"
#include "Light.h"
#include "Shader.h"
#include "VIBuffer_Rect.h"

CLight::CLight(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: m_pDevice(pDevice)
	, m_pContext(pContext)
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
}

HRESULT CLight::Initialize(const LIGHTDESC& LightDesc)
{
	m_LightDesc = LightDesc;
	
	_vector vPos = XMLoadFloat4(&m_LightDesc.vPosition);
	_vector vLook = XMLoadFloat4(&m_LightDesc.vDirection);
	_vector vRight = XMVector3Cross(XMVectorSet(0.f, 1.f, 0.f, 0.f), vLook);
	_vector vUp = XMVector3Cross(vLook, vRight);

	_matrix TransformMatrix = XMMatrixIdentity();
	TransformMatrix.r[0] = XMVector3Normalize(vRight);
	TransformMatrix.r[1] = XMVector3Normalize(vUp);
	TransformMatrix.r[2] = XMVector3Normalize(vLook);
	TransformMatrix.r[3] = vPos;

	XMStoreFloat4x4(&m_TransformState[D3DTS_VIEW], XMMatrixInverse(nullptr, TransformMatrix));
	XMStoreFloat4x4(&m_TransformState_TP[D3DTS_VIEW], XMMatrixTranspose(XMMatrixInverse(nullptr, TransformMatrix)));

	D3D11_VIEWPORT			ViewPortDesc;
	ZeroMemory(&ViewPortDesc, sizeof(D3D11_VIEWPORT));

	_uint		iNumViewports = 1;

	m_pContext->RSGetViewports(&iNumViewports, &ViewPortDesc);

	_matrix		ProjMatrix = XMMatrixPerspectiveFovLH(XMConvertToRadians(120.0f), ViewPortDesc.Width / ViewPortDesc.Height, 0.2f, m_LightDesc.fRange);
	//_matrix		ProjMatrix = XMMatrixPerspectiveFovLH(XMConvertToRadians(120.0f), 1280.f / 720.f, 0.2f, 1000.f);
	XMStoreFloat4x4(&m_TransformState[D3DTS_PROJ], ProjMatrix);
	XMStoreFloat4x4(&m_TransformState_TP[D3DTS_PROJ], XMMatrixTranspose(ProjMatrix));

	return S_OK;
}

HRESULT CLight::Render(CVIBuffer_Rect* pVIBuffer, CShader* pShader)
{
	_uint		iPassIndex = 1;

	if (LIGHTDESC::LIGHT_DIRECTIONAL == m_LightDesc.eType)
	{
		FAILED_CHECK_RETURN(pShader->Set_RawValue(L"g_vLightDir", &m_LightDesc.vDirection, sizeof(_float4)), E_FAIL);
		iPassIndex = 1;
	}
	else if(LIGHTDESC::LIGHT_POINT == m_LightDesc.eType)
	{
		FAILED_CHECK_RETURN(pShader->Set_RawValue(L"g_vLightPos", &m_LightDesc.vPosition, sizeof(_float4)), E_FAIL);
		FAILED_CHECK_RETURN(pShader->Set_RawValue(L"g_fLightRange", &m_LightDesc.fRange, sizeof(_float)), E_FAIL);
		iPassIndex = 2;
	}
	else if(LIGHTDESC::LIGHT_FOV == m_LightDesc.eType)
	{
		FAILED_CHECK_RETURN(pShader->Set_RawValue(L"g_vLightPos", &m_LightDesc.vPosition, sizeof(_float4)), E_FAIL);
		FAILED_CHECK_RETURN(pShader->Set_RawValue(L"g_fLightRange", &m_LightDesc.fRange, sizeof(_float)), E_FAIL);
		FAILED_CHECK_RETURN(pShader->Set_RawValue(L"g_vLightDir", &m_LightDesc.vDirection, sizeof(_float4)), E_FAIL);
		iPassIndex = 11;
	}

	FAILED_CHECK_RETURN(pShader->Set_RawValue(L"g_vLightDiffuse", &m_LightDesc.vDiffuse, sizeof(_float4)), E_FAIL);
	FAILED_CHECK_RETURN(pShader->Set_RawValue(L"g_vLightAmbient", &m_LightDesc.vAmbient, sizeof(_float4)), E_FAIL);
	FAILED_CHECK_RETURN(pShader->Set_RawValue(L"g_vLightSpecular", &m_LightDesc.vSpecular, sizeof(_float4)), E_FAIL);

	pShader->Begin(iPassIndex);
	// ¼ÎÀÌµå Å¸°Ù¿¡ ±×¸°´Ù. 
	pVIBuffer->Render();

	return S_OK;
}

void CLight::Update(_fmatrix CamWorldMatrix, _float4* vFrustumCorners)
{
	if(m_LightDesc.eType == LIGHTDESC::LIGHT_DIRECTIONAL)
	{
		_vector vCenterPos = XMVectorSet(0.f, 0.f, 0.f, 0.f);
		_vector vCorner[8];
		for(_uint i =0; i < 8; ++i)
		{
			vCorner[i] = XMVector3TransformCoord(XMLoadFloat4(&vFrustumCorners[i]), CamWorldMatrix);
			vCenterPos += vCorner[i];
		}
		vCenterPos /= 8.0f;
		float fRadius = 0.0f;

		for(_uint i =0; i < 8; ++i)
		{
			float fDistance = XMVectorGetX(XMVector3Length(vCorner[i] - vCenterPos));
			fRadius = (fRadius < fDistance) ? fDistance : fRadius;
		}

		_vector vMaxExtents = XMVectorSet(fRadius, fRadius, fRadius, 0.f);
		_vector vMinExtents = -vMaxExtents;

		XMStoreFloat4(&m_LightDesc.vPosition, vCenterPos + XMLoadFloat4(&m_LightDesc.vDirection) * XMVectorGetZ(vMinExtents));

		_vector vPos = XMLoadFloat4(&m_LightDesc.vPosition);
		_vector vLook = XMLoadFloat4(&m_LightDesc.vDirection);
		_vector vRight = XMVector3Cross(XMVectorSet(0.f, 1.f, 0.f, 0.f), vLook);
		_vector vUp = XMVector3Cross(vLook, vRight);

		_matrix TransformMatrix = XMMatrixIdentity();
		TransformMatrix.r[0] = XMVector3Normalize(vRight);
		TransformMatrix.r[1] = XMVector3Normalize(vUp);
		TransformMatrix.r[2] = XMVector3Normalize(vLook);
		TransformMatrix.r[3] = vPos;
		XMStoreFloat4x4(&m_TransformState[D3DTS_VIEW], XMMatrixInverse(nullptr, TransformMatrix));
		XMStoreFloat4x4(&m_TransformState_TP[D3DTS_VIEW], XMMatrixTranspose(XMMatrixInverse(nullptr, TransformMatrix)));

		_vector vCascadeExtents = vMaxExtents - vMinExtents;
		_matrix ProjMatrix = XMMatrixOrthographicLH(XMVectorGetX(vCascadeExtents), XMVectorGetY(vCascadeExtents), 0.f, XMVectorGetZ(vCascadeExtents));

		XMStoreFloat4x4(&m_TransformState[D3DTS_PROJ], ProjMatrix);
		XMStoreFloat4x4(&m_TransformState_TP[D3DTS_PROJ], XMMatrixTranspose(ProjMatrix));
	}
}

CLight* CLight::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const LIGHTDESC& LightDesc)
{
	CLight*		pInst = new CLight(pDevice, pContext);

	if(FAILED(pInst->Initialize(LightDesc)))
	{
		MSG_BOX("Failed to Create : CLight");
		Safe_Release(pInst);
	}

	return pInst;
}

void CLight::Free()
{
	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);
}
