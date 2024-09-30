#include "pch.h"
#include "MyQueue.h"

MyQueue::MyQueue()
{
	this->vec.resize(16 * 16 * 256 * 2);
	this->s_idx = 0;
	this->e_idx = 0;
}

void MyQueue::push(pair<Index2, Index3> const& pair)
{
	this->vec[e_idx].first.x = pair.first.x;
	this->vec[e_idx].first.y = pair.first.y;
	this->vec[e_idx].second.x = pair.second.x;
	this->vec[e_idx].second.y = pair.second.y;
	this->vec[e_idx].second.z = pair.second.z;
	this->e_idx += 1;
}

void MyQueue::push(Index2 const& cidx, Index3 const& bidx)
{
	this->vec[e_idx].first.x = cidx.x;
	this->vec[e_idx].first.y = cidx.y;
	this->vec[e_idx].second.x = bidx.x;
	this->vec[e_idx].second.y = bidx.y;
	this->vec[e_idx].second.z = bidx.z;
	this->e_idx += 1;
}

void MyQueue::push(Index2 const& cidx, int x, int y, int z)
{
	this->vec[e_idx].first.x = cidx.x;
	this->vec[e_idx].first.y = cidx.y;
	this->vec[e_idx].second.x = x;
	this->vec[e_idx].second.y = y;
	this->vec[e_idx].second.z = z;
	this->e_idx += 1;
}

pair<Index2, Index3>& MyQueue::front()
{
	return this->vec[s_idx];
}

void MyQueue::pop()
{
	this->s_idx += 1;
	if (this->s_idx == this->e_idx) {
		this->s_idx = 0;
		this->e_idx = 0;
	}
}

void MyQueue::clear()
{
	this->s_idx = 0;
	this->e_idx = 0;
}

int MyQueue::size()
{
	return this->e_idx - this->s_idx;
}

