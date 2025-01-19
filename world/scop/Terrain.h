#pragma once

//#include "Map.h"

#include <chrono>

#include "EntitySystem.h"
#include "TerrainSystem.h"
#include "RenderSystem.h"
#include "PhysicsSystem.h"

#include "InputRegistry.h"

#include "AssetManager.h"

#include "BlockDataRegistry.h"
#include "EntityReigstry.h"
#include "ChunkRegistry.h"
#include "ChunkMeshRegistry.h"
#include "TerrainDB.h"

#include "Graphics.h"

#include "WindowCallback.h"

class Terrain : public WindowCallback
{
public:
	Terrain(HWND hwnd, int width, int height);
	void update();
	LRESULT	onEvent(HWND handle, UINT msg, WPARAM w_param, LPARAM l_param);
	//Terrain(int size_w, int size_h, 
	//	HWND hwnd, UINT width, 
	//	UINT height, int fov_chunk, 
	//	int thread_cnt
	//);
	//~Terrain();
	/*void setSightChunk(int cnt);
	void userPositionCheck(float x, float z);
	int16 getHeight(float x, float z) const;
	void putBlock(vec3 const& ray_pos, vec3 const& ray_dir, int type);
	void deleteBlock(vec3 const& ray_pos, vec3 const& ray_dir);
	pair<int, int> getBlock(float x, float y, float z);*/
	//void testClickLightBlock(vec3 const& ray_pos, 
		//vec3 const& ray_dir);
	//void render(
	//	Mat const& cam_view, 
	//	Mat const& cam_proj,
	//	vec3 const& cam_pos,
	//	Mat const& shadow_view,
	//	Mat const& shadow_proj
	//);
private:
	using Time = std::chrono::time_point<std::chrono::high_resolution_clock>;
	Time	last_time;

	TerrainDB terrain_db;

	Graphics graphics;

	InputRegistry	input_registry;
	AssetManager asset_manager;
	TextureRegistry texture_registry;
	BlockDataRegistry block_data_registry;
	EntityRegistry entity_registry;
	ChunkRegistry chunk_registry;
	ChunkMeshRegistry chunk_mesh_registry;

	EntitySystem entity_system;
	TerrainSystem terrain_system;
	PhysicsSystem physics_system;
	RenderSystem render_system;


	//shared_ptr<EntityAssetManager> ega;
	//shared_ptr<EntitySystem> entity_system;
	//shared_ptr<Map> m_manager;
	//shared_ptr<DeferredGraphics> deff_graphic;
	//set<string> file_book;
	//map<vec3, uint32> object_book;
	//map<vec3, shared_ptr<Chunk>> tmp_storage;
};

