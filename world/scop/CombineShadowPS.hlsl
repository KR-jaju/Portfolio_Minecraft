struct MVP
{
    matrix world;
    matrix view;
    matrix proj;
};
StructuredBuffer<MVP> mvp_arr : register(t0);
Texture2D shadow_0 : register(t1);
Texture2D shadow_1 : register(t2);
Texture2D shadow_2 : register(t3);
Texture2D shadow_3 : register(t4);
Texture2D shadow_4 : register(t5);
Texture2D shadow_5 : register(t6);
Texture2D shadow_6 : register(t7);
Texture2D shadow_7 : register(t8);

Texture2D pos_tex : register(t9); // world space
Texture2D normal_tex : register(t10); // world space

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

matrix rotateY(float degree)
{
    float radian = radians(degree);
    float c = cos(radian);
    float s = sin(radian);
    matrix rotY =
    {
        c, 0, s, 0,
        0, 1, 0, 0,
        -s, 0, c, 0,
        0, 0, 0, 1
    };
    return rotY;
}

float shadowCheck(float4 w_pos, int shadow_idx, float3 normal, float p_dis)
{
    float ans = 1;
    float3 light_dir = light_pos.xyz;
    light_dir.z = w_pos.z;
    light_dir = normalize(w_pos.xyz - light_dir);
    //light_dir = mul(float4(light_dir, 0), rotateY(45));
    
    w_pos += float4(normal, 0) * 0.1; // self shadow 방지
    w_pos = mul(w_pos, mvp_arr[shadow_idx].view);
    w_pos = mul(w_pos, mvp_arr[shadow_idx].proj);
    w_pos /= w_pos.w; // ndc
    float3 uvw = float3((w_pos.x + 1.0) * 0.5, (-w_pos.y + 1.0) * 0.5,
        shadow_idx); // ndc uv
    
    float z; // linear
    //z = shadow_arr.Sample(shadow_point_sampler, uvw).r;
    if (shadow_idx == 0)
        z = shadow_0.Sample(shadow_point_sampler, uvw.xy).r;
    else if (shadow_idx == 1)
        z = shadow_1.Sample(shadow_point_sampler, uvw.xy).r;
    else if (shadow_idx == 2)
        z = shadow_2.Sample(shadow_point_sampler, uvw.xy).r;
    else if (shadow_idx == 3)
        z = shadow_3.Sample(shadow_point_sampler, uvw.xy).r;
    else if (shadow_idx == 4)
        z = shadow_4.Sample(shadow_point_sampler, uvw.xy).r;
    else if (shadow_idx == 5)
        z = shadow_5.Sample(shadow_point_sampler, uvw.xy).r;
    else if (shadow_idx == 6)
        z = shadow_6.Sample(shadow_point_sampler, uvw.xy).r;
    else
        z = shadow_7.Sample(shadow_point_sampler, uvw.xy).r;
    
    float bias = max(0.0001 * (1.0 + dot(normal, light_dir)), 0.0);
    //float bias = max(0.05 * (1.0 + dot(normal, light_dir)), 0.005);
    bias *= 1.0 / (p_dis * 0.5f);
    bias = 0.0;
    if (z < w_pos.z) // bias 가 0 이여도 shadow acne 가 안일어남
        ans = 0;
    return ans;
};

float4 main(PS_INPUT input) : SV_TARGET
{
    float res = -1;
    float4 w_pos = pos_tex.Sample(sampler0, input.uv);
    float4 p_eye = mul(w_pos, view);
    float3 normal = normal_tex.Sample(sampler0, input.uv);
    if (normal.x == 0 && normal.y == 0 && normal.z == 0)
        discard;
    float4 c_arr[5] = { float4(1, 0, 0, 1), float4(0, 1, 0, 1),
            float4(0, 0, 1, 1), float4(1, 1, 0, 1),
            float4(1, 0, 1, 1)};
    
    [unroll]
    for (int i = 0; i < light_pos.w; i++)
    {
        if (p_eye.z < z_arr[i])
        {
            if (p_eye.z > z_arr[i] * 0.8 && p_eye.z < z_arr[i])
            {
                float r1 = 
                    shadowCheck(w_pos, i, normal, z_arr[i]);
                float r2 =
                    shadowCheck(w_pos, i + 1, normal, z_arr[i + 1]);
                
                //return c_arr[i];
                if (r1 == 0 || r2 == 0)
                    return float4(0, 0, 0, 1);
                else
                    return float4(1, 1, 1, 1);
            }
            else
            {
                res = shadowCheck(w_pos, i, normal, z_arr[i]);
                //if (i < 4)
                    //return float4(0, 0, 1, 1);
                //return float4(res, res, res, 1) * c_arr[i];
                return float4(res, res, res, 1);
            }
        }
    }
    return float4(1, 1, 1, 1);
}