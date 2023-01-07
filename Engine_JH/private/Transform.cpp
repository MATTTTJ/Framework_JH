#include "stdafx.h"
#include "..\public\Transform.h"
#include "Shader.h"
#include "GameInstance.h"

CTransform::CTransform(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CComponent(pDevice, pContext)
{

}

CTransform::CTransform(const CTransform & rhs)
	: CComponent(rhs)
	, m_WorldMatrix(rhs.m_WorldMatrix)
{

}

void CTransform::FinalUpdate()
{
	if (m_pParent == nullptr)
		return;

	_matrix  matParent = m_pParent->Get_WorldMatrix();
	_matrix  matScaleSet = XMMatrixIdentity();
	_vector vRight, vUp, vLook, vTranslate;

	memcpy(&vRight, &matParent.r[0], sizeof(_vector));
	memcpy(&vUp, &matParent.r[1], sizeof(_vector));
	memcpy(&vLook, &matParent.r[2], sizeof(_vector));
	memcpy(&vTranslate, &matParent.r[3], sizeof(_vector));

	memcpy(&matScaleSet.r[0], &XMVector3Normalize(vRight), sizeof(_vector));
	memcpy(&matScaleSet.r[1], &XMVector3Normalize(vUp), sizeof(_vector));
	memcpy(&matScaleSet.r[2], &XMVector3Normalize(vLook), sizeof(_vector));
	memcpy(&matScaleSet.r[3], &vTranslate, sizeof(_vector));

	XMStoreFloat4x4(&m_WorldWithParentMatrix, XMLoadFloat4x4(&m_WorldMatrix) * matScaleSet);
}

void CTransform::Set_Scaled(STATE eState, _float fScale)
{
	if (eState == STATE_TRANSLATION)
		return;

	_vector		vState = Get_State(eState);

	vState = XMVector3Normalize(vState) * fScale;

	Set_State(eState, vState);
}

void CTransform::Set_Scaled(_float3 vScale)
{
	Set_State(STATE_RIGHT, XMVector3Normalize(Get_State(STATE_RIGHT)) * vScale.x);
	Set_State(STATE_UP, XMVector3Normalize(Get_State(STATE_UP)) * vScale.y);
	Set_State(STATE_LOOK, XMVector3Normalize(Get_State(STATE_LOOK)) * vScale.z);
}

void CTransform::Scaling(STATE eState, _float fScale)
{
	if (eState == STATE_TRANSLATION)
		return;

	Set_State(eState, Get_State(eState) * fScale);
}

HRESULT CTransform::Initialize_Prototype()
{
	XMStoreFloat4x4(&m_WorldMatrix, XMMatrixIdentity());

	return S_OK;
}

HRESULT CTransform::Initialize_Clone(CGameObject* pOwner, void * pArg)
{
	FAILED_CHECK_RETURN(__super::Initialize_Clone(pOwner, pArg), E_FAIL);

	if (nullptr != pArg)
		memcpy(&m_TransformDesc, pArg, sizeof(TRANSFORMDESC));

	m_dInitSpeed = m_TransformDesc.fSpeedPerSec;

	return S_OK;
}

void CTransform::Imgui_RenderProperty()
{
	ImGuizmo::BeginFrame();

	static ImGuizmo::OPERATION CurGuizmoType(ImGuizmo::TRANSLATE);

	ImGui::BulletText("ImGuizmo Type");
	if (ImGui::RadioButton("Translate", CurGuizmoType == ImGuizmo::TRANSLATE))
		CurGuizmoType = ImGuizmo::TRANSLATE;
	ImGui::SameLine();
	if (ImGui::RadioButton("Scale", CurGuizmoType == ImGuizmo::SCALE))
		CurGuizmoType = ImGuizmo::SCALE;
	ImGui::SameLine();
	if (ImGui::RadioButton("Rotate", CurGuizmoType == ImGuizmo::ROTATE))
		CurGuizmoType = ImGuizmo::ROTATE;

	_float	vPos[3], vScale[3], vAngle[3];
	ImGuizmo::DecomposeMatrixToComponents((_float*)&m_WorldMatrix, vPos, vAngle, vScale);
	IMGUI_LEFT_LABEL(ImGui::InputFloat3, "Translate", vPos);
	IMGUI_LEFT_LABEL(ImGui::InputFloat3, "Scale", vScale);
	IMGUI_LEFT_LABEL(ImGui::InputFloat3, "Rotate", vAngle);
	ImGuizmo::RecomposeMatrixFromComponents(vPos, vAngle, vScale, (_float*)&m_WorldMatrix);

	ImGuiIO&	io = ImGui::GetIO();
	RECT		rt;
	GetClientRect(CGameInstance::GetInstance()->GetHWND(), &rt);
	POINT		LT{ rt.left, rt.top };
	ClientToScreen(CGameInstance::GetInstance()->GetHWND(), &LT);
	ImGuizmo::SetRect((_float)LT.x, (_float)LT.y, io.DisplaySize.x, io.DisplaySize.y);

	_float4x4		matView, matProj;
	XMStoreFloat4x4(&matView, CGameInstance::GetInstance()->Get_TransformMatrix(CPipeLine::D3DTS_VIEW));
	XMStoreFloat4x4(&matProj, CGameInstance::GetInstance()->Get_TransformMatrix(CPipeLine::D3DTS_PROJ));

	ImGuizmo::Manipulate((_float*)&matView, (_float*)&matProj, CurGuizmoType, ImGuizmo::WORLD, (_float*)&m_WorldMatrix);
}

void CTransform::Go_Straight(_double TimeDelta, TRANSTYPE eType, CNavigation* pNaviCom)
{


	_vector	vPosition = Get_State(CTransform::STATE_TRANSLATION);
	_float4	vLook = Get_State(CTransform::STATE_LOOK);

	_vector vMovePos = vPosition + XMVector3Normalize(vLook)* m_TransformDesc.fSpeedPerSec * (_float)TimeDelta;
	// vPosition += XMVector3Normalize(vLook) * m_TransformDesc.fSpeedPerSec * (_float)TimeDelta;
	if (nullptr == pNaviCom)
	{
		Set_State(CTransform::STATE_TRANSLATION, vPosition);
	}
	else
	{
		_float4 vBlockedLine = { 0.f, 0.f, 0.f, 0.f };
		_float4 vBlockedLineNormal = { 0.f ,0.f, 0.f, 0.f };

		if (true == pNaviCom->IsMove_OnNavigation(vMovePos, vBlockedLine, vBlockedLineNormal))
			Set_State(CTransform::STATE_TRANSLATION, vMovePos);
		else
		{
			// TODO :: 셀의 평균 높이가 플레이어보다 일정 수치가 높으면
			// if(pNaviCom->Is_OverHeight())
			// {
			// 	Set_State(CTransform::STATE_TRANSLATION, vPosition);
			// }
			// else
			// {
			_vector vInDir = vMovePos - vPosition;
			_vector vOutDir = vPosition - vMovePos;
			_float	fLength = XMVectorGetX(XMVector3Dot(vOutDir, vBlockedLineNormal));

			_vector vSlidingDir = vInDir + XMLoadFloat4(&vBlockedLineNormal) * fLength;

			vMovePos = vPosition + vSlidingDir;

			if (pNaviCom->IsMove_OnNavigation(vMovePos, vBlockedLine, vBlockedLineNormal))
			{
				Set_State(CTransform::STATE_TRANSLATION, vMovePos);
			}
		}
	}
	// }

	if (TRANS_BULLET == eType)
	{
		Set_State(CTransform::STATE_TRANSLATION, vMovePos);
	}




}

void CTransform::Go_Backward(_double TimeDelta)
{
	_vector	vPosition = Get_State(CTransform::STATE_TRANSLATION);
	_vector	vLook =
		Get_State(CTransform::STATE_LOOK);

	/* 이렇게 얻어온 VlOOK은 Z축 스케일을 포함하낟. */
	vPosition -= XMVector3Normalize(vLook) * m_TransformDesc.fSpeedPerSec * (_float)TimeDelta;
	Set_State(CTransform::STATE_TRANSLATION, vPosition);
}

void CTransform::Go_Left(_double TimeDelta)
{
	_vector	vPosition = Get_State(CTransform::STATE_TRANSLATION);
	_vector	vRight = Get_State(CTransform::STATE_RIGHT);

	/* 이렇게 얻어온 VlOOK은 Z축 스케일을 포함하낟. */
	vPosition -= XMVector3Normalize(vRight) * m_TransformDesc.fSpeedPerSec * (_float)TimeDelta;

	Set_State(CTransform::STATE_TRANSLATION, vPosition);
}

void CTransform::Go_Right(_double TimeDelta)
{
	_vector	vPosition = Get_State(CTransform::STATE_TRANSLATION);
	_vector	vRight = Get_State(CTransform::STATE_RIGHT);

	/* 이렇게 얻어온 VlOOK은 Z축 스케일을 포함하낟. */
	vPosition += XMVector3Normalize(vRight) * m_TransformDesc.fSpeedPerSec * (_float)TimeDelta;

	Set_State(CTransform::STATE_TRANSLATION, vPosition);


}

void CTransform::Turn(_fvector vAxis, _double TimeDelta)
{
	_matrix		RotationMatrix = XMMatrixRotationAxis(vAxis, m_TransformDesc.fRotationPerSec * (_float)TimeDelta);

	_vector		vRight = Get_State(CTransform::STATE_RIGHT);
	_vector		vUp = Get_State(CTransform::STATE_UP);
	_vector		vLook = Get_State(CTransform::STATE_LOOK);

	Set_State(CTransform::STATE_RIGHT, XMVector4Transform(vRight, RotationMatrix));
	Set_State(CTransform::STATE_UP, XMVector4Transform(vUp, RotationMatrix));
	Set_State(CTransform::STATE_LOOK, XMVector4Transform(vLook, RotationMatrix));
}

void CTransform::Rotation(_fvector vAxis, _float fRadian)
{
	_matrix		RotationMatrix = XMMatrixRotationAxis(vAxis, fRadian);

	_float3		vScale = Get_Scaled();

	_vector		vRight = XMVectorSet(1.f, 0.f, 0.f, 0.f) * vScale.x;
	_vector		vUp = XMVectorSet(0.f, 1.f, 0.f, 0.f) * vScale.y;
	_vector		vLook = XMVectorSet(0.f, 0.f, 1.f, 0.f) * vScale.z;

	Set_State(CTransform::STATE_RIGHT, XMVector4Transform(vRight, RotationMatrix));
	Set_State(CTransform::STATE_UP, XMVector4Transform(vUp, RotationMatrix));
	Set_State(CTransform::STATE_LOOK, XMVector4Transform(vLook, RotationMatrix));
}

void CTransform::LookAt(_fvector vTargetPos)
{
	_float3		vScale = Get_Scaled();

	_vector		vLook = XMVector3Normalize(vTargetPos - Get_State(CTransform::STATE_TRANSLATION)) * vScale.z;
	_vector		vRight = XMVector3Normalize(XMVector3Cross(XMVectorSet(0.f, 1.f, 0.f, 0.f), vLook)) * vScale.x;
	_vector		vUp = XMVector3Normalize(XMVector3Cross(vLook, vRight)) * vScale.y;

	Set_State(CTransform::STATE_RIGHT, vRight);
	Set_State(CTransform::STATE_UP, vUp);
	Set_State(CTransform::STATE_LOOK, vLook);
}

void CTransform::Chase(_fvector vTargetPos, _double TimeDelta, _float fLimit)
{
	_vector		vPosition = Get_State(CTransform::STATE_TRANSLATION);
	_vector		vDir = vTargetPos - vPosition;

	_float		fDistance = XMVectorGetX(XMVector3Length(vDir));

	if (fDistance > fLimit)
	{
		vPosition += XMVector3Normalize(vDir) * m_TransformDesc.fSpeedPerSec * (_float)TimeDelta;
		Set_State(CTransform::STATE_TRANSLATION, vPosition);
	}
}

void CTransform::Jump(_double dTimeDelta, _float& fGravity, _float& fCurJumpSpeed)
{
	_float4	vPos = Get_State(CTransform::STATE_TRANSLATION);

	vPos.y += fCurJumpSpeed * (_float)m_TransformDesc.fSpeedPerSec * (_float)dTimeDelta;

	Set_State(CTransform::STATE_TRANSLATION, vPos);

	fCurJumpSpeed -= fGravity;
}

void CTransform::Dash(_double dTimeDelta, _float& fFriction, _float& fCurDashTickCount, _fmatrix matCamWorld,
	DIRECTION eDir)
{
	if (fCurDashTickCount < 0.f)
		return;

	_float4	vPos;
	XMStoreFloat4(&vPos, Get_State(CTransform::STATE_TRANSLATION));
	_float4	vLook;
	XMStoreFloat4(&vLook, Get_State(CTransform::STATE_LOOK));
	_float3	vScale = Get_Scaled();

	_float4	vDir;

	if (eDir != DIR_END)
	{
		switch (eDir)
		{
		case DIR_W:
			vDir = XMVector3Normalize(matCamWorld.r[2]);
			break;

		case DIR_A:
			vDir = XMVector3Normalize(matCamWorld.r[0]) * -1.f;
			break;

		case DIR_S:
			vDir = XMVector3Normalize(matCamWorld.r[2]) * -1.f;
			break;

		case DIR_D:
			vDir = XMVector3Normalize(matCamWorld.r[0]);
			break;

		case DIR_WA:
			vDir = XMVector3Normalize(matCamWorld.r[2] + matCamWorld.r[0] * -1.f);
			break;

		case DIR_WD:
			vDir = XMVector3Normalize(matCamWorld.r[2] + matCamWorld.r[0]);
			break;

		case DIR_SA:
			vDir = XMVector3Normalize(matCamWorld.r[2] * -1.f + matCamWorld.r[0] * -1.f);
			break;

		case DIR_SD:
			vDir = XMVector3Normalize(matCamWorld.r[2] * -1.f + matCamWorld.r[0]);
			break;
		}

		vLook = XMVector3Normalize(XMVectorSet(vDir.x, 0.f, vDir.z, 0.f)) * vScale.z;
		_vector	vRight = XMVector3Normalize(XMVector3Cross(XMVectorSet(0.f, 1.f, 0.f, 0.f), vLook)) * vScale.x;
		_vector	vUp = XMVector3Normalize(XMVector3Cross(vLook, vRight)) * vScale.y;

		Set_State(STATE_RIGHT, vRight);
		Set_State(STATE_UP, vUp);
		Set_State(STATE_LOOK, vLook);
	}

	vPos += XMVector3Normalize(vLook) * (_float)dTimeDelta * (_float)m_TransformDesc.fSpeedPerSec * 2.f;
	Set_State(STATE_TRANSLATION, vPos);
}

void CTransform::Speed_Up(_bool bKeyState)
{
	if (bKeyState)
		m_TransformDesc.fSpeedPerSec = (_float)(m_dInitSpeed * 5.0);
	else
		m_TransformDesc.fSpeedPerSec = (_float)m_dInitSpeed;
}

HRESULT CTransform::Bind_ShaderResource(CShader* pShaderCom, const wstring&  pConstantName)
{
	NULL_CHECK_RETURN(pShaderCom, E_FAIL);

	return pShaderCom->Set_Matrix(pConstantName, &m_WorldMatrix);
}

// void CTransform::Go_Straight(_double TimeDelta, CNavigation* pNaviCom)
// {
// 	_vector	vPosition = Get_State(CTransform::STATE_TRANSLATION);
// 	_vector	vLook = Get_State(CTransform::STATE_LOOK);
//
// 	/* 이렇게 얻어온 VlOOK은 Z축 스케일을 포함하낟. */
// 	vPosition += XMVector3Normalize(vLook) * m_TransformDesc.fSpeedPerSec * TimeDelta;
//
// 	if (nullptr == pNaviCom)
// 		Set_State(CTransform::STATE_TRANSLATION, vPosition);
//
// 	else
// 	{
// 		if (true == pNaviCom->IsMove_OnNavigation(vPosition))
// 			Set_State(CTransform::STATE_TRANSLATION, vPosition);
// 	}
// }


CTransform * CTransform::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CTransform*		pInstance = new CTransform(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CTransform");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CComponent * CTransform::Clone(CGameObject* pOwner, void * pArg)
{
	CTransform*		pInstance = new CTransform(*this);

	if (FAILED(pInstance->Initialize_Clone(pOwner, pArg)))
	{
		MSG_BOX("Failed to Cloned : CTransform");
		Safe_Release(pInstance);
	}

	return pInstance;
}


void CTransform::Free()
{
	__super::Free();


}
