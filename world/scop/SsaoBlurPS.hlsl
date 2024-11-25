Texture2D normal_map : register(t0); // world_space
Texture2D depth_map : register(t1); // ndc space
Texture2D ssao_map : register(t2);

cbuffer cbPerFrame : register(b0)
{
    float gTexelWidth; // 입력 texture의  1.0f / 너비
    float gTexelHeight; // 입력 texture의 1.0f / 높이
    float gWidth; // 화면의 1.0f / 너비
    float gHeight; // 화면의 1.0f / 높이
    int wh_flag; // 0 가로, 1이 세로
    float3 dummy;
    matrix proj; // 투영행렬
    matrix view;
};

cbuffer cbSettings : register(b1)
{
    static float gWeights[11] = {0.05f, 0.05f, 0.1f, 0.1f, 0.1f, 0.2f, 
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

float NdcDepthToViewDepth(float z_ndc)
{
    float z_view = proj[3][2] / (z_ndc - proj[2][2]);
    return z_view;
}

struct PS_INPUT
{
    float4 pos : SV_Position;
    float2 uv : TEXCOORD;
};

float4 main(PS_INPUT input) : SV_TARGET
{
    float2 texOffset;
    float2 offset;
    if (wh_flag == 0)
    {
        texOffset = float2(gTexelWidth, 0.0f);
        offset = float2(gWidth, 0.0f);
    }
    else
    {
        texOffset = float2(0.0f, gTexelHeight);
        offset = float2(0.0f, gHeight);
    }
    float4 color = gWeights[5] * ssao_map.Sample(sample_image,
        input.uv);
    float total_weight = gWeights[5];
    float3 center_normal = normal_map.Sample(sample_normal_depth,
        input.uv).xyz;
    center_normal = mul(center_normal, (float3x3) view);
    float center_depth = depth_map.Sample(sample_normal_depth,
        input.uv).r;
    center_depth = NdcDepthToViewDepth(center_depth);
    
    if (gTexelHeight == 0 || gTexelWidth == 0)
        return float4(1, 0, 0, 1);
    for (int i = -gBlurRadius; i <= gBlurRadius; i++)
    {
        if (i == 0)
            continue;
        float2 tex = input.uv + i * texOffset;
        float2 nd_tex = input.uv + i * offset;
        float3 neighbor_normal =
            normal_map.Sample(sample_normal_depth, nd_tex).xyz;
        neighbor_normal = mul(neighbor_normal, (float3x3) view);
        float neighbor_depth =
            depth_map.Sample(sample_normal_depth, nd_tex).r;
        neighbor_depth = NdcDepthToViewDepth(neighbor_depth);
        if (dot(neighbor_normal, center_normal) >= 0.8 &&
            abs(neighbor_depth - center_depth) <= 0.2f)
        {
            float weight = gWeights[i + gBlurRadius];
            color += weight * ssao_map.Sample(
                sample_image, tex);
            total_weight += weight;
        }
    }
    color /= total_weight;
    return color;
}