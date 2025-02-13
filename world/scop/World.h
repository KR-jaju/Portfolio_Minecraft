#pragma once

/*
TODO: World라는 이름보다는 하나의 씬처럼 동작하게 하는게 좋을 것 같다.
*/

#include <chrono>

#include "ThreadPool.h"

#include "EntitySystem.h"
#include "TerrainSystem.h"
#include "LightSystem.h"
#include "RenderSystem.h"
#include "PhysicsSystem.h"

#include "InputRegistry.h"
#include "ConstantRegistry.h"
#include "AssetManager.h"
#include "BlockDataRegistry.h"
#include "EntityReigstry.h"
#include "ChunkRegistry.h"
#include "TerrainDB.h"

#include "Graphics.h"

#include "WindowCallback.h"

class World : public WindowCallback
{
public:
	World(HWND hwnd, int width, int height);
	~World();
	void update();
	LRESULT	onEvent(HWND handle, UINT msg, WPARAM w_param, LPARAM l_param);
	//World(int size_w, int size_h, 
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

	ThreadPool thread_pool;

	TerrainDB terrain_db;

	Graphics graphics;

	InputRegistry	input_registry;
	EventManager event_manager;
	AssetManager asset_manager;
	ConstantRegistry constant_registry;
	LightRegistry light_registry;
	BlockDataRegistry block_data_registry;
	EntityRegistry entity_registry;
	ChunkRegistry chunk_registry;

	EntitySystem entity_system;
	TerrainSystem terrain_system;
	PhysicsSystem physics_system;
	LightSystem light_system;
	RenderSystem render_system;
};

