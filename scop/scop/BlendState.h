#pragma once
// 어느 때 언제 어떻게 사용하는 지 잘 모르겠음
// 강의에 나온 기본으로 사용하다가 필요한게 있을 시 추가 할 것

// 혼합공식 C = Csrc☒Fsrc⊕Cdst☒Fdst
class BlendState
{
public:
	BlendState(
		ComPtr<ID3D11Device> device,
		D3D11_RENDER_TARGET_BLEND_DESC const& blend_desc =
		{
			true,
			D3D11_BLEND_SRC_ALPHA,
			D3D11_BLEND_INV_SRC_ALPHA,
			D3D11_BLEND_OP_ADD,
			D3D11_BLEND_ONE, // f = (1, 1, 1)
			D3D11_BLEND_ZERO, // f = (0,0,0)
			D3D11_BLEND_OP_ADD, // operator
			D3D11_COLOR_WRITE_ENABLE_ALL
		}, 
		float const& factor = 0.f
	);
	BlendState(BlendState const&) = delete;
	~BlendState();
	BlendState& operator=(BlendState const&) = delete;
	float const* getBlendFactor() { return &(this->blend_factor); }
	uint32 getSampleMask() { return this->sample_mask;  }
	ComPtr<ID3D11BlendState> getComPtr() const;
private:
	ComPtr<ID3D11Device> device;
	ComPtr<ID3D11BlendState> blend_state;
	float blend_factor = 0.f;
	uint32 sample_mask = 0xFFFFFFFF;
};

