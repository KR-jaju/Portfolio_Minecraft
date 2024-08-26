#pragma once

namespace d3d11
{
	class Device;
	class RenderTarget;
	class Primitive;
	class Program;
	class Context {
	public:
		Context(Device const& device);
		Context(Context const&) = delete;

		void	setViewport(uint32 x, uint32 y, uint32 w, uint32 h); 

		void	setRenderTarget(RenderTarget const* target);
		
		void	clearColor(uint32 idx, float r, float g, float b, float a);
		void	clearDepthStencil(float depth, uint32 stencil);

		void	setProgram(Program const* program); // vs, fs, constant buffers
		
		void	setPrimitive(Primitive const* primitive); // buffer, input layout

		//vao, program, fbo, draw
		//void draw();
		//d3d11 private
		ID3D11DeviceContext* getInternalResource() const;
	private:
		ComPtr<ID3D11DeviceContext> device_context;
		RenderTarget const* render_target;
		Primitive const* primitive;
		Program const* program;
	};
}