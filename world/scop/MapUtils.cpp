#include "pch.h"
#include "MapUtils.h"
#include "Chunk.h"

#define STDZERO 0.00000001

MapUtils::MapUtils(int size_w, int size_h)
{
	this->size_h = size_h;
	this->size_w = size_w;
	this->s_pos.x = -8 * this->size_w;
	this->s_pos.y = 8 * this->size_h;
	this->sv_pos.x = this->s_pos.x + 16;
	this->sv_pos.y = this->s_pos.y - 16;
	this->ev_pos.x = -this->sv_pos.x;
	this->ev_pos.y = -this->sv_pos.y;

	this->blocks = new int[16 * 16 * size_h * size_w * 256];
	this->light_map = new uint8[16 * 16 * size_h * size_w * 256];
	this->h_map = new int[16 * 16 * size_h * size_w];
	fill(this->blocks, this->blocks + 16 * 16 * size_h * size_w * 256, 0);
	fill(this->light_map, this->light_map + 16 * 16 * size_h * size_w * 256, 0);
	fill(this->h_map, this->h_map + 16 * 16 * size_h * size_w, 0);
}

MapUtils::~MapUtils()
{
	delete[] this->blocks;
	delete[] this->h_map;
	delete[] this->light_map;
}

vec3 MapUtils::intersectionRayAndPlane(
	vec3 const& r_pos,
	vec3 const& r_dir,
	vec3 const& p_pos,
	vec3 const& p_dir
)
{
	vec3 res;
	float t;
	t = (p_pos.Dot(p_dir) - r_pos.Dot(p_dir)) / p_dir.Dot(r_dir);
	res = r_pos + t * r_dir;
	return res;
}

bool MapUtils::inChunkBoundary(int x, int y, int z) {
	if (x >= 0 && x < 16 && z >= 0 && z < 16 && y >= 0 && y < 256)
		return true;
	return false;
}

bool MapUtils::inChunkBoundary(Index3 const& bidx)
{
	if (bidx.x >= 0 && bidx.x < 16 && 
		bidx.z >= 0 && bidx.z < 16 && bidx.y >= 0 && bidx.y < 256)
		return true;
	return false;
}

WorldIndex MapUtils::getBlockIndex(float x, float y, float z) const
{
	WorldIndex ans;
	int w_x = static_cast<int>(floor(x)) / 16;
	int w_z = static_cast<int>(floor(z)) / 16;

	if (w_x * 16 > x)
		w_x--;
	if (w_z * 16 < z)
		w_z++;
	Index2 c_idx = this->findChunkIndex(w_x * 16, w_z * 16);
	Index2 pos = this->chunks[c_idx.y][c_idx.x]->chunk_pos;
	int ex = pos.x + 16;
	int ez = pos.y - 16;
	if (x < pos.x || x > ex || z > pos.y || z < ez || y < 0 || y > 255)
		return ans;
	ans.c_idx = c_idx;
	int ix = static_cast<int>(floor(x) - pos.x);
	int iy = static_cast<int>(floor(y));
	int iz = floor(z);
	if (iz < z)
		iz += 1;
	iz = static_cast<int>(pos.y - iz);
	ans.b_idx = Index3(ix, iy, iz);
	ans.flag = true;
	return ans;
}

WorldIndex MapUtils::pickBlock(vec3 r_pos, vec3 r_dir)
{
	// 1. x y z index 찾기
	WorldIndex ans;
	int x, y, z;
	vec3 tmp = r_pos;
	if (r_pos.y >= 256) {
		if (r_dir.y >= 0)
			return ans;
		tmp = this->intersectionRayAndPlane(
			r_pos,
			r_dir,
			vec3(r_pos.x, 256, r_pos.z),
			vec3(0, 1, 0)
		);
	}
	else if (r_pos.y < 0) {
		if (r_dir.y <= 0)
			return ans;
		tmp = this->intersectionRayAndPlane(
			r_pos,
			r_dir,
			vec3(r_pos.x, 0, r_pos.z),
			vec3(0, 1, 0)
		);
	}
	x = static_cast<int>(floor(tmp.x));
	y = static_cast<int>(floor(tmp.y));
	z = static_cast<int>(floor(tmp.z));
	if (z < tmp.z)
		z += 1;
	int step_x = 1;
	int step_y = 1;
	int step_z = 1;
	if (r_dir.x < 0)
		step_x = -1;
	if (r_dir.y < 0)
		step_y = -1;
	if (r_dir.z < 0)
		step_z = -1;

	// 2 max x y z, delta x y z 구하기
	float max_x, max_y, max_z;
	float delta_x = 0, delta_y = 0, delta_z = 0;
	bool flag_x = true, flag_y = true, flag_z = true;
	int sx, sy, sz;
	vec3 first, second;
	sx = x;
	if (r_dir.x > 0)
		sx += 1;
	max_x = r_dir.Dot(vec3(1, 0, 0));
	if (max_x <= STDZERO && max_x >= -STDZERO)
		flag_x = false;
	if (flag_x) {
		first = this->intersectionRayAndPlane(
			r_pos,
			r_dir,
			vec3(sx, 0, 0),
			vec3(1, 0, 0)
		);
		second = this->intersectionRayAndPlane(
			r_pos,
			r_dir,
			vec3(sx + step_x, 0, 0),
			vec3(1, 0, 0)
		);
		max_x = (r_pos - first).Length();
		delta_x = (first - second).Length();
	}
	sy = y;
	if (r_dir.y > 0)
		sy += 1;
	max_y = r_dir.Dot(vec3(0, 1, 0));
	if (max_y <= STDZERO && max_y >= -STDZERO)
		flag_y = false;
	if (flag_y) {
		first = this->intersectionRayAndPlane(
			r_pos,
			r_dir,
			vec3(0, sy, 0),
			vec3(0, 1, 0)
		);
		second = this->intersectionRayAndPlane(
			r_pos,
			r_dir,
			vec3(0, sy + step_y, 0),
			vec3(0, 1, 0)
		);
		max_y = (r_pos - first).Length();
		delta_y = (first - second).Length();
	}
	sz = z;
	if (r_dir.z < 0)
		sz -= 1;
	max_z = r_dir.Dot(vec3(0, 0, 1));
	if (max_z <= STDZERO && max_z >= -STDZERO)
		flag_z = false;
	if (flag_z) {
		first = this->intersectionRayAndPlane(
			r_pos,
			r_dir,
			vec3(0, 0, sz),
			vec3(0, 0, 1)
		);
		second = this->intersectionRayAndPlane(
			r_pos,
			r_dir,
			vec3(0, 0, sz + step_z),
			vec3(0, 0, 1)
		);
		max_z = (r_pos - first).Length();
		delta_z = (first - second).Length();
	}
	if (flag_x == false)
		max_x = INT_MAX;
	if (flag_y == false)
		max_y = INT_MAX;
	if (flag_z == false)
		max_z = INT_MAX;

	for (int i = 0; i < 8; i++) {
		if (max_x < max_y) {
			if (max_x < max_z) {
				x += step_x;
				max_x += delta_x;
				ans.dir = 2;
			}
			else {
				z += step_z;
				max_z += delta_z;
				ans.dir = 1;
			}
		}
		else {
			if (max_y < max_z) {
				y += step_y;
				max_y += delta_y;
				ans.dir = 0;
			}
			else {
				z += step_z;
				max_z += delta_z;
				ans.dir = 1;
			}
		}
		ans = this->getBlockIndex(x, y, z);
		if (ans.flag && this->findBlock(ans.c_idx, ans.b_idx)) {
			ans.pos = vec3(x, y, z);
			return ans;
		}
	}
	ans.flag = false;
	return ans;
}

Index2 MapUtils::findChunkIndex(int w_x, int w_z) const
{
	int t = w_x / 16;
	int x = (t % this->size_w + this->size_w) % this->size_w;
	t = w_z / 16;
	int z = (t % this->size_h + this->size_h) % this->size_h;
	Index2 ans = Index2(x, z);
	if (this->chunks[ans.y][ans.x] == nullptr) {
		ans.flag = false;
		return ans;
	}
	Index2 pos = this->chunks[ans.y][ans.x]->chunk_pos;
	if (pos.x != w_x || pos.y != w_z)
		ans.flag = false;
	return ans;
}

Index2 MapUtils::getChunkIndex(int w_x, int w_z) const
{
	int t = w_x / 16;
	int x = (t % this->size_w + this->size_w) % this->size_w;
	t = w_z / 16;
	int z = (t % this->size_h + this->size_h) % this->size_h;
	Index2 ans = Index2(x, z);
	return ans;
}

int MapUtils::findBlock(Index2 const& c_idx, int x, int y, int z) const
{
	int idx = 16 * 16 * 256 * (c_idx.x + this->size_w * c_idx.y) +
		x + 16 * (z + 16 * y);
	return this->blocks[idx];
}

int MapUtils::findBlock(Index2 const& c_idx, Index3 const& b_idx) const
{
	int idx = 16 * 16 * 256 * (c_idx.x + this->size_w * c_idx.y) +
		b_idx.x + 16 * (b_idx.z + 16 * b_idx.y);
	return this->blocks[idx];
}

void MapUtils::addBlock(Index2 const& c_idx, int x, int y, int z, int type)
{
	int idx = 16 * 16 * 256 * (c_idx.x + this->size_w * c_idx.y) + x +
		16 * (z + 16 * y);
	this->blocks[idx] = type;
}

void MapUtils::addBlock(Index2 const& c_idx, Index3 const& b_idx, int type)
{
	int idx = 16 * 16 * 256 * (c_idx.x + this->size_w * c_idx.y) +
		b_idx.x + 16 * (b_idx.z + 16 * b_idx.y);
	this->blocks[idx] = type;
}

void MapUtils::setHeight(Index2 const& c_idx, int x, int z, int h)
{
	int idx = 16 * 16 * (c_idx.x + this->size_w * c_idx.y) + x + 16 * z;
	this->h_map[idx] = h;
}

void MapUtils::setHeight(Index2 const& c_idx, Index2 const& b_idx, int h)
{
	int idx = 16 * 16 * (c_idx.x + this->size_w * c_idx.y) +
		b_idx.x + 16 * b_idx.y;
	this->h_map[idx] = h;
}

int MapUtils::findHeight(Index2 const& c_idx, Index2 const& h_idx) const
{
	int idx = 16 * 16 * (c_idx.x + this->size_w * c_idx.y) +
		h_idx.x + 16 * h_idx.y;
	return this->h_map[idx];
}

int MapUtils::findHeight(Index2 const& c_idx, Index3 const& b_idx) const
{
	int idx = 16 * 16 * (c_idx.x + this->size_w * c_idx.y) +
		b_idx.x + 16 * b_idx.z;
	return this->h_map[idx];
}

int MapUtils::findHeight(Index2 const& c_idx, int x, int z) const
{
	int idx = 16 * 16 * (c_idx.x + this->size_w * c_idx.y) + x + 16 * z;
	return this->h_map[idx];
}

uint8 MapUtils::findLight(Index2 const& c_idx, int x, int y, int z) const
{
	int idx = 16 * 16 * 256 * (c_idx.x + this->size_w * c_idx.y) +
		x + 16 * (z + 16 * y);
	return this->light_map[idx];
}

uint8 MapUtils::findLight(Index2 const& c_idx, Index3 const& b_idx) const
{
	int idx = 16 * 16 * 256 * (c_idx.x + this->size_w * c_idx.y) +
		b_idx.x + 16 * (b_idx.z + 16 * b_idx.y);
	return this->light_map[idx];
}

void MapUtils::setLight(Index2 const& c_idx, int x, int y, int z, uint8 type)
{
	int idx = 16 * 16 * 256 * (c_idx.x + this->size_w * c_idx.y) +
		x + 16 * (z + 16 * y);
	this->light_map[idx] = type;
}

void MapUtils::setLight(Index2 const& c_idx, Index3 const& b_idx, uint8 type)
{
	int idx = 16 * 16 * 256 * (c_idx.x + this->size_w * c_idx.y) +
		b_idx.x + 16 * (b_idx.z + 16 * b_idx.y);
	this->light_map[idx] = type;
}