#include "pch.h"
#include "TerrainSystem.h"
#include "Chunk.h"
#include <iostream>
#include <chrono>

TerrainSystem::TerrainSystem(ComPtr<ID3D11Device> device, ThreadPool& thread_pool, ConstantRegistry const& constant_registry, TerrainDB& terrain_db, EventManager const& event_manager, EntityRegistry& entity_registry, ChunkRegistry& chunk_registry)
	: device(device),
	thread_pool(thread_pool),
	constant_registry(constant_registry),
	terrain_db(terrain_db),
	event_manager(event_manager),
	entity_registry(entity_registry),
	chunk_registry(chunk_registry),
	subchunk_mesh_generator(thread_pool, constant_registry.block_texture_data)
{
	this->chunk_future_buffer.max_load_factor(0.7);
	this->pending_subchunk_meshes.max_load_factor(0.7);
	
	this->initChunkWindow();
}

void TerrainSystem::update()
{
	Player const& player = this->entity_registry.getPlayer();

	this->applyChunkChanges(); // 변화한 청크 적용
	this->updateChunkWindow(player.getChunkIndex()); // 청크 윈도우 업데이트(변화만)
	this->updateSubchunkMeshes(); // 서브청크 메쉬 업데이트

}

void	TerrainSystem::initChunkWindow() // N * N을 전부 요청 (초기화)
{
	int const simulation_distance = 5;
	int const load_distance = this->chunk_registry.addressing_half_stride;
	ivec2 center = this->chunk_registry.addressing_offset;

	for (int z = center.y - load_distance; z <= center.y + load_distance; ++z)
	{
		for (int x = center.x - load_distance; x <= center.x + load_distance; ++x)
		{
			ivec2 chunk_idx = ivec2(x, z);

			this->chunk_future_buffer.emplace(chunk_idx, std::move(this->terrain_db.loadChunkAsync(chunk_idx))); // 요청해서 선물 버퍼에 저장
		}
	}
}

void	TerrainSystem::applyChunkChanges()
{
	for (auto& pair : this->chunk_registry.changed_chunks)
	{
		this->terrain_db.storeChunk(pair.first, pair.second); // 변화를 적용하기
		//TODO : simulation 거리 안에 있다면 거기에도 적용하기
	}
	this->chunk_registry.changed_chunks.clear(); // 이미 적용했으니 치우기
}

void	TerrainSystem::updateChunkWindow(ivec2 center) // toroidal addressing update
{
	int const simulation_distance = 5;
	int const load_distance = this->chunk_registry.addressing_half_stride;
	ivec2 old_center = this->chunk_registry.addressing_offset;

	this->chunk_registry.addressing_offset = center; // 새로운 중심 업데이트
	for (int z = center.y - load_distance; z <= center.y + load_distance; ++z)
	{
		for (int x = center.x - load_distance; x <= center.x + load_distance; ++x)
		{
			ivec2 chunk_idx = ivec2(x, z);

			if (std::abs(x - old_center.x) <= load_distance && std::abs(z - old_center.y) <= load_distance) // 이전에 있던 부분은 넘어가고
				continue;
			this->chunk_future_buffer.emplace(chunk_idx, std::move(this->terrain_db.loadChunkAsync(chunk_idx))); // 새로 생긴 부분만 요청
		}
	} // toroidal addressing에 맞춰서 청크 로드 요청
	auto chunk_future_it = this->chunk_future_buffer.begin();
	while (chunk_future_it != this->chunk_future_buffer.end())
	{
		ivec2 chunk_idx = chunk_future_it->first;

		// 범위를 벗어난 청크 선물은 제거 (작업 취소)
		if (std::abs(chunk_idx.x - center.x) > load_distance || std::abs(chunk_idx.y - center.y) > load_distance)
		{
			chunk_future_it = this->chunk_future_buffer.erase(chunk_future_it);
			continue;
		}
		// simulation_distance 밖인데 아직 로드가 완료되지 않은 경우 넘어감 (대기 X)
		if ((std::abs(chunk_idx.x - center.x) > simulation_distance || std::abs(chunk_idx.y - center.y) > simulation_distance)
			&& !chunk_future_it->second.isDone())
		{
			++chunk_future_it;
			continue;
		}
		this->chunk_registry.setChunk(chunk_idx, chunk_future_it->second.get());
		for (int y = 0; y < 16; ++y)
			this->chunk_registry.dirty_subchunks.emplace(chunk_idx.x, y, chunk_idx.y);
		if (chunk_idx.x - center.x < load_distance)
			for (int y = 0; y < 16; ++y)
				this->chunk_registry.dirty_subchunks.emplace(chunk_idx.x + 1, y, chunk_idx.y);
		if (chunk_idx.x - center.x > -load_distance)
			for (int y = 0; y < 16; ++y)
				this->chunk_registry.dirty_subchunks.emplace(chunk_idx.x - 1, y, chunk_idx.y);
		if (chunk_idx.y - center.y < load_distance)
			for (int y = 0; y < 16; ++y)
				this->chunk_registry.dirty_subchunks.emplace(chunk_idx.x, y, chunk_idx.y + 1);
		if (chunk_idx.y - center.y > -load_distance)
			for (int y = 0; y < 16; ++y)
				this->chunk_registry.dirty_subchunks.emplace(chunk_idx.x, y, chunk_idx.y - 1);
		chunk_future_it = this->chunk_future_buffer.erase(chunk_future_it); // 실물을 받았으니 선물 버퍼에서 제거
	}
	/*
	로드가 됐다면 일단 자신은 무조건 더티 서브청크에 넣는게 맞음. (메쉬 재생성)
	자신이 방금 로드가 됐기에 주변은 무조건 청크가 없음.
	아래에서 메쉬를 생성할 때 생성 가능성 판단 후 지연 삭제됨(Lazy deletion)
	마지막까지 다 실패하고 마지막에 한번만 성공하게 될거임
	* 로드와 블럭 수정은 다른 이벤트임 *
	*/
}

void TerrainSystem::updateSubchunkMeshes()
{
	using Clock = std::chrono::high_resolution_clock;
	using Duration = std::chrono::duration<double, std::milli>;

	int const simulation_distance = 5;
	int const generation_distance = this->chunk_registry.addressing_half_stride - 1;
	ivec2 offset = this->chunk_registry.addressing_offset;

	for (ivec3 subchunk_idx : this->chunk_registry.dirty_subchunks)
	{
		std::shared_ptr<Chunk const> const& center = this->chunk_registry.getChunk(subchunk_idx.x, subchunk_idx.z);
		std::shared_ptr<Chunk const> const& east = this->chunk_registry.getChunk(subchunk_idx.x + 1, subchunk_idx.z);
		std::shared_ptr<Chunk const> const& west = this->chunk_registry.getChunk(subchunk_idx.x - 1, subchunk_idx.z);
		std::shared_ptr<Chunk const> const& north = this->chunk_registry.getChunk(subchunk_idx.x, subchunk_idx.z + 1);
		std::shared_ptr<Chunk const> const& south = this->chunk_registry.getChunk(subchunk_idx.x, subchunk_idx.z - 1);

		if (center == nullptr || east == nullptr || west == nullptr || north == nullptr || south == nullptr)
			continue;
		ThreadPool::JobID job_id = this->subchunk_mesh_generator.dispatch(this->device, center, east, west, north, south, subchunk_idx);

		if (auto [it, exists] = this->pending_subchunk_meshes.try_emplace(subchunk_idx, job_id); !exists)
		{
			this->thread_pool.cancel(it->second); // 기존 작업 취소
			it->second = job_id;                  // 새로운 작업 ID로 갱신
		}
	}

	std::unordered_map<ivec3, SubchunkMesh> result;
	result.max_load_factor(0.7);

	this->subchunk_mesh_generator.drainResult(result);
	for (auto& pair : result)
	{
		this->chunk_registry.setSubchunkMesh(pair.first, std::move(pair.second));
		this->pending_subchunk_meshes.erase(pair.first);
	}
	this->chunk_registry.dirty_subchunks.clear();
}


//void TerrainSystem::updateSubchunkMeshes()
//{
//	using Clock = std::chrono::high_resolution_clock;
//	using Duration = std::chrono::duration<double, std::milli>;
//
//
//	int const simulation_distance = 5;
//	int const generation_distance = this->chunk_registry.addressing_half_stride - 1;
//	ivec2 offset = this->chunk_registry.addressing_offset;
//
//	auto t0 = Clock::now();
//	for (ivec3 subchunk_idx : this->chunk_registry.dirty_subchunks) // 메쉬 생성이 필요한 모든 서브청크 인덱스
//	{
//		std::shared_ptr<Chunk const> const& center = this->chunk_registry.getChunk(subchunk_idx.x, subchunk_idx.z);
//		if (center == nullptr)
//			continue;
//		std::shared_ptr<Chunk const> const& east = this->chunk_registry.getChunk(subchunk_idx.x + 1, subchunk_idx.z);
//		std::shared_ptr<Chunk const> const& west = this->chunk_registry.getChunk(subchunk_idx.x - 1, subchunk_idx.z);
//		std::shared_ptr<Chunk const> const& north = this->chunk_registry.getChunk(subchunk_idx.x, subchunk_idx.z + 1);
//		std::shared_ptr<Chunk const> const& south = this->chunk_registry.getChunk(subchunk_idx.x, subchunk_idx.z - 1);
//
//		if (east == nullptr || west == nullptr || north == nullptr || south == nullptr)
//			continue;
//
//		ThreadPool::JobID job_id = this->subchunk_mesh_generator.dispatch(this->device, center, east, west, north, south, subchunk_idx);
//
//		// 기존 메쉬 생성 작업이 있다면 중단 처리 / 덮어쓰기
//		if (auto [it, exists] = this->pending_subchunk_meshes.try_emplace(subchunk_idx, job_id); !exists)
//		{
//			this->thread_pool.cancel(it->second); // 기존 작업 취소
//			it->second = job_id;                  // 새로운 작업 ID로 갱신
//		}
//	}
//	std::unordered_map<ivec3, SubchunkMesh> result;
//	auto t1 = Clock::now();
//	result.max_load_factor(0.7);
//	this->subchunk_mesh_generator.drainResult(result);
//	for (auto& pair : result)
//	{
//		this->chunk_registry.setSubchunkMesh(pair.first, std::move(pair.second));
//		this->pending_subchunk_meshes.erase(pair.first); // 더 이상 대기 중이 아님
//	}
//	auto t2 = Clock::now();
//
//	std::cout << "Total dispatch: " << Duration(t1 - t0).count() << " ms, count:" << this->chunk_registry.dirty_subchunks.size() << "\n";
//	std::cout << "Total setSubchunkMesh: " << Duration(t2 - t1).count() << " ms, count:" << result.size() << "\n\n";
//
//	this->chunk_registry.dirty_subchunks.clear(); // TODO: 나중에 재사용할 수도 있으니 필요에 따라 조정
//}



//void TerrainSystem::fillChunk(Index2 const& c_idx, Index2 const& c_pos)
//{
	//float x, sy, z;
	//float offset = 0.000001f;
	//int16& max_h = this->m_info->chunks[c_idx.y][c_idx.x]->max_h;
	//for (int i = 0; i < 16; i++) {
	//	z = (c_pos.y - i + offset) / 32.f;
	//	for (int j = 0; j < 16; j++) {
	//		x = (c_pos.x + j + offset) / 32.f;
	//		double h_ = this->perlin_noise.getNoise2D(x, z, 3, 0.5);
	//		int16 h = static_cast<int16>((h_ + 0.8) * 0.5 * 50.f);
	//		max_h = max(max_h, h);
	//		this->m_info->setHeight(c_idx, j, i, h);
	//		for (int y = 0; y < h; y++) {
	//			sy = y / 16.f;
	//			h_ = this->perlin_noise.getNoise3D(x, sy, z, 3, 0.5);
	//			if (h_ < 0.1 || y < 1)
	//				this->m_info->addBlock(c_idx, j, y, i, 1);
	//		}
	//	}
	//}
//}
//
//void TerrainSystem::createHeightMap()
//{
//	for (int i = 0; i < this->m_info->size_h; i++) {
//		for (int j = 0; j < this->m_info->size_w; j++) {
//			Index2 c_pos = this->m_info->s_pos + Index2(j * 16, -i * 16);
//			Index2 c_idx;
//			c_idx = this->m_info->getChunkIndex(c_pos.x, c_pos.y);
//			this->m_info->chunks[c_idx.y][c_idx.x] = make_shared<Chunk>();
//			this->m_info->chunks[c_idx.y][c_idx.x]->start_pos =
//				vec3(c_pos.x + 0.5f, 0.5f, c_pos.y - 0.5f);
//			this->m_info->chunks[c_idx.y][c_idx.x]->chunk_pos = c_pos;
//			this->fillChunk(c_idx, c_pos);
//		}
//	}
//}