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

void Chunk::setVerticesAndIndices()
{
	VertexBlockUV vertex;
	uint32 index = 0;
	for (int y = 0; y < 256; y++) {
		for (int z = 0; z < 16; z++) {
			for (int x = 0; x < 16; x++) {
				if (this->chunk[x + z * 16 + y * 16 * 16] == 0)
					continue;
				vector<int> check_arr = this->checkBlock(x, y, z);
				if (check_arr.size() == 0)
					continue;
				this->blocks.insert({ { x, y, z}, true });
				vector<VertexBlockUV> block_vertices =
					this->getBlockVertexBlockUV(x, y, z,
						this->chunk[x + z * 16 + y * 16 * 16],
						check_arr
					);
				vector<uint32> block_indices =
					this->getBlockIndices(x, y, z, index,
						check_arr
					);
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

void Chunk::updateVerticesAndIndices()
{
	uint32 index = 0;
	this->vertices.clear();
	this->indices.clear();
	int jz = 16;
	int jy = 16 * 16;
	unordered_map<Index3, bool>::iterator it = this->blocks.begin();
	for (it; it != this->blocks.end(); it++) {
		vector<int> check_arr = this->checkBlock(
			it->first.x, it->first.y, it->first.z);
		vector<VertexBlockUV> block_vertices =
			this->getBlockVertexBlockUV(
				it->first.x, it->first.y, it->first.z,
				this->chunk[it->first.x + it->first.z * jz + it->first.y * jy],
				check_arr
			);
		vector<uint32> block_indices =
			this->getBlockIndices(
				it->first.x, it->first.y, it->first.z, 
				index,
				check_arr
			);
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



void Chunk::setBlockInChunk(int x, int y, int z, int16 type)
{
	this->chunk[x + z * 16 + y * 16 * 16] = type;
	this->block_cnt++;
}

void Chunk::addBlock(Index3, int16 type)
{
}

void Chunk::deleteBlock(vector<Index3> const& block_arr)
{
	unordered_map<Index3, bool>::iterator it;
	int dx[] = { 0, 0, 0, 0, -1, 1 };
	int dy[] = { 1, -1, 0, 0, 0, 0 };
	int dz[] = { 0, 0, 1, -1, 0, 0 };

	int jz = 16;
	int jy = 16 * 16;
	for (int i = 0; i < block_arr.size(); i++) {
		it = this->blocks.find(block_arr[i]);
		if (it == this->blocks.end())
			continue;
		vector<int> move_arr = this->checkBlockReverse(
			block_arr[i].x,
			block_arr[i].y,
			block_arr[i].z
		);
		for (int j = 0; j < move_arr.size(); j++) {
			int nx = block_arr[i].x + dx[move_arr[j]];
			int ny = block_arr[i].y + dy[move_arr[j]];
			int nz = block_arr[i].z + dz[move_arr[j]];
			unordered_map<Index3, bool>::iterator itt = this->blocks.find({ nx, ny, nz });
			if (itt == this->blocks.end())
				this->blocks.insert({ {nx, ny, nz}, true });
		}
		this->chunk[block_arr[i].x + block_arr[i].y * jy + block_arr[i].z * jz] = 0;
		this->blocks.erase(it);
	}
	this->updateVerticesAndIndices();
}

int Chunk::getBlock(int x, int y, int z) const
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

void Chunk::setChunk(Chunk* chunk, string const& str)
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

int Chunk::getBlockCnt()
{
	return this->block_cnt;
}

vector<VertexBlockUV> Chunk::getBlockVertexBlockUV(
	int x,
	int y,
	int z,
	int type,
	vector<int> const& check_arr
) const
{
	vector<VertexBlockUV> cube_vertices;
	vec2 start = vec2(0.f, 0.f);
	vec2 end = vec2(1.f, 1.f);

	for (int i = 0; i < check_arr.size(); i++) {
		Face flag = static_cast<Face>(check_arr[i]);
		VertexBlockUV tmp_vertex;
		vector<vec3> positions;
		vector<vec2> texcoords;
		positions = this->getBlockFacePos(x, y, z, flag);
		texcoords = this->getBlockFaceTexcoord(
			start,
			end,
			flag
		);
		for (int j = 0; j < 4; j++) {
			tmp_vertex.type = type;
			tmp_vertex.pos = positions[j];
			tmp_vertex.uv = texcoords[j];
			tmp_vertex.dir = check_arr[i];
			cube_vertices.push_back(tmp_vertex);
		}
	}
	return cube_vertices;
}

vector<uint32> Chunk::getBlockIndices(
	int x,
	int y,
	int z,
	uint32& start,
	vector<int> const& check_arr
) const
{
	vector<uint32> block_indices;
	for (int i = 0; i < check_arr.size(); i++) {
		vector<uint32> face_indices;
		face_indices = this->getBlockFaceIndices(start);
		start += 4;
		block_indices.insert(
			block_indices.end(),
			face_indices.begin(),
			face_indices.end()
		);
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

void defineTrueFace(vector<int>& vec, int flag, int idx) {
	if (flag != 0)
		return;
	vec.push_back(idx);
}


vector<int> Chunk::checkBlock(int x, int y, int z) const
{
	vector<int> face_arr;
	int dx[] = { 0, 0, 0, 0, -1, 1 };
	int dy[] = { 1, -1, 0, 0, 0, 0 };
	int dz[] = { 0, 0, 1, -1, 0, 0 };
	int jz = 16;
	int jy = 16 * 16;
	for (int i = 0; i < 6; i++) {
		int nx = x + dx[i];
		int ny = y + dy[i];
		int nz = z + dz[i];
		if (this->checkBoundary(nx, ny, nz))
			defineTrueFace(face_arr, this->chunk[nx + ny * jy + nz * jz], i);
		else {
			if (ny < 0 || ny >= 256)
				face_arr.push_back(i);
			else if (nx < 0 && this->left == nullptr)
				face_arr.push_back(i);
			else if (nx >= 16 && this->right == nullptr)
				face_arr.push_back(i);
			else if (nz < 0 && this->back == nullptr)
				face_arr.push_back(i);
			else if (nz >= 16 && this->front == nullptr)
				face_arr.push_back(i);
			else
				defineTrueFace(face_arr, this->getBlock(nx, ny, nz), i);
		}
	}
	return face_arr;
}

vector<int> Chunk::checkBlockReverse(int x, int y, int z) const
{
	vector<int> face_arr;
	int dx[] = { 0, 0, 0, 0, -1, 1 };
	int dy[] = { 1, -1, 0, 0, 0, 0 };
	int dz[] = { 0, 0, 1, -1, 0, 0 };
	int jy = 16 * 16;
	int jz = 16;
	for (int i = 0; i < 6; i++) {
		int nx = x + dx[i];
		int ny = y + dy[i];
		int nz = z + dz[i];
		if (this->checkBoundary(nx, ny, nz)) {
			if (this->chunk[nx + ny * jy + nz * jz])
				face_arr.push_back(i);
		}
	}
	return face_arr;
}

vector<vec3> Chunk::getBlockFacePos
(
	float x,
	float y,
	float z,
	Face block_face
) const
{
	vector<vec3> vertices_pos;
	x = this->start_pos.x + x;
	y = this->start_pos.y + y;
	z = this->start_pos.z - z;
	if (block_face == Face::Top) {
		vertices_pos.push_back(vec3(x - 0.5f, y + 0.5f, z - 0.5f));
		vertices_pos.push_back(vec3(x - 0.5f, y + 0.5f, z + 0.5f));
		vertices_pos.push_back(vec3(x + 0.5f, y + 0.5f, z + 0.5f));
		vertices_pos.push_back(vec3(x + 0.5f, y + 0.5f, z - 0.5f));
		return vertices_pos;
	}
	if (block_face == Face::Bottom) {
		vertices_pos.push_back(vec3(x - 0.5f, y - 0.5f, z - 0.5f));
		vertices_pos.push_back(vec3(x + 0.5f, y - 0.5f, z - 0.5f));
		vertices_pos.push_back(vec3(x + 0.5f, y - 0.5f, z + 0.5f));
		vertices_pos.push_back(vec3(x - 0.5f, y - 0.5f, z + 0.5f));
		return vertices_pos;
	}
	if (block_face == Face::Front) {
		vertices_pos.push_back(vec3(x - 0.5f, y - 0.5f, z - 0.5f));
		vertices_pos.push_back(vec3(x - 0.5f, y + 0.5f, z - 0.5f));
		vertices_pos.push_back(vec3(x + 0.5f, y + 0.5f, z - 0.5f));
		vertices_pos.push_back(vec3(x + 0.5f, y - 0.5f, z - 0.5f));
		return vertices_pos;
	}
	if (block_face == Face::Back) {
		vertices_pos.push_back(vec3(x - 0.5f, y - 0.5f, z + 0.5f));
		vertices_pos.push_back(vec3(x + 0.5f, y - 0.5f, z + 0.5f));
		vertices_pos.push_back(vec3(x + 0.5f, y + 0.5f, z + 0.5f));
		vertices_pos.push_back(vec3(x - 0.5f, y + 0.5f, z + 0.5f));
		return vertices_pos;
	}
	if (block_face == Face::Left) {
		vertices_pos.push_back(vec3(x - 0.5f, y - 0.5f, z + 0.5f));
		vertices_pos.push_back(vec3(x - 0.5f, y + 0.5f, z + 0.5f));
		vertices_pos.push_back(vec3(x - 0.5f, y + 0.5f, z - 0.5f));
		vertices_pos.push_back(vec3(x - 0.5f, y - 0.5f, z - 0.5f));
		return vertices_pos;
	}
	if (block_face == Face::Right) {
		vertices_pos.push_back(vec3(x + 0.5f, y - 0.5f, z + 0.5f));
		vertices_pos.push_back(vec3(x + 0.5f, y - 0.5f, z - 0.5f));
		vertices_pos.push_back(vec3(x + 0.5f, y + 0.5f, z - 0.5f));
		vertices_pos.push_back(vec3(x + 0.5f, y + 0.5f, z + 0.5f));
	}
	return vertices_pos;
}

vector<vec2> Chunk::getBlockFaceTexcoord
(
	vec2 start,
	vec2 end,
	Face block_face
) const
{
	vector<vec2> texcoord;

	if (block_face == Face::Top) {
		texcoord.push_back(vec2(start.x, end.y));
		texcoord.push_back(start);
		texcoord.push_back(vec2(end.x, start.y));
		texcoord.push_back(end);
		return texcoord;
	}
	if (block_face == Face::Bottom) {
		texcoord.push_back(start);
		texcoord.push_back(vec2(end.x, start.y));
		texcoord.push_back(end);
		texcoord.push_back(vec2(start.x, end.y));
		return texcoord;
	}
	if (block_face == Face::Front) {
		texcoord.push_back(vec2(start.x, end.y));
		texcoord.push_back(start);
		texcoord.push_back(vec2(end.x, start.y));
		texcoord.push_back(end);
		return texcoord;
	}
	if (block_face == Face::Back) {
		texcoord.push_back(end);
		texcoord.push_back(vec2(start.x, end.y));
		texcoord.push_back(start);
		texcoord.push_back(vec2(end.x, start.y));
		return texcoord;
	}
	if (block_face == Face::Left) {
		texcoord.push_back(vec2(start.x, end.y));
		texcoord.push_back(start);
		texcoord.push_back(vec2(end.x, start.y));
		texcoord.push_back(end);
		return texcoord;
	}
	if (block_face == Face::Right) {
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

void Chunk::Render
(
	Mat view,
	Mat proj,
	shared_ptr<TextureArray> const& texture_array
)
{
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