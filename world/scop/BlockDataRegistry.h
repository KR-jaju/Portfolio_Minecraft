#pragma once

#include <vector>
#include "AABB.h"
#include "BlockData.h"

class BlockDataRegistry
{
public:
	BlockDataRegistry();
	std::vector<AABB> const& getShape(BlockData data);
private:
	std::vector<std::vector<AABB>> block_shape;
};