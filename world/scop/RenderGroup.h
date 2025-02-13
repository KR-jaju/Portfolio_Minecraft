#pragma once

#include <functional>

class SubchunkMesh;

class RenderGroup
{
public:
	struct Subchunk
	{
		std::reference_wrapper<SubchunkMesh const> mesh;
		int x;
		int y;
		int z;
	};
	RenderGroup();
	void	add(int x, int y, int z, SubchunkMesh const& mesh);
	void	add(Subchunk const& subchunk);
	std::vector<Subchunk> const& getSubchunks() const;
private:
	std::vector<Subchunk> subchunks;
};