#pragma once
// utils wictexture위해 추가
#include <InitGuid.h>
#include <dinput.h>
#include <dsound.h>

#include <windows.h>
#include "DirectXMath.h"
#include "SimpleMath.h"
#include "WICTextureLoader11.h"

using int8 = __int8;
using int16 = __int16;
using int32 = __int32;
using int64 = __int64;

using uint8 = unsigned __int8;
using uint16 = unsigned __int16;
using uint32 = unsigned __int32;
using uint64 = unsigned __int64;

using vec2 = DirectX::SimpleMath::Vector2;
using vec3 = DirectX::SimpleMath::Vector3;
using vec4 = DirectX::SimpleMath::Vector4;
using color = DirectX::XMFLOAT4;

using Mat = DirectX::SimpleMath::Matrix;

struct Vertex {
	vec3 pos;
	vec4 color;
};

struct VertexUV {
	vec3 pos;
	vec2 uv;
};

struct VertexBlockUV {
	int type;
	vec3 pos;
	vec2 uv;
	int dir;
};

struct VertexBlock {
	int type;
	vec3 pos;
	color col;
};

struct MVP {
	Mat model = Mat::Identity;
	Mat view = Mat::Identity;
	Mat proj = Mat::Identity;
};

#include <map>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>

using namespace std;

#include <assert.h>


#include <wrl.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXTex/DirectXTex.h>
#include <DirectXTex/DirectXTex.inl>
using namespace DirectX;
using namespace Microsoft::WRL;

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(linker, "/entry:wWinMainCRTStartup /subsystem:console")

#ifdef _DEBUG
#pragma comment(lib, "DirectXTex\\DirectXTex_debug.lib")
#else
#pragma comment(lib, "DirectXTex\\DirectXTex.lib")
#endif

#define CHECK(p)	assert(SUCCEEDED(p))
