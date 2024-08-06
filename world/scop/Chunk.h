#pragma once

enum class Face {
	Top,
	Bottom,
	Front,
	Back,
	Left,
	Right
};

class PixelShader;
class VertexShader;
template<typename T> class Buffer;
class Graphics;
class ConstantBuffer;
class RasterizerState;
class BlendState;
class InputLayout;
class SamplerState;

class Chunk
{
public:
	Chunk();
	~Chunk();
	void setVerticesAndIndices();
	void setRender(
		shared_ptr<Graphics> graphic,
		shared_ptr<RasterizerState> reaterizer_state, 
		shared_ptr<SamplerState> sampler_state,
		wstring const& vertex_shader_path,
		wstring const& pixel_shader_path,
		shared_ptr<BlendState> blend_state
	);
	void Render(Mat view, Mat proj);
	void setBlockInChunk(int x, int y, int z, int type);
	int getBlock(int x, int y, int z) const;
	void setStartPos(float x, float y, float z);
	void setLeft(Chunk* chunk);
	void setRight(Chunk* chunk);
	void setFront(Chunk* chunk);
	void setBack(Chunk* chunk);
	int getBlockCnt();

private:
	vector<VertexBlockUV> getBlockVertexBlockUV(
		int x, 
		int y, 
		int z,
		int type
	) const;
	vector<uint32> getBlockIndices(
		int x,
		int y,
		int z,
		uint32& start
	) const;
	bool checkBoundary(int x, int y, int z) const;
	vector<bool> checkBlock(int x, int y, int z) const;
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
	int chunk[16][256][16];
	int height_map[16][16];
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

