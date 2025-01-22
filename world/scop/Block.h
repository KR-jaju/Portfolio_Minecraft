#pragma once

#include "WorldUtils.h"

// grass type 1

inline Mat calcTangentSpace(
	vec3 pos1, vec3 pos2, vec3 pos3,
	vec2 uv1, vec2 uv2, vec2 uv3
)
{
	Mat res;
	vec2 deltaUV1 = uv2 - uv1;
	vec2 deltaUV2 = uv3 - uv1;
	vec3 edge1 = pos2 - pos1; // edge
	vec3 edge2 = pos3 - pos1; // edge
	float f = 1.0f /
		(deltaUV1.x * deltaUV2.y - deltaUV1.y * deltaUV2.x);

	vec3 tangent;
	tangent.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
	tangent.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
	tangent.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
	tangent.Normalize();

	vec3 bitangent;
	bitangent.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
	bitangent.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
	bitangent.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);
	bitangent.Normalize();

	vec3 normal = tangent.Cross(bitangent);
	normal.Normalize();

	res = Mat(tangent, bitangent, normal);

	return res;
}

namespace Block {

	static vector<vec3> pos = {
		//top
		{-0.5f, +0.5f, +0.5f},
		{+0.5f, +0.5f, +0.5f},
		{-0.5f, +0.5f, -0.5f},
		{+0.5f, +0.5f, -0.5f},

		//bottom
		{-0.5f, -0.5f, +0.5f},
		{-0.5f, -0.5f, -0.5f},
		{+0.5f, -0.5f, +0.5f},
		{+0.5f, -0.5f, -0.5f},

		// front
		{-0.5f, +0.5f, -0.5f},
		{+0.5f, +0.5f, -0.5f},
		{-0.5f, -0.5f, -0.5f},
		{+0.5f, -0.5f, -0.5f},

		// back
		{-0.5f, +0.5f, +0.5f},
		{-0.5f, -0.5f, +0.5f},
		{+0.5f, +0.5f, +0.5f},
		{+0.5f, -0.5f, +0.5f},

		// left
		{-0.5f, +0.5f, +0.5f},
		{-0.5f, +0.5f, -0.5f},
		{-0.5f, -0.5f, +0.5f},
		{-0.5f, -0.5f, -0.5f},

		//right
		{+0.5f, +0.5f, -0.5f},
		{+0.5f, +0.5f, +0.5f},
		{+0.5f, -0.5f, -0.5f},
		{+0.5f, -0.5f, +0.5f},
	};
	static vector<vec2> uv = {
		//top
		{0.f, 0.f},
		{1.f, 0.f},
		{0.f, 1.f},
		{1.f, 1.f},

		//bottom
		{0.f, 1.f},
		{0.f, 0.f},
		{1.f, 1.f},
		{1.f, 0.f},

		//front
		{0.f, 0.f},
		{1.f, 0.f},
		{0.f, 1.f},
		{1.f, 1.f},

		//back
		{1.f, 0.f},
		{1.f, 1.f},
		{0.f, 0.f},
		{0.f, 1.f},

		//left
		{0.f, 0.f},
		{1.f, 0.f},
		{0.f, 1.f},
		{1.f, 1.f},

		//right
		{0.f, 0.f},
		{1.f, 0.f},
		{0.f, 1.f},
		{1.f, 1.f}
	};
	static vector<vec3> normals = {
			{0, 1, 0},
			{0, -1, 0},
			{0, 0, -1},
			{0, 0, 1},
			{-1, 0, 0},
			{1, 0, 0}
	};
	static vector<vec3> tangents = {
		{1, 0, 0},
		{1, 0, 0},
		{1, 0, 0}, // front
		{-1, 0, 0}, // back
		{0, 0, -1}, // left
		{0, 0, 1} // right
	};
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
	static vector<vec3> sample_normals = {
		{0, 1, 0},
		{0, -1, 0},
		{0, 0, -1},
		{0, 0, 1},
		{-1, 0, 0},
		{1, 0, 0}
	};

	inline void addFaceQuadPosAndTex(
		vec3 const& start_pos,
		int dir,
		float x,
		float y,
		float z,
		int type,
		vector<VertexGeo>& vertices
	)
	{
		VertexGeo vertex;
		x = start_pos.x + x;
		y = start_pos.y + y;
		z = start_pos.z - z;
		
		int idx;
		type = type - 1 + 2 * (type - 1);
		if (dir == 0)
			idx = type;
		else if (dir == 1)
			idx = type + 2;
		else
			idx = type + 1;
		for (int i = dir * 4; i < dir * 4 + 4; i++) {
			vertex.pos = pos[i] + vec3(x, y, z);
			vertex.normal = normals[dir];
			vertex.tangent = tangents[dir];
			vertex.uv = uv[i];
			vertex.tex_arr_idx = idx;
			vertices.push_back(vertex);
		}
	}

	inline void addBlocQuadIndices( // test
		uint32 start,
		vector<uint32>& indices
	)
	{
		indices.push_back(start);
		indices.push_back(start + 1);
		indices.push_back(start + 2);
		indices.push_back(start + 3);
		indices.push_back(start + 2);
		indices.push_back(start + 1);
	}

	inline void addBlockFacePosAndTex(
		vec3 const& start_pos,
		float x,
		float y,
		float z,
		int dir,
		int shadow_flag,
		vector<VertexShadow>& vertices
	)
	{
		
		VertexShadow vertex;
		x = start_pos.x + x;
		y = start_pos.y + y;
		z = start_pos.z - z;
		for (int i = dir * 4; i < dir * 4 + 4; i++) {
			vertex.pos = sample_pos[i] + vec3(x, y, z);
			vertex.normal = sample_normals[dir];
			vertex.shadow_flag = shadow_flag;
			vertices.push_back(vertex);
		}
	}

	inline void addBlockFacePosAndCol(
		vec3 const& start_pos,
		float x,
		float y,
		float z,
		int dir,
		int type,
		vector<VertexColor>& vertices
	)
	{
		vec4 col;
		if (type == -3)
			col = vec4(1, 0, 0, 0.3);
		else if (type == -2)
			col = vec4(0, 1, 0, 0.3);
		else if (type == -1)
			col = vec4(0, 0, 1, 0.3);
		VertexColor vertex;
		x = start_pos.x + x;
		y = start_pos.y + y;
		z = start_pos.z - z;

		for (int i = dir * 4; i < dir * 4 + 4; i++) {
			vertex.pos = sample_pos[i] + vec3(x, y, z);
			vertex.col = col;
			vertices.push_back(vertex);
		}
	}

	inline void addBlockFaceWater( //  todo mesh optimization 공부
		vec3 const& start_pos,
		float x,
		float y,
		float z,
		int dir,
		vector<VertexColor>& vertices
	) 
	{
		vec4 col;
		col = vec4(0, 0, 1, 0.3);
		VertexColor vertex;
		x = start_pos.x + x;
		y = start_pos.y + y;
		z = start_pos.z - z;

		for (int i = dir * 4; i < dir * 4 + 4; i++) {
			vertex.pos = sample_pos[i] + vec3(x, y, z);
			vertex.col = col;
			vertices.push_back(vertex);
		}
	}

	inline void addBlockFaceIndices(
		uint32 start, 
		vector<uint32>& indices
	)
	{
		indices.push_back(start);
		indices.push_back(start + 1);
		indices.push_back(start + 2);
		indices.push_back(start);
		indices.push_back(start + 2);
		indices.push_back(start + 3);
	}

	inline void makeCubeMap(
		float scale,
		vector<VertexDefer>& vertices,
		vector<uint32>& indices
	)
	{
		vector<vec3> positions;
		vector<vec2> texcoords; // 텍스춰 좌표

		// 윗면
		positions.push_back(vec3(-1.0f, 1.0f, -1.0f) * scale);
		positions.push_back(vec3(-1.0f, 1.0f, 1.0f) * scale);
		positions.push_back(vec3(1.0f, 1.0f, 1.0f) * scale);
		positions.push_back(vec3(1.0f, 1.0f, -1.0f) * scale);
		texcoords.push_back(vec2(0.0f, 0.0f));
		texcoords.push_back(vec2(1.0f, 0.0f));
		texcoords.push_back(vec2(1.0f, 1.0f));
		texcoords.push_back(vec2(0.0f, 1.0f));

		// 아랫면
		positions.push_back(vec3(-1.0f, -1.0f, -1.0f) * scale);
		positions.push_back(vec3(1.0f, -1.0f, -1.0f) * scale);
		positions.push_back(vec3(1.0f, -1.0f, 1.0f) * scale);
		positions.push_back(vec3(-1.0f, -1.0f, 1.0f) * scale);
		texcoords.push_back(vec2(0.0f, 0.0f));
		texcoords.push_back(vec2(1.0f, 0.0f));
		texcoords.push_back(vec2(1.0f, 1.0f));
		texcoords.push_back(vec2(0.0f, 1.0f));

		// 앞면
		positions.push_back(vec3(-1.0f, -1.0f, -1.0f) * scale);
		positions.push_back(vec3(-1.0f, 1.0f, -1.0f) * scale);
		positions.push_back(vec3(1.0f, 1.0f, -1.0f) * scale);
		positions.push_back(vec3(1.0f, -1.0f, -1.0f) * scale);
		texcoords.push_back(vec2(0.0f, 0.0f));
		texcoords.push_back(vec2(1.0f, 0.0f));
		texcoords.push_back(vec2(1.0f, 1.0f));
		texcoords.push_back(vec2(0.0f, 1.0f));

		// 뒷면
		positions.push_back(vec3(-1.0f, -1.0f, 1.0f) * scale);
		positions.push_back(vec3(1.0f, -1.0f, 1.0f) * scale);
		positions.push_back(vec3(1.0f, 1.0f, 1.0f) * scale);
		positions.push_back(vec3(-1.0f, 1.0f, 1.0f) * scale);
		texcoords.push_back(vec2(0.0f, 0.0f));
		texcoords.push_back(vec2(1.0f, 0.0f));
		texcoords.push_back(vec2(1.0f, 1.0f));
		texcoords.push_back(vec2(0.0f, 1.0f));

		// 왼쪽
		positions.push_back(vec3(-1.0f, -1.0f, 1.0f) * scale);
		positions.push_back(vec3(-1.0f, 1.0f, 1.0f) * scale);
		positions.push_back(vec3(-1.0f, 1.0f, -1.0f) * scale);
		positions.push_back(vec3(-1.0f, -1.0f, -1.0f) * scale);
		texcoords.push_back(vec2(0.0f, 0.0f));
		texcoords.push_back(vec2(1.0f, 0.0f));
		texcoords.push_back(vec2(1.0f, 1.0f));
		texcoords.push_back(vec2(0.0f, 1.0f));

		// 오른쪽
		positions.push_back(vec3(1.0f, -1.0f, 1.0f) * scale);
		positions.push_back(vec3(1.0f, -1.0f, -1.0f) * scale);
		positions.push_back(vec3(1.0f, 1.0f, -1.0f) * scale);
		positions.push_back(vec3(1.0f, 1.0f, 1.0f) * scale);
		texcoords.push_back(vec2(0.0f, 0.0f));
		texcoords.push_back(vec2(1.0f, 0.0f));
		texcoords.push_back(vec2(1.0f, 1.0f));
		texcoords.push_back(vec2(0.0f, 1.0f));

		VertexDefer vertex;
		for (int i = 0; i < positions.size(); i++) {
			vertex.pos = positions[i];
			vertex.uv = texcoords[i];
			vertices.push_back(vertex);
		}
		indices = {
		0,  1,  2,  0,  2,  3,  // 윗면
		4,  5,  6,  4,  6,  7,  // 아랫면
		8,  9,  10, 8,  10, 11, // 앞면
		12, 13, 14, 12, 14, 15, // 뒷면
		16, 17, 18, 16, 18, 19, // 왼쪽
		20, 21, 22, 20, 22, 23  // 오른쪽
		};
		reverse(indices.begin(), indices.end());
	}

	inline void makeBox(
		float scale,
		vector<VertexDefer>& vertices,
		vector<uint32>& indices
	) {
		static const vector<vec3> sample_pos = {
			// front
			{-1.f, -1.f, 0.f},
			{-1.f, +1.f, 0.f},
			{+1.f, +1.f, 0.f},
			{+1.f, -1.f, 0.f},
		};
		static const vector<vec2> sample_uv = {
			{0.f, 1.f},
			{0.f, 0.f},
			{1.f, 0.f},
			{1.f, 1.f},
		};
		VertexDefer v_deff;
		for (int i = 0; i < 4; i++) {
			v_deff.pos = sample_pos[i] * scale;
			v_deff.uv = sample_uv[i];
			vertices.push_back(v_deff);
		}
		indices.push_back(0);
		indices.push_back(1);
		indices.push_back(2);
		indices.push_back(0);
		indices.push_back(2);
		indices.push_back(3);
	}
}