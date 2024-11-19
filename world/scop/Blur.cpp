#include "pch.h"
#include "Blur.h"
#include "DeferredGraphics.h"
#include "Filter.h"


Blur::Blur(
	DeferredGraphics* graphic, 
	UINT width, 
	UINT height,
	ComPtr<ID3D11ShaderResourceView> start_srv
)
{
	this->d_graphic = graphic;
	this->width = width;
	this->height = height;
	this->start_srv = start_srv.Get();
	ComPtr<ID3D11Device> device = graphic->getDevice();
	//this->makeFilter();
	this->makeFilterMultiLevel();
}

ComPtr<ID3D11ShaderResourceView> Blur::getSRV()
{
	return final_filter->getSRV();
}

void Blur::render()
{
	for (int i = 0; i < this->down_filter.size(); i++)
		this->down_filter[i]->render();
	for (int i = 0; i < this->up_blur_x_filter.size(); i++) {
		for (int j = 0; j < 2; j++) {
			this->up_blur_x_filter[i]->render();
			this->up_blur_y_filter[i]->render();
		}
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
	this->down_filter.back()->setStartSRV(this->start_srv);
	this->up_blur_x_filter.push_back(
		make_shared<Filter>(
			this->d_graphic,
			w, h,
			L"SamplingVS.hlsl",
			L"BlurXPS.hlsl"
		)
	);
	this->up_blur_x_filter.back()->setStartSRV(
		this->down_filter.back()->getSRV());
	this->up_blur_y_filter.push_back(
		make_shared<Filter>(
			this->d_graphic,
			w, h,
			L"SamplingVS.hlsl",
			L"BlurYPS.hlsl"
		)
	);
	this->up_blur_y_filter.back()->setStartSRV(
		this->up_blur_x_filter.back()->getSRV());
	this->final_filter = make_shared<Filter>(
		this->d_graphic,
		this->width,
		this->height,
		L"SamplingVS.hlsl",
		L"SamplingPS.hlsl"
	);
	this->final_filter->setStartSRV(
		this->up_blur_y_filter.back()->getSRV());
}

void Blur::makeFilterMultiLevel()
{
	int target = 32;
	for (int i = 2; i <= target; i *= 2) {
		auto last = make_shared<Filter>(
			this->d_graphic,
			this->width / i,
			this->height / i,
			L"SamplingVS.hlsl",
			L"SamplingPS.hlsl"
		);
		if (i == 2)
			last->setStartSRV(this->start_srv);
		else
			last->setStartSRV(this->down_filter.back()->getSRV());
		this->down_filter.push_back(last);
	}
	for (int i = target; i >= 1; i /= 2) {
		this->up_blur_x_filter.push_back(
			make_shared<Filter>(
				this->d_graphic,
				this->width / i,
				this->height / i,
				L"SamplingVS.hlsl",
				L"BlurXPS.hlsl"
			)
		);
		if (i == target)
			this->up_blur_x_filter.back()->setStartSRV(
				this->down_filter.back()->getSRV());
		else {
			this->up_blur_x_filter.back()->setStartSRV(
				this->tmp_up_filter.back()->getSRV()
			);
		}
		this->up_blur_y_filter.push_back(
			make_shared<Filter>(
				this->d_graphic,
				this->width / i,
				this->height / i,
				L"SamplingVS.hlsl",
				L"BlurYPS.hlsl"
			)
		);
		this->up_blur_y_filter.back()->setStartSRV(
			this->up_blur_x_filter.back()->getSRV()
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
			this->tmp_up_filter.back()->setStartSRV(
				this->up_blur_y_filter.back()->getSRV()
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
	this->final_filter->setStartSRV(
		this->up_blur_y_filter.back()->getSRV());
}
