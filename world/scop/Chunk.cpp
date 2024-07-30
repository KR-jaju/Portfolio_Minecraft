#include "pch.h"
#include "Chunk.h"

Chunk::Chunk()
{
}

Chunk::~Chunk()
{
}

void Chunk::setVerticesAndIndices()
{
	VertexUV vertex;
	uint32 index = 0;
	for (int z = 0; z < 16; z++) {
		for (int y = 0; y < 256; y++) {
			for (int x = 0; x < 16; x++) {
				if (this->chunk[x][y][z] == 0)
					continue;
				vector<VertexUV> block_vertices =
					this->getBlockVertexUV(x, y, z);
				vector<uint32> block_indices =
					this->getBlockIndices(x, y, z, index);
				this->vertices.insert(
					this->vertices.end(),
					block_vertices.begin(),
					block_vertices.end()
				);
				this->indices.insert(
					this->indices.end(),
					block_indices.begin(),
					block_indices.end()
				);
			}
		}
	}
}

void Chunk::setBlockInChunk(int x, int y, int z, int type)
{
	this->chunk[x][y][z] = type;
}

int Chunk::getBlock(int x, int y, int z) const
{
	if (x < 0)
		return this->left.lock()->getBlock(15, y, z);
	if (x > 15)
		return this->right.lock()->getBlock(0, y, z);
	if (z < 0)
		return this->front.lock()->getBlock(x, y, 15);
	if (z > 15)
		return this->back.lock()->getBlock(x, y, 0);
	return this->chunk[x][y][z];
}

void Chunk::putChunk(float x, float y, float z)
{
	this->start_pos = vec3(x, y, z);
}

void Chunk::setStartPos(float x, float y, float z)
{
	this->start_pos = vec3(x, y, z);
}

vector<VertexUV> Chunk::getBlockVertexUV(
	int x, 
	int y, 
	int z
) const
{
	vector<VertexUV> cube_vertices;
	vector<bool> put_able_arr = this->checkBlock(x, y, z);
	vec2 start = vec2(0.f, 0.f);
	vec2 end = vec2(1.f, 1.f);

	for (int i = 0; i < put_able_arr.size(); i++) {
		Face flag = static_cast<Face>(i);
		VertexUV tmp_vertex;
		vector<vec3> positions;
		vector<vec2> texcoords;
		if (put_able_arr[i]) {
			positions = this->getBlockFaceVertexPos(x, y, z, flag);
			texcoords = this->getBlockFaceTexcoords(
				start,
				end,
				flag
			);
			for (int j = 0; j < 4; j++) {
				tmp_vertex.pos = positions[j];
				tmp_vertex.uv = texcoords[j];
				cube_vertices.push_back(tmp_vertex);
			}
		}
	}
	return cube_vertices;
}

vector<uint32> Chunk::getBlockIndices(
	int x,
	int y,
	int z,
	uint32& start
) const
{
	vector<uint32> block_indices;
	vector<bool> able_set_arr = this->checkBlock(x, y, z);
	for (int i = 0; i < 6; i++) {
		vector<uint32> face_indices;
		if (able_set_arr[i]) {
			face_indices = this->getBlockFaceIndices(start);
			start += 4;
			block_indices.insert(
				block_indices.end(),
				face_indices.begin(),
				face_indices.end()
			);
		}
	}
	return block_indices;
}

bool Chunk::checkBoundary(int x, int y, int z) const
{
	if (x < 0 || x >= 16)
		return false;
	if (y < 0 || y >= 256)
		return false;
	if (z < 0 || z >= 16)
		return false;
	return true;
}

vector<bool> Chunk::checkBlock(int x, int y, int z) const
{
	vector<bool> block_check_arr;
	int dx[] = {0, 0, 0, 0, -1, 1};
	int dy[] = {1, -1, 0, 0, 0, 0};
	int dz[] = {0, 0, -1, 1, 0, 0};
	for (int i = 0; i < 6; i++) {
		int nx = x + dx[i];
		int ny = y + dy[i];
		int nz = z + dz[i];
		if (this->checkBoundary(nx, ny, nz)) {
			if (this->chunk[nx][ny][nz])
				block_check_arr.push_back(false);
			else
				block_check_arr.push_back(true);
		}
		else if (ny < 0 || ny >= 256) {
			block_check_arr.push_back(true);
		}
		else if (nx < 0 || nx >= 16) {
			int block_flag = 0;
			if (nx < 0 && this->left.lock().use_count())
				block_flag = this->left.lock()->getBlock(15, ny, nz);
			else if (this->right.lock().use_count())
				block_flag = this->right.lock()->getBlock(0, ny, nz);
			if (block_flag == 0)
				block_check_arr.push_back(true);
			else
				block_check_arr.push_back(false);
		}
		else {
			int block_flag = 0;
			if (nz < 0 && this->front.lock().use_count())
				block_flag = front.lock()->getBlock(nx, ny, 15);
			else if (this->back.lock().use_count())
				block_flag = back.lock()->getBlock(nx, ny, 0);
			if (block_flag == 0)
				block_check_arr.push_back(true);
			else
				block_check_arr.push_back(false);
		}
	}
	return block_check_arr;
}

vector<vec3> Chunk::getBlockFaceVertexPos(
	float move_x,
	float move_y,
	float move_z,
	Face face_flag
) const
{
	vector<vec3> vertices_pos;
	float x = move_x + this->start_pos.x;
	float y = move_y + this->start_pos.y;
	float z = move_z + this->start_pos.z;

	if (face_flag == Face::Top) {
		vertices_pos.push_back(vec3(x - 0.5f, y + 0.5f, z - 0.5f));
		vertices_pos.push_back(vec3(x - 0.5f, y + 0.5f, z - 0.5f));
		vertices_pos.push_back(vec3(x + 0.5f, y + 0.5f, z + 0.5f));
		vertices_pos.push_back(vec3(x + 0.5f, y + 0.5f, z - 0.5f));
		return vertices_pos;
	}
	if (face_flag == Face::Bottom) {
		vertices_pos.push_back(vec3(x - 0.5f, y - 0.5f, z - 0.5f));
		vertices_pos.push_back(vec3(x + 0.5f, y - 0.5f, z - 0.5f));
		vertices_pos.push_back(vec3(x + 0.5f, y - 0.5f, z + 0.5f));
		vertices_pos.push_back(vec3(x - 0.5f, y - 0.5f, z + 0.5f));
		return vertices_pos;
	}
	if (face_flag == Face::Front) {
		vertices_pos.push_back(vec3(x - 0.5f, y - 0.5f, z - 0.5f));
		vertices_pos.push_back(vec3(x - 0.5f, y + 0.5f, z - 0.5f));
		vertices_pos.push_back(vec3(x + 0.5f, y + 0.5f, z - 0.5f));
		vertices_pos.push_back(vec3(x + 0.5f, y - 0.5f, z - 0.5f));
		return vertices_pos;
	}
	if (face_flag == Face::Back) {
		vertices_pos.push_back(vec3(x - 0.5f, y - 0.5f, z + 0.5f));
		vertices_pos.push_back(vec3(x + 0.5f, y - 0.5f, z + 0.5f));
		vertices_pos.push_back(vec3(x + 0.5f, y + 0.5f, z + 0.5f));
		vertices_pos.push_back(vec3(x - 0.5f, y + 0.5f, z + 0.5f));
		return vertices_pos;
	}
	if (face_flag == Face::Left) {
		vertices_pos.push_back(vec3(x - 0.5f, y - 0.5f, z + 0.5f));
		vertices_pos.push_back(vec3(x - 0.5f, y + 0.5f, z + 0.5f));
		vertices_pos.push_back(vec3(x - 0.5f, y + 0.5f, z - 0.5f));
		vertices_pos.push_back(vec3(x - 0.5f, y - 0.5f, z - 0.5f));
		return vertices_pos;
	}
	if (face_flag == Face::Right) {
		vertices_pos.push_back(vec3(x + 0.5f, y - 0.5f, z + 0.5f));
		vertices_pos.push_back(vec3(x + 0.5f, y - 0.5f, z - 0.5f));
		vertices_pos.push_back(vec3(x + 0.5f, y + 0.5f, z - 0.5f));
		vertices_pos.push_back(vec3(x + 0.5f, y + 0.5f, z + 0.5f));
	}
	return vertices_pos;
}

vector<vec2> Chunk::getBlockFaceTexcoords(
	vec2 start, 
	vec2 end,
	Face face_flag
) const
{
	vector<vec2> texcoord;

	if (face_flag == Face::Top) {
		texcoord.push_back(vec2(start.x, end.y));
		texcoord.push_back(start);
		texcoord.push_back(vec2(end.x, start.y));
		texcoord.push_back(end);
		return texcoord;
	}
	if (face_flag == Face::Bottom) {
		texcoord.push_back(start);
		texcoord.push_back(vec2(end.x, start.y));
		texcoord.push_back(end);
		texcoord.push_back(vec2(start.x, end.y));
		return texcoord;
	}
	if (face_flag == Face::Front) {
		texcoord.push_back(vec2(start.x, end.y));
		texcoord.push_back(start);
		texcoord.push_back(vec2(end.x, start.y));
		texcoord.push_back(end);
		return texcoord;
	}
	if (face_flag == Face::Back) {
		texcoord.push_back(end);
		texcoord.push_back(vec2(start.x, end.y));
		texcoord.push_back(start);
		texcoord.push_back(vec2(end.x, start.y));
		return texcoord;
	}
	if (face_flag == Face::Left) {
		texcoord.push_back(vec2(start.x, end.y));
		texcoord.push_back(start);
		texcoord.push_back(vec2(end.x, start.y));
		texcoord.push_back(end);
		return texcoord;
	}
	if (face_flag == Face::Right) {
		texcoord.push_back(end);
		texcoord.push_back(vec2(start.x, end.y));
		texcoord.push_back(start);
		texcoord.push_back(vec2(end.x, start.y));
	}
	return texcoord;
}

vector<uint32> Chunk::getBlockFaceIndices(uint32 start) const
{
	vector<uint32> indices = {
		start,
		start + 1,
		start + 2,
		start,
		start + 2,
		start + 3
	};
	return indices;
}


// temp code


void Chunk::initRenderForTest(HWND hwnd, UINT width, UINT height)
{
	cout << "vertices size: " << this->t_vertices.size() << endl;
	this->graphic = make_shared<Graphics>(hwnd, width, height);
	
	this->vertex_buffer = make_shared<Buffer<Vertex>>(
		this->graphic->getDevice(),
		this->t_vertices.data(),
		this->t_vertices.size(),
		D3D11_BIND_VERTEX_BUFFER
	);
	this->index_buffer = make_shared<Buffer<uint32>>(
		this->graphic->getDevice(),
		this->indices.data(),
		this->indices.size(),
		D3D11_BIND_INDEX_BUFFER
	);

	this->vertex_shader = make_shared<VertexShader>(
		this->graphic->getDevice(),
		L"VertexShader.hlsl",
		"main",
		"vs_5_0"
	);

	vector<D3D11_INPUT_ELEMENT_DESC> layout = {
		{
			"POSITION",
			0,
			DXGI_FORMAT_R32G32B32_FLOAT,
			0,
			0,
			D3D11_INPUT_PER_VERTEX_DATA,
			0
		},
		{
			"COLOR",
			0,
			DXGI_FORMAT_R32G32B32A32_FLOAT,
			0,
			12,
			D3D11_INPUT_PER_VERTEX_DATA,
			0
		}
	};

	this->input_layout = make_shared<InputLayout>(
		this->graphic->getDevice(),
		layout,
		this->vertex_shader->getBlob()
	);

	this->pixel_shader = make_shared<PixelShader>(
		this->graphic->getDevice(),
		L"PixelShader.hlsl",
		"main",
		"ps_5_0"
	);

	this->rasterizer_state = make_shared<RasterizerState>(
		this->graphic->getDevice(),
		D3D11_FILL_SOLID,
		D3D11_CULL_BACK
	);

	MVP mvp;
	mvp.model = Mat::Identity;
	mvp.view = XMMatrixLookAtLH(
		vec3(0, 0, -5),
		vec3(0, 0, 1),
		vec3(0, 1, 0)
	);
	mvp.view = mvp.view.Transpose();
	mvp.proj = XMMatrixPerspectiveFovLH(
		XMConvertToRadians(70),
		800.f / 650.f,
		0.01f,
		1000.f
	);
	mvp.proj = mvp.proj.Transpose();
	this->constant_buffer = make_shared<ConstantBuffer>(
		this->graphic->getDevice(),
		this->graphic->getContext(),
		mvp
	);

	this->graphic->getContext()->IASetInputLayout(
		this->input_layout->getComPtr().Get()
	);
	uint32 stride = this->vertex_buffer->getStride();
	uint32 offset = 0;
	this->graphic->getContext()->IASetVertexBuffers(
		0,
		1,
		this->vertex_buffer->getComPtr().GetAddressOf(),
		&stride,
		&offset
	);
	this->graphic->getContext()->IASetIndexBuffer(
		this->index_buffer->getComPtr().Get(),
		DXGI_FORMAT_R32_UINT,
		0
	);

	this->graphic->getContext()->VSSetShader(
		this->vertex_shader->getComPtr().Get(),
		nullptr,
		0
	);
	this->graphic->getContext()->VSSetConstantBuffers(
		0,
		1,
		this->constant_buffer->getComPtr().GetAddressOf()
	);

	this->graphic->getContext()->PSSetShader(
		this->pixel_shader->getComPtr().Get(),
		nullptr,
		0
	);

	this->graphic->setClearColor(0.3f, 0.3f, 0.3f, 1.f);

	this->constant_buffer->update(mvp);
}

void Chunk::setVerticesAndIndicesForTest()
{
	Vertex vertex;
	uint32 index = 0;
	for (int z = 0; z < 16; z++) {
		for (int y = 0; y < 256; y++) {
			for (int x = 0; x < 16; x++) {
				if (this->chunk[x][y][z] == 0)
					continue;
				vector<Vertex> block_vertices =
					this->getBlockVertexForTest(x, y, z);
				vector<uint32> block_indices =
					this->getBlockIndices(x, y, z, index);
				this->t_vertices.insert(
					this->t_vertices.end(),
					block_vertices.begin(),
					block_vertices.end()
				);
				this->indices.insert(
					this->indices.end(),
					block_indices.begin(),
					block_indices.end()
				);
			}
		}
	}
}

void Chunk::renderTest()
{
	this->graphic->renderBegin();
	this->graphic->getContext()->DrawIndexed(
		this->indices.size(),
		0,
		0
	);
	this->graphic->renderEnd();
}

vector<Vertex> Chunk::getBlockVertexForTest(
	int x, 
	int y, 
	int z
) const
{
	vector<Vertex> block;
	vector<bool> set_able_arr = this->checkBlock(x, y, z);
	for (int i = 0; i < 6; i++) {
		Face flag = static_cast<Face>(i);
		vector<vec3> positions;
		color col;
		Vertex vertex;
		if (set_able_arr[i]) {
			positions = this->getBlockFaceVertexPos(x, y, z, flag);
			if (i == 0)
				col = color(1, 0, 0, 1);
			else if (i == 1)
				col = color(0, 1, 0, 1);
			else if (i == 2)
				col = color(0, 0, 1, 1);
			else if (i == 3)
				col = color(1, 1, 0, 1);
			else if (i == 4)
				col = color(1, 0, 1, 1);
			else
				col = color(0, 1, 1, 1);
			for (int j = 0; j < 4; j++) {
				vertex.pos = positions[j];
				vertex.color = col;
				block.push_back(vertex);
			}
		}
	}
	return block;
}
