#pragma once

#include "AABB.h"

class BlockData
{
public:
	BlockData(unsigned short id = 0, unsigned short tag = 0);
	int	getId() const;
	int getTag() const;
	bool	isOpaque() const;

	void	getBoundingBox(std::vector<AABB>& out);

	bool	operator<(const BlockData& other) const;
private:
	unsigned int data;
};