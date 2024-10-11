Texture2D color_map : register(t0);
Texture2D normal_map : register(t1);
Texture2D position_map : register(t2);
Texture2D shadow_map : register(t3);

SamplerState sampler0 : register(s0);

struct PS_INPUT
{
    float4 pos : SV_Position;
    float2 uv : TEXCOORD;
};

cbuffer Index : register(b0) 
{
    int idx;
    float3 dummy;
};

float4 main(PS_INPUT input) : SV_TARGET
{
    // test
    float4 color = color_map.Sample(sampler0, input.uv);
    float sp = shadow_map.Sample(sampler0, input.uv).r;
    sp /= 15.f;
    sp = max(sp, 0.3);
    return color * float4(sp, sp, sp, 1);
}
