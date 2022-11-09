#pragma once
// #pragma comment(lib, "d3d11.lib")

#pragma warning (disable : 4005)
#pragma warning (disable : 4251)

#include "d3d11.h"
#define DIRECTINPUT_VERSION 0x0800

#include <dinput.h>
#include <DirectXMath.h>
using namespace DirectX;

#include <vector>
#include <list>
#include <map>
#include <unordered_map>
#include <algorithm>
using namespace std;

#include "Engine_Macro.h"
#include "Engine_Utility.h"
#include "Engine_Struct.h"
#include "Engine_Typedef.h"
#include "Engine_Function.h"


#ifdef _DEBUG

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#ifndef DBG_NEW 

#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ ) 
#define new DBG_NEW 

#endif

#endif // _DEBUG

using namespace Engine;


