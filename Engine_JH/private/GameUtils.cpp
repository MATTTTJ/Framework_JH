#include "stdafx.h"
#include "..\public\GameUtils.h"
#include "Cell.h"
#include "GameInstance.h"


void CGameUtils::wc2c(const wchar_t* szWchar, char* szChar)
{
	_int len;
	_int slength = lstrlen(szWchar) + 1;
	len = ::WideCharToMultiByte(CP_ACP, 0, szWchar, slength, 0, 0, 0, 0);
	::WideCharToMultiByte(CP_ACP, 0, szWchar, slength, szChar, len, 0, 0);
}
void CGameUtils::ctwc(const char * szChar, wchar_t * wszChar)
{
	_int iLength = (_int)strlen(szChar) + 1;
	_int slength;
	slength = ::MultiByteToWideChar(CP_ACP, 0, szChar, -1, 0, 0);
	::MultiByteToWideChar(CP_ACP, MB_COMPOSITE, szChar, iLength, wszChar, slength);
}

void CGameUtils::SplitTag(wchar_t * wszTag, wstring & wszChar)
{
	for (_int i = 0; i < lstrlen(wszTag); ++i)
	{
		wszChar += wszTag[i];

		wchar_t wszTemp[2] = { wszTag[i], L'\0' };

		if (!lstrcmp(wszTemp, L"_"))
			wszChar = L"";
	}
}

void CGameUtils::ConvertNumberTo_d(char * szChar)
{
	_int	iLastWord = (_int)strlen(szChar) - 1;

	if (isdigit(szChar[iLastWord]))
	{
		if (isdigit(szChar[iLastWord - 1]))
		{
			szChar[iLastWord - 1] = '%';
			szChar[iLastWord] = 'd';
		}
		else
		{
			szChar[iLastWord] = '%';
			szChar[iLastWord + 1] = 'd';
			szChar[iLastWord + 2] = '\0';
		}
	}
}

COMPONENTTYPE CGameUtils::CheckComponentTypeFromTag(const wstring & wstrComponentTag)
{
	COMPONENTTYPE	eType = COMPONENTTYPE_END;
	wstring				wstrTemp = L"";

	for (size_t i = 0; i < wstrComponentTag.length() + 1; ++i)
	{
		wstrTemp += wstrComponentTag[i];

		wchar_t wszTemp = L'_';

		if (wstrTemp.back() == L'_')
		{
			wstrTemp = L"";
			continue;
		}

		if (wstrTemp == L"<None>")
			break;
		if (wstrTemp == L"Renderer")
		{
			eType = COM_RENDERER;
			break;
		}
		if (wstrTemp == L"VIBuffer")
		{
			eType = COM_VIBUFFER;
			break;
		}
		if (wstrTemp == L"Shader")
		{
			eType = COM_SHADER;
			break;
		}
		if (wstrTemp == L"Transform")
		{
			eType = COM_TRANSFORM;
			break;
		}
		if (wstrTemp == L"Texture")
		{
			eType = COM_TEXTURE;
			break;
		}
		if (wstrTemp == L"Model")
		{
			eType = COM_MODEL;
			break;
		}
		if (wstrTemp == L"Collider")
		{
			eType = COM_COLLIDER;
			break;
		}
	}

	return eType;
}


string CGameUtils::wstrTostr(const wstring& wstrSour)
{
	string		strDest = "";

	if (wstrSour != L"")
		strDest.assign(wstrSour.begin(), wstrSour.end());

	return strDest;
}

wstring CGameUtils::s2ws(const string& strSour)
{
	wstring wstrDest = L"";

	if(strSour != "")
	{
		wstrDest.assign(strSour.begin(), strSour.end());
	}

	return wstrDest;
}

void CGameUtils::Saturate(int & InValue, int InMax, int InMin)
{
	if (InValue > InMax)
		InValue = InMin;
	if (InValue < InMin)
		InValue = InMax;
}

void CGameUtils::SaturateF(float& InValue, float InMax, float InMin)
{
	if (fabs(InValue - InMax) < EPSILON)
		InValue = InMax;
	if (fabs(InValue - InMin) < EPSILON)
		InValue = InMin;
}

void CGameUtils::DecomposePitchYawRollFromXMMatrix(_fmatrix matRotation, _float& fPitch, _float& fYaw, _float& fRoll)
{
	_float4x4		matPYR;

	XMStoreFloat4x4(&matPYR, XMMatrixTranspose(matRotation));
	fPitch = asinf(-matPYR._23);
	fYaw = atan2f(matPYR._13, matPYR._33);
	fRoll = atan2f(matPYR._21, matPYR._22);
}

pair<_bool, _float> CGameUtils::Picking(HWND& hWnd, _float fScreenWidth, _float fScreenHeight,
	CTransform* pTransformCom, _float3* pVertexPos, _uint iNumVerticesX, _uint iNumVerticesZ, _float3& vPickingPoint)
{
	POINT		pt;
	GetCursorPos(&pt);
	ScreenToClient(hWnd, &pt);

	/* ViewPort -> Projection */
	_float3	vViewportPoint;
	vViewportPoint.x = (_float)pt.x / (fScreenWidth * 0.5f) - 1.f;
	vViewportPoint.y = (_float)pt.y / (fScreenHeight * -0.5f) + 1.f;
	vViewportPoint.z = 0.f;

	/* Now in Projection Space */
	_vector	vMousePoint = XMLoadFloat3(&vViewportPoint);
	vMousePoint = XMVectorSetW(vMousePoint, 1.f);

	/* Projection -> View */
	_matrix	matProjInv = CGameInstance::GetInstance()->Get_TransformMatrix_Inverse(CPipeLine::D3DTS_PROJ);

	vMousePoint = XMVector3TransformCoord(vMousePoint, matProjInv);

	/* Now in View Space */
	_vector	vRayPos = vMousePoint;
	_vector	vRayDir = (vMousePoint - vRayPos);

	/* View -> World */
	_matrix	matViewInv = CGameInstance::GetInstance()->Get_TransformMatrix_Inverse(CPipeLine::D3DTS_VIEW);

	vRayPos = XMVector3TransformCoord(vRayPos, matViewInv);
	vRayDir = (XMVector3TransformNormal(vRayDir, matViewInv));

	/* World -> Local */
	_matrix	matWorldInv = pTransformCom->Get_WorldMatrix_Inverse();

	vRayPos = XMVector3TransformCoord(vRayPos, matWorldInv);
	vRayDir = XMVector3Normalize(XMVector3TransformNormal(vRayDir, matWorldInv));

	/* Now in Local Space */
	_float		fDist = 0.f;
	pair<_bool, _float>	Result{ false, 100000.f };

	for (_uint z = 0; z < iNumVerticesZ - 1; ++z)
	{
		for (_uint x = 0; x < iNumVerticesX - 1; ++x)
		{
			_uint		iIndex = z * iNumVerticesX + x;

			_uint		iIndices[4] = {
				iIndex + iNumVerticesX,
				iIndex + iNumVerticesX + 1,
				iIndex + 1,
				iIndex
			};

			fDist = 0.f;

			if (TriangleTests::Intersects(vRayPos, vRayDir, pVertexPos[iIndices[0]], pVertexPos[iIndices[1]], pVertexPos[iIndices[2]], fDist))
			{
				if (Result.second > fDist)
				{
					vPickingPoint = XMVector3TransformCoord(vRayPos + vRayDir * fDist, pTransformCom->Get_WorldMatrix());
					Result.first = true;
					Result.second = fDist;
				}
			}

			fDist = 0.f;

			if (TriangleTests::Intersects(vRayPos, vRayDir, pVertexPos[iIndices[0]], pVertexPos[iIndices[2]], pVertexPos[iIndices[3]], fDist))
			{
				if (Result.second > fDist)
				{
					vPickingPoint = XMVector3TransformCoord(vRayPos + vRayDir * fDist, pTransformCom->Get_WorldMatrix());
					Result.first = true;
					Result.second = fDist;
				}
			}
		}
	}

	return Result;
}

pair<_bool, _float> CGameUtils::Picking(HWND& hWnd, _float fScreenWidth, _float fScreenHeight,
	CTransform* pTransformCom, VTXMODEL* pVertices, FACEINDICES32* pIndices, _uint iNumPrimitive,
	_float3& vPickingPoint)
{
	POINT		pt;
	GetCursorPos(&pt);
	ScreenToClient(hWnd, &pt);

	/* ViewPort -> Projection */
	_float3	vViewportPoint;
	vViewportPoint.x = (_float)pt.x / (fScreenWidth * 0.5f) - 1.f;
	vViewportPoint.y = (_float)pt.y / (fScreenHeight * -0.5f) + 1.f;
	vViewportPoint.z = 0.f;

	/* Now in Projection Space */
	_vector	vMousePoint = XMLoadFloat3(&vViewportPoint);
	vMousePoint = XMVectorSetW(vMousePoint, 1.f);

	/* Projection -> View */
	_matrix	matProjInv = CGameInstance::GetInstance()->Get_TransformMatrix_Inverse(CPipeLine::D3DTS_PROJ);

	vMousePoint = XMVector3TransformCoord(vMousePoint, matProjInv);

	/* Now in View Space */
	_vector	vRayPos = XMVectorSet(0.f, 0.f, 0.f, 1.f);
	_vector	vRayDir = (vMousePoint - vRayPos);

	/* View -> World */
	_matrix	matViewInv = CGameInstance::GetInstance()->Get_TransformMatrix_Inverse(CPipeLine::D3DTS_VIEW);

	vRayPos = XMVector3TransformCoord(vRayPos, matViewInv);
	vRayDir = (XMVector3TransformNormal(vRayDir, matViewInv));

	/* World -> Local */
	_matrix	matWorldInv = pTransformCom->Get_WorldMatrix_Inverse();

	vRayPos = XMVector3TransformCoord(vRayPos, matWorldInv);
	vRayDir = XMVector3Normalize(XMVector3TransformNormal(vRayDir, matWorldInv));

	/* Now in Local Space */
	_float4	vCameraPos = CGameInstance::GetInstance()->Get_CamPos();
	_vector	vPickingPos{ 0.f, 0.f, 0.f, 1.f };
	_float		fDist = 0.f;
	pair<_bool, _float>	Result{ false, 100000.f };

	for (_uint i = 0; i < iNumPrimitive; ++i)
	{
		//_vector	vDir = XMLoadFloat4(&vCameraPos) - XMVectorSetW(XMLoadFloat3(&pVertices[pIndices[i]._0].vPosition), 1.f);

		//if (XMVectorGetX(XMVector3Dot(XMVector3Normalize(vDir), XMVector3Normalize(XMLoadFloat3(&pVertices[pIndices[i]._0].vNormal)))))
		//	continue;

		fDist = 0.f;

		if (TriangleTests::Intersects(vRayPos, vRayDir,
			XMLoadFloat3(&pVertices[pIndices[i]._0].vPosition),
			XMLoadFloat3(&pVertices[pIndices[i]._1].vPosition),
			XMLoadFloat3(&pVertices[pIndices[i]._2].vPosition),
			fDist))
		{
			if (Result.second > fDist)
			{
				vPickingPoint = XMVector3TransformCoord(vRayPos + vRayDir * fDist, pTransformCom->Get_WorldMatrix());
				Result.first = true;
				Result.second = fDist;
			}
		}
	}

	return Result;
}

pair<_bool, _int> CGameUtils::Picking_Cell(HWND& hWnd, _float fScreenWidth, _float fScreenHeight, CTransform* pTransformCom, CNavigation* pNaviCom)
{

	POINT		pt;
	GetCursorPos(&pt);
	ScreenToClient(hWnd, &pt);

	/* ViewPort -> Projection */
	_float3	vViewportPoint;
	vViewportPoint.x = (_float)pt.x / (fScreenWidth * 0.5f) - 1.f;
	vViewportPoint.y = (_float)pt.y / (fScreenHeight * -0.5f) + 1.f;
	vViewportPoint.z = 0.f;

	/* Now in Projection Space */
	_vector	vMousePoint = XMLoadFloat3(&vViewportPoint);
	vMousePoint = XMVectorSetW(vMousePoint, 1.f);

	/* Projection -> View */
	_matrix	matProjInv = CGameInstance::GetInstance()->Get_TransformMatrix_Inverse(CPipeLine::D3DTS_PROJ);

	vMousePoint = XMVector3TransformCoord(vMousePoint, matProjInv);

	/* Now in View Space */
	_vector	vRayPos = XMVectorSet(0.f, 0.f, 0.f, 1.f);
	_vector	vRayDir = (vMousePoint - vRayPos);

	/* View -> World */
	_matrix	matViewInv = CGameInstance::GetInstance()->Get_TransformMatrix_Inverse(CPipeLine::D3DTS_VIEW);

	vRayPos = XMVector3TransformCoord(vRayPos, matViewInv);
	vRayDir = (XMVector3TransformNormal(vRayDir, matViewInv));

	/* World -> Local */
	_matrix	matWorldInv = pTransformCom->Get_WorldMatrix_Inverse();

	vRayPos = XMVector3TransformCoord(vRayPos, matWorldInv);
	vRayDir = XMVector3Normalize(XMVector3TransformNormal(vRayDir, matWorldInv));

	/* Now in Local Space */
	_float		fDist = 0.f;
	pair<_bool, _int>	Result{ false, 100000 };

	vector<CCell*> vecCell = *pNaviCom->Get_vecCell();

	for (auto iter : vecCell)
	{
		_vector vPointA, vPointB, vPointC, vPlane;
		ZeroMemory(&vPointA, sizeof(_vector));
		ZeroMemory(&vPointB, sizeof(_vector));
		ZeroMemory(&vPointC, sizeof(_vector));
		ZeroMemory(&vPlane, sizeof(_vector));

		vPointA = XMLoadFloat3(&iter->Get_Point(CCell::POINT_A));
		vPointB = XMLoadFloat3(&iter->Get_Point(CCell::POINT_B));
		vPointC = XMLoadFloat3(&iter->Get_Point(CCell::POINT_C));

		if (TriangleTests::Intersects(vRayPos, vRayDir, vPointA, vPointB, vPointC, fDist))
		{
			Result.first = true;
			Result.second = iter->Get_Index();
			break;
		}
	}
	return Result;
}

void CGameUtils::SortPointsByCW(_float3* vPoints)
{
	_float3	vSortedPoints[3];
	_float		vMinPointX, vMaxPointX;

	vMinPointX = min(vPoints[0].x, vPoints[1].x);
	vMinPointX = min(vMinPointX, vPoints[2].x);

	vMaxPointX = max(vPoints[0].x, vPoints[1].x);
	vMaxPointX = max(vMaxPointX, vPoints[2].x);

	_uint	iIndex = 0;
	for (_uint i = 0; i < 3; ++i)
	{
		if (vMinPointX < vPoints[i].x && vMaxPointX > vPoints[i].x)
		{
			vSortedPoints[0] = vPoints[i];
			iIndex = i;
		}
	}

	_float		vMinPointZ, vMaxPointZ;

	if (iIndex == 0)
	{
		vMinPointZ = min(vPoints[1].z, vPoints[2].z);
		vMaxPointZ = max(vPoints[1].z, vPoints[2].z);

		if (vMinPointZ < vSortedPoints[0].z && vMaxPointZ > vSortedPoints[0].z)
		{
			if (vPoints[1].x < vPoints[2].x)
			{
				vSortedPoints[1] = vPoints[2];
				vSortedPoints[2] = vPoints[1];
			}
			else
			{
				vSortedPoints[1] = vPoints[1];
				vSortedPoints[2] = vPoints[2];
			}

			_float     fDiagonalA = (vSortedPoints[1].z - vSortedPoints[2].z) / (vSortedPoints[1].x - vSortedPoints[2].x);
			_float     fDiagonalB = (vSortedPoints[1].z - vSortedPoints[0].z) / (vSortedPoints[1].x - vSortedPoints[0].x);

			if (fDiagonalA < fDiagonalB)
			{
				_float3  vTemp;
				vTemp = vSortedPoints[1];
				vSortedPoints[1] = vSortedPoints[2];
				vSortedPoints[2] = vTemp;
			}
		}
		else if (vSortedPoints[0].z > vMaxPointZ)
		{
			if (vPoints[1].x < vPoints[2].x)
			{
				vSortedPoints[1] = vPoints[2];
				vSortedPoints[2] = vPoints[1];
			}
			else if (vPoints[1].x > vPoints[2].x)
			{
				vSortedPoints[1] = vPoints[1];
				vSortedPoints[2] = vPoints[2];
			}
		}
		else if (vSortedPoints[0].z < vMinPointZ)
		{
			if (vPoints[1].x < vPoints[2].x)
			{
				vSortedPoints[1] = vPoints[1];
				vSortedPoints[2] = vPoints[2];
			}
			else if (vPoints[1].x > vPoints[2].x)
			{
				vSortedPoints[1] = vPoints[2];
				vSortedPoints[2] = vPoints[1];
			}
		}
	}

	else if (iIndex == 1)
	{
		vMinPointZ = min(vPoints[0].z, vPoints[2].z);
		vMaxPointZ = max(vPoints[0].z, vPoints[2].z);

		if (vMinPointZ < vSortedPoints[0].z && vMaxPointZ > vSortedPoints[0].z)
		{
			if (vPoints[0].x < vPoints[2].x)
			{
				vSortedPoints[1] = vPoints[2];
				vSortedPoints[2] = vPoints[0];
			}
			else
			{
				vSortedPoints[1] = vPoints[0];
				vSortedPoints[2] = vPoints[2];
			}

			_float     fDiagonalA = (vSortedPoints[1].z - vSortedPoints[2].z) / (vSortedPoints[1].x - vSortedPoints[2].x);
			_float     fDiagonalB = (vSortedPoints[1].z - vSortedPoints[0].z) / (vSortedPoints[1].x - vSortedPoints[0].x);

			if (fDiagonalA < fDiagonalB)
			{
				_float3  vTemp;
				vTemp = vSortedPoints[1];
				vSortedPoints[1] = vSortedPoints[2];
				vSortedPoints[2] = vTemp;
			}
		}

		else if (vSortedPoints[0].z > vMaxPointZ)
		{
			if (vPoints[0].x < vPoints[2].x)
			{
				vSortedPoints[1] = vPoints[2];
				vSortedPoints[2] = vPoints[0];
			}
			else if (vPoints[0].x > vPoints[2].x)
			{
				vSortedPoints[1] = vPoints[0];
				vSortedPoints[2] = vPoints[2];
			}
		}

		else if (vSortedPoints[0].z < vMinPointZ)
		{
			if (vPoints[0].x < vPoints[2].x)
			{
				vSortedPoints[1] = vPoints[0];
				vSortedPoints[2] = vPoints[2];
			}
			else if (vPoints[0].x > vPoints[2].x)
			{
				vSortedPoints[1] = vPoints[2];
				vSortedPoints[2] = vPoints[0];
			}
		}
	}

	else if (iIndex == 2)
	{
		vMinPointZ = min(vPoints[0].z, vPoints[1].z);
		vMaxPointZ = max(vPoints[0].z, vPoints[1].z);

		if (vMinPointZ < vSortedPoints[0].z && vMaxPointZ > vSortedPoints[0].z)
		{
			if (vPoints[0].x < vPoints[1].x)
			{
				vSortedPoints[1] = vPoints[1];
				vSortedPoints[2] = vPoints[0];
			}
			else
			{
				vSortedPoints[1] = vPoints[0];
				vSortedPoints[2] = vPoints[1];
			}

			_float     fDiagonalA = (vSortedPoints[1].z - vSortedPoints[2].z) / (vSortedPoints[1].x - vSortedPoints[2].x);
			_float     fDiagonalB = (vSortedPoints[1].z - vSortedPoints[0].z) / (vSortedPoints[1].x - vSortedPoints[0].x);

			if (fDiagonalA < fDiagonalB)
			{
				_float3  vTemp;
				vTemp = vSortedPoints[1];
				vSortedPoints[1] = vSortedPoints[2];
				vSortedPoints[2] = vTemp;
			}
		}

		else if (vSortedPoints[0].z > vMaxPointZ)
		{
			if (vPoints[0].x < vPoints[1].x)
			{
				vSortedPoints[1] = vPoints[1];
				vSortedPoints[2] = vPoints[0];
			}
			else if (vPoints[0].x > vPoints[1].x)
			{
				vSortedPoints[1] = vPoints[0];
				vSortedPoints[2] = vPoints[1];
			}
		}

		else if (vSortedPoints[0].z < vMinPointZ)
		{
			if (vPoints[0].x < vPoints[1].x)
			{
				vSortedPoints[1] = vPoints[0];
				vSortedPoints[2] = vPoints[1];
			}
			else if (vPoints[0].x > vPoints[1].x)
			{
				vSortedPoints[1] = vPoints[1];
				vSortedPoints[2] = vPoints[0];
			}
		}
	}

	vPoints[0] = vSortedPoints[0];
	vPoints[1] = vSortedPoints[1];
	vPoints[2] = vSortedPoints[2];
}

void CGameUtils::Cell_Sorting(_float3* vPoints)
{
}

_bool CGameUtils::FloatCmp(const _float& f1, const _float& f2, _float fEpsilon)
{
	return fabs(f1 - f2) < fEpsilon;
}

_matrix CGameUtils::Get_PlayerPivotMatrix()
{
	return XMMatrixIdentity() * XMMatrixRotationY(XMConvertToRadians(180.f));
}

_bool CGameUtils::CollisionSphereSphere(CCollider * pColliderA, CCollider * pColliderB, _float4 & vDir)
{
	BoundingSphere*	pSphereA = (BoundingSphere*)pColliderA->Get_Collider(CCollider::COLLIDER_SPHERE);
	NULL_CHECK_RETURN(pSphereA, false);

	BoundingSphere*	pSphereB = (BoundingSphere*)pColliderB->Get_Collider(CCollider::COLLIDER_SPHERE);
	NULL_CHECK_RETURN(pSphereB, false);

	/* 충돌 비교 */
	_vector	vCenterA = XMLoadFloat3(&pSphereA->Center);
	_vector	vRadiusA = XMVectorReplicatePtr(&pSphereA->Radius);

	_vector	vCenterB = XMLoadFloat3(&pSphereB->Center);
	_vector	vRadiusB = XMVectorReplicatePtr(&pSphereB->Radius);

	_vector	vDelta = vCenterB - vCenterA;
	_vector	vDistanceSquared = XMVector3LengthSq(vDelta);

	_vector	vRadiusSquared = XMVectorAdd(vRadiusA, vRadiusB);
	vRadiusSquared = XMVectorMultiply(vRadiusSquared, vRadiusSquared);

	/* 충돌됨 */
	if (XMVector3LessOrEqual(vDistanceSquared, vRadiusSquared) == true)
	{
		_float P = XMVectorGetX(XMVector3Dot(vDelta, vDelta));
		_float Q = (pSphereA->Radius * pSphereA->Radius - pSphereB->Radius * pSphereB->Radius + P) / (2.f * P);
		_vector	vMiddlePoint = vCenterA + Q * vDelta;

		_float		fMiddlePointToCenterA = pSphereB->Radius - XMVectorGetX(XMVector3Length(vMiddlePoint - XMLoadFloat3(&pSphereB->Center)));
		_float		fMiddlePointToCenterB = pSphereA->Radius - XMVectorGetX(XMVector3Length(vMiddlePoint - XMLoadFloat3(&pSphereA->Center)));

		_float		fIntersectDist = fMiddlePointToCenterA + fMiddlePointToCenterB;
		_vector	vIntersectDir = XMVector3Normalize(vDelta);

		vDir = vIntersectDir * fIntersectDist;

		return true;
	}

	return false;
}

_float CGameUtils::GetRandomFloat(_float lowBound, _float highBound)
{

	if (lowBound >= highBound)
		return lowBound;

	float f = (rand() % 10000) * 0.0001f;

	return (f * (highBound - lowBound)) + lowBound;
}


void CGameUtils::GetRandomVector(_float4* out, _float4* min, _float4* max)
{
	out->x = GetRandomFloat(min->x, max->x);
	out->y = GetRandomFloat(min->y, max->y);
	out->z = GetRandomFloat(min->z, max->z);
}