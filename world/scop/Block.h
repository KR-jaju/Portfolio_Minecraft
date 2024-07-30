#pragma once


enum class Face {
	Top,
	Bottom,
	Front,
	Back,
	Left,
	Right
};

class Block
{
public:
	Block();
	~Block();
	vector<uint32> getAllIndices(uint32 start) const;
	vector<uint32> getFaceIndices(uint32 start) const;
	vector<vec3> getAllVertexPosition(float scale) const;
	vector<vec3> getFaceVertexPos(Face flag, float scale) const;
	vector<vec2> getAllUV(vec2 start, vec2 end) const;
	vector<vec2> getFaceUV(vec2 start, vec2 end, Face flag) const;
	vector<color> getColorForTest(color col, Face flag) const;
};

