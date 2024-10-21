Texture2D normal_map : register(t0);
Texture2D depth_map : register(t1);
Texture2D ssao_map : register(t2);

cbuffer cbPerFrame : register(b0)
{
    float gTexelWidth; // 1.0f / ssao_width 
    float gTexelHeight; // 1.0f / ssao_height
    int wh_flag;
};

cbuffer cbSettings : register(b1)
{
    float gWeights[11] = {0.05f, 0.05f, 0.1f, 0.1f, 0.1f, 0.2f, 
        0.1f, 0.1f, 0.1f, 0.05f, 0.05f};
};

cbuffer cbFixed
{
    static const int gBlurRadius = 5;
};

SamplerState sample_normal_depth
{
    Filter = MIN_MAG_LINEAR_MIP_POINT;

    AddressU = CLAMP;
    AddressV = CLAMP;
};

SamplerState sample_image
{
    Filter = MIN_MAG_LINEAR_MIP_POINT;

    AddressU = CLAMP;
    AddressV = CLAMP;
};

struct PS_INPUT
{
    float4 pos : SV_Position;
    float2 uv : TEXCOORD;
};

float4 main(PS_INPUT input) : SV_TARGET
{
    float2 texOffset;
    if (wh_flag == 0)
    {
        texOffset = float2(gTexelHeight, 0.0f);
    }
    else
    {
        texOffset = float2(0.0f, gTexelWidth);
    }
    
    float4 color = gWeights[5] * ssao_map.Sample(sample_image,
        input.uv);
    float total_weight = gWeights[5];
    float3 center_normal = normal_map.Sample(sample_normal_depth,
        input.uv).xyz;
    float center_depth = depth_map.Sample(sample_normal_depth,
        input.uv).r;
    
    for (int i = -gBlurRadius; i < gBlurRadius; i++)
    {
        if (i == 0)
            continue;
        float2 tex = input.uv + i * texOffset;
        float3 neighbor_normal =
            normal_map.Sample(sample_normal_depth, tex).xyz;
        float neighbor_depth =
            depth_map.Sample(sample_normal_depth, tex).r;
        if (dot(neighbor_normal, center_normal) >= 0.8 &&
            abs(neighbor_depth - center_depth) <= 0.2f)
        {
            float weight = gWeights[i + gBlurRadius];
            color += weight * ssao_map.Sample(
                sample_image, tex);
            total_weight += weight;
        }
    }
    return color / total_weight;
}