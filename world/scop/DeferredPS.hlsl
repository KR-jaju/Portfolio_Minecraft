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
    matrix proj;
};

cbuffer Offset : register(b1)
{
    float4 offsets[14];
};

static float g_surface_epsilon = 0.01f;
static float g_occlusion_fade_end = 2.0f;
static float g_occlusion_fade_start = 0.2f;
static float g_occlusion_r = 0.1f;

float occlusionFunction(float dist_z)
{
    float occlusion = 0.0f;
    if (dist_z > g_surface_epsilon && dist_z < g_occlusion_fade_end)
    {
        float fade_length = g_occlusion_fade_end - g_occlusion_fade_start;
        occlusion = saturate((g_occlusion_fade_end - dist_z) / fade_length);
    }
    return occlusion;
}

float ndcDepthToViewDepth(float z_ndc)
{
    float view_z = proj[3][2] / (z_ndc - proj[2][2]);
    return view_z;
}

float2 ndcToTextureUV(float2 ndc)
{
    float2 uv;
    uv.x = (ndc.x + 1.0f) / 2.0f;
    uv.y = (-ndc.y + 1.0f) / 2.0f;
    return uv;
}

float4 main(PS_INPUT input) : SV_TARGET
{
    float4 color = color_map.Sample(sampler0, input.uv);
    float sp = shadow_map.Sample(sampler0, input.uv).r;
    sp /= 15.f;
    sp = max(sp, 0.3);
    
    float3 n = normal_map.Sample(sampler0, input.uv).xyz;
    float3 p = position_map.Sample(sampler0, input.uv).xyz;
    float3 rand_vec = random_map.Sample(sampler0, input.uv).xyz;
    rand_vec = 2.0f * rand_vec - 1.0f;
    rand_vec = normalize(rand_vec);
    float total_occlusion = 0.0f;
    for (int i = 0; i < 14; i++)
    {
        float3 offset = reflect(offsets[i].xyz, rand_vec);
        float flip = sign(dot(offset, n));
        float3 q = p + flip * g_occlusion_r * offset;
        float4 ndc_q = mul(float4(q, 1), proj);
        ndc_q /= ndc_q.w;
        float2 uv = ndcToTextureUV(ndc_q.xy);
        float rz = depth_map.Sample(sampler0, uv).r;
        rz = ndcDepthToViewDepth(rz);
        float3 r = (rz / q.z) * q;
        
        float dist_z = p.z - r.z;
        float dp = max(dot(n, normalize(r - p)), 0.0f);
        float occlusion = dp * occlusionFunction(dist_z);
        total_occlusion += occlusion;
    }
    total_occlusion /= 14;
    float access = 1.0f - total_occlusion;
    access = saturate(pow(access, 4.0f));
    return access;
    sp *= access;
    return color * float4(sp, sp, sp, 1);
}
