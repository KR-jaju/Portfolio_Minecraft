Texture2D color_map : register(t0);
Texture2D normal_map : register(t1);
Texture2D position_map : register(t2);
Texture2D shadow_map : register(t3);
Texture2D depth_map : register(t4);
Texture2D random_map : register(t5);

SamplerState sampler0 : register(s0);

struct PS_INPUT
{
    float4 pos : SV_Position;
    float2 uv : TEXCOORD;
};

cbuffer Index : register(b0) 
{
    int idx;
    float3 cam_pos;
    float2 screen;
    float2 padding2;
};

static float g_sample_rad = 1.0f;
static float g_scale = 0.1f;
static float g_bias = 0.06f;
static float g_intensity = 1.0f;

float3 getPosition(float2 uv)
{
    return position_map.Sample(sampler0, uv).xyz;
}

float3 getNormal(float2 uv)
{
    return normal_map.Sample(sampler0, uv).xyz;
}

float doAmbientOcclusion(float2 tcoord, float2 uv, float3 p, float3 n)
{
    float3 diff = getPosition(tcoord + uv) - p;
    if (diff.z < 0)
        return 0;
    const float3 v = normalize(diff);
    const float d = length(diff) * g_scale;
    return max(0.0, dot(n, v)) * (1.0 / (1.0 + d)) * g_intensity;
}

float2 getRandom(float2 uv)
{
    return normalize(random_map.Sample(sampler0, 
        screen * uv / 64 * 64).xy * 2.f - 1.f);
}

float4 main(PS_INPUT input) : SV_TARGET
{
    float4 color = color_map.Sample(sampler0, input.uv);
    float sp = shadow_map.Sample(sampler0, input.uv).r;
    sp /= 15.f;
    sp = max(sp, 0.3);
    
    const float2 vec[4] = { 
        float2(1, 0), float2(-1, 0), 
        float2(0, 1), float2(0, -1) };
    float3 p = getPosition(input.uv);
    float3 n = getNormal(input.uv);
    float2 rand = getRandom(input.uv);
    float ao = 0;
    if (g_sample_rad == 0)
        return color;
    float d = length(p - cam_pos);
    float rad = g_sample_rad / d;
    int total = 4;
    for (int i = 0; i < total; i++)
    {
        float2 coord1 = reflect(vec[i], rand) * rad;
        float2 coord2 = float2(coord1.x * 0.707 - coord1.y * 0.707, 
            coord1.x * 0.707 + coord1.y * 0.707);
        if (coord2.x == 0 && coord2.y == 0)
            return float4(0, 1, 0, 1);
        ao += doAmbientOcclusion(input.uv, coord1 * 0.25, p, n);
        ao += doAmbientOcclusion(input.uv, coord2 * 0.5, p, n);
        ao += doAmbientOcclusion(input.uv, coord1 * 0.75, p, n);
        ao += doAmbientOcclusion(input.uv, coord2, p, n);
    }
    ao /= total * 4;
    ao = saturate(ao);
    sp *= 1 - ao;
    sp = pow(sp, 2);
    return color * float4(sp, sp, sp, 1);
}
