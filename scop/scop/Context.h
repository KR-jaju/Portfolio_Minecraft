#pragma once

class RenderTarget;

class Context
{
public:
	virtual ~Context() = default;
	virtual void	setViewport(uint32 x, uint32 y, uint32 w, uint32 h) = 0;
	virtual void	setRenderTarget(RenderTarget const& target) = 0; // visitor pattern

	//virtual 
};
/*
Texture2D
Texture2DArray
Texture3D

Buffer

Mesh?
vao
buffer, input layout


*/

