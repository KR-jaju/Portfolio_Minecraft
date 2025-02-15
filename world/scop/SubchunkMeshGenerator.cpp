#include "pch.h"
#include "SubchunkMeshGenerator.h"
#include "Promise.h"

SubchunkMeshGenerator::SubchunkMeshGenerator(ThreadPool& thread_pool, std::vector<BlockTextureData> const& block_texture_data)
	: thread_pool(thread_pool),
	block_texture_data(block_texture_data)
{

}

ThreadPool::JobID	SubchunkMeshGenerator::dispatch(ComPtr<ID3D11Device> device, 
	std::shared_ptr<Chunk const> const& center,
	std::shared_ptr<Chunk const> const& east,
	std::shared_ptr<Chunk const> const& west,
	std::shared_ptr<Chunk const> const& north,
	std::shared_ptr<Chunk const> const& south, 
	ivec2 chunk_idx, std::bitset<16> dirty_info)
{

	ThreadPool::JobID job_id = this->thread_pool.enqueue(ThreadPool::Priority::Normal, [this, device, center, east, west, north, south, chunk_idx, dirty_info]()
		{
			std::unordered_map<int, SubchunkMesh> result(24); // 최대 16개니까 대충 24면 load_factor 최대 0.66
			
			for (int subchunk_y = 0; subchunk_y < 16; ++subchunk_y)
			{
				if (!dirty_info.test(subchunk_y))
					continue;
				MeshGenerationTask task(this->block_texture_data, center, east, west, north, south, subchunk_y);

				result.emplace(subchunk_y, std::move(task(device)));
			}
			std::unique_lock<std::mutex> lock(this->result_mutex);

			this->results.emplace(chunk_idx, std::move(result));
		});
	return (job_id);
}

void SubchunkMeshGenerator::drainResult(std::unordered_map<ivec2, std::unordered_map<int, SubchunkMesh>>& output)
{
	output.clear();
	std::unique_lock<std::mutex> lock(this->result_mutex);

	output.swap(this->results);
}

SubchunkMeshGenerator::MeshGenerationTask::MeshGenerationTask(
	std::vector<BlockTextureData> const& block_texture_data,
	std::shared_ptr<Chunk const> const& center,
	std::shared_ptr<Chunk const> const& east,
	std::shared_ptr<Chunk const> const& west,
	std::shared_ptr<Chunk const> const& north,
	std::shared_ptr<Chunk const> const& south, int subchunk_y)
	: block_texture_data(block_texture_data),
	center(center),
	east(east), west(west),
	north(north), south(south),
	subchunk_y(subchunk_y) {}

SubchunkMesh SubchunkMeshGenerator::MeshGenerationTask::operator()(ComPtr<ID3D11Device> device)
{
	this->generateOpaqueMesh();

	return SubchunkMesh(device, std::move(this->vertices), std::move(this->indices));
}

void SubchunkMeshGenerator::MeshGenerationTask::generateOpaqueMesh()
{
	std::vector<ChunkVertex>& vertices = this->vertices;
	std::vector<uint32>& indices = this->indices;
	int const y_offset = this->subchunk_y * 16;

	for (int y = 0; y < 16; ++y)
	{
		for (int z = 0; z < 16; ++z)
		{
			for (int x = 0; x < 16; ++x)
			{
				BlockData block_data = this->getBlock(x, y + y_offset, z);

				switch (block_data.getId())
				{
				case 0:
					continue;
				default:
					this->buildOpaqueBlock(x, y, z, block_data, y_offset);
				}
			}
		}
	}
}

BlockData SubchunkMeshGenerator::MeshGenerationTask::getBlock(int x, int y, int z) const
{
	if (x < 0)
		return this->west->getBlock(15, y, z);
	else if (x >= 16)
		return this->east->getBlock(0, y, z);
	else if (z < 0)
		return this->south->getBlock(x, y, 15);
	else if (z >= 16)
		return this->north->getBlock(x, y, 0);
	else if (y < 0)
		return (0);
	else if (y >= 256)
		return (0);
	return this->center->getBlock(x, y, z);
}


void	SubchunkMeshGenerator::MeshGenerationTask::buildOpaqueBlock(int x, int y, int z, BlockData block_data, int y_offset)
{
	BlockTextureData textures = this->block_texture_data[block_data.getId()];

	if (this->getBlock(x - 1, y + y_offset, z).isOpaque())
		textures.west = -1;
	if (this->getBlock(x + 1, y + y_offset, z).isOpaque())
		textures.east = -1;
	if (this->getBlock(x, y + y_offset - 1, z).isOpaque())
		textures.down = -1;
	if (this->getBlock(x, y + y_offset + 1, z).isOpaque())
		textures.up = -1;
	if (this->getBlock(x, y + y_offset, z - 1).isOpaque())
		textures.south = -1;
	if (this->getBlock(x, y + y_offset, z + 1).isOpaque())
		textures.north = -1;
	this->buildCube(x, y, z, textures);
}

void	SubchunkMeshGenerator::MeshGenerationTask::buildCube(float x, float y, float z, BlockTextureData const& textures)
{
	std::vector<ChunkVertex>& vertices = this->vertices;
	std::vector<uint32>& indices = this->indices;

	if (textures.east != -1)
	{
		int offset = vertices.size();

		vertices.push_back(ChunkVertex{ vec3(x + 1, y + 1, z + 1),vec2(0, 0),textures.east,static_cast<int>(Direction::East) });
		vertices.push_back(ChunkVertex{ vec3(x + 1, y + 1, z),vec2(1, 0),textures.east,static_cast<int>(Direction::East) });
		vertices.push_back(ChunkVertex{ vec3(x + 1, y, z + 1),vec2(0, 1),textures.east,static_cast<int>(Direction::East) });
		vertices.push_back(ChunkVertex{ vec3(x + 1, y, z),vec2(1, 1),textures.east,static_cast<int>(Direction::East) });

		indices.push_back(offset);
		indices.push_back(offset + 1);
		indices.push_back(offset + 2);
		indices.push_back(offset + 1);
		indices.push_back(offset + 3);
		indices.push_back(offset + 2);
	}
	if (textures.west != -1)
	{
		int offset = vertices.size();

		vertices.push_back(ChunkVertex{ vec3(x, y + 1, z),vec2(0, 0),textures.west,static_cast<int>(Direction::West) });
		vertices.push_back(ChunkVertex{ vec3(x, y + 1, z + 1),vec2(1, 0),textures.west,static_cast<int>(Direction::West) });
		vertices.push_back(ChunkVertex{ vec3(x, y, z),vec2(0, 1),textures.west,static_cast<int>(Direction::West) });
		vertices.push_back(ChunkVertex{ vec3(x, y, z + 1),vec2(1, 1),textures.west,static_cast<int>(Direction::West) });

		indices.push_back(offset);
		indices.push_back(offset + 1);
		indices.push_back(offset + 2);
		indices.push_back(offset + 1);
		indices.push_back(offset + 3);
		indices.push_back(offset + 2);
	}
	if (textures.down != -1)
	{
		int offset = vertices.size();

		vertices.push_back(ChunkVertex{ vec3(x, y, z + 1),vec2(0, 0),textures.down,static_cast<int>(Direction::Down) });
		vertices.push_back(ChunkVertex{ vec3(x + 1, y, z + 1),vec2(1, 0),textures.down,static_cast<int>(Direction::Down) });
		vertices.push_back(ChunkVertex{ vec3(x, y, z),vec2(0, 1),textures.down,static_cast<int>(Direction::Down) });
		vertices.push_back(ChunkVertex{ vec3(x + 1, y, z),vec2(1, 1),textures.down,static_cast<int>(Direction::Down) });

		indices.push_back(offset);
		indices.push_back(offset + 1);
		indices.push_back(offset + 2);
		indices.push_back(offset + 1);
		indices.push_back(offset + 3);
		indices.push_back(offset + 2);
	}
	if (textures.up != -1)
	{
		int offset = vertices.size();

		vertices.push_back(ChunkVertex{ vec3(x, y + 1, z),vec2(0, 0),textures.up,static_cast<int>(Direction::Up) });
		vertices.push_back(ChunkVertex{ vec3(x + 1, y + 1, z),vec2(1, 0),textures.up,static_cast<int>(Direction::Up) });
		vertices.push_back(ChunkVertex{ vec3(x, y + 1, z + 1),vec2(0, 1),textures.up,static_cast<int>(Direction::Up) });
		vertices.push_back(ChunkVertex{ vec3(x + 1, y + 1, z + 1),vec2(1, 1),textures.up,static_cast<int>(Direction::Up) });

		indices.push_back(offset);
		indices.push_back(offset + 1);
		indices.push_back(offset + 2);
		indices.push_back(offset + 1);
		indices.push_back(offset + 3);
		indices.push_back(offset + 2);
	}
	if (textures.south != -1)
	{
		int offset = vertices.size();

		vertices.push_back(ChunkVertex{ vec3(x + 1, y + 1, z),vec2(0, 0),textures.south,static_cast<int>(Direction::South) });
		vertices.push_back(ChunkVertex{ vec3(x, y + 1, z),vec2(1, 0),textures.south,static_cast<int>(Direction::South) });
		vertices.push_back(ChunkVertex{ vec3(x + 1, y, z),vec2(0, 1),textures.south,static_cast<int>(Direction::South) });
		vertices.push_back(ChunkVertex{ vec3(x, y, z),vec2(1, 1),textures.south,static_cast<int>(Direction::South) });

		indices.push_back(offset);
		indices.push_back(offset + 1);
		indices.push_back(offset + 2);
		indices.push_back(offset + 1);
		indices.push_back(offset + 3);
		indices.push_back(offset + 2);
	}
	if (textures.north != -1)
	{
		int offset = vertices.size();

		vertices.push_back(ChunkVertex{ vec3(x, y + 1, z + 1),vec2(0, 0),textures.north,static_cast<int>(Direction::North) });
		vertices.push_back(ChunkVertex{ vec3(x + 1, y + 1, z + 1),vec2(1, 0),textures.north,static_cast<int>(Direction::North) });
		vertices.push_back(ChunkVertex{ vec3(x, y, z + 1),vec2(0, 1),textures.north,static_cast<int>(Direction::North) });
		vertices.push_back(ChunkVertex{ vec3(x + 1, y, z + 1),vec2(1, 1),textures.north,static_cast<int>(Direction::North) });

		indices.push_back(offset);
		indices.push_back(offset + 1);
		indices.push_back(offset + 2);
		indices.push_back(offset + 1);
		indices.push_back(offset + 3);
		indices.push_back(offset + 2);
	}
}
