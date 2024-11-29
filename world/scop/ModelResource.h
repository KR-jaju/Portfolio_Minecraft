#pragma once
#include "EntityVertex.h"
#include <vector>
#include "Transform.h"

struct Model
{
	std::vector<EntityVertex> vertices;
	std::vector<uint32> indices;
	std::vector<Mat> bindposes;
	std::vector<Transform> default_pose;
	std::vector<int> topological_order;
	std::vector<int> parent;
};


extern const Model pig_model;
