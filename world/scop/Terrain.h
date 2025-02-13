#pragma once

#include <memory>
#include <variant>

struct Terrain
{
	struct Node
	{
		std::unique_ptr<Node> children[4];
	};
	Node	root;
};
