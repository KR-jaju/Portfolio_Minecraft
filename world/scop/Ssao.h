#pragma once
class Ssao
{
public:
	Ssao(ComPtr<ID3D11Device> device);

private:
	void buildOffset();
	void buildRandomVecTexture(ComPtr<ID3D11Device> device);

public:
	vec4 mOffsets[14];
	ComPtr<ID3D11ShaderResourceView> random_vec_SRV;
};

