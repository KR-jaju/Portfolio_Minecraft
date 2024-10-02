#include "pch.h"
#include "MeshRenderer.h"

MeshRenderer::MeshRenderer(ComPtr<ID3D11Device> device)
	:Super(ComponentType::MeshRenderer), device(device)
{
}

MeshRenderer::~MeshRenderer()
{
}
