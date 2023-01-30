#pragma once
#include "Client_Defines.h"

BEGIN(Client)
class CJson_Utils
{
public:
	static HRESULT		Load_Json(const wchar_t* _strPath, json* pOut);
	static HRESULT		Save_Json(const wchar_t* _strPath, json _json);



public:
	static XMFLOAT4		Get_VectorFromJson(json _json);
	static XMFLOAT4X4	Get_MatrixFromJson(json _json);

public:
	static wstring Complete_Path(const wchar_t* _strFileName);

};

END