#pragma once

#include "Buffer.h"
#include "InputLayout.h"
#include "Graphics.h"
#include "ConstantBuffer.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "Texture.h"
#include "RasterizerState.h"
#include "BlendState.h"
#include "SamplerState.h"


class Chunk
{
public:
	Chunk();
	~Chunk();
	void setVertexAndIndex();
	int getBlock(int x, int y, int z) const;
	void setBlock(int x, int y, int z, int type);
	void putChunk(float x, float y, float z);

public: // 미확정 용
	void renderTest();
	void initRender(HWND hwnd, UINT width, UINT height);

private:
	bool checkBoundary(int x, int y, int z) const;
	vector<bool> checkBlock(int x, int y, int z) const;

private:
	int chunk[16][256][16];
	int height_map[16][16];
	weak_ptr<Chunk> front;
	weak_ptr<Chunk> back;
	weak_ptr<Chunk> left;
	weak_ptr<Chunk> right;
	vector<VertexUV> vertices;
	vector<uint32> indices;
	Mat model;

private: // 미확정 용
	// 밖에서 정의됨
	shared_ptr<Graphics> graphic;

	shared_ptr<Buffer<Vertex>> vertex_buffer;
	shared_ptr<Buffer<VertexUV>> vertex_uv_buffer;
	shared_ptr<Buffer<uint32>> index_buffer;
	shared_ptr<InputLayout> input_layout;
	shared_ptr<ConstantBuffer> constant_buffer;
	shared_ptr<VertexShader> vertex_shader;
	shared_ptr<PixelShader> pixel_shader;
	shared_ptr<Texture> texture;
	shared_ptr<RasterizerState> rasterizer_state;
	shared_ptr<SamplerState> sampler_state;
	shared_ptr<BlendState> blend_state;
};

