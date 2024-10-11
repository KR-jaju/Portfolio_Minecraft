#pragma once

class DeferredBuffer
{
public:
	DeferredBuffer(int rtv_num);
	~DeferredBuffer();
	void setRTVsAndSRVs(
		ComPtr<ID3D11Device> device,
		int width,
		int height
	);
	ComPtr<ID3D11RenderTargetView> getRTV(int idx);
	ComPtr<ID3D11ShaderResourceView> getSRV(int idx);
	int getCnt();
private:


private:
	int rtv_num;
	vector<ComPtr<ID3D11RenderTargetView>> RTVs;
	vector<ComPtr<ID3D11ShaderResourceView>> SRVs;
};

