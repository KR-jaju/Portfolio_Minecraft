#pragma once

class RenderTarget 
{
public:
	virtual ~RenderTarget() = default;
	virtual ID3D11Texture2D* getInternalResource() const = 0;
};
