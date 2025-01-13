#pragma once

#include "WorldUtils.h"

class DeferredGraphics;
class VertexShader;
template<typename T> class Buffer;

class TPChunk
{
public:
	TPChunk();
	~TPChunk();
	void createTPBuffer(ComPtr<ID3D11Device> const& device);
	void setTPBuffer(
		ComPtr<ID3D11DeviceContext> const& context
	);
	void update(ComPtr<ID3D11Device> const& device);
	void reset();

public:
	uint32 vertices_idx = 0;
	bool render_flag = false;
	vector<VertexColor> vertices;
	vector<uint32> indices;

private:
	shared_ptr<Buffer<VertexColor>> tp_vbuffer;
	shared_ptr<Buffer<uint32>> tp_ibuffer;
};

