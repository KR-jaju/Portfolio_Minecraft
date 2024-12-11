#pragma once

class DeferredGraphics;
class Filter;

class Blur
{
public:
	Blur(
		DeferredGraphics* graphic, 
		UINT width, 
		UINT height
	);
	void setStartSRV(ComPtr<ID3D11ShaderResourceView> start_srv);
	ComPtr<ID3D11ShaderResourceView> getSRV();
	void render();

private:
	void makeFilter();
	void makeFilterSetSRV();
	void makeFilterMultiLevel();
	void makeFilterSetMultiLevelSRV();
private:
	vector<shared_ptr<Filter>> down_filter;
	vector<shared_ptr<Filter>> up_blur_x_filter;
	vector<shared_ptr<Filter>> tmp_up_filter;
	vector<shared_ptr<Filter>> up_blur_y_filter;
	shared_ptr<Filter> final_filter;
private:
	DeferredGraphics* d_graphic = nullptr;
	UINT width;
	UINT height;
	int multi_level_target;
private:
	ComPtr<ID3D11ShaderResourceView> start_srv;
};

