#pragma once

#include "WorldUtils.h"

class DeferredGraphics;
template<typename T> class Buffer;

class Moon
{
public:
	Moon(DeferredGraphics* dgraphic, UINT radius);
	shared_ptr<Buffer<VertexColor>> getVertexBffer();
	shared_ptr<Buffer<uint32>> getIndexBuffer();

private:
	shared_ptr<Buffer<VertexColor>> vbuffer;
	shared_ptr<Buffer<uint32>> ibuffer;
};

