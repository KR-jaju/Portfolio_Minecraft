#include "pch.h"
#include "DepthMap.h"

DepthMap::DepthMap(ComPtr<ID3D11Device>& device)
{
	this->device = device;
}
