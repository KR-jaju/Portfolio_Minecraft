#pragma once

#include "ThreadPool.h"

#include "PerlinNoise.h"
#include "EntityReigstry.h"
#include "EventManager.h"
#include "ChunkRegistry.h"
#include "Entity.h"
#include "TerrainDB.h"
#include "ConstantRegistry.h"
#include "SubchunkMeshGenerator.h"
#include "Future.h"

class TerrainSystem
{
public:
	TerrainSystem(ComPtr<ID3D11Device> device, ThreadPool& thread_pool, ConstantRegistry const& constant_registry, TerrainDB& terrain_db, EventManager const& event_manager, EntityRegistry& entity_registry, ChunkRegistry& chunk_registry);

	void	update();
private:
	ComPtr<ID3D11Device> device;
	ThreadPool& thread_pool;
	ConstantRegistry const& constant_registry;
	TerrainDB& terrain_db;
	EventManager const& event_manager;
	EntityRegistry& entity_registry;
	ChunkRegistry& chunk_registry;

	SubchunkMeshGenerator subchunk_mesh_generator;
	std::unordered_map<ivec2, Future<std::shared_ptr<Chunk const>>> chunk_future_buffer; // 로드 중인 청크 선물 버퍼
	std::unordered_map<ivec2, ThreadPool::JobID> pending_mesh_generation_task; // 생성 중인 서브청크 메쉬 작업 ID 버퍼

	void	initChunkWindow();

	void	applyChunkChanges(); // 변화한 청크 적용
	void	updateChunkWindow(ivec2 center); // 청크 윈도우 업데이트(변화만)
	void	updateSubchunkMeshes(); // 서브청크 메쉬 업데이트
};

