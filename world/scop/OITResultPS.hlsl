Texture2D result_color : register(t0);

SamplerState sampler0 : register(s0);

struct PS_INPUT
{
    float4 pos : SV_Position;
    float2 uv : TEXCOORD;
};

float4 main(PS_INPUT input) : SV_TARGET
{
    return result_color.Sample(sampler0, input.uv);
}