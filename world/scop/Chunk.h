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
		vector<VertexGeo> const& vertices
	);
	void createShadowBuffer(
		ComPtr<ID3D11Device> device,
		vector<VertexShadow> const& vertices,
		vector<uint32> const& indices
	);
	void reset();
	void setPos(Index2 const& c_pos);

public: // test
	void createGeoIBuffer(
		ComPtr<ID3D11Device> device,
		vector<uint32> const& indices
	);
	void setGeoRender(
		ComPtr<ID3D11DeviceContext> const& context,
		shared_ptr<VertexShader> const& v_shader,
		int test_flag
	);

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
	shared_ptr<Buffer<uint32>> shadow_ibuffer;

private: // test
	shared_ptr<Buffer<uint32>> geo_ibuffer;
};

