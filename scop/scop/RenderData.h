#pragma once
#include "pch.h"
struct CameraData
{
	Mat mat_view = Mat::Identity;
	Mat mat_projection = Mat::Identity;
};

struct TransformData
{
	Mat mat_world = Mat::Identity;
};

