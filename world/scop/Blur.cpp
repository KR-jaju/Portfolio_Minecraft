#include "pch.h"
#include "Blur.h"
#include "DeferredGraphics.h"
#include "Filter.h"


Blur::Blur(
	DeferredGraphics* graphic, 
	UINT width, 
	UINT height
)
{
	this->d_graphic = graphic;
	this->width = width;
	this->height = height;
	ComPtr<ID3D11Device> device = graphic->getDevice();
	this->makeFilter();
	//this->makeFilterMultiLevel();
}

void Blur::setStartSRV(ComPtr<ID3D11ShaderResourceView> start_srv)
{
	this->start_srv = start_srv;
}

ComPtr<ID3D11ShaderResourceView> Blur::getSRV()
{
	return final_filter->getSRV();
}

void Blur::render()
{
	this->makeFilterSetSRV();
	//this->makeFilterSetMultiLevelSRV();
	for (int i = 0; i < this->down_filter.size(); i++)
		this->down_filter[i]->render();
	for (int i = 0; i < this->up_blur_x_filter.size(); i++) {
		this->up_blur_x_filter[i]->render();
		this->up_blur_y_filter[i]->render();
		if (i)
			this->tmp_up_filter[i - 1]->render();
	}
	this->final_filter->render();
}

void Blur::makeFilter()
{
	UINT w = this->width / 16;
	UINT h = this->height / 16;
	this->down_filter.push_back(
		make_shared<Filter>(
			this->d_graphic,
			w, h,
			L"SamplingVS.hlsl",
			L"SamplingPS.hlsl"
		)
	);
	this->up_blur_x_filter.push_back(
		make_shared<Filter>(
			this->d_graphic,
			w, h,
			L"SamplingVS.hlsl",
			L"BlurXPS.hlsl"
		)
	);
	this->up_blur_y_filter.push_back(
		make_shared<Filter>(
			this->d_graphic,
			w, h,
			L"SamplingVS.hlsl",
			L"BlurYPS.hlsl"
		)
	);
	this->final_filter = make_shared<Filter>(
		this->d_graphic,
		this->width,
		this->height,
		L"SamplingVS.hlsl",
		L"SamplingPS.hlsl"
	);
}

void Blur::makeFilterSetSRV()
{
	this->down_filter.back()->setStartSRV(this->start_srv);
	this->up_blur_x_filter.back()->setStartSRV(
		this->down_filter.back()->getSRV());
	this->up_blur_y_filter.back()->setStartSRV(
		this->up_blur_x_filter.back()->getSRV());
	this->final_filter->setStartSRV(
		this->up_blur_y_filter.back()->getSRV());
}

void Blur::makeFilterMultiLevel()
{
	this->multi_level_target = 16;
	for (int i = 2; i <= this->multi_level_target; i *= 2) {
		auto last = make_shared<Filter>(
			this->d_graphic,
			this->width / i,
			this->height / i,
			L"SamplingVS.hlsl",
			L"SamplingPS.hlsl"
		);
		this->down_filter.push_back(last);
	}
	for (int i = this->multi_level_target; i >= 1; i /= 2) {
		this->up_blur_x_filter.push_back(
			make_shared<Filter>(
				this->d_graphic,
				this->width / i,
				this->height / i,
				L"SamplingVS.hlsl",
				L"BlurXPS.hlsl"
			)
		);
		this->up_blur_y_filter.push_back(
			make_shared<Filter>(
				this->d_graphic,
				this->width / i,
				this->height / i,
				L"SamplingVS.hlsl",
				L"BlurYPS.hlsl"
			)
		);
		if (i > 1) {
			this->tmp_up_filter.push_back(
				make_shared<Filter>(
					this->d_graphic,
					this->width / i * 2,
					this->height / i * 2,
					L"SamplingVS.hlsl",
					L"SamplingPS.hlsl"
				)
			);
		}
	}
	this->final_filter = make_shared<Filter>(
		this->d_graphic,
		this->width,
		this->height,
		L"SamplingVS.hlsl",
		L"SamplingPS.hlsl"
	);
}

void Blur::makeFilterSetMultiLevelSRV()
{
	int idx = 0;
	for (int i = 2; i <= this->multi_level_target; i *= 2) {
		if (i == 2)
			this->down_filter[idx]->setStartSRV(this->start_srv);
		else
			this->down_filter[idx]->
				setStartSRV(this->down_filter[idx - 1]->getSRV());
		idx++;
	}

	idx = 0;
	for (int i = this->multi_level_target; i >= 1; i /= 2) {
		if (i == this->multi_level_target)
			this->up_blur_x_filter[idx]->setStartSRV(
				this->down_filter.back()->getSRV());
		else {
			this->up_blur_x_filter[idx]->setStartSRV(
				this->tmp_up_filter[idx - 1]->getSRV()
			);
		}
		this->up_blur_y_filter[idx]->setStartSRV(
			this->up_blur_x_filter[idx]->getSRV()
		);
		if (i > 1) {
			this->tmp_up_filter[idx]->setStartSRV(
				this->up_blur_y_filter[idx]->getSRV()
			);
		}
		idx++;
	}
	this->final_filter->setStartSRV(
		this->up_blur_y_filter.back()->getSRV());
}
