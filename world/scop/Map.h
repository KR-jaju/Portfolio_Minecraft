#pragma once

#include "MapUtils.h"
#include "TerrainSystem.h"
#include "LightSystem.h"
#include "RenderSystem.h"

class Chunk;
class Graphic;

class Map
{
public:
	Map(
		int size_w,
		int size_h,
		int fov_chunk,
		int thread_cnt,
		HWND hwnd,
		UINT window_w,
		UINT window_h
	);
	void setDeffGraphic(shared_ptr<DeferredGraphics> defer_graphic);
	void userPositionCheck(float x, float z);
	void chunksSetVerticesAndIndices( // 그림자와 면 생성
		vector<Index2> const& v_idx,
		int st,
		int ed
	);
	void chunkSetVerticesAndIndicesShadow( // 그림자만 생성
		vector<Index2> const& v_idx,
		int st,
		int ed
	);
	void terrainSetVerticesAndIndices();
	void vertexAndIndexGenerator(
		Index2 const& c_idx,
		Index2 const& adj_idx,
		Index3 const& move,
		int dir,
		vector<VertexGeo>& vertices,
		vector<uint32>* indices = nullptr,
		uint32* index = nullptr
	);
	void vertexShadowGenerator(
		Index2 const& c_idx,
		Index2 const& adj_idx,
		Index3 const& move,
		int dir,
		vector<VertexShadow>& vertices,
		vector<uint32>* indices = nullptr,
		uint32* index = nullptr
	);
	void vertexAndIndexGeneratorTP(
		Index2 const& c_idx
	);
	void setSightChunk(int chunk_cnt);

private:
	void resetChunk(Index2 const& c_idx);
	int checkTerrainBoundary(float x, float z) const;
	bool checkLeaves(Index2 const& c_idx, Index3 const& b_idx, int type);
	void threadFunc(vector<Index2>& vec, int dir);

public:
	MapUtils m_info;
	LightSystem l_system;
	TerrainSystem t_system;
	RenderSystem r_system;

private:
	DeferredGraphics* d_graphic;
	int c_fov;
	int thread_cnt;
};

