struct PS_INPUT
{
    float4 pos : SV_Position;
    float2 uv : TEXCOORD;
};

Texture2D color_map : register(t0);
Texture2D shadow_map : register(t1);
Texture2D ssao_map : register(t2);

SamplerState sampler0 : register(s0);

float2 ndcToTextureUV(float2 ndc)
{
    float2 uv;
    uv.x = (ndc.x + 1.0f) * 0.5f;
    uv.y = (-ndc.y + 1.0f) * 0.5f;
    return uv;
}

float4 main(PS_INPUT input) : SV_TARGET
{
    float4 color = color_map.Sample(sampler0, input.uv);
    float sp = shadow_map.Sample(sampler0, input.uv).r;
    sp /= 15.f;
    sp = max(sp, 0.1);
    float4 ssao = ssao_map.Sample(sampler0, input.uv);
    float4 res = float4(sp, sp, sp, 1) * ssao;
    return color * res;
}