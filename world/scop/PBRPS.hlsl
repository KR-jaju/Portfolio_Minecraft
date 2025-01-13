Texture2D color_tex : register(t0);
Texture2D normal_tex : register(t1);
Texture2D world_pos_tex : register(t2);
Texture2D rma_tex : register(t3);
TextureCube irradiance_tex : register(t4);
TextureCube specular_tex : register(t5);
Texture2D brdf_tex : register(t6);

SamplerState linear_sampler : register(s0);
SamplerState clamp_sampler : register(s1);

cbuffer c_buffer : register(b0)
{
    float3 eye_pos;
    float3 light_pos;
}


struct PS_INPUT
{
    float4 pos : SV_Position;
    float2 uv : TEXCOORD;
};

struct PS_OUTPUT
{
    float4 ambient_light : SV_Target0;
    float4 direct_light : SV_Target1;
};

static const float3 Fdielectric = float3(0.04, 0.04, 0.04);

float3 schilckFresnel(float3 F0, float NdotH)
{
    return F0 + (1.0 - F0) *
        pow(2.0, (-5.55473 * NdotH - 6.98316) * NdotH);
}

float3 diffuseIBL(
    float3 albedo,
    float3 normal_w,
    float3 pixel_to_eye,
    float metallic
)
{
    float3 F0 = lerp(Fdielectric, albedo, metallic);
    float3 F = schilckFresnel(F0, max(0.0,
        dot(normal_w, pixel_to_eye)));
    float3 kd = lerp(float3(1, 1, 1) - F, float3(0, 0, 0), metallic);
    
    float3 irradiance = irradiance_tex.Sample(linear_sampler,
        normal_w).rgb;
    return kd * albedo * irradiance;
}

float3 specularIBL(
    float3 albedo,
    float3 normal_w,
    float3 pixel_to_eye,
    float mettalic,
    float roughness
)
{
    float2 specularBRDF = brdf_tex.Sample(clamp_sampler,
        float2(dot(normal_w, pixel_to_eye), 1.0 - roughness)).rg;
    
    float3 specular_irr = specular_tex.SampleLevel(linear_sampler,
        reflect(-pixel_to_eye, normal_w), roughness * 10.f).rgb;
    float3 F0 = lerp(Fdielectric, albedo, mettalic);
    
    return (F0 * specularBRDF.x + specularBRDF.y) *
        specular_irr;
}

float3 ambientLighting(
    float3 albedo,
    float3 normal_w,
    float3 pixel_to_eye,
    float ao,
    float mettalic,
    float roughness
)
{
    float3 diffuse = diffuseIBL(albedo, normal_w, pixel_to_eye,
        mettalic);
    float3 specular = specularIBL(albedo, normal_w, pixel_to_eye,
        mettalic, roughness);

    return (diffuse + specular) * ao;
}

// GGX/Towbridge-Reitz normal distribution function.
// Uses Disney's reparametrization of alpha = roughness^2.
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

PS_OUTPUT main(PS_INPUT input)
{
    PS_OUTPUT output;
    
    float3 pos = world_pos_tex.Sample(linear_sampler, input.uv).xyz;
    float3 normal = normal_tex.Sample(linear_sampler, input.uv).xyz;
    float3 albedo = color_tex.Sample(linear_sampler, input.uv).rgb;
    if (normal.x == 0 && normal.y == 0 && normal.z == 0)
    {
        output.ambient_light = float4(albedo, 1);
        output.direct_light = float4(0, 0, 0, 0);
        return output;
    }
    normal = normalize(normal);
    float3 pixel_to_eye = normalize(eye_pos.xyz - pos);
    float ao = rma_tex.Sample(linear_sampler, input.uv).b;
    float metallic = rma_tex.Sample(linear_sampler, input.uv).g;
    float roughness = rma_tex.Sample(linear_sampler, input.uv).r;
    
    float3 ambient_light = ambientLighting(albedo, normal,
        pixel_to_eye, ao, metallic, roughness);
    
    float3 direct_light = float3(0, 0, 0);
    float3 l_pos = light_pos.xyz;
    l_pos.z = pos.z; // directional light
    
    float3 light_vec = normalize(l_pos - pos);
    float3 halfway = normalize(pixel_to_eye + light_vec);
    
    float NdotI = max(0.0, dot(normal, light_vec));
    float NdotH = max(0.0, dot(normal, halfway));
    float NdotO = max(0.0, dot(normal, pixel_to_eye));
    
    float3 F0 = lerp(Fdielectric, albedo, metallic);
    float3 F = schilckFresnel(F0,
        max(0.0, dot(halfway, pixel_to_eye)));
    
    float3 kd = lerp(float3(1, 1, 1) - F, float3(0, 0, 0), 
        metallic);
    float3 diffuse_brdf = kd * albedo;
    
    float D = NdfGGX(NdotH, roughness);
    float3 G = SchlickGGX(NdotI, NdotO, roughness);
    float3 specualr_brdf = (F * D * G) / max(1e-5, 4.0 * NdotI * NdotO);
    float3 radiance = 1.0f;
    direct_light = (diffuse_brdf + specualr_brdf) * radiance * NdotI;
    output.ambient_light = float4(ambient_light, 1);
    output.direct_light = float4(direct_light, 1);
    
    return output;
}