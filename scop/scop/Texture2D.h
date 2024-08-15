#pragma once

#include "Context.h"

class Texture2D {
public:
	virtual ID3D11Texture2D* getInternalResource() const = 0;
};
