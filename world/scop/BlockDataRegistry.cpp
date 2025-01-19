#include "pch.h"
#include "BlockDataRegistry.h"

BlockDataRegistry::BlockDataRegistry()
{
	this->block_shape.push_back({});
	this->block_shape.push_back({ {vec3(0.5f, 0.5f, 0.5f), vec3(0.5f, 0.5f, 0.5f)} });
}

std::vector<AABB> const& BlockDataRegistry::getShape(BlockData data)
{
	return (this->block_shape[data.getId()]);
}