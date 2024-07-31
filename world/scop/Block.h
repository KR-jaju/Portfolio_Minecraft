#pragma once

enum class BlockFace {
	Top,
	Bottom,
	Front,
	Back,
	Left,
	Right
};

class Texture;

class Block
{
public:
	Block(
		ComPtr<ID3D11Device> device,
		vector<wstring> const& path_arr
	);
	~Block();
	vector<vec3> getBlockFacePos(
		float x,
		float y,
		float z,
		BlockFace BlockFace_flag
	);
	vector<vec2> getBlockFaceTexcoord(
		vec2 start,
		vec2 end,
		BlockFace BlockFace_flag
	);
	void registerSRV(ComPtr<ID3D11DeviceContext> context);
private:
	vector<shared_ptr<Texture>> texture;
};

