#pragma once

#include "VertexArrayObject.h"
#include "Graphics.h"
#include "PipelinePartManager.h"
#include "PipeManager.h"
#include "Pipeline.h"
#include "ConstantBuffer.h"

class Test
{
public:
	Test(HWND hwnd, UINT width, UINT height);
	~Test();
	void setDrawBox();
	void setDrawTexSkel();
	void render();
	void renderUV();
	void update();

private:
	void geometrySkel();
	void geometryBox();

private:
	// 밖에서 정의됨
	shared_ptr<PipelinePartManager> pipe_part;
	shared_ptr<Graphics> graphic;
	shared_ptr<PipeManager> pipe_manager;
	
	// 실제 object들이 render 하기 위해 갖고 있을 부분
	shared_ptr<Pipeline> pipe;
	shared_ptr<VertexArrayObject> vao;
	shared_ptr<ConstantBuffer> constant_buffer;

	// test를 위해 필요 -> buffer를 만들면 실질적으로 필요 x
	vector<Vertex> vertices;
	vector<uint32> indices;
	vector<VertexUV> vertices_uv;
};

