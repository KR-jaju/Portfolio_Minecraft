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
    int2   dimension;
};

cbuffer Light : register(b1)
{
    matrix light_view_projection[4];
    float3 light_direction;
    float3 light_color; // color(rgb) + bias
};

struct PSInput {
    float4 position : SV_POSITION;
    float2 uv : TEXCOORD0;
    float2 scaled_uv : TEXCOORD1;
};

static const float3 Fdielectric = float3(0.04, 0.04, 0.04);

float3 schlickFresnel(float3 F0, float VdotH)
{
    return F0 + (1.0 - F0) * pow(2.0, (-5.55473 * VdotH - 6.98316) * VdotH);
}

float NdfGGX(float NdotH, float roughness)
{
    float alpha = roughness * roughness;
    float alpha_squared = alpha * alpha;
    float factor = (NdotH * NdotH) * (alpha_squared - 1.0) + 1.0;

    return alpha_squared / (3.141592 * factor * factor);
}

float shlickG1(float NdotV, float k)
{
    return NdotV / (NdotV * (1.0 - k) + k);
}

float SchlickGGX(float NdotI, float NdotV, float roughness)
{
    float r = roughness + 1.0;
    float k = (r * r) / 8.0;
    return shlickG1(NdotI, k) * shlickG1(NdotV, k);
}

float3  reconstructViewSpacePosition(float2 uv, float depth)
{
    float4 cs_position = float4((2.0f * uv - 1) * float2(1.0, -1.0), depth, 1.0);
    float4 vs_position = mul(cs_position, projection_inverse);

    return (vs_position.xyz / vs_position.w);
}

float3 decodeNormal(float2 encoded)
{
    float2 f_enc = encoded * 4 - 2;
    float f = dot(f_enc, f_enc);
    float g = sqrt(1 - f / 4);
    float3 n = float3(f_enc * g, f / 2 - 1);

    return (n);
}

/*
(xy / (2 * sqrt(-2z + 2)) + 0.5) * 4 - 2

fenc = 2xy / sqrt(-2z+2)
4x^2 / (-2z+2)
f = 2x^2/ (-z + 1)
g = (-z + 1 - x^2/2)/(-z+1)
fenc * g = 

*/

float4 main(PSInput input) : SV_TARGET
{
    float4 albedo = albedo_texture.Load(int3(input.scaled_uv, 0));
    float2 normal = normal_texture.Load(int3(input.scaled_uv, 0)).xy;
    float  depth = depth_texture.Load(int3(input.scaled_uv, 0));
    float3 vs_position = reconstructViewSpacePosition(input.uv, depth);

    float metallic = 0.0;
    float roughness = 0.5;

    float3 ws_L = normalize(float3(0.0, 1.0, 0.0));

    float3 V = normalize(-vs_position.xyz); // view dir
    float3 N = decodeNormal(normal);
    float3 L = mul(ws_L, (float3x3)view).xyz;
    float3 H = normalize(V + L);

    float NdotL = max(0.0, dot(N, L));
    float NdotH = max(0.0, dot(N, H));
    float NdotV = max(0.0, dot(N, V));

    float3 F0 = lerp(Fdielectric, albedo.xyz, metallic);
    float F = schlickFresnel(F0, max(0.0, dot(V, H)));

    float3 kd = lerp(float3(1, 1, 1) - F, float3(0, 0, 0), metallic);
    float3 diffuse_brdf = kd * albedo.rgb / 3.141592;

    float D = NdfGGX(NdotH, roughness);
    float G = SchlickGGX(NdotL, NdotV, roughness);
    float3 specular_brdf = (F * D * G) / max(1e-5, 4.0 * NdotL * NdotV);
    float3 radiance = 1.0f;
    float3 direct_lighting = (diffuse_brdf + specular_brdf) * radiance * NdotL;

    return float4(direct_lighting, 1.0);
}