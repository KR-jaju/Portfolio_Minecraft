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

enum class Face {
	Top,
	Bottom,
	Front,
	Back,
	Left,
	Right
};

class Chunk
{
public:
	Chunk();
	~Chunk();
	void setVerticesAndIndices();
	void setBlockInChunk(int x, int y, int z, int type);
	int getBlock(int x, int y, int z) const;
	void putChunk(float x, float y, float z);
	void setStartPos(float x, float y, float z);

public: // 미확정 용
	void renderTest();
	void initRenderForTest(HWND hwnd, UINT width, UINT height);
	void setVerticesAndIndicesForTest();
	vector<Vertex> getBlockVertexForTest(
		int x,
		int y,
		int z
	) const;

private:
	vector<VertexUV> getBlockVertexUV(
		int x, 
		int y, 
		int z
	) const;
	vector<uint32> getBlockIndices(
		int x,
		int y,
		int z,
		uint32& start
	) const;
	bool checkBoundary(int x, int y, int z) const;
	vector<bool> checkBlock(int x, int y, int z) const;
	vector<vec3> getBlockFaceVertexPos(
		float move_x,
		float move_y,
		float move_z,
		Face face_flag
	) const;
	vector<vec2> getBlockFaceTexcoords(
		vec2 start, 
		vec2 end, 
		Face face_flag
	) const;
	vector<uint32> getBlockFaceIndices(uint32 start) const;

private:
	int chunk[16][256][16];
	int height_map[16][16];
	weak_ptr<Chunk> front;
	weak_ptr<Chunk> back;
	weak_ptr<Chunk> left;
	weak_ptr<Chunk> right;
	vector<VertexUV> vertices;
	vector<uint32> indices;
	vec3 start_pos;

private:
	// for test
	vector<Vertex> t_vertices;

private: // 미확정 용
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

