#pragma once

#include "BlockData.h"

struct SubchunkSnapshot
{
	BlockData	center[16 * 16 * 16];
	BlockData	east[16 * 16];
	BlockData	west[16 * 16];
	BlockData	up[16 * 16];
	BlockData	down[16 * 16];
	BlockData	north[16 * 16];
	BlockData	south[16 * 16];
};