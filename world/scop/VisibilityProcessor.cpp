#include "pch.h"
#include "VisibilityProcessor.h"

#include "SubchunkMesh.h"
#include "InputLayouts.h"

void VisibilityProcessor::initialize(ComPtr<ID3D11Device> const& device, AssetManager& asset_manager, int width, int height)
{
	this->intializeTextures(device, width, height);
	this->initializeSubchunkData(device);
	this->initializeHiZData(device, width, height);
	this->initializeOcclusionCullingData(device);
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

	this->hi_z_srvs.resize(max_mip_level);
	this->hi_z_uavs.resize(max_mip_level);
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
		ComPtr<ID3D11ShaderResourceView>& srv = this->hi_z_srv;
		D3D11_TEXTURE2D_DESC desc = {};
		D3D11_SHADER_RESOURCE_VIEW_DESC srv_desc = {};

		desc.Width = width;
		desc.Height = height;
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.Format = DXGI_FORMAT_R32_FLOAT;
		desc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS;
		desc.ArraySize = 1;
		desc.MipLevels = 0;
		desc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;
		desc.SampleDesc.Count = 1;
		desc.SampleDesc.Quality = 0;
		desc.CPUAccessFlags = 0;

		srv_desc.Format = DXGI_FORMAT_R32_FLOAT; // SRV용 포맷
		srv_desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srv_desc.Texture2D.MostDetailedMip = 0;
		srv_desc.Texture2D.MipLevels = -1;

		HRESULT hr = device->CreateTexture2D(&desc, nullptr, texture.GetAddressOf());
		CHECK(hr);
		hr = device->CreateRenderTargetView(texture.Get(), nullptr, rtv.GetAddressOf());
		CHECK(hr);
		hr = device->CreateShaderResourceView(texture.Get(), &srv_desc, srv.GetAddressOf());
		CHECK(hr);
	}
	for (int level = 0; level < max_mip_level; ++level)
	{
		ComPtr<ID3D11Texture2D>& texture = this->hi_z_texture;
		ComPtr<ID3D11ShaderResourceView>& srv = this->hi_z_srvs[level];
		ComPtr<ID3D11UnorderedAccessView>& uav = this->hi_z_uavs[level];
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

			context->CSSetShaderResources(0, 1, this->hi_z_srvs[level - 1].GetAddressOf());
			context->CSSetUnorderedAccessViews(0, 1, this->hi_z_uavs[level].GetAddressOf(), nullptr);
			
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
	D3D11_VIEWPORT const viewport = { 0, 0, this->width, this->height, 0, 1 };
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

	for (RenderGroup::Subchunk const& subchunk : occluders.getSubchunks())
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
	context->ExecuteCommandList(this->hi_z_commands.Get(), FALSE); // Hi-z buffer 생성
	RenderGroup result;
	std::vector<vec4> occludees;
	std::vector<uint32> visibility;

	for (RenderGroup::Subchunk const& subchunk : this->render_group.getSubchunks())
	{
		vec3 const center = vec3(subchunk.x * 16 + 8, subchunk.y * 16 + 8, subchunk.z * 16 + 8);
		float const radius = 14.0f;

		occludees.emplace_back(center.x, center.y, center.z, radius);
	}
	D3D11_MAPPED_SUBRESOURCE resource;
	HRESULT hr;

	context->CSSetShader(this->occlusion_cs.Get(), nullptr, 0);
	context->CSSetConstantBuffers(0, 1, camera_info_cb.GetAddressOf());
	context->CSSetSamplers(0, 1, this->hi_z_ss.GetAddressOf());
	context->CSSetShaderResources(0, 1, this->hi_z_srv.GetAddressOf());
	for (int offset = 0; offset < occludees.size(); offset += 1024)
	{
		int const count = std::min<int>(occludees.size() - offset, 1024);
		D3D11_MAPPED_SUBRESOURCE resource;
		// 오클루디 리스트 채우기
		HRESULT hr = context->Map(this->occludee_cb.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);
		CHECK(hr);
		int& object_count = *reinterpret_cast<int*>(resource.pData);
		vec4* object_array = reinterpret_cast<vec4*>(reinterpret_cast<char*>(resource.pData) + 16);

		object_count = count;
		memcpy(object_array, occludees.data() + offset, sizeof(vec4) * count);
		context->Unmap(this->occludee_cb.Get(), 0);
		context->CSSetConstantBuffers(2, 1, this->occludee_cb.GetAddressOf());
		context->CSSetUnorderedAccessViews(0, 1, this->visibility_uav.GetAddressOf(), nullptr);
		context->Dispatch((count + 63) / 64, 1, 1);
		context->CopyResource(this->visibility_staging.Get(), this->visibility_sb.Get());
		hr = context->Map(this->visibility_staging.Get(), 0, D3D11_MAP_READ, 0, &resource);
		CHECK(hr);
		uint32 const* begin = reinterpret_cast<uint32*>(resource.pData);
		uint32 const* end = begin + count;
		visibility.insert(visibility.end(), begin, end);
		context->Unmap(this->visibility_staging.Get(), 0);
	} // 오클루전 테스트
	int idx = 0;
	for (RenderGroup::Subchunk const& subchunk : this->render_group.getSubchunks())
	{
		if (visibility[idx])
		{
			result.add(subchunk);
		}
		idx += 1;
	}
    this->render_group = std::move(result); // 가시성 필터링된 결과로 업데이트
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


void VisibilityProcessor::initializeOcclusionCullingData(ComPtr<ID3D11Device> const& device)
{
	{ // Occludee Constant Buffer (Input)
		D3D11_BUFFER_DESC desc = {};
		D3D11_SHADER_RESOURCE_VIEW_DESC srv_desc = {};

		desc.Usage = D3D11_USAGE_DYNAMIC;
		desc.ByteWidth = sizeof(vec4) * 1024 + 16; // 최대 1024개 + 물체 개수
		desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

		HRESULT hr = device->CreateBuffer(&desc, nullptr, this->occludee_cb.GetAddressOf());
		CHECK(hr);
	}
	{ // Hi-Z buffer sampler
		D3D11_SAMPLER_DESC sampDesc = {};
		sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
		sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
		sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
		sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
		sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
		sampDesc.MinLOD = 0;
		sampDesc.MaxLOD = D3D11_FLOAT32_MAX;

		HRESULT hr = device->CreateSamplerState(&sampDesc, this->hi_z_ss.GetAddressOf());
		CHECK(hr);
	}
	{ // Visibility Structured Buffer (Output)
		D3D11_BUFFER_DESC desc = {};
		D3D11_UNORDERED_ACCESS_VIEW_DESC uav_desc = {}; // UAV 생성에 필요한 구조체

		desc.Usage = D3D11_USAGE_DEFAULT; // Compute Shader 결과를 GPU에 유지
		desc.ByteWidth = sizeof(uint32) * 1024; // 최대 1024개의 32비트 정수
		desc.BindFlags = D3D11_BIND_UNORDERED_ACCESS; // UAV 사용을 위한 플래그
		desc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED; // Structured Buffer로 설정
		desc.StructureByteStride = sizeof(uint32); // 각 요소의 크기 설정

		uav_desc.Format = DXGI_FORMAT_UNKNOWN; // Structured Buffer는 항상 UNKNOWN 포맷
		uav_desc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
		uav_desc.Buffer.FirstElement = 0;
		uav_desc.Buffer.NumElements = 1024; // 요소 개수 설정
		uav_desc.Buffer.Flags = 0; // 추가 플래그는 없음

		HRESULT hr = device->CreateBuffer(&desc, nullptr, this->visibility_sb.GetAddressOf());
		CHECK(hr);
		hr = device->CreateUnorderedAccessView(this->visibility_sb.Get(), &uav_desc, this->visibility_uav.GetAddressOf());
		CHECK(hr);
	}
	{
		D3D11_BUFFER_DESC desc = {};
		// Staging 버퍼이므로 GPU에 바인딩되지 않으며, CPU에서 읽기만 합니다.
		desc.Usage = D3D11_USAGE_STAGING;
		desc.ByteWidth = sizeof(uint32) * 1024; // 최대 1024개의 32비트 정수
		desc.BindFlags = 0; // 스테이징 버퍼는 바인딩 플래그가 없음
		desc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED; // Structured Buffer로 설정
		desc.StructureByteStride = sizeof(uint32);
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;

		HRESULT hr = device->CreateBuffer(&desc, nullptr, this->visibility_staging.GetAddressOf());
		CHECK(hr);
	}
	{ // Occlusion Setting Constant Buffer
		uint32 const compile_flag = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
		ComPtr<ID3DBlob> blob;

		HRESULT hr = D3DCompileFromFile(L"HiZOcclusionCullingCS.hlsl", nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, "main", "cs_5_0", compile_flag, 0, blob.GetAddressOf(), nullptr);
		CHECK(hr);
		hr = device->CreateComputeShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, this->occlusion_cs.GetAddressOf());
		CHECK(hr);
	}
}
