#pragma once
class IndexBuffer
{
public:
	IndexBuffer(ComPtr<ID3D11Device> device);
	~IndexBuffer();
	ComPtr<ID3D11Buffer> getComPtr() const;
	void create(const vector<uint32>& indices);
	uint32 getStride() const;
	uint32 getOffset() const;
	uint32 getCount() const;
private:
	ComPtr<ID3D11Device> device;
	ComPtr<ID3D11Buffer> index_buffer;
	uint32 stride = 0;
	uint32 offset = 0;
	uint32 count = 0;
};

