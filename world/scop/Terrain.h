#pragma once

#include "Map.h"

// test

class Terrain
{
public:
	Terrain(int size_w, int size_h, 
		HWND hwnd, UINT width, 
		UINT height, int fov_chunk, 
		int thread_cnt
	);
	~Terrain();
	void setSightChunk(int cnt);
	void userPositionCheck(float x, float z);// Map
	int16 getHeight(float x, float z) const;
	void putBlock(vec3 const& ray_pos, vec3 const& ray_dir, int type);
	void deleteBlock(vec3 const& ray_pos, vec3 const& ray_dir);
	pair<int, int> getBlock(float x, float y, float z);

public: // test func & temp func
	void setRenderPipeLine(int flag);
	void DepthRender();
	void Render(
		Mat const& cam_view, 
		Mat const& cam_proj, 
		vec3 const& cam_pos
	);
	void setRender();	

private:
	shared_ptr<Map> m_manager;
	set<string> file_book;
	map<vec3, uint32> object_book;
	map<vec3, shared_ptr<Chunk>> tmp_storage;

private:
	shared_ptr<Graphics> graphic;
	shared_ptr<RasterizerState> rasterizer_state;
	shared_ptr<RasterizerState> depth_rasterizer_state;
	vector <shared_ptr<BlendState>> blend_state_arr;
	shared_ptr<TextureArray> texture_array;
	shared_ptr<SamplerState> sampler_state;
	shared_ptr<SamplerState> depth_sampler_state;
	shared_ptr<VertexShader> vertex_shader;
	shared_ptr<PixelShader> pixel_shader;
	shared_ptr<InputLayout> input_layout;
	shared_ptr<VertexShader> depth_vertex_shader;
	shared_ptr<PixelShader> depth_pixel_shader;
	shared_ptr<InputLayout> depth_input_layout;
};

