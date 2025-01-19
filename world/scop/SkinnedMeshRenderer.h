#pragma once

#include "SkinnedMesh.h"
#include "ConstantBuffer.h"

class SkinnedMeshRenderer
{
public:
	void	render(SkinnedMesh& mesh);
private:
	std::unique_ptr<ConstantBuffer> bone_buffer;
	std::unique_ptr<ConstantBuffer> bindpose_buffer;
};
