#include "pch.h"
#include "Terrain.h"
#include "Graphics.h"
#include "Chunk.h"

Terrain::Terrain(
	int size_w,
	int size_h,
	HWND hwnd,
	UINT width,
	UINT height,
	int fov_chunk,
	int thread_cnt
)
{
	this->graphic = make_shared<Graphics>(hwnd, width, height);
	this->m_manager = make_shared<Map>(
		size_w,
		size_h,
		fov_chunk,
		thread_cnt,
		this->graphic
	);
}

Terrain::~Terrain()
{
}

void Terrain::setRender(int depth_flag)
{
	if (depth_flag)
		this->m_manager->r_system.setDepthRender();
	this->m_manager->r_system.setRender();
}

void Terrain::setRenderPipeLine(int flag)
{
	if (flag == 0)
		this->m_manager->r_system.setPipeLine();
	else
		this->m_manager->r_system.setDepthPipeLine();
}

void Terrain::DepthRender()
{
	this->m_manager->r_system.depthRender();
}

void Terrain::Render
(
	Mat const& cam_view,
	Mat const& cam_proj,
	vec3 const& cam_pos
)
{
	this->m_manager->r_system.Render(cam_view, cam_proj, cam_pos);
}

void Terrain::putBlock(
	vec3 const& ray_pos,
	vec3 const& ray_dir,
	int type
)
{
	WorldIndex widx = this->m_manager->m_info.pickBlock(ray_pos, ray_dir);
	if (widx.flag) {
		WorldIndex add_idx;
		Index2& cidx = widx.c_idx;
		Index3& bidx = widx.b_idx;
		vec3 const& pos = widx.pos;
		int dir_flag = -1;
		if (widx.dir == 0) {
			if (ray_pos.y > pos.y && pos.y + 1 < 256) {
				add_idx = this->m_manager->m_info.getBlockIndex(pos.x, pos.y + 1, pos.z);
				dir_flag = 0;
			}
			else if (ray_pos.y < pos.y && pos.y - 1 > -1) {
				add_idx = this->m_manager->m_info.getBlockIndex(pos.x, pos.y - 1, pos.z);
				dir_flag = 1;
			}
		}
		else if (widx.dir == 1) {
			if (ray_pos.z < pos.z) {
				add_idx = this->m_manager->m_info.getBlockIndex(pos.x, pos.y, pos.z - 1);
				dir_flag = 2;
			}
			else {
				add_idx = this->m_manager->m_info.getBlockIndex(pos.x, pos.y, pos.z + 1);
				dir_flag = 3;
			}
		}
		else {
			if (ray_pos.x < pos.x) {
				add_idx = this->m_manager->m_info.getBlockIndex(pos.x - 1, pos.y, pos.z);
				dir_flag = 4;
			}
			else {
				add_idx = this->m_manager->m_info.getBlockIndex(pos.x + 1, pos.y, pos.z);
				dir_flag = 5;
			}
		}
		if (add_idx.flag) {
			vector<Index2> v_idx;
			cidx = add_idx.c_idx;
			bidx = add_idx.b_idx;
			Index2 adj_idx;
			Index2 const& cpos = this->m_manager->m_info.chunks[cidx.y][cidx.x]->chunk_pos;
			if (bidx.x == 0) {
				adj_idx = this->m_manager->m_info.findChunkIndex(cpos.x - 16, cpos.y);
				if (adj_idx.flag && this->m_manager->m_info.findBlock(adj_idx, 15, bidx.y, bidx.z)) {
					this->m_manager->m_info.chunks[adj_idx.y][adj_idx.x]->vertices_idx = 0;
					v_idx.push_back(adj_idx);
				}
			}
			if (bidx.x == 15) {
				adj_idx = this->m_manager->m_info.findChunkIndex(cpos.x + 16, cpos.y);
				if (adj_idx.flag && this->m_manager->m_info.findBlock(adj_idx, 0, bidx.y, bidx.z)) {
					this->m_manager->m_info.chunks[adj_idx.y][adj_idx.x]->vertices_idx = 0;
					v_idx.push_back(adj_idx);
				}
			}
			if (bidx.z == 15) {
				adj_idx = this->m_manager->m_info.findChunkIndex(cpos.x, cpos.y - 16);
				if (adj_idx.flag && this->m_manager->m_info.findBlock(adj_idx, bidx.x, bidx.y, 0)) {
					this->m_manager->m_info.chunks[adj_idx.y][adj_idx.x]->vertices_idx = 0;
					v_idx.push_back(adj_idx);
				}
			}
			if (bidx.z == 0) {
				adj_idx = this->m_manager->m_info.findChunkIndex(cpos.x, cpos.y + 16);
				if (adj_idx.flag && this->m_manager->m_info.findBlock(adj_idx, bidx.x, bidx.y, 15)) {
					this->m_manager->m_info.chunks[adj_idx.y][adj_idx.x]->vertices_idx = 0;
					v_idx.push_back(adj_idx);
				}
			}
			this->m_manager->m_info.addBlock(cidx, bidx, type);
			Index3 bidx;
			// TODO: light system 적용
			for (int y = bidx.y; y >= 0; y--) {
				this->m_manager->m_info.setLight(cidx, bidx.x, y, bidx.z, 0);
			}
			this->m_manager->m_info.chunks[cidx.y][cidx.x]->vertices_idx = 0;
			int16& max_h = this->m_manager->m_info.chunks[cidx.y][cidx.x]->max_h;
			max_h = max(max_h, bidx.y + 1);
			v_idx.push_back(cidx);
			this->m_manager->chunksSetVerticesAndIndices(v_idx, 0, v_idx.size());
		}
	}
}

void Terrain::deleteBlock(vec3 const& ray_pos, vec3 const& ray_dir)
{
	WorldIndex widx = this->m_manager->m_info.pickBlock(ray_pos, ray_dir);
	if (widx.flag) {
		this->m_manager->m_info.addBlock(widx.c_idx, widx.b_idx, 0);
		// TODO light system
		this->m_manager->l_system.lightPropagationGather(widx.c_idx, widx.b_idx);
		this->m_manager->m_info.chunks[widx.c_idx.y][widx.c_idx.x]->vertices_idx = 0;
		vector<Index2> v_idx;
		v_idx.push_back(widx.c_idx);
		Index2 cidx;
		Index2 const& pos = this->m_manager->m_info.chunks[widx.c_idx.y][widx.c_idx.x]->chunk_pos;
		if (widx.b_idx.x == 0) {
			cidx = this->m_manager->m_info.findChunkIndex(pos.x - 16, pos.y);
			if (cidx.flag) {
				this->m_manager->m_info.chunks[cidx.y][cidx.x]->vertices_idx = 0;
				v_idx.push_back(cidx);
			}
		}
		if (widx.b_idx.x == 15) {
			cidx = this->m_manager->m_info.findChunkIndex(pos.x + 16, pos.y);
			if (cidx.flag) {
				this->m_manager->m_info.chunks[cidx.y][cidx.x]->vertices_idx = 0;
				v_idx.push_back(cidx);
			}
		}
		if (widx.b_idx.z == 0) {
			cidx = this->m_manager->m_info.findChunkIndex(pos.x, pos.y + 16);
			if (cidx.flag) {
				this->m_manager->m_info.chunks[cidx.y][cidx.x]->vertices_idx = 0;
				v_idx.push_back(cidx);
			}
		}
		if (widx.b_idx.z == 15) {
			cidx = this->m_manager->m_info.findChunkIndex(pos.x, pos.y - 16);
			if (cidx.flag) {
				this->m_manager->m_info.chunks[cidx.y][cidx.x]->vertices_idx = 0;
				v_idx.push_back(cidx);
			}
		}
		this->m_manager->chunksSetVerticesAndIndices(v_idx, 0, v_idx.size());
	}
}

pair<int, int> Terrain::getBlock(float x, float y, float z)
{
	WorldIndex widx = this->m_manager->m_info.getBlockIndex(x, y, z);
	pair<int, int> block_info(0, 0);
	if (widx.flag) {
		block_info.first = this->m_manager->m_info.findBlock(widx.c_idx, widx.b_idx);
		block_info.second = this->m_manager->m_info.findLight(
			widx.c_idx,
			widx.b_idx.x,
			widx.b_idx.y,
			widx.b_idx.z
		);
	}
	return block_info;
}

void Terrain::setSightChunk(int cnt)
{
	int max_fov = min(this->m_manager->m_info.size_h - 1, 
		this->m_manager->m_info.size_w - 1);
	this->m_manager->setSightChunk(min(max_fov, cnt));
}

void Terrain::userPositionCheck(float x, float z)
{
	this->m_manager->userPositionCheck(x, z);
}

int16 Terrain::getHeight(float x, float z) const
{
	WorldIndex w_idx = m_manager->m_info.getBlockIndex(x, 0, z);
	return m_manager->m_info.findHeight(w_idx.c_idx, w_idx.b_idx);
}
