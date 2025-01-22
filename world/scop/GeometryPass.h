#pragma once

#include "RenderPass.h"
#include "Renderer.h"

class GeometryPass : public RenderPass
{
public:
	GeometryPass(Renderer& renderer, RenderingContext& context);
	void execute(RenderingContext& context);
private:
	Renderer& renderer;
	ComPtr<ID3D11RenderTargetView> albedo_metallic_rtv;
	ComPtr<ID3D11RenderTargetView> normal_rtv;
	ComPtr<ID3D11DepthStencilView> dsv;

	ConstantBuffer	model_cb;
	ConstantBuffer	armature_cb;

	void bind(RenderingContext& context);
	void unbind(RenderingContext& context);
};