#pragma once

#include <vector>
#include "AABB.h"
#include "BlockData.h"

/*
TODO: Constant Registry로 대체 예정인데 귀찮음
*/

class BlockDataRegistry
{
public:
	BlockDataRegistry();
	std::vector<AABB> const& getShape(BlockData data);
private:
	std::vector<std::vector<AABB>> block_shape;
};