#pragma once

#include <vector>
#include <set>
#include "ThreadPool.h"
#include "LRUCache.h"
#include "Buffer.h"
#include "ChunkRegistry.h"

#include "SubchunkMeshData.h"

#include <unordered_map>
#include <map>


class ChunkMeshRegistry : public ChunkRegistry::Callback
{
private:
	struct KeyComparator {
		bool operator()(const ivec3& a, const ivec3& b) const
		{
			if (a.x != b.x)
				return a.x < b.x;
			if (a.y != b.y)
				return a.y < b.y;
			return a.z < b.z;
		}
	};
public:
	ChunkMeshRegistry(TextureRegistry& texture_registry, ChunkRegistry& chunk_registry);
	void	onBlockChanged(ivec3 position);
	void	onChunkLoaded(ivec2 chunk_idx);
	void	onChunkUnload(ivec2 chunk_idx);


	void	update();
	std::map<ivec3, SubchunkMeshData, KeyComparator>::iterator begin();
	std::map<ivec3, SubchunkMeshData, KeyComparator>::iterator end();
private:
	TextureRegistry& texture_registry;
	ChunkRegistry const& chunk_registry;

	std::map<ivec3, SubchunkMeshData, KeyComparator> subchunk_mesh_cache;
	std::set<ivec3, KeyComparator> change_set;
	ThreadPool thread_pool;
};