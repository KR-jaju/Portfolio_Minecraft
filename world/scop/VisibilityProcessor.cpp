#include "pch.h"
#include "VisibilityProcessor.h"

#include "SubchunkMesh.h"
#include "InputLayouts.h"

//VisibilityProcessor::VisibilityProcessor(RenderGroup const& input)
//    : render_group(input) {}
//
//VisibilityProcessor::VisibilityProcessor(RenderGroup&& input)
//    : render_group(std::move(input)) {}

void VisibilityProcessor::initialize(ComPtr<ID3D11Device> const& device, AssetManager& asset_manager, int width, int height)
{
	this->intializeTextures(device, width, height);
	this->initializeSubchunkData(device);
	this->initializeHiZData(device, width, height);
	this->width = width;
	this->height = height;
}

void VisibilityProcessor::process(ComPtr<ID3D11DeviceContext> const& context, RenderGroup const& render_group, Camera const& camera, RenderGroup const& occluders, ComPtr<ID3D11Buffer> const& camera_info_cb)
{
    this->render_group = render_group; // 복사
    this->applyFrustumCulling(camera.getFrustum());
	this->applyOcclusionCulling(context, occluders, camera_info_cb);
}

void VisibilityProcessor::intializeTextures(ComPtr<ID3D11Device> const& device, int width, int height)
{
	int const max_mip_level = std::max<int>(std::log2(width), std::log2(height)) + 1;

	this->hi_z_srv.resize(max_mip_level);
	this->hi_z_uav.resize(max_mip_level);
	{// Depth-Stencil
		ComPtr<ID3D11Texture2D>& texture = this->depth_texture;
		ComPtr<ID3D11DepthStencilView>& dsv = this->depth_dsv;
		D3D11_TEXTURE2D_DESC desc = {};
		D3D11_DEPTH_STENCIL_VIEW_DESC dsv_desc = {};

		desc.Width = width;
		desc.Height = height;
		desc.ArraySize = 1;
		desc.MipLevels = 1;
		desc.Format = DXGI_FORMAT_R32_TYPELESS;
		desc.SampleDesc.Count = 1;
		desc.SampleDesc.Quality = 0;
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		desc.CPUAccessFlags = 0;
		desc.MiscFlags = 0;

		dsv_desc.Format = DXGI_FORMAT_D32_FLOAT;
		dsv_desc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		dsv_desc.Texture2D.MipSlice = 0;

		HRESULT hr = device->CreateTexture2D(&desc, nullptr, texture.GetAddressOf());
		CHECK(hr);
		hr = device->CreateDepthStencilView(texture.Get(), &dsv_desc, dsv.GetAddressOf());
		CHECK(hr);
	}
	{
		ComPtr<ID3D11Texture2D>& texture = this->hi_z_texture;
		ComPtr<ID3D11RenderTargetView>& rtv = this->hi_z_rtv;
		D3D11_TEXTURE2D_DESC desc = {};

		desc.Width = width;
		desc.Height = height;
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.Format = DXGI_FORMAT_R32_FLOAT;
		desc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS;
		desc.ArraySize = 1;
		desc.MipLevels = 0;
		desc.SampleDesc.Count = 1;
		desc.SampleDesc.Quality = 0;
		desc.CPUAccessFlags = 0;

		HRESULT hr = device->CreateTexture2D(&desc, nullptr, texture.GetAddressOf());
		CHECK(hr);
		hr = device->CreateRenderTargetView(texture.Get(), nullptr, rtv.GetAddressOf());
		CHECK(hr);
	}
	for (int level = 0; level < max_mip_level; ++level)
	{
		ComPtr<ID3D11Texture2D>& texture = this->hi_z_texture;
		ComPtr<ID3D11ShaderResourceView>& srv = this->hi_z_srv[level];
		ComPtr<ID3D11UnorderedAccessView>& uav = this->hi_z_uav[level];
		D3D11_SHADER_RESOURCE_VIEW_DESC srv_desc = {};
		D3D11_UNORDERED_ACCESS_VIEW_DESC uav_desc = {};

		srv_desc.Format = DXGI_FORMAT_R32_FLOAT; // SRV용 포맷
		srv_desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srv_desc.Texture2D.MostDetailedMip = level;
		srv_desc.Texture2D.MipLevels = 1;

		uav_desc.Format = DXGI_FORMAT_R32_FLOAT;
		uav_desc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2D;
		uav_desc.Texture2D.MipSlice = level;

		HRESULT hr = device->CreateShaderResourceView(texture.Get(), &srv_desc, srv.GetAddressOf());
		CHECK(hr);
		hr = device->CreateUnorderedAccessView(texture.Get(), &uav_desc, uav.GetAddressOf());
		CHECK(hr);
	}
}

void	VisibilityProcessor::initializeSubchunkData(ComPtr<ID3D11Device> const& device)
{
	{// VS, IL
		uint32 const compile_flag = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
		ComPtr<ID3DBlob> blob;

		HRESULT hr = D3DCompileFromFile(L"DepthPassSubchunkVS.hlsl", nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, "main", "vs_5_0", compile_flag, 0, blob.GetAddressOf(), nullptr);
		CHECK(hr);
		hr = device->CreateVertexShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, this->subchunk_vs.GetAddressOf());
		CHECK(hr);
		hr = device->CreateInputLayout(InputLayouts::layout_chunk, 4, blob->GetBufferPointer(), blob->GetBufferSize(), this->subchunk_il.GetAddressOf());
		CHECK(hr); //TODO : 필요없는 정보 제거 가능
		hr = D3DCompileFromFile(L"DepthPassSubchunkPS.hlsl", nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, "main", "ps_5_0", compile_flag, 0, blob.GetAddressOf(), nullptr);
		CHECK(hr);
		hr = device->CreatePixelShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, this->subchunk_ps.GetAddressOf());
		CHECK(hr);
	}
	{// CB
		Mat const initial_data = Mat::identity();
		ComPtr<ID3D11Buffer> subchunk_cb;
		D3D11_BUFFER_DESC desc = {};
		D3D11_SUBRESOURCE_DATA data = {};

		desc.Usage = D3D11_USAGE_DYNAMIC;
		desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		desc.ByteWidth = 16; // (x, y, z, padding)
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		data.pSysMem = &initial_data;
		device->CreateBuffer(&desc, &data, this->subchunk_cb.GetAddressOf());
	}
	{// RS
		D3D11_RASTERIZER_DESC desc = {};

		desc.FillMode = D3D11_FILL_SOLID;
		desc.CullMode = D3D11_CULL_BACK;
		desc.FrontCounterClockwise = false;
		HRESULT hr = device->CreateRasterizerState(&desc, this->subchunk_rs.GetAddressOf());
		CHECK(hr);
	}
}

void VisibilityProcessor::initializeHiZData(ComPtr<ID3D11Device> const& device, int width, int height)
{
	{
		uint32 const compile_flag = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
		ComPtr<ID3DBlob> blob;

		HRESULT hr = D3DCompileFromFile(L"HiZPassCS.hlsl", nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, "main", "cs_5_0", compile_flag, 0, blob.GetAddressOf(), nullptr);
		CHECK(hr);
		hr = device->CreateComputeShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, this->hi_z_cs.GetAddressOf());
		CHECK(hr);
	}
	{ // CB
		ComPtr<ID3D11Buffer> subchunk_cb;
		D3D11_BUFFER_DESC desc = {};
		D3D11_SUBRESOURCE_DATA data = {};

		desc.Usage = D3D11_USAGE_DYNAMIC;
		desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		desc.ByteWidth = 16; // (input_width, input_height, output_width, output_height)
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		HRESULT hr = device->CreateBuffer(&desc, nullptr, this->hi_z_cb.GetAddressOf());
		CHECK(hr);
	}
	{
		int const max_mip_level = std::max<int>(std::log2(width), std::log2(height)) + 1;
		ComPtr<ID3D11DeviceContext> context;

		HRESULT hr = device->CreateDeferredContext(0, &context);
		CHECK(hr);

		context->CSSetShader(this->hi_z_cs.Get(), nullptr, 0);

		for (int level = 1; level < max_mip_level; level++)
		{
			int src_width = std::max<int>(1, width >> (level - 1));
			int src_height = std::max<int>(1, height >> (level - 1));
			int dst_width = std::max<int>(1, width >> level);
			int dst_height = std::max<int>(1, height >> level);

			int threadGroupX = (dst_width + 7) / 8;
			int threadGroupY = (dst_height + 7) / 8;

			context->CSSetShaderResources(0, 1, this->hi_z_srv[level - 1].GetAddressOf());
			context->CSSetUnorderedAccessViews(0, 1, this->hi_z_uav[level].GetAddressOf(), nullptr);
			
			D3D11_MAPPED_SUBRESOURCE subresource = {};
			HRESULT hr = context->Map(this->hi_z_cb.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &subresource);
			CHECK(hr);
			ivec4& info = *reinterpret_cast<ivec4*>(subresource.pData);
			
			info.x = src_width;
			info.y = src_height;
			info.z = dst_width;
			info.w = dst_height;

			context->Unmap(this->hi_z_cb.Get(), 0);
			context->CSSetConstantBuffers(0, 1, this->hi_z_cb.GetAddressOf());

			context->Dispatch(threadGroupX, threadGroupY, 1);
			ID3D11ShaderResourceView* nullSRV[1] = { nullptr };
			context->CSSetShaderResources(0, 1, nullSRV);

			ID3D11UnorderedAccessView* nullUAV[1] = { nullptr };
			context->CSSetUnorderedAccessViews(0, 1, nullUAV, nullptr);
		}
		context->FinishCommandList(FALSE, this->hi_z_commands.GetAddressOf());
	}
}



void VisibilityProcessor::applyOcclusionCulling(ComPtr<ID3D11DeviceContext> const& context, RenderGroup const& occluders, ComPtr<ID3D11Buffer> const& camera_info_cb)
{
	D3D11_VIEWPORT const viewport = { 0, 0, 800, 800, 0, 1 };
	//context.viewport_width, context.viewport_height, 0.0f, 1.0f };
	float const z_clear[4] = { 1.0f, 0.0f, 0.0f, 1.0f };

	context->ClearRenderTargetView(this->hi_z_rtv.Get(), z_clear);
	context->ClearDepthStencilView(this->depth_dsv.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0);
	context->RSSetState(this->subchunk_rs.Get());
	context->RSSetViewports(1, &viewport);
	context->IASetInputLayout(this->subchunk_il.Get());
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	context->VSSetShader(this->subchunk_vs.Get(), nullptr, 0);
	context->VSSetConstantBuffers(0, 1, camera_info_cb.GetAddressOf());
	context->PSSetShader(this->subchunk_ps.Get(), nullptr, 0);
	context->OMSetRenderTargets(1, this->hi_z_rtv.GetAddressOf(), this->depth_dsv.Get());

	for (RenderGroup::Subchunk const& subchunk : render_group.getSubchunks())
	{
		SubchunkMesh const& mesh = subchunk.mesh.get();
		D3D11_MAPPED_SUBRESOURCE subresource = {};
		HRESULT hr = context->Map(this->subchunk_cb.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &subresource);
		CHECK(hr);
		ivec3& position = *reinterpret_cast<ivec3*>(subresource.pData);

		position = ivec3(subchunk.x, subchunk.y, subchunk.z);
		context->Unmap(this->subchunk_cb.Get(), 0);
		context->VSSetConstantBuffers(1, 1, this->subchunk_cb.GetAddressOf());
		mesh.draw(context);
	}
	context->ExecuteCommandList(this->hi_z_commands.Get(), FALSE);
	RenderGroup result;

    //this->render_group = std::move(result); // 가시성 필터링된 결과로 업데이트
}

void VisibilityProcessor::getResult(RenderGroup& out)
{
	out.swap(this->render_group);
}



void VisibilityProcessor::applyFrustumCulling(Frustum const& frustum)
{
    RenderGroup result;

    for (RenderGroup::Subchunk const& subchunk : this->render_group.getSubchunks())
    {
        vec3 const center = vec3(subchunk.x * 16 + 8, subchunk.y * 16 + 8, subchunk.z * 16 + 8);
        float const radius = 14.0f;

        if (!frustum.intersectsSphere(center, radius))
            continue;
        result.add(subchunk);
    }
    this->render_group = std::move(result);
}

