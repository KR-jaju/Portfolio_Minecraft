#pragma once

#include "Asset.h"
#include "Buffer.h"
#include "ConstantBuffer.h"
#include "EntityVertex.h"
#include "Armature.h"
#include "Graphics.h"

class SkinnedMesh : public Asset
{
public:
	static constexpr AssetType getAssetType()
	{
		return AssetType::AnimationClip;
	}

	SkinnedMesh(std::wstring const& path);

	BoneData const& getBindposes() const;
	std::vector<int> const& getBoneParents() const;
	uint32 getBoneCount() const;
	
	void	render(Graphics& graphics);

private:

	std::vector<EntityVertex> vertices;
	std::vector<uint32> indices;
	BoneData bindposes;
	std::vector<int> bone_parents;

	std::unique_ptr<Buffer<EntityVertex>> vertex_buffer;
	std::unique_ptr<Buffer<uint32>> index_buffer;
	std::unique_ptr<ConstantBuffer> bindposes_buffer; // TODO : 이 놈을 어떻게 하면 좋을까
};
