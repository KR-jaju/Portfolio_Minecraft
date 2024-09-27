#pragma once
#include <queue>

#include "WorldUtils.h"

class MapUtils;

class LightSystem
{
public:
	LightSystem(MapUtils* minfo);
	
public:
	void createLightMap();
	void fillLight(Index2 const& c_idx, Index2 const& c_pos);
	void resetLight(Index2 const& c_idx);
	void lightPropagationGather(
		Index2 const& cidx,
		Index3 const& bidx
	);
	void lightBFS(queue<pair<Index2, Index3>>& que);
private:
	MapUtils* m_info;
};

