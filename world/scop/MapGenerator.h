#pragma once

#include "PerlinNoise.h"

class MapGenerator
{
public:
	MapGenerator();
	~MapGenerator();


private:
	PerlinNoise noise;
};

