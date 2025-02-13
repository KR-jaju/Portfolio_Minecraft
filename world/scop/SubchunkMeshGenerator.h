#pragma once

#include "TerrainDB.h"
#include "Future.h"
#include "SubchunkMesh.h"

class SubchunkMeshGenerator
{
public:
	SubchunkMeshGenerator(ThreadPool& thread_pool, std::vector<BlockTextureData> const& block_texture_data);
	ThreadPool::JobID dispatch(std::shared_ptr<Chunk const> const& center, std::shared_ptr<Chunk const> const& east, std::shared_ptr<Chunk const> const& west, std::shared_ptr<Chunk const> const& north, std::shared_ptr<Chunk const> const& south, ivec3 subchunk_idx);
	void drainResult(std::unordered_map<ivec3, SubchunkMesh>& output);
private:
	ThreadPool& thread_pool;
	std::vector<BlockTextureData> const& block_texture_data;
	std::unordered_map<ivec3, SubchunkMesh> results;
	std::mutex result_mutex;
	
	class MeshGenerationTask
	{
	public:
		MeshGenerationTask(std::vector<BlockTextureData> const& block_texture_data, std::shared_ptr<Chunk const> const& center,
			std::shared_ptr<Chunk const> const& east,
			std::shared_ptr<Chunk const> const& west,
			std::shared_ptr<Chunk const> const& north,
			std::shared_ptr<Chunk const> const& south, int subchunk_y);
		SubchunkMesh operator()();
	private:
		std::vector<BlockTextureData> const& block_texture_data;
		std::shared_ptr<Chunk const> center;
		std::shared_ptr<Chunk const> east;
		std::shared_ptr<Chunk const> west;
		std::shared_ptr<Chunk const> north;
		std::shared_ptr<Chunk const> south;
		int subchunk_y;
		std::vector<ChunkVertex> vertices;
		std::vector<uint32> indices;

		BlockData	getBlock(int x, int y, int z) const;

		void generateOpaqueMesh();

		void	buildOpaqueBlock(int x, int y, int z, BlockData block_data, int y_offset);
		void	buildCube(float x, float y, float z, BlockTextureData const& textures);
	};
};