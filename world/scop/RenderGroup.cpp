#include "pch.h"
#include "RenderGroup.h"


RenderGroup::Subchunk::Subchunk(std::reference_wrapper<SubchunkMesh const> mesh, int x, int y, int z)
	: mesh(mesh), x(x), y(y), z(z) {}

RenderGroup::RenderGroup()
{
	this->subchunks.reserve(50 * 50);
}

void	RenderGroup::add(int x, int y, int z, SubchunkMesh const& mesh)
{
	this->subchunks.emplace_back(mesh, x, y, z);
}

void	RenderGroup::add(Subchunk const& subchunk)
{
	this->subchunks.emplace_back(subchunk);
}

std::vector<RenderGroup::Subchunk> const& RenderGroup::getSubchunks() const
{
	return (this->subchunks);
}
