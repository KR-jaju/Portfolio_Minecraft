#pragma once

class Graphics;
class Pipeline;
class VertexArrayObject;
class ConstantBuffer;

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
	shared_ptr<Graphics> graphic;
	shared_ptr<Pipeline> pipe;
	shared_ptr<VertexArrayObject> vao;
	shared_ptr<ConstantBuffer> constant_buffer;
	vector<Vertex> vertices;
	vector<uint32> indices;
	vector<VertexUV> vertices_uv;
};

