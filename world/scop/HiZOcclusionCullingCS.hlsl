RWStructuredBuffer<uint> visibility : register(u0);
Texture2D depth_texture : register(t0);
SamplerState depth_sampler : register(s0);

cbuffer CameraMatrices : register(b0)
{
    matrix view;
    matrix projection;
    matrix view_projection;
    matrix view_inverse;
    matrix projection_inverse;
    int2   dimension;
};

cbuffer Occludees : register(b2)
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
    float diameter = distance(P0_uv, P1_uv); // UV 스페이스에서 지름
    float radius = 0.5 * diameter;
    /*
    True Positive -> 그려야할 물체가 제대로 그려짐
    True Negative -> 그려지지 말아야할 물체가 제대로 제거됨
    False Positive -> 그려지지 말아야할 물체가 그려짐
    False Negative -> 그려져야할 물체가 그려지지 않음

    길이 < i레벨 픽셀 하나의 크기
    - 이 조건을 만족하는 제일 작은 i를 찾는 것이 목표

    i레벨 픽셀 하나 크기 = 1.0 / floor(dimension * pow(0.5, level))
    이미지 크기는 항상 정수이므로 floor를 사용한다.

    레벨을 예측할 때는 floor를 쓰지 않는데, floor는 수학 계산이 어렵기 때문이다(불가능).
    계산식에서는 floor를 쓸 때에 비해 분모가 항상 크거나 같아지므로 예측은 항상 실제에 비해 보수적으로 하게 된다.(더 높은 레벨을 선택하려고 함)
    하지만 보수적 예측은 최악의 경우 False-Positive만 일으키므로 그래픽 디펙트는 없다.

    길이 <= 1.0 / dimension * 0.5 ^ level
    -> 길이 * dimension <= 2 ^ level
    -> log2(길이 * dimension) <= level
    ceil(log2(길이 * dimension)) <= level // 예측된 레벨, 보수적 선택(ceil)
    */
    int mip_level = ceil(log2(diameter * max(dimension.x, dimension.y)));
    float2 center_uv = (P0_uv + P1_uv) * 0.5;

    if (occludee_depth <= depth_texture.SampleLevel(depth_sampler, center_uv + dot(diameter.xx, float2(0.5, 0.5)), mip_level).x)
        return (1);
    if (occludee_depth <= depth_texture.SampleLevel(depth_sampler, center_uv + dot(diameter.xx, float2(-0.5, 0.5)), mip_level).x)
        return (1);
    if (occludee_depth <= depth_texture.SampleLevel(depth_sampler, center_uv + dot(diameter.xx, float2(0.5, -0.5)), mip_level).x)
        return (1);
    if (occludee_depth <= depth_texture.SampleLevel(depth_sampler, center_uv + dot(diameter.xx, float2(-0.5, -0.5)), mip_level).x)
        return (1);
    return (0); // Hi-Z Occlusion Culling Test Fail
}

[numthreads(64, 1, 1)]
void main(uint3 dispatchThreadID : SV_DispatchThreadID) {
    uint idx = dispatchThreadID.x;

    if (idx >= occludee_count)
        return;
    float4 occludee = occludees[idx];
    visibility[idx] = HiZOcclusionTest(occludee);
}
