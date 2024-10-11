#pragma once


#include "WorldUtils.h"

class DepthMap;
class Graphics;
class DeferedGraphics;
class VertexShader;
template <typename T> class Buffer;

// 다른 device에서 만들어진 index buffer 사용해도 문제 없는 지 확인

struct Chunk
{
public: // create
	Chunk();
	~Chunk();
	void setGeoRender(
		ComPtr<ID3D11DeviceContext> context,
		shared_ptr<VertexShader> vertex_shader
	);
	void setShadowRender(
		ComPtr<ID3D11DeviceContext> context,
		shared_ptr<VertexShader> vertex_shader
	);
	void createGeoBuffer(
		ComPtr<ID3D11Device> device,
		vector<VertexGeo> const& vertices,
		vector<uint32> const& indices
	);
	void createShadowBuffer(
		ComPtr<ID3D11Device> device,
		vector<VertexShadow> const& vertices,
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
	uint32 shadow_idx;
	bool render_flag;

private:
	shared_ptr<Buffer<VertexGeo>> geo_vbuffer;
	shared_ptr<Buffer<VertexShadow>> shadow_vbuffer;
	shared_ptr<Buffer<uint32>> geo_ibuffer;
	shared_ptr<Buffer<uint32>> shadow_ibuffer;

public: // temp
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

private: //tmp
	shared_ptr<Buffer<VertexBlockUV>> vertex_buffer;
	shared_ptr<Buffer<uint32>> index_buffer;
	MVP light;
	shared_ptr<DepthMap> depth_map;
};

