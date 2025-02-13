#include "pch.h"
#include "ChunkMeshBuilder.h"

ChunkMeshBuilder::ChunkMeshBuilder(ConstantRegistry const& constant_registry, Chunk const& chunk, Chunk const& east, Chunk const& west, Chunk const& north, Chunk const& south)
	: block_texture_data(constant_registry.block_texture_data),
	chunk(chunk),
	east(east), west(west), north(north), south(south)
{

}

void ChunkMeshBuilder::buildOpaqueMesh(std::vector<ChunkVertex>& vertices, std::vector<uint32>& indices)
{
	for (int y = 0; y < 256; ++y)
	{
		for (int z = 0; z < 16; ++z)
		{
			for (int x = 0; x < 16; ++x)
			{
				BlockData block_data = this->getBlock(x, y, z);

				switch (block_data.getId())
				{
				case 0:
					continue;
				default:
					this->buildOpaqueBlock(vertices, indices, x, y, z, block_data);
				}
			}
		}
	}
}

BlockData ChunkMeshBuilder::getBlock(int x, int y, int z) const
{
	if (x < 0)
		return this->west.getBlock(15, y, z);
	else if (x >= 16)
		return this->east.getBlock(0, y, z);
	else if (z < 0)
		return this->south.getBlock(x, y, 15);
	else if (z >= 16)
		return this->north.getBlock(x, y, 0);
	else if (y < 0)
		return (0);
	else if (y >= 256)
		return (0);
	return this->chunk.getBlock(x, y, z);
}

void	ChunkMeshBuilder::buildOpaqueBlock(std::vector<ChunkVertex>& vertices, std::vector<uint32>& indices, int x, int y, int z, BlockData block_data)
{
	BlockTextureData textures = this->block_texture_data[block_data.getId()];

	if (this->getBlock(x - 1, y, z).isOpaque())
		textures.west = -1;
	if (this->getBlock(x + 1, y, z).isOpaque())
		textures.east = -1;
	if (this->getBlock(x, y - 1, z).isOpaque())
		textures.down = -1;
	if (this->getBlock(x, y + 1, z).isOpaque())
		textures.up = -1;
	if (this->getBlock(x, y, z - 1).isOpaque())
		textures.south = -1;
	if (this->getBlock(x, y, z + 1).isOpaque())
		textures.north = -1;
	this->buildCube(vertices, indices, x, y, z, textures);
}

void	ChunkMeshBuilder::buildCube(std::vector<ChunkVertex>& vertices, std::vector<uint32>& indices, float x, float y, float z, BlockTextureData const& textures)
{
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
