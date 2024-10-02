#pragma once

#include "Buffer.h"
#include "Camera.h"
#include "InputLayout.h"
#include "Graphics.h"
#include "ConstantBuffer.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "Texture.h"
#include "RasterizerState.h"
#include "BlendState.h"
#include "SamplerState.h"
#include "CameraMove.h"
#include "RenderSystem.h"
#include "SceneSystem.h"
#include "ResourceSystem.h"
class InputSystem;
class RenderSystem;
class ResourceSystem;
class SceneSystem;
class TimeSystem;

class Test
{
public:
	Test(HWND hwnd, UINT width, UINT height);
	~Test();
	void Update();
	void Render();
	
	shared_ptr<InputSystem> GetInputSystem() { return this->input_system; }
	shared_ptr<RenderSystem> GetRenderSystem() { return this->render_system; }
	shared_ptr<ResourceSystem> GetResourceSystem() { return this->resource_system; }
	shared_ptr<SceneSystem> GetSceneSystem() { return this->scene_system; }
	shared_ptr<TimeSystem> GetTimeSystem() { return this->time_system; }

private:
	HWND hwnd;

private:
	// 밖에서 정의됨
	shared_ptr<Graphics> graphic;
	shared_ptr<TimeSystem> time_system;
	shared_ptr<InputSystem> input_system;
	shared_ptr<SceneSystem> scene_system;
	shared_ptr<ResourceSystem> resource_system;
	shared_ptr<RenderSystem> render_system;
};

