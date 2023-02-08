#pragma once
#include "Engine_Defines.h"


BEGIN(Engine)
class ENGINE_DLL CGameUtils
{
private:
	CGameUtils() = default;

public:
	static string				wstrTostr(const wstring& wstrSour);
	static wstring				s2ws(const string& strSour);

	static void					wc2c(const wchar_t* szWchar, char* szChar);
	static void					ctwc(const char* szChar, wchar_t* wszChar);
	
	static void					SplitTag(wchar_t* wszTag, wstring& wszChar);	// Split Tag by '_'
	static void					ConvertNumberTo_d(char* szChar);
	static COMPONENTTYPE		CheckComponentTypeFromTag(const wstring& wstrComponentTag);

	static void					Saturate(int& InValue, int InMax, int InMin);
	static void					SaturateF(float& InValue, float InMax, float InMin);
	static void					DecomposePitchYawRollFromXMMatrix(_fmatrix matRotation, _float& fPitch, _float& fYaw, _float& fRoll);

	static pair<_bool, _float>	Picking(HWND& hWnd, _float fScreenWidth, _float fScreenHeight, class CTransform* pTransformCom, _float3* pVertexPos, _uint iNumVerticesX, _uint iNumVerticesZ, _float3& vPickingPoint);
	static pair<_bool, _float>	Picking(HWND& hWnd, _float fScreenWidth, _float fScreenHeight, class CTransform* pTransformCom, VTXMODEL* pVertices, FACEINDICES32* pIndices, _uint iNumPrimitive, _float3& vPickingPoint);
	static pair<_bool, _int>	Picking_Cell(HWND& hWnd, _float fScreenWidth, _float fScreenHeight, class CTransform* pTransformCom, class CNavigation* pNaviCom);

	static void					SortPointsByCW(_float3* vPoints);
	static void					Cell_Sorting(_float3* vPoints);
	static _bool				FloatCmp(const _float& f1, const _float& f2, _float fEpsilon);

	static _bool				Rect_Picking(HWND& hWnd, const RECT& Rect);

	static _matrix			Get_PlayerPivotMatrix();
	static	_bool			CollisionSphereSphere(class CCollider* pColliderA, class CCollider* pColliderB, _float4& vDir);
	static _float			GetRandomFloat(_float lowBound, _float highBound);
	static void				GetRandomVector(_float4* out, _float4* min, _float4* max);


};

END