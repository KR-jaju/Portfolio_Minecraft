#include "pch.h"
#include "RenderingContext.h"
#include "Graphics.h"
#include "InputLayouts.h"

RenderingContext::RenderingContext(TextureRegistry& texture_registry, EntityRegistry& entity_registry, ChunkMeshRegistry& chunk_mesh_registry, Graphics& graphics, int width, int height)
	: texture_registry(texture_registry),
	entity_registry(entity_registry),
	chunk_mesh_registry(chunk_mesh_registry),
	graphics(graphics),
	viewport_width(width),
	viewport_height(height),
	rasterizer_state(graphics.getDevice(), D3D11_FILL_SOLID, D3D11_CULL_BACK),
	steve(graphics.getDevice(), L"steve.png"),
	camera_data(graphics.getDevice(), graphics.getContext(), Mat::Identity),
	chunk_geometry_vs(graphics.getDevice(), L"GeometryPassVS.hlsl", "main", "vs_5_0"),
	chunk_geometry_ps(graphics.getDevice(), L"GeometryPassPS.hlsl", "main", "ps_5_0"),
	chunk_input_layout(graphics.getDevice(), InputLayouts::layout_chunk, 3, this->chunk_geometry_vs.getBlob()),
	chunk_texture_sampler(graphics.getDevice()),
	entity_geometry_vs(graphics.getDevice(), L"EntityGeometryPassVS.hlsl", "main", "vs_5_0"),
	entity_geometry_ps(graphics.getDevice(), L"EntityGeometryPassPS.hlsl", "main", "ps_5_0"),
	entity_input_layout(graphics.getDevice(), InputLayouts::layout_entity, 6, this->entity_geometry_vs.getBlob())
{
	this->prepareTemporaryTextures();
}

void	RenderingContext::prepareTemporaryTextures()
{
	ComPtr<ID3D11Device> const device = this->graphics.getDevice().Get();
	{ // Create HDR Temporary Textures
		D3D11_TEXTURE2D_DESC desc = {};
		HRESULT hr;

		desc.Width = this->viewport_width;
		desc.Height = this->viewport_height;
		desc.ArraySize = 1;
		desc.MipLevels = 1;
		desc.Format = DXGI_FORMAT_R16G16B16A16_FLOAT;
		desc.SampleDesc.Count = 1;
		desc.SampleDesc.Quality = 0;
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
		desc.CPUAccessFlags = 0;
		desc.MiscFlags = 0;

		{
			ComPtr<ID3D11Texture2D>& texture = this->textures["hdr_temporary[0]"];
			
			hr = device->CreateTexture2D(&desc, nullptr, texture.GetAddressOf());
			CHECK(hr);
			hr = device->CreateRenderTargetView(texture.Get(), nullptr, this->rtvs["hdr_temporary[0]"].GetAddressOf());
			CHECK(hr);
			hr = device->CreateShaderResourceView(texture.Get(), nullptr, this->srvs["hdr_temporary[0]"].GetAddressOf());
			CHECK(hr);
		}
		{
			ComPtr<ID3D11Texture2D>& texture = this->textures["hdr_temporary[1]"];

			hr = device->CreateTexture2D(&desc, nullptr, texture.GetAddressOf());
			CHECK(hr);
			hr = device->CreateRenderTargetView(texture.Get(), nullptr, this->rtvs["hdr_temporary[1]"].GetAddressOf());
			CHECK(hr);
			hr = device->CreateShaderResourceView(texture.Get(), nullptr, this->srvs["hdr_temporary[1]"].GetAddressOf());
			CHECK(hr);
		}
	}
	{ // Create LDR Temporary Textures
		D3D11_TEXTURE2D_DESC desc = {};
		HRESULT hr;

		desc.Width = this->viewport_width;
		desc.Height = this->viewport_height;
		desc.ArraySize = 1;
		desc.MipLevels = 1;
		desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		desc.SampleDesc.Count = 1;
		desc.SampleDesc.Quality = 0;
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
		desc.CPUAccessFlags = 0;
		desc.MiscFlags = 0;

		{
			ComPtr<ID3D11Texture2D>& texture = this->textures["ldr_temporary[0]"];

			hr = device->CreateTexture2D(&desc, nullptr, texture.GetAddressOf());
			CHECK(hr);
			hr = device->CreateRenderTargetView(texture.Get(), nullptr, this->rtvs["ldr_temporary[0]"].GetAddressOf());
			CHECK(hr);
			hr = device->CreateShaderResourceView(texture.Get(), nullptr, this->srvs["ldr_temporary[0]"].GetAddressOf());
			CHECK(hr);
		}
		{
			ComPtr<ID3D11Texture2D>& texture = this->textures["ldr_temporary[1]"];

			hr = device->CreateTexture2D(&desc, nullptr, texture.GetAddressOf());
			CHECK(hr);
			hr = device->CreateRenderTargetView(texture.Get(), nullptr, this->rtvs["ldr_temporary[1]"].GetAddressOf());
			CHECK(hr);
			hr = device->CreateShaderResourceView(texture.Get(), nullptr, this->srvs["ldr_temporary[1]"].GetAddressOf());
			CHECK(hr);
		}
	}
}
