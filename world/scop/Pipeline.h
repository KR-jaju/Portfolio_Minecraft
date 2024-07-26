#pragma once

class VertexShader;
class PixelShader;
class SamplerState;
class Texture;
class RasterizerState;
class BlendState;
class ConstantBuffer;

enum class ShaderType {
	VERTEX = (1 << 0),
	PIXEL = (1 << 1),
	BOTH = VERTEX | PIXEL
};

class Pipeline
{
public:
	Pipeline(ComPtr<ID3D11DeviceContext> context);
	~Pipeline();
	void updatePipeline();
	void setVertexShader(shared_ptr<VertexShader> vertex_shader);
	void setPixelShader(shared_ptr<PixelShader> pixel_shader);
	void setRasterizerState(shared_ptr<RasterizerState> rasterizer_state);
	void setBlendState(shared_ptr<BlendState> blend_state);
	void setTexture(shared_ptr<Texture> texture);
	void setSamplerState(shared_ptr<SamplerState> sampler_state);
	void setPipeConstantBuffer(
		uint32 slot,
		uint32 scope,
		shared_ptr<ConstantBuffer> ptr
	);
	void setPipeTexture(uint32 slot, uint32 scope, uint32 cnt);
	void setPipeSamplerState(uint32 slot, uint32 scope, uint32 cnt);

	void drawIndexed(
		uint32 index_count,
		uint32 start_index_location,
		uint32 base_vertex_location
	);
	void draw(uint32 vertex_count, uint32 start_vertex_location);
private:
	ComPtr<ID3D11DeviceContext> context;
	shared_ptr<VertexShader> vertex_shader;
	shared_ptr<PixelShader> pixel_shader;
	shared_ptr<SamplerState> sampler_state;
	shared_ptr<Texture> texture;
	shared_ptr<RasterizerState> rasterizer_state;
	shared_ptr<BlendState> blend_state;
};

