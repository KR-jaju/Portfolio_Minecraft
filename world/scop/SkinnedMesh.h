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
	SkinnedMesh(std::wstring const& path);

	Mat const& getBindpose(int bone_idx) const;
	std::vector<int> const& getBoneParent() const;
	

	
	void	render(Graphics& graphics);

private:

	std::vector<EntityVertex> vertices;
	std::vector<uint32> indices;
	BoneData bindposes;
	std::vector<int> bone_parent;

	std::unique_ptr<Buffer<EntityVertex>> vertex_buffer;
	std::unique_ptr<Buffer<uint32>> index_buffer;
	std::unique_ptr<ConstantBuffer> bindposes_buffer;
};
