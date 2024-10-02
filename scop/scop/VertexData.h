#pragma once
struct VertexData
{
	vec3 position = { 0, 0, 0 };
};

struct VertexColorData
{
	vec3 position = { 0, 0, 0 };
	color color = { 0, 0, 0, 0 };
};

struct VertexTextureNormalData
{
	vec3 position = { 0, 0, 0 };
	vec2 uv = { 0, 0 };
	vec3 normal = { 0, 0, 0 };
	vector<D3D11_INPUT_ELEMENT_DESC> desc =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
};

