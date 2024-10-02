#include "pch.h"
#include "Pipeline.h"

Pipeline::Pipeline(ComPtr<ID3D11DeviceContext> device_context)
	:device_context(device_context)
{
}

Pipeline::~Pipeline()
{
}

void Pipeline::UpdatePipeline(PipelineInfo info)
{
	//IA
	this->device_context->IASetInputLayout(info.input_layout->getComPtr().Get());
	this->device_context->IASetPrimitiveTopology(info.topology);

	//VS
	if (info.vertex_shader)
		this->device_context->VSSetShader(info.vertex_shader->getComPtr().Get(), nullptr, 0);

	//RS
	if (info.rasterizer_state)
		this->device_context->RSSetState(info.rasterizer_state->getComPtr().Get());

	//PS
	if (info.pixel_shader)
		this->device_context->PSSetShader(info.pixel_shader->getComPtr().Get(), nullptr, 0);

	//OM
	if (info.blend_state)
		this->device_context->OMSetBlendState(info.blend_state->getComPtr().Get(), info.blend_state->getBlendFactor(), info.blend_state->getSampleMask());

}


void Pipeline::SetVertexBuffer(shared_ptr<Buffer<VertexUV>> buffer)
{
	uint32 stride = buffer->getStride();
	uint32 offset = buffer->getOffset();
	this->device_context->IASetVertexBuffers(0, 1, buffer->getComPtr().GetAddressOf(), &stride, &offset);
}

void Pipeline::SetColorVertexBuffer(shared_ptr<Buffer<Vertex>> buffer)
{
	uint32 stride = buffer->getStride();
	uint32 offset = buffer->getOffset();
	this->device_context->IASetVertexBuffers(0, 1, buffer->getComPtr().GetAddressOf(), &stride, &offset);
}

void Pipeline::SetIndexBuffer(shared_ptr<Buffer<uint32>> buffer)
{
	this->device_context->IASetIndexBuffer(buffer->getComPtr().Get(), DXGI_FORMAT_R32_UINT, 0);
}

void Pipeline::SetVSConstantBuffer(uint32 slot, shared_ptr<ConstantBuffer> buffer)
{
	this->device_context->VSSetConstantBuffers(slot, 1, buffer->getComPtr().GetAddressOf());
}

void Pipeline::SetPSConstantBuffer(uint32 slot, shared_ptr<ConstantBuffer> buffer)
{
	this->device_context->PSSetConstantBuffers(slot, 1, buffer->getComPtr().GetAddressOf());
}

void Pipeline::SetVSTexture(uint32 slot, shared_ptr<TextureResource> texture)
{
	this->device_context->VSSetShaderResources(slot, 1, texture->GetComPtr().GetAddressOf());
}

void Pipeline::SetPSTexture(uint32 slot, shared_ptr<TextureResource> texture)
{
	this->device_context->PSSetShaderResources(slot, 1, texture->GetComPtr().GetAddressOf());
}

void Pipeline::SetVSSamplerState(uint32 slot, shared_ptr<SamplerState> sampler_state)
{
	this->device_context->VSSetSamplers(0, 1, sampler_state->getComPtr().GetAddressOf());
}

void Pipeline::SetPSSamplerState(uint32 slot, shared_ptr<SamplerState> sampler_state)
{
	this->device_context->PSSetSamplers(0, 1, sampler_state->getComPtr().GetAddressOf());
}

void Pipeline::Draw(uint32 vertex_count, uint32 start_vertex_location)
{
	this->device_context->Draw(vertex_count, start_vertex_location);
}

void Pipeline::DrawIndexed(uint32 index_count, uint32 start_vertex_location, uint32 base_vertex_location)
{
	this->device_context->DrawIndexed(index_count, start_vertex_location, base_vertex_location);
}
