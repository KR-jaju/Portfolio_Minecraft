struct MVP
{
    matrix world;
    matrix view;
    matrix proj;
};
Texture2DArray shadow_arr : register(t0);
StructuredBuffer<MVP> mvp_arr : register(t1);

Texture2D pos_tex : register(t2); // world space
Texture2D normal_tex : register(t3); // world space

SamplerState shadow_point_sampler
{
    Filter = MIN_MAG_MIP_POINT;
    AddressU = BORDER;
    AddressV = BORDER;
    AddressW = BORDER;
    BoarderColor = float4(100.f, 1.0f, 1.0f, 1.0f);
};

SamplerState sampler0
{
    Filter = MIN_MAG_MIP_LINEAR;
    AddressU = WRAP;
    AddressV = WRAP;
    AddressW = WRAP;
};

struct PS_INPUT
{
    float4 pos : SV_Position;
    float2 uv : TEXCOORD;
};

// 최대 8개 까지 나눔
cbuffer Split : register(b0)
{
    float4 light_pos; // world space
    float z_arr[8]; // view space
    matrix view;
};

float shadowCheck(float4 w_pos, int shadow_idx, float3 normal, float p_dis)
{
    float ans = 1;
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
        ans = 0;
    return ans;
};

float4 main(PS_INPUT input) : SV_TARGET
{
    float res = -1;
    float4 p_eye = mul(input.pos, view);
    float4 w_pos = pos_tex.Sample(sampler0, input.uv);
    float3 normal = normal_tex.Sample(sampler0, input.uv);
    for (int i = 0; i < 8; i++)
    {
        if (p_eye.z < z_arr[i])
        {
            if (i < 7 && p_eye.z > z_arr[i] * 0.8 && p_eye.z < z_arr[i] * 1.2)
            {
                float r1 = 
                    shadowCheck(w_pos, i, normal, z_arr[i]);
                float r2 =
                    shadowCheck(w_pos, i + 1, normal, z_arr[i + 1]);
                if (r1 == 0 || r2 == 0)
                    return (0, 0, 0, 1);
                else
                    return (1, 1, 1, 1);
            }
            else
            {
                res = shadowCheck(w_pos, i, normal, z_arr[i]);
                return float4(res, res, res, 1);
            }
        }
    }
    return float4(1, 1, 1, 1.0f);
}