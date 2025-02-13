#include "pch.h"
#include "ConstantRegistry.h"

static std::vector<BlockTextureData> initBlockTextureData()
{
	std::vector<BlockTextureData> result;

	result.push_back({ 0, 0, 0, 0, 0, 0 });
	result.push_back({ 1, 1, 1, 1, 1, 1});
	return (result);
}

ConstantRegistry::ConstantRegistry()
	: block_texture_data(initBlockTextureData()),
	load_distance(25),
	render_distance(15),
	simulation_distance(10)
{
	
}
