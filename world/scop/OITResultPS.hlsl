Texture2D result_color : register(t0);

SamplerState sampler0 : register(s0);

struct PS_INPUT
{
    float4 pos : SV_Position;
    float2 uv : TEXCOORD;
};

float4 main(PS_INPUT input) : SV_TARGET
{
    //float3 color = result_color.Sample(sampler0, input.uv).rgb;
    //return float4(color, 1);
    return result_color.Sample(sampler0, input.uv);
}