#pragma once

#include "Context.h"

namespace d3d11
{
	class Texture2D {
	public:
		virtual ID3D11Texture2D* getInternalResource() const = 0;
	};
}