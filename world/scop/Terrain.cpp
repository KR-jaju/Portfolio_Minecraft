#include "pch.h"
#include "Terrain.h"
#include "Chunk.h"
#include "DeferredGraphics.h"
#include "Graphics.h"
#include "Block.h"

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
	this->deff_graphic = 
		make_shared<DeferredGraphics>(hwnd, width, height);
	this->m_manager = make_shared<Map>(
		size_w,
		size_h,
		fov_chunk,
		thread_cnt,
		hwnd,
		width,
		height
	);
	this->m_manager->setDeffGraphic(this->deff_graphic);
}

Terrain::~Terrain()
{
}

void Terrain::Render
(
	Mat const& cam_view,
	Mat const& cam_proj,
	vec3 const& cam_pos
)
{
	this->m_manager->r_system.Render(cam_view, 
		cam_proj, cam_pos);
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
		if (widx.dir == 0) {
			if (ray_pos.y > pos.y && pos.y + 1 < 256) {
				add_idx = this->m_manager->
					m_info.getBlockIndex(pos.x, pos.y + 1, pos.z);
			}
			else if (ray_pos.y < pos.y && pos.y - 1 > -1) {
				add_idx = this->
					m_manager->m_info.getBlockIndex(pos.x, pos.y - 1, pos.z);
			}
		}
		else if (widx.dir == 1) {
			if (ray_pos.z < pos.z) {
				add_idx = this->m_manager->
					m_info.getBlockIndex(pos.x, pos.y, pos.z - 1);
			}
			else {
				add_idx = this->m_manager->
					m_info.getBlockIndex(pos.x, pos.y, pos.z + 1);
			}
		}
		else {
			if (ray_pos.x < pos.x) {
				add_idx = this->
					m_manager->m_info.getBlockIndex(pos.x - 1, pos.y, pos.z);
			}
			else {
				add_idx = this->
					m_manager->m_info.getBlockIndex(pos.x + 1, pos.y, pos.z);
			}
		}
		if (add_idx.flag) {
			vector<Index2> v_idx;
			cidx = add_idx.c_idx;
			bidx = add_idx.b_idx;
			Index2 adj_idx;
			Index2 const& cpos = 
				this->m_manager->m_info.chunks[cidx.y][cidx.x]->chunk_pos;
			
			if (type < 0) {
				this->m_manager->m_info.addBlock(cidx, bidx, type);
				Chunk& chunk = *(this->m_manager->m_info.chunks[cidx.y][cidx.x]);
				chunk.tp_block_cnt += 1;
				vector<VertexColor>& vertices = chunk.tp_chunk.vertices;
				vector<uint32>& indices = chunk.tp_chunk.indices;
				uint32& idx = chunk.tp_chunk.vertices_idx;
				int16& max_h = chunk.max_h;
				max_h = max(max_h, bidx.y + 1);
				vec4 col;
				if (type == -3)
					col = vec4(1, 0, 0, 0.3);
				for (int i = 0; i < 6; i++) {
					Block::addBlockFacePosAndCol(
						chunk.start_pos, bidx.x, bidx.y, bidx.z, i, 
							type, vertices);
					Block::addBlockFaceIndices(idx, indices);
					idx += 4;
				}
				chunk.tp_chunk.update(this->deff_graphic->getDevice());
				return;
			}
			
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
			this->m_manager->m_info.setLight(cidx, bidx, 0);
			this->m_manager->m_info.chunks[cidx.y][cidx.x]->vertices_idx = 0;
			int16& max_h = this->m_manager->m_info.chunks[cidx.y][cidx.x]->max_h;
			max_h = max(max_h, bidx.y + 1);
			this->m_manager->l_system.chunkSetLight(cidx);
			v_idx.push_back(cidx);
			this->m_manager->chunksSetVerticesAndIndices(v_idx, 0, v_idx.size());
		}
	}
}

void Terrain::deleteBlock(vec3 const& ray_pos, vec3 const& ray_dir)
{
	WorldIndex widx = this->m_manager->m_info.pickBlock(ray_pos, ray_dir);
	if (widx.flag) {
		int type = this->m_manager->m_info.findBlock(widx.c_idx, widx.b_idx);
		this->m_manager->m_info.addBlock(widx.c_idx, widx.b_idx, 0);
		int16& max_h = 
			this->m_manager->m_info.chunks[widx.c_idx.y][widx.c_idx.x]->max_h;
		max_h = max(max_h, widx.b_idx.y - 1);
		
		if (type < 0) { // 반투명인 경우
			Chunk& chunk = 
				*(this->m_manager->m_info.chunks[widx.c_idx.y][widx.c_idx.x]);
			chunk.tp_block_cnt -= 1;
			chunk.tp_chunk.reset();
			this->m_manager->vertexAndIndexGeneratorTP(widx.c_idx);
			chunk.tp_chunk.update(this->deff_graphic->getDevice());
			return;
		}
		this->m_manager->l_system.chunkSetLight(widx.c_idx);
		this->m_manager->
			m_info.chunks[widx.c_idx.y][widx.c_idx.x]->vertices_idx = 0;
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

void Terrain::testClickLightBlock(
	vec3 const& ray_pos,
	vec3 const& ray_dir)
{
	WorldIndex widx = this->m_manager->m_info.pickBlock(ray_pos, ray_dir);
	if (widx.flag) {
		WorldIndex add_idx;
		Index2& cidx = widx.c_idx;
		Index3& bidx = widx.b_idx;
		vec3 const& pos = widx.pos;
		int dir_flag = -1;
		if (widx.dir == 0) {
			if (ray_pos.y > pos.y && pos.y + 1 < 256)
				add_idx = this->m_manager->m_info.getBlockIndex(pos.x, pos.y + 1, pos.z);
			else if (ray_pos.y < pos.y && pos.y - 1 > -1)
				add_idx = this->m_manager->m_info.getBlockIndex(pos.x, pos.y - 1, pos.z);
		}
		else if (widx.dir == 1) {
			if (ray_pos.z < pos.z)
				add_idx = this->m_manager->m_info.getBlockIndex(pos.x, pos.y, pos.z - 1);
			else
				add_idx = this->m_manager->m_info.getBlockIndex(pos.x, pos.y, pos.z + 1);
		}
		else {
			if (ray_pos.x < pos.x)
				add_idx = this->m_manager->m_info.getBlockIndex(pos.x - 1, pos.y, pos.z);
			else
				add_idx = this->m_manager->m_info.getBlockIndex(pos.x + 1, pos.y, pos.z);
		}
		cidx = add_idx.c_idx;
		bidx = add_idx.b_idx;
		cout << "chunk idx: " << cidx.y << ' ' << cidx.x << endl;
		cout << "block idx: " << bidx.x << ' ' << bidx.y << ' ' << bidx.z << endl;
		int light = this->m_manager->m_info.findLight(cidx, bidx);
		cout << "block light: " << light << endl << endl;
	}
}
