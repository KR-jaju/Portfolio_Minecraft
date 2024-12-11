#include "pch.h"
#include "StructuredBuffer.h"

StructuredBuffer::StructuredBuffer(
	ID3D11Device* device,
	ID3D11DeviceContext* context,
	void* inputData, 
	uint32 inputStride, 
	uint32 inputCount, 
	uint32 outputStride, 
	uint32 outputCount
)
	: device(device), context(context),
	_inputData(inputData), _inputStride(inputStride), 
	_inputCount(inputCount), _outputStride(outputStride), 
	_outputCount(outputCount)
{
	if (outputStride == 0 || outputCount == 0)
	{
		_outputStride = inputStride;
		_outputCount = inputCount;
	}

	CreateBuffer();
}

StructuredBuffer::~StructuredBuffer()
{

}

void StructuredBuffer::CreateBuffer()
{
	CreateInput();
	CreateSRV();
	CreateOutput();
	CreateUAV();
	CreateResult();
}

void StructuredBuffer::CreateInput()
{
	D3D11_BUFFER_DESC desc;
	ZeroMemory(&desc, sizeof(desc));

	desc.ByteWidth = GetInputByteWidth();
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	desc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
	desc.StructureByteStride = _inputStride;
	desc.Usage = D3D11_USAGE_DYNAMIC;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	D3D11_SUBRESOURCE_DATA subResource = { 0 };
	subResource.pSysMem = _inputData;

	if (_inputData != nullptr)
		CHECK(device->CreateBuffer(&desc, &subResource, _input.GetAddressOf()));
	else
		CHECK(device->CreateBuffer(&desc, nullptr, _input.GetAddressOf()));
}

void StructuredBuffer::CreateSRV()
{
	D3D11_BUFFER_DESC desc;
	_input->GetDesc(&desc);

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	ZeroMemory(&srvDesc, sizeof(srvDesc));
	srvDesc.Format = DXGI_FORMAT_UNKNOWN;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFEREX;
	srvDesc.BufferEx.NumElements = _inputCount;

	CHECK(device->CreateShaderResourceView(_input.Get(), &srvDesc, _srv.GetAddressOf()));
}

void StructuredBuffer::CreateOutput()
{
	D3D11_BUFFER_DESC desc;
	ZeroMemory(&desc, sizeof(desc));

	desc.ByteWidth = GetOutputByteWidth();
	desc.BindFlags = D3D11_BIND_UNORDERED_ACCESS;
	desc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
	desc.StructureByteStride = _outputStride;

	CHECK(device->CreateBuffer(&desc, nullptr, _output.GetAddressOf()));
}

void StructuredBuffer::CreateUAV()
{
	D3D11_BUFFER_DESC desc;
	_output->GetDesc(&desc);

	D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc;
	ZeroMemory(&uavDesc, sizeof(uavDesc));
	uavDesc.Format = DXGI_FORMAT_UNKNOWN;
	uavDesc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
	uavDesc.Buffer.NumElements = _outputCount;

	CHECK(device->CreateUnorderedAccessView(_output.Get(), &uavDesc, _uav.GetAddressOf()));
}


void StructuredBuffer::CreateResult()
{
	D3D11_BUFFER_DESC desc;
	_output->GetDesc(&desc);

	desc.Usage = D3D11_USAGE_STAGING;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
	desc.BindFlags = 0;
	desc.MiscFlags = 0;

	CHECK(device->CreateBuffer(&desc, NULL, _result.GetAddressOf()));
}


void StructuredBuffer::CopyToInput(void* data)
{
	D3D11_MAPPED_SUBRESOURCE subResource;
	context->Map(_input.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &subResource);
	{
		memcpy(subResource.pData, data, GetInputByteWidth());
	}
	context->Unmap(_input.Get(), 0);
}

void StructuredBuffer::CopyFromOutput(void* data)
{
	context->CopyResource(_result.Get(), _output.Get());

	D3D11_MAPPED_SUBRESOURCE subResource;
	context->Map(_result.Get(), 0, D3D11_MAP_READ, 0, &subResource);
	{
		memcpy(data, subResource.pData, GetOutputByteWidth());
	}
	context->Unmap(_result.Get(), 0);
}