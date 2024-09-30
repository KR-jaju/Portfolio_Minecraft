#include "pch.h"
#include "LightSystem.h"
#include "MapUtils.h"
#include "Chunk.h"
#include "MyQueue.h"
#include <thread>

LightSystem::LightSystem(MapUtils* minfo)
{
	this->m_info = minfo;
}

void LightSystem::lightPropagationGather(
	Index2 const& cidx,
	Index3 const& bidx
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
	Index3 nbidx;
	Index2 ncidx = cidx, pos;
	uint8 light;
	uint8 nlight = m_info->findLight(cidx, bidx.x, bidx.y, bidx.z);
	for (int i = 0; i < 6; i++) {
		ncidx = cidx;
		nbidx = bidx + move_arr[i];
		pos = m_info->chunks[cidx.y][cidx.x]->chunk_pos;
		if (nbidx.y > 255)
			light = 15;
		else if (nbidx.y < 0)
			continue;
		else if (nbidx.x < 0) {
			pos = pos + Index2(-16, 0);
			ncidx = m_info->findChunkIndex(pos.x, pos.y);
			if (ncidx.flag == false)
				continue;
			light = m_info->findLight(ncidx, 15, nbidx.y, nbidx.z);
		}
		else if (nbidx.x > 15) {
			pos = pos + Index2(16, 0);
			ncidx = m_info->findChunkIndex(pos.x, pos.y);
			if (ncidx.flag == false)
				continue;
			light = m_info->findLight(ncidx, 0, nbidx.y, nbidx.z);
		}
		else if (nbidx.z < 0) {
			pos = pos + Index2(0, 16);
			ncidx = m_info->findChunkIndex(pos.x, pos.y);
			if (ncidx.flag == false)
				continue;
			light = m_info->findLight(ncidx, nbidx.x, nbidx.y, 15);
		}
		else if (nbidx.z > 15) {
			pos = pos + Index2(0, -16);
			ncidx = m_info->findChunkIndex(pos.x, pos.y);
			if (ncidx.flag == false)
				continue;
			light = m_info->findLight(ncidx, nbidx.x, nbidx.y, 0);
		}
		else
			light = m_info->findLight(ncidx, nbidx.x, nbidx.y, nbidx.z);
		nlight = max(nlight, light);
	}
	m_info->setLight(cidx, bidx.x, bidx.y, bidx.z, nlight);
}

void LightSystem::lightBFS()
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
	while (this->que.size()) {
		here = this->que.front();
		light = this->m_info->findLight(here.first, here.second);
		this->que.pop();
		if (light <= 1)
			continue;
		for (int i = 0; i < 6; i++) {
			next = here.second + move_arr[i];
			if (this->m_info->inChunkBoundary(next) == false)
				continue;
			if (this->m_info->findBlock(here.first, next))
				continue;
			if (light - 1 <= this->m_info->findLight(here.first, next))
				continue;
			this->m_info->setLight(here.first, next, light - 1);
			this->que.push(here.first, next);
		}
	}
}

void LightSystem::checkBoundary(Index2 const& c_idx)
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
	this->que.clear();
	if (apz_idx.flag) {
		for (int i = 0; i < 256; i++) {
			for (int j = 0; j < 16; j++) {
				setIndex3(adj, j, i, 15);
				setIndex3(bidx, j, i, 0);
				if (this->m_info->findBlock(c_idx, bidx))
					continue;
				light = this->m_info->findLight(c_idx, bidx);
				alight = this->m_info->findLight(apz_idx, adj);
				if (light < alight - 1) {
					this->m_info->setLight(c_idx, bidx, alight);
					this->que.push(c_idx, bidx);
				}
			}
		}
	}
	if (amz_idx.flag) {
		for (int i = 0; i < 256; i++) {
			for (int j = 0; j < 16; j++) {
				setIndex3(adj, j, i, 0);
				setIndex3(bidx, j, i, 15);
				if (this->m_info->findBlock(c_idx, bidx))
					continue;
				light = this->m_info->findLight(c_idx, bidx);
				alight = this->m_info->findLight(amz_idx, adj);
				if (light < alight - 1) {
					this->m_info->setLight(c_idx, bidx, alight);
					this->que.push(c_idx, bidx);
				}
			}
		}
	}
	if (apx_idx.flag) {
		for (int i = 0; i < 256; i++) {
			for (int j = 0; j < 16; j++) {
				setIndex3(adj, 0, i, j);
				setIndex3(bidx, 15, i, j);
				if (this->m_info->findBlock(c_idx, bidx))
					continue;
				light = this->m_info->findLight(c_idx, bidx);
				alight = this->m_info->findLight(apx_idx, adj);
				if (light < alight - 1) {
					this->m_info->setLight(c_idx, bidx, alight);
					this->que.push(c_idx, bidx);
				}
			}
		}
	}
	if (amx_idx.flag) {
		for (int i = 0; i < 256; i++) {
			for (int j = 0; j < 16; j++) {
				setIndex3(adj, 15, i, j);
				setIndex3(bidx, 0, i, j);
				if (this->m_info->findBlock(c_idx, bidx))
					continue;
				light = this->m_info->findLight(c_idx, bidx);
				alight = this->m_info->findLight(amx_idx, adj);
				if (light < alight - 1) {
					this->m_info->setLight(c_idx, bidx, alight);
					this->que.push(c_idx, bidx);
				}
			}
		}
	}
	if (this->que.size())
		this->lightBFS();
}



void LightSystem::createLightMap()
{
	for (int i = 0; i < this->m_info->size_h; i++) {
		for (int j = 0; j < this->m_info->size_w; j++) {
			Index2 c_pos = this->m_info->s_pos + Index2(j * 16, -i * 16);
			Index2 c_idx;
			c_idx = this->m_info->getChunkIndex(c_pos.x, c_pos.y);
			this->fillLight(c_idx);
		}
	}
	for (int i = 0; i < this->m_info->size_h; i++) {
		for (int j = 0; j < this->m_info->size_w; j++) {
			Index2 c_pos = this->m_info->s_pos + Index2(j * 16, -i * 16);
			Index2 c_idx;
			c_idx = this->m_info->getChunkIndex(c_pos.x, c_pos.y);
			this->checkBoundary(c_idx);
		}
	}
}

void LightSystem::fillLight(Index2 const& c_idx)
{
	this->que.clear();
	for (int i = 0; i < 16; i++) {
		for (int j = 0; j < 16; j++) {
			for (int y = 255; y >= 0; y--) {
				if (m_info->findBlock(c_idx, j, y, i))
					break;
				Index3 bidx(j, y, i);
				m_info->setLight(c_idx, bidx, 15);
				this->que.push(c_idx, bidx);
			}
		}
	}
	this->lightBFS();
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
				if (m_info->findBlock(c_idx, x, y, z))
					break;
				Index3 bidx(x, y, z);
				m_info->setLight(c_idx, x, y, z, 15);
			}
		}
	}
}