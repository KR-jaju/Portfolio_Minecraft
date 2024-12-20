#pragma once
#include "WorldUtils.h"
#include "MyQueue.h"
#include <queue>

class MapUtils;

class LightSystem
{
public:
	LightSystem(MapUtils* minfo, int thread_cnt);
	
public:
	void createLightMap(); // 빛을 모든 chunk에 대해 채움
	void createLightMap( // 빛을 특정청크에 대해 채움
		vector<Index2>& cidxs, // 특정 청크 인덱스
		int dir // 방향
	);
	void resetLight(Index2 const& c_idx);
	void chunkSetLight(Index2 const& chunk_idx); // 청크 1개 light map 계산

private:
	void lightBFS(int idx); // 빛 퍼짐 알고리즘
	void fillLightThread( // [st, ed) 까지 빛 채우기
		vector<Index2> const& vec,
		int st,
		int ed,
		int idx
	);
	void fillLight(Index2 const& c_idx, int idx); // 특정 청크 빛
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

