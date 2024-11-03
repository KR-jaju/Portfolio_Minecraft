#pragma once


#include "WorldUtils.h"
template<typename T> class Buffer;
class DeferredGraphics;

class Sun
{
public:
	Sun(DeferredGraphics* dgraphic, UINT radius);
	shared_ptr<Buffer<VertexColor>> getVertexBuffer();
	shared_ptr<Buffer<uint32>> getIndexBuffer();

private:
	shared_ptr<Buffer<VertexColor>> vbuffer;
	shared_ptr<Buffer<uint32>> ibuffer;
};

