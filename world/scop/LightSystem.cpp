#include "pch.h"
#include "LightSystem.h"
#include "MapUtils.h"
#include "Chunk.h"
#include "MyQueue.h"
#include <thread>

LightSystem::LightSystem(MapUtils* minfo, int thread_cnt)
{
	this->m_info = minfo;
	this->thread_cnt = thread_cnt;
	if (this->thread_cnt > 8)
		this->thread_cnt = 8;
}

void LightSystem::lightBFS(int idx) // 청크 하나에 대해 bfs
{
	static const Index3 move_arr[6] = {
		Index3(0, 1, 0),
		Index3(0, -1, 0),
		Index3(0, 0, -1),
		Index3(0, 0, 1),
		Index3(-1, 0, 0),
		Index3(1, 0, 0)
	};

	pair<Index2, Index3> here;
	Index3 next;
	Index2 cpos;
	uint8 light;
	while (this->que[idx].size()) {
		here = this->que[idx].front();
		this->que[idx].pop();
		light = this->m_info->findLight(here.first, here.second);
		if (light <= 1)
			continue;
		for (int i = 0; i < 6; i++) {
			next = here.second + move_arr[i];
			if (this->m_info->inChunkBoundary(next) == false)
				continue;
			if (this->m_info->findBlock(here.first, next) > 0)
				continue;
			if (light - 1 <= this->m_info->findLight(here.first, next))
				continue;
			this->m_info->setLight(here.first, next, light - 1);
			this->que[idx].push(here.first, next);
		}
	}
}

void LightSystem::fillLightThread(
	vector<Index2> const& vec,
	int st,
	int ed,
	int idx
)
{
	if (idx > 8)
		idx = 8;
	for (int i = st; i < ed; i++)
		this->fillLight(vec[i], idx);
}

void LightSystem::checkBoundary(
	Index2 const& c_idx,
	vector<Index2>* cidxs,
	int dir
)
{
	Index2 apz_idx, amz_idx, apx_idx, amx_idx;
	Index2 cpos, tpos;
	Index3 bidx, adj;
	uint8 light, alight;
	cpos = this->m_info->chunks[c_idx.y][c_idx.x]->chunk_pos;
	tpos = cpos + Index2(0, 16);
	apz_idx = this->m_info->findChunkIndex(tpos.x, tpos.y);
	tpos = cpos + Index2(0, -16);
	amz_idx = this->m_info->findChunkIndex(tpos.x, tpos.y);
	tpos = cpos + Index2(16, 0);
	apx_idx = this->m_info->findChunkIndex(tpos.x, tpos.y);
	tpos = cpos + Index2(-16, 0);
	amx_idx = this->m_info->findChunkIndex(tpos.x, tpos.y);
	this->que[0].clear();
	this->que[1].clear();
	int dir_flag = 0;
	if (apz_idx.flag) {
		for (int i = 0; i < 256; i++) {
			for (int j = 0; j < 16; j++) {
				setIndex3(adj, j, i, 15);
				setIndex3(bidx, j, i, 0);
				if (this->m_info->findBlock(c_idx, bidx) > 0)
					continue;
				if (this->m_info->findBlock(apz_idx, adj) > 0)
					continue;
				light = this->m_info->findLight(c_idx, bidx);
				alight = this->m_info->findLight(apz_idx, adj);
				if (light < alight - 1) {
					this->m_info->setLight(c_idx, bidx, alight);
					this->que[0].push(c_idx, bidx);
				}
				else if (light > alight + 1) {
					this->m_info->setLight(apz_idx, adj, light - 1);
					this->que[1].push(apz_idx, adj);
					dir_flag |= 8;
				}
			}
		}
	}
	if (amz_idx.flag) {
		for (int i = 0; i < 256; i++) {
			for (int j = 0; j < 16; j++) {
				setIndex3(adj, j, i, 0);
				setIndex3(bidx, j, i, 15);
				if (this->m_info->findBlock(c_idx, bidx) > 0)
					continue;
				if (this->m_info->findBlock(amz_idx, adj) > 0)
					continue;
				light = this->m_info->findLight(c_idx, bidx);
				alight = this->m_info->findLight(amz_idx, adj);
				if (light < alight - 1) {
					this->m_info->setLight(c_idx, bidx, alight);
					this->que[0].push(c_idx, bidx);
				}
				else if (light > alight + 1) {
					this->m_info->setLight(amz_idx, adj, light - 1);
					this->que[1].push(amz_idx, adj);
					dir_flag |= 4;
				}
			}
		}
	}
	if (apx_idx.flag) {
		for (int i = 0; i < 256; i++) {
			for (int j = 0; j < 16; j++) {
				setIndex3(adj, 0, i, j);
				setIndex3(bidx, 15, i, j);
				if (this->m_info->findBlock(c_idx, bidx) > 0)
					continue;
				if (this->m_info->findBlock(apx_idx, adj) > 0)
					continue;
				light = this->m_info->findLight(c_idx, bidx);
				alight = this->m_info->findLight(apx_idx, adj);
				if (light < alight - 1) {
					this->m_info->setLight(c_idx, bidx, alight);
					this->que[0].push(c_idx, bidx);
				}
				else if (light > alight + 1) {
					this->m_info->setLight(apx_idx, adj, light - 1);
					this->que[1].push(apx_idx, adj);
					dir_flag |= 1;
				}
			}
		}
	}
	if (amx_idx.flag) {
		for (int i = 0; i < 256; i++) {
			for (int j = 0; j < 16; j++) {
				setIndex3(adj, 15, i, j);
				setIndex3(bidx, 0, i, j);
				if (this->m_info->findBlock(c_idx, bidx) > 0)
					continue;
				if (this->m_info->findBlock(amx_idx, adj) > 0)
					continue;
				light = this->m_info->findLight(c_idx, bidx);
				alight = this->m_info->findLight(amx_idx, adj);
				if (light < alight - 1) {
					this->m_info->setLight(c_idx, bidx, alight);
					this->que[0].push(c_idx, bidx);
				}
				else if (light > alight + 1) {
					this->m_info->setLight(amx_idx, adj, light - 1);
					this->que[1].push(amx_idx, adj);
					dir_flag |= 2;
				}
			}
		}
	}
	if ((dir_flag & 1) && (dir & 1))
		cidxs->push_back(apx_idx);
	if ((dir_flag & 2) && (dir & 2))
		cidxs->push_back(amx_idx);
	if ((dir_flag & 4) && (dir & 4))
		cidxs->push_back(amz_idx);
	if ((dir_flag & 8) && (dir & 8))
		cidxs->push_back(apz_idx);
	thread t1;
	thread t2;
	t1 = thread(&LightSystem::lightBFS, this, 0); // 자기 자신 bfs
	t2 = thread(&LightSystem::lightBFS, this, 1); // 인접 청크 bfs
	t1.join();
	t2.join();
}

void LightSystem::createLightMap()
{
	vector<thread> threads;
	vector<Index2> cidxs;
	for (int i = 0; i < this->m_info->size_h; i++) {
		for (int j = 0; j < this->m_info->size_w; j++) {
			Index2 c_pos = this->m_info->s_pos + Index2(j * 16, -i * 16);
			Index2 c_idx;
			c_idx = this->m_info->getChunkIndex(c_pos.x, c_pos.y);
			cidxs.push_back(c_idx);
		}
	}
	int t = cidxs.size() / this->thread_cnt;
	int m = cidxs.size() % this->thread_cnt;
	int st = 0;
	int siz;
	int idx = 0;
	for (int i = 0; i < this->thread_cnt; i++) {
		if (m) {
			siz = t + 1;
			m--;
		}
		else
			siz = t;
		threads.push_back(thread(&LightSystem::fillLightThread,
			this, cidxs, st, st + siz, idx));
		st = st + siz;
		idx++;
	}
	for (int i = 0; i < this->thread_cnt; i++)
		threads[i].join();

	for (int i = 0; i < this->m_info->size_h; i++) {
		for (int j = 0; j < this->m_info->size_w; j++) {
			Index2 c_pos = this->m_info->s_pos + Index2(j * 16, -i * 16);
			Index2 c_idx;
			c_idx = this->m_info->getChunkIndex(c_pos.x, c_pos.y);
			this->checkBoundary(c_idx);
		}
	}
}

void LightSystem::createLightMap(
	vector<Index2>& cidxs,
	int dir
)
{
	vector<thread> threads;
	int t = cidxs.size() / this->thread_cnt;
	int m = cidxs.size() % this->thread_cnt;
	int st = 0;
	int siz;
	int idx = 0;
	for (int i = 0; i < this->thread_cnt; i++) {
		if (m) {
			siz = t + 1;
			m--;
		}
		else
			siz = t;
		threads.push_back(thread(&LightSystem::fillLightThread,
			this, cidxs, st, st + siz, idx));
		st = st + siz;
		idx++;
	}
	for (int i = 0; i < this->thread_cnt; i++)
		threads[i].join();
	size_t cidxs_size = cidxs.size();
	for (int i = 0; i < cidxs_size; i++) {
		this->checkBoundary(cidxs[i], &cidxs, dir);
	}
}

void LightSystem::fillLight(Index2 const& c_idx, int idx)
{
	this->que[idx].clear();
	for (int i = 0; i < 16; i++) {
		for (int j = 0; j < 16; j++) {
			for (int y = 255; y >= 0; y--) {
				if (m_info->findBlock(c_idx, j, y, i) > 0)
					break;
				Index3 bidx(j, y, i);
				m_info->setLight(c_idx, bidx, 15);
				this->que[idx].push(c_idx, bidx);
			}
		}
	}
	this->lightBFS(idx);
}

void LightSystem::resetLight(Index2 const& c_idx)
{
	for (int y = 0; y < 256; y++) {
		for (int z = 0; z < 16; z++) {
			for (int x = 0; x < 16; x++)
				m_info->setLight(c_idx, x, y, z, 0);
		}
	}
	for (int z = 0; z < 16; z++) {
		for (int x = 0; x < 16; x++) {
			for (int y = 255; y >= 0; y--) {
				if (m_info->findBlock(c_idx, x, y, z) > 0)
					break;
				Index3 bidx(x, y, z);
				m_info->setLight(c_idx, x, y, z, 15);
			}
		}
	}
}

// chunk의 light을 다시 계산
void LightSystem::chunkSetLight(Index2 const& chunk_idx)
{
	this->que[0].clear();
	this->resetLight(chunk_idx);
	int max_h = this->m_info->chunks[chunk_idx.y][chunk_idx.x]->max_h;
	for (int z = 0; z < 15; z++) {
		for (int y = 0; y < max_h; y++) {
			for (int x = 0; x < 15; x++)
				if (this->m_info->findLight(chunk_idx, x, y, z) == 15)
					this->que[0].push({chunk_idx, Index3(x, y, z)});
		}
	}
	this->lightBFS(0);
}
