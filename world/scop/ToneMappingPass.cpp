#include "pch.h"
#include "ToneMappingPass.h"

ToneMappingPass::ToneMappingPass(ThreadPool& thread_pool)
	: thread_pool(thread_pool) {}

void ToneMappingPass::execute(ComPtr<ID3D11DeviceContext> const& context, LightingPass::Resources const& lighting)
{
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	context->VSSetShader(this->tone_mapping_vs.Get(), nullptr, 0);
	context->PSSetShader(this->tone_mapping_ps.Get(), nullptr, 0);
	context->PSSetSamplers(0, 1, this->copy_sampler.GetAddressOf());
	context->PSSetShaderResources(0, 1, lighting.srv.GetAddressOf());
	context->OMSetRenderTargets(1, this->resources.rtv.GetAddressOf(), nullptr);
	context->Draw(4, 0);
	context->OMSetRenderTargets(0, nullptr, nullptr);
}

ToneMappingPass::Resources const& ToneMappingPass::getResources() const
{
	return (this->resources);
}

void ToneMappingPass::initialize(ComPtr<ID3D11Device> const& device, AssetManager& asset_manager)
{
	this->initializeTextures(device);
	this->initializeData(device);
}


void ToneMappingPass::initializeTextures(ComPtr<ID3D11Device> const& device)
{
	int const width = 800;
	int const height = 800;

	{// Result
		ComPtr<ID3D11Texture2D>& texture_holder = this->resources.texture;
		ComPtr<ID3D11RenderTargetView>& rtv = this->resources.rtv;
		ComPtr<ID3D11ShaderResourceView>& srv = this->resources.srv;
		D3D11_TEXTURE2D_DESC desc = {};

		desc.Width = width;
		desc.Height = height;
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		desc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
		desc.ArraySize = 1;
		desc.MipLevels = 1;
		desc.SampleDesc.Count = 1;
		desc.SampleDesc.Quality = 0;
		desc.CPUAccessFlags = 0;

		HRESULT hr = device->CreateTexture2D(&desc, nullptr, texture_holder.GetAddressOf());
		CHECK(hr);
		hr = device->CreateRenderTargetView(texture_holder.Get(), nullptr, rtv.GetAddressOf());
		CHECK(hr);
		hr = device->CreateShaderResourceView(texture_holder.Get(), nullptr, srv.GetAddressOf());
		CHECK(hr);
	}
}

void ToneMappingPass::initializeData(ComPtr<ID3D11Device> const& device)
{
	uint32 const compileFlag = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
	ComPtr<ID3DBlob> shader_blob;

	HRESULT hr = D3DCompileFromFile(L"ToneMappingPassVS.hlsl", nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, "main", "vs_5_0", compileFlag, 0, shader_blob.GetAddressOf(), nullptr);
	CHECK(hr);
	hr = device->CreateVertexShader(shader_blob->GetBufferPointer(), shader_blob->GetBufferSize(), nullptr, this->tone_mapping_vs.GetAddressOf());
	CHECK(hr);
	hr = D3DCompileFromFile(L"ToneMappingPassPS.hlsl", nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, "main", "ps_5_0", compileFlag, 0, shader_blob.GetAddressOf(), nullptr);
	CHECK(hr);
	hr = device->CreatePixelShader(shader_blob->GetBufferPointer(), shader_blob->GetBufferSize(), nullptr, this->tone_mapping_ps.GetAddressOf());
	CHECK(hr);
}