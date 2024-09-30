#pragma once

#include "WorldUtils.h"

class MyQueue
{
public:
	MyQueue();
	void push(pair<Index2, Index3> const& pair);
	void push(Index2 const& cidx, Index3 const& bidx);
	void push(Index2 const& cidx, int x, int y, int z);
	pair<Index2, Index3>& front();
	void pop();
	void clear();
	int size();
private:
	vector<pair<Index2, Index3>> vec;
	int s_idx;
	int e_idx;
};

