SamplerState sampler0 : register(s0);
Texture2D albedo_texture : register(t0);
Texture2D normal_texture : register(t1);
Texture2D depth_texture : register(t2);

cbuffer CameraMatrices : register(b0)
{
    matrix view;
    matrix projection;
    matrix view_projection;
    matrix view_inverse_transpose;
    matrix projection_inverse;
};

struct PSInput {
    float4 position : SV_POSITION;
    float2 uv : TEXCOORD0;
};


static const float3 Fdielectric = float3(0.04, 0.04, 0.04);

float3 schilckFresnel(float3 F0, float NdotH)
{
    return F0 + (1.0 - F0) *
        pow(2.0, (-5.55473 * NdotH - 6.98316) * NdotH);
}

float NdfGGX(float NdotH, float roughness)
{
    float alpha = roughness * roughness;
    float alphasq = alpha * alpha;
    float denom = (NdotH * NdotH) * (alphasq - 1.0) + 1.0;
    return alphasq / (3.141592 * denom * denom);
}

float shlickG1(float NdotV, float k)
{
    return NdotV / (NdotV * (1.0 - k) + k);
}

float SchlickGGX(float NdotI, float NdotO, float roughness)
{
    float r = roughness + 1.0;
    float k = (r * r) / 8.0;
    return shlickG1(NdotI, k) * shlickG1(NdotO, k);
}

float3  reconstructViewSpacePosition(float2 uv, float depth)
{
    float4 cs_position = float4(2.0f * uv - 1, depth * 2.0 - 1.0, 1.0);
    float4 vs_position = mul(cs_position, projection_inverse);

    return (vs_position.xyz / vs_position.w);
}

float4 main(PSInput input) : SV_TARGET
{
    float4 albedo = albedo_texture.Sample(sampler0, input.uv);
    float2 normal = normal_texture.Sample(sampler0, input.uv).xy;
    float  depth = depth_texture.Sample(sampler0, input.uv);
    float3 vs_position = reconstructViewSpacePosition(input.uv, depth);

    float metallic = 0.0;
    float roughness = 0.5;

    float3 ws_L = float3(0.0, 1.0, 0.0);

    float3 V = normalize(-vs_position.xyz); // view dir
    float3 N = float3(normal, -sqrt(1 - dot(normal, normal))); // 뷰 스페이스 노말 복원
    float3 L = mul(ws_L, (float3x3)view);
    float3 H = normalize(V + L);

    float NdotL = max(0.0, dot(N, L));
    float NdotH = max(0.0, dot(N, H));
    float NdotV = max(0.0, dot(N, V));

    float3 F0 = lerp(Fdielectric, albedo.xyz, metallic);
    float3 F = schilckFresnel(F0, max(0.0, dot(H, V)));

    float3 kd = lerp(float3(1, 1, 1) - F0, float3(0, 0, 0), metallic);
    float3 diffuse_brdf = kd * albedo;

    float D = NdfGGX(NdotH, roughness);
    float3 G = SchlickGGX(NdotL, NdotV, roughness);
    float3 specular_brdf = (F * D * G) / max(1e-5, 4.0 * NdotL * NdotV);
    float3 radiance = 1.0f;
    float3 direct_lighting = (diffuse_brdf + specular_brdf) * radiance * NdotL;

    return float4(direct_lighting, 1.0);
}