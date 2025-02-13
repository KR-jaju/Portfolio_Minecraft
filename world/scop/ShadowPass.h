#pragma once

#include "RenderPass.h"
#include "LightRegistry.h"
#include "CameraMatrices.h"
#include "Camera.h"

class ShadowPass : public RenderPass
{
public:
	ShadowPass(Renderer& renderer, RenderingContext& context);
	void execute(RenderingContext& context, RenderGroup const& render_group);
private:
	struct ShadowCasterData
	{
		Mat view_projection;
	};
	Renderer& renderer;
	RenderingContext& context;
	ComPtr<ID3D11DepthStencilView> main_light_dsv[4];

	ConstantBuffer shadow_caster_cb;
	VertexShader shadow_vs;
	InputLayout shadow_il;

	void bind(RenderingContext& context);
	void unbind(RenderingContext& context);
};
