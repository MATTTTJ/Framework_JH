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

HRESULT CTransform::Initialize_Clone(void * pArg)
{
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

void CTransform::Go_Straight(_double TimeDelta)
{
	_vector	vPosition = Get_State(CTransform::STATE_TRANSLATION);
	_vector	vLook = Get_State(CTransform::STATE_LOOK);

	/* 이렇게 얻어온 VlOOK은 Z축 스케일을 포함하낟. */
	vPosition += XMVector3Normalize(vLook) * m_TransformDesc.fSpeedPerSec * TimeDelta;

	Set_State(CTransform::STATE_TRANSLATION, vPosition);
}

void CTransform::Go_Backward(_double TimeDelta)
{
	_vector	vPosition = Get_State(CTransform::STATE_TRANSLATION);
	_vector	vLook = Get_State(CTransform::STATE_LOOK);

	/* 이렇게 얻어온 VlOOK은 Z축 스케일을 포함하낟. */
	vPosition -= XMVector3Normalize(vLook) * m_TransformDesc.fSpeedPerSec * TimeDelta;

	Set_State(CTransform::STATE_TRANSLATION, vPosition);
}

void CTransform::Go_Left(_double TimeDelta)
{
	_vector	vPosition = Get_State(CTransform::STATE_TRANSLATION);
	_vector	vRight = Get_State(CTransform::STATE_RIGHT);

	/* 이렇게 얻어온 VlOOK은 Z축 스케일을 포함하낟. */
	vPosition -= XMVector3Normalize(vRight) * m_TransformDesc.fSpeedPerSec * TimeDelta;

	Set_State(CTransform::STATE_TRANSLATION, vPosition);
}

void CTransform::Go_Right(_double TimeDelta)
{
	_vector	vPosition = Get_State(CTransform::STATE_TRANSLATION);
	_vector	vRight = Get_State(CTransform::STATE_RIGHT);

	/* 이렇게 얻어온 VlOOK은 Z축 스케일을 포함하낟. */
	vPosition += XMVector3Normalize(vRight) * m_TransformDesc.fSpeedPerSec * TimeDelta;

	Set_State(CTransform::STATE_TRANSLATION, vPosition);
}

void CTransform::Turn(_fvector vAxis, _double TimeDelta)
{
	_matrix		RotationMatrix = XMMatrixRotationAxis(vAxis, m_TransformDesc.fRotationPerSec * TimeDelta);

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

	_vector		vRight = XMVectorSet(1.f, 0.f, 0.f, 0.f);
	_vector		vUp = XMVectorSet(0.f, 1.f, 0.f, 0.f);
	_vector		vLook = XMVectorSet(0.f, 0.f, 1.f, 0.f);

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
		vPosition += XMVector3Normalize(vDir) * m_TransformDesc.fSpeedPerSec * TimeDelta;
		Set_State(CTransform::STATE_TRANSLATION, vPosition);
	}
}

void CTransform::Speed_Up(_bool bKeyState)
{
	if (bKeyState)
		m_TransformDesc.fSpeedPerSec = m_dInitSpeed * 5.0;
	else
		m_TransformDesc.fSpeedPerSec = m_dInitSpeed;
}

HRESULT CTransform::Bind_ShaderResource(CShader* pShaderCom, const wstring&  pConstantName)
{
	NULL_CHECK_RETURN(pShaderCom, E_FAIL);

	return pShaderCom->Set_Matrix(pConstantName, &m_WorldMatrix);
}

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

CComponent * CTransform::Clone(void * pArg)
{
	CTransform*		pInstance = new CTransform(*this);

	if (FAILED(pInstance->Initialize_Clone(pArg)))
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
