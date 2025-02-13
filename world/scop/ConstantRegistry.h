#pragma once

struct ConstantRegistry
{
	ConstantRegistry();
	std::vector<BlockTextureData> block_texture_data;
	int load_distance;
	int render_distance;
	int simulation_distance;
};