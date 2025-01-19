#pragma once

#include "Chunk.h"
#include "TerrainDB.h"
#include "TextureRegistry.h"
#include "EntityReigstry.h"
#include "TerrainGenerator.h"
#include "RenderChunk.h"
#include "ThreadPool.h"

class ChunkRegistry
{
public:
	class Callback
	{
	public:
		virtual ~Callback() = default;
		virtual void	onBlockChanged(ivec3 position) = 0;
		virtual void	onChunkLoaded(ivec2 chunk_idx) = 0;
		virtual void	onChunkUnload(ivec2 chunk_idx) = 0; //TODO: 구현이 필요할 지 미래에 판단
	};
	ChunkRegistry(TerrainDB& db, TextureRegistry& texture_registry, EntityRegistry& entity_registry);
	void	update();

	void	setBlock(int x, int y, int z, BlockData data);
	BlockData		getBlock(int x, int y, int z) const;

	int	getRenderDistance() const;
	int getSimulationDistance() const;

	Chunk const& getChunk(int chunk_x, int chunk_z) const;

	bool	isLoaded(int chunk_x, int chunk_z) const;

	void	addCallback(Callback* callback);
private:
	TerrainDB& terrain_db;
	TextureRegistry& texture_registry;
	EntityRegistry& entity_registry;

	int render_distance; //TODO: 렌더링은 분리가 됐으니 이름도 바꿔야하지만, 비동기 로드로 바뀔 수도 있음
	int simulation_distance;
	std::vector<Chunk> loaded;
	ivec2 center;
	std::vector<Callback*> callback_list;

	TerrainGenerator terrain_generator;

	bool	isLoadedChunk(int chunk_x, int chunk_z);
	bool	isSimulationChunk(int chunk_x, int chunk_z);
	bool	isRenderingChunk(int chunk_x, int chunk_z);

	void	initializeChunks();
	void	updateChunks(ivec2 new_offset);

	void	loadChunk(int chunk_x, int chunk_z, Chunk& dest);
	void	swapChunk(int offset_x, int offset_z, ivec2 new_offset);

	Chunk& getChunk(int chunk_x, int chunk_z);
};