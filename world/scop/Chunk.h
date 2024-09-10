#pragma once


#include "WorldUtils.h"

struct Chunk
{
	shared_ptr<Buffer<VertexBlockUV>> vertex_buffer;
	shared_ptr<Buffer<uint32>> index_buffer;
	vec3 start_pos;
	Index2 chunk_pos;
	uint32 vertices_idx;
	bool render_flag;

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
	void reset();
	void setPos(Index2 const& c_pos);
public: // temp
	void readFile(string const& path);
	void updateFile() const;
	int block_cnt = 0;
	int16 max_h = -1;
};

