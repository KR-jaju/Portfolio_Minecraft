#include "pch.h"
#include "Block.h"

Block::Block()
{
}

Block::~Block()
{
}

vector<uint32> Block::getAllIndices(uint32 start) const
{
	vector<uint32> indices;
	uint32 index = start;

	for (int i = 0; i < 6; i++) {
		indices.insert(
			indices.end(),
			{
				index, index + 1,
				index + 2, index,
				index + 2, index + 3
			}
		);
		index += 4;
	}

	return indices;
}

vector<uint32> Block::getFaceIndices(uint32 start) const
{
	vector<uint32> indices = {
		start,
		start + 1,
		start + 2,
		start,
		start + 2,
		start + 3
	};
	return indices;
}

vector<vec3> Block::getAllVertexPosition(float scale) const
{
	vector<vec3> vertices_pos;

	// top
	vertices_pos.push_back(vec3(-1.0f, 1.0f, -1.0f) * scale);
	vertices_pos.push_back(vec3(-1.0f, 1.0f, 1.0f) * scale);
	vertices_pos.push_back(vec3(1.0f, 1.0f, 1.0f) * scale);
	vertices_pos.push_back(vec3(1.0f, 1.0f, -1.0f) * scale);

	// bottom
	vertices_pos.push_back(vec3(-1.0f, -1.0f, -1.0f) * scale);
	vertices_pos.push_back(vec3(1.0f, -1.0f, -1.0f) * scale);
	vertices_pos.push_back(vec3(1.0f, -1.0f, 1.0f) * scale);
	vertices_pos.push_back(vec3(-1.0f, -1.0f, 1.0f) * scale);

	// front
	vertices_pos.push_back(vec3(-1.0f, -1.0f, -1.0f) * scale);
	vertices_pos.push_back(vec3(-1.0f, 1.0f, -1.0f) * scale);
	vertices_pos.push_back(vec3(1.0f, 1.0f, -1.0f) * scale);
	vertices_pos.push_back(vec3(1.0f, -1.0f, -1.0f) * scale);

	// back
	vertices_pos.push_back(vec3(-1.0f, -1.0f, 1.0f) * scale);
	vertices_pos.push_back(vec3(1.0f, -1.0f, 1.0f) * scale);
	vertices_pos.push_back(vec3(1.0f, 1.0f, 1.0f) * scale);
	vertices_pos.push_back(vec3(-1.0f, 1.0f, 1.0f) * scale);
	
	// left
	vertices_pos.push_back(vec3(-1.0f, -1.0f, 1.0f) * scale);
	vertices_pos.push_back(vec3(-1.0f, 1.0f, 1.0f) * scale);
	vertices_pos.push_back(vec3(-1.0f, 1.0f, -1.0f) * scale);
	vertices_pos.push_back(vec3(-1.0f, -1.0f, -1.0f) * scale);

	// right
	vertices_pos.push_back(vec3(1.0f, -1.0f, 1.0f) * scale);
	vertices_pos.push_back(vec3(1.0f, -1.0f, -1.0f) * scale);
	vertices_pos.push_back(vec3(1.0f, 1.0f, -1.0f) * scale);
	vertices_pos.push_back(vec3(1.0f, 1.0f, 1.0f) * scale);

	return vertices_pos;
}

vector<vec3> Block::getFaceVertexPos(Face flag, float scale) const
{
	vector<vec3> vertices_pos;
	if (flag == Face::Top) {
		vertices_pos.push_back(vec3(-1.0f, 1.0f, -1.0f) * scale);
		vertices_pos.push_back(vec3(-1.0f, 1.0f, 1.0f) * scale);
		vertices_pos.push_back(vec3(1.0f, 1.0f, 1.0f) * scale);
		vertices_pos.push_back(vec3(1.0f, 1.0f, -1.0f) * scale);
	}
	else if (flag == Face::Bottom) {
		vertices_pos.push_back(vec3(-1.0f, -1.0f, -1.0f) * scale);
		vertices_pos.push_back(vec3(1.0f, -1.0f, -1.0f) * scale);
		vertices_pos.push_back(vec3(1.0f, -1.0f, 1.0f) * scale);
		vertices_pos.push_back(vec3(-1.0f, -1.0f, 1.0f) * scale);
	}
	else if (flag == Face::Front) {
		vertices_pos.push_back(vec3(-1.0f, -1.0f, -1.0f) * scale);
		vertices_pos.push_back(vec3(-1.0f, 1.0f, -1.0f) * scale);
		vertices_pos.push_back(vec3(1.0f, 1.0f, -1.0f) * scale);
		vertices_pos.push_back(vec3(1.0f, -1.0f, -1.0f) * scale);
	}
	else if (flag == Face::Back) {
		vertices_pos.push_back(vec3(-1.0f, -1.0f, 1.0f) * scale);
		vertices_pos.push_back(vec3(1.0f, -1.0f, 1.0f) * scale);
		vertices_pos.push_back(vec3(1.0f, 1.0f, 1.0f) * scale);
		vertices_pos.push_back(vec3(-1.0f, 1.0f, 1.0f) * scale);
	}
	else if (flag == Face::Left) {
		vertices_pos.push_back(vec3(-1.0f, -1.0f, 1.0f) * scale);
		vertices_pos.push_back(vec3(-1.0f, 1.0f, 1.0f) * scale);
		vertices_pos.push_back(vec3(-1.0f, 1.0f, -1.0f) * scale);
		vertices_pos.push_back(vec3(-1.0f, -1.0f, -1.0f) * scale);
	}
	else if (flag == Face::Right) {
		vertices_pos.push_back(vec3(1.0f, -1.0f, 1.0f) * scale);
		vertices_pos.push_back(vec3(1.0f, -1.0f, -1.0f) * scale);
		vertices_pos.push_back(vec3(1.0f, 1.0f, -1.0f) * scale);
		vertices_pos.push_back(vec3(1.0f, 1.0f, 1.0f) * scale);
	}
	return vertices_pos;
}

vector<vec2> Block::getAllUV(vec2 start, vec2 end) const
{
	vector<vec2> texcoord;

	// top
	texcoord.push_back(vec2(start.x, end.y));
	texcoord.push_back(start);
	texcoord.push_back(vec2(end.x, start.y));
	texcoord.push_back(end);

	// bottom
	texcoord.push_back(start);
	texcoord.push_back(vec2(end.x, start.y));
	texcoord.push_back(end);
	texcoord.push_back(vec2(start.x, end.y));

	// front
	texcoord.push_back(vec2(start.x, end.y));
	texcoord.push_back(start);
	texcoord.push_back(vec2(end.x, start.y));
	texcoord.push_back(end);
	
	// back
	texcoord.push_back(end);
	texcoord.push_back(vec2(start.x, end.y));
	texcoord.push_back(start);
	texcoord.push_back(vec2(end.x, start.y));

	// left
	texcoord.push_back(vec2(start.x, end.y));
	texcoord.push_back(start);
	texcoord.push_back(vec2(end.x, start.y));
	texcoord.push_back(end);

	//right
	texcoord.push_back(end);
	texcoord.push_back(vec2(start.x, end.y));
	texcoord.push_back(start);
	texcoord.push_back(vec2(end.x, start.y));

	return texcoord;
}

vector<vec2> Block::getFaceUV(vec2 start, vec2 end, Face flag) const
{
	vector<vec2> texcoord;
	if (flag == Face::Top) {
		texcoord.push_back(vec2(start.x, end.y));
		texcoord.push_back(start);
		texcoord.push_back(vec2(end.x, start.y));
		texcoord.push_back(end);
	}
	else if (flag == Face::Bottom) {
		texcoord.push_back(start);
		texcoord.push_back(vec2(end.x, start.y));
		texcoord.push_back(end);
		texcoord.push_back(vec2(start.x, end.y));
	}
	else if (flag == Face::Front) {
		texcoord.push_back(vec2(start.x, end.y));
		texcoord.push_back(start);
		texcoord.push_back(vec2(end.x, start.y));
		texcoord.push_back(end);
	}
	else if (flag == Face::Back) {
		texcoord.push_back(end);
		texcoord.push_back(vec2(start.x, end.y));
		texcoord.push_back(start);
		texcoord.push_back(vec2(end.x, start.y));
	}
	else if (flag == Face::Left) {
		texcoord.push_back(vec2(start.x, end.y));
		texcoord.push_back(start);
		texcoord.push_back(vec2(end.x, start.y));
		texcoord.push_back(end);
	}
	else if (flag == Face::Right) {
		texcoord.push_back(end);
		texcoord.push_back(vec2(start.x, end.y));
		texcoord.push_back(start);
		texcoord.push_back(vec2(end.x, start.y));
	}
	return texcoord;
}

vector<color> Block::getColorForTest(color col, Face flag) const
{
	vector<color> test;
	if (flag == Face::Top) {
		test.push_back(col);
		test.push_back(col);
		test.push_back(col);
		test.push_back(col);
	}

	else if (flag == Face::Bottom) {
		test.push_back(col);
		test.push_back(col);
		test.push_back(col);
		test.push_back(col);
	}

	else if (flag == Face::Front) {
		test.push_back(col);
		test.push_back(col);
		test.push_back(col);
		test.push_back(col);
	}

	else if (flag == Face::Left) {
		test.push_back(col);
		test.push_back(col);
		test.push_back(col);
		test.push_back(col);
	}

	else if (flag == Face::Right) {
		test.push_back(col);
		test.push_back(col);
		test.push_back(col);
		test.push_back(col);
	}
	return test;
}

