#pragma once


#include "WorldUtils.h"

class DepthMap;

struct Chunk
{
public: // create
	Chunk();
	~Chunk();
	void setRender(
		shared_ptr<Graphics> graphic,
		shared_ptr<VertexShader> vertex_shader,
		bool shadow_map_flag
	);
	void setDepthRender(
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

private:
	void createDepthMap(shared_ptr<Graphics> graphic);

public:
	int16 max_h = -1;
	vec3 start_pos;
	Index2 chunk_pos;
	uint32 vertices_idx;
	bool render_flag;

private:
	shared_ptr<Buffer<VertexBlockUV>> vertex_buffer;
	shared_ptr<Buffer<uint32>> index_buffer;
	shared_ptr<DepthMap> depth_map;
	MVP light;
};

