#pragma once
// utils wictexture위해 추가
#include <InitGuid.h>
#include <dinput.h>
#include <dsound.h>

#include <windows.h>
#include "DirectXMath.h"
#include "SimpleMath.h"
#include "WICTextureLoader11.h"
#include "json.hpp"

#include <utility>
#include <functional>

#include "IntVector2.h"
#include "IntVector3.h"

#include "Matrix.h"

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

using Quaternion = DirectX::XMVECTOR;

//using ivec2 = DirectX::XMINT2;
//using ivec3 = DirectX::XMINT3;
using ivec2 = IntVector2;
using ivec3 = IntVector3;
using ivec4 = DirectX::XMINT4;


using Mat = Matrix;
//DirectX::SimpleMath::Matrix;

struct Vertex {
	vec3 pos;
	vec4 color;
};

struct VertexUV {
	vec3 pos;
	vec2 uv;
};

struct ChunkVertex
{
	vec3 position;
	vec2 uv;
	int texture_id;
	int	direction;
};

struct MVP {
	Mat model = Mat::identity();
	Mat view = Mat::identity();
	Mat proj = Mat::identity();
};

struct CameraData
{
	Mat view = Mat::identity();
	Mat projection = Mat::identity();
};

struct CameraMatrices
{
	Mat view;
	Mat projection;
	Mat view_projection;
	Mat view_inverse_transpose;
	Mat	projection_inverse;
	ivec4 dimension;
};
//TODO: 구조체 정리 좀 해야함.

struct BoneData {
	Mat matrix[32];
};

struct BlockTextureData
{
	int east;
	int west;
	int up;
	int down;
	int north;
	int south;
};

enum class Direction : uint8_t
{
	East = 0,
	West = 1,
	Up = 2,
	Down = 3,
	North = 4,
	South = 5,
	Undefined = 6
};

enum DirectionFlags
{
	DIRECTION_CLEAR_BIT = 0,
	DIRECTION_EAST_BIT = (1 << static_cast<int>(Direction::East)),
	DIRECTION_WEST_BIT = (1 << static_cast<int>(Direction::West)),
	DIRECTION_UP_BIT = (1 << static_cast<int>(Direction::Up)),
	DIRECTION_DOWN_BIT = (1 << static_cast<int>(Direction::Down)),
	DIRECTION_NORTH_BIT = (1 << static_cast<int>(Direction::North)),
	DIRECTION_SOUTH_BIT = (1 << static_cast<int>(Direction::South)),
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