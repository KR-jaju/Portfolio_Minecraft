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
	bool flag;
	
	Index3() : x(0), y(0), z(0), flag(false) {}
	Index3(int x, int y, int z) : x(x), y(y), z(z), flag(true) {}
	Index3(Index3 const& idx3) { *this = idx3; }
	Index3& operator=(Index3 const& idx3) {
		this->x = idx3.x;
		this->y = idx3.y;
		this->z = idx3.z;
		this->flag = idx3.flag;
		return *this;
	}

	Index3& operator+=(Index3 const& idx3) {
		this->x += idx3.x;
		this->y += idx3.y;
		this->z += idx3.z;
		this->flag |= idx3.flag;
		return *this;
	}

	bool operator<(Index3 const& idx3) const {
		if (this->x == idx3.x) {
			if (this->y == idx3.y)
				return this->z < idx3.z;
			return this->y < idx3.y;
		}
		return this->x < idx3.x;
	}

	bool operator==(Index3 const& idx3) const {
		return this->x == idx3.x && this->y == idx3.y &&
			this->z == idx3.z;
	}
};

inline Index3 operator+(Index3 const& idx1, Index3 const& idx2) {
	Index3 res(idx1.x + idx2.x, idx1.y + idx2.y, idx1.z + idx2.z);
	return res;
}

inline Index3 operator-(Index3 const& idx1, Index3 const& idx2) {
	Index3 res(idx1.x - idx2.x, idx1.y - idx2.y, idx1.z - idx2.z);
	return res;
}

struct Index2 {
	int x;
	int y;
	bool flag;

	Index2() : x(0), y(0), flag(false) {}
	Index2(int x, int y) : x(x), y(y), flag(true) {}
	Index2(Index2 const& idx2) { *this = idx2; }
	
	Index2& operator=(Index2 const& idx2) {
		this->x = idx2.x;
		this->y = idx2.y;
		this->flag = idx2.flag;
		return *this;
	}
	bool operator<(Index2 const& idx2) const {
		if (this->x == idx2.x) {
			return this->y < idx2.y;
		}
		return this->x < idx2.x;
	}

	bool operator==(Index2 const& idx2) const {
		return this->x == idx2.x && this->y == idx2.y;
	}
};

inline Index2 operator+(Index2 const& idx1, Index2 const& idx2) {
	Index2 res(idx1.x + idx2.x, idx1.y + idx2.y);
	return res;
}

inline Index2 operator-(Index2 const& idx1, Index2 const& idx2) {
	Index2 res(idx1.x - idx2.x, idx1.y - idx2.y);
	return res;
}

struct Index3Hash {
	size_t operator() (Index3 const& idx3) const {
		return hash<int>()(idx3.x) ^ hash<int>()(idx3.y) ^ hash<int>()(idx3.z);
	}
};

struct Index2Hash {
	size_t operator() (Index2 const& idx2) const {
		return hash<int>()(idx2.x) ^ hash<int>()(idx2.y);
	}
};

// c_idx : chunk index, b_idx : block index
struct WorldIndex {
	Index2 c_idx;
	Index3 b_idx;
	bool flag;
	WorldIndex() { this->flag = false; }
	WorldIndex(Index2 const& cidx, Index3 const& bidx)
		: c_idx(cidx), b_idx(bidx), flag(true) {}
	WorldIndex(WorldIndex const& world) { *this = world; }
	WorldIndex& operator=(WorldIndex const& world) {
		this->c_idx = world.c_idx;
		this->b_idx = world.b_idx;
		this->flag = world.flag;
	}
};

struct CamPos {
	vec3 pos;
	float dummy;
	float r;
	vec3 dummys;
};