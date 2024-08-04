#pragma once

enum class BlockFace {
	Top,
	Bottom,
	Front,
	Back,
	Left,
	Right
};

class Texture;
class TextureArray;

class Block
{
public:
	Block(int type);
	~Block();

private:
	int type;
};

