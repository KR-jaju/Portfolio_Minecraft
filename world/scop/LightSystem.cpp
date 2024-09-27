#include "pch.h"
#include "LightSystem.h"
#include "MapUtils.h"
#include "Chunk.h"

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

void LightSystem::lightBFS(queue<pair<Index2, Index3>>& que)
{
	static const Index3 move_arr[6] = {
		Index3(0, 1, 0),
		Index3(0, -1, 0),
		Index3(0, 0, -1),
		Index3(0, 0, 1),
		Index3(-1, 0, 0),
		Index3(1, 0, 0)
	};
	queue<pair<Index2, Index3>> tque;
	Index2 cidx, ncidx, cpos;
	Index3 bidx, nbidx;
	int light;
	bool flag;
	while (que.size() || tque.size()) {
		if (que.size()) {
			cidx = que.front().first;
			bidx = que.front().second;
			que.pop();
			flag = false;
		}
		else {
			cidx = tque.front().first;
			bidx = tque.front().second;
			tque.pop();
			flag = true;
		}
		light = m_info->findLight(cidx, bidx.x, bidx.y, bidx.z);
		if (light <= 0)
			continue;
		for (int i = 0; i < 6; i++) {
			ncidx = cidx;
			nbidx = bidx + move_arr[i];
			if (m_info->inChunkBoundary(nbidx.x, nbidx.y, nbidx.z) == false) {
				if (nbidx.y < 0 || nbidx.y > 255)
					continue;
				else {
					cpos = m_info->chunks[cidx.y][cidx.x]->chunk_pos;
					if (nbidx.x < 0) {
						nbidx.x = 15;
						cpos = cpos + Index2(-16, 0);
					}
					else if (nbidx.x > 15) {
						nbidx.x = 0;
						cpos = cpos + Index2(16, 0);
					}
					else if (nbidx.z < 0) {
						nbidx.z = 15;
						cpos = cpos + Index2(0, 16);
					}
					else {
						nbidx.z = 0;
						cpos = cpos + Index2(0, -16);
					}
					ncidx = m_info->findChunkIndex(cpos.x, cpos.y);
					if (ncidx.flag == false)
						continue;
					if (m_info->findBlock(ncidx, nbidx))
						continue;
					if (light - 1 <= m_info->findLight(ncidx, nbidx.x, nbidx.y, nbidx.z))
						continue;
					m_info->setLight(ncidx, nbidx.x, nbidx.y, nbidx.z, light - 1);
				}
			}
			if (m_info->findBlock(ncidx, nbidx))
				continue;
			if (light - 1 <= m_info->findLight(ncidx, nbidx.x, nbidx.y, nbidx.z))
				continue;
			m_info->setLight(ncidx, nbidx.x, nbidx.y, nbidx.z, light - 1);
			if (flag == false)
				tque.push(make_pair(ncidx, nbidx));
			else
				que.push(make_pair(ncidx, nbidx));
		}
	}
}

void LightSystem::createLightMap()
{
	for (int i = 0; i < this->m_info->size_h; i++) {
		for (int j = 0; j < this->m_info->size_w; j++) {
			Index2 c_pos = this->m_info->s_pos + Index2(j * 16, -i * 16);
			Index2 c_idx;
			c_idx = this->m_info->getChunkIndex(c_pos.x, c_pos.y);
			this->fillLight(c_idx, c_pos);
		}
	}
}

void LightSystem::fillLight(Index2 const& c_idx, Index2 const& c_pos)
{
	for (int i = 0; i < 16; i++) {
		for (int j = 0; j < 16; j++) {
			for (int y = 255; y >= 0; y--) {
				if (m_info->findBlock(c_idx, j, y, i))
					break;
				Index3 bidx(j, y, i);
				m_info->setLight(c_idx, j, y, i, 15);
			}
		}
	}
}

void LightSystem::resetLight(Index2 const& c_idx)
{
	for (int y = 0; y < 256; y++) {
		for (int z = 0; z < 16; z++) {
			for (int x = 0; x < 16; x++)
				m_info->setLight(c_idx, x, y, z, 0);
		}
	}
	//queue<pair<Index2, Index3>> que;
	for (int z = 0; z < 16; z++) {
		for (int x = 0; x < 16; x++) {
			for (int y = 255; y >= 0; y--) {
				if (m_info->findBlock(c_idx, x, y, z))
					break;
				Index3 bidx(x, y, z);
				m_info->setLight(c_idx, x, y, z, 15);
				//que.push(make_pair(c_idx, bidx));
			}
		}
	}
	//this->lightBFS(que);
}