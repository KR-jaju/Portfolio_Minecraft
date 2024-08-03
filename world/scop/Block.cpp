#include "pch.h"
#include "Block.h"
#include "Texture.h"
#include "TextureArray.h"



Block::Block(int type)
{
	this->type = type;
}

Block::~Block()
{
}

vector<vec3> Block::getBlockFacePos(
	float x, 
	float y, 
	float z, 
	BlockFace BlockFace_flag
)
{
	vector<vec3> vertices_pos;

	if (BlockFace_flag == BlockFace::Top) {
		vertices_pos.push_back(vec3(x - 0.5f, y + 0.5f, z - 0.5f));
		vertices_pos.push_back(vec3(x - 0.5f, y + 0.5f, z + 0.5f));
		vertices_pos.push_back(vec3(x + 0.5f, y + 0.5f, z + 0.5f));
		vertices_pos.push_back(vec3(x + 0.5f, y + 0.5f, z - 0.5f));
		return vertices_pos;
	}
	if (BlockFace_flag == BlockFace::Bottom) {
		vertices_pos.push_back(vec3(x - 0.5f, y - 0.5f, z - 0.5f));
		vertices_pos.push_back(vec3(x + 0.5f, y - 0.5f, z - 0.5f));
		vertices_pos.push_back(vec3(x + 0.5f, y - 0.5f, z + 0.5f));
		vertices_pos.push_back(vec3(x - 0.5f, y - 0.5f, z + 0.5f));
		return vertices_pos;
	}
	if (BlockFace_flag == BlockFace::Front) {
		vertices_pos.push_back(vec3(x - 0.5f, y - 0.5f, z - 0.5f));
		vertices_pos.push_back(vec3(x - 0.5f, y + 0.5f, z - 0.5f));
		vertices_pos.push_back(vec3(x + 0.5f, y + 0.5f, z - 0.5f));
		vertices_pos.push_back(vec3(x + 0.5f, y - 0.5f, z - 0.5f));
		return vertices_pos;
	}
	if (BlockFace_flag == BlockFace::Back) {
		vertices_pos.push_back(vec3(x - 0.5f, y - 0.5f, z + 0.5f));
		vertices_pos.push_back(vec3(x + 0.5f, y - 0.5f, z + 0.5f));
		vertices_pos.push_back(vec3(x + 0.5f, y + 0.5f, z + 0.5f));
		vertices_pos.push_back(vec3(x - 0.5f, y + 0.5f, z + 0.5f));
		return vertices_pos;
	}
	if (BlockFace_flag == BlockFace::Left) {
		vertices_pos.push_back(vec3(x - 0.5f, y - 0.5f, z + 0.5f));
		vertices_pos.push_back(vec3(x - 0.5f, y + 0.5f, z + 0.5f));
		vertices_pos.push_back(vec3(x - 0.5f, y + 0.5f, z - 0.5f));
		vertices_pos.push_back(vec3(x - 0.5f, y - 0.5f, z - 0.5f));
		return vertices_pos;
	}
	if (BlockFace_flag == BlockFace::Right) {
		vertices_pos.push_back(vec3(x + 0.5f, y - 0.5f, z + 0.5f));
		vertices_pos.push_back(vec3(x + 0.5f, y - 0.5f, z - 0.5f));
		vertices_pos.push_back(vec3(x + 0.5f, y + 0.5f, z - 0.5f));
		vertices_pos.push_back(vec3(x + 0.5f, y + 0.5f, z + 0.5f));
	}
	return vertices_pos;
}

vector<vec2> Block::getBlockFaceTexcoord(vec2 start, vec2 end, BlockFace BlockFace_flag)
{
	vector<vec2> texcoord;

	if (BlockFace_flag == BlockFace::Top) {
		texcoord.push_back(vec2(start.x, end.y));
		texcoord.push_back(start);
		texcoord.push_back(vec2(end.x, start.y));
		texcoord.push_back(end);
		return texcoord;
	}
	if (BlockFace_flag == BlockFace::Bottom) {
		texcoord.push_back(start);
		texcoord.push_back(vec2(end.x, start.y));
		texcoord.push_back(end);
		texcoord.push_back(vec2(start.x, end.y));
		return texcoord;
	}
	if (BlockFace_flag == BlockFace::Front) {
		texcoord.push_back(vec2(start.x, end.y));
		texcoord.push_back(start);
		texcoord.push_back(vec2(end.x, start.y));
		texcoord.push_back(end);
		return texcoord;
	}
	if (BlockFace_flag == BlockFace::Back) {
		texcoord.push_back(end);
		texcoord.push_back(vec2(start.x, end.y));
		texcoord.push_back(start);
		texcoord.push_back(vec2(end.x, start.y));
		return texcoord;
	}
	if (BlockFace_flag == BlockFace::Left) {
		texcoord.push_back(vec2(start.x, end.y));
		texcoord.push_back(start);
		texcoord.push_back(vec2(end.x, start.y));
		texcoord.push_back(end);
		return texcoord;
	}
	if (BlockFace_flag == BlockFace::Right) {
		texcoord.push_back(end);
		texcoord.push_back(vec2(start.x, end.y));
		texcoord.push_back(start);
		texcoord.push_back(vec2(end.x, start.y));
	}
	return texcoord;
}

vector<uint32> Block::getBlockFaceIndices(uint32 start) const
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

void Block::registerSRV(
	ComPtr<ID3D11Device> device, 
	ComPtr<ID3D11DeviceContext> context, 
	vector<wstring> const& path_arr
)
{
	this->texture_arr = make_shared<TextureArray>(
		device,
		context,
		path_arr
	);
}
