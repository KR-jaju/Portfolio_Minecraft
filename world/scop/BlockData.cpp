#include "pch.h"

#include "BlockData.h"

BlockData::BlockData(unsigned short id, unsigned short tag)
	: data((tag << 16) | id)
{}

int	BlockData::getId() const
{
	return (this->data & 0xFFFF);
}

int	BlockData::getTag() const
{
	return (this->data >> 16);
}

bool	BlockData::isOpaque() const
{
	if (this->getId() != 0)
		return (true);
	return (false);
}

void	BlockData::getBoundingBox(std::vector<AABB>& out)
{
	switch (this->getId())
	{
	case 0:
		break;
	case 1:
		out.push_back({ vec3(0.5f, 0.5f, 0.5f), vec3(0.5f, 0.5f, 0.5f) });
		break;
	}
}

bool	BlockData::operator<(const BlockData& other) const
{
	return (this->data < other.data);
}