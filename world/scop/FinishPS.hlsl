Texture2D result_tex : register(t0);

SamplerState sampler_0
{
    Filter = MIN_MAG_MIP_LINEAR;
    AddressU = WRAP;
    AddressV = WRAP;
};

struct PS_INPUT
{
    float4 pos : SV_Position;
    float2 uv : TEXCOORD;
};

float4 main(PS_INPUT input) : SV_TARGET
{
    float4 color = result_tex.Sample(sampler_0, input.uv);
    return color;
}