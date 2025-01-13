#pragma once


#include "TPChunk.h"

// 다른 device에서 만들어진 index buffer 사용해도 문제 없는 지 확인

struct Chunk
{
public: // create
	Chunk();
	~Chunk();
	void setGeoRender(
		ComPtr<ID3D11DeviceContext> const& context
	);
	void setShadowRender(
		ComPtr<ID3D11DeviceContext> const& context
	);
	void createGeoBuffer(
		ComPtr<ID3D11Device> const& device,
		vector<VertexGeo> const& vertices
	);
	void createShadowBuffer(
		ComPtr<ID3D11Device> const& device,
		vector<VertexShadow> const& vertices,
		vector<uint32> const& indices
	);
	void reset();
	void setPos(Index2 const& c_pos);

public: // test
	void createGeoIBuffer(
		ComPtr<ID3D11Device> const& device,
		vector<uint32> const& indices
	);
	void setGeoRenderIndices(
		ComPtr<ID3D11DeviceContext> const& context
	);

public:
	int16 max_h = -1;
	uint32 tp_block_cnt = 0;
	vec3 start_pos; // 0 0 0 번째 블록의 가운데 위치
	Index2 chunk_pos; // 청크의 시작위치
	uint32 vertices_idx;
	uint32 shadow_idx;
	bool render_flag;
	TPChunk tp_chunk; // 반투명 처리

private:
	shared_ptr<Buffer<VertexGeo>> geo_vbuffer;
	shared_ptr<Buffer<VertexShadow>> shadow_vbuffer;
	shared_ptr<Buffer<uint32>> shadow_ibuffer;

private: // test
	shared_ptr<Buffer<uint32>> geo_ibuffer;
};

