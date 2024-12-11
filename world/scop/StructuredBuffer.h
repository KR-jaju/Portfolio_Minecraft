#pragma once


class StructuredBuffer
{
public:
	StructuredBuffer(
		ID3D11Device* device,
		ID3D11DeviceContext* context,
		void* inputData, 
		uint32 inputStride, 
		uint32 inputCount, 
		uint32 outputStride = 0, 
		uint32 outputCount = 0
	);
	~StructuredBuffer();

public:
	void CreateBuffer();

private:
	void CreateInput();
	void CreateSRV();
	void CreateOutput();
	void CreateUAV();
	void CreateResult();

public:
	uint32 GetInputByteWidth() { return _inputStride * _inputCount; }
	uint32 GetOutputByteWidth() { return _outputStride * _outputCount; }

	void CopyToInput(void* data);
	void CopyFromOutput(void* data);

public:
	ComPtr<ID3D11ShaderResourceView> GetSRV() { return _srv; }
	ComPtr<ID3D11UnorderedAccessView> GetUAV() { return _uav; }

private:
	ID3D11Device* device;
	ID3D11DeviceContext* context;
	ComPtr<ID3D11Buffer> _input;
	ComPtr<ID3D11ShaderResourceView> _srv; // Input
	ComPtr<ID3D11Buffer> _output;
	ComPtr<ID3D11UnorderedAccessView> _uav; // Output
	ComPtr<ID3D11Buffer> _result;

private:
	void* _inputData;
	uint32 _inputStride = 0;
	uint32 _inputCount = 0;
	uint32 _outputStride = 0;
	uint32 _outputCount = 0;
};