#pragma once

#include "WorldUtils.h"

class Chunk
{
public:
	Chunk();
	~Chunk();
	void setVerticesAndIndices();
	void updateVerticesAndIndices();
	void setRender(
		shared_ptr<Graphics> graphic,
		shared_ptr<RasterizerState> rasterizer_state, 
		shared_ptr<SamplerState> sampler_state,
		wstring const& vertex_shader_path,
		wstring const& pixel_shader_path,
		shared_ptr<BlendState> blend_state
	);
	void Render(
		Mat view, 
		Mat proj,
		shared_ptr<TextureArray> const& texture_array
	);
	void setBlockInChunk(int x, int y, int z, int16 type);
	void addBlock(Index3, int16 type);
	void deleteBlock(vector<Index3> const& block_arr);
	int getBlock(int x, int y, int z) const;
	void setStartPos(float x, float y, float z);
	vec3 getStartPos() const;
	void setChunk(Chunk* chunk, string const& str);
	int getBlockCnt();
	void readFile(string const& path);
	void updateFile() const;

private:
	vector<VertexBlockUV> getBlockVertexBlockUV(
		int x, 
		int y, 
		int z,
		int type,
		vector<int> const& check_arr
	) const;
	vector<uint32> getBlockIndices(
		int x,
		int y,
		int z,
		uint32& start,
		vector<int> const& check_arr
	) const;
	bool checkBoundary(int x, int y, int z) const;
	vector<int> checkBlock(int x, int y, int z) const;
	vector<int> checkBlockReverse(int x, int y, int z) const;
	vector<vec3> getBlockFacePos(
		float x,
		float y,
		float z,
		Face block_face
	) const;

	vector<vec2> getBlockFaceTexcoord(
		vec2 start,
		vec2 end,
		Face block_face
	) const;
	vector<uint32> getBlockFaceIndices(uint32 start) const;
	
private:
	int16 chunk[16][256][16];
	int block_cnt;
	Chunk* front;
	Chunk* back;
	Chunk* left;
	Chunk* right;
	shared_ptr<Buffer<VertexBlockUV>> vertex_buffer;
	vector<VertexBlockUV> vertices;
	shared_ptr<Buffer<uint32>> index_buffer;
	vector<uint32> indices;
	vector<D3D11_INPUT_ELEMENT_DESC> layout;
	shared_ptr<ConstantBuffer> constant_buffer;
	vec3 start_pos;
	MVP mvp;
	set<Index3> blocks;

private:
	shared_ptr<Graphics> graphic;
	shared_ptr<VertexShader> vertex_shader;
	shared_ptr<PixelShader> pixel_shader;
	shared_ptr<InputLayout> input_layout;
	shared_ptr<SamplerState> sampler_state;
	shared_ptr<RasterizerState> rasterizer_state;
	shared_ptr<BlendState> blend_state;
};

