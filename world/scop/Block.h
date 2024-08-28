#pragma once

#include "WorldUtils.h"

class Texture;
class TextureArray;

class Block
{
public:
	Block();
	~Block();
	void addBlockFacePosAndTex(
		vec3 const& start_pos,
		int dir,
		float x,
		float y,
		float z,
		int type,
		vector<VertexBlockUV> &vertices
	);

	void addBlockFaceIndices(
		uint32 start, 
		vector<uint32>& indices
	);
};

