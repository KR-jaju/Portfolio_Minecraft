#pragma once
#include "WorldUtils.h"
#include "MyQueue.h"

class MapUtils;

class LightSystem
{
public:
	LightSystem(MapUtils* minfo);
	
public:
	void createLightMap();
	void fillLight(Index2 const& c_idx);
	void resetLight(Index2 const& c_idx);
	void lightPropagationGather(
		Index2 const& cidx,
		Index3 const& bidx
	);
	void lightBFS();

private:
	void fillLightThread(
		vector<Index2> const& vec, 
		int st, 
		int ed
	);
	void checkBoundary(Index2 const& c_idx);
private:
	MapUtils* m_info;
	MyQueue que;
};

