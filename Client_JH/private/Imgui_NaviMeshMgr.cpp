#include "stdafx.h"
#include "..\public\Imgui_NaviMeshMgr.h"
#include "GameObject.h"
#include "GameInstance.h"
#include "PickingCube.h"


CImgui_NaviMeshMgr::CImgui_NaviMeshMgr(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: m_pDevice(pDevice)
	, m_pContext(pContext)
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
}

HRESULT CImgui_NaviMeshMgr::Initialize(void* pArg)
{
	m_szWindowName = "NaviMesh Manager";
	m_pGameInstance = CGameInstance::GetInstance();
	return S_OK;
}

void CImgui_NaviMeshMgr::Imgui_RenderWindow()
{
	ImGui::BeginTabBar("NaviMesh Manager");
	Picking_On_Mesh();
	// GameObject_Editor();
	// CloneObject_Editor();
	ImGui::EndTabBar();
}

void CImgui_NaviMeshMgr::Picking_On_Mesh()
{
	POINT      ptMouse{};

	GetCursorPos(&ptMouse);
	ScreenToClient(g_hWnd, &ptMouse);

	_float3         vMousePos;

	vMousePos.x = _float(ptMouse.x / (g_iWinSizeX * 0.5f) - 1);
	vMousePos.y = _float(ptMouse.y / (g_iWinSizeY * -0.5f) + 1);
	vMousePos.z = 0.f;

	_vector   vecMousePos = XMLoadFloat3(&vMousePos);
	vecMousePos = XMVectorSetW(vecMousePos, 1.f);

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	_matrix     ProjMatrix, ProjMatrixInv;
	ProjMatrix = pGameInstance->Get_TransformMatrix(CPipeLine::D3DTS_PROJ);
	ProjMatrixInv = XMMatrixInverse(nullptr, ProjMatrix);
	vecMousePos = XMVector3TransformCoord(vecMousePos, ProjMatrixInv);

	_vector   vRayDir, vRayPos;

	vRayPos = { 0.f, 0.f, 0.f , 1.f };
	vRayDir = vecMousePos - vRayPos;

	_matrix    ViewMatrix, ViewMatrixInv;
	ViewMatrix = pGameInstance->Get_TransformMatrix(CPipeLine::D3DTS_VIEW);
	ViewMatrixInv = XMMatrixInverse(nullptr, ViewMatrix);
	vRayPos = XMVector3TransformCoord(vRayPos, ViewMatrixInv);
	vRayDir = XMVector3TransformNormal(vRayDir, ViewMatrixInv);

	vRayPos = XMVector3TransformCoord(vRayPos, m_pTransformCom->Get_WorldMatrix_Inverse());
	vRayDir = XMVector3TransformNormal(vRayDir, m_pTransformCom->Get_WorldMatrix_Inverse());
	vRayDir = XMVector3Normalize(vRayDir);

	_float4 vCamPos = pGameInstance->Get_CamPosition();

	RELEASE_INSTANCE(CGameInstance);

	const _uint NumMeshes = m_pModelCom->Get_NumMeshes();

	_float   fMinDist = 1000.f; // �������� ��ŷ���� ��, ���� ����� ���� ����
	_vector   vPickingPos[3]{ 0.f, 0.f, 0.f };
	_bool IsPick = false;

	for (_uint i = 0; i < NumMeshes; ++i)
	{
		_float fDist = 0.f;

		const vector<CMesh*> Meshes = m_pModelCom->Get_Meshes();
		VTXMODEL* pVtxModel = Meshes[i]->Get_NonAnimVertices();
		//���� ���۷���

		FACEINDICES32* pIndices = Meshes[i]->Get_Indices();
		const _uint   iNumPrimitive = Meshes[i]->Get_NumPrimitive();

		for (_uint j = 0; j < iNumPrimitive; ++j)
		{

			//����ȭ - ī�޶� �޽��� ������������ ��ŷ�� �� �ȸԴ´ٸ� �Ʒ� 3�� �ּ�.
			//�� ���� ���°Ͱ����� ���� �̼��ϰ� ��������..
			_vector vDir = XMLoadFloat4(&vCamPos) - XMLoadFloat3(&pVtxModel[pIndices[j]._0].vPosition);

			if (0 > XMVectorGetX(XMVector3Dot(XMVector3Normalize(vDir), XMVector3Normalize(XMLoadFloat3(&pVtxModel[pIndices[j]._0].vNormal)))))
				continue;


			if (TriangleTests::Intersects(vRayPos, vRayDir,
				XMLoadFloat3(&pVtxModel[pIndices[j]._0].vPosition),
				XMLoadFloat3(&pVtxModel[pIndices[j]._1].vPosition),
				XMLoadFloat3(&pVtxModel[pIndices[j]._2].vPosition),
				fDist))
			{
				if (fMinDist > fDist)
				{
					fMinDist = fDist;
					vPickingPos[0] = XMLoadFloat3(&pVtxModel[pIndices[j]._0].vPosition);
					vPickingPos[1] = XMLoadFloat3(&pVtxModel[pIndices[j]._1].vPosition);
					vPickingPos[2] = XMLoadFloat3(&pVtxModel[pIndices[j]._2].vPosition);
					IsPick = true;
				}
			}
		}
	}

	_matrix WVPMatrix = XMLoadFloat4x4(&m_pTransformCom->Get_WorldMatrix()) * ViewMatrix * ProjMatrix;

	//���� 3���� ��հ��� �ְ�����
	vPickingPos[0] = XMVector3TransformNormal(vPickingPos[0], WVPMatrix);
	vPickingPos[1] = XMVector3TransformNormal(vPickingPos[1], WVPMatrix);
	vPickingPos[2] = XMVector3TransformNormal(vPickingPos[2], WVPMatrix);

	_vector PickFinalPos = (vPickingPos[0] + vPickingPos[1] + vPickingPos[2]) / 3.f;
	XMStoreFloat3(&vPickPos, PickFinalPos);

	return IsPick;

}

void CImgui_NaviMeshMgr::Cell_Sorting()
{
}

CImgui_NaviMeshMgr* CImgui_NaviMeshMgr::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, void* pArg)
{
}

void CImgui_NaviMeshMgr::Free()
{
	CImguiObject::Free();
}
