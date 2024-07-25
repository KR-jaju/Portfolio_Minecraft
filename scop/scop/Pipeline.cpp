#include "pch.h"
#include "Pipeline.h"
#include "PixelShader.h"
#include "VertexShader.h"
#include "SamplerState.h"
#include "Texture.h"
#include "VertexArrayObject.h"
#include "RasterizerState.h"
#include "BlendState.h"
#include "ConstantBuffer.h"

Pipeline::Pipeline(ComPtr<ID3D11DeviceContext> context)
	: context(context)
{
}

Pipeline::~Pipeline()
{
}

void Pipeline::updatePipeline()
{

	// VS
	this->context->VSSetShader(
		this->vertex_shader->getComPtr().Get(),
		nullptr, 
		0
	);

	// RS
	this->context->RSSetState(
		this->rasterizer_state->getComPtr().Get()
	);

	// PS
	this->context->PSSetShader(
		this->pixel_shader->getComPtr().Get(), 
		nullptr, 
		0
	);

	// OM
	if (this->blend_state) {
		this->context->OMSetBlendState(
			this->blend_state->getComPtr().Get(),
			this->blend_state->getBlendFactor(),
			this->blend_state->getSampleMask()
		);
	}
}

void Pipeline::setVertexShader(shared_ptr<VertexShader> vertex_shader)
{
	this->vertex_shader = vertex_shader;
}

void Pipeline::setPixelShader(shared_ptr<PixelShader> pixel_shader)
{
	this->pixel_shader = pixel_shader;
}

void Pipeline::setRasterizerState(
	shared_ptr<RasterizerState> rasterizer_state
)
{
	this->rasterizer_state = rasterizer_state;
}

void Pipeline::setBlendState(shared_ptr<BlendState> blend_state)
{
	this->blend_state = blend_state;
}

void Pipeline::setTexture(shared_ptr<Texture> texture)
{
	this->texture = texture;
}

void Pipeline::setSamplerState(shared_ptr<SamplerState> sampler_state)
{
	this->sampler_state = sampler_state;
}

void Pipeline::setPipeConstantBuffer(
	uint32 slot, 
	uint32 scope, 
	shared_ptr<ConstantBuffer> ptr)
{
	if (scope & static_cast<uint32>(ShaderType::VERTEX))
		this->context->VSSetConstantBuffers(
			slot,
			1,
			ptr->getComPtr().GetAddressOf()
		);
	if (scope & static_cast<uint32>(ShaderType::PIXEL))
		this->context->PSSetConstantBuffers(
			slot,
			1,
			ptr->getComPtr().GetAddressOf()
		);
}

void Pipeline::setPipeTexture(
	uint32 slot, 
	uint32 scope,
	uint32 cnt
)
{
	if (scope & static_cast<uint32>(ShaderType::VERTEX)) {
		this->context->VSSetShaderResources(
			slot,
			cnt,
			this->texture->getComPtr().GetAddressOf()
		);
	}
	if (scope & static_cast<uint32>(ShaderType::PIXEL)) {
		this->context->PSSetShaderResources(
			slot,
			cnt,
			this->texture->getComPtr().GetAddressOf()
		);
	}
}

void Pipeline::setPipeSamplerState(
	uint32 slot, 
	uint32 scope, 
	uint32 cnt
)
{
	if (scope & static_cast<uint32>(ShaderType::VERTEX)) {
		this->context->VSSetSamplers(
			slot,
			cnt,
			this->sampler_state->getComPtr().GetAddressOf()
		);
	}
	if (scope & static_cast<uint32>(ShaderType::PIXEL)) {
		this->context->PSSetSamplers(
			slot,
			cnt,
			this->sampler_state->getComPtr().GetAddressOf()
		);
	}
}

void Pipeline::drawIndexed(
	uint32 index_count, 
	uint32 start_index_location, 
	uint32 base_vertex_location)
{
	this->context->DrawIndexed(
		index_count,
		start_index_location,
		base_vertex_location
	);
}

void Pipeline::draw(
	uint32 vertex_count, 
	uint32 start_vertex_location
)
{
	this->context->Draw(vertex_count, start_vertex_location);
}
