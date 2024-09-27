#pragma once

#include "MapUtils.h"
#include "TerrainSystem.h"
#include "LightSystem.h"

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
		shared_ptr<Graphics> graphic
	);
	void userPositionCheck(float x, float z);
	void chunksSetVerticesAndIndices(
		vector<Index2> const& v_idx,
		int st,
		int ed
	);
	void terrainsetVerticesAndIndices();
	void vertexAndIndexGenerator(
		Index2 const& c_idx,
		Index2 const& adj_idx,
		Index3 const& move,
		int dir,
		vector<VertexBlockUV>& vertices,
		vector<uint32>& indices
	);
	void setSightChunk(int chunk_cnt);

private:
	void resetChunk(Index2 const& c_idx);
	int checkTerrainBoundary(float x, float z) const;

public:
	MapUtils m_info;
	LightSystem l_system;
	TerrainSystem t_system;

private:
	shared_ptr<Graphics> graphic;
	int c_fov;
	int thread_cnt;

private:
};

