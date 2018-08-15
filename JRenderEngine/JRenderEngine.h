#pragma once
#include <Windows.h>
#include <string>
#include <stdlib.h>
#include <memory.h>
#include <assert.h>

#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dsound.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "dwrite.lib")
#pragma comment(lib, "d2d1.lib")

#ifdef _DEBUG
#pragma comment(lib, "d3dx11d.lib")
#pragma comment(lib, "d3dx10d.lib")
#pragma comment(lib,"D3dx9d.lib")

#else
#pragma comment(lib, "d3dx11.lib")
#pragma comment(lib, "d3dx10.lib")
#pragma comment(lib,"D3dx9.lib")
#endif

#include<d3d11.h>
#include<D3DX11.h>
#include<d3dx9core.h>
#include<D3DX10math.h>
#include<D3Dcompiler.h>
#include<d3dcommon.h>
#include<dinput.h>
#include<dsound.h>
#include<d2d1helper.h>
#include<d2dbasetypes.h>
#include<D2DErr.h>
#include<d2d1effects.h>
#include<d2d1effecthelpers.h>
#include<d2d1.h>
#include<dwrite.h>
#include<dxgi.h>
#include <d3dx11async.h>
#include <wrl.h>

// 사용자 정의 헤더
#include"EnumTemplate.h"
#include"JMessage.h"
#include"JTrace.h"

#define TransRadian(x) x * 0.0174532925f 
