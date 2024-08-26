#pragma once

#include <memory>
#include "WindowCallback.h"
#include "d3d11/Device.h"
#include "d3d11/Context.h"
#include "d3d11/SwapChain.h"

class Test2 : public WindowCallback
{
	using Device = d3d11::Device;
	using SwapChain = d3d11::SwapChain;
	using Context = d3d11::Context;
public:
	Test2(std::shared_ptr<Device> const &device, HWND hwnd, UINT width, UINT height);
	~Test2();
	void setDrawTexSkel();
	void renderUV();
	void update();
	LRESULT	onEvent(HWND handle, UINT msg, WPARAM w_param, LPARAM l_param);
private:
	void geometrySkel();

private:
	std::shared_ptr<Device> device;
	SwapChain swap_chain;
	Context	context;

	vector<uint32> indices; // quad의 indices
	vector<VertexUV> vertices_uv; // quad의 모양 정의
};

