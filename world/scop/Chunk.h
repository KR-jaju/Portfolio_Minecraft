#pragma once

#include "WorldUtils.h"

class Chunk
{
public:
	Chunk();
	~Chunk();
	void setVerticesAndIndices2();
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
	void getBlockFacePosAndTex(
		int dir, float x, float y, float z, int type);
	void vertexAndIndexGenerator(
		Face const& face, 
		int const& dx,
		int const& dy,
		int const& dz,
		uint32& idx);
	bool checkBoundary(int x, int y, int z) const;
	void getBlockFaceIndices(uint32 start);
	
private:
	int16 chunk[16 * 16 * 256];
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

private:
	shared_ptr<Graphics> graphic;
	shared_ptr<VertexShader> vertex_shader;
	shared_ptr<PixelShader> pixel_shader;
	shared_ptr<InputLayout> input_layout;
	shared_ptr<SamplerState> sampler_state;
	shared_ptr<RasterizerState> rasterizer_state;
	shared_ptr<BlendState> blend_state;
};

