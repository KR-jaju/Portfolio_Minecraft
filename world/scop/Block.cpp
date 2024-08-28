#include "pch.h"
#include "Block.h"
#include "Texture.h"
#include "TextureArray.h"



Block::Block()
{
}

Block::~Block()
{
}

void Block::addBlockFacePosAndTex(
	vec3 const& start_pos,
	int dir, 
	float x, 
	float y, 
	float z, 
	int type, 
	vector<VertexBlockUV>& vertices
)
{
	static vector<vec3> sample_pos = {
		{-0.5f, +0.5f, -0.5f},
		{-0.5f, +0.5f, +0.5f},
		{+0.5f, +0.5f, +0.5f},
		{+0.5f, +0.5f, -0.5f},

		{-0.5f, -0.5f, -0.5f},
		{+0.5f, -0.5f, -0.5f},
		{+0.5f, -0.5f, +0.5f},
		{-0.5f, -0.5f, +0.5f},

		{-0.5f, -0.5f, -0.5f},
		{-0.5f, +0.5f, -0.5f},
		{+0.5f, +0.5f, -0.5f},
		{+0.5f, -0.5f, -0.5f},

		{-0.5f, -0.5f, +0.5f},
		{+0.5f, -0.5f, +0.5f},
		{+0.5f, +0.5f, +0.5f},
		{-0.5f, +0.5f, +0.5f},

		{-0.5f, -0.5f, +0.5f},
		{-0.5f, +0.5f, +0.5f},
		{-0.5f, +0.5f, -0.5f},
		{-0.5f, -0.5f, -0.5f},

		{+0.5f, -0.5f, +0.5f},
		{+0.5f, -0.5f, -0.5f},
		{+0.5f, +0.5f, -0.5f},
		{+0.5f, +0.5f, +0.5f}
	};
	static vector<vec2> sample_uv = {
		{0.f, 1.f},
		{0.f, 0.f},
		{1.f, 0.f},
		{1.f, 1.f},

		{0.f, 0.f},
		{1.f, 0.f},
		{1.f, 1.f},
		{0.f, 1.f},

		{0.f, 1.f},
		{0.f, 0.f},
		{1.f, 0.f},
		{1.f, 1.f},

		{1.f, 1.f},
		{0.f, 1.f},
		{0.f, 0.f},
		{1.f, 0.f},

		{0.f, 1.f},
		{0.f, 0.f},
		{1.f, 0.f},
		{1.f, 1.f},

		{1.f, 1.f},
		{0.f, 1.f},
		{0.f, 0.f},
		{1.f, 0.f}
	};
	VertexBlockUV vertex;
	x = start_pos.x + x;
	y = start_pos.y + y;
	z = start_pos.z - z;
	for (int i = dir * 4; i < dir * 4 + 4; i++) {
		vertex.pos = sample_pos[i] + vec3(x, y, z);
		vertex.uv = sample_uv[i];
		vertex.type = type;
		vertex.dir = dir;
		vertices.push_back(vertex);
	}
}

void Block::addBlockFaceIndices(uint32 start, vector<uint32>& indices)
{
	indices.push_back(start);
	indices.push_back(start + 1);
	indices.push_back(start + 2);
	indices.push_back(start);
	indices.push_back(start + 2);
	indices.push_back(start + 3);
}
