#include "..\public\GameUtils.h"


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

void CGameUtils::Saturate(int & InValue, int InMax, int InMin)
{
	if (InValue > InMax)
		InValue = InMin;
	if (InValue < InMin)
		InValue = InMax;
}
