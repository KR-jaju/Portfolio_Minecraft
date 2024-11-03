struct PS_INPUT
{
    float4 pos : SV_Position;
    float2 uv : TEXCOORD;
};

Texture2D color_map : register(t0);
Texture2D shadow_map : register(t1);
Texture2D ssao_map : register(t2);
Texture2D cube_map : register(t3);

SamplerState sampler0 : register(s0);


float4 main(PS_INPUT input) : SV_TARGET
{
    float4 color = color_map.Sample(sampler0, input.uv);
    if (color.r == 0 && color.g == 0 && color.b == 0)
        return cube_map.Sample(sampler0, input.uv);
    float sp = shadow_map.Sample(sampler0, input.uv).r;
    sp /= 15.f;
    sp = max(sp, 0.1);
    float4 ssao = ssao_map.Sample(sampler0, input.uv);
    float4 res = float4(sp, sp, sp, 1) * ssao;
    return color * res;
}