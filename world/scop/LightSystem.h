#pragma once
#include "WorldUtils.h"
#include "MyQueue.h"

class MapUtils;

class LightSystem
{
public:
	LightSystem(MapUtils* minfo, int thread_cnt);
	
public:
	void createLightMap();
	void createLightMap(
		vector<Index2>& cidxs,
		int dir
	);
	void resetLight(Index2 const& c_idx);
	void lightPropagationGather(
		Index2 const& cidx,
		Index3 const& bidx
	);
	void lightBFS(int idx);
	void fillLightThread(
		vector<Index2> const& vec,
		int st,
		int ed,
		int idx
	);
	void fillLight(Index2 const& c_idx, int idx);

private:
	void checkBoundary(
		Index2 const& c_idx,
		vector<Index2>* cidxs = nullptr,
		int dir = 0
	);
private:
	MapUtils* m_info;
	MyQueue que[8];
	int thread_cnt;
};

