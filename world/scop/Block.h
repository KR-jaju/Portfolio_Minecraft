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
class TextureArray;

class Block
{
public:
	Block(int type);
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
	void registerSRV(
		ComPtr<ID3D11Device> device,
		ComPtr<ID3D11DeviceContext> context,
		vector<wstring> const& path_arr
	);
	vector<uint32> getBlockFaceIndices(uint32 start) const;

private:
	int type;
	vector<shared_ptr<Texture>> texture;
	vector<ID3D11ShaderResourceView*> view_arr;
	shared_ptr<TextureArray> texture_arr;
	
	//tmp
	vector<wstring> path_arr;
};

