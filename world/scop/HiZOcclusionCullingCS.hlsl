RWStructuredBuffer<uint> visibility : register(u0);
Texture2D depth_texture : register(t0);

cbuffer CameraMatrices : register(b0)
{
    matrix view;
    matrix projection;
    matrix view_projection;
    matrix view_inverse;
    matrix projection_inverse;
    int2   dimension;
};


cbuffer DepthTextureInfo : register(b1)
{
    int max_mip_level;
};

cbuffer Settings : register(b2)
{
    int occludee_count;
    float4 occludees[1024];
};


uint HiZOcclusionTest(float4 occludee)
{
    float3 v = mul(float4(occludee.xyz, 1.0), view);
    float r = occludee.w;
    float mag = length(v);
    float3 nv = v / mag;
    float3 nv_squared = nv * nv;
    float nr = occludee.w / mag;
    float a = 1 - nr * nr;
    if (a <= 0)
        return (1); // 오클루디의 바운딩 스피어가 카메라를 포함함
    float A = r * sqrt(a) * sqrt((nv_squared.x + nv_squared.y * nv_squared.z) / (nv_squared.x + nv_squared.z));
    float B = v.z * a;
    float z0 = -A + B;
    float c0 = nv.z * z0 - mag * a;
    float x0 = -nv.x * c0 / (nv_squared.x + nv_squared.y);
    float y0 = (nv.y == 0.0) ? 0.0 : -(nv.x * x0 + c0) / nv.y;
    float z1 = A + B;
    float c1 = nv.z * z1 - mag * a;
    float x1 = -nv.x * c1 / (nv_squared.x + nv_squared.y);
    float y1 = (nv.y == 0.0) ? 0.0 : -(nv.x * x1 + c1) / nv.y;
    if (z0 < 0 || z1 < 1) // z가 음수가 되면 perspective division 때문에 왜곡됨
        return (1);
    float4 P0 = mul(float4(x0, y0, z0, 1.0), projection);
    float4 P1 = mul(float4(x1, y1, z1, 1.0), projection);
    float4 P2 = mul(float4(v - float3(0, 0, r), 1.0), projection);
    
    float2 P0_uv = (P0.xy / P0.w * float2(0.5, -0.5) + float2(0.5, 0.5));
    float2 P1_uv = (P1.xy / P1.w * float2(0.5, -0.5) + float2(0.5, 0.5)); // y뒤집어야함
    float occludee_depth = (P2.z / P2.w); // NDC depth
    float diameter = distance(P0_uv, P1_uv); // UV 스페이스에서 지름 (1:1 비율이라서 이미지 크기만 곱하면 한번만 구해도 됨)
    int mip_level = max_mip_level - clamp(floor(-log2(diameter)), 0, max_mip_level); // diameter가 1 ~ 0.5 + delta 이면 최대레벨, 0.5 ~ 0.25 + delta면 최대레벨 - 1레벨;
    int mip_size = pow(2, max_mip_level - mip_level);
    float2 center_scaled = (P0_uv + P1_uv) * 0.5 * mip_size;
    
    int2 sample_point_base = int2(floor(center_scaled - float2(0.5, 0.5)));

    int2 sample_point0 = clamp(sample_point_base, (0).xx, (mip_size - 1).xx);
    int2 sample_point1 = clamp(sample_point_base + int2(1, 0), (0).xx, (mip_size - 1).xx);
    int2 sample_point2 = clamp(sample_point_base + int2(0, 1), (0).xx, (mip_size - 1).xx);
    int2 sample_point3 = clamp(sample_point_base + int2(1, 1), (0).xx, (mip_size - 1).xx);

    if (occludee_depth <= depth_texture.Load(int3(sample_point0, mip_level)).x) // 보여야함
        return (1);
    if (occludee_depth <= depth_texture.Load(int3(sample_point1, mip_level)).x)
        return (1);
    if (occludee_depth <= depth_texture.Load(int3(sample_point2, mip_level)).x)
        return (1);
    if (occludee_depth <= depth_texture.Load(int3(sample_point3, mip_level)).x)
        return (1);
    return (0);
}

[numthreads(64, 1, 1)]
void main(uint3 dispatchThreadID : SV_DispatchThreadID) {
    uint idx = dispatchThreadID.x;

    if (idx >= occludee_count)
        return;
    float4 occludee = occludees[idx];
    visibility[idx] = HiZOcclusionTest(occludee);
}
