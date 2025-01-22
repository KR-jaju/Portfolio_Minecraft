#include "pch.h"
#include "TerrainSystem.h"
#include "MapUtils.h"
#include "Chunk.h"
#include "Trees.h"

TerrainSystem::TerrainSystem(MapUtils* minfo)
{
	this->m_info = minfo;
}

void TerrainSystem::fillChunk(Index2 const& c_idx, Index2 const& c_pos)
{
	float x, sy, z;
	float offset = 0.000001f;
	int16& max_h = this->m_info->chunks[c_idx.y][c_idx.x]->max_h;
	for (int i = 0; i < 16; i++) {
		z = (c_pos.y - i + offset) / 32.f;
		for (int j = 0; j < 16; j++) {
			x = (c_pos.x + j + offset) / 32.f;
			double h_ = this->perlin_noise.getNoise2D(x, z, 3, 0.5);
			int16 h = static_cast<int16>((h_ + 0.8) * 0.5 * 50.f);
			max_h = max(max_h, h);
			this->m_info->setHeight(c_idx, j, i, h);
			for (int y = 0; y < h; y++) {
				this->m_info->addBlock(c_idx, j, y, i, 1);
			}
		}
	}
}

void TerrainSystem::createHeightMap()
{
	for (int i = 0; i < this->m_info->size_h; i++) {
		for (int j = 0; j < this->m_info->size_w; j++) {
			Index2 c_pos = this->m_info->s_pos + Index2(j * 16, -i * 16);
			Index2 c_idx;
			c_idx = this->m_info->getChunkIndex(c_pos.x, c_pos.y);
			this->m_info->chunks[c_idx.y][c_idx.x] = make_shared<Chunk>();
			this->m_info->chunks[c_idx.y][c_idx.x]->start_pos =
				vec3(c_pos.x + 0.5f, 0.5f, c_pos.y - 0.5f);
			this->m_info->chunks[c_idx.y][c_idx.x]->chunk_pos = c_pos;
			this->fillChunk(c_idx, c_pos);
			this->fillWater(c_idx);
			this->makeCave(c_idx, c_pos);
		}
	}
}

void TerrainSystem::createTrees()
{
	for (int i = 1; i < this->m_info->size_h - 1; i++) {
		for (int j = 1; j < this->m_info->size_w - 1; j++) {
			Index2 c_pos = this->m_info->s_pos + Index2(j * 16, -i * 16);
			Index2 c_idx;
			c_idx = this->m_info->getChunkIndex(c_pos.x, c_pos.y);
			this->makeTree(c_idx);
		}
	}
}

void TerrainSystem::createTrees(vector<Index2>& chunks, int dir)
{
	vector<Index2> p_chunks;
	p_chunks.resize(chunks.size());
	Index2 cpos;
	for (Index2& cidx : chunks)
		this->makeTree(cidx);
	if (dir == 1) {
		for (int i = 0; i < chunks.size(); i++) {
			cpos = this->m_info->chunks[chunks[i].y][chunks[i].x]->chunk_pos;
			cpos = cpos + Index2(16, 0);
			p_chunks[i] = this->m_info->findChunkIndex(cpos.x, cpos.y);
		}
	}
	else if (dir == 2) {
		for (int i = 0; i < chunks.size(); i++) {
			cpos = this->m_info->chunks[chunks[i].y][chunks[i].x]->chunk_pos;
			cpos = cpos + Index2(-16, 0);
			p_chunks[i] = this->m_info->findChunkIndex(cpos.x, cpos.y);
		}
	}
	else if (dir == 4) {
		for (int i = 0; i < chunks.size(); i++) {
			cpos = this->m_info->chunks[chunks[i].y][chunks[i].x]->chunk_pos;
			cpos = cpos + Index2(0, -16);
			p_chunks[i] = this->m_info->findChunkIndex(cpos.x, cpos.y);
		}
	}
	else {
		for (int i = 0; i < chunks.size(); i++) {
			cpos = this->m_info->chunks[chunks[i].y][chunks[i].x]->chunk_pos;
			cpos = cpos + Index2(0, 16);
			p_chunks[i] = this->m_info->findChunkIndex(cpos.x, cpos.y);
		}
	}
	for (int i = 0; i < p_chunks.size(); i++)
		chunks.push_back(p_chunks[i]);
}

void TerrainSystem::setTree(Index2 const& c_idx, int x, int y, int z)
{
	this->putTree(c_idx, x, y, z, 0);
}

int TerrainSystem::getAdjInfo(
	Index2 const& c_idx, 
	int x, 
	int y, 
	int z,
	int type,
	int dir
)
{
	Index2 adj_idx;
	Index2 pos = this->m_info->chunks[c_idx.y][c_idx.x]->chunk_pos;
	if (dir == 0) {
		if (x - 1 < 0) {
			pos.x -= 16;
			adj_idx = this->m_info->findChunkIndex(pos.x, pos.y);
			if (adj_idx.flag == false)
				return 0;
			if (type == 0)
				return this->m_info->findBlock(adj_idx, 15, y, z);
			if (type == 1)
				return this->m_info->getWater(adj_idx, 15, y, z);
		}
		x -= 1;
	}
	else if (dir == 1) {
		if (x + 1 > 15) {
			pos.x += 16;
			adj_idx = this->m_info->findChunkIndex(pos.x, pos.y);
			if (adj_idx.flag == false)
				return 0;
			if (type == 0)
				return this->m_info->findBlock(adj_idx, 0, y, z);
			if (type == 1)
				return this->m_info->getWater(adj_idx, 0, y, z);
		}
		x += 1;
	}
	else if (dir == 2) {
		if (z - 1 < 0) {
			pos.y += 16;
			adj_idx = this->m_info->findChunkIndex(pos.x, pos.y);
			if (adj_idx.flag == false)
				return 0;
			if (type == 0)
				return this->m_info->findBlock(adj_idx, x, y, 15);
			if (type == 1)
				return this->m_info->getWater(adj_idx, x, y, 15);
		}
		z -= 1;
	}
	else if (dir == 3) {
		if (z + 1 > 15) {
			pos.y -= 16;
			adj_idx = this->m_info->findChunkIndex(pos.x, pos.y);
			if (adj_idx.flag == false)
				return 0;
			if (type == 0)
				return this->m_info->findBlock(adj_idx, x, y, 0);
			if (type == 1)
				return this->m_info->getWater(adj_idx, x, y, 0);
		}
		z += 1;
	}
	else if (dir == 4) {
		if (y - 1 < 0)
			return 0;
		y -= 1;
	}
	else {
		if (y + 1 > 255)
			return 0;
		y += 1;
	}
	int res;
	if (type == 0)
		res = this->m_info->findBlock(c_idx, x, y, z);
	else if (type == 1)
		res = this->m_info->getWater(c_idx, x, y, z);
	return res;
}

void TerrainSystem::fillWater(Index2 const& c_idx)
{
	for (int y = 0; y < 15; y++) {
		for (int z = 0; z < 16; z++) {
			for (int x = 0; x < 16; x++) {
				if (this->m_info->findBlock(c_idx, x, y, z))
					continue;
				this->m_info->setWater(c_idx, x, y, z, true);
			}
		}
	}
}

void TerrainSystem::makeCave(Index2 const& c_idx, Index2 const& c_pos)
{
	float x, sy, z;
	float offset = 0.000001f;
	int16& max_h = this->m_info->chunks[c_idx.y][c_idx.x]->max_h;
	for (int i = 0; i < 16; i++) {
		z = (c_pos.y - i + offset) / 32.f;
		for (int j = 0; j < 16; j++) {
			x = (c_pos.x + j + offset) / 32.f;
			int h = this->m_info->findHeight(c_idx, j, i);
			for (int y = 0; y < h; y++) {
				sy = y / 16.f;
				double h_ = this->perlin_noise.getNoise3D(x, sy, z, 3, 0.5);
				if (h_ <= 0.1)
					continue;
				if (y < 1)
					continue;
				if (this->m_info->getWater(c_idx, j, y, i))
					continue;
				bool flag = false;
				for (int dir = 0; dir < 4; dir++) {
					if (this->getAdjInfo(c_idx, j, y, i, 1, dir)) {
						flag = true;
						break;
					}
				}
				if (flag)
					continue;
				this->m_info->addBlock(c_idx, j, y, i, 0);
			}
		}
	}
}

void TerrainSystem::makeTree(Index2 const& c_idx)
{
	Index2 cpos = this->m_info->chunks[c_idx.y][c_idx.x]->chunk_pos;
	bool continue_flag = false;
	double offset = 0.00001;
	for (int z = 0; z < 16; z++) {
		double d_z = (cpos.y - z + offset) / 32.f;
		for (int x = 0; x < 16; x++) {
			double d_x = (cpos.x + x + offset) / 32.f;
			int h = this->m_info->findHeight(c_idx, x, z);
			if (this->m_info->findBlock(c_idx, x, h - 1, z) == 0)
				continue;
			for (int y = h; y < h + 7; y++) {
				if (this->m_info->findBlock(c_idx, x, y, z)) {
					continue_flag = true;
					break;
				}
			}
			if (continue_flag)
				continue;
			if (h < 10 || this->m_info->findLight(c_idx, x, h, z) < 10)
				continue;
			double p = this->perlin_noise.getNoise2D(d_x, d_z, 3, 0.5);
			if (p < 0.0)
				continue;
			this->putTree(c_idx, x, h, z, 0);
		}
	}
}

void TerrainSystem::checkChunk(
	Index2 const& c_idx, 
	int x, int y, int z, 
	Index2& res_cidx, Index3& res_bidx
)
{
	Index2 pos = this->m_info->chunks[c_idx.y][c_idx.x]->chunk_pos;
	Index2 mx_cidx = this->m_info->findChunkIndex(pos.x - 16, pos.y);
	Index2 px_cidx = this->m_info->findChunkIndex(pos.x + 16, pos.y);
	Index2 mz_cidx = this->m_info->findChunkIndex(pos.x, pos.y + 16);
	Index2 pz_cidx = this->m_info->findChunkIndex(pos.x, pos.y - 16);
	Index2 mxmz_cidx = this->m_info->findChunkIndex(pos.x - 16, pos.y + 16);
	Index2 mxpz_cidx = this->m_info->findChunkIndex(pos.x - 16, pos.y - 16);
	Index2 pxmz_cidx = this->m_info->findChunkIndex(pos.x + 16, pos.y + 16);
	Index2 pxpz_cidx = this->m_info->findChunkIndex(pos.x + 16, pos.y - 16);

	int here_x = x;
	int here_z = z;
	if (x < 0)
		here_x = 16 + x;
	else if (x > 15)
		here_x = abs(16 - x);
	if (z < 0)
		here_z = 16 + z;
	else if (z > 15)
		here_z = abs(16 - z);

	res_cidx = c_idx;
	if (x < 0 && z < 0)
		res_cidx = mxmz_cidx;
	else if (x < 0 && z > 15)
		res_cidx = mxpz_cidx;
	else if (x > 15 && z < 0)
		res_cidx = pxmz_cidx;
	else if (x > 15 && z > 15)
		res_cidx = pxpz_cidx;
	else if (x < 0)
		res_cidx = mx_cidx;
	else if (x > 15)
		res_cidx = px_cidx;
	else if (z < 0)
		res_cidx = mz_cidx;
	else if (z > 15)
		res_cidx = pz_cidx;
	else
		res_cidx = c_idx;
	res_bidx = Index3(here_x, y, here_z);
}

void TerrainSystem::putTree(
	Index2 const& c_idx, 
	int x, 
	int y, 
	int z, 
	int type
)
{
	int ix = -1;
	int iy = -1;
	int iz = -1;
	Index2 here_cidx;
	Index3 here_bidx;
	Index2 pos = this->m_info->chunks[c_idx.y][c_idx.x]->chunk_pos;
	double nx, nz;
	double offset = 0.001;

	if (type == 0) {
		for (int sy = y; sy < y + 7; sy++) {
			iy++;
			iz = -1;
			for (int sz = z - 2; sz <= z + 2; sz++) {
				iz++;
				ix = -1;
				for (int sx = x - 2; sx <= x + 2; sx++) {
					ix++;
					int tr = Trees::getOakTreeInfo(ix, iy, iz);
					if (tr) {
						this->checkChunk(c_idx, sx, sy, sz, 
							here_cidx, here_bidx);
						if (here_cidx.flag == false)
							continue;
						int b_type = 
							this->m_info->findBlock(here_cidx, here_bidx);
						if (b_type && b_type != 3)
							continue;
						if (tr == 2 && b_type == 3)
							continue;
						if (tr == 1)
							this->m_info->addBlock(here_cidx, here_bidx,
								static_cast<int>(BlockType::OAK_LOG));
						if (tr == 2)
							this->m_info->addBlock(here_cidx, here_bidx,
								static_cast<int>(BlockType::OAK_LEAVES));
						this->resetChunkHeight(here_cidx, here_bidx.x,
							here_bidx.z, here_bidx.y);
					}
				}
			}
		}
	}
}

void TerrainSystem::resetChunkHeight(
	Index2 const& c_idx, 
	int x, 
	int z, 
	int h
)
{
	int now_h = this->m_info->findHeight(c_idx, x, z);
	if (now_h < h)
		this->m_info->setHeight(c_idx, x, z, h);
	int16& max_h = this->m_info->chunks[c_idx.y][c_idx.x]->max_h;
	max_h = max(max_h, static_cast<int16>(h + 1));
}