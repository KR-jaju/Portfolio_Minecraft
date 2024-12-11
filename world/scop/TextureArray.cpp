#define _CRT_SECURE_NO_WARNINGS // stb_image_write compile error fix

/*
1.솔루션 탐색기에서 프로젝트 이름을 마우스 오른쪽 버튼으로 클릭하고 
**속성(Properties)**을 선택합니다.
2. C/C++ -> Preprocessor -> Preprocessor Definitions 항목에 
_CRT_SECURE_NO_WARNINGS를 추가합니다.
*/

#include "pch.h"
#include "TextureArray.h"
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image.h"
#include "stb_image_write.h"

TextureArray::TextureArray(
	ComPtr<ID3D11Device> device,
	ComPtr<ID3D11DeviceContext> context,
	vector<wstring> const& path_arr
)
{
	D3D11_TEXTURE2D_DESC texture_desc;
	vector<ID3D11Texture2D*> textures(path_arr.size());
	for (int i = 0; i < path_arr.size(); i++) {
		ID3D11Resource* resource = nullptr;
		HRESULT hr = CreateWICTextureFromFile(
			device.Get(),
			context.Get(),
			path_arr[i].c_str(),
			&resource,
			nullptr
		); // img 파일 읽어서 texture resource 변환
		CHECK(hr);
		textures[i] = static_cast<ID3D11Texture2D*>(resource);

		if (i == 0) {
			textures[i]->GetDesc(&texture_desc);
		}
		else {
			D3D11_TEXTURE2D_DESC desc;
			textures[i]->GetDesc(&desc);
			// 모든 텍스처가 동일한 메타데이터를 가지는지 확인
			if (desc.Width != texture_desc.Width ||
				desc.Height != texture_desc.Height ||
				//desc.Format != texture_desc.Format ||
				desc.MipLevels != texture_desc.MipLevels) {
				throw std::runtime_error("All textures must have the same dimensions, format, and mip levels.");
			}
		}
	}

	texture_desc.ArraySize = static_cast<UINT>(path_arr.size());
	texture_desc.Usage = D3D11_USAGE_DEFAULT;
	texture_desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	texture_desc.CPUAccessFlags = 0;
	texture_desc.MiscFlags = 0;

	HRESULT hr = device->CreateTexture2D(
		&texture_desc,
		nullptr,
		this->texture_arr.GetAddressOf()
	);
	CHECK(hr);

	// texture array 에 textures 의 내용 복사
	for (int i = 0; i < textures.size(); i++) {
		context->CopySubresourceRegion(
			this->texture_arr.Get(),
			D3D11CalcSubresource(
				0,
				static_cast<UINT>(i),
				texture_desc.MipLevels
			),
			0,
			0,
			0,
			textures[i],
			0,
			nullptr
		);
	}
	D3D11_SHADER_RESOURCE_VIEW_DESC view_desc;
	ZeroMemory(&view_desc, sizeof(view_desc));
	view_desc.Format = texture_desc.Format;
	view_desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
	view_desc.Texture2DArray.MostDetailedMip = 0;
	view_desc.Texture2DArray.MipLevels = texture_desc.MipLevels;
	view_desc.Texture2DArray.FirstArraySlice = 0;
	view_desc.Texture2DArray.ArraySize = texture_desc.ArraySize;

	hr = device->CreateShaderResourceView(
		this->texture_arr.Get(),
		&view_desc,
		this->shader_resource_view.GetAddressOf()
	);
	CHECK(hr);

	// 리소스 해제
	for (ID3D11Texture2D* tex : textures) {
		if (tex) {
			tex->Release();
		}
	}
}

void ReadImage(const std::string filename, std::vector<uint8_t>& image,
	int& width, int& height) {

	int channels;

	unsigned char* img =
		stbi_load(filename.c_str(), &width, &height, &channels, 0);

	// assert(channels == 4);

	// 4채널로 만들어서 복사
	image.resize(width * height * 4);

	if (channels == 3) {
		// 채널3 (rgb) -> 채널4(rgba)로 바꿔줌
		for (size_t i = 0; i < width * height; i++) {
			for (size_t c = 0; c < 3; c++) {
				image[4 * i + c] = img[i * channels + c];
			}
			image[4 * i + 3] = 255; // alpha 값을 항상 255로 설정
		}
	}
	else if (channels == 4) {
		for (size_t i = 0; i < width * height; i++) {
			for (size_t c = 0; c < 4; c++) {
				image[4 * i + c] = img[i * channels + c];
			}
		}
	}
	else {
		std::cout << "Read 3 or 4 channels images only. " << channels
			<< " channels" << endl;
	}
	delete img;
}

// mipmap 적용
TextureArray::TextureArray(
	ComPtr<ID3D11Device> device, 
	ComPtr<ID3D11DeviceContext> context, 
	vector<wstring> const& filenames, 
	int mip_level,
	bool srgb_flag
)
{
	 int width = 0, height = 0;
        vector<vector<uint8_t>> imageArray;
        for (const auto& f : filenames) {

            std::vector<uint8_t> image;
			string file;
			file.assign(f.begin(), f.end());
            ReadImage(file, image, width, height);

            imageArray.push_back(image);
        }

        UINT size = UINT(filenames.size());

        // Texture2DArray를 만듭니다. 이때 데이터를 CPU로부터 복사하지 않습니다.
        D3D11_TEXTURE2D_DESC txtDesc;
        ZeroMemory(&txtDesc, sizeof(txtDesc));
        txtDesc.Width = UINT(width);
        txtDesc.Height = UINT(height);
        txtDesc.MipLevels = mip_level; // 밉맵 내가 만들수 있는 만큼 = 0
        txtDesc.ArraySize = size;
		if (srgb_flag == false)
			txtDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		else
			txtDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
        txtDesc.SampleDesc.Count = 1;
        txtDesc.SampleDesc.Quality = 0;
        txtDesc.Usage = D3D11_USAGE_DEFAULT; // 스테이징 텍스춰로부터 복사 가능
        txtDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | 
			D3D11_BIND_RENDER_TARGET;// 읽기도 가능하고 결과도 출력 가능
        txtDesc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS; // 밉맵 사용

        // 초기 데이터 없이 텍스춰를 만듭니다.
        device->CreateTexture2D(&txtDesc, nullptr, 
			this->texture_arr.GetAddressOf());

        // 실제로 만들어진 MipLevels를 확인
        this->texture_arr->GetDesc(&txtDesc);
        // cout << txtDesc.MipLevels << endl;

        // StagingTexture를 만들어서 하나씩 복사합니다.
        for (size_t i = 0; i < imageArray.size(); i++) {

            auto& image = imageArray[i];
            
			context->UpdateSubresource(
				this->texture_arr.Get(),
				D3D11CalcSubresource(0, UINT(i), txtDesc.MipLevels),
				nullptr,
				image.data(),
				width * 4,
				width * height * 4
			);
        }

        device->CreateShaderResourceView(
			this->texture_arr.Get(), 
			nullptr,
            this->shader_resource_view.GetAddressOf());

        context->GenerateMips(this->shader_resource_view.Get());
}

TextureArray::TextureArray(
	ComPtr<ID3D11Device> device,
	ComPtr<ID3D11DeviceContext> context,
	vector<ComPtr<ID3D11ShaderResourceView>> const& srvs,
	DXGI_FORMAT dxgi_format
)
{
	vector<ComPtr<ID3D11Resource>> resources;
	ComPtr<ID3D11Resource> resource;
	for (int i = 0; i < srvs.size(); i++) {
		srvs[i]->GetResource(resource.GetAddressOf());
		resources.push_back(resource);
	}
	vector<ComPtr<ID3D11Texture2D>> textures;
	ComPtr<ID3D11Texture2D> texture;
	D3D11_TEXTURE2D_DESC tex_desc;
	for (int i = 0; i < srvs.size(); i++) {
		HRESULT h = resources[i]->QueryInterface(__uuidof(ID3D11Texture2D),
			(void**)texture.GetAddressOf());
		CHECK(h);
		D3D11_TEXTURE2D_DESC current_desc;
		texture->GetDesc(&current_desc);

		if (i == 0)
			tex_desc = current_desc;
		else {
			if (current_desc.Width != tex_desc.Width ||
				current_desc.Height != tex_desc.Height ||
				current_desc.Format != tex_desc.Format ||
				current_desc.MipLevels != tex_desc.MipLevels) {
				throw std::runtime_error("모든 텍스처의 크기 및 포맷이 동일하지 않습니다.");
			}
		}
		textures.push_back(texture);
	}
	tex_desc.ArraySize = static_cast<UINT>(srvs.size());
	tex_desc.Usage = D3D11_USAGE_DEFAULT;
	tex_desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	tex_desc.CPUAccessFlags = 0;
	tex_desc.MiscFlags = 0;

	HRESULT hr = device->CreateTexture2D(
		&tex_desc,
		nullptr,
		this->texture_arr.GetAddressOf()
	);
	CHECK(hr);

	for (int i = 0; i < textures.size(); i++) {
		context->CopySubresourceRegion(
			this->texture_arr.Get(),
			D3D11CalcSubresource(
				0,
				static_cast<UINT>(i),
				tex_desc.MipLevels
			),
			0,
			0,
			0,
			textures[i].Get(),
			0,
			nullptr
		);
	}
	D3D11_SHADER_RESOURCE_VIEW_DESC view_desc;
	ZeroMemory(&view_desc, sizeof(view_desc));
	view_desc.Format = dxgi_format;
	view_desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
	view_desc.Texture2DArray.MostDetailedMip = 0;
	view_desc.Texture2DArray.MipLevels = tex_desc.MipLevels;
	view_desc.Texture2DArray.FirstArraySlice = 0;
	view_desc.Texture2DArray.ArraySize = tex_desc.ArraySize;

	hr = device->CreateShaderResourceView(
		this->texture_arr.Get(),
		&view_desc,
		this->shader_resource_view.GetAddressOf()
	);
	CHECK(hr); // TODO 오류 해결
}

void TextureArray::updateTextureArray(
	ComPtr<ID3D11DeviceContext> context, 
	ComPtr<ID3D11ShaderResourceView> target_srv, 
	UINT arr_idx
)
{
	ComPtr<ID3D11Resource> resource;
	target_srv->GetResource(resource.GetAddressOf());

	ComPtr<ID3D11Texture2D> texture;
	HRESULT hr = resource->QueryInterface(__uuidof(ID3D11Texture2D),
		(void**)texture.GetAddressOf());
	CHECK(hr);

	UINT subresource_idx = D3D11CalcSubresource(0, arr_idx, 1);

	context->CopySubresourceRegion(
		this->texture_arr.Get(),
		subresource_idx,
		0, 0, 0, texture.Get(), 0, nullptr
	);
}

ComPtr<ID3D11ShaderResourceView> TextureArray::getComPtr()
{
	return this->shader_resource_view;
}
