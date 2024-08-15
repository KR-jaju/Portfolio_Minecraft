#pragma once

class RenderTarget 
{
public:
	virtual ~RenderTarget() = default;
	virtual  void	bind() = 0;
};
