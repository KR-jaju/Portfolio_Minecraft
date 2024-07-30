#include "pch.h"
#include "Chunk.h"
#include "Block.h"

Chunk::Chunk()
{
}

Chunk::~Chunk()
{
}

void Chunk::setVertexAndIndex()
{
	VertexUV vertex;
	uint32 index = 0;
	for (int z = 0; z < 16; z++) {
		for (int y = 0; y < 256; y++) {
			for (int x = 0; x < 16; x++) {
				vector<bool> check_box = this->checkBlock(x, y, z);
				for (int i = 0; i < check_box.size(); i++) {
					
				}
			}
		}
	}
}

int Chunk::getBlock(int x, int y, int z) const
{
	return this->chunk[x][y][z];
}

void Chunk::setBlock(int x, int y, int z, int type)
{
	this->chunk[x][y][z] = type;
}

void Chunk::putChunk(float x, float y, float z)
{
	this->model = XMMatrixTranslation(x, y, z);
}

void Chunk::renderTest()
{
}

void Chunk::initRender(HWND hwnd, UINT width, UINT height)
{
	this->graphic = make_shared<Graphics>(hwnd, width, height);
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
			block_check_arr.push_back(false);
		}
		else if (nx < 0 || nx >= 16) {
			int block_flag = 0;
			if (nx < 0 && this->left.lock().use_count())
				block_flag = this->left.lock()->getBlock(15, ny, nz);
			else if (this->right.lock().use_count())
				block_flag = this->right.lock()->getBlock(0, ny, nz);
			if (block_flag == 0)
				block_check_arr.push_back(false);
			else
				block_check_arr.push_back(true);
		}
		else {
			int block_flag = 0;
			if (nz < 0 && this->front.lock().use_count())
				block_flag = front.lock()->getBlock(nx, ny, 15);
			else if (this->back.lock().use_count())
				block_flag = back.lock()->getBlock(nx, ny, 0);
			if (block_flag == 0)
				block_check_arr.push_back(false);
			else
				block_check_arr.push_back(true);
		}
	}
	return block_check_arr;
}
