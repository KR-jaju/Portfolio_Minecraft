#include "pch.h"
#include "Chunk.h"

#include "Buffer.h"
#include "InputLayout.h"
#include "Graphics.h"
#include "ConstantBuffer.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "RasterizerState.h"
#include "BlendState.h"
#include "SamplerState.h"
#include "TextureArray.h"
#include <fstream>
#include <sstream>

Chunk::Chunk()
{
	this->front = nullptr;
	this->back = nullptr;
	this->left = nullptr;
	this->right = nullptr;
	this->block_cnt = 0;
	fill(
		this->chunk,
		this->chunk + 16 * 256 * 16,
		0
	);
}

Chunk::~Chunk()
{
}


void Chunk::vertexAndIndexGenerator(
	Face const& face, 
	int const& dx,
	int const& dy,
	int const& dz,
	uint32& idx)
{
	int my = 16 * 16;
	int mz = 16;
	
	for (int y = 0; y < 256; y++) {
		for (int z = 0; z < 16; z++) {
			for (int x = 0; x < 16; x++) {
				int nx = x + dx;
				int ny = y + dy;
				int nz = z + dz;
				if (this->chunk[x + z * mz + y * my] == 0)
					continue;
				if (this->checkBoundary(nx, ny, nz)) {
					if (this->getBlock(nx, ny, nz))
						continue;
				}
				else {
					if (face == Face::Left && this->left && this->getBlock(nx, ny, nz))
						continue;
					if (face == Face::Right && this->right && this->getBlock(nx, ny, nz))
						continue;
					if (face == Face::Front && this->front && this->getBlock(nx, ny, nz))
						continue;
					if (face == Face::Back && this->back && this->getBlock(nx, ny, nz))
						continue;
				}
				this->getBlockFacePosAndTex(static_cast<int>(face), x, y, z,
					this->chunk[x + z * mz + y * my]);
				this->getBlockFaceIndices(idx);
				idx += 4;
			}
		}
	}
}

void Chunk::setVerticesAndIndices()
{
	uint32 index = 0;
	this->vertexAndIndexGenerator(Face::Right, 1, 0, 0, index);
	this->vertexAndIndexGenerator(Face::Left, -1, 0, 0, index);
	this->vertexAndIndexGenerator(Face::Front, 0, 0, 1, index);
	this->vertexAndIndexGenerator(Face::Back, 0, 0, -1, index);
	this->vertexAndIndexGenerator(Face::Top, 0, 1, 0, index);
	this->vertexAndIndexGenerator(Face::Bottom, 0, -1, 0, index);
}



void Chunk::setBlockInChunk(int x, int y, int z, int16 type)
{
	this->chunk[x + z * 16 + y * 16 * 16] = type;
	this->block_cnt++;
}

void Chunk::addBlock(Index3 idx3, int16 type)
{
	this->chunk[idx3.x + idx3.y * 16 * 16 + idx3.z * 16] = type;
	this->block_cnt++;
	this->indices.clear();
	this->vertices.clear();
	this->setVerticesAndIndices();
}

void Chunk::deleteBlock(vector<Index3> const& block_arr) // terrain
{
	int jz = 16;
	int jy = 16 * 16;
	for (int i = 0; i < block_arr.size(); i++) {
		this->chunk[block_arr[i].x + 
			block_arr[i].y * jy + block_arr[i].z * jz] = 0;
	}
	this->block_cnt -= block_arr.size();
	this->indices.clear();
	this->vertices.clear();
	this->setVerticesAndIndices();
}

int Chunk::getBlock(int x, int y, int z) const // terrain
{
	if (x < 0)
		return this->left->getBlock(16 + x, y, z);
	if (x >= 16)
		return this->right->getBlock(x % 16, y, z);
	if (z < 0)
		return this->back->getBlock(x, y, 16 + z);
	if (z >= 16)
		return this->front->getBlock(x, y, z % 16);
	return this->chunk[x + y * 16 * 16 + z * 16];
}

void Chunk::setStartPos(float x, float y, float z)
{
	this->start_pos = vec3(x, y, z) + vec3(0.5f, 0.5f, -0.5f);
}

vec3 Chunk::getStartPos() const
{
	return this->start_pos;
}

void Chunk::setChunk(Chunk* chunk, string const& str) // no
{
	if (str == "left")
		this->left = chunk;
	else if (str == "right")
		this->right = chunk;
	else if (str == "front")
		this->front = chunk;
	else if (str == "back")
		this->back = chunk;
}

int Chunk::getBlockCnt() // no
{
	return this->block_cnt;
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

void defineTrueFace(vector<int>& vec, int flag, int idx) {
	if (flag != 0)
		return;
	vec.push_back(idx);
}


void Chunk::getBlockFacePosAndTex(
	int dir, 
	float x, 
	float y, 
	float z, 
	int type
)
{
	static vector<vec3> sample_pos = {
		{-0.5f, +0.5f, -0.5f},
		{-0.5f, +0.5f, +0.5f},
		{+0.5f, +0.5f, +0.5f},
		{+0.5f, +0.5f, -0.5f},

		{-0.5f, -0.5f, -0.5f},
		{+0.5f, -0.5f, -0.5f},
		{+0.5f, -0.5f, +0.5f},
		{-0.5f, -0.5f, +0.5f},

		{-0.5f, -0.5f, -0.5f},
		{-0.5f, +0.5f, -0.5f},
		{+0.5f, +0.5f, -0.5f},
		{+0.5f, -0.5f, -0.5f},

		{-0.5f, -0.5f, +0.5f},
		{+0.5f, -0.5f, +0.5f},
		{+0.5f, +0.5f, +0.5f},
		{-0.5f, +0.5f, +0.5f},

		{-0.5f, -0.5f, +0.5f},
		{-0.5f, +0.5f, +0.5f},
		{-0.5f, +0.5f, -0.5f},
		{-0.5f, -0.5f, -0.5f},

		{+0.5f, -0.5f, +0.5f},
		{+0.5f, -0.5f, -0.5f},
		{+0.5f, +0.5f, -0.5f},
		{+0.5f, +0.5f, +0.5f}
	};
	static vector<vec2> sample_uv = {
		{0.f, 1.f},
		{0.f, 0.f},
		{1.f, 0.f},
		{1.f, 1.f},

		{0.f, 0.f},
		{1.f, 0.f},
		{1.f, 1.f},
		{0.f, 1.f},

		{0.f, 1.f},
		{0.f, 0.f},
		{1.f, 0.f},
		{1.f, 1.f},

		{1.f, 1.f},
		{0.f, 1.f},
		{0.f, 0.f},
		{1.f, 0.f},

		{0.f, 1.f},
		{0.f, 0.f},
		{1.f, 0.f},
		{1.f, 1.f},

		{1.f, 1.f},
		{0.f, 1.f},
		{0.f, 0.f},
		{1.f, 0.f}
	};
	VertexBlockUV vertex;
	x = this->start_pos.x + x;
	y = this->start_pos.y + y;
	z = this->start_pos.z - z;
	for (int i = dir * 4; i < dir * 4 + 4; i++) {
		vertex.pos = sample_pos[i] + vec3(x, y, z);
		vertex.uv = sample_uv[i];
		vertex.type = type;
		vertex.dir = dir;
		this->vertices.push_back(vertex);
	}
}

void Chunk::getBlockFaceIndices(uint32 start)
{
	this->indices.push_back(start);
	this->indices.push_back(start + 1);
	this->indices.push_back(start + 2);
	this->indices.push_back(start);
	this->indices.push_back(start + 2);
	this->indices.push_back(start + 3);
}

void Chunk::setCamPos(vec3 const& pos)
{
	this->cam_pos = pos;
}


void Chunk::setRender
(
	shared_ptr<Graphics> graphic,
	shared_ptr<RasterizerState> rasterizer_state,
	shared_ptr<SamplerState> sampler_state,
	wstring const& vertex_shader_path,
	wstring const& pixel_shader_path,
	shared_ptr<BlendState> blend_state
)
{
	this->graphic = graphic;
	this->rasterizer_state = rasterizer_state;
	this->sampler_state = sampler_state;
	this->vertex_buffer = make_shared<Buffer<VertexBlockUV>>(
		graphic->getDevice(),
		this->vertices.data(),
		static_cast<uint32>(this->vertices.size()),
		D3D11_BIND_VERTEX_BUFFER
	);
	this->index_buffer = make_shared<Buffer<uint32>>(
		graphic->getDevice(),
		this->indices.data(),
		static_cast<uint32>(this->indices.size()),
		D3D11_BIND_INDEX_BUFFER
	);

	this->vertex_shader = make_shared<VertexShader>(
		this->graphic->getDevice(),
		vertex_shader_path,
		"main",
		"vs_5_0"
	);
	this->layout = {
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
		layout.data(),
		layout.size(),
		vertex_shader->getBlob()
	);
	this->pixel_shader = make_shared<PixelShader>(
		this->graphic->getDevice(),
		pixel_shader_path,
		"main",
		"ps_5_0"
	);
	this->blend_state = blend_state;
}

void Chunk::setBuffer()
{
	this->vertex_buffer = make_shared<Buffer<VertexBlockUV>>(
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
}

void Chunk::Render
(
	Mat view,
	Mat proj,
	shared_ptr<TextureArray> const& texture_array
)
{
	CamPos cam;
	cam.pos = this->cam_pos;
	cam.r = 16.f * 1.f;
	size_t test = sizeof(cam);
	shared_ptr<ConstantBuffer> pixel_cbuffer;
	pixel_cbuffer = make_shared<ConstantBuffer>(
		this->graphic->getDevice(),
		this->graphic->getContext(),
		cam
	);
	pixel_cbuffer->update(cam);
	this->mvp.view = view.Transpose();
	this->mvp.proj = proj.Transpose();
	this->constant_buffer = make_shared<ConstantBuffer>(
		this->graphic->getDevice(),
		this->graphic->getContext(),
		this->mvp
	);
	this->constant_buffer->update(this->mvp);

	uint32 stride = this->vertex_buffer->getStride();
	uint32 offset = this->vertex_buffer->getOffset();
	this->graphic->getContext()->IASetVertexBuffers(
		0,
		1,
		this->vertex_buffer->getComPtr().GetAddressOf(),
		&stride,
		&offset
	);
	this->graphic->getContext()->IASetPrimitiveTopology(
		D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST
	);
	this->graphic->getContext()->IASetInputLayout(
		this->input_layout->getComPtr().Get()
	);
	this->graphic->getContext()->IASetIndexBuffer(
		this->index_buffer->getComPtr().Get(),
		DXGI_FORMAT_R32_UINT,
		0
	);

	// vs
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

	// rs
	this->graphic->getContext()->RSSetState(
		this->rasterizer_state->getComPtr().Get()
	);

	//ps
	this->graphic->getContext()->PSSetShader(
		this->pixel_shader->getComPtr().Get(),
		nullptr,
		0
	);
	this->graphic->getContext()->PSSetConstantBuffers(
		0,
		1,
		pixel_cbuffer->getComPtr().GetAddressOf()
	);
	this->graphic->getContext()->PSSetSamplers(
		0,
		1,
		this->sampler_state->getComPtr().GetAddressOf()
	);
	this->graphic->getContext()->PSSetShaderResources(
		0,
		1,
		texture_array->getComPtr().GetAddressOf()
	);

	// om
	this->graphic->getContext()->OMSetBlendState(
		this->blend_state->getComPtr().Get(),
		this->blend_state->getBlendFactor(),
		this->blend_state->getSampleMask()
	);
	this->graphic->getContext()->DrawIndexed(
		this->index_buffer->getCount(),
		0,
		0
	);
}

void Chunk::readFile(string const& path)
{
	ifstream ifile;
	int flag = 0;
	char buffer[200];
	VertexBlockUV vertex;
	int jy = 16 * 16;
	int jz = 16;
	fill(
		this->chunk,
		this->chunk + 16 * 256 * 16,
		0
	);
	this->vertices = {};
	this->indices = {};
	ifile.open(path.c_str());
	ifile.getline(buffer, 200);
	string str, token;
	str = buffer;
	stringstream stream(str);
	getline(stream, token, ',');
	this->start_pos.x = stof(token);
	this->start_pos.y = 0.5f;
	getline(stream, token, ',');
	this->start_pos.z = stof(token);
	for (int j = 0; j < 256; j++) {
		for (int i = 0; j < 16; i++) {
			ifile.getline(buffer, 200);
			str = buffer;
			stream.clear();
			stream.str(str);
			int idx = 0;
			while (getline(stream, token, ',')) {
				this->chunk[i * jz + j * jy + idx] = stoi(token);
				idx++;
			}
		}
	}
	ifile.getline(buffer, 200);
	str = buffer;
	int total = stoi(str);
	if (total) {
		for (int i = 0; i < total; i++) {
			ifile.getline(buffer, 200);
			str = buffer;
			stream.clear();
			stream.str(str);
			int idx = 0;
			while (getline(stream, token, ',')) {
				if (idx == 0)
					vertex.type = stoi(token);
				else if (idx == 1)
					vertex.pos.x = stof(token);
				else if (idx == 2)
					vertex.pos.y = stof(token);
				else if (idx == 3)
					vertex.pos.z = stof(token);
				else if (idx == 4)
					vertex.uv.x = stof(token);
				else if (idx == 5)
					vertex.uv.y = stof(token);
				else
					vertex.dir = stoi(token);
				idx++;
			}
			this->vertices.push_back(vertex);
		}
	}
	ifile.getline(buffer, 200);
	str = buffer;
	total = stoi(str);
	if (total) {
		for (int i = 0; i < total; i++) {
			ifile.getline(buffer, 200);
			str = buffer;
			stream.clear();
			stream.str(str);
			while (getline(stream, token, ','))
				this->indices.push_back(stoi(token));
		}
	}
	ifile.getline(buffer, 200);
	str = buffer;
	this->block_cnt = stoi(str);
	ifile.close();
}

void Chunk::updateFile() const
{
	int jy = 16 * 16;
	int jz = 16;
	string file_name = "../chunk_files/";
	file_name = file_name + to_string(this->start_pos.x) + "_"
		+ to_string(this->start_pos.z) + ".txt";
	ofstream ofile;
	ofile.open(file_name);
	string str = to_string(this->start_pos.x) + ",";
	str += to_string(this->start_pos.y) + ",";
	str += to_string(this->start_pos.z) + "\n";
	ofile.write(str.c_str(), str.size());
	for (int j = 0; j < 256; j++) {
		for (int i = 0; i < 16; i++) {
			str = "";
			for (int k = 0; k < 16; k++) {
				str += to_string(this->chunk[i * jz + j * jy + k]);
				if (k == 15)
					str += '\n';
				else
					str += ',';
			}
			ofile.write(str.c_str(), str.size());
		}
	}
	str = to_string(this->vertices.size()) + '\n';
	ofile.write(str.c_str(), str.size());
	for (int i = 0; i < this->vertices.size(); i++) {
		str = to_string(this->vertices[i].type) + ',';
		str += to_string(this->vertices[i].pos.x) + ',';
		str += to_string(this->vertices[i].pos.y) + ',';
		str += to_string(this->vertices[i].pos.z) + ',';
		str += to_string(this->vertices[i].uv.x) + ',';
		str += to_string(this->vertices[i].uv.y) + ',';
		str += to_string(this->vertices[i].dir) + '\n';
		ofile.write(str.c_str(), str.size());
	}
	str = to_string(this->indices.size() / 3) + "\n";
	ofile.write(str.c_str(), str.size());
	int cnt = 0;
	str = "";
	for (int i = 0; i < this->indices.size(); i++) {
		str += to_string(this->indices[i]);
		cnt++;
		if (cnt == 3) {
			str += '\n';
			ofile.write(str.c_str(), str.size());
			str = "";
			cnt = 0;
		}
		else
			str += ',';
	}
	str = to_string(this->block_cnt);
	ofile.write(str.c_str(), str.size());
	ofile.close();
}

void Chunk::setHeight(int x, int y, int16 h)
{
	this->height_map[y][x] = h;
}

int16 Chunk::getHeight(int x, int y) const
{
	return this->height_map[y][x];
}
