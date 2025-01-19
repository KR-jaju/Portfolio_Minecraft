#include "pch.h"
#include "ChunkMeshBuilder.h"
#include "TextureRegistry.h"

ChunkMeshBuilder::ChunkMeshBuilder(TextureRegistry& texture_registry, SubchunkSnapshot& snapshot)
	: texture_registry(texture_registry),
	snapshot(snapshot)
{

}




void ChunkMeshBuilder::buildOpaqueMesh(std::vector<ChunkVertex>& vertices, std::vector<uint32>& indices)
{
	for (int y = 0; y < 16; ++y)
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
	SubchunkSnapshot const& snapshot = this->snapshot;

	if (x < 0)
		return snapshot.west[z + y * 16];
	if (x >= 16)
		return snapshot.east[z + y * 16];
	if (y < 0)
		return snapshot.down[x + z * 16];
	if (y >= 16)
		return snapshot.up[x + z * 16];
	if (z < 0)
		return snapshot.south[x + y * 16];
	if (z >= 16)
		return snapshot.north[x + y * 16];
	return (snapshot.center[x + z * 16 + y * 16 * 16]);
}

void	ChunkMeshBuilder::buildOpaqueBlock(std::vector<ChunkVertex>& vertices, std::vector<uint32>& indices, int x, int y, int z, BlockData block_data)
{
	BlockTextureData textures;
	
	//switch (block_id)
	//{
	//case 1:
	//	textures = this->texture_registry.getBlockTextureData(block_id);

	//	break;
	//}

	textures = this->texture_registry.getBlockTextureData(block_data);

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

		vertices.push_back(ChunkVertex{ vec3(x + 1, y + 1, z + 1),vec2(0, 0),textures.east });
		vertices.push_back(ChunkVertex{ vec3(x + 1, y + 1, z),vec2(1, 0),textures.east });
		vertices.push_back(ChunkVertex{ vec3(x + 1, y, z + 1),vec2(0, 1),textures.east });
		vertices.push_back(ChunkVertex{ vec3(x + 1, y, z),vec2(1, 1),textures.east });

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

		vertices.push_back(ChunkVertex{ vec3(x, y + 1, z),vec2(0, 0),textures.west });
		vertices.push_back(ChunkVertex{ vec3(x, y + 1, z + 1),vec2(1, 0),textures.west });
		vertices.push_back(ChunkVertex{ vec3(x, y, z),vec2(0, 1),textures.west });
		vertices.push_back(ChunkVertex{ vec3(x, y, z + 1),vec2(1, 1),textures.west });

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

		vertices.push_back(ChunkVertex{ vec3(x, y, z + 1),vec2(0, 0),textures.down });
		vertices.push_back(ChunkVertex{ vec3(x + 1, y, z + 1),vec2(1, 0),textures.down });
		vertices.push_back(ChunkVertex{ vec3(x, y, z),vec2(0, 1),textures.down });
		vertices.push_back(ChunkVertex{ vec3(x + 1, y, z),vec2(1, 1),textures.down });

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

		vertices.push_back(ChunkVertex{ vec3(x, y + 1, z),vec2(0, 0),textures.up });
		vertices.push_back(ChunkVertex{ vec3(x + 1, y + 1, z),vec2(1, 0),textures.up });
		vertices.push_back(ChunkVertex{ vec3(x, y + 1, z + 1),vec2(0, 1),textures.up });
		vertices.push_back(ChunkVertex{ vec3(x + 1, y + 1, z + 1),vec2(1, 1),textures.up });

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

		vertices.push_back(ChunkVertex{ vec3(x + 1, y + 1, z),vec2(0, 0),textures.south });
		vertices.push_back(ChunkVertex{ vec3(x, y + 1, z),vec2(1, 0),textures.south });
		vertices.push_back(ChunkVertex{ vec3(x + 1, y, z),vec2(0, 1),textures.south });
		vertices.push_back(ChunkVertex{ vec3(x, y, z),vec2(1, 1),textures.south });

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

		vertices.push_back(ChunkVertex{ vec3(x, y + 1, z + 1),vec2(0, 0),textures.north });
		vertices.push_back(ChunkVertex{ vec3(x + 1, y + 1, z + 1),vec2(1, 0),textures.north });
		vertices.push_back(ChunkVertex{ vec3(x, y, z + 1),vec2(0, 1),textures.north });
		vertices.push_back(ChunkVertex{ vec3(x + 1, y, z + 1),vec2(1, 1),textures.north });

		indices.push_back(offset);
		indices.push_back(offset + 1);
		indices.push_back(offset + 2);
		indices.push_back(offset + 1);
		indices.push_back(offset + 3);
		indices.push_back(offset + 2);
	}
}
