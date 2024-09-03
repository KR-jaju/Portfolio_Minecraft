#pragma once


#include "WorldUtils.h"

class Chunk
{
public: // create
	Chunk();
	~Chunk();
	void setVIBuffer(
		shared_ptr<Graphics> graphic,
		shared_ptr<VertexShader> vertex_shader
	);
	void createVIBuffer(
		shared_ptr<Graphics> graphic,
		vector<VertexBlockUV> const& vertices,
		vector<uint32> const& indices
	);

public: // info
	uint32 getVerticesIdx() const;
	vec3 getChunkPos() const;
	int getBlock(int x, int y, int z) const;
	int getBlock(Index3 const& idx3) const;
	int16 getHeight(int x, int y) const;
	int16 getHeight(Index2 const& idx2);
	bool getRenderFlag();

public: // setting & add & delete
	void setVerticesIdx(uint32 idx);
	void addBlock(Index3 idx3, int16 type);
	void addBlock(int x, int y, int z, int16 type);
	void setHeight(int x, int y, int16 h);
	void setHeight(Index2 const& idx2, int16 h);
	void deleteBlock(vector<Index3> const& block_arr);
	void setStartPos(float x, float y, float z);
	vec3 getStartPos() const;

public: // temp
	void readFile(string const& path);
	void updateFile() const;
	int block_cnt = 0;

private: // new 
	int16 chunk[16 * 16 * 256];
	int16 height_map[16][16];
	shared_ptr<Buffer<VertexBlockUV>> vertex_buffer;
	shared_ptr<Buffer<uint32>> index_buffer;
	vec3 start_pos;
	vec3 chunk_pos;
	uint32 vertices_idx;
	bool render_flag;
};

