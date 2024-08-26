#pragma once

namespace d3d11
{
	class Device;
	class ConstantBuffer
	{
	public:
		ConstantBuffer(Device const& device, uint32 size);
		ConstantBuffer(ConstantBuffer const&) = delete;
		~ConstantBuffer() = default;
		ConstantBuffer& operator=(ConstantBuffer const&) = delete;

		uint32 getByteSize() const;

		//d3d11 private
		ID3D11Buffer* getInternalResource() const;
		//void	update(void* data, uint32 byte_size, uint32 offset = 0);
	private:
		ComPtr<ID3D11Buffer> buffer;
		uint32 size;
	};
}
