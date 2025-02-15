#pragma once

#include "RenderPass.h"
#include "VertexShader.h"
#include "Cubemap.h"
#include "GeometryPass.h"

class LightingPass
{
public:
	struct Resources
	{
		ComPtr<ID3D11RenderTargetView> rtv;
		ComPtr<ID3D11ShaderResourceView> srv;
	};
	LightingPass(ThreadPool& thread_pool);
	void initialize(ComPtr<ID3D11Device> const& device, AssetManager& asset_manager);

	std::shared_future<void> dispatch(
		ComPtr<ID3D11DeviceContext> const& immediate_context,
		std::mutex& context_mutex,
		std::shared_future<void> const& geometry_result_future,
		GeometryPass::Resources const& geometry_result
	);
	void execute(ComPtr<ID3D11DeviceContext> const& context, GeometryPass::Resources const& geometry_result);
	Resources const& getResources() const;
private:
	ThreadPool& thread_pool;
	ComPtr<ID3D11Device> device;
	Resources resources;

	ComPtr<ID3D11VertexShader>	lighting_vs;
	ComPtr<ID3D11PixelShader>	lighting_ps;

	ComPtr<ID3D11VertexShader>	sky_vs;
	ComPtr<ID3D11PixelShader>	sky_ps;
	ComPtr<ID3D11InputLayout>	sky_il;
	std::vector<vec3> sky_vertices;
	std::vector<uint32> sky_indices;
	ComPtr<ID3D11Buffer> sky_vertex_buffer;
	ComPtr<ID3D11Buffer> sky_index_buffer;
	ComPtr<ID3D11SamplerState> sky_ss;

	std::shared_ptr<Cubemap>	sky_texture;

	void intializeTextures(ComPtr<ID3D11Device> const& device);
	void initializeLightingData(ComPtr<ID3D11Device> const& device);
	void initializeSkyData(ComPtr<ID3D11Device> const& device);
};
