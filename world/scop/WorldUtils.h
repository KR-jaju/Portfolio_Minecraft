#pragma once

#include <set>

class PixelShader;
class VertexShader;
template<typename T> class Buffer;
class Graphics;
class ConstantBuffer;
class RasterizerState;
class BlendState;
class InputLayout;
class SamplerState;
class TextureArray;

enum class Face {
	Top,
	Bottom,
	Front,
	Back,
	Left,
	Right
};

struct Index3 {
	int x;
	int y;
	int z;

	bool operator<(Index3 const& idx3) const {
		if (this->x == idx3.x) {
			if (this->y == idx3.y)
				return this->z < idx3.z;
			return this->y < idx3.y;
		}
		return this->x < idx3.x;
	}
};