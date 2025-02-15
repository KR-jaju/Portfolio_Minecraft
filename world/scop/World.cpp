#include "pch.h"
#include "World.h"
#include "Chunk.h"
#include "DeferredGraphics.h"
#include "Graphics.h"

#include "Pig.h"
#include "TestMob.h"

World::World(HWND hwnd, int width, int height)
	: last_time(std::chrono::high_resolution_clock::now()),
	thread_pool(),
	terrain_db(thread_pool, "DBPath"),
	graphics(hwnd, width, height),
	input_registry(hwnd, width, height),
	event_manager(),
	asset_manager(graphics),
	constant_registry(),
	light_registry(),
	block_data_registry(),
	entity_registry(this->camera, this->input_registry, this->asset_manager),
	chunk_registry(),
	entity_system(this->input_registry, this->entity_registry),
	terrain_system(this->graphics.getDevice(), this->thread_pool, this->constant_registry, this->terrain_db, this->event_manager, this->entity_registry, this->chunk_registry),
	physics_system(this->block_data_registry, this->entity_registry, this->chunk_registry),
	light_system(this->light_registry, this->entity_registry),
	render_system(this->thread_pool, this->camera, this->asset_manager, this->light_registry, this->entity_registry, this->chunk_registry, this->graphics, width, height)
{
	/*this->deff_graphic = 
		make_shared<DeferredGraphics>(hwnd, width, height);*/
	/*this->m_manager = make_shared<Map>(
		size_w,
		size_h,
		fov_chunk,
		thread_cnt,
		hwnd,
		width,
		height
	);*/
	//this->m_manager->setDeffGraphic(this->deff_graphic);
	//this->ega = make_shared<EntityAssetManager>(this->deff_graphic);
	//this->ega->registerGeometry("test", L"pig_model.json");
	//this->ega->registerTexture("test", L"steve.png");
	//shared_ptr<Entity> entity = make_shared<Pig>(this->ega->getGeometry("test"), this->ega->getTexture("test"));
	//uint32 id = this->m_manager->e_info.registerEntity(entity);
	this->entity_registry.createEntity<TestMob>();
}

World::~World()
{
	if (this->thread_pool.joinable())
		this->thread_pool.join(); // 다른 객체들이 소멸되기 전에 스레드풀을 끝내야함
}

void World::update()
{
	using Clock = std::chrono::high_resolution_clock;
	using Duration = std::chrono::duration<double, std::milli>;
	Time now = std::chrono::high_resolution_clock::now();
	float dt = std::chrono::duration<float>(now - this->last_time).count();
	this->last_time = now;
	auto t0 = Clock::now();
	this->input_registry.update();
	auto t1 = Clock::now();
	this->entity_system.update(dt);
	auto t2 = Clock::now();
	this->terrain_system.update();
	auto t3 = Clock::now();
	this->physics_system.update(dt);
	auto t4 = Clock::now();
	this->light_system.update();
	auto t5 = Clock::now();
	this->render_system.update();
	auto t6 = Clock::now();
	std::cout << "Total input update: " << Duration(t1 - t0).count() << " ms\n";
	std::cout << "Total entity update: " << Duration(t2 - t1).count() << " ms\n";
	std::cout << "Total terrain update: " << Duration(t3 - t2).count() << " ms\n";
	std::cout << "Total physics update: " << Duration(t4 - t3).count() << " ms\n";
	std::cout << "Total light update: " << Duration(t5 - t4).count() << " ms\n";
	std::cout << "Total render update: " << Duration(t6 - t5).count() << " ms\n";
	std::cout << "Total total update: " << Duration(t6 - t0).count() << " ms\n\n";
}


#include "framework.h"
#include "scop.h"

LRESULT	World::onEvent(HWND handle, UINT msg, WPARAM w_param, LPARAM l_param)
{
	switch (msg)
	{
	case WM_COMMAND:
	{
		cout << "hello\n";
		int wmId = LOWORD(w_param);
		// 메뉴 선택을 구문 분석합니다:
		switch (wmId)
		{
		case IDM_ABOUT:
			//DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(handle);
			break;
		default:
			return DefWindowProc(handle, msg, w_param, l_param);
		}
	}
	break;

	case WM_INPUT:
		this->input_registry.onInputEvent(l_param);
		break;
	case WM_KEYDOWN:
		if (w_param == 27)
			DestroyWindow(handle);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(handle, msg, w_param, l_param);
	}
	return (0);
}


//void World::render
//(
//	Mat const& cam_view,
//	Mat const& cam_proj,
//	vec3 const& cam_pos,
//	Mat const& shadow_view,
//	Mat const& shadow_proj
//)
//{
//	this->m_manager->r_system.Render(cam_view, 
//		cam_proj, cam_pos, shadow_view, shadow_proj);
//}

//void World::putBlock(
//	vec3 const& ray_pos,
//	vec3 const& ray_dir,
//	int type
//)
//{
//	WorldIndex widx = this->m_manager->m_info.pickBlock(ray_pos, ray_dir);
//	if (widx.flag) {
//		WorldIndex add_idx;
//		Index2& cidx = widx.c_idx;
//		Index3& bidx = widx.b_idx;
//		vec3 const& pos = widx.pos;
//		int dir_flag = -1;
//		if (widx.dir == 0) {
//			if (ray_pos.y > pos.y && pos.y + 1 < 256) {
//				add_idx = this->m_manager->m_info.getBlockIndex(pos.x, pos.y + 1, pos.z);
//				dir_flag = 0;
//			}
//			else if (ray_pos.y < pos.y && pos.y - 1 > -1) {
//				add_idx = this->m_manager->m_info.getBlockIndex(pos.x, pos.y - 1, pos.z);
//				dir_flag = 1;
//			}
//		}
//		else if (widx.dir == 1) {
//			if (ray_pos.z < pos.z) {
//				add_idx = this->m_manager->m_info.getBlockIndex(pos.x, pos.y, pos.z - 1);
//				dir_flag = 2;
//			}
//			else {
//				add_idx = this->m_manager->m_info.getBlockIndex(pos.x, pos.y, pos.z + 1);
//				dir_flag = 3;
//			}
//		}
//		else {
//			if (ray_pos.x < pos.x) {
//				add_idx = this->m_manager->m_info.getBlockIndex(pos.x - 1, pos.y, pos.z);
//				dir_flag = 4;
//			}
//			else {
//				add_idx = this->m_manager->m_info.getBlockIndex(pos.x + 1, pos.y, pos.z);
//				dir_flag = 5;
//			}
//		}
//		if (add_idx.flag) {
//			vector<Index2> v_idx;
//			cidx = add_idx.c_idx;
//			bidx = add_idx.b_idx;
//			Index2 adj_idx;
//			Index2 const& cpos = this->m_manager->m_info.chunks[cidx.y][cidx.x]->chunk_pos;
//			if (bidx.x == 0) {
//				adj_idx = this->m_manager->m_info.findChunkIndex(cpos.x - 16, cpos.y);
//				if (adj_idx.flag && this->m_manager->m_info.findBlock(adj_idx, 15, bidx.y, bidx.z)) {
//					this->m_manager->m_info.chunks[adj_idx.y][adj_idx.x]->vertices_idx = 0;
//					v_idx.push_back(adj_idx);
//				}
//			}
//			if (bidx.x == 15) {
//				adj_idx = this->m_manager->m_info.findChunkIndex(cpos.x + 16, cpos.y);
//				if (adj_idx.flag && this->m_manager->m_info.findBlock(adj_idx, 0, bidx.y, bidx.z)) {
//					this->m_manager->m_info.chunks[adj_idx.y][adj_idx.x]->vertices_idx = 0;
//					v_idx.push_back(adj_idx);
//				}
//			}
//			if (bidx.z == 15) {
//				adj_idx = this->m_manager->m_info.findChunkIndex(cpos.x, cpos.y - 16);
//				if (adj_idx.flag && this->m_manager->m_info.findBlock(adj_idx, bidx.x, bidx.y, 0)) {
//					this->m_manager->m_info.chunks[adj_idx.y][adj_idx.x]->vertices_idx = 0;
//					v_idx.push_back(adj_idx);
//				}
//			}
//			if (bidx.z == 0) {
//				adj_idx = this->m_manager->m_info.findChunkIndex(cpos.x, cpos.y + 16);
//				if (adj_idx.flag && this->m_manager->m_info.findBlock(adj_idx, bidx.x, bidx.y, 15)) {
//					this->m_manager->m_info.chunks[adj_idx.y][adj_idx.x]->vertices_idx = 0;
//					v_idx.push_back(adj_idx);
//				}
//			}
//			this->m_manager->m_info.addBlock(cidx, bidx, type);
//			// TODO: light system 적용
//			for (int y = bidx.y; y >= 0; y--) {
//				this->m_manager->m_info.setLight(cidx, bidx.x, y, bidx.z, 0);
//			}
//			this->m_manager->m_info.chunks[cidx.y][cidx.x]->vertices_idx = 0;
//			int16& max_h = this->m_manager->m_info.chunks[cidx.y][cidx.x]->max_h;
//			max_h = max(max_h, bidx.y + 1);
//			v_idx.push_back(cidx);
//			this->m_manager->chunksSetVerticesAndIndices(v_idx, 0, v_idx.size());
//		}
//	}
//}
//
//void World::deleteBlock(vec3 const& ray_pos, vec3 const& ray_dir)
//{
//	WorldIndex widx = this->m_manager->m_info.pickBlock(ray_pos, ray_dir);
//	if (widx.flag) {
//		this->m_manager->m_info.addBlock(widx.c_idx, widx.b_idx, 0);
//		// TODO light system
//		this->m_manager->l_system.lightPropagationGather(widx.c_idx, widx.b_idx);
//		this->m_manager->m_info.chunks[widx.c_idx.y][widx.c_idx.x]->vertices_idx = 0;
//		vector<Index2> v_idx;
//		v_idx.push_back(widx.c_idx);
//		Index2 cidx;
//		Index2 const& pos = this->m_manager->m_info.chunks[widx.c_idx.y][widx.c_idx.x]->chunk_pos;
//		if (widx.b_idx.x == 0) {
//			cidx = this->m_manager->m_info.findChunkIndex(pos.x - 16, pos.y);
//			if (cidx.flag) {
//				this->m_manager->m_info.chunks[cidx.y][cidx.x]->vertices_idx = 0;
//				v_idx.push_back(cidx);
//			}
//		}
//		if (widx.b_idx.x == 15) {
//			cidx = this->m_manager->m_info.findChunkIndex(pos.x + 16, pos.y);
//			if (cidx.flag) {
//				this->m_manager->m_info.chunks[cidx.y][cidx.x]->vertices_idx = 0;
//				v_idx.push_back(cidx);
//			}
//		}
//		if (widx.b_idx.z == 0) {
//			cidx = this->m_manager->m_info.findChunkIndex(pos.x, pos.y + 16);
//			if (cidx.flag) {
//				this->m_manager->m_info.chunks[cidx.y][cidx.x]->vertices_idx = 0;
//				v_idx.push_back(cidx);
//			}
//		}
//		if (widx.b_idx.z == 15) {
//			cidx = this->m_manager->m_info.findChunkIndex(pos.x, pos.y - 16);
//			if (cidx.flag) {
//				this->m_manager->m_info.chunks[cidx.y][cidx.x]->vertices_idx = 0;
//				v_idx.push_back(cidx);
//			}
//		}
//		this->m_manager->chunksSetVerticesAndIndices(v_idx, 0, v_idx.size());
//	}
//}
//
//pair<int, int> World::getBlock(float x, float y, float z)
//{
//	WorldIndex widx = this->m_manager->m_info.getBlockIndex(x, y, z);
//	pair<int, int> block_info(0, 0);
//	if (widx.flag) {
//		block_info.first = this->m_manager->m_info.findBlock(widx.c_idx, widx.b_idx);
//		block_info.second = this->m_manager->m_info.findLight(
//			widx.c_idx,
//			widx.b_idx.x,
//			widx.b_idx.y,
//			widx.b_idx.z
//		);
//	}
//	return block_info;
//}

//void World::setSightChunk(int cnt)
//{
//	int max_fov = min(this->m_manager->m_info.size_h - 1, 
//		this->m_manager->m_info.size_w - 1);
//	this->m_manager->setSightChunk(min(max_fov, cnt));
//}
//
//void World::userPositionCheck(float x, float z)
//{
//	this->m_manager->userPositionCheck(x, z);
//}
//
//int16 World::getHeight(float x, float z) const
//{
//	WorldIndex w_idx = m_manager->m_info.getBlockIndex(x, 0, z);
//	return m_manager->m_info.findHeight(w_idx.c_idx, w_idx.b_idx);
//}

//void World::testClickLightBlock(
//	vec3 const& ray_pos,
//	vec3 const& ray_dir)
//{
//	WorldIndex widx = this->m_manager->m_info.pickBlock(ray_pos, ray_dir);
//	if (widx.flag) {
//		WorldIndex add_idx;
//		Index2& cidx = widx.c_idx;
//		Index3& bidx = widx.b_idx;
//		vec3 const& pos = widx.pos;
//		int dir_flag = -1;
//		if (widx.dir == 0) {
//			if (ray_pos.y > pos.y && pos.y + 1 < 256)
//				add_idx = this->m_manager->m_info.getBlockIndex(pos.x, pos.y + 1, pos.z);
//			else if (ray_pos.y < pos.y && pos.y - 1 > -1)
//				add_idx = this->m_manager->m_info.getBlockIndex(pos.x, pos.y - 1, pos.z);
//		}
//		else if (widx.dir == 1) {
//			if (ray_pos.z < pos.z)
//				add_idx = this->m_manager->m_info.getBlockIndex(pos.x, pos.y, pos.z - 1);
//			else
//				add_idx = this->m_manager->m_info.getBlockIndex(pos.x, pos.y, pos.z + 1);
//		}
//		else {
//			if (ray_pos.x < pos.x)
//				add_idx = this->m_manager->m_info.getBlockIndex(pos.x - 1, pos.y, pos.z);
//			else
//				add_idx = this->m_manager->m_info.getBlockIndex(pos.x + 1, pos.y, pos.z);
//		}
//		cidx = add_idx.c_idx;
//		bidx = add_idx.b_idx;
//		cout << "block idx: " << bidx.x << ' ' << bidx.y << ' ' << bidx.z << endl;
//		int light = this->m_manager->m_info.findLight(cidx, bidx);
//		cout << "block light: " << light << endl << endl;
//	}
//}
