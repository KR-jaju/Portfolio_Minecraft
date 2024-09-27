#pragma once

#include "WorldUtils.h"

namespace Block {

	inline void addBlockFacePosAndTex(
		vec3 const& start_pos,
		int dir,
		float x,
		float y,
		float z,
		int type,
		int shadow_flag,
		vector<VertexBlockUV>& vertices
	)
	{
		static vector<vec3> sample_pos = {
			// top
			{-0.5f, +0.5f, -0.5f},
			{-0.5f, +0.5f, +0.5f},
			{+0.5f, +0.5f, +0.5f},
			{+0.5f, +0.5f, -0.5f},

			// bottom
			{-0.5f, -0.5f, -0.5f},
			{+0.5f, -0.5f, -0.5f},
			{+0.5f, -0.5f, +0.5f},
			{-0.5f, -0.5f, +0.5f},

			// front
			{-0.5f, -0.5f, -0.5f},
			{-0.5f, +0.5f, -0.5f},
			{+0.5f, +0.5f, -0.5f},
			{+0.5f, -0.5f, -0.5f},

			// back
			{-0.5f, -0.5f, +0.5f},
			{+0.5f, -0.5f, +0.5f},
			{+0.5f, +0.5f, +0.5f},
			{-0.5f, +0.5f, +0.5f},

			// left
			{-0.5f, -0.5f, +0.5f},
			{-0.5f, +0.5f, +0.5f},
			{-0.5f, +0.5f, -0.5f},
			{-0.5f, -0.5f, -0.5f},

			// right
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
		static vector<vec3> normals = {
			{0, 1, 0},
			{0, -1, 0},
			{0, 0, -1},
			{0, 0, 1},
			{-1, 0, 0},
			{1, 0, 0}
		};
		VertexBlockUV vertex;
		x = start_pos.x + x;
		y = start_pos.y + y;
		z = start_pos.z - z;
		for (int i = dir * 4; i < dir * 4 + 4; i++) {
			vertex.pos = sample_pos[i] + vec3(x, y, z);
			vertex.normal = normals[dir];
			vertex.uv = sample_uv[i];
			vertex.type = type;
			vertex.dir = dir;
			vertex.shadow_flag = shadow_flag;
			vertices.push_back(vertex);
		}
	}

	inline void addBlockFaceIndices(uint32 start, vector<uint32>& indices)
	{
		indices.push_back(start);
		indices.push_back(start + 1);
		indices.push_back(start + 2);
		indices.push_back(start);
		indices.push_back(start + 2);
		indices.push_back(start + 3);
	}
}