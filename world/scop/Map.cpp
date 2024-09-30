#include "pch.h"
#include "Map.h"
#include "Chunk.h"
#include "Graphics.h"
#include "Block.h"
#include <thread>

#include <time.h>

Map::Map(
	int size_w, 
	int size_h, 
	int fov_chunk, 
	int thread_cnt,
	shared_ptr<Graphics> graphic
) : m_info(size_w, size_h), l_system(&m_info), t_system(&m_info), r_system(&m_info)
{
	this->c_fov = fov_chunk;
	this->thread_cnt = thread_cnt;
	this->graphic = graphic;
	this->r_system.setGraphic(this->graphic);
	clock_t start, finish;
	start = clock();
	this->t_system.createHeightMap();
	finish = clock();
	cout << "set height(ms)" << static_cast<double>(finish - start) << endl;
	start = clock();
	this->l_system.createLightMap();
	finish = clock();
	cout << "set light(ms): " << static_cast<double>(finish - start) << endl;
	start = clock();
	this->terrainsetVerticesAndIndices();
	finish = clock();
	cout << "set vi(ms): " << static_cast<double>(finish - start) << endl;
}


void Map::resetChunk(Index2 const& c_idx)
{
	for (int y = 0; y < 256; y++) {
		for (int z = 0; z < 16; z++) {
			for (int x = 0; x < 16; x++) {
				m_info.addBlock(c_idx, x, y, z, 0);
				m_info.setLight(c_idx, x, y, z, 0);
			}
		}
	}
}

void Map::terrainsetVerticesAndIndices()
{
	vector<Index2> v_idxs;
	int c_cnt = (this->m_info.size_h - 2) * (this->m_info.size_w - 2);
	int t = c_cnt / this->thread_cnt;
	int m = c_cnt % this->thread_cnt;

	for (int i = 1; i < this->m_info.size_h - 1; i++) {
		for (int j = 1; j < this->m_info.size_w - 1; j++) {
			Index2 pos = this->m_info.s_pos + Index2(16 * j, -16 * i);
			Index2 c_idx = this->m_info.findChunkIndex(pos.x, pos.y);
			v_idxs.push_back(c_idx);
		}
	}
	vector<thread> threads;
	int st = 0;
	int siz;
	for (int i = 0; i < this->thread_cnt; i++) {
		if (m) {
			siz = t + 1;
			m--;
		}
		else
			siz = t;
		threads.push_back(thread(&Map::chunksSetVerticesAndIndices,
			this, v_idxs, st, st + siz));
		st = st + siz;
	}
	for (int i = 0; i < this->thread_cnt; i++)
		threads[i].join();
}

void Map::vertexAndIndexGenerator(
	Index2 const& c_idx,
	Index2 const& adj_idx,
	Index3 const& move,
	int dir,
	vector<VertexBlockUV>& vertices,
	vector<uint32>& indices
)
{
	uint32 index = this->m_info.chunks[c_idx.y][c_idx.x]->vertices_idx;
	int16& max_h = this->m_info.chunks[c_idx.y][c_idx.x]->max_h;
	int shadow_flag;
	for (int y = 0; y < max_h; y++) {
		for (int z = 0; z < 16; z++) {
			for (int x = 0; x < 16; x++) {
				int type = this->m_info.findBlock(c_idx, x, y, z);
				if (type == 0)
					continue;
				Index3 next(x + move.x, y + move.y, z - move.z); // index
				if (this->m_info.inChunkBoundary(next.x, next.y, next.z) 
					&& this->m_info.findBlock(c_idx, next))
					continue;
				if (adj_idx.flag) {
					if (next.x == -1 && this->m_info.findBlock(adj_idx, 15, next.y, next.z))
						continue;
					if (next.x == 16 && this->m_info.findBlock(adj_idx, 0, next.y, next.z))
						continue;
					if (next.z == -1 && this->m_info.findBlock(adj_idx, next.x, next.y, 15))
						continue;
					if (next.z == 16 && this->m_info.findBlock(adj_idx, next.x, next.y, 0))
						continue;
				}
				if (this->m_info.inChunkBoundary(next.x, next.y, next.z))
					shadow_flag = this->m_info.findLight(c_idx, next.x, next.y, next.z);
				else if (next.x == -1)
					shadow_flag = this->m_info.findLight(adj_idx, 15, y, z);
				else if (next.x == 16)
					shadow_flag = this->m_info.findLight(adj_idx, 0, y, z);
				else if (next.z == -1)
					shadow_flag = this->m_info.findLight(adj_idx, x, y, 15);
				else if (next.z == 16)
					shadow_flag = this->m_info.findLight(adj_idx, x, y, 0);
				else if (next.y == -1)
					shadow_flag = 0;
				else if (next.y == 256)
					shadow_flag = 15;
				Block::addBlockFacePosAndTex(
					this->m_info.chunks[c_idx.y][c_idx.x]->start_pos,
					dir,
					x, y, z, type,
					shadow_flag,
					vertices
				);
				Block::addBlockFaceIndices(index, indices);
				index += 4;
			}
		}
	}
	this->m_info.chunks[c_idx.y][c_idx.x]->vertices_idx = index;
}

void Map::setSightChunk(int chunk_cnt)
{
	this->c_fov = chunk_cnt;
}

void Map::chunksSetVerticesAndIndices(
	vector<Index2> const& v_idx,
	int st,
	int ed
)
{
	static const Index3 move_arr[6] = {
		Index3(0, 1, 0),
		Index3(0, -1, 0),
		Index3(0, 0, -1),
		Index3(0, 0, 1),
		Index3(-1, 0, 0),
		Index3(1, 0, 0)
	};
	vector<VertexBlockUV> vertices;
	vector<uint32> indices;
	ed = min(ed, v_idx.size());
	for (int i = st; i < ed; i++) {
		Index2 const& c_idx = v_idx[i];
		Index2 apos = this->m_info.chunks[c_idx.y][c_idx.x]->chunk_pos;
		for (int dir = 0; dir < 6; dir++) {
			Index2 pos = apos + Index2(16 * move_arr[dir].x,
				16 * move_arr[dir].z);
			Index2 adj_idx = this->m_info.findChunkIndex(pos.x, pos.y);
			this->vertexAndIndexGenerator(
				c_idx,
				adj_idx,
				move_arr[dir],
				dir,
				vertices,
				indices
			);
		}
		this->m_info.chunks[c_idx.y][c_idx.x]->createVIBuffer(
			this->graphic,
			vertices,
			indices
		);
		vertices.clear();
		indices.clear();
	}
}

int Map::checkTerrainBoundary(float x, float z) const
{
	float r = 16.f * this->c_fov;
	int mask = 0;
	if (x - r < this->m_info.sv_pos.x)
		mask |= 1 << 0; // left out
	if (x + r > this->m_info.ev_pos.x)
		mask |= 1 << 1; // right out
	if (z + r > this->m_info.sv_pos.y)
		mask |= 1 << 2; // back out
	if (z - r < this->m_info.ev_pos.y)
		mask |= 1 << 3; // front out
	return mask;
}

void Map::userPositionCheck(float x, float z)
{
	int mask = this->checkTerrainBoundary(x, z);
	vector<Index2> v_idxs, f_pos;
	Index2 cidx, cpos;
	if (mask == 1) {
		for (int i = 0; i < this->m_info.size_h; i++) {
			if (i && i < this->m_info.size_h - 1) {
				cpos = Index2(this->m_info.s_pos.x, this->m_info.s_pos.y - 16 * i);
				cidx = this->m_info.findChunkIndex(cpos.x, cpos.y);
				this->m_info.chunks[cidx.y][cidx.x]->reset();
				this->m_info.chunks[cidx.y][cidx.x]->setPos(cpos);
				this->resetChunk(cidx);
				this->t_system.fillChunk(cidx, cpos);
				this->l_system.fillLight(cidx);
				v_idxs.push_back(cidx);
				cidx = this->m_info.findChunkIndex(this->m_info.ev_pos.x - 16, cpos.y);
				this->m_info.chunks[cidx.y][cidx.x]->render_flag = false;
			}
			cpos = this->m_info.s_pos + Index2(-16, -16 * i);
			f_pos.push_back(cpos);
		}
		this->m_info.sv_pos.x -= 16;
		this->m_info.s_pos.x -= 16;
		this->m_info.ev_pos.x -= 16;
	}
	else if (mask == 2) {
		for (int i = 0; i < this->m_info.size_h; i++) {
			if (i && i < this->m_info.size_h - 1) {
				cpos = Index2(this->m_info.ev_pos.x, this->m_info.s_pos.y - 16 * i);
				cidx = this->m_info.findChunkIndex(cpos.x, cpos.y);
				this->m_info.chunks[cidx.y][cidx.x]->reset();
				this->m_info.chunks[cidx.y][cidx.x]->setPos(cpos);
				this->resetChunk(cidx);
				this->t_system.fillChunk(cidx, cpos);
				this->l_system.fillLight(cidx);
				v_idxs.push_back(cidx);
				cidx = this->m_info.findChunkIndex(this->m_info.sv_pos.x, cpos.y);
				this->m_info.chunks[cidx.y][cidx.x]->render_flag = false;
			}
			cpos.x = this->m_info.ev_pos.x + 16;
			cpos.y = this->m_info.s_pos.y - 16 * i;
			f_pos.push_back(cpos);
		}
		this->m_info.sv_pos.x += 16;
		this->m_info.s_pos.x += 16;
		this->m_info.ev_pos.x += 16;
	}
	else if (mask == 4) {
		for (int i = 0; i < this->m_info.size_w; i++) {
			if (i && i < this->m_info.size_w - 1) {
				cpos = Index2(this->m_info.s_pos.x + 16 * i, this->m_info.s_pos.y);
				cidx = this->m_info.findChunkIndex(cpos.x, cpos.y);
				this->m_info.chunks[cidx.y][cidx.x]->reset();
				this->m_info.chunks[cidx.y][cidx.x]->setPos(cpos);
				this->resetChunk(cidx);
				this->t_system.fillChunk(cidx, cpos);
				this->l_system.fillLight(cidx);
				v_idxs.push_back(cidx);
				cidx = this->m_info.findChunkIndex(cpos.x, this->m_info.ev_pos.y + 16);
				this->m_info.chunks[cidx.y][cidx.x]->render_flag = false;
			}
			cpos = this->m_info.s_pos + Index2(16 * i, 16);
			f_pos.push_back(cpos);
		}
		this->m_info.sv_pos.y += 16;
		this->m_info.s_pos.y += 16;
		this->m_info.ev_pos.y += 16;
	}
	else if (mask == 8) {
		for (int i = 0; i < this->m_info.size_w; i++) {
			if (i && i < this->m_info.size_w - 1) {
				cpos = Index2(this->m_info.s_pos.x + 16 * i, this->m_info.ev_pos.y);
				cidx = this->m_info.findChunkIndex(cpos.x, cpos.y);
				this->m_info.chunks[cidx.y][cidx.x]->reset();
				this->m_info.chunks[cidx.y][cidx.x]->setPos(cpos);
				this->resetChunk(cidx);
				this->t_system.fillChunk(cidx, cpos);
				this->l_system.fillLight(cidx);
				v_idxs.push_back(cidx);
				cidx = this->m_info.getChunkIndex(cpos.x, this->m_info.sv_pos.y);
				this->m_info.chunks[cidx.y][cidx.x]->render_flag = false;
			}
			cpos.x = this->m_info.s_pos.x + 16 * i;
			cpos.y = this->m_info.ev_pos.y - 16;
			f_pos.push_back(cpos);
		}
		this->m_info.sv_pos.y -= 16;
		this->m_info.s_pos.y -= 16;
		this->m_info.ev_pos.y -= 16;
	}
	if (f_pos.size()) {
		for (Index2& pos : f_pos) {
			cidx = this->m_info.getChunkIndex(pos.x, pos.y);
			this->m_info.chunks[cidx.y][cidx.x]->reset();
			this->m_info.chunks[cidx.y][cidx.x]->setPos(pos);
			this->resetChunk(cidx);
			this->t_system.fillChunk(cidx, pos);
			this->l_system.fillLight(cidx);
		}
	}
	if (v_idxs.size()) {
		vector<thread> threads;
		int t = v_idxs.size() / this->thread_cnt;
		int m = v_idxs.size() % this->thread_cnt;
		int st = 0;
		int siz;
		for (int i = 0; i < this->thread_cnt; i++) {
			if (m) {
				siz = t + 1;
				m--;
			}
			else
				siz = t;
			threads.push_back(thread(&Map::chunksSetVerticesAndIndices,
				this, v_idxs, st, st + siz));
			st = st + siz;
		}
		for (int i = 0; i < this->thread_cnt; i++)
			threads[i].join();
	}
}