#include "pch.h"
#include "Chunk.h"

Chunk::Chunk(shared_ptr<Graphics> graphic)
{
	this->graphic = graphic;

	// test
	this->block = make_shared<Block>(1);
}

Chunk::~Chunk()
{
}

void Chunk::setVerticesAndIndices()
{
	VertexBlockUV vertex;
	uint32 index = 0;
	for (int z = 0; z < 16; z++) {
		for (int y = 0; y < 256; y++) {
			for (int x = 0; x < 16; x++) {
				if (this->chunk[x][y][z] == 0)
					continue;
				vector<VertexBlockUV> block_vertices =
					this->getBlockVertexBlockUV(x, y, z, 
						this->chunk[x][y][z]);
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

vector<VertexBlockUV> Chunk::getBlockVertexBlockUV(
	int x, 
	int y, 
	int z,
	int type
) const
{
	vector<VertexBlockUV> cube_vertices;
	vector<bool> put_able_arr = this->checkBlock(x, y, z);
	vec2 start = vec2(0.f, 0.f);
	vec2 end = vec2(1.f, 1.f);

	for (int i = 0; i < put_able_arr.size(); i++) {
		BlockFace flag = static_cast<BlockFace>(i);
		VertexBlockUV tmp_vertex;
		vector<vec3> positions;
		vector<vec2> texcoords;
		if (put_able_arr[i]) {
			positions = this->block->getBlockFacePos(
				this->start_pos.x + x, 
				this->start_pos.y + y, 
				this->start_pos.z + z, 
				flag
			);
			texcoords = this->block->getBlockFaceTexcoord(
				start,
				end,
				flag
			);
			for (int j = 0; j < 4; j++) {
				tmp_vertex.type = type;
				tmp_vertex.pos = positions[j];
				tmp_vertex.uv = texcoords[j];
				tmp_vertex.dir = i;
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
			face_indices = this->block->getBlockFaceIndices(start);
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


// temp code


void Chunk::initRenderForTest(HWND hwnd, UINT width, UINT height)
{
	cout << "vertices size: " << this->vertices.size() << endl;
	

	this->vertex_uv_buffer = make_shared<Buffer<VertexBlockUV>>(
		this->graphic->getDevice(),
		this->vertices.data(),
		this->vertices.size(),
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
		L"WorldVertexShader.hlsl",
		"main",
		"vs_5_0"
	);

	vector<D3D11_INPUT_ELEMENT_DESC> layout = {
		{
			"TYPE",
			0,
			DXGI_FORMAT_R32_SINT,
			0,
			0,
			D3D11_INPUT_PER_VERTEX_DATA,
			0
		},
		{
			"POSITION",
			0,
			DXGI_FORMAT_R32G32B32_FLOAT,
			0,
			4,
			D3D11_INPUT_PER_VERTEX_DATA,
			0
		},
		{
			"TEXCOORD",
			0,
			DXGI_FORMAT_R32G32_FLOAT,
			0,
			16,
			D3D11_INPUT_PER_VERTEX_DATA,
			0
		},
		{
			"DIRECTION",
			0,
			DXGI_FORMAT_R32_SINT,
			0,
			24,
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
		L"WorldPixelShader.hlsl",
		"main",
		"ps_5_0"
	);

	this->sampler_state = make_shared<SamplerState>(
		this->graphic->getDevice()
	);

	this->blend_state = make_shared<BlendState>(
		this->graphic->getDevice()
	);

	this->rasterizer_state = make_shared<RasterizerState>(
		this->graphic->getDevice(),
		D3D11_FILL_SOLID,
		D3D11_CULL_BACK
	);

	MVP mvp;
	mvp.model = Mat::Identity;
	mvp.view = XMMatrixLookAtLH(
		vec3(0, 3, 0),
		vec3(0, -1, 0.00001),
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
	uint32 stride = this->vertex_uv_buffer->getStride();
	uint32 offset = 0;
	this->graphic->getContext()->IASetVertexBuffers(
		0,
		1,
		this->vertex_uv_buffer->getComPtr().GetAddressOf(),
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
	this->graphic->getContext()->PSSetSamplers(
		0,
		1,
		this->sampler_state->getComPtr().GetAddressOf()
	);

	vector<wstring> path_arr = {
		L"grass_top.png",
		L"grass_bottom.png",
		L"grass_side.png"
	};
	this->block->registerSRV(
		this->graphic->getDevice(),
		this->graphic->getContext(),
		path_arr
	);
	this->graphic->getContext()->OMSetBlendState(
		this->blend_state->getComPtr().Get(),
		this->blend_state->getBlendFactor(),
		this->blend_state->getSampleMask()
	);

	this->graphic->setClearColor(0.3f, 0.3f, 0.3f, 1.f);

	this->constant_buffer->update(mvp);
}

void Chunk::setVerticesAndIndicesForTest()
{
	uint32 index = 0;
	for (int z = 0; z < 16; z++) {
		for (int y = 0; y < 256; y++) {
			for (int x = 0; x < 16; x++) {
				if (this->chunk[x][y][z] == 0)
					continue;
				vector<VertexBlock> block_vertices =
					this->getBlockVertexForTest(x, y, z, 
						this->chunk[x][y][z]);
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

vector<VertexBlock> Chunk::getBlockVertexForTest(
	int x, 
	int y, 
	int z,
	int type
) const
{
	vector<VertexBlock> block;
	vector<bool> set_able_arr = this->checkBlock(x, y, z);
	for (int i = 0; i < 6; i++) {
		BlockFace flag = static_cast<BlockFace>(i);
		vector<vec3> positions;
		color col;
		VertexBlock vertex;
		if (set_able_arr[i]) {
			positions = this->block->getBlockFacePos(x, y, z, flag);
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
				vertex.type = type;
				vertex.pos = positions[j];
				vertex.col = col;
				block.push_back(vertex);
			}
		}
	}
	return block;
}
