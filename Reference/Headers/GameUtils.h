#pragma once
#include "Engine_Defines.h"

class ENGINE_DLL CGameUtils
{
private:
	CGameUtils() = default;

public:
	static void		wc2c(const wchar_t* szWchar, char* szChar);
	static void		ctwc(const char* szChar, wchar_t* wszChar);
	static void		SplitTag(wchar_t* wszTag, wstring& wszChar);	// Split Tag by '_'
	static void		ConvertNumberTo_d(char* szChar);
	static COMPONENTTYPE	CheckComponentTypeFromTag(const wstring& wstrComponentTag);

	static void		Saturate(int& InValue, int InMax, int InMin);

};

