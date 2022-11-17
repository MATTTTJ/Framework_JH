#pragma once

#include <Process.h>

namespace Client
{
	static const unsigned int g_iWinSizeX = 1280;
	static const unsigned int g_iWinSizeY = 720;

	enum LEVEL { LEVEL_LOADING, LEVEL_LOGO, LEVEL_GAMEPLAY, LEVEL_END };
}

extern HWND g_hWnd;
extern HINSTANCE g_hInst;

using namespace Client;



