#pragma once

class Chunk;

class Terrain
{
public:
	Terrain();
	~Terrain();

private:
	map<vec3, uint32> object_book;
	Chunk terrain[30][30];

private:
};

