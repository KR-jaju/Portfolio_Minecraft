struct MVP
{
    matrix world;
    matrix view;
    matrix proj;
};

Texture2DArray shadow_arr : register(t0);
StructuredBuffer<MVP> mvp_arr : register(t1);

SamplerState shadow_point_sampler
{
    Filter = MIN_MAG_MIP_POINT;
    AddressU = BORDER;
    AddressV = BORDER;
    AddressW = BORDER;
    BoarderColor = float4(100.f, 1.0f, 1.0f, 1.0f);
};

struct PS_INPUT
{
    float4 pos : SV_Position;
    float4 w_pos : POSITION;
    float3 normal : NORMAL;
    float eye_z : VIEW_SPACE_Z;
    int shadow_flag : SHADOW;
};

// 최대 8개 까지 나눔
cbuffer Split : register(b0)
{
    float4 light_pos; // world space
    float z_arr[8]; // view space
};

float shadowCheck(float4 w_pos, int shadow_idx, float3 normal, float p_dis)
{
    float ans = 1.0f;
    float3 light_dir = light_pos.xyz;
    light_dir.z = w_pos.z;
    light_dir = normalize(w_pos.xyz - light_dir);
    w_pos += float4(normal, 0) * 0.1; // self shadow 방지
    w_pos = mul(w_pos, mvp_arr[shadow_idx].view);
    w_pos = mul(w_pos, mvp_arr[shadow_idx].proj);
    w_pos /= w_pos.w; // ndc
    float3 uvw = float3((w_pos.x + 1.0) * 0.5, (-w_pos.y + 1.0) * 0.5,
        shadow_idx); // ndc uv
    
    float z = shadow_arr.Sample(shadow_point_sampler, uvw).r;
    
    float bias = max(0.05 * (1.0 - dot(normal, light_dir)), 0.005);
    bias *= 1.0 / (p_dis * 0.5f);
    if (z + bias < w_pos.z)
        ans = 0.0f;
    return ans;
};

float4 main(PS_INPUT input) : SV_TARGET
{
    float res = -1;
    for (int i = 0; i < 8; i++)
    {
        if (input.eye_z < z_arr[i])
        {
            res = shadowCheck(input.w_pos, i, input.normal, z_arr[i]);
            return float4(res, res, res, 1);
        }
    }
    res = 1;
    return float4(res, res, res, 1.0f);
}