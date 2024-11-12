#pragma once
#include "EntityVertex.h"
#include <vector>

struct Model
{
	vector<EntityVertex> vertices;
	vector<uint32> indices;
	vector<Mat> bindposes;
	vector<Mat> default_pose;
};


extern const Model humanoid_model;
