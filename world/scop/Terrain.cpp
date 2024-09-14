#include "pch.h"
#include "Terrain.h"
#include "Block.h"
#include "Chunk.h"
#include "thread"

#include "Graphics.h"
#include "TextureArray.h"
#include "RasterizerState.h"
#include "BlendState.h"
#include "SamplerState.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "ConstantBuffer.h"
#include "InputLayout.h"
#include <fstream>

#include <time.h>
#include "Buffer.h"

#define STDZERO 0.00000001

vec3 intersectionRayAndPlane(
	vec3 const& r_pos,
	vec3 const& r_dir,
	vec3 const& p_pos,
	vec3 const& p_dir
)
{
	vec3 res;
	float t;
	t = (p_pos.Dot(p_dir) - r_pos.Dot(p_dir)) / p_dir.Dot(r_dir);
	res = r_pos + t * r_dir;
	return res;
}

Terrain::Terrain(
	int size_w,
	int size_h,
	HWND hwnd,
	UINT width,
	UINT height,
	int fov_chunk,
	int thread_cnt
)
{
	this->size_h = size_h;
	this->size_w = size_w;

	this->s_pos.x = -8 * this->size_w;
	this->s_pos.y = 8 * this->size_h;
	this->sv_pos.x = this->s_pos.x + 16;
	this->sv_pos.y = this->s_pos.y - 16;
	this->ev_pos.x = -this->sv_pos.x;
	this->ev_pos.y = -this->sv_pos.y;

	this->graphic = make_shared<Graphics>(hwnd, width, height);
	this->blocks = new int[16 * 16 * size_h * size_w * 256];
	this->b_arr_size = 16 * 16 * size_h * size_w * 256;
	this->h_arr_size = 16 * 16 * size_h * size_w;
	this->h_map = new int[16 * 16 * size_h * size_w];
	this->c_fov = fov_chunk;
	this->thread_cnt = thread_cnt;
	fill(this->blocks, this->blocks + 16 * 16 * size_h * size_w * 256, 0);
	fill(this->h_map, this->h_map + 16 * 16 * size_h * size_w, 0);
	clock_t start, finish;
	start = clock();
	this->createHeightMap();
	finish = clock();
	cout << "set height(ms): " << static_cast<double>(finish - start) << endl;
	start = clock();
	this->terrainsetVerticesAndIndices();
	finish = clock();
	cout << "set vi(ms): " << static_cast<double>(finish - start) << endl;
}

Terrain::~Terrain()
{
	delete[] this->blocks;
	delete[] this->h_map;
}

Index2 Terrain::getChunkIndex(int w_x, int w_z) const
{
	int t = w_x / 16;
	int x = (t % size_w + size_w) % size_w;
	t = w_z / 16;
	int z = (t % size_h + size_h) % size_h;
	Index2 ans = Index2(x, z);
	return ans;
}

Index2 Terrain::findChunkIndex(int w_x, int w_z) const
{
	int t = w_x / 16;
	int x = (t % size_w + size_w) % size_w;
	t = w_z / 16;
	int z = (t % size_h + size_h) % size_h;
	Index2 ans = Index2(x, z);
	Index2 pos = this->chunks[ans.y][ans.x]->chunk_pos;
	if (pos.x != w_x || pos.y != w_z)
		ans.flag = false;
	return ans;
}

void Terrain::fillChunk(Index2 const& c_idx, Index2 const& c_pos)
{
	float x, z;
	float offset = 0.000001f;
	int16& max_h = this->chunks[c_idx.y][c_idx.x]->max_h;
	for (int i = 0; i < 16; i++) {
		z = (c_pos.y - i + offset) / 32.f;
		for (int j = 0; j < 16; j++) {
			x = (c_pos.x + j + offset) / 32.f;
			double h_ = this->perlin_noise.getNoise2D(x, z, 3, 0.5);
			int16 h = static_cast<int16>((h_ + 0.8) * 0.5 * 30.f);
			max_h = max(max_h, h);
			this->setHeight(c_idx, j, i, h);
			for (int y = 0; y < 10; y++)
				this->addBlock(c_idx, j, y, i, 1);
			for (int y = 10; y < h; y++) {
				this->addBlock(c_idx, j, y, i, 1);
			}
		}
	}
}

void Terrain::createHeightMap()
{
	for (int i = 0; i < this->size_h; i++) {
		for (int j = 0; j < this->size_w; j++) {
			Index2 c_pos = this->s_pos + Index2(j * 16, -i * 16);
			Index2 c_idx;
			c_idx = this->getChunkIndex(c_pos.x, c_pos.y);
			this->chunks[c_idx.y][c_idx.x] = make_shared<Chunk>();
			this->chunks[c_idx.y][c_idx.x]->start_pos =
				vec3(c_pos.x + 0.5f, 0.5f, c_pos.y - 0.5f);
			this->chunks[c_idx.y][c_idx.x]->chunk_pos = c_pos;
			this->fillChunk(c_idx, c_pos);
		}
	}
}

void Terrain::setRender()
{
	vector<wstring> path_arr = {
		L"grass_top.png",
		L"grass_bottom.png",
		L"grass_side.png"
	};
	this->texture_array = make_shared<TextureArray>(
		this->graphic->getDevice(),
		this->graphic->getContext(),
		path_arr,
		0
	);
	this->blend_state_arr.push_back(
		make_shared<BlendState>(this->graphic->getDevice())
	);
	this->rasterizer_state = make_shared<RasterizerState>(
		this->graphic->getDevice(),
		D3D11_FILL_SOLID,
		D3D11_CULL_BACK
	);
	this->sampler_state = make_shared<SamplerState>(this->graphic->getDevice());
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
		},
	};
	this->vertex_shader = make_shared<VertexShader>(
		this->graphic->getDevice(),
		L"TestVertexShader.hlsl",
		"main",
		"vs_5_0"
	);
	this->input_layout = make_shared<InputLayout>(
		this->graphic->getDevice(),
		layout.data(),
		layout.size(),
		this->vertex_shader->getBlob()
	);
	this->pixel_shader = make_shared<PixelShader>(
		this->graphic->getDevice(),
		L"TestPixelShader.hlsl",
		"main",
		"ps_5_0"
	);

	this->graphic->getContext()->IASetPrimitiveTopology(
		D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST
	);
	this->graphic->getContext()->IASetInputLayout(
		this->input_layout->getComPtr().Get()
	);
	
	this->graphic->getContext()->VSSetShader(
		this->vertex_shader->getComPtr().Get(),
		nullptr,
		0
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
	this->graphic->getContext()->PSSetShaderResources(
		0,
		1,
		this->texture_array->getComPtr().GetAddressOf()
	);

	this->graphic->getContext()->OMSetBlendState(
		this->blend_state_arr[0]->getComPtr().Get(),
		this->blend_state_arr[0]->getBlendFactor(),
		this->blend_state_arr[0]->getSampleMask()
	);
}

void Terrain::showChunk(Index2 const& c_idx)
{
	int cnt = 0, t;
	for (int y = 0; y < 256; y++) {
		for (int z = 0; z < 16; z++) {
			for (int x = 0; x < 16; x++) {
				t = this->findBlock(c_idx, x, y, z);
				if (t)
					cnt++;
			}
		}
	}
}

void Terrain::selectBlockTest(vec3 ray_pos, vec3 ray_dir)
{
	WorldIndex block = this->pickBlock(ray_pos, ray_dir);
	if (block.flag == true) {
		Index2& cidx = block.c_idx;
		Index3& bidx = block.b_idx;
		vector<Index2> v_idx;
		v_idx.push_back(cidx);
		this->addBlock(cidx, bidx, 2);
		this->chunks[cidx.y][cidx.x]->vertices_idx = 0;
		this->chunksSetVerticesAndIndices(v_idx, 0, 1);
	}
}

vec3 Terrain::getBlockIdxToWorld(Index2& c_idx, int x, int y, int z)
{
	vec3 pos = this->chunks[c_idx.y][c_idx.x]->start_pos;
	pos += vec3(x, y, -z);
	return pos;
}

void Terrain::Render
(
	Mat const& proj,
	Mat const& view,
	vec3 const& cam_pos
)
{
	CamPos cam;
	cam.pos = cam_pos;
	cam.r = 16.f * 1.f;
	MVP mvp;
	mvp.proj = proj.Transpose();
	mvp.view = view.Transpose();
	shared_ptr<ConstantBuffer> vertex_cbuffer;
	vertex_cbuffer = make_shared<ConstantBuffer>(
		this->graphic->getDevice(),
		this->graphic->getContext(),
		mvp
	);
	vertex_cbuffer->update(mvp);
	this->graphic->getContext()->VSSetConstantBuffers(
		0,
		1,
		vertex_cbuffer->getComPtr().GetAddressOf()
	);

	shared_ptr<ConstantBuffer> pixel_cbuffer;
	pixel_cbuffer = make_shared<ConstantBuffer>(
		this->graphic->getDevice(),
		this->graphic->getContext(),
		cam
	);
	pixel_cbuffer->update(cam);
	this->graphic->getContext()->PSSetConstantBuffers(
		0,
		1,
		pixel_cbuffer->getComPtr().GetAddressOf()
	);
	this->graphic->setClearColor(0.3, 0.3, 0.3, 1);
	this->graphic->renderBegin();
	for (int i = 0; i < this->size_h; i++) {
		for (int j = 0; j < this->size_w; j++) {
			if (this->chunks[i][j]->render_flag == false) {
				continue;
			}
			this->chunks[i][j]->setVIBuffer(
				this->graphic,
				this->vertex_shader
			);
		}
	}
	this->graphic->renderEnd();
}

bool inChunkBoundary(int x, int y, int z) {
	if (x >= 0 && x < 16 && z >= 0 && z < 16 && y >= 0 && y < 256)
		return true;
	return false;
}

void Terrain::vertexAndIndexGenerator(
	Index2 const& c_idx,
	Index2 const& adj_idx,
	Index3 const& move,
	int dir,
	vector<VertexBlockUV>& vertices,
	vector<uint32>& indices
)
{
	uint32 index = this->chunks[c_idx.y][c_idx.x]->vertices_idx;
	int16& max_h = this->chunks[c_idx.y][c_idx.x]->max_h;
	for (int y = 0; y < max_h; y++) {
		for (int z = 0; z < 16; z++) {
			for (int x = 0; x < 16; x++) {
				int type = this->findBlock(c_idx, x, y, z);
				if (type == 0)
					continue;
				Index3 next(x + move.x, y + move.y, z + move.z);
				if (inChunkBoundary(next.x, next.y, next.z) && this->findBlock(c_idx, next))
					continue;
				if (adj_idx.flag) {
					if (next.x == -1 && this->findBlock(adj_idx, 15, next.y, next.z))
						continue;
					if (next.x == 16 && this->findBlock(adj_idx, 0, next.y, next.z))
						continue;
					if (next.z == -1 && this->findBlock(adj_idx, next.x, next.y, 15))
						continue;
					if (next.z == 16 && this->findBlock(adj_idx, next.x, next.y, 0))
						continue;
				}
				Block::addBlockFacePosAndTex(
					this->chunks[c_idx.y][c_idx.x]->start_pos,
					dir, 
					x, y, z, type, 
					vertices
				);
				Block::addBlockFaceIndices(index, indices);
				index += 4;
			}
		}
	}
	this->chunks[c_idx.y][c_idx.x]->vertices_idx = index;
}

void Terrain::chunksSetVerticesAndIndices(
	vector<Index2> const& v_idx, 
	int st, 
	int ed
)
{
	static const Index3 move_arr[6] = {
		Index3(0, 1, 0),
		Index3(0, -1, 0),
		Index3(0, 0, 1),
		Index3(0, 0, -1),
		Index3(-1, 0, 0),
		Index3(1, 0, 0)
	};
	vector<VertexBlockUV> vertices;
	vector<uint32> indices;
	ed = min(ed, v_idx.size());
	for (int i = st; i < ed; i++) {
		Index2 const& c_idx = v_idx[i];
		Index2 apos = this->chunks[c_idx.y][c_idx.x]->chunk_pos;
		for (int dir = 0; dir < 6; dir++) {
			Index2 pos = apos + Index2(16 * move_arr[5 - dir].x,
				-16 * move_arr[5 - dir].z);
			Index2 adj_idx = this->findChunkIndex(pos.x, pos.y);
			this->vertexAndIndexGenerator(
				c_idx,
				adj_idx,
				move_arr[5 - dir],
				5 - dir,
				vertices,
				indices
			);
		}
		this->chunks[c_idx.y][c_idx.x]->createVIBuffer(
			this->graphic,
			vertices,
			indices
		);
		vertices.clear();
		indices.clear();
	}
}

void Terrain::terrainsetVerticesAndIndices()
{
	vector<Index2> v_idxs;
	int c_cnt = (this->size_h - 2) * (this->size_w - 2);
	int t = c_cnt / this->thread_cnt;
	int m = c_cnt % this->thread_cnt;

	for (int i = 1; i < this->size_h - 1; i++) {
		for (int j = 1; j < this->size_w - 1; j++) {
			Index2 pos = this->s_pos + Index2(16 * j, -16 * i);
			Index2 c_idx = this->findChunkIndex(pos.x, pos.y);
			v_idxs.push_back(c_idx);
		}
	}
	vector<thread> threads;
	int st = 0;
	int siz;
	for (int i = 0; i < this->thread_cnt; i++) {
		if (m) {
			siz = t + 1;
			m--;
		}
		else
			siz = t;
		threads.push_back(thread(&Terrain::chunksSetVerticesAndIndices,
			this, v_idxs, st, st + siz));
		st = st + siz;
	}
	for (int i = 0; i < this->thread_cnt; i++)
		threads[i].join();
}

void Terrain::setSightChunk(int cnt)
{
	int max_fov = min(this->size_h - 1, this->size_w - 1);
	this->c_fov = min(max_fov, cnt);
}

WorldIndex Terrain::getBlockIndexRay(
	vec3 const& pos, 
	vec3 const& r_pos,
	int dir
)
{
	Index3 b_pos;
	b_pos.x = static_cast<int>(floor(pos.x));
	b_pos.y = static_cast<int>(floor(pos.y));
	b_pos.z = static_cast<int>(floor(pos.z));
	if (dir == 0 && r_pos.x > b_pos.x)
		b_pos.x -= 1;
	else if (dir == 1 && r_pos.z < b_pos.z)
		b_pos.z += 1;
	else if (dir == 2 && r_pos.y < b_pos.y)
		b_pos.y += 1;
	return this->getBlockIndex(b_pos.x, b_pos.y, b_pos.z);
}

WorldIndex Terrain::pickBlock(vec3 r_pos, vec3 r_dir)
{
	// 1. x y z index 찾기
	WorldIndex ans;
	int x, y, z;
	vec3 tmp = r_pos;
	if (r_pos.y >= 256) {
		if (r_dir.y >= 0)
			return ans;
		tmp = intersectionRayAndPlane(
			r_pos,
			r_dir,
			vec3(r_pos.x, 256, r_pos.z),
			vec3(0, 1, 0)
		);
	}
	else if (r_pos.y < 0) {
		if (r_dir.y <= 0)
			return ans;
		tmp = intersectionRayAndPlane(
			r_pos,
			r_dir,
			vec3(r_pos.x, 0, r_pos.z),
			vec3(0, 1, 0)
		);
	}
	x = static_cast<int>(floor(tmp.x));
	y = static_cast<int>(floor(tmp.y));
	z = static_cast<int>(floor(tmp.z));
	if (z < tmp.z)
		z += 1;
	int step_x = 1;
	int step_y = 1;
	int step_z = 1;
	if (r_dir.x < 0)
		step_x = -1;
	if (r_dir.y < 0)
		step_y = -1;
	if (r_dir.z < 0)
		step_z = -1;

	// 2 max x y z, delta x y z 구하기
	float max_x, max_y, max_z;
	float delta_x = 0, delta_y = 0, delta_z = 0;
	bool flag_x = true, flag_y = true, flag_z = true;
	int sx, sy, sz;
	vec3 first, second;
	sx = x;
	if (r_dir.x > 0)
		sx += 1;
	max_x = r_dir.Dot(vec3(1, 0, 0));
	if (max_x <= STDZERO && max_x >= -STDZERO)
		flag_x = false;
	if (flag_x) {
		first = intersectionRayAndPlane(
			r_pos,
			r_dir,
			vec3(sx, 0, 0),
			vec3(1, 0, 0)
		);
		second = intersectionRayAndPlane(
			r_pos,
			r_dir,
			vec3(sx + step_x, 0, 0),
			vec3(1, 0, 0)
		);
		max_x = (r_pos - first).Length();
		delta_x = (first - second).Length();
	}
	sy = y;
	if (r_dir.y > 0)
		sy += 1;
	max_y = r_dir.Dot(vec3(0, 1, 0));
	if (max_y <= STDZERO && max_y >= -STDZERO)
		flag_y = false;
	if (flag_y) {
		first = intersectionRayAndPlane(
			r_pos,
			r_dir,
			vec3(0, sy, 0),
			vec3(0, 1, 0)
		);
		second = intersectionRayAndPlane(
			r_pos,
			r_dir,
			vec3(0, sy + step_y, 0),
			vec3(0, 1, 0)
		);
		max_y = (r_pos - first).Length();
		delta_y = (first - second).Length();
	}
	sz = z;
	if (r_dir.z < 0)
		sz -= 1;
	max_z = r_dir.Dot(vec3(0, 0, 1));
	if (max_z <= STDZERO && max_z >= -STDZERO)
		flag_z = false;
	if (flag_z) {
		first = intersectionRayAndPlane(
			r_pos,
			r_dir,
			vec3(0, 0, sz),
			vec3(0, 0, 1)
		);
		second = intersectionRayAndPlane(
			r_pos,
			r_dir,
			vec3(0, 0, sz + step_z),
			vec3(0, 0, 1)
		);
		max_z = (r_pos - first).Length();
		delta_z = (first - second).Length();
	}
	if (flag_x == false)
		max_x = INT_MAX;
	if (flag_y == false)
		max_y = INT_MAX;
	if (flag_z == false)
		max_z = INT_MAX;

	for (int i = 0; i < 8; i++) {
		if (max_x < max_y) {
			if (max_x < max_z) {
				x += step_x;
				max_x += delta_x;
			}
			else {
				z += step_z;
				max_z += delta_z;
			}
		}
		else {
			if (max_y < max_z) {
				y += step_y;
				max_y += delta_y;
			}
			else {
				z += step_z;
				max_z += delta_z;
			}
		}
		ans = this->getBlockIndex(x, y, z);
		if (ans.flag && this->findBlock(ans.c_idx, ans.b_idx)) {
			if (this->findBlock(ans.c_idx, ans.b_idx)) {
				vec3 bp = this->getBlockIdxToWorld(
					ans.c_idx,
					ans.b_idx.x,
					ans.b_idx.y,
					ans.b_idx.z
				);
				return ans;
			}
		}
	}
	ans.flag = false;
	return ans;
}

WorldIndex Terrain::getBlockIndex(float x, float y, float z) const
{
	WorldIndex ans;
	int w_x = static_cast<int>(floor(x)) / 16;
	int w_z = static_cast<int>(floor(z)) / 16;
	
	if (w_x * 16 > x)
		w_x--;
	if (w_z * 16 < z)
		w_z++;
	Index2 c_idx = this->findChunkIndex(w_x * 16, w_z * 16);
	Index2 pos = this->chunks[c_idx.y][c_idx.x]->chunk_pos;
	int ex = pos.x + 16;
	int ez = pos.y - 16;
	if (x < pos.x || x > ex || z > pos.y || z < ez || y < 0 || y > 255)
		return ans;
	ans.c_idx = c_idx;
	int ix = static_cast<int>(floor(x) - pos.x);
	int iy = static_cast<int>(floor(y));
	int iz = floor(z);
	if (iz < z)
		iz += 1;
	iz = static_cast<int>(pos.y - iz);
	ans.b_idx = Index3(ix, iy, iz);
	ans.flag = true;
	return ans;
}


int Terrain::checkTerrainBoundary(float x, float z) const
{
	float r = 16.f * this->c_fov;
	int mask = 0;
	if (x - r < this->sv_pos.x)
		mask |= 1 << 0; // left out
	if (x + r > this->ev_pos.x)
		mask |= 1 << 1; // right out
	if (z + r > this->sv_pos.y)
		mask |= 1 << 2; // back out
	if (z - r < this->ev_pos.y)
		mask |= 1 << 3; // front out
	return mask;
}

void Terrain::userPositionCheck(float x, float z)
{
	int mask = this->checkTerrainBoundary(x, z);
	vector<Index2> v_idxs, f_pos;
	Index2 cidx, cpos;
	if (mask == 1) {
		for (int i = 0; i < this->size_h; i++) {
			if (i && i < this->size_h - 1) {
				cpos = Index2(this->s_pos.x, this->s_pos.y - 16 * i);
				cidx = this->findChunkIndex(cpos.x, cpos.y);
				this->chunks[cidx.y][cidx.x]->reset();
				this->chunks[cidx.y][cidx.x]->setPos(cpos);
				this->resetChunk(cidx);
				this->fillChunk(cidx, cpos);
				v_idxs.push_back(cidx);
				cidx = this->findChunkIndex(this->ev_pos.x - 16, cpos.y);
				this->chunks[cidx.y][cidx.x]->render_flag = false;
			}
			cpos = this->s_pos + Index2(-16, -16 * i);
			f_pos.push_back(cpos);
		}
		this->sv_pos.x -= 16;
		this->s_pos.x -= 16;
		this->ev_pos.x -= 16;
	}
	else if (mask == 2) {
		this->test_flag = true;
		for (int i = 0; i < this->size_h; i++) {
			if (i && i < this->size_h - 1) {
				cpos = Index2(this->ev_pos.x, this->s_pos.y - 16 * i);
				cidx = this->findChunkIndex(cpos.x, cpos.y);
				this->chunks[cidx.y][cidx.x]->reset();
				this->chunks[cidx.y][cidx.x]->setPos(cpos);
				this->resetChunk(cidx);
				this->fillChunk(cidx, cpos);
				v_idxs.push_back(cidx);
				cidx = this->findChunkIndex(this->sv_pos.x, cpos.y);
				this->chunks[cidx.y][cidx.x]->render_flag = false;
			}
			cpos.x = this->ev_pos.x + 16;
			cpos.y = this->s_pos.y - 16 * i;
			f_pos.push_back(cpos);			
		}
		this->sv_pos.x += 16;
		this->s_pos.x += 16;
		this->ev_pos.x += 16;
	}
	else if (mask == 4) {
		for (int i = 0; i < this->size_w; i++) {
			if (i && i < this->size_w - 1) {
				cpos = Index2(this->s_pos.x + 16 * i, this->s_pos.y);
				cidx = this->findChunkIndex(cpos.x, cpos.y);
				this->chunks[cidx.y][cidx.x]->reset();
				this->chunks[cidx.y][cidx.x]->setPos(cpos);
				this->resetChunk(cidx);
				this->fillChunk(cidx, cpos);
				v_idxs.push_back(cidx);
				cidx = this->findChunkIndex(cpos.x, this->ev_pos.y + 16);
				this->chunks[cidx.y][cidx.x]->render_flag = false;
			}
			cpos = this->s_pos + Index2(16 * i, 16);
			f_pos.push_back(cpos);
		}
		this->sv_pos.y += 16;
		this->s_pos.y += 16;
		this->ev_pos.y += 16;
	}
	else if (mask == 8) {
		for (int i = 0; i < this->size_w; i++) {
			if (i && i < this->size_w - 1) {
				cpos = Index2(this->s_pos.x + 16 * i, this->ev_pos.y);
				cidx = this->findChunkIndex(cpos.x, cpos.y);
				this->chunks[cidx.y][cidx.x]->reset();
				this->chunks[cidx.y][cidx.x]->setPos(cpos);
				this->resetChunk(cidx);
				this->fillChunk(cidx, cpos);
				v_idxs.push_back(cidx);
				cidx = this->getChunkIndex(cpos.x, this->sv_pos.y);
				this->chunks[cidx.y][cidx.x]->render_flag = false;
			}
			cpos.x = this->s_pos.x + 16 * i;
			cpos.y = this->ev_pos.y - 16;
			f_pos.push_back(cpos);
		}
		this->sv_pos.y -= 16;
		this->s_pos.y -= 16;
		this->ev_pos.y -= 16;
	}
	if (f_pos.size()) {
		for (Index2& pos : f_pos) {
			cidx = this->getChunkIndex(pos.x, pos.y);
			this->chunks[cidx.y][cidx.x]->reset();
			this->chunks[cidx.y][cidx.x]->setPos(pos);
			this->resetChunk(cidx);
			this->fillChunk(cidx, pos);
		}
	}
	if (v_idxs.size()) {
		vector<thread> threads;
		int t = v_idxs.size() / this->thread_cnt;
		int m = v_idxs.size() % this->thread_cnt;
		int st = 0;
		int siz;
		for (int i = 0; i < this->thread_cnt; i++) {
			if (m) {
				siz = t + 1;
				m--;
			}
			else
				siz = t;
			threads.push_back(thread(&Terrain::chunksSetVerticesAndIndices,
				this, v_idxs, st, st + siz));
			st = st + siz;
		}
		for (int i = 0; i < this->thread_cnt; i++)
			threads[i].join();
	}
}

int16 Terrain::getHeight(float x, float z) const
{
	WorldIndex w_idx = this->getBlockIndex(x, 0, z);
	return this->findHeight(w_idx.c_idx, w_idx.b_idx.x, w_idx.b_idx.z);
}


/*
대각선 이동 후 대각선의 기저 벡터의 
*/

// helper

void Terrain::resetChunk(Index2 const& c_idx)
{
	for (int y = 0; y < 256; y++) {
		for (int z = 0; z < 16; z++) {
			for (int x = 0; x < 16; x++)
				this->addBlock(c_idx, x, y, z, 0);
		}
	}
}


int Terrain::findBlock(Index2 const& c_idx, int x, int y, int z) const
{
	int idx = 16 * 16 * 256 * (c_idx.x + this->size_w * c_idx.y) +
		x + 16 * (z + 16 * y);
	return this->blocks[idx];
}

int Terrain::findBlock(Index2 const& c_idx, Index3 const& b_idx) const
{
	int idx = 16 * 16 * 256 * (c_idx.x + this->size_w * c_idx.y) +
		b_idx.x + 16 * (b_idx.z + 16 * b_idx.y);
	return this->blocks[idx];
}

void Terrain::addBlock(Index2 const& c_idx, int x, int y, int z, int type)
{
	int idx = 16 * 16 * 256 * (c_idx.x + this->size_w * c_idx.y) + x +
		16 * (z + 16 * y);
	this->blocks[idx] = type;
}

void Terrain::addBlock(Index2 const& c_idx, Index3 const& b_idx, int type)
{
	int idx = 16 * 16 * 256 * (c_idx.x + this->size_w * c_idx.y) +
		b_idx.x + 16 * (b_idx.z + 16 * b_idx.y);
	this->blocks[idx] = type;
}

void Terrain::setHeight(Index2 const& c_idx, int x, int z, int h)
{
	int idx = 16 * 16 * (c_idx.x + this->size_w * c_idx.y) + x + 16 * z;
	this->h_map[idx] = h;
}

void Terrain::setHeight(Index2 const& c_idx, Index2 const& b_idx, int h)
{
	int idx = 16 * 16 * (c_idx.x + this->size_w * c_idx.y) + 
		b_idx.x + 16 * b_idx.y;
	this->h_map[idx] = h;
}

int Terrain::findHeight(Index2 const& c_idx, Index2 const& h_idx) const
{
	int idx = 16 * 16 * (c_idx.x + this->size_w * c_idx.y) + 
		h_idx.x + 16 * h_idx.y;
	return this->h_map[idx];
}

int Terrain::findHeight(Index2 const& c_idx, int x, int z) const
{
	int idx = 16 * 16 * (c_idx.x + this->size_w * c_idx.y) + x + 16 * z;
	return this->h_map[idx];
}
