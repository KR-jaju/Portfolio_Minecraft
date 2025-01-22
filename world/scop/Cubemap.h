#pragma once
class Cubemap
{
public:
	Cubemap(
		ComPtr<ID3D11Device> device,
		wstring const& path
	);
	Cubemap(Cubemap const&) = delete;
	~Cubemap();
	Cubemap& operator=(Cubemap const&) = delete;
	ComPtr<ID3D11ShaderResourceView> getComPtr() const;
private:
	ComPtr<ID3D11Device> device;
	ComPtr<ID3D11ShaderResourceView> sharder_resource_view;
};

