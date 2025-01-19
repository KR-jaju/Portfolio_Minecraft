SamplerState sampler0 : register(s0);
Texture2D albedo_texture : register(t0);

struct PSInput {
    float4 position : SV_POSITION;
    float2 uv : TEXCOORD0;
};

float4 main(PSInput input) : SV_TARGET
{
	return albedo_texture.Sample(sampler0, input.uv);
}